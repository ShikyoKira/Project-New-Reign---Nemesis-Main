#pragma once

#include <mutex>
#include <filesystem>

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    struct AlterAnimSet
    {
    private:
        std::string Prefix;

        Vec<UPtr<std::filesystem::path>> Slots;
        mutable Vec<const std::filesystem::path*> CacheSlots;

        static const std::filesystem::path& CanonizePath(const std::filesystem::path& path);

    public:
        AlterAnimSet(const std::filesystem::path& relative_dir_path,
                     const std::filesystem::path& dir_path,
                     const std::filesystem::path& anim_path,
                     const std::string& prefix,
                     size_t slot_size);

        const std::string& GetPrefix() const noexcept;
        size_t GetSize() const noexcept;
        const Vec<const std::filesystem::path*>& GetSlots() const noexcept;
    };
}
