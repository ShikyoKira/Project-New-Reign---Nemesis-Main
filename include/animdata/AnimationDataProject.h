#pragma once

#include "animdata/AnimationDataClipData.h"
#include "animdata/AnimationDataMotionData.h"

namespace nemesis
{
    struct CollectionObject;

	struct AnimationDataProject : public nemesis::NObject
    {
    private:
        enum ParseStage
        {
            IsActiveStage,
            HkxFileCountStage,
            HkxFileStage,
            ClipDataStage,
            MotionDataStage
        };

        std::string Name;
        UPtr<nemesis::CollectionObject> HkxFiles;
        Vec<UPtr<nemesis::AnimationDataClipData>> ClipDataList;
        Vec<UPtr<nemesis::AnimationDataMotionData>> MotionDataList;

        static UPtr<nemesis::CollectionObject> DeserailizeHeaderFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataProject> ParseProject(nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager,
                                                                const std::string& project_name);

    public:
        AnimationDataProject(const std::string& name) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataProject> Clone() const;

        const std::string& GetName() const;

        nemesis::AnimationDataClipData* GetClipData(const std::string& name, const std::string& code);
        nemesis::AnimationDataMotionData* GetMotionData(const std::string& code);

        UPtr<nemesis::AnimationDataClipData>& AddClipData(UPtr<nemesis::AnimationDataClipData>&& clip_data);
        UPtr<nemesis::AnimationDataMotionData>&
        AddMotionData(UPtr<nemesis::AnimationDataMotionData>&& motion_data);

        void SerializeToDirectory(const std::filesystem::path& directory_path) const;
        static UPtr<nemesis::AnimationDataProject>
        DeserializeFromDirectory(const std::filesystem::path& directory_path);
        static UPtr<nemesis::AnimationDataProject>
        DeserializeFromDirectory(const std::filesystem::path& directory_path, const std::string project_name);

        static Vec<UPtr<nemesis::AnimationDataProject>> ParseObjects(nemesis::LineStream& stream,
                                                                     nemesis::SemanticManager& manager,
                                                                     const VecNstr& project_names);
    };
}
