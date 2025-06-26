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

    Map<size_t, std::filesystem::path> template_files;

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

        std::regex templt_rgx("^" + templt_class.GetName() + "_([0-9]+)$");
        std::smatch match;
        std::string filename = nemesis::to_utf8_string(path.stem());

        if (std::regex_match(filename, match, templt_rgx))
        {
            template_files[std::stoul(match[1])] = path;
            continue;
        }

        AddTemplateToHkxFile(relative_parent_path, path, templt_class, *hkx_file);
    }

    if (template_files.empty()) return;

    nemesis::TemplateObject* templt_obj = nullptr;
    size_t start_index                  = template_files.find(0) == template_files.end();

    for (size_t i = start_index; i < template_files.size(); i++)
    {
        auto itr = template_files.find(i);

        if (itr == template_files.end()) break;

        auto templt_uptr = nemesis::TemplateObject::ParseFromFile(itr->second, &templt_class, thread_pool);

        if (!templt_obj)
        {
            templt_obj = (templt_obj->SetChild(std::move(templt_uptr))).get();
            continue;
        }

        SPtr<nemesis::TemplateObject> templt_sptr(std::move(templt_uptr));
        templt_class.AddTemplate(templt_sptr);
        templt_obj = templt_sptr.get();
    }
}

void nemesis::TemplateClass::AddTemplateToHkxFile(const std::filesystem::path& relative_parent_path,
                                                  const std::filesystem::path& templt_path,
                                                  nemesis::TemplateClass& templt_class,
                                                  nemesis::HkxFile& hkx_file)
{
    std::string filename = nemesis::to_utf8_string(templt_path.stem());
    auto node            = hkx_file.GetNodeById(filename);

    if (!node)
    {
        throw std::runtime_error("Node id does not exist in behavior (Behavior: "
                                 + nemesis::to_utf8_string(templt_path) + ", Node Id: " + filename + ")");
    }

    auto m_node = nemesis::HkxNode::DeserializeHkxNodeFromFile(templt_path, &templt_class);
    node->MatchAndUpdate(*m_node);
}

void nemesis::TemplateClass::AddTemplateToAnimDataSingleFile(const std::filesystem::path& dir,
                                                             nemesis::TemplateClass& templt_class,
                                                             nemesis::AnimationDataSingleFile& singlefile,
                                                             nemesis::ThreadPool& thread_pool)
{
    static const std::regex str_num_rgx("^([^~]+)~([0-9]+)$");

    if (!std::filesystem::exists(dir)) return;

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

            if (nemesis::iequals(filename, "$" + templt_class.GetName() + "$"))
            {
                auto templt_obj = nemesis::TemplateAnimDataClipData::ParseFromFile(
                    &templt_class, inner_path, thread_pool);
                templt_class.AddTemplate(templt_obj);
                project->AddClipDataTemplate(templt_obj);
                continue;
            }

            if (!nemesis::iequals(filename, "$" + templt_class.GetName() + "$UC")) continue;

            auto templt_obj
                = nemesis::TemplateAnimDataMotionData::ParseFromFile(&templt_class, inner_path, thread_pool);
            templt_class.AddTemplate(templt_obj);
            project->AddMotionDataTemplate(templt_obj);
        }
    }
}

void nemesis::TemplateClass::AddTemplateToAnimSetDataSingleFile(
    const std::filesystem::path& dir,
    nemesis::TemplateClass& templt_class,
    nemesis::AnimationSetDataSingleFile& singlefile)
{
    static const std::regex str_num_rgx("^([^~]+)~([0-9]+)$");

    if (!std::filesystem::exists(dir)) return;

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

nemesis::TemplateClass::TemplateClass(const std::filesystem::path& template_info_path)
    : Name(nemesis::to_utf8_string(template_info_path.parent_path().stem()))
    , InfoPath(template_info_path)
{
    Logger::Log("Templates Class: " + Name);

    Json template_info;
    std::ifstream json_file(template_info_path);
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
        OptionModelList.emplace_back(
            std::make_unique<nemesis::TemplateOptionModel>(name, aliases, variables, b_array));
    }

    std::sort(OptionModelList.begin(),
              OptionModelList.end(),
              [](UPtr<nemesis::TemplateOptionModel>& model_1, UPtr<nemesis::TemplateOptionModel>& model_2)
              { return model_1->GetName().size() > model_2->GetName().size(); });
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

UPtr<nemesis::AnimationRequest> nemesis::TemplateClass::CreateRequest(
    const std::string& request_info, size_t linenum, const std::filesystem::path& filepath) const
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

    request = std::make_unique<nemesis::AnimationRequest>(*this);

    if (!(ss >> component)) return nullptr;

    if (component[0] == '-')
    {
        std::stringstream opt_ss(component.substr(1));

        while (std::getline(opt_ss, component, ','))
        {
            if (component.empty()) continue;

            for (auto& option_model : OptionModelList)
            {
                auto option = option_model->TryCreateOption(component, linenum, filepath);

                if (!option) continue;

                request->AddOption(std::move(option));
                break;
            }
        }

        if (!(ss >> component)) return nullptr;
    }

    request->SetAnimationEvent(component);

    if (!(ss >> component)) return nullptr;

    request->SetAnimationFilePath(component);

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

UPtr<nemesis::TemplateClass> nemesis::TemplateClass::ParseTemplateClassFromDirectory(
    const std::filesystem::path& dir, nemesis::NObjectRepository& repo, nemesis::ThreadPool& thread_pool)
{
    auto info_path = dir / "template_info.json";

    if (!sf::exists(info_path) || !sf::is_regular_file(info_path)) return nullptr;

    UPtr<nemesis::TemplateClass> templt_class = std::make_unique<nemesis::TemplateClass>(info_path);

    if (!std::filesystem::exists(dir)) return templt_class;

    for (auto& entry : sf::directory_iterator(dir))
    {
        if (!entry.is_directory()) continue;

        auto path = entry.path();

        if (!nemesis::iequals(PATH_TO_STRING(path.filename()), LITERAL_PATH("meshes")))
        {
            sf::path relative_path = sf::path(LITERAL_PATH("data"))
                                     / PATH_TO_STRING(path).substr(PATH_TO_STRING(dir).size() + 1);
            ParseHkxTemplatesLoopDirectory(relative_path, path, *templt_class, repo, thread_pool);
            continue;
        }

        if (!std::filesystem::exists(path)) continue;

        for (auto& inner_entry : sf::directory_iterator(path))
        {
            auto inner_path = inner_entry.path();

            if (!inner_entry.is_directory()) continue;

            sf::path relative_path = sf::path(LITERAL_PATH("data"))
                                     / PATH_TO_STRING(inner_path).substr(PATH_TO_STRING(dir).size() + 1);
            auto filename = PATH_TO_STRING(inner_path.stem());

            if (nemesis::iequals(filename, LITERAL_PATH("animationdatasinglefile")))
            {
                AddTemplateToAnimDataSingleFile(
                    inner_path, *templt_class, *repo.GetAnimDataSingleFile(), thread_pool);
                continue;
            }

            if (nemesis::iequals(filename, LITERAL_PATH("animationdatasinglefile")))
            {
                AddTemplateToAnimSetDataSingleFile(
                    inner_path, *templt_class, *repo.GetAnimSetDataSingleFile());
                continue;
            }

            ParseHkxTemplatesLoopDirectory(relative_path, inner_path, *templt_class, repo, thread_pool);
        }
    }

    return templt_class;
}
