#pragma once

#include "core/NObject.h"

#include "animsetdata/AnimationSetDataProject.h"

namespace nemesis
{
    struct AnimationSetDataSingleFile : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationSetDataProject>> ProjectList;

        std::mutex UpdaterMutex;

        static VecNstr ParseHeaders(nemesis::LineStream& stream, nemesis::SemanticManager& manager);

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataSingleFile> Clone() const;

        UPtr<nemesis::AnimationSetDataProject>& AddProject(UPtr<nemesis::AnimationSetDataProject>&& project);

        nemesis::AnimationSetDataProject* GetProject(const std::string& project_name);
        const nemesis::AnimationSetDataProject* GetProject(const std::string& project_name) const;

        void SerializeToDirectory(const std::filesystem::path& directory_path) const;
        static UPtr<nemesis::AnimationSetDataSingleFile>
        DeserializeFromDirectory(const std::filesystem::path& directory_path);

        static UPtr<nemesis::AnimationSetDataSingleFile> ParseFromFile(const std::filesystem::path& filepath);
    };
}
