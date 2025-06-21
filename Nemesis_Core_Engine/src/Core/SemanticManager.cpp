#include "Core/SemanticManager.h"

#include "Core/Template.h"

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

bool nemesis::SemanticManager::HasRequestInQueue(const std::string& group) const noexcept
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

bool nemesis::SemanticManager::TryAddRequestMapToQueue(const std::string& templt_code, const std::string& key)
{
    auto& queue = RequestMapQueue[templt_code];

    for (auto& k : queue)
    {
        if (k == key) return false;
    }

    queue.emplace_back(key);
    return true;
}

bool nemesis::SemanticManager::HasRequestMapInQueue(const std::string& templt_code,
                                                    const std::string& key) const noexcept
{
    auto itr = RequestMapQueue.find(templt_code);

    if (itr == RequestMapQueue.end()) return false;

    for (auto& k : itr->second)
    {
        if (k == key) return true;
    }

    return false;
}

bool nemesis::SemanticManager::TryRemoveLastRequestMap(const std::string& templt_code) noexcept
{
    auto itr = RequestMapQueue.find(templt_code);

    if (itr == RequestMapQueue.end()) return false;

    if (itr->second.empty()) return false;

    itr->second.pop_back();
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

bool nemesis::SemanticManager::HasMapInQueue(const std::string& key) const noexcept
{
    for (auto& k : MapQueue)
    {
        if (k == key) return true;
    }

    return false;
}

bool nemesis::SemanticManager::TryRemoveLastMap() noexcept
{
    if (MapQueue.empty()) return false;

    MapQueue.pop_back();
    return true;
}

void nemesis::SemanticManager::AddRequestMotionDataToQueue(const std::string& templt_code) noexcept
{
    ++RequestMotionQueue[templt_code];
}

bool nemesis::SemanticManager::HasRequestMotionDataInQueue(const std::string& templt_code) const noexcept
{
    auto itr = RequestMotionQueue.find(templt_code);

    if (itr == RequestMotionQueue.end()) return false;

    return itr->second > 0;
}

bool nemesis::SemanticManager::TryRemoveLastRequestMotionData(const std::string& templt_code) noexcept
{
    auto itr = RequestMotionQueue.find(templt_code);

    if (itr == RequestMotionQueue.end()) return false;

    if (itr->second == 0)
    {
        RequestMotionQueue.erase(itr);
        return false;
    }

    if (--itr->second == 0)
    {
        RequestMotionQueue.erase(itr);
    }

    return true;
}

void nemesis::SemanticManager::AddMotionDataToQueue() noexcept
{
    ++MotionQueue;
}

bool nemesis::SemanticManager::HasMotionDataInQueue() const noexcept
{
    return MotionQueue > 0;
}

bool nemesis::SemanticManager::TryRemoveLastMotionData() noexcept
{
    if (MotionQueue == 0) return false;

    --MotionQueue;
    return true;
}

void nemesis::SemanticManager::AddRequestRotationDataToQueue(const std::string& templt_code) noexcept
{
    ++RequestRotationQueue[templt_code];
}

bool nemesis::SemanticManager::HasRequestRotationDataInQueue(const std::string& templt_code) const noexcept
{
    auto itr = RequestRotationQueue.find(templt_code);

    if (itr == RequestRotationQueue.end()) return false;

    return itr->second > 0;
}

bool nemesis::SemanticManager::TryRemoveLastRequestRotationData(const std::string& templt_code) noexcept
{
    auto itr = RequestRotationQueue.find(templt_code);

    if (itr == RequestRotationQueue.end()) return false;

    if (itr->second == 0)
    {
        RequestRotationQueue.erase(itr);
        return false;
    }

    if (--itr->second == 0)
    {
        RequestRotationQueue.erase(itr);
    }

    return true;
}

void nemesis::SemanticManager::AddRotationDataToQueue() noexcept
{
    ++RotationQueue;
}

bool nemesis::SemanticManager::HasRotationDataInQueue() const noexcept
{
    return RotationQueue > 0;
}

bool nemesis::SemanticManager::TryRemoveLastRotationData() noexcept
{
    if (RotationQueue == 0) return false;

    --RotationQueue;
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

bool nemesis::SemanticManager::HasForEachInQueue(const std::string& expression) const noexcept
{
    for (auto& each : ForEachQueue)
    {
        if (each == expression) return true;
    }

    return false;
}

void nemesis::SemanticManager::RemoveTopForEachFromQueue(const std::string& expression) noexcept
{
    ForEachQueue.pop_back();
}

size_t nemesis::SemanticManager::GetForEachQueueSize() const noexcept
{
    return ForEachQueue.size();
}

void nemesis::SemanticManager::ClearCacheInFileScope()
{
    FileScopeCache.clear();
}
