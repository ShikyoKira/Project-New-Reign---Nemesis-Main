#pragma once

#include <mutex>

#include "AlternateAnimation/AlterAnimGroup.h"
#include "AlternateAnimation/AlterAnimPrefix.h"

namespace nemesis
{
	struct AlterAnimRepository
    {
    private:
        Vec<UPtr<nemesis::AlterAnimGroup>> GroupList;
        Vec<const nemesis::AlterAnimGroup*> GroupListCache;
        UMap<std::string, nemesis::AlterAnimGroup*> GroupAnimationMap;

        std::filesystem::path ProjectDirectory;
        std::filesystem::path AnimationDirectory;
        
        Vec<std::filesystem::path> Behaviors;
        Vec<std::filesystem::path> Characters;

        Vec<nemesis::AlterAnimPrefix> PrefixList;
        mutable std::mutex PrefixListMutex;

        mutable Vec<std::filesystem::path> AnimationListCache;
        mutable std::mutex AnimationListCacheMutex;

        static const std::filesystem::path& CanonizePath(const std::filesystem::path& path);

    public:
        AlterAnimRepository(const std::filesystem::path& alter_anim_json);

        bool IsSupportedBehavior(const std::filesystem::path& behavior_path) const;
        bool IsSupportedCharacter(const std::filesystem::path& behavior_path) const;

        const std::filesystem::path& GetProjectDirectory() const noexcept;
        const std::filesystem::path& GetAnimationDirectory() const noexcept;

        void AddPrefix(const std::string& name);
        const Vec<nemesis::AlterAnimPrefix>& GetPrefixList() const noexcept;
        const nemesis::AlterAnimPrefix* GetPrefixById(size_t id) const noexcept;
        const nemesis::AlterAnimPrefix* GetPrefixByName(const std::string& name) const noexcept;

        const Vec<const nemesis::AlterAnimGroup*>& GetAlterAnimGroupList() const noexcept;
        nemesis::AlterAnimGroup* GetAlterAnimGroupByName(const std::string& group_name);
        const nemesis::AlterAnimGroup* GetAlterAnimGroupByName(const std::string& group_name) const;
        const nemesis::AlterAnimGroup* GetAlterAnimGroupByAnimation(const std::string& anim_path) const;

        Vec<std::filesystem::path> GetAlterAnimationList() const;
    };
}
