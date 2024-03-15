#pragma once

#include "core/NLine.h"
#include "core/LineStream.h"

namespace nemesis
{
    struct IfObject;
    struct TemplateClass;
    struct ForEachObject;
    struct CollectionObject;

	struct AnimationSetDataState : public nemesis::NObject
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
        UPtr<nemesis::CollectionObject> StateData;

        std::mutex UpdaterMutex;

    public:
        AnimationSetDataState(const std::string& name) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataState> Clone() const;

        void MatchAndUpdate(const nemesis::AnimationSetDataState& state);

        const std::string& GetName() const noexcept;

        void SerializeToFile(const std::filesystem::path& filepath) const;

        static UPtr<nemesis::AnimationSetDataState> Deserialize(const std::string& name,
                                                                nemesis::LineStream& stream);
        static UPtr<nemesis::AnimationSetDataState>
        Deserialize(const std::string& name, nemesis::LineStream& stream, nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationSetDataState>
        DeserializeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationSetDataState>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::AnimationSetDataState> DeserializeFromFile(const std::filesystem::path& filepath,
                                                                        nemesis::SemanticManager& manager);

        static Vec<UPtr<nemesis::AnimationSetDataState>> ParseObjects(nemesis::LineStream& stream,
                                                                      nemesis::SemanticManager& manager,
                                                                      const std::string& project_name,
                                                                      const VecNstr& state_names);

    };
}
