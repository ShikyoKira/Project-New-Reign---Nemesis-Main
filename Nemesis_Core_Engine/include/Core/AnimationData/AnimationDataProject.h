#pragma once

#include "Core/AnimationData/AnimationDataClipData.h"
#include "Core/AnimationData/AnimationDataMotionData.h"

#include "Core/CollectionObject.h"

namespace nemesis
{
    struct ThreadPool;
    struct TemplateObject;

	struct AnimationDataProject : public nemesis::NObject
    {
        struct Headers : public nemesis::CollectionObject
        {
        private:
            std::filesystem::path FilePath;

        public:
            UPtr<nemesis::CollectionObject> Clone() const override;
            UPtr<nemesis::AnimationDataProject::Headers> CloneHeaders() const;

            const std::filesystem::path& GetFilePath() const noexcept;
        };

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
        UPtr<nemesis::AnimationDataProject::Headers> HkxFiles;
        Vec<UPtr<nemesis::AnimationDataClipData>> ClipDataList;
        Vec<UPtr<nemesis::AnimationDataMotionData>> MotionDataList;

        Vec<SPtr<nemesis::TemplateObject>> ClipDataTemplateList;
        Vec<SPtr<nemesis::TemplateObject>> MotionDataTemplateList;

        mutable std::mutex HkxFileMutex;
        mutable std::mutex ClipDataMutex;
        mutable std::mutex MotionDataMutex;
        
        mutable std::mutex ClipTemplateMutex;
        mutable std::mutex MotionTemplateMutex;

        static UPtr<nemesis::AnimationDataProject> ParseProject(nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager,
                                                                const std::string& project_name);

    public:
        AnimationDataProject(const std::string& name) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataProject> Clone() const;

        void MatchAndUpdateHeader(const nemesis::CollectionObject& hkxfiles);
        void MatchAndUpdateHeader(const std::string& mod_code, const nemesis::CollectionObject& hkxfiles);

        const std::string& GetName() const;

        nemesis::AnimationDataClipData* GetClipData(const std::string& name, const std::string& code);
        nemesis::AnimationDataMotionData* GetMotionData(const std::string& code);

        UPtr<nemesis::AnimationDataClipData>& AddClipData(UPtr<nemesis::AnimationDataClipData>&& clip_data);
        UPtr<nemesis::AnimationDataMotionData>&
        AddMotionData(UPtr<nemesis::AnimationDataMotionData>&& motion_data);

        SPtr<nemesis::TemplateObject>& AddClipDataTemplate(const SPtr<nemesis::TemplateObject>& templt_obj);
        SPtr<nemesis::TemplateObject>& AddMotionDataTemplate(const SPtr<nemesis::TemplateObject>& templt_obj);

        void SerializeToDirectory(const std::filesystem::path& directory_path) const;
        static UPtr<nemesis::AnimationDataProject>
        DeserializeFromDirectory(const std::filesystem::path& directory_path);
        static UPtr<nemesis::AnimationDataProject>
        DeserializeFromDirectory(const std::filesystem::path& directory_path, const std::string project_name);
        static UPtr<nemesis::AnimationDataProject>
        DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                 nemesis::ThreadPool& threadpool);
        static UPtr<nemesis::AnimationDataProject>
        DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                 const std::string project_name,
                                 nemesis::ThreadPool& threadpool);

        static UPtr<nemesis::AnimationDataProject::Headers>
        DeserializeHeaderFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataProject::Headers>
        DeserializeHeaderFromFile(const std::filesystem::path& filepath, nemesis::ThreadPool& threadpool);

        static Vec<UPtr<nemesis::AnimationDataProject>> ParseObjects(nemesis::LineStream& stream,
                                                                     nemesis::SemanticManager& manager,
                                                                     const VecNstr& project_names);
    };
}
