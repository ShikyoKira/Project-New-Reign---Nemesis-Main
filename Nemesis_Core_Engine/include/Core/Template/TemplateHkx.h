#pragma once

#include "Core/Template/TemplateObject.h"

namespace nemesis
{
    struct LineStream;
    struct SemanticManager;

	struct TemplateHkx : public nemesis::TemplateObject
    {
    public:
        TemplateHkx(const nemesis::TemplateClass* template_class) noexcept;

        static SPtr<nemesis::TemplateHkx> ParseFromFile(const nemesis::TemplateClass* template_class,
                                                         const std::filesystem::path& filepath);
        static SPtr<nemesis::TemplateHkx> ParseFromFile(const nemesis::TemplateClass* template_class,
                                                         const std::filesystem::path& filepath,
                                                         nemesis::ThreadPool& thread_pool);
    };
}
