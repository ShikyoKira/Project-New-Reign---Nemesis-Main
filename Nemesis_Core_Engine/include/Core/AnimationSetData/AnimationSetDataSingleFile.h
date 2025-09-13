#pragma once

#include "Core/NObject.h"

#include "Utilities/ThreadPool.h"

#include "Core/AnimationSetData/AnimationSetDataProject.h"

namespace nemesis
{
    struct TemplateObject;

    struct AnimationSetDataSingleFile : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationSetDataProject>> ProjectList;
        UMap<std::string, nemesis::AnimationSetDataProject*> ProjectNameMap;

        Vec<SPtr<nemesis::TemplateObject>> ProjectTemplateList;

        std::filesystem::path FilePath;
        std::filesystem::path TargetPath;
        std::filesystem::path RelativePath;

        USetStr ModInUsedList;
        mutable std::mutex ModInUsedListMutex;

        mutable std::mutex ProjectMutex;
        mutable std::mutex ProjectTemplateMutex;

        mutable Vec<std::future<void>> CompileFuture;
        mutable std::mutex CompileFutureMutex;

        static VecNstr ParseHeaders(nemesis::LineStream& stream, nemesis::SemanticManager& manager);

        std::future<void> CompileFileCore(const std::filesystem::path& filepath,
                                          nemesis::CompileState& state,
                                          std::function<void()> callback) const;

    public:
        std::string GetHash(nemesis::CompileState& state) const;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataSingleFile> Clone() const;

        std::filesystem::path CompileFile(nemesis::CompileState& state) const;
        void CompileFileAs(const std::filesystem::path& filepath, nemesis::CompileState& state) const;
        std::filesystem::path ScheduleCompileFile(nemesis::CompileState& state) const;
        std::filesystem::path ScheduleCompileFile(nemesis::CompileState& state,
                                                  std::function<void()> callback) const;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath, nemesis::CompileState& state) const;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                   nemesis::CompileState& state,
                                   std::function<void()> callback) const;
        void WaitForCompleteCompilation() const;

        std::filesystem::path GetFilePath() const;
        std::filesystem::path GetTargetPath() const;

        void AddModNode(const std::string& modcode);

        UPtr<nemesis::AnimationSetDataProject>& AddProject(UPtr<nemesis::AnimationSetDataProject>&& project);
        SPtr<nemesis::TemplateObject>& AddProjectTemplate(const SPtr<nemesis::TemplateObject>& templt_obj);

        nemesis::AnimationSetDataProject* GetProject(const std::string& project_name);
        const nemesis::AnimationSetDataProject* GetProject(const std::string& project_name) const;

        void SerializeToDirectory(const std::filesystem::path& directory_path) const;
        static UPtr<nemesis::AnimationSetDataSingleFile>
        DeserializeFromDirectory(const std::filesystem::path& directory_path);

        static UPtr<nemesis::AnimationSetDataSingleFile> ParseFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationSetDataSingleFile> ParseFromFile(const std::filesystem::path& filepath,
                                                                       nemesis::ThreadPool& threadpool);
    };
}
