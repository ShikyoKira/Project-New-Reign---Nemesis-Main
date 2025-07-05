#pragma once

#include "Core/Template/TemplateObject.h"

namespace nemesis
{
    struct LineStream;
    struct SemanticManager;

    struct TemplateAnimDataClipData : public nemesis::TemplateObject
    {
    public:
        TemplateAnimDataClipData(const nemesis::TemplateClass* template_class) noexcept;

        static UPtr<nemesis::TemplateAnimDataClipData>
        ParseFromFile(const std::filesystem::path& filepath,
                      const nemesis::TemplateClass* template_class,
                      size_t index);
        static UPtr<nemesis::TemplateAnimDataClipData>
        ParseFromFile(const std::filesystem::path& filepath,
                      const nemesis::TemplateClass* template_class,
                      size_t index,
                      nemesis::ThreadPool& thread_pool);
    };
}
