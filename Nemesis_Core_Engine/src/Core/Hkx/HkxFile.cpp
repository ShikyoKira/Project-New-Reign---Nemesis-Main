#include "Core/Hkx/HkxFile.h"

#include <chrono>

#include "Core/CoreObject.h"

#include "Utilities/Algorithm.h"
#include "Utilities/Crc32.h"
#include "Utilities/LimitedConcurrency.h"
#include "Utilities/OnScopeEnds.h"
#include "Utilities/Sha256.h"

#include "Logger.h"
#include "NemesisInfo.h"

#include "Serialize/PackfileSerializer.h"
#include "Serialize/XmlDeserializer.h"

#include "Havok/hkPackfile.h"

DeqNstr nemesis::HkxFile::CompileAllTemplates(nemesis::CompileState& state) const
{
    DeqNstr template_lines;
    std::scoped_lock<std::mutex> lock(TemplateMutex);

    for (auto& templt_obj : TemplateMap)
    {
        auto& requests = state.GetRequests(templt_obj.first);

        if (requests.empty()) continue;

        size_t index = templt_obj.second->GetIndex();

        if (index == 0)
        {
            state.SetBaseRequest(nullptr);
            templt_obj.second->CompileTo(template_lines, state);
            continue;
        }

        for (auto& request : requests)
        {
            state.SetBaseRequest(request);
            templt_obj.second->CompileTo(template_lines, state);
        }
    }

    state.SetBaseRequest(nullptr);

    if (template_lines.empty()) return template_lines;

    return template_lines;
}

DeqNstr nemesis::HkxFile::CompileAllSubTemplates(nemesis::CompileState& state) const
{
    DeqNstr lines;
    auto subrequests = state.GetSubTemplateRequestList();

    if (subrequests.empty()) return lines;

    state.SetBaseRequest(nullptr);
    Vec<UPtr<nemesis::CompileState>> state_list;
    USetStr checker;

    for (auto& request : subrequests)
    {
        auto new_state = state.Clone();
        new_state->SetCurrentSubTemplateRequest(*request);
        auto template_name   = request->GetArgument(1);
        auto sub_templt_list = state.GetManager().GetTemplateRepository().GetSubTemplateList();
        const nemesis::SubTemplateObject* st_obj = nullptr;

        for (auto& each : sub_templt_list)
        {
            if (!nemesis::iequals(each->GetName(), template_name)) continue;

            st_obj = each;
            break;
        }

        if (!st_obj)
        {
            throw nemesis::NObjectException("SubTemplate cannot be found (\"" + template_name + "\")");
        }

        checker.insert(request->GetArgument(0));
        st_obj->CompileTo(lines, *new_state);
        lines.emplace_back("");

        if (new_state->GetSubTemplateRequestList().empty()) continue;

        state_list.emplace_back(std::move(new_state));
    }

    for (size_t i = 0; i < state_list.size(); ++i)
    {
        auto cur_state = state_list[i].get();

        for (auto& request : cur_state->GetSubTemplateRequestList())
        {
            auto& id = request->GetArgument(0);

            if (checker.find(id) != checker.end()) continue;

            auto new_state = state.Clone();
            new_state->SetCurrentSubTemplateRequest(*request);

            auto template_name   = request->GetArgument(1);
            auto sub_templt_list = state.GetManager().GetTemplateRepository().GetSubTemplateList();
            const nemesis::SubTemplateObject* st_obj = nullptr;

            for (auto& each : sub_templt_list)
            {
                if (!nemesis::iequals(each->GetName(), template_name)) continue;

                st_obj = each;
                break;
            }

            if (!st_obj)
            {
                throw nemesis::NObjectException("SubTemplate cannot be found (\"" + template_name + "\")");
            }

            checker.insert(id);
            st_obj->CompileTo(lines, *new_state);
            lines.emplace_back("");

            if (new_state->GetSubTemplateRequestList().empty()) continue;

            state_list.emplace_back(std::move(new_state));
        }
    }

    return lines;
}

std::future<void> nemesis::HkxFile::CompileToHkx(const std::string& hash,
                                                 const std::filesystem::path& hkx_path,
                                                 const std::string& contents,
                                                 nemesis::CompileState& state,
                                                 nemesis::PlatformType platform,
                                                 nemesis::HavokVersion version,
                                                 bool include_xml,
                                                 const UMap<size_t, const nemesis::Line*>& modded_lines,
                                                 std::function<void()> callback) const
{
    std::filesystem::create_directories(hkx_path.parent_path());
    std::filesystem::remove(hkx_path);

    return std::async(
        [hash, hkx_path, contents, platform, version, include_xml, &state, modded_lines, callback]()
        {
            std::filesystem::path xml_path = hkx_path;
            xml_path.replace_extension(".xml");

            if (include_xml)
            {
                std::ofstream file(xml_path);

                if (!file.is_open())
                {
                    std::error_code ec(errno, std::system_category());
                    throw nemesis::NObjectException("Failed to open file: \"" + to_utf8_string(xml_path)
                                                    + "\"\nMessage: " + ec.message());
                }

                file << contents;
                file.close();
            }

            try
            {
                nemesis::XmlDeserializer des;
                des.LoadXml(contents);
                auto packfile = des.Deserialize();

                nemesis::PackfileSerializer ser(platform, version);
                ser.Serialize(packfile);
                ser.Save(hkx_path);

                std::string data = ser.RawData();
                nemesis::CacheManager::AddEntry(hash, data);
            }
            catch (const std::exception& ex)
            {
                std::regex rgx(R"(([,\s\(]Line\:)\s([0-9]+))");
                std::smatch match;
                std::string msg(ex.what());

                if (std::regex_search(msg, match, rgx))
                {
                    auto itr = modded_lines.find(std::stoul(match[2]));

                    if (itr != modded_lines.end())
                    {
                        msg = std::regex_replace(msg,
                                                 rgx,
                                                 "$1 " + std::to_string(itr->second->GetLineNumber())
                                                     + ", File: "
                                                     + nemesis::to_utf8_string(itr->second->GetFilePath()));
                    }
                }

                throw nemesis::NObjectException(
                    msg + "\nFailed to output hkx file (File: " + nemesis::to_utf8_string(hkx_path) + ")");
            }

            if (std::filesystem::exists(hkx_path))
            {
                if (!nemesis::iequals(PATH_TO_STRING(hkx_path.filename()), LITERAL_PATH("build_info.hkx")))
                {
                    static nemesis::CRC32 crc32;
                    size_t checksum = crc32.FullCRC(contents);
                    state.AddCheckSum(hkx_path, std::to_string(checksum));
                }

                callback();
                return;
            }

            throw nemesis::NObjectException(
                "Failed to output hkx file (File: " + nemesis::to_utf8_string(hkx_path) + ")");
        });
}

std::future<void> nemesis::HkxFile::CopyFromCache(const nemesis::CacheEntry& entry,
                                                  const std::filesystem::path& output_path) const
{
    return std::async(
        [&entry, output_path]()
        {
            std::ofstream out(output_path, std::ios::binary);

            if (!out)
            {
                std::error_code ec(errno, std::system_category());
                throw nemesis::NObjectException("Failed to open file: \"" + to_utf8_string(output_path)
                                                + "\"\nMessage: " + ec.message());
            }

            out.write(entry.Data.data(), entry.Data.size());
        });
}

std::filesystem::path nemesis::HkxFile::CompileFile(nemesis::CompileState& state,
                                                    nemesis::PlatformType platform,
                                                    nemesis::HavokVersion version,
                                                    bool include_xml) const
{
    std::filesystem::path target_path = NemesisInfo::PatchOutputPath(TargetPath);
    CompileFileAsHkx(target_path, state, platform, version, include_xml);
    return target_path;
}

void nemesis::HkxFile::CompileFileAsXml(const std::filesystem::path& filepath,
                                        nemesis::CompileState& state) const
{
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + PATH_TO_STRING(filepath));

    DeqNstr lines = Compile(state);
    std::ostringstream stream;

    for (auto& line : lines)
    {
        stream << line + "\n";
    }

    std::ofstream file(filepath);

    if (!file.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw nemesis::NObjectException("Failed to open file: \"" + to_utf8_string(filepath)
                                        + "\"\nMessage: " + ec.message());
    }

    file << stream.str();
    file.close();

    Logger::Log(LITERAL_PATH("Compiled Target File: ") + PATH_TO_STRING(filepath));
}

void nemesis::HkxFile::CompileFileAsHkx(const std::filesystem::path& filepath,
                                        nemesis::CompileState& state,
                                        nemesis::PlatformType platform,
                                        nemesis::HavokVersion version,
                                        bool include_xml) const
{
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + PATH_TO_STRING(filepath));

    std::string hash = GetHash(state) + static_cast<char>(version);
    auto* entry      = nemesis::CacheManager::GetEntry(hash);

    if (entry)
    {
        CopyFromCache(*entry, filepath).get();
    }
    else
    {
        DeqNstr lines = Compile(state);
        std::ostringstream stream;
        UMap<size_t, const nemesis::Line*> modded_lines;
        size_t line_counter = 0;

        for (auto& line : lines)
        {
            stream << line + "\n";
            line_counter += std::count(line.begin(), line.end(), '\n') + 1;
            auto* file_ptr = line.GetFilePathPtr();

            if (!file_ptr) continue;

            modded_lines.insert({line_counter, &line});
        }

        CompileToHkx(hash, filepath, stream.str(), state, platform, version, include_xml, modded_lines, []() {})
            .get();
    }

    Logger::Log(LITERAL_PATH("Compiled Target File: ") + PATH_TO_STRING(filepath));
}

std::filesystem::path nemesis::HkxFile::ScheduleCompileFile(nemesis::CompileState& state,
                                                            nemesis::PlatformType platform,
                                                            nemesis::HavokVersion version,
                                                            bool include_xml) const
{
    std::filesystem::path target_path = NemesisInfo::PatchOutputPath(TargetPath);
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + PATH_TO_STRING(target_path));

    ScheduleCompileFileAs(
        target_path,
        state,
        platform,
        version,
        include_xml,
        [target_path] { Logger::Log(LITERAL_PATH("Compiled Target File: ") + PATH_TO_STRING(target_path)); });
    return target_path;
}

void nemesis::HkxFile::ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                             nemesis::CompileState& state,
                                             nemesis::PlatformType platform,
                                             nemesis::HavokVersion version,
                                             bool include_xml) const
{
    ScheduleCompileFileAs(filepath, state, platform, version, include_xml, [] {});
}

#include "Utilities/StringExtension.h"

void nemesis::HkxFile::ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                             nemesis::CompileState& state,
                                             nemesis::PlatformType platform,
                                             nemesis::HavokVersion version,
                                             bool include_xml,
                                             std::function<void()> callback) const
{
    std::string hash = GetHash(state) + static_cast<char>(version);
    auto* entry      = nemesis::CacheManager::GetEntry(hash);
    std::future<void> future;

    if (entry)
    {
        future = CopyFromCache(*entry, filepath);
    }
    else
    {
        SPtr<DeqNstr> lines = std::make_shared<DeqNstr>();
        CompileTo(*lines, state);

        std::ostringstream stream;
        UMap<size_t, const nemesis::Line*> modded_lines;
        size_t line_counter = 0;

        for (auto& line : *lines)
        {
            stream << line + "\n";
            line_counter += std::count(line.begin(), line.end(), '\n') + 1;
            auto* file_ptr = line.GetFilePathPtr();

            if (!file_ptr) continue;

            modded_lines.insert({line_counter, &line});
        }

        future = CompileToHkx(hash,
                              filepath,
                              stream.str(),
                              state,
                              platform,
                              version,
                              include_xml,
                              modded_lines,
                              // lines is required to be included to keep it alive throughout
                              // the whole async process for modded_lines reference when
                              // there is an error during compilation
                              [hash, lines, callback]() { callback(); });
    }

    std::scoped_lock<std::mutex> lock(CompileFutureMutex);
    CompileFuture.emplace_back(std::move(future));
}

void nemesis::HkxFile::WaitForCompleteCompilation() const
{
    std::scoped_lock<std::mutex> lock(CompileFutureMutex);

    for (auto& future : CompileFuture)
    {
        future.get();
    }

    CompileFuture.clear();
}

void nemesis::HkxFile::AddTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    std::scoped_lock<std::mutex> lock(TemplateMutex);
    TemplateMap[templt_obj->GetClassName()] = templt_obj;
}

void nemesis::HkxFile::AddModNode(const std::string& modcode)
{
    std::scoped_lock<std::mutex> lock(ModInUsedListMutex);
    ModInUsedList.insert(modcode);
}

nemesis::HkxNode* nemesis::HkxFile::AddModNode(const std::string& modcode, UPtr<nemesis::HkxNode>&& node)
{
    nemesis::SemanticManager manager;
    auto* node_ptr = node.get();
    auto mod_obj
        = std::make_unique<nemesis::ModObject>(modcode, 0, node->GetFilePath(), manager, std::move(node));

    {
        std::scoped_lock<std::mutex> lock(NodeMutex);
        NewNodes[node_ptr->GetNodeId()] = std::move(mod_obj);
    }

    auto& mod_list = manager.GetModInUsedList();

    {
        std::scoped_lock<std::mutex> lock(ModInUsedListMutex);
        ModInUsedList.insert(mod_list.begin(), mod_list.end());
    }

    return node_ptr;
}

nemesis::HkxNode* nemesis::HkxFile::GetNodeById(const std::string& node_id)
{
    auto itr = NodeMap.find(node_id);

    if (itr == NodeMap.end()) return nullptr;

    return itr->second;
}

const nemesis::HkxNode* nemesis::HkxFile::GetNodeById(const std::string& node_id) const
{
    auto itr = NodeMap.find(node_id);

    if (itr == NodeMap.end()) return nullptr;

    return itr->second;
}

const std::filesystem::path& nemesis::HkxFile::GetFilePath() const noexcept
{
    return FilePath;
}

const std::filesystem::path& nemesis::HkxFile::GetTargetPath() const noexcept
{
    return TargetPath;
}

const std::filesystem::path& nemesis::HkxFile::GetCachedFilePath() const noexcept
{
    return CachedFilePath;
}

const std::filesystem::path& nemesis::HkxFile::GetRelativePath() const noexcept
{
    return RelativePath;
}

bool nemesis::HkxFile::IsSameAsCached(nemesis::CompileState& state) const
{
    if (!std::filesystem::exists(CachedFilePath)) return false;

    auto mods = state.GetSelectedMods();

    for (auto& mod : mods)
    {
        if (ModInUsedList.find(mod) != ModInUsedList.end()) return false;
    }

    for (auto& templt_list : TemplateMap)
    {
        auto& requests = state.GetRequests(templt_list.first);

        if (!requests.empty()) return false;
    }

    return true;
}

size_t nemesis::HkxFile::GetSize() const
{
    return NodeMap.size() + RegularNodes->Size();
}

std::string nemesis::HkxFile::GetHash(nemesis::CompileState& state) const
{
    std::ostringstream oss("HkxFile:" + nemesis::to_utf8_string(FilePath));
    USetStr mod_set;

    for (auto& mod : state.GetSelectedMods())
    {
        mod_set.insert(mod);

        if (ModInUsedList.find(mod) == ModInUsedList.end()) continue;

        oss << mod << "\n";
    }

    SetStr hash_set;

    for (auto& node : NodeMap)
    {
        hash_set.insert(node.first + ":" + node.second->GetHash());
    }

    for (auto& node : NewNodes)
    {
        if (mod_set.find(node.second->GetStatement().GetExpression()) == mod_set.end()) continue;

        hash_set.insert(node.first + ":" + node.second->GetHash());
    }

    for (auto& templt_obj : TemplateMap)
    {
        hash_set.insert(templt_obj.first + ":" + templt_obj.second->GetHash());
    }

    for (auto& sub_templt_obj : state.GetManager().GetTemplateRepository().GetSubTemplateList())
    {
        hash_set.insert(sub_templt_obj->GetHash());
    }

    for (auto& hash : hash_set)
    {
        oss << hash;
    }

    return nemesis::SHA256::hex(oss.str());
}

bool nemesis::HkxFile::TryGetValueInHkcString(const std::string& line, std::string& value)
{
    size_t pos = line.find("<hkcstring>");

    if (pos == NOT_FOUND) return false;

    size_t cpos = line.find("</hkcstring>", pos);

    if (cpos == NOT_FOUND) return false;

    value = line.substr(pos + 11, cpos - pos - 11);
    return true;
}
