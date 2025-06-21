#pragma once

#include "ExAnimation/ExAnimationProject.h"

namespace nemesis
{
	struct ExAnimationRepository
    {
    private:
        Vec<UPtr<nemesis::ExAnimationProject>> ProjectList;

        void AddAllProjectsFromFolderRecursively(const std::filesystem::path& dir,
                                                 nemesis::ThreadPool& thread_pool);

    public:
        ExAnimationRepository(const std::filesystem::path& meshes_dir);

        const nemesis::ExAnimationProject*
        GetProjectByBehavior(const std::filesystem::path& hkxfile_path) const;
        const nemesis::ExAnimationProject*
        GetProjectByCharacter(const std::filesystem::path& hkxfile_path) const;
        Vec<const std::filesystem::path*>
        GetExAnimationList(const std::filesystem::path& character_filepath) const;
        Vec<const nemesis::ExAnimationProject*> GetProjectList() const;

        void OutputExAnimationInfo() const;
    };
}
