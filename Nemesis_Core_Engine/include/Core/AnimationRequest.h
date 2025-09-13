#pragma once

#include <atomic>
#include <filesystem>

#include "Core/Template/TemplateOption.h"

#include "Utilities/Line.h"

namespace nemesis
{
    struct TemplateClass;

    struct AnimationRequest
    {
    private:
        Vec<UPtr<nemesis::TemplateOption>> Options;
        Vec<const nemesis::TemplateOption*> OptionsCache;
        UMap<std::string, Vec<const nemesis::TemplateOption*>> OptionsCacheMap;

        UMap<std::string, VecStr> StringListMap;

        Vec<UPtr<nemesis::AnimationRequest>> Requests;

        size_t Id;
        mutable long Index = -1;

        std::string ListName;
        std::string AnimationEvent;
        std::filesystem::path AnimationFilePath;
        const nemesis::TemplateClass& TemplateClass;

        VecNstr MotionDataList;
        VecNstr RotationDataList;

        Vec<nemesis::AnimationRequest*> Parents;

        std::string Hash;

        static std::atomic_uint32_t IdCounter;

    public:
        AnimationRequest(const std::string& list_name, const nemesis::TemplateClass& templt_class) noexcept;
        ~AnimationRequest() noexcept;

        void SetHash(const std::string& hash);
        const std::string& GetHash() const;

        void SetIndex(size_t index) noexcept;

        const std::string& GetListName() const noexcept;

        size_t GetId() const noexcept;
        size_t GetIndex() const noexcept;
        size_t GetLevel() const noexcept;

        const std::string& GetTemplateName() const noexcept;
        const nemesis::TemplateClass& GetTemplateClass() const noexcept;

        const std::string& GetAnimationEvent() const noexcept;
        void SetAnimationEvent(const std::string& animation_event) noexcept;

        const std::filesystem::path& GetAnimationFilePath() const noexcept;
        void SetAnimationFilePath(const std::string& filepath) noexcept;

        const VecNstr GetMotionDataList() const noexcept;
        nemesis::Line& SetMotionData(const nemesis::Line& line);

        const VecNstr GetRotationDataList() const noexcept;
        nemesis::Line& SetRotationData(const nemesis::Line& line);

        const nemesis::TemplateOption* GetOption(const std::string& option_name) const;
        const Vec<const nemesis::TemplateOption*>& GetOptions(const std::string& option_name) const;
        const Vec<const nemesis::TemplateOption*>& GetOptions() const noexcept;
        void AddOption(UPtr<nemesis::TemplateOption>&& option);

        Vec<const std::string*> GetMapValueList(const std::string& key) const;
        void AddMapValue(const std::string& key, const std::string& value);

        Vec<const nemesis::AnimationRequest*> GetRequests() const noexcept;
        void AddRequest(UPtr<nemesis::AnimationRequest>&& request);

        const Vec<nemesis::AnimationRequest*>& GetParents() const noexcept;
    };
}
