#pragma once

#include "Core/Template/TemplateClass.h"

#include "Core/SubTemplateObject.h"

namespace nemesis
{
	struct TemplateRepository
    {
    private:
        Vec<UPtr<nemesis::TemplateClass>> TemplateClassList;
        Vec<UPtr<nemesis::SubTemplateObject>> SubTemplateList;

        mutable UPtr<Vec<const nemesis::TemplateClass*>> TemplateClassList_Cache;
        mutable UPtr<Vec<const nemesis::SubTemplateObject*>> SubTemplateList_Cache;

    public:
        TemplateRepository() = default;
        TemplateRepository(const std::filesystem::path& templt_dir, nemesis::NObjectRepository& repo);

        UPtr<nemesis::AnimationRequest> CreateRequest(const std::string& list_name,
                                                      const std::string& request_info,
                                                      size_t linenum,
                                                      const std::filesystem::path& filepath);

        Vec<const nemesis::TemplateClass*> GetTemplateClassList() const noexcept;
        Vec<const nemesis::SubTemplateObject*> GetSubTemplateList() const noexcept;
    };
}
