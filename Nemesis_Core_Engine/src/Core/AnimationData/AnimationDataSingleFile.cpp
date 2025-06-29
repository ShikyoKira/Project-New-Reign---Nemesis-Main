#include <fstream>

#include "Core/AnimationData/AnimationDataSingleFile.h"

#include "Core/CollectionObject.h"
#include "Core/CompilationManager.h"
#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Core/Template/TemplateObject.h"

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/Crc32.h"
#include "Utilities/File.h"

VecNstr nemesis::AnimationDataSingleFile::ParseHeaders(nemesis::LineStream& stream,
                                                       nemesis::SemanticManager& manager)
{
    VecNstr headers;

    if (stream.IsEoF()) return headers;

    auto& token = stream.GetToken();
    auto& value = token.Value;

    if (token.Type != nemesis::LineStream::NONE || !is_only_number(value.ToString()))
    {
        throw std::runtime_error("nemesis::AnimationDataSingleFile::ParseHeaders format error (Line: "
                                 + std::to_string(value.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    for (++stream; !stream.IsEoF() && !is_only_number(stream.GetToken().Value.ToString()); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::NONE)
        {
            auto& token_value = stream.GetToken().Value;
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
        }

        headers.emplace_back(token.Value);
    }

    return headers;
}

std::future<void> nemesis::AnimationDataSingleFile::CompileFileCore(const std::filesystem::path& filepath,
                                                                    nemesis::CompileState& state,
                                                                    std::function<void()> callback) const
{
    std::filesystem::create_directories(filepath.parent_path());

    return std::async(
        [this, filepath, &state, callback]
        {
            DeqNstr lines = Compile(state);
            std::ofstream file(filepath);

            if (!file.is_open())
            {
                std::error_code ec(errno, std::system_category());
                throw std::runtime_error("Failed to open file: \"" + to_utf8_string(filepath)
                                         + "\"\nMessage: " + ec.message());
            }

            std::string full_text;

            for (auto& line : lines)
            {
                std::string sline = line + "\n";
                file << sline;
                full_text.append(sline);
            }

            file.close();

            static nemesis::CRC32 crc32;
            size_t checksum = crc32.FullCRC(full_text);
            state.AddCheckSum(TargetPath, std::to_string(checksum));
            callback();
        });
}

void nemesis::AnimationDataSingleFile::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr header_lines;
    DeqNstr project_contents;

    {
        std::scoped_lock lock(ProjectMutex, ProjectTemplateMutex);

        for (auto& project : ProjectList)
        {
            DeqNstr temp_lines = project->Compile(state);

            if (temp_lines.empty()) continue;

            header_lines.emplace_back(project->GetName());

            for (auto& line : temp_lines)
            {
                project_contents.emplace_back(std::move(line));
            }
        }

        for (auto& templt_obj : ProjectTemplateList)
        {
            DeqNstr temp_lines = templt_obj->Compile(state);

            if (temp_lines.empty()) continue;

            header_lines.emplace_back(nemesis::to_utf8_string(templt_obj->GetFilePath().stem()));

            for (auto& line : temp_lines)
            {
                project_contents.emplace_back(std::move(line));
            }
        }
    }

    lines.emplace_back(std::to_string(header_lines.size()));

    for (auto& line : header_lines)
    {
        lines.emplace_back(std::move(line));
    }

    for (auto& line : project_contents)
    {
        lines.emplace_back(std::move(line));
    }
}

void nemesis::AnimationDataSingleFile::SerializeTo(DeqNstr& lines) const
{
    throw std::runtime_error("nemesis::AnimationDataSingleFile::SerializeTo is not supported");
}

UPtr<nemesis::NObject> nemesis::AnimationDataSingleFile::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationDataSingleFile> nemesis::AnimationDataSingleFile::Clone() const
{
    std::scoped_lock lock(ProjectMutex, ProjectTemplateMutex);
    auto singlefile = std::make_unique<nemesis::AnimationDataSingleFile>();

    for (auto& project : ProjectList)
    {
        auto& proj = singlefile->ProjectList.emplace_back(project->Clone());
        auto& name = proj->GetName();
        auto itr   = singlefile->ProjectNameMap.find(name);

        if (itr == singlefile->ProjectNameMap.end())
        {
            itr = singlefile->ProjectNameMap.insert({name, Vec<nemesis::AnimationDataProject*>()}).first;
        }

        itr->second.emplace_back(proj.get());
    }

    return singlefile;
}

std::filesystem::path nemesis::AnimationDataSingleFile::CompileFile(nemesis::CompileState& state) const
{
    auto target_path = NemesisInfo::PatchOutputPath(TargetPath);
    auto path        = PATH_TO_STRING(target_path);
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + path);

    CompileFileAs(target_path, state);

    Logger::Log(LITERAL_PATH("Compiled Target File: ") + path);
    return target_path;
}

void nemesis::AnimationDataSingleFile::CompileFileAs(const std::filesystem::path& filepath,
                                                     nemesis::CompileState& state) const
{
    CompileFileCore(filepath, state, [] {}).get();
}

std::filesystem::path
nemesis::AnimationDataSingleFile::ScheduleCompileFile(const std::filesystem::path& filepath,
                                                      nemesis::CompileState& state) const
{
    auto target_path = NemesisInfo::PatchOutputPath(TargetPath);
    auto path        = PATH_TO_STRING(target_path);
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + path);

    ScheduleCompileFileAs(
        target_path, state, [path] { Logger::Log(LITERAL_PATH("Compiled Target File: ") + path); });
    return target_path;
}

void nemesis::AnimationDataSingleFile::ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                                             nemesis::CompileState& state) const
{
    ScheduleCompileFileAs(filepath, state, [] {});
}

void nemesis::AnimationDataSingleFile::ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                                             nemesis::CompileState& state,
                                                             std::function<void()> callback) const
{
    auto future = CompileFileCore(filepath, state, callback);

    std::scoped_lock<std::mutex> lock(CompileFutureMutex);
    CompileFuture.emplace_back(std::move(future));
}

void nemesis::AnimationDataSingleFile::WaitForCompleteCompilation() const
{
    std::scoped_lock<std::mutex> lock(CompileFutureMutex);

    for (auto& future : CompileFuture)
    {
        future.get();
    }

    CompileFuture.clear();
}

std::filesystem::path nemesis::AnimationDataSingleFile::GetFilePath() const
{
    return FilePath;
}

std::filesystem::path nemesis::AnimationDataSingleFile::GetTargetPath() const
{
    return TargetPath;
}

std::filesystem::path nemesis::AnimationDataSingleFile::GetRelativePath() const
{
    return RelativePath;
}

nemesis::AnimationDataProject* nemesis::AnimationDataSingleFile::GetProject(const std::string& project_name,
                                                                            size_t order_position)
{
    std::scoped_lock<std::mutex> lock(ProjectMutex);
    auto itr = ProjectNameMap.find(project_name);

    if (itr == ProjectNameMap.end()) return nullptr;

    if (order_position > itr->second.size()) return nullptr;

    return itr->second[order_position - 1];
}

UPtr<nemesis::AnimationDataProject>&
nemesis::AnimationDataSingleFile::AddProject(UPtr<nemesis::AnimationDataProject>&& project)
{
    auto& proj = ProjectList.emplace_back(std::move(project));
    auto& name = proj->GetName();

    std::scoped_lock<std::mutex> lock(ProjectMutex);
    auto itr = ProjectNameMap.find(name);

    if (itr == ProjectNameMap.end())
    {
        itr = ProjectNameMap.insert({name, Vec<nemesis::AnimationDataProject*>()}).first;
    }

    itr->second.emplace_back(proj.get());
    return proj;
}

SPtr<nemesis::TemplateObject>&
nemesis::AnimationDataSingleFile::AddProjectTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    std::scoped_lock<std::mutex> lock(ProjectTemplateMutex);
    return ProjectTemplateList.emplace_back(templt_obj);
}

void nemesis::AnimationDataSingleFile::SerializeToDirectory(const std::filesystem::path& directory_path) const
{
    std::filesystem::create_directories(directory_path);

    for (size_t i = 0; i < ProjectList.size(); i++)
    {
        auto& project    = ProjectList[i];
        auto folder_name = std::to_string(i + 1) + "~" + project->GetName();
        project->SerializeToDirectory(directory_path / folder_name);
    }
}

UPtr<nemesis::AnimationDataSingleFile>
nemesis::AnimationDataSingleFile::DeserializeFromDirectory(const std::filesystem::path& directory_path)
{
    auto singlefile = std::make_unique<nemesis::AnimationDataSingleFile>();
    Map<size_t, std::pair<std::filesystem::path, std::string>> project_dir;

    for (auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (!entry.is_directory()) continue;

        auto path     = entry.path();
        auto filename = nemesis::to_utf8_string(path.filename());
        size_t pos    = filename.rfind("~");

        if (pos == NOT_FOUND) continue;

        std::string num = filename.substr(pos);

        if (is_only_number(num)) continue;

        project_dir[std::stoul(num)] = std::make_pair(path, filename.substr(pos + 1));
    }

    for (auto& each : project_dir)
    {
        singlefile->AddProject(
            nemesis::AnimationDataProject::DeserializeFromDirectory(each.second.first, each.second.second));
    }

    return singlefile;
}

UPtr<nemesis::AnimationDataSingleFile>
nemesis::AnimationDataSingleFile::ParseFromFile(const std::filesystem::path& filepath)
{
    Logger::Log(LITERAL_PATH("AnimationDataSingleFile: ") + PATH_TO_STRING(filepath));

    auto singlefile        = std::make_unique<nemesis::AnimationDataSingleFile>();
    auto filename          = PATH_TO_STRING(filepath.filename());
    singlefile->FilePath   = filepath;
    singlefile->TargetPath
        = filepath.parent_path()
          / (nemesis::istarts_with(filename, LITERAL_PATH("nemesis_")) ? filename.substr(8) : filename);
    singlefile->RelativePath
        = PATH_TO_STRING(singlefile->TargetPath).substr(PATH_TO_STRING(NemesisInfo::DataPath()).length() + 1);

    Logger::Log(LITERAL_PATH("Processing AnimationDataSingleFile: ")
                + PATH_TO_STRING(singlefile->GetFilePath()));

    nemesis::SemanticManager manager;
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    nemesis::LineStream stream(lines.begin(), lines.end());

    VecNstr headers         = nemesis::AnimationDataSingleFile::ParseHeaders(stream, manager);
    singlefile->ProjectList = nemesis::AnimationDataProject::ParseObjects(stream, manager, headers);

    for (auto& proj : singlefile->ProjectList)
    {
        auto& name = proj->GetName();
        auto itr   = singlefile->ProjectNameMap.find(name);

        if (itr == singlefile->ProjectNameMap.end())
        {
            itr = singlefile->ProjectNameMap.insert({name, Vec<nemesis::AnimationDataProject*>()}).first;
        }

        itr->second.emplace_back(proj.get());
    }

    return singlefile;
}

UPtr<nemesis::AnimationDataSingleFile>
nemesis::AnimationDataSingleFile::ParseFromFile(const std::filesystem::path& filepath,
                                                nemesis::ThreadPool& threadpool)
{
    Logger::Log(LITERAL_PATH("AnimationDataSingleFile: ") + PATH_TO_STRING(filepath));

    auto singlefile        = std::make_unique<nemesis::AnimationDataSingleFile>();
    auto filename          = PATH_TO_STRING(filepath.filename());
    singlefile->FilePath   = filepath;
    singlefile->TargetPath = filepath.parent_path()
          / (nemesis::istarts_with(filename, LITERAL_PATH("nemesis_")) ? filename.substr(8) : filename);
    singlefile->RelativePath
        = PATH_TO_STRING(singlefile->TargetPath).substr(PATH_TO_STRING(NemesisInfo::DataPath()).length() + 1);
    auto singlefile_ptr = singlefile.get();

    threadpool.enqueue(
        [singlefile_ptr]()
        {
            Logger::Log(LITERAL_PATH("Processing AnimationDataSingleFile: ")
                        + PATH_TO_STRING(singlefile_ptr->GetFilePath()));

            nemesis::SemanticManager manager;
            VecNstr lines;
            GetFileLines(singlefile_ptr->GetFilePath(), lines, false);
            nemesis::LineStream stream(lines.begin(), lines.end());

            VecNstr headers = nemesis::AnimationDataSingleFile::ParseHeaders(stream, manager);
            singlefile_ptr->ProjectList
                = nemesis::AnimationDataProject::ParseObjects(stream, manager, headers);

            for (auto& proj : singlefile_ptr->ProjectList)
            {
                auto& name = proj->GetName();
                auto itr   = singlefile_ptr->ProjectNameMap.find(name);

                if (itr == singlefile_ptr->ProjectNameMap.end())
                {
                    itr = singlefile_ptr->ProjectNameMap.insert({name, Vec<nemesis::AnimationDataProject*>()})
                              .first;
                }

                itr->second.emplace_back(proj.get());
            }
        });
    return singlefile;
}
