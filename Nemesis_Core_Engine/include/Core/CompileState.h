#pragma once

#include <mutex>
#include <functional>

#include "Core/SubTemplateRequest.h"
#include "Core/AnimationRequestRepository.h"

#include "Core/Statement/Statement.h"

#include "ExAnimation/ExAnimationRepository.h"

#include "AlternateAnimation/AlterAnimRepository.h"

#include "Utilities/NonCopyable.h"

namespace nemesis
{
    struct NObject;
    struct CompilationManager;

	struct CompileState : public nemesis::NonCopyableStruct
    {
    private:
        const nemesis::AnimationRequest* BaseRequest = nullptr;
        UMap<std::string, Vec<const nemesis::AnimationRequest*>> CurrentRequest;
        UMap<std::string, Vec<Vec<const nemesis::AnimationRequest*>>> ChildRequestList;
        UMap<std::string, Vec<const nemesis::AnimationRequest*>> RequestListCache;

        UMap<const nemesis::AnimationRequest*, UMap<std::string, Vec<const nemesis::TemplateOption*>>>
            CurrentRequestOption;
        UMap<std::string, Vec<const nemesis::TemplateOption*>> CurrentOption;

        UMap<const nemesis::AnimationRequest*, UMap<std::string, Vec<Pair<const std::string*, size_t>>>>
            CurrentRequestMap;
        UMap<std::string, Vec<Pair<const std::string*, size_t>>> CurrentMap;

        UMap<const nemesis::AnimationRequest*, Vec<Pair<const std::string*, size_t>>>
            CurrentRequestMotionData;
        Vec<Pair<const std::string*, size_t>> CurrentMotionData;

        UMap<const nemesis::AnimationRequest*, Vec<Pair<const std::string*, size_t>>>
            CurrentRequestRotationData;
        Vec<Pair<const std::string*, size_t>> CurrentRotationData;

        Vec<SPtr<std::function<void(nemesis::Line&, const nemesis::NObject&)>>> AddLineEvents;
        Vec<SPtr<std::function<void()>>> EOFEvents;

        Map<std::string, nemesis::SubTemplateRequest> SubTemplateRequests;

        const nemesis::SubTemplateRequest* CurrentSubTemplateRequest = nullptr;

        UMap<std::string, std::string> EventMap;
        UMap<std::string, std::string> EventNameMap;
        UMap<std::string, std::string> VariableMap;
        UMap<std::string, std::string> VariableNameMap;
        UMap<std::string, std::string> AttributeMap;
        UMap<std::string, std::string> PropertyMap;
        UMap<std::string, std::string> StateMap;
        UMap<std::string, size_t> AnimationOrderMap;

        UMap<std::string, int*> CounterMap;

        UMap<std::string, bool> ConditionCache;
        Vec<UMap<const nemesis::Statement*, std::string>> StatementValueCacheList;

        nemesis::CompilationManager& Manager;

        int StateRunningID = -1852516352;

        mutable std::mutex AnimationOrderMapMutex;
        mutable std::mutex SubTemplateRequestsMutex;

        const Pair<const std::string*, size_t>& GetCurrentRequestMap(const nemesis::AnimationRequest* request,
                                                               const std::string& key) const;
        const Pair<const std::string*, size_t>& GetCurrentMap(const std::string& key) const;
        const Pair<const std::string*, size_t>&
        GetCurrentRequestMotion(const nemesis::AnimationRequest* request) const;
        const Pair<const std::string*, size_t>& GetCurrentMotion() const;
        const Pair<const std::string*, size_t>&
        GetCurrentRequestRotation(const nemesis::AnimationRequest* request) const;
        const Pair<const std::string*, size_t>& GetCurrentRotation() const;

    public:
        CompileState(nemesis::CompilationManager& manager) noexcept;

        const nemesis::CompilationManager& GetManager() const noexcept;

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
        void DequeChildRequestList(const std::string& group) noexcept;
        const Vec<const nemesis::AnimationRequest*>& GetChildRequestList(const std::string& group) const;

        void QueueCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                         const std::string& key,
                                         const std::string& value,
                                         size_t index);
        void DequeCurrentRequestMapValue(const nemesis::AnimationRequest* request, const std::string& key);
        const std::string& GetCurrentRequestMapValue(const nemesis::AnimationRequest* request,
                                                     const std::string& key) const;
        size_t GetCurrentRequestMapIndex(const nemesis::AnimationRequest* request,
                                         const std::string& key) const;

        void QueueCurrentMapValue(const std::string& key, const std::string& value, size_t index);
        void DequeCurrentMapValue(const std::string& key);
        const std::string& GetCurrentMapValue(const std::string& key) const;
        size_t GetCurrentMapIndex(const std::string& key) const;

        void QueueCurrentRequestMotionData(const nemesis::AnimationRequest* request,
                                           const std::string& value,
                                           size_t index);
        void DequeCurrentRequestMotionData(const nemesis::AnimationRequest* request) noexcept;
        const std::string& GetCurrentRequestMotionData(const nemesis::AnimationRequest* request) const;
        size_t GetCurrentRequestMotionIndex(const nemesis::AnimationRequest* request) const;

        void QueueCurrentMotionData(const std::string& value, size_t index);
        void DequeCurrentMotionData() noexcept;
        const std::string& GetCurrentMotionData() const;
        size_t GetCurrentMotionIndex() const;

        void QueueCurrentRequestRotationData(const nemesis::AnimationRequest* request,
                                             const std::string& value,
                                             size_t index);
        void DequeCurrentRequestRotationData(const nemesis::AnimationRequest* request) noexcept;
        const std::string& GetCurrentRequestRotationData(const nemesis::AnimationRequest* request) const;
        size_t GetCurrentRequestRotationIndex(const nemesis::AnimationRequest* request) const;

        void QueueCurrentRotationData(const std::string& value, size_t index);
        void DequeCurrentRotationData() noexcept;
        const std::string& GetCurrentRotationData() const;
        size_t GetCurrentRotationIndex() const;

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

        void* InsertAddLineHandler(const std::function<void(nemesis::Line&)>& event);
        void* InsertAddLineHandler(const std::function<void(nemesis::Line&, const nemesis::NObject&)>& event);
        void RemoveAddLineHandler(void* handler_address);
        void RaiseAddLineEvent(nemesis::Line& line, const nemesis::NObject& nobject) const;

        std::function<void()>* InsertEOFHandler(const std::function<void()>& event);
        void RemoveEOFHandler(std::function<void()>* handler_address);
        void RaiseEOFEvent() const;

        bool IsModSelected(const std::string& modcode) const;
        bool IsModSelected(const std::string_view& modcode) const;
        const VecStr& GetSelectedMods() const noexcept;

        void AddSubTemplateRequest(const VecStr& arguments);
        void RemoveSubTemplateRequest(const std::string& expression);

        Vec<const nemesis::SubTemplateRequest*> GetSubTemplateRequestList() const;
        void ClearSubTemplateRequestList();

        void SetCurrentSubTemplateRequest(const nemesis::SubTemplateRequest& request) noexcept;
        const nemesis::SubTemplateRequest* GetCurrentSubTemplateRequest() const noexcept;

        bool TryGetEventID(const std::string& name, std::string& out) const;
        const std::string& GetEventID(const std::string& name) const;
        bool TryGetEventName(const std::string& id, std::string& out) const;
        const std::string& GetEventName(const std::string& id) const;
        const std::string& AddEventName(const std::string& name);

        bool TryGetVariableID(const std::string& name, std::string& out) const;
        const std::string& GetVariableID(const std::string& name);
        bool TryGetVariableName(const std::string& id, std::string& out) const;
        const std::string& GetVariableName(const std::string& id) const;
        const std::string& AddVariableName(const std::string& name);
        
        bool TryGetAttributeID(const std::string& name, std::string& out) const;
        const std::string& GetAttributeID(const std::string& name) const;
        const std::string& AddAttributeName(const std::string& name);

        const std::string& GetPropertyID(const std::string& name) const;
        const std::string& AddPropertyName(const std::string& name);

        bool TryGetAnimationOrder(const std::string& name, size_t& value) const;
        size_t GetAnimationOrder(const std::filesystem::path& char_path, const std::string& anim_path) const;
        size_t AddAnimationToOrder(const std::string& name);

        const std::string& GetStateID(const std::string& unique_key);

        int* GetCounter(const std::string& name) const;
        void AddCounter(const std::string& name, int* counter);
        void RemoveCounter(const std::string& name) noexcept;

        const nemesis::AlterAnimRepository& GetAlterAnimRepository() const noexcept;
        const nemesis::ExAnimationRepository& GetExAnimationRepository() const noexcept;

        void CacheConditionResult(const std::string& condition_syntax, bool result);
        Vec<Pair<std::string, bool>> RemoveConditionCacheContaining(const std::string& syntax);
        void ClearAllConditionCache() noexcept;
        const bool* TryGetCacheConditionResult(const std::string& condition_syntax);

        void CacheStatementValue(const nemesis::Statement* statement_ptr, const std::string& value);
        const std::string* TryGetCachedStatementValue(const nemesis::Statement* statement_ptr) const;
        void PushStatementValueScope();
        void PopStatementValueScope();
        void ClearAllStatementValueCache();

        void AddCheckSum(const std::filesystem::path& target_path, const std::string& checksum);

        UPtr<nemesis::CompileState> Clone();
    };
}
