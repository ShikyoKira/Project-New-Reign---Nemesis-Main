#pragma once

#include <mutex>

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

#include "AlternateAnimation/AlterAnim.h"

#include "nlohmann/json.hpp"

namespace nemesis
{
    struct AlterAnimRepository;

    struct AlterAnimGroup
    {
    private:
        size_t Id;
        std::string Name;

        UMap<std::string, UPtr<nemesis::AlterAnim>> AlterAnimMap;
        Vec<const nemesis::AlterAnim*> AlterAnimList;

        nemesis::AlterAnimRepository& Repository;

        static const std::filesystem::path& CanonizePath(const std::filesystem::path& path);

    public:
        AlterAnimGroup(nemesis::AlterAnimRepository& repo, const nlohmann::json& group_info) noexcept;

        void AddPrefix(const std::filesystem::path& dir_path, const std::string& prefix, size_t slot_size);

        const nemesis::AlterAnimRepository& GetRepository() const noexcept;

        VecStr GetAnimations() const;
        const nemesis::AlterAnim* GetAlternateSet(const std::string& anim_path) const;
        const Vec<const nemesis::AlterAnim*>& GetAlterAnimationList() const noexcept;

        size_t GetId() const noexcept;
        const std::string& GetName() const noexcept;
        std::string GetVariableName() const noexcept;
    };
}
