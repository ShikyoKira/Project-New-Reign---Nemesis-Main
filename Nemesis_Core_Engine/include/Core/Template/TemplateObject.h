#pragma once

#include "Core/NObjectParser.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/File.h"
#include "Utilities/ThreadPool.h"

namespace nemesis
{
    struct TemplateObject : public nemesis::NObject
    {
    protected:
        size_t Index;
        std::filesystem::path FilePath;
        UPtr<nemesis::CollectionObject> Data;

        UPtr<nemesis::TemplateObject> Child;

        const nemesis::TemplateClass* TemplateClass;

    protected:
        template <typename TemplateType>
        static UPtr<TemplateType> NewCustomTemplateObject(const nemesis::TemplateClass* template_class,
                                                          size_t index,
                                                          const std::filesystem::path& filepath)
        {
            static_assert(std::is_base_of_v<nemesis::TemplateObject, TemplateType>
                              || std::is_same_v<nemesis::TemplateObject, TemplateType>,
                          "Non-TemplateObject type is passed to TemplateType");

            auto templt      = std::make_unique<TemplateType>(template_class);
            auto templt_ptr  = templt.get();
            templt->Index    = index;
            templt->Data     = std::make_unique<nemesis::CollectionObject>();
            templt->FilePath = filepath;

            auto& data_ref = *templt_ptr->Data;
            nemesis::SemanticManager manager;
            manager.SetCurrentTemplate(templt_ptr);

            for (size_t i = 1; i <= templt->Index; i++)
            {
                manager.TryAddRequestToQueue(template_class->GetName() + "_" + std::to_string(i));
            }

            VecNstr lines;
            GetFileLines(filepath, lines, false);

            for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
            {
                data_ref.AddObject(nemesis::NObjectParser::ParseLine(stream, manager));
            }

            return templt;
        }

        template <typename TemplateType>
        static UPtr<TemplateType> NewCustomTemplateObject(const nemesis::TemplateClass* template_class,
                                                          size_t index,
                                                          const std::filesystem::path& filepath,
                                                          nemesis::ThreadPool& thread_pool)
        {
            static_assert(std::is_base_of_v<nemesis::TemplateObject, TemplateType>
                              || std::is_same_v<nemesis::TemplateObject, TemplateType>,
                          "Non-TemplateObject type is passed to TemplateType");

            auto templt      = std::make_unique<TemplateType>(template_class);
            auto templt_ptr  = templt.get();
            templt->Index    = index;
            templt->Data     = std::make_unique<nemesis::CollectionObject>();
            templt->FilePath = filepath;

            thread_pool.enqueue(
                [templt_ptr, template_class]()
                {
                    VecNstr lines;
                    GetFileLines(templt_ptr->FilePath, lines, false);

                    auto& data_ref = *templt_ptr->Data;
                    nemesis::SemanticManager manager;
                    manager.SetCurrentTemplate(templt_ptr);

                    for (size_t i = 1; i <= templt_ptr->Index; i++)
                    {
                        manager.TryAddRequestToQueue(template_class->GetName() + "_" + std::to_string(i));
                    }

                    for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
                    {
                        data_ref.AddObject(nemesis::NObjectParser::ParseLine(stream, manager));
                    }
                });

            return templt;
        }

    public:
        TemplateObject(const nemesis::TemplateClass* template_class) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::TemplateObject> Clone() const;
        SPtr<nemesis::TemplateObject> SClone() const;

        const std::string& GetClassName() const noexcept;
        size_t GetIndex() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;
        std::filesystem::path GetTargetPath() const noexcept;

        const nemesis::TemplateClass* GetTemplateClass() const noexcept;

        nemesis::TemplateObject* GetChild();
        const nemesis::TemplateObject* GetChild() const;
        UPtr<nemesis::TemplateObject>& SetChild(UPtr<nemesis::TemplateObject>&& child);

        static UPtr<nemesis::TemplateObject> ParseFromFile(const std::filesystem::path& filepath,
                                                           const nemesis::TemplateClass* templt_class);
        static UPtr<nemesis::TemplateObject> ParseFromFile(const std::filesystem::path& filepath,
                                                           const nemesis::TemplateClass* templt_class,
                                                           nemesis::ThreadPool& thread_pool);
    };
}
