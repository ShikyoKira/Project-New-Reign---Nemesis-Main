#include "core/AnimationRequest.h"

nemesis::AnimationRequest::AnimationRequest(const std::string& template_name,
                                            size_t index,
                                            bool support_array) noexcept
    : TemplateName(template_name)
    , Index(index)
    , SupportArray(support_array)
{
}

size_t nemesis::AnimationRequest::GetId() const noexcept
{
    return reinterpret_cast<size_t>(this);
}

size_t nemesis::AnimationRequest::GetIndex() const noexcept
{
    return Index;
}

size_t nemesis::AnimationRequest::GetLevel() const noexcept
{
    return Parents.size() + 1;
}

const std::string& nemesis::AnimationRequest::GetTemplateName() const noexcept
{
    return TemplateName;
}

const std::string& nemesis::AnimationRequest::GetAnimationEvent() const noexcept
{
    return AnimationEvent;
}

void nemesis::AnimationRequest::SetAnimationEvent(const std::string& animation_event) noexcept 
{
    AnimationEvent = animation_event;
}

const std::filesystem::path& nemesis::AnimationRequest::GetAnimationFilePath() const noexcept
{
    return AnimationFilePath;
}

void nemesis::AnimationRequest::SetAnimationFilePath(const std::string& filepath) noexcept
{
    AnimationFilePath = filepath;
}

const nemesis::TemplateOption* nemesis::AnimationRequest::GetOption(const std::string& option_name) const
{
    for (size_t i = 0; i < Options.size(); i++)
    {
        if (Options[i]->GetName() != option_name) continue;

        if (!Options[i]->HasAlias(option_name)) continue;

        return Options[i].get();
    }

    return nullptr;
}

const Vec<const nemesis::TemplateOption*>&
nemesis::AnimationRequest::GetOptions(const std::string& option_name) const
{
    auto itr = OptionsCacheMap.find(option_name);

    if (itr != OptionsCacheMap.end()) return itr->second;

    static Vec<const nemesis::TemplateOption*> proxy;
    return proxy;
}

const Vec<const nemesis::TemplateOption*>& nemesis::AnimationRequest::GetOptions() const noexcept
{
    return OptionsCache;
}

void nemesis::AnimationRequest::AddOption(UPtr<nemesis::TemplateOption>&& option)
{
    OptionsCache.emplace_back(option.get());
    OptionsCacheMap[option->GetName()].emplace_back(option.get());

    auto& children = option->GetChildren();

    for (auto& child : children)
    {
        OptionsCacheMap[child].emplace_back(option.get());
    }

    Options.emplace_back(std::move(option));
}

Vec<const std::string*> nemesis::AnimationRequest::GetMapValueList(const std::string& key) const
{
    Vec<const std::string*> values;

    auto itr = StringListMap.find(key);

    if (itr == StringListMap.end()) return values;

    for (auto& each : itr->second)
    {
        values.emplace_back(&each);
    }

    return values;
}

void nemesis::AnimationRequest::AddMapValue(const std::string& key, const std::string& value)
{
    StringListMap[key].emplace_back(value);
}

Vec<const nemesis::AnimationRequest*> nemesis::AnimationRequest::GetRequests() const noexcept
{
    Vec<const nemesis::AnimationRequest*> requests;
    requests.reserve(Requests.size());

    for (size_t i = 0; i < Requests.size(); i++)
    {
        requests.emplace_back(Requests[i].get());
    }

    return requests;
}

void nemesis::AnimationRequest::AddRequest(UPtr<nemesis::AnimationRequest>&& request)
{
    request->Parents = Parents;
    request->Parents.emplace_back(this);
    Requests.emplace_back(std::move(request));
}

const Vec<nemesis::AnimationRequest*>& nemesis::AnimationRequest::GetParents() const noexcept
{
    return Parents;
}
