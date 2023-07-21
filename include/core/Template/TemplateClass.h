#pragma once

#include "core/Template/TemplateHkx.h"
#include "core/Template/TemplateObject.h"
#include "core/Template/TemplateOptionModel.h"

#include "core/AnimationRequest.h"

namespace nemesis
{
	struct TemplateClass
    {
    private:
        std::string Name;
        size_t MinimumArray = 0;
        bool IsArray        = false;

        Vec<UPtr<nemesis::TemplateOptionModel>> OptionModelList;
        Vec<SPtr<nemesis::TemplateObject>> Templates;

        std::filesystem::path InfoPath;

    public:
        TemplateClass(const std::filesystem::path& template_info_path);

        void AddTemplate(const SPtr<nemesis::TemplateObject>& template_object);
        SPtr<nemesis::TemplateObject> AddTemplate(const std::filesystem::path& filepath);
        SPtr<nemesis::TemplateObject> AddTemplate(const std::filesystem::path& filepath, nemesis::ThreadPool& thread_pool);

        SPtr<nemesis::TemplateObject> GetTemplate(size_t index);
        const nemesis::TemplateObject* GetTemplate(size_t index) const;

        Vec<const nemesis::TemplateObject*> GetTemplateList() const;

        size_t GetSize() const noexcept;
        const std::string& GetName() const noexcept;

        const nemesis::TemplateOptionModel* GetModel(const std::string& name) const;

        const std::filesystem::path& GetInfoPath() const noexcept;

        UPtr<nemesis::AnimationRequest> CreateRequest(const std::string& request_info,
                                                      size_t index,
                                                      size_t linenum,
                                                      const std::filesystem::path& filepath);
    };
}
