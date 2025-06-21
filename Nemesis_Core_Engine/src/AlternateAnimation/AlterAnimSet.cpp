#include "AlternateAnimation/AlterAnimSet.h"

const std::filesystem::path& nemesis::AlterAnimSet::CanonizePath(const std::filesystem::path& path)
{
    static Map<std::wstring, std::filesystem::path> PathCache;
    static std::mutex PathCacheMutex;

    std::scoped_lock<std::mutex> lock(PathCacheMutex);
    auto anim_path_it = PathCache.find(path);

    if (anim_path_it != PathCache.end()) return anim_path_it->second;

    return PathCache.insert({path, std::filesystem::absolute(path)}).first->second;
}

nemesis::AlterAnimSet::AlterAnimSet(const std::filesystem::path& relative_dir_path,
                                    const std::filesystem::path& dir_path,
                                    const std::filesystem::path& anim_path,
                                    const std::string& prefix,
                                    size_t slot_size)
    : Prefix(prefix)
{
    for (size_t i = 0; i < slot_size; i++)
    {
        std::filesystem::path* slot_ptr = nullptr;
        auto relative_path              = prefix + std::to_string(i) + "_" + anim_path.filename().string();
        auto absolute_path              = dir_path / relative_path;

        if (!std::filesystem::exists(absolute_path))
        {
            slot_ptr = Slots.emplace_back(nullptr).get();
        }
        else
        {
            slot_ptr = Slots
                           .emplace_back(
                               std::make_unique<std::filesystem::path>(relative_dir_path / relative_path))
                           .get();
        }

        CacheSlots.emplace_back(slot_ptr);
    }
}

const std::string& nemesis::AlterAnimSet::GetPrefix() const noexcept
{
    return Prefix;
}

size_t nemesis::AlterAnimSet::GetSize() const noexcept
{
    return Slots.size();
}

const Vec<const std::filesystem::path*>& nemesis::AlterAnimSet::GetSlots() const noexcept
{
    return CacheSlots;
}
