#pragma once

#include "Core/NObject.h"

#include "Utilities/ThreadPool.h"

#include "Core/AnimationData/AnimationDataProject.h"

namespace nemesis
{
    struct TemplateObject;

    struct AnimationDataSingleFile : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationDataProject>> ProjectList;
        UMap<std::string, Vec<nemesis::AnimationDataProject*>> ProjectNameMap;

        Vec<SPtr<nemesis::TemplateObject>> ProjectTemplateList;

        std::filesystem::path FilePath;
        std::filesystem::path TargetPath;
        std::filesystem::path RelativePath;

        mutable std::mutex ProjectMutex;
        mutable std::mutex ProjectTemplateMutex;

        mutable Vec<std::future<void>> CompileFuture;
        mutable std::mutex CompileFutureMutex;

        static VecNstr ParseHeaders(nemesis::LineStream& stream, nemesis::SemanticManager& manager);

        std::future<void> CompileFileCore(const std::filesystem::path& filepath,
                                          nemesis::CompileState& state,
                                          std::function<void()> callback) const;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataSingleFile> Clone() const;

        std::filesystem::path CompileFile(nemesis::CompileState& state) const;
        void CompileFileAs(const std::filesystem::path& filepath, nemesis::CompileState& state) const;
        std::filesystem::path ScheduleCompileFile(const std::filesystem::path& filepath,
                                                  nemesis::CompileState& state) const;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath, nemesis::CompileState& state) const;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                   nemesis::CompileState& state,
                                   std::function<void()> callback) const;
        void WaitForCompleteCompilation() const;

        std::filesystem::path GetFilePath() const;
        std::filesystem::path GetTargetPath() const;
        std::filesystem::path GetRelativePath() const;

        nemesis::AnimationDataProject* GetProject(const std::string& project_name, size_t order_position);
        UPtr<nemesis::AnimationDataProject>& AddProject(UPtr<nemesis::AnimationDataProject>&& project);
        SPtr<nemesis::TemplateObject>& AddProjectTemplate(const SPtr<nemesis::TemplateObject>& templt_obj);

        void SerializeToDirectory(const std::filesystem::path& directory_path) const;
        static UPtr<nemesis::AnimationDataSingleFile>
        DeserializeFromDirectory(const std::filesystem::path& directory_path);

        static UPtr<nemesis::AnimationDataSingleFile> ParseFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::AnimationDataSingleFile> ParseFromFile(const std::filesystem::path& filepath,
                                                                    nemesis::ThreadPool& threadpool);
    };
}
