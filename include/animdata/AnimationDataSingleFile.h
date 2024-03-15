#pragma once

#include "core/NObject.h"

#include "animdata/AnimationDataProject.h"

namespace nemesis
{
    struct AnimationDataSingleFile : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationDataProject>> ProjectList;

        std::mutex UpdaterMutex;

        static VecNstr ParseHeaders(nemesis::LineStream& stream, nemesis::SemanticManager& manager);

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationDataSingleFile> Clone() const;

        nemesis::AnimationDataProject* GetProject(const std::string& project_name, size_t order_position);
        UPtr<nemesis::AnimationDataProject>& AddProject(UPtr<nemesis::AnimationDataProject>&& project);

        void SerializeToDirectory(const std::filesystem::path& directory_path) const;
        static UPtr<nemesis::AnimationDataSingleFile>
        DeserializeFromDirectory(const std::filesystem::path& directory_path);

        static UPtr<nemesis::AnimationDataSingleFile> ParseFromFile(const std::filesystem::path& filepath);
    };
}
