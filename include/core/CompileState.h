#pragma once

#include "core/SubTemplateRequest.h"
#include "core/AnimationRequestRepository.h"

namespace nemesis
{
    struct NObject;

	struct CompileState
    {
    private:
        VecStr SelectedMods;

        const nemesis::AnimationRequest* BaseRequest;
        UMap<std::string, Vec<const nemesis::AnimationRequest*>> CurrentRequest;
        UMap<std::string, Vec<Vec<const nemesis::AnimationRequest*>>> ChildRequestList;
        const nemesis::AnimationRequestRepository& Repository;

        UMap<const nemesis::AnimationRequest*, UMap<std::string, Vec<const nemesis::TemplateOption*>>>
            CurrentRequestOption;
        UMap<std::string, Vec<const nemesis::TemplateOption*>> CurrentOption;

        UMap<const nemesis::AnimationRequest*, UMap<std::string, Vec<const std::string*>>> CurrentRequestMap;
        UMap<std::string, Vec<const std::string*>> CurrentMap;

        Vec<UPtr<std::function<void(nemesis::Line&, const nemesis::NObject&)>>> AddLineEvents;
        Vec<UPtr<std::function<void()>>> EOFEvents;

        Map<std::string, nemesis::SubTemplateRequest> SubTemplateRequests;

        const nemesis::SubTemplateRequest* CurrentSubTemplateRequest;

        UMap<std::string, std::string> EventMap;
        UMap<std::string, std::string> VariableMap;
        UMap<std::string, std::string> AttributeMap;
        UMap<std::string, std::string> PropertyMap;
        UMap<std::string, std::string> StateMap;

        UMap<std::string, int*> CounterMap;

        UMap<std::string, bool> ConditionCache;

        int StateRunningID = -1852516352;

    public:
        CompileState(const nemesis::AnimationRequestRepository& repository) noexcept;
        CompileState(nemesis::CompileState& state) noexcept;

        void SetBaseRequest(const nemesis::AnimationRequest* request);
        const nemesis::AnimationRequest* GetBaseRequest() const noexcept;
        const nemesis::AnimationRequest* GetFirstRequest(const std::string& group) const;
        const nemesis::AnimationRequest* GetLastRequest(const std::string& group) const;
        const nemesis::AnimationRequest* GetNextRequest(const std::string& group) const;
        const nemesis::AnimationRequest* GetBackRequest(const std::string& group) const;

        void QueueCurrentRequest(const std::string& group, const nemesis::AnimationRequest* request);
        void DequeCurrentRequest(const std::string& group);
        const nemesis::AnimationRequest* GetCurrentRequest(const std::string& group) const;

        void QueueChildRequestList(const std::string& group, const Vec<const nemesis::AnimationRequest*>& requests);
        void DequeChildRequestList(const std::string& group);
        const Vec<const nemesis::AnimationRequest*>& GetChildRequestList(const std::string& group) const;

        void QueueCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                         const std::string& key,
                                         const std::string& value);
        void DequeCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                         const std::string& key);
        const std::string* GetCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                                     const std::string& key) const;

        void QueueCurrentMapValue(const std::string& key, const std::string& value);
        void DequeCurrentMapValue(const std::string& key);
        const std::string* GetCurrentMapValue(const std::string& key) const;

        const nemesis::AnimationRequestCollection& GetRequests(const std::string& template_name) const;

        void QueueRequestOption(const nemesis::AnimationRequest* request,
                                const std::string& option_name,
                                const nemesis::TemplateOption* option);
        void DequeueRequestOption(const nemesis::AnimationRequest* request,
                                  const std::string& option_name);
        const nemesis::TemplateOption* GetCurrentRequestOption(const nemesis::AnimationRequest* request,
                                                               const std::string& option_name) const;
        
        void QueueOption(const std::string& option_name, const nemesis::TemplateOption* option);
        void DequeueOption(const std::string& option_name);
        const nemesis::TemplateOption* GetCurrentOption(const std::string& option_name) const;

        uintptr_t InsertAddLineHandler(std::function<void(nemesis::Line&)> event);
        uintptr_t InsertAddLineHandler(std::function<void(nemesis::Line&, const nemesis::NObject&)> event);
        void RemoveAddLineHandler(uintptr_t handler_address);
        void RaiseAddLineEvent(nemesis::Line& line, const nemesis::NObject& nobject) const;

        uintptr_t InsertEOFHandler(std::function<void()> event);
        void RemoveEOFHandler(uintptr_t handler_address);
        void RaiseEOFEvent() const;

        void SelectMod(const std::string& modcode);
        void DeselectMod(const std::string& modcode);

        bool IsModSelected(const std::string& modcode) const;
        bool IsModSelected(const std::string_view& modcode) const;
        const VecStr& GetSelectedMods() const noexcept;

        void AddSubTemplateRequest(const VecStr& arguments);
        void RemoveSubTemplateRequest(const std::string& expression);

        Vec<const nemesis::SubTemplateRequest*> GetSubTemplateRequestList() const;
        void ClearSubTemplateRequestList();

        void SetCurrentSubTemplateRequest(const nemesis::SubTemplateRequest& request) noexcept;
        const nemesis::SubTemplateRequest* GetCurrentSubTemplateRequest() const noexcept;

        const std::string& GetEventID(const std::string& name) const;
        const std::string& AddEventName(const std::string& name);

        const std::string& GetVariableID(const std::string& name);
        const std::string& AddVariableName(const std::string& name);
        
        const std::string& GetAttributeID(const std::string& name) const;
        const std::string& AddAttributeName(const std::string& name);

        const std::string& GetPropertyID(const std::string& name) const;
        const std::string& AddPropertyName(const std::string& name);

        const std::string& GetStateID(const std::string& unique_key);

        int* GetCounter(const std::string& name) const;
        void AddCounter(const std::string& name, int* counter);
        void RemoveCounter(const std::string& name);

        void CacheConditionResult(const std::string& condition_syntax, bool result);
        Vec<Pair<std::string, bool>> RemoveConditionCacheContaining(const std::string& syntax);
        void ClearAllConditionCache();
        const bool* TryGetCacheConditionResult(const std::string& condition_syntax);
    };
}
