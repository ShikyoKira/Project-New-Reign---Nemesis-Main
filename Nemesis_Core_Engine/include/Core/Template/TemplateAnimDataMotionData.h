#pragma once

#include "Core/Template/TemplateObject.h"

namespace nemesis
{
    struct LineStream;
    struct SemanticManager;

	struct TemplateAnimDataMotionData : public nemesis::TemplateObject
    {
    public:
        TemplateAnimDataMotionData(const nemesis::TemplateClass* template_class) noexcept;

        static SPtr<nemesis::TemplateAnimDataMotionData>
        ParseFromFile(const nemesis::TemplateClass* template_class, const std::filesystem::path& filepath);
        static SPtr<nemesis::TemplateAnimDataMotionData>
        ParseFromFile(const nemesis::TemplateClass* template_class,
                      const std::filesystem::path& filepath,
                      nemesis::ThreadPool& thread_pool);
    };
}
