#pragma once

#include <mutex>

#include "Core/HashableObject.h"
#include "Core/LineStream.h"
#include "Core/NLine.h"

namespace nemesis
{
    struct ModClass;
    struct IfObject;
    struct ThreadPool;
    struct TemplateClass;
    struct ForEachObject;
    struct CollectionObject;

	struct AnimationSetDataState : public nemesis::NObject, public nemesis::HashableObject
    {
    private:
        enum DataType
        {
            EquipCounter,
            EquipList,
            TypeCounter,
            TypeList,
            AnimationCounter,
            AnimationList,
            Crc32Counter,
            Crc32List,
        };

        std::string Name;
        std::filesystem::path FilePath;
        UPtr<nemesis::CollectionObject> StateData;

        mutable std::mutex DataMutex;

    public:
        AnimationSetDataState(const std::string& name) noexcept;

        std::string GetHash() const override;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataState> Clone() const;
        UPtr<nemesis::AnimationSetDataState> Clone(const nemesis::ModClass& mod_class,
                                                   const std::filesystem::path& filepath) const;

        void MatchAndUpdate(const nemesis::AnimationSetDataState& state);
        void MatchAndUpdate(const std::string& mod_code, const nemesis::AnimationSetDataState& state);

        const std::string& GetName() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;

        void SerializeToFile(const std::filesystem::path& filepath) const;

        static void Deserialize(nemesis::CollectionObject& collection,
                                nemesis::LineStream& stream,
                                nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationSetDataState>
        DeserializeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationSetDataState>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::AnimationSetDataState> DeserializeFromFile(const std::filesystem::path& filepath,
                                                                        nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationSetDataState> DeserializeFromFile(const std::filesystem::path& filepath,
                                                                        nemesis::ThreadPool& threadpool);
        static UPtr<nemesis::AnimationSetDataState>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class,
                            nemesis::ThreadPool& threadpool);

        static Vec<UPtr<nemesis::AnimationSetDataState>> ParseObjects(nemesis::LineStream& stream,
                                                                      nemesis::SemanticManager& manager,
                                                                      const std::string& project_name,
                                                                      const VecNstr& state_names);

    };
}
