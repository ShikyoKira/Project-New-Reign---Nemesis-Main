#include "core/CompileState.h"
#include "core/AnimationRequest.h"
#include "core/AnimationRequestRepository.h"

nemesis::CompileState::CompileState(const nemesis::AnimationRequestRepository& repository) noexcept
    : Repository(repository)
{
}

nemesis::CompileState::CompileState(nemesis::CompileState& state) noexcept
    : Repository(state.Repository)
{
}

void nemesis::CompileState::SetBaseRequest(const nemesis::AnimationRequest* request)
{
    BaseRequest = request;
    CurrentRequest.clear();

    auto& parents = request->GetParents();
    auto& name    = request->GetTemplateName();

    QueueCurrentRequest(name + "_" + std::to_string(parents.size() + 1), request);

    for (size_t i = 0; i < parents.size(); ++i)
    {
        QueueCurrentRequest(name + "_" + std::to_string(i + 1), parents[i]);
    }
}

const nemesis::AnimationRequest* nemesis::CompileState::GetBaseRequest() const noexcept
{
    return BaseRequest;
}

const nemesis::AnimationRequest* nemesis::CompileState::GetFirstRequest(const std::string& group) const
{
    return GetChildRequestList(group).front();
}

const nemesis::AnimationRequest* nemesis::CompileState::GetLastRequest(const std::string& group) const
{
    return GetChildRequestList(group).back();
}

const nemesis::AnimationRequest* nemesis::CompileState::GetNextRequest(const std::string& group) const
{
    auto request = GetCurrentRequest(group);
    auto parents = request->GetParents();
    size_t index = request->GetIndex() + 1;

    if (!parents.empty())
    {
        auto requests = parents.back()->GetRequests();

        if (index >= requests.size()) return nullptr;

        return requests[index];
    }

    auto& name     = request->GetTemplateName();
    auto& requests = GetRequests(name);

    if (index >= requests.size()) return nullptr;

    return requests[index].get();
}

const nemesis::AnimationRequest* nemesis::CompileState::GetBackRequest(const std::string& group) const
{
    auto request = GetCurrentRequest(group);
    size_t index = request->GetIndex() - 1;

    if (index < 0) return nullptr;

    auto parents = request->GetParents();

    if (!parents.empty())
    {
        auto requests = parents.back()->GetRequests();
        return requests[index];
    }

    auto& name     = request->GetTemplateName();
    auto& requests = GetRequests(name);
    return requests[index].get();
}

void nemesis::CompileState::QueueCurrentRequest(const std::string& group,
                                                const nemesis::AnimationRequest* request)
{
    CurrentRequest[group].emplace_back(request);
    auto requests = request->GetRequests();
    auto parents  = request->GetParents();
    auto& name    = request->GetTemplateName();

    if (parents.empty())
    {
        std::string cur_group = name + "_1";
        auto& collection      = GetRequests(name);
        Vec<const nemesis::AnimationRequest*> anim_requests;
        
        for (auto& each : collection)
        {
            anim_requests.emplace_back(each.get());
        }

        QueueChildRequestList(cur_group, anim_requests);
    }
    else
    {
        auto parent = parents.back();
        std::string cur_group = name + "_" + std::to_string(parents.size() + 1);
        QueueChildRequestList(cur_group, parent->GetRequests());
    }

    if (requests.empty()) return;

    std::string child_group = name + "_" + std::to_string(parents.size() + 2);
    QueueChildRequestList(child_group, requests);
}

void nemesis::CompileState::DequeCurrentRequest(const std::string& group)
{
    auto request  = CurrentRequest[group].back();
    auto requests = request->GetRequests();
    auto parents  = request->GetParents();
    auto& name    = request->GetTemplateName();

    std::string cur_group = name + "_" + std::to_string(parents.size() + 1);
    DequeChildRequestList(cur_group);

    if (!requests.empty())
    {
        std::string child_group = name + "_" + std::to_string(parents.size() + 2);
        DequeChildRequestList(child_group);
    }

    if (CurrentRequest[group].size() == 1)
    {
        CurrentRequest.erase(group);
        return;
    }

    CurrentRequest[group].pop_back();
}

const nemesis::AnimationRequest* nemesis::CompileState::GetCurrentRequest(const std::string& group) const
{
    auto itr = CurrentRequest.find(group);

    if (itr != CurrentRequest.end()) return itr->second.back();

    throw std::runtime_error("Syntax error: Request group does not exist in current context");
}

void nemesis::CompileState::QueueChildRequestList(const std::string& group,
                                                  const Vec<const nemesis::AnimationRequest*>& requests)
{
    ChildRequestList[group].emplace_back(requests);
}

void nemesis::CompileState::DequeChildRequestList(const std::string& group)
{
    if (ChildRequestList[group].size() == 1)
    {
        ChildRequestList.erase(group);
        return;
    }

    ChildRequestList[group].pop_back();
}

const Vec<const nemesis::AnimationRequest*>&
nemesis::CompileState::GetChildRequestList(const std::string& group) const
{
    auto itr = ChildRequestList.find(group);

    if (itr != ChildRequestList.end()) return itr->second.back();

    throw std::runtime_error("Syntax error: Child request group does not exist in current context");
}

void nemesis::CompileState::QueueCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                                        const std::string& key,
                                                        const std::string& value)
{
    CurrentRequestMap[request][key].emplace_back(&value);
}

void nemesis::CompileState::DequeCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                                        const std::string& key)
{
    auto r_itr = CurrentRequestMap.find(request);
    auto itr   = r_itr->second.find(key);

    if (itr->second.size() != 1)
    {
        itr->second.pop_back();
        return;
    }

    r_itr->second.erase(itr);

    if (!r_itr->second.empty()) return;

    CurrentRequestMap.erase(r_itr);
}

const std::string* nemesis::CompileState::GetCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                                                    const std::string& key) const
{
    auto itr = CurrentRequestMap.find(request);

    if (itr == CurrentRequestMap.end()) return nullptr;

    auto k_itr = itr->second.find(key);
    return k_itr == itr->second.end() ? nullptr : k_itr->second.back();
}

void nemesis::CompileState::QueueCurrentMapValue(const std::string& key, const std::string& value)
{
    CurrentMap[key].emplace_back(&value);
}

void nemesis::CompileState::DequeCurrentMapValue(const std::string& key)
{
    auto itr = CurrentMap.find(key);

    if (itr->second.size() == 1)
    {
        CurrentMap.erase(itr);
        return;
    }

    itr->second.pop_back();
}

const std::string* nemesis::CompileState::GetCurrentMapValue(const std::string& key) const
{
    auto itr = CurrentMap.find(key);
    return itr == CurrentMap.end() ? nullptr : itr->second.back();
}

const nemesis::AnimationRequestCollection&
nemesis::CompileState::GetRequests(const std::string& template_name) const
{
    return Repository.GetRequests(template_name);
}

void nemesis::CompileState::QueueRequestOption(const nemesis::AnimationRequest* request,
                                               const std::string& option_name,
                                               const nemesis::TemplateOption* option)
{
    CurrentRequestOption[request][option_name].emplace_back(option);
}

void nemesis::CompileState::DequeueRequestOption(const nemesis::AnimationRequest* request,
                                                 const std::string& option_name)
{
    auto r_itr = CurrentRequestOption.find(request);
    auto itr   = r_itr->second.find(option_name);

    if (itr->second.size() != 1)
    {
        itr->second.pop_back();
        return;
    }

    r_itr->second.erase(itr);

    if (!r_itr->second.empty()) return;

    CurrentRequestOption.erase(r_itr);
}

const nemesis::TemplateOption*
nemesis::CompileState::GetCurrentRequestOption(const nemesis::AnimationRequest* request,
                                               const std::string& option_name) const
{
    auto itr = CurrentRequestOption.find(request);

    if (itr == CurrentRequestOption.end()) return nullptr;

    auto k_itr = itr->second.find(option_name);
    return k_itr == itr->second.end() ? nullptr : k_itr->second.back();
}

void nemesis::CompileState::QueueOption(const std::string& option_name, const nemesis::TemplateOption* option)
{
    CurrentOption[option_name].emplace_back(option);
}

void nemesis::CompileState::DequeueOption(const std::string& option_name)
{
    auto itr = CurrentOption.find(option_name);

    if (itr->second.size() == 1)
    {
        CurrentOption.erase(itr);
        return;
    }

    itr->second.pop_back();
}

const nemesis::TemplateOption* nemesis::CompileState::GetCurrentOption(const std::string& option_name) const
{
    auto itr = CurrentOption.find(option_name);
    return itr == CurrentOption.end() ? nullptr : itr->second.back();
}

uintptr_t nemesis::CompileState::InsertAddLineHandler(std::function<void(nemesis::Line&)> event)
{
    auto func_ptr = std::make_unique<std::function<void(nemesis::Line&, const nemesis::NObject&)>>(
        [event](nemesis::Line& line, const nemesis::NObject& nobject) { event(line); });
    return reinterpret_cast<uintptr_t>(AddLineEvents.emplace_back(std::move(func_ptr)).get());
}

uintptr_t nemesis::CompileState::InsertAddLineHandler(
    std::function<void(nemesis::Line&, const nemesis::NObject&)> event)
{
    auto func_ptr = std::make_unique<std::function<void(nemesis::Line&, const nemesis::NObject&)>>(
        [event](nemesis::Line& line, const nemesis::NObject& nobject) { event(line, nobject); });
    return reinterpret_cast<uintptr_t>(AddLineEvents.emplace_back(std::move(func_ptr)).get());
}

void nemesis::CompileState::RemoveAddLineHandler(uintptr_t handler_address)
{
    for (size_t i = 0; i < AddLineEvents.size(); ++i)
    {
        if (reinterpret_cast<uintptr_t>(AddLineEvents[i].get()) != handler_address) continue;

        AddLineEvents.erase(AddLineEvents.begin() + i);
        return;
    }

    throw std::runtime_error("Invalid handler address");
}

void nemesis::CompileState::RaiseAddLineEvent(nemesis::Line& line, const nemesis::NObject& nobject) const
{
    Vec<std::function<void(nemesis::Line&, const nemesis::NObject&)>*> event_pointers; 

    for (auto& event : AddLineEvents)
    {
        event_pointers.emplace_back(event.get());
    }

    for (auto& event_ptr : event_pointers)
    {
        (*event_ptr)(line, nobject);
    }
}

uintptr_t nemesis::CompileState::InsertEOFHandler(std::function<void()> event)
{
    auto func_ptr = std::make_unique<std::function<void()>>([event]() { event(); });
    return reinterpret_cast<uintptr_t>(EOFEvents.emplace_back(std::move(func_ptr)).get());
}

void nemesis::CompileState::RemoveEOFHandler(uintptr_t handler_address)
{
    for (size_t i = 0; i < EOFEvents.size(); ++i)
    {
        if (reinterpret_cast<uintptr_t>(EOFEvents[i].get()) != handler_address) continue;

        EOFEvents.erase(EOFEvents.begin() + i);
        return;
    }

    throw std::runtime_error("Invalid handler address");
}

void nemesis::CompileState::RaiseEOFEvent() const
{
    Vec<std::function<void()>*> event_pointers;

    for (auto& event : EOFEvents)
    {
        event_pointers.emplace_back(event.get());
    }

    for (auto& event_ptr : event_pointers)
    {
        (*event_ptr)();
    }
}

void nemesis::CompileState::SelectMod(const std::string& modcode)
{
    SelectedMods.emplace_back(modcode);
}

void nemesis::CompileState::DeselectMod(const std::string& modcode)
{
    auto start = SelectedMods.begin();

    for (size_t i = 0; i < SelectedMods.size(); ++i)
    {
        if (SelectedMods[i] != modcode) continue;

        SelectedMods.erase(start + i);
    }
}

bool nemesis::CompileState::IsModSelected(const std::string& modcode) const
{
    for (size_t i = 0; i < SelectedMods.size(); ++i)
    {
        if (modcode == SelectedMods[i]) return true;
    }

    return false;
}

bool nemesis::CompileState::IsModSelected(const std::string_view& modcode) const
{
    for (size_t i = 0; i < SelectedMods.size(); ++i)
    {
        if (modcode == SelectedMods[i]) return true;
    }

    return false;
}

const VecStr& nemesis::CompileState::GetSelectedMods() const noexcept
{
    return SelectedMods;
}

void nemesis::CompileState::AddSubTemplateRequest(const VecStr& arguments)
{
    if (SubTemplateRequests.find(arguments.front()) != SubTemplateRequests.end()) return;

    SubTemplateRequests.insert_or_assign(arguments.front(), SubTemplateRequest(arguments));
}

void nemesis::CompileState::RemoveSubTemplateRequest(const std::string& expression)
{
    SubTemplateRequests.erase(expression);
}

Vec<const nemesis::SubTemplateRequest*> nemesis::CompileState::GetSubTemplateRequestList() const
{
    Vec<const nemesis::SubTemplateRequest*> requests;

    for (auto& each : SubTemplateRequests)
    {
        requests.emplace_back(&each.second);
    }

    return requests;
}

void nemesis::CompileState::ClearSubTemplateRequestList()
{
    SubTemplateRequests.clear();
}

void nemesis::CompileState::SetCurrentSubTemplateRequest(const nemesis::SubTemplateRequest& request) noexcept
{
    CurrentSubTemplateRequest = &request;
}

const nemesis::SubTemplateRequest* nemesis::CompileState::GetCurrentSubTemplateRequest() const noexcept
{
    return CurrentSubTemplateRequest;
}

const std::string& nemesis::CompileState::GetEventID(const std::string& name) const
{
    auto itr = EventMap.find(name);

    if (itr != EventMap.end()) return itr->second;

    throw std::runtime_error("Event id not found (name: " + name + ")");
}

const std::string& nemesis::CompileState::AddEventName(const std::string& name)
{
    return EventMap[name] = std::to_string(EventMap.size());
}

const std::string& nemesis::CompileState::GetVariableID(const std::string& name)
{
    auto itr = VariableMap.find(name);

    if (itr != VariableMap.end()) return itr->second;

    throw std::runtime_error("Variable id not found (name: " + name + ")");
}

const std::string& nemesis::CompileState::AddVariableName(const std::string& name)
{
    return VariableMap[name] = std::to_string(VariableMap.size());
}

const std::string& nemesis::CompileState::GetAttributeID(const std::string& name) const
{
    auto itr = AttributeMap.find(name);

    if (itr != AttributeMap.end()) return itr->second;

    throw std::runtime_error("Attribute id not found (name: " + name + ")");
}

const std::string& nemesis::CompileState::AddAttributeName(const std::string& name)
{
    return AttributeMap[name] = std::to_string(AttributeMap.size());
}

const std::string& nemesis::CompileState::GetPropertyID(const std::string& name) const
{
    auto itr = PropertyMap.find(name);

    if (itr != PropertyMap.end()) return itr->second;

    throw std::runtime_error("Property id not found (name: " + name + ")");
}

const std::string& nemesis::CompileState::AddPropertyName(const std::string& name)
{
    return PropertyMap[name] = std::to_string(PropertyMap.size());
}

const std::string& nemesis::CompileState::GetStateID(const std::string& unique_key)
{
    auto itr = StateMap.find(unique_key);

    if (itr != StateMap.end()) return itr->second;

    return StateMap[unique_key] = std::to_string(StateRunningID++);
}

int* nemesis::CompileState::GetCounter(const std::string& name) const
{
    auto itr = CounterMap.find(name);

    if (itr != CounterMap.end()) return itr->second;

    throw std::runtime_error("Element counter name not found (name: " + name + ")");
}

void nemesis::CompileState::AddCounter(const std::string& name, int* counter)
{
    CounterMap[name] = counter;
}

void nemesis::CompileState::RemoveCounter(const std::string& name)
{
    CounterMap.erase(name);
}

void nemesis::CompileState::CacheConditionResult(const std::string& condition_syntax, bool result)
{
    ConditionCache[condition_syntax] = result;
}

Vec<Pair<std::string, bool>> nemesis::CompileState::RemoveConditionCacheContaining(const std::string& syntax)
{
    Vec<Pair<std::string, bool>> removing_list;

    for (auto& each : ConditionCache)
    {
        if (each.first.find(syntax) == NOT_FOUND) continue;

        removing_list.emplace_back(each.first, each.second);
    }

    for (auto& each : removing_list)
    {
        ConditionCache.erase(each.first);
    }

    return removing_list;
}

void nemesis::CompileState::ClearAllConditionCache()
{
    ConditionCache.clear();
}

const bool* nemesis::CompileState::TryGetCacheConditionResult(const std::string& condition_syntax)
{
    auto itr = ConditionCache.find(condition_syntax);

    if (itr != ConditionCache.end()) return &itr->second;

    return nullptr;
}
