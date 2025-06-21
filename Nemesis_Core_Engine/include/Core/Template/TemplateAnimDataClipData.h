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

        static SPtr<nemesis::TemplateAnimDataClipData>
        ParseFromFile(const nemesis::TemplateClass* template_class, const std::filesystem::path& filepath);
        static SPtr<nemesis::TemplateAnimDataClipData>
        ParseFromFile(const nemesis::TemplateClass* template_class,
                      const std::filesystem::path& filepath,
                      nemesis::ThreadPool& thread_pool);
    };
}
