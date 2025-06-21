#pragma once

#include <mutex>
#include <filesystem>
#include <functional>

#include "AlternateAnimation/AlterAnimSet.h"

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    struct CompileState;
    struct AlterAnimGroup;

	struct AlterAnim
    {
    private:
        std::filesystem::path AnimPath;
        Map<std::string, UPtr<nemesis::AlterAnimSet>> PrefixSet;

        mutable UPtr<Vec<const nemesis::AlterAnimSet*>> PrefixSetCache;
        mutable std::mutex PrefixSetCacheMutex;

        const nemesis::AlterAnimGroup& Group;

    public:
        AlterAnim(const nemesis::AlterAnimGroup& group, const std::filesystem::path& anim_path) noexcept;

        void AddAlternateSlots(const std::filesystem::path& relative_dir_path,
                               const std::filesystem::path& dir_path,
                               const std::string& prefix,
                               size_t slot_size);

        VecStr
        CompileAsXml(std::string& node_id,
                     nemesis::CompileState& state,
                     const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>&
                         add_clip_func) const;
        const std::filesystem::path GetAnimPath() const noexcept;
        const Vec<const nemesis::AlterAnimSet*>& GetAnimationSetList() const noexcept;
        const nemesis::AlterAnimGroup& GetGroup() const noexcept;
    };
}
