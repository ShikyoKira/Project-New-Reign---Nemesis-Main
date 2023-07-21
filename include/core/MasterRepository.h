#pragma once

#include "hkx/HkxBehavior.h"

#include "core/Template/TemplateClass.h"

#include "core/AnimationRequestRepository.h"

namespace nemesis
{
	struct MasterRepository
    {
    private:
        Vec<UPtr<nemesis::HkxBehavior>> BehaviorList;
        UMap<std::wstring, nemesis::HkxBehavior*> BehaviorMap;

        Vec<UPtr<nemesis::TemplateClass>> TemplateClasses;

        nemesis::AnimationRequestRepository RequestRepository;
        
    public:
        void AddBehavior(UPtr<nemesis::HkxBehavior>&& behavior);

        nemesis::HkxBehavior* GetBehaviorByPath(const std::filesystem::path& filepath);
        const nemesis::HkxBehavior* GetBehaviorByPath(const std::filesystem::path& filepath) const;

        Vec<nemesis::HkxBehavior*> GetBehaviorList();
        Vec<const nemesis::HkxBehavior*> GetBehaviorList() const;

        void CompileAllBehaviors() const;
        void CompileAllBehaviors(nemesis::ThreadPool& tp) const;

        void AddTemplateClass(UPtr<nemesis::TemplateClass>&& templt_class);

        nemesis::TemplateClass* GetTemplateClassByName(const std::string& name);
        const nemesis::TemplateClass* GetTemplateClassByName(const std::string& name) const;

        Vec<nemesis::TemplateClass*> GetTemplateClassList();
        Vec<const nemesis::TemplateClass*> GetTemplateClassList() const;

        nemesis::AnimationRequestRepository& GetAnimationRequestRepository();
        const nemesis::AnimationRequestRepository& GetAnimationRequestRepository() const;
    };
}
