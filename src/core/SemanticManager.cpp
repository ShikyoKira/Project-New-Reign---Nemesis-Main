#include "core/SemanticManager.h"

#include "core/Template/TemplateClass.h"

void nemesis::SemanticManager::SetCurrentTemplateClass(const nemesis::TemplateClass* template_class) noexcept
{
    TemplateClass = template_class;
}

const nemesis::TemplateClass* nemesis::SemanticManager::GetCurrentTemplateClass() const noexcept
{
    if (TemplateClass) return TemplateClass;

    if (!TemplateObject) return nullptr;

    return TemplateObject->GetTemplateClass();
}

void nemesis::SemanticManager::SetCurrentTemplate(const nemesis::TemplateObject* template_object) noexcept
{
    TemplateObject = template_object;
}

const nemesis::TemplateObject* nemesis::SemanticManager::GetCurrentTemplate() const noexcept
{
    return TemplateObject;
}

bool nemesis::SemanticManager::TryAddOptionToQueue(const std::string& option_name,
                                                   const std::string& option_syntax)
{
    auto template_class = GetCurrentTemplateClass();

    if (!template_class->GetModel(option_name)) return false;

    if (HasOptionInQueue(option_syntax)) return false;

    OptionQueue.emplace_back(option_syntax);
    return true;
}

bool nemesis::SemanticManager::HasOptionInQueue(const std::string& option_syntax) const
{
    for (auto& name : OptionQueue)
    {
        if (name == option_syntax) return true;
    }

    return false;
}

bool nemesis::SemanticManager::TryRemoveLastOption() noexcept
{
    if (OptionQueue.empty()) return false;

    OptionQueue.pop_back();
    return true;
}

bool nemesis::SemanticManager::TryAddRequestToQueue(const std::string& group)
{
    for (auto& grp : RequestQueue)
    {
        if (grp == group) return false;
    }

    RequestQueue.emplace_back(group);
    return true;
}

bool nemesis::SemanticManager::HasRequestInQueue(const std::string& group) const
{
    for (auto& grp : RequestQueue)
    {
        if (grp == group) return true;
    }

    return false;
}

bool nemesis::SemanticManager::TryRemoveLastRequest() noexcept
{
    if (RequestQueue.empty()) return false;

    RequestQueue.pop_back();
    return true;
}

bool nemesis::SemanticManager::TryAddMapToQueue(const std::string& key)
{
    for (auto& k : MapQueue)
    {
        if (k == key) return false;
    }

    MapQueue.emplace_back(key);
    return true;
}

bool nemesis::SemanticManager::HasMapInQueue(const std::string& key) const
{
    for (auto& k : MapQueue)
    {
        if (k == key) return true;
    }

    return false;
}

bool nemesis::SemanticManager::TryRemoveLastMap(const std::string& key)
{
    if (MapQueue.empty()) return false;

    MapQueue.pop_back();
    return true;
}

void nemesis::SemanticManager::AddModInUsed(const std::string& mod_code)
{
    ModInUsedList.insert(mod_code);
}

const USetStr& nemesis::SemanticManager::GetModInUsedList() const noexcept
{
    return ModInUsedList;
}

void nemesis::SemanticManager::AddForEachToQueue(const std::string& expression)
{
    ForEachQueue.emplace_back(expression);
}

bool nemesis::SemanticManager::HasForEachInQueue(const std::string& expression) const
{
    for (auto& each : ForEachQueue)
    {
        if (each == expression) return true;
    }

    return false;
}

void nemesis::SemanticManager::RemoveTopForEachFromQueue(const std::string& expression)
{
    ForEachQueue.pop_back();
}

size_t nemesis::SemanticManager::GetForEachQueueSize() const
{
    return ForEachQueue.size();
}
