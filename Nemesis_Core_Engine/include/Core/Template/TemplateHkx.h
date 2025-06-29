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

        static UPtr<nemesis::TemplateHkx> ParseFromFile(const std::filesystem::path& filepath,
                                                        const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::TemplateHkx> ParseFromFile(const std::filesystem::path& filepath,
                                                        const nemesis::TemplateClass* template_class,
                                                        nemesis::ThreadPool& thread_pool);
    };
}
