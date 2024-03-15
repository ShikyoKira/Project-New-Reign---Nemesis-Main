#pragma once

#include "core/NLine.h"
#include "core/LineStream.h"

namespace nemesis
{
    struct IfObject;
    struct TemplateClass;
    struct ForEachObject;
    struct CollectionObject;

    struct AnimationDataClipData : public nemesis::NObject
    {
    private:
        std::string Name;
        std::string Code;
        UPtr<nemesis::CollectionObject> ClipData;

        std::mutex UpdaterMutex;

        static bool IsCode(const std::string& value);
        static bool IsPlaybackSpeeed(const std::string& value);
        static bool IsCropStartAmountLocalTime(const std::string& value);
        static bool IsCropEndAmountLocalTime(const std::string& value);
        static bool IsEventCounter(const std::string& value);

    public:
        AnimationDataClipData(const std::string& name, const std::string& code) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataClipData> Clone() const;

        void MatchAndUpdate(const nemesis::AnimationDataClipData& clip_data);

        const std::string& GetName() const;
        const std::string& GetCode() const;

        void SerializeToFile(const std::filesystem::path& filepath) const;

        static UPtr<nemesis::AnimationDataClipData>
        Deserialize(const std::string& name, const std::string& code, nemesis::LineStream& stream);
        static UPtr<nemesis::AnimationDataClipData>
        Deserialize(const std::string& name, const std::string& code, nemesis::LineStream& stream, nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationDataClipData>
        DeserializeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataClipData>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::AnimationDataClipData> DeserializeFromFile(const std::filesystem::path& filepath,
                                                                        nemesis::SemanticManager& manager);

        static Vec<UPtr<nemesis::AnimationDataClipData>>
        ParseObjects(nemesis::LineStream& stream,
                     nemesis::SemanticManager& manager,
                     const nemesis::LineStream::Token* end_token);
    };
}
