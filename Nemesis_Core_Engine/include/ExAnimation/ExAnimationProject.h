#pragma once

#include "ExAnimation/ExAnimationPack.h"
#include "ExAnimation/LinkedExAnimList.h"

#include "Utilities/ThreadPool.h"


namespace nemesis
{
	struct ExAnimationProject
    {
    private:
        std::string Name;

        std::filesystem::path WorkingDirectory;
        std::filesystem::path AnimationDirectory;

        Vec<std::filesystem::path> Characters;
        USet<std::filesystem::path> CanonCharacters;

        Vec<std::filesystem::path> Behaviors;
        USet<std::filesystem::path> CanonBehaviors;

        Map<size_t, UPtr<nemesis::ExAnimationPack>> ExAnimMap;

        void LoadExAnim();

        static const std::filesystem::path& CanonizePath(const std::filesystem::path& path);
        static void PopulateProjectDataTo(nemesis::ExAnimationProject& project,
                                          const std::filesystem::path& json_path);

    public:
        bool MatchBehavior(const std::filesystem::path& hkxfile_path) const;
        bool MatchCharacter(const std::filesystem::path& hkxfile_path) const;

        const std::string& GetName() const noexcept;

        Vec<const nemesis::ExAnimationRequest*>
        GetExAnimationRequest(const std::filesystem::path& anim_path) const;

        Vec<const nemesis::ExAnimationPack*> GetExAnimationPackList() const;
        UPtr<nemesis::LinkedExAnimList> GetLinkedExAnim(const std::filesystem::path& anim_path) const;
        Vec<const std::filesystem::path*> GetExAnimationList() const;

        static UPtr<nemesis::ExAnimationProject>
        ParseExAnimationProject(const std::filesystem::path& json_path);
        static UPtr<nemesis::ExAnimationProject>
        ParseExAnimationProject(const std::filesystem::path& json_path, nemesis::ThreadPool& thread_pool);
    };
}
