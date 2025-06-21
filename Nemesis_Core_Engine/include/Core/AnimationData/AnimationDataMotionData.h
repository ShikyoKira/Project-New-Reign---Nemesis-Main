#pragma once

#include <mutex>

#include "Core/NLine.h"
#include "Core/LineStream.h"

namespace nemesis
{
    struct ModClass;
    struct IfObject;
    struct ThreadPool;
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
        std::filesystem::path FilePath;
        UPtr<nemesis::CollectionObject> MotionData;

        mutable std::mutex DataMutex;

    public:
        AnimationDataMotionData(const std::string& code) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataMotionData> Clone() const;
        UPtr<nemesis::AnimationDataMotionData> Clone(const nemesis::ModClass& mod_class,
                                                     const std::filesystem::path& filepath) const;

        void MatchAndUpdate(const nemesis::AnimationDataMotionData& motion_data);
        void MatchAndUpdate(const std::string& mod_code, const nemesis::AnimationDataMotionData& motion_data);

        const std::string& GetCode() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;

        void SerializeToFile(const std::filesystem::path& filepath) const;

        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath, nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath, nemesis::ThreadPool& threadpool);
        static UPtr<nemesis::AnimationDataMotionData>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class,
                            nemesis::ThreadPool& threadpool);

        static Vec<UPtr<nemesis::AnimationDataMotionData>> ParseObjects(nemesis::LineStream& stream,
                                                                        nemesis::SemanticManager& manager);
    };
}
