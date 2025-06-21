#pragma once

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    struct TemplateClass;
    struct TemplateObject;

	struct SemanticManager
    {
    private:
        const nemesis::TemplateClass* TemplateClass = nullptr;
        const nemesis::TemplateObject* TemplateObject = nullptr;

        VecStr OptionQueue;
        VecStr RequestQueue;
        VecStr MapQueue;
        VecStr ForEachQueue;

        UMap<std::string, VecStr> RequestMapQueue;
        UMap<std::string, size_t> RequestMotionQueue;
        UMap<std::string, size_t> RequestRotationQueue;

        UMap<std::string, SPtr<void>> FileScopeCache;
        UMap<std::string, SPtr<void>> ForEachScopeCache;

        USetStr ModInUsedList;

        size_t MotionQueue = 0;
        size_t RotationQueue = 0;

    public:
        void SetCurrentTemplateClass(const nemesis::TemplateClass* template_class) noexcept;
        const nemesis::TemplateClass* GetCurrentTemplateClass() const noexcept;

        void SetCurrentTemplate(const nemesis::TemplateObject* template_object) noexcept;
        const nemesis::TemplateObject* GetCurrentTemplate() const noexcept;

        bool TryAddOptionToQueue(const std::string& option_name, const std::string& option_syntax);
        bool HasOptionInQueue(const std::string& option_syntax) const;
        bool TryRemoveLastOption() noexcept;

        bool TryAddRequestToQueue(const std::string& group);
        bool HasRequestInQueue(const std::string& group) const noexcept;
        bool TryRemoveLastRequest() noexcept;

        bool TryAddRequestMapToQueue(const std::string& templt_code, const std::string& key);
        bool HasRequestMapInQueue(const std::string& templt_code, const std::string& key) const noexcept;
        bool TryRemoveLastRequestMap(const std::string& templt_code) noexcept;
        
        bool TryAddMapToQueue(const std::string& key);
        bool HasMapInQueue(const std::string& key) const noexcept;
        bool TryRemoveLastMap() noexcept;

        void AddRequestMotionDataToQueue(const std::string& templt_code) noexcept;
        bool HasRequestMotionDataInQueue(const std::string& templt_code) const noexcept;
        bool TryRemoveLastRequestMotionData(const std::string& templt_code) noexcept;
        
        void AddMotionDataToQueue() noexcept;
        bool HasMotionDataInQueue() const noexcept;
        bool TryRemoveLastMotionData() noexcept;
        
        void AddRequestRotationDataToQueue(const std::string& templt_code) noexcept;
        bool HasRequestRotationDataInQueue(const std::string& templt_code) const noexcept;
        bool TryRemoveLastRequestRotationData(const std::string& templt_code) noexcept;
        
        void AddRotationDataToQueue() noexcept;
        bool HasRotationDataInQueue() const noexcept;
        bool TryRemoveLastRotationData() noexcept;
        
        void AddModInUsed(const std::string& mod_code);
        const USetStr& GetModInUsedList() const noexcept;

        void AddForEachToQueue(const std::string& expression);
        bool HasForEachInQueue(const std::string& expression) const noexcept;
        void RemoveTopForEachFromQueue(const std::string& expression) noexcept;
        size_t GetForEachQueueSize() const noexcept;

        template <typename Ty>
        void AddCacheInFileScope(const std::string& key, SPtr<Ty> obj)
        {
            FileScopeCache[key] = obj;
        }

        template<typename Ty>
        Ty* GetCacheFromFileScope(const std::string& key)
        {
            auto itr = FileScopeCache.find(key);

            if (itr == FileScopeCache.end()) return nullptr;

            return dynamic_cast<Ty*>(itr->second.get());
        }

        void ClearCacheInFileScope();
    };
}
