#pragma once

#include "core/Template/TemplateObject.h"

namespace nemesis
{
    struct LineStream;
    struct SemanticManager;

	struct TemplateHkx : public nemesis::TemplateObject
    {
    private:
        static Vec<UPtr<nemesis::NObject>> ParseObjects(nemesis::LineStream& stream,
                                                        nemesis::SemanticManager& manager);

    public:
        TemplateHkx(const nemesis::TemplateClass* template_class) noexcept;

        static SPtr<nemesis::TemplateHkx> CreateFromFile(const nemesis::TemplateClass* template_class,
                                                         const std::filesystem::path& filepath);
        static SPtr<nemesis::TemplateHkx> CreateFromFile(const nemesis::TemplateClass* template_class,
                                                         const std::filesystem::path& filepath,
                                                         nemesis::ThreadPool& thread_pool);
    };
}
