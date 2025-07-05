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

        static UPtr<nemesis::TemplateAnimDataMotionData>
        ParseFromFile(const std::filesystem::path& filepath,
                      const nemesis::TemplateClass* template_class,
                      size_t index);
        static UPtr<nemesis::TemplateAnimDataMotionData>
        ParseFromFile(const std::filesystem::path& filepath,
                      const nemesis::TemplateClass* template_class,
                      size_t index,
                      nemesis::ThreadPool& thread_pool);
    };
}
