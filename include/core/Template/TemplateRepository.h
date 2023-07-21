#pragma once

#include "core/Template/TemplateClass.h"

namespace nemesis
{
	struct TemplateRepository
    {
    private:
        Vec<UPtr<nemesis::TemplateClass>> TemplateClasses;

    public:
        void AddTemplateClass(UPtr<nemesis::TemplateClass>&& templt_class);

        Vec<const nemesis::TemplateObject*> GetTemplateListByBehavior(const std::filesystem::path& behavior_path) const;
        Vec<const nemesis::TemplateObject*> GetTemplateListByClass(const std::string& class_name) const;
        const nemesis::TemplateClass* GetClassByName(const std::string& class_name) const;

    };
}
