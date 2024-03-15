#pragma once

#include "core/NLine.h"
#include "core/LineStream.h"

namespace nemesis
{
    struct IfObject;
    struct TemplateClass;
    struct ForEachObject;
    struct CollectionObject;

    struct AnimationDataMotionData : public nemesis::NObject
    {
    private:
        enum CompileStage
        {
            CodeStage,
            LengthStage,
            PositionLengthStage,
            PositionInfoStage,
            DirectionLengthStage,
            DirectionInfoStage
        };

        std::string Code;
        UPtr<nemesis::CollectionObject> MotionData;

    public:
        AnimationDataMotionData(const std::string& code) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataMotionData> Clone() const;

        void MatchAndUpdate(const nemesis::AnimationDataMotionData& motion_data);

        const std::string& GetCode() const;

        void SerializeToFile(const std::filesystem::path& filepath) const;

        static UPtr<nemesis::AnimationDataMotionData> Deserialize(const std::string& code,
                                                                  nemesis::LineStream& stream);
        static UPtr<nemesis::AnimationDataMotionData>
        Deserialize(const std::string& code, nemesis::LineStream& stream, nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath, nemesis::SemanticManager& manager);

        static Vec<UPtr<nemesis::AnimationDataMotionData>> ParseObjects(nemesis::LineStream& stream,
                                                                        nemesis::SemanticManager& manager);
    };
}
