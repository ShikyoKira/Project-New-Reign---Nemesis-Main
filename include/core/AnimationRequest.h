#pragma once

#include <memory>

#include "core/Template/TemplateOption.h"

namespace nemesis
{
	struct AnimationRequest
    {
    private:
        Vec<UPtr<nemesis::TemplateOption>> Options;
        Vec<const nemesis::TemplateOption*> OptionsCache;
        UMap<std::string, Vec<const nemesis::TemplateOption*>> OptionsCacheMap;

        UMap<std::string, VecStr> StringListMap;

        Vec<UPtr<nemesis::AnimationRequest>> Requests;

        size_t Index;

        std::string TemplateName;
        std::string AnimationEvent;
        std::filesystem::path AnimationFilePath;

        Vec<nemesis::AnimationRequest*> Parents;

        bool SupportArray;

    public:
        AnimationRequest(const std::string& template_name, size_t index, bool support_array) noexcept;

        size_t GetId() const noexcept;
        size_t GetIndex() const noexcept;
        size_t GetLevel() const noexcept;

        const std::string& GetTemplateName() const noexcept;

        const std::string& GetAnimationEvent() const noexcept;
        void SetAnimationEvent(const std::string& animation_event) noexcept;

        const std::filesystem::path& GetAnimationFilePath() const noexcept;
        void SetAnimationFilePath(const std::string& filepath) noexcept;

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
