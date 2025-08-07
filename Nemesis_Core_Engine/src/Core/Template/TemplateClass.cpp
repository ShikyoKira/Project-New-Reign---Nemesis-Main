#include <fstream>
#include <regex>
#include <sstream>

#include "Core/Template/TemplateAnimDataClipData.h"
#include "Core/Template/TemplateAnimDataMotionData.h"
#include "Core/Template/TemplateClass.h"
#include "Core/Template/TemplateHkx.h"

#include "nlohmann/json.hpp"

#include "Core/NObjectRepository.h"

#include "Core/Hkx/HkxBehavior.h"
#include "Core/Hkx/HkxCharacter.h"
#include "Core/Hkx/HkxNode.h"

#include "Core/AnimationData/AnimationDataSingleFile.h"

#include "Core/AnimationSetData/AnimationSetDataSingleFile.h"

#include "Utilities/Algorithm.h"

#include "Logger.h"

using Json = nlohmann::json;

namespace sf = std::filesystem;

void nemesis::TemplateClass::ParseHkxTemplatesLoopDirectory(const std::filesystem::path& relative_parent_path,
                                                            const std::filesystem::path& dir,
                                                            nemesis::TemplateClass& templt_class,
                                                            nemesis::NObjectRepository& repo,
                                                            nemesis::ThreadPool& thread_pool)
{
    nemesis::HkxFile* hkx_file
        = repo.GetBehavior(PATH_TO_STRING(relative_parent_path) + LITERAL_PATH(".nemx"));

    if (!hkx_file)
    {
        hkx_file = repo.GetCharacter(PATH_TO_STRING(relative_parent_path) + LITERAL_PATH(".nemx"));
    }

    if (!hkx_file)
    {
        for (auto& entry : sf::directory_iterator(dir))
        {
            if (!entry.is_directory()) continue;

            auto path = entry.path();
            ParseHkxTemplatesLoopDirectory(
                relative_parent_path / path.filename(), path, templt_class, repo, thread_pool);
        }

        return;
    }

    Map<size_t, sf::path> template_files;
    std::regex templt_rgx("^" + templt_class.GetName() + "_([0-9]+)$");

    for (auto& entry : sf::directory_iterator(dir))
    {
        auto path = entry.path();

        if (entry.is_directory())
        {
            ParseHkxTemplatesLoopDirectory(
                relative_parent_path / path.filename(), path, templt_class, repo, thread_pool);
            continue;
        }

        if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".nemx"))) continue;

        std::smatch match;
        std::string filename = nemesis::to_utf8_string(path.stem());

        if (std::regex_match(filename, match, templt_rgx))
        {
            template_files[std::stoul(match[1])] = path;
            continue;
        }

        AddTemplateToHkxFile(path, templt_class, *hkx_file, thread_pool);
    }

    if (template_files.empty()) return;

    size_t start_index = template_files.find(0) == template_files.end();

    auto itr = template_files.find(start_index);

    if (itr == template_files.end()) return;

    SPtr<nemesis::TemplateObject> templt_sptr(
        nemesis::TemplateHkx::ParseFromFile(itr->second, &templt_class, thread_pool).release());
    templt_class.AddTemplate(templt_sptr);
    hkx_file->AddTemplate(templt_sptr);
    nemesis::TemplateObject* templt_obj = templt_sptr.get();

    for (++itr; itr != template_files.end(); ++itr)
    {
        auto templt_uptr = nemesis::TemplateHkx::ParseFromFile(itr->second, &templt_class, thread_pool);
        templt_obj       = (templt_obj->SetChild(std::move(templt_uptr))).get();
    }
}

void nemesis::TemplateClass::AddTemplateToHkxFile(const std::filesystem::path& templt_path,
                                                  nemesis::TemplateClass& templt_class,
                                                  nemesis::HkxFile& hkx_file,
                                                  nemesis::ThreadPool& thread_pool)
{
    auto filename = nemesis::to_utf8_string(templt_path.stem());
    auto* node    = hkx_file.GetNodeById(filename);

    if (!node)
    {
        throw std::runtime_error("Node id does not exist in behavior (Behavior: "
                                 + nemesis::to_utf8_string(templt_path) + ", Node Id: " + filename + ")");
    }

    thread_pool.enqueue(
        [templt_path, &templt_class, node]()
        {
            auto m_node = nemesis::HkxNode::DeserializeHkxNodeFromFile(templt_path, &templt_class);
            node->MatchAndUpdate(*m_node);
        });
}

void nemesis::TemplateClass::AddTemplateToAnimDataSingleFile(const std::filesystem::path& dir,
                                                             nemesis::TemplateClass& templt_class,
                                                             nemesis::AnimationDataSingleFile& singlefile,
                                                             nemesis::ThreadPool& thread_pool)
{
    static const std::regex str_num_rgx("^([^~]+)~([0-9]+)$");
    std::regex templt_rgx("^\\$" + templt_class.GetName() + "_([0-9]+)\\$(UC|)$");

    if (!sf::exists(dir)) return;

    for (auto& entry : sf::directory_iterator(dir))
    {
        sf::path path = entry.path();

        if (!entry.is_directory())
        {
            if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".txt"))) continue;

            SPtr<nemesis::TemplateObject> templt_obj(
                nemesis::TemplateObject::ParseFromFile(path, &templt_class, thread_pool).release());
            templt_class.AddTemplate(templt_obj);
            singlefile.AddProjectTemplate(templt_obj);
            continue;
        }

        std::smatch match;
        std::string proj_name = nemesis::to_utf8_string(path.filename());

        if (!std::regex_match(proj_name, match, str_num_rgx)) continue;

        auto project = singlefile.GetProject(match[1], std::stoul(match[2]));

        if (!project) continue;

        Map<size_t, sf::path> clip_template_files;
        Map<size_t, sf::path> motion_template_files;

        for (auto& inner_entry : sf::directory_iterator(path))
        {
            if (!inner_entry.is_regular_file()) continue;

            sf::path inner_path = inner_entry.path();

            if (!nemesis::iequals(PATH_TO_STRING(inner_path.extension()), LITERAL_PATH(".txt"))) continue;

            std::string filename = nemesis::to_utf8_string(inner_path.stem());

            if (nemesis::iequals(filename, "$header$"))
            {
                auto header = nemesis::AnimationDataProject::DeserializeHeaderFromFile(inner_path);
                project->MatchAndUpdateHeader(*header);
                continue;
            }

            std::smatch inner_match;

            if (std::regex_match(filename, inner_match, str_num_rgx))
            {
                auto clipdata = project->GetClipData(inner_match[1], inner_match[2]);

                if (!clipdata) continue;

                auto m_clipdata
                    = nemesis::AnimationDataClipData::DeserializeFromFile(inner_path, &templt_class);
                clipdata->MatchAndUpdate(*m_clipdata);
                continue;
            }

            if (is_only_number(filename))
            {
                auto motiondata = project->GetMotionData(filename);

                if (!motiondata) continue;

                auto m_motiondata
                    = nemesis::AnimationDataMotionData::DeserializeFromFile(inner_path, &templt_class);
                motiondata->MatchAndUpdate(*m_motiondata);
                continue;
            }

            std::smatch templt_match;

            if (std::regex_match(filename, templt_match, templt_rgx))
            {
                if (templt_match.str(2).empty())
                {
                    clip_template_files[std::stoul(templt_match[1])] = inner_path;
                }
                else
                {
                    motion_template_files[std::stoul(templt_match[1])] = inner_path;
                }
            }
        }

        if (!clip_template_files.empty())
        {
            size_t start_index = clip_template_files.find(0) == clip_template_files.end();

            auto itr = clip_template_files.find(start_index);

            if (itr != clip_template_files.end())
            {
                auto templt_sptr = SPtr<nemesis::TemplateAnimDataClipData>(
                    nemesis::TemplateAnimDataClipData::ParseFromFile(itr->second, &templt_class, start_index, thread_pool)
                        .release());
                templt_class.AddTemplate(templt_sptr);
                project->AddClipDataTemplate(templt_sptr);
                nemesis::TemplateObject* templt_obj = templt_sptr.get();

                for (size_t i = start_index + 1; i < clip_template_files.size(); i++)
                {
                    auto itr = clip_template_files.find(i);

                    if (itr == clip_template_files.end()) break;

                    auto templt_uptr = nemesis::TemplateAnimDataClipData::ParseFromFile(
                        itr->second, &templt_class, i, thread_pool);
                    templt_obj = (templt_obj->SetChild(std::move(templt_uptr))).get();
                }
            }
        }

        if (motion_template_files.empty()) return;

        size_t start_index = motion_template_files.find(0) == motion_template_files.end();

        auto itr = motion_template_files.find(start_index);

        if (itr == motion_template_files.end()) return;

        auto templt_sptr = SPtr<nemesis::TemplateAnimDataMotionData>(
            nemesis::TemplateAnimDataMotionData::ParseFromFile(
                itr->second, &templt_class, start_index, thread_pool)
                .release());
        templt_class.AddTemplate(templt_sptr);
        project->AddMotionDataTemplate(templt_sptr);
        nemesis::TemplateObject* templt_obj = templt_sptr.get();

        for (size_t i = start_index + 1; i < motion_template_files.size(); i++)
        {
            auto itr = motion_template_files.find(i);

            if (itr == motion_template_files.end()) break;

            auto templt_uptr = nemesis::TemplateAnimDataMotionData::ParseFromFile(
                itr->second, &templt_class, i, thread_pool);
            templt_obj = (templt_obj->SetChild(std::move(templt_uptr))).get();
        }
    }
}

void nemesis::TemplateClass::AddTemplateToAnimSetDataSingleFile(
    const std::filesystem::path& dir,
    nemesis::TemplateClass& templt_class,
    nemesis::AnimationSetDataSingleFile& singlefile)
{
    static const std::regex str_num_rgx("^([^~]+)~([0-9]+)$");

    if (!sf::exists(dir)) return;

    for (auto& entry : sf::directory_iterator(dir))
    {
        sf::path path = entry.path();

        if (!entry.is_directory())
        {
            if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".txt"))) continue;

            SPtr<nemesis::TemplateObject> templt_obj(
                nemesis::TemplateObject::ParseFromFile(path, &templt_class).release());
            templt_class.AddTemplate(templt_obj);
            singlefile.AddProjectTemplate(templt_obj);
            continue;
        }

        std::string name = nemesis::to_utf8_string(path.filename());
        nemesis::replace(name, '~', '\\');
        auto project = singlefile.GetProject(name);

        if (!project)
        {
            for (auto& inner_entry : sf::directory_iterator(path))
            {
                if (!inner_entry.is_regular_file()) continue;

                sf::path inner_path = inner_entry.path();

                if (!nemesis::iequals(PATH_TO_STRING(inner_path.extension()), LITERAL_PATH(".txt"))) continue;

                auto m_state = nemesis::AnimationSetDataState::DeserializeFromFile(inner_path, &templt_class);
                project->AddState(std::move(m_state));
            }

            continue;
        }

        for (auto& inner_entry : sf::directory_iterator(path))
        {
            if (!inner_entry.is_regular_file()) continue;

            sf::path inner_path = inner_entry.path();

            if (!nemesis::iequals(PATH_TO_STRING(inner_path.extension()), LITERAL_PATH(".txt"))) continue;

            auto state = project->GetState(nemesis::to_utf8_string(inner_path.filename()));

            if (!state)
            {
                SPtr<nemesis::TemplateObject> templt_obj(
                    nemesis::TemplateObject::ParseFromFile(inner_path, &templt_class).release());
                project->AddStateTemplate(templt_obj);
                continue;
            }

            auto m_state = nemesis::AnimationSetDataState::DeserializeFromFile(inner_path, &templt_class);
            state->MatchAndUpdate(*m_state);
        }
    }
}

void nemesis::TemplateClass::LoadConfig()
{
    Json template_info;
    std::ifstream json_file(InfoPath);
    json_file >> template_info;

    IsArray        = template_info["IsArray"].get<bool>();
    MinArraySize   = template_info["MinimumArray"].get<size_t>();
    auto& elements = template_info["Options"];

    for (Json::iterator it = elements.begin(); it != elements.end(); ++it)
    {
        auto& body = *it;

        if (!body.contains("Name")) continue;

        std::string name = body["Name"].get<std::string>();
        VecStr aliases   = body.contains("Aliases") ? body["Aliases"].get<VecStr>() : VecStr();
        VecStr variables = body.contains("Variables") ? body["Variables"].get<VecStr>() : VecStr();
        bool b_array     = body.contains("IsArray") ? body["IsArray"].get<bool>() : false;
        auto& opt_uptr   = OptionModelList.emplace_back(
            std::make_unique<nemesis::TemplateOptionModel>(name, aliases, variables, b_array));

        for (auto& var : opt_uptr->GetAliases())
        {
            OptionModelTrie.Add(var, opt_uptr.get());
        }

        OptionModelTrie.Add(opt_uptr->GetName(), opt_uptr.get());
    }

    std::sort(OptionModelList.begin(),
              OptionModelList.end(),
              [](UPtr<nemesis::TemplateOptionModel>& model_1, UPtr<nemesis::TemplateOptionModel>& model_2)
              { return model_1->GetName().size() > model_2->GetName().size(); });
}

std::future<void> nemesis::TemplateClass::SetupAsync(nemesis::NObjectRepository& repo,
                                                     nemesis::ThreadPool& thread_pool)
{
    return std::async(
        [this, &repo, &thread_pool]()
        {
            sf::path dir = InfoPath.parent_path();

            for (auto& entry : sf::directory_iterator(dir))
            {
                if (!entry.is_directory()) continue;

                auto path = entry.path();

                if (!nemesis::iequals(PATH_TO_STRING(path.filename()), LITERAL_PATH("meshes")))
                {
                    sf::path relative_path = PATH_TO_STRING(path).substr(PATH_TO_STRING(dir).size() + 1);
                    ParseHkxTemplatesLoopDirectory(relative_path, path, *this, repo, thread_pool);
                    continue;
                }

                if (!sf::exists(path)) continue;

                for (auto& inner_entry : sf::directory_iterator(path))
                {
                    auto inner_path = inner_entry.path();

                    if (!inner_entry.is_directory()) continue;

                    auto filename = PATH_TO_STRING(inner_path.stem());

                    if (nemesis::iequals(filename, LITERAL_PATH("animationdatasinglefile")))
                    {
                        auto* file = repo.GetAnimDataSingleFile();

                        if (!file) continue;

                        AddTemplateToAnimDataSingleFile(inner_path, *this, *file, thread_pool);
                        continue;
                    }

                    if (nemesis::iequals(filename, LITERAL_PATH("animationdatasinglefile")))
                    {
                        auto* file = repo.GetAnimSetDataSingleFile();

                        if (!file) continue;

                        AddTemplateToAnimSetDataSingleFile(inner_path, *this, *file);
                        continue;
                    }

                    sf::path relative_path
                        = PATH_TO_STRING(inner_path).substr(PATH_TO_STRING(dir).size() + 1);
                    ParseHkxTemplatesLoopDirectory(
                        relative_path, inner_path, *this, repo, thread_pool);
                }
            }
        });
}

nemesis::TemplateClass::TemplateClass(const std::filesystem::path& template_info_path,
                                      nemesis::NObjectRepository& repo,
                                      nemesis::ThreadPool& thread_pool)
    : Name(nemesis::to_utf8_string(template_info_path.parent_path().stem()))
    , InfoPath(template_info_path)
{
    Logger::Log("Templates Class: " + Name);

    LoadConfig();
    InitializerFuture = SetupAsync(repo, thread_pool);
}

void nemesis::TemplateClass::FinalizeInitialization()
{
    InitializerFuture.get();
}

void nemesis::TemplateClass::AddTemplate(const SPtr<nemesis::TemplateObject>& template_object)
{
    if (template_object->GetTemplateClass() != this)
    {
        throw std::runtime_error("Template object and template class do not match");
    }

    Templates.emplace_back(template_object);
}

SPtr<nemesis::TemplateObject> nemesis::TemplateClass::AddTemplate(const std::filesystem::path& filepath)
{
    return Templates.emplace_back(nemesis::TemplateObject::ParseFromFile(filepath, this).release());
}

SPtr<nemesis::TemplateObject> nemesis::TemplateClass::AddTemplate(const std::filesystem::path& filepath,
                                                                  nemesis::ThreadPool& thread_pool)
{
    return Templates.emplace_back(
        nemesis::TemplateObject::ParseFromFile(filepath, this, thread_pool).release());
}

SPtr<nemesis::TemplateObject> nemesis::TemplateClass::GetTemplate(size_t index)
{
    return Templates[index];
}

const nemesis::TemplateObject* nemesis::TemplateClass::GetTemplate(size_t index) const
{
    return Templates[index].get();
}

Vec<const nemesis::TemplateObject*> nemesis::TemplateClass::GetTemplateList() const
{
    return Vec<const nemesis::TemplateObject*>();
}

size_t nemesis::TemplateClass::GetSize() const noexcept
{
    return Templates.size();
}

const std::string& nemesis::TemplateClass::GetName() const noexcept
{
    return Name;
}

size_t nemesis::TemplateClass::GetRequestMinArraySize() const noexcept
{
    return MinArraySize;
}

size_t nemesis::TemplateClass::GetRequestMaxArraySize() const noexcept
{
    return MaxArraySize;
}

bool nemesis::TemplateClass::IsRequestArray() const noexcept
{
    return IsArray;
}

const nemesis::TemplateOptionModel* nemesis::TemplateClass::GetModel(const std::string& name) const
{
    for (auto& option_model : OptionModelList)
    {
        if (option_model->GetName() != name && !option_model->HasAlias(name)) continue;

        return option_model.get();
    }

    return nullptr;
}

const std::filesystem::path& nemesis::TemplateClass::GetInfoPath() const noexcept
{
    return InfoPath;
}

UPtr<nemesis::AnimationRequest>
nemesis::TemplateClass::CreateRequest(const std::string& list_name,
                                      const std::string& request_info,
                                      size_t linenum,
                                      const std::filesystem::path& filepath) const
{
    UPtr<nemesis::AnimationRequest> request;
    std::stringstream ss(request_info);
    std::string component;

    if (!(ss >> component)) return nullptr;

    if (component != Name)
    {
        for (auto& ch : component)
        {
            if (ch == '+') continue;

            return nullptr;
        }
    }

    request = std::make_unique<nemesis::AnimationRequest>(list_name, *this);

    if (!(ss >> component)) return nullptr;

    if (component[0] == '-')
    {
        std::stringstream opt_ss(component.substr(1));
        USet<std::string> matched_options;

        while (std::getline(opt_ss, component, ','))
        {
            if (component.empty()) continue;

            auto matches = OptionModelTrie.FindMatches(component);

            if (matches.empty()) continue;

            Vec<std::runtime_error> exceptions_caught;

            for (auto& match : matches)
            {
                try
                {
                    nemesis::TemplateOptionModel* option_model = *match;
                    auto option = option_model->TryCreateOption(component, linenum, filepath);

                    if (!option) continue;

                    if (matched_options.find(option_model->GetName()) != matched_options.end())
                    {
                        throw std::runtime_error(
                            "Repeated non-array option found (Option: " + option_model->GetName()
                            + ", Request info : " + request_info + ", Line: " + std::to_string(linenum)
                            + ", File: " + nemesis::to_utf8_string(filepath) + ")");
                    }

                    if (!option_model->IsArray())
                    {
                        matched_options.insert(option_model->GetName());
                    }

                    request->AddOption(std::move(option));
                    goto OptionCreated;
                }
                catch (const std::runtime_error& ex)
                {
                    exceptions_caught.push_back(ex);
                }
            }

            if (!exceptions_caught.empty()) throw exceptions_caught.front();

        OptionCreated:
        }

        if (!(ss >> component)) return nullptr;
    }

    request->SetAnimationEvent(component);

    if (!(ss >> component)) return nullptr;

    // window's path separator is used for consistency
    request->SetAnimationFilePath(nemesis::to_utf8_string(filepath.parent_path().stem()) + "\\" + component);

    while (ss >> component)
    {
        size_t pos = component.rfind("/");

        if (pos == NOT_FOUND)
        {
            request->AddMapValue("1", component);
            continue;
        }

        if (pos == 0)
        {
            throw std::runtime_error("Invalid Map key request (Request info: " + request_info
                                     + ", Line: " + std::to_string(linenum)
                                     + ", File: " + nemesis::to_utf8_string(filepath) + ")");
        }

        std::string value = component.substr(0, pos);
        std::string key   = component.substr(pos + 1);
        request->AddMapValue(key, value);
    }

    return request;
}
