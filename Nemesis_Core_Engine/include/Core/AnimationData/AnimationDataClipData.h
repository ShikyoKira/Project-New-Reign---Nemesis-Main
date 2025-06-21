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

    struct AnimationDataClipData : public nemesis::NObject
    {
    private:
        std::string Name;
        std::string Code;
        std::filesystem::path FilePath;
        UPtr<nemesis::CollectionObject> ClipData;

        mutable std::mutex DataMutex;

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
        UPtr<nemesis::AnimationDataClipData> Clone(const nemesis::ModClass& mod_class,
                                                   const std::filesystem::path& filepath) const;

        void MatchAndUpdate(const nemesis::AnimationDataClipData& clip_data);
        void MatchAndUpdate(const std::string& mod_code, const nemesis::AnimationDataClipData& clip_data);

        const std::string& GetName() const noexcept;
        const std::string& GetCode() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;

        void SerializeToFile(const std::filesystem::path& filepath) const;

        static void Deserialize(nemesis::CollectionObject& collection,
                                nemesis::LineStream& stream,
                                nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationDataClipData>
        DeserializeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataClipData>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::AnimationDataClipData> DeserializeFromFile(const std::filesystem::path& filepath,
                                                                        nemesis::SemanticManager& manager);
        static UPtr<nemesis::AnimationDataClipData> DeserializeFromFile(const std::filesystem::path& filepath,
                                                                        nemesis::ThreadPool& threadpool);
        static UPtr<nemesis::AnimationDataClipData>
        DeserializeFromFile(const std::filesystem::path& filepath,
                            const nemesis::TemplateClass* template_class,
                            nemesis::ThreadPool& threadpool);

        static Vec<UPtr<nemesis::AnimationDataClipData>>
        ParseObjects(nemesis::LineStream& stream,
                     nemesis::SemanticManager& manager,
                     const nemesis::LineStream::Token* end_token);
    };
}
