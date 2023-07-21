#pragma once

#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

#include "utilities/threadpool.h"

namespace nemesis
{
    struct TemplateClass;

    struct TemplateObject : public nemesis::NObject
    {
    protected:
        size_t Index;
        std::filesystem::path FilePath;
        UPtr<nemesis::CollectionObject> Data;

        UPtr<nemesis::TemplateObject> Child;

        const nemesis::TemplateClass* TemplateClass;

    public:
        TemplateObject(const nemesis::TemplateClass* template_class) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        const std::string& GetClassName() const noexcept;
        size_t GetIndex() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;
        std::filesystem::path GetTargetPath() const noexcept;

        const nemesis::TemplateClass* GetTemplateClass() const noexcept;

        nemesis::TemplateObject* GetChild(UPtr<nemesis::TemplateObject>&& child);
        void SetChild(UPtr<nemesis::TemplateObject>&& child);

        static SPtr<nemesis::TemplateObject> CreateFromFile(const nemesis::TemplateClass* template_class,
                                                            const std::filesystem::path& filepath);
        static SPtr<nemesis::TemplateObject> CreateFromFile(const nemesis::TemplateClass* template_class,
                                                            const std::filesystem::path& filepath,
                                                            nemesis::ThreadPool& thread_pool);
    };
}
