#include "core/Template/TemplateClass.h"

#include "external/nlohmann-json/json.hpp"

using Json = nlohmann::json;

nemesis::TemplateClass::TemplateClass(const std::filesystem::path& template_info_path)
    : Name(template_info_path.parent_path().stem().string())
    , InfoPath(template_info_path)
{
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
    return Templates.emplace_back(nemesis::TemplateObject::CreateFromFile(this, filepath));
}

SPtr<nemesis::TemplateObject> nemesis::TemplateClass::AddTemplate(const std::filesystem::path& filepath,
                                                                  nemesis::ThreadPool& thread_pool)
{
    return Templates.emplace_back(nemesis::TemplateObject::CreateFromFile(this, filepath, thread_pool));
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

UPtr<nemesis::AnimationRequest> nemesis::TemplateClass::CreateRequest(const std::string& request_info,
                                                                      size_t index,
                                                                      size_t linenum,
                                                                      const std::filesystem::path& filepath) const
{
    UPtr<nemesis::AnimationRequest> request;
    std::stringstream ss(request_info);
    std::string component;

    if (!(ss >> component)) return nullptr;

    if (component != Name && component != "+") return nullptr;

    request = std::make_unique<nemesis::AnimationRequest>(Name, index, IsArray);

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

                if (option == nullptr) continue;

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
            throw std::runtime_error("Invalid Map key request (Request info: " + request_info + ", Line: "
                                     + std::to_string(linenum) + ", File: " + filepath.string() + ")");
        }

        std::string value = component.substr(0, pos);
        std::string key = component.substr(pos + 1);

        request->AddMapValue(key, value);
    }

    return request;
}
