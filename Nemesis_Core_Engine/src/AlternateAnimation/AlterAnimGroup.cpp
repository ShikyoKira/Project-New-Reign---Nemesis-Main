#include "AlternateAnimation/AlterAnimGroup.h"
#include "AlternateAnimation/AlterAnimRepository.h"

#include "Utilities/Algorithm.h"

#include "Logger.h"
#include "NemesisInfo.h"

const std::filesystem::path& nemesis::AlterAnimGroup::CanonizePath(const std::filesystem::path& path)
{
    static Map<std::u8string, std::filesystem::path> PathCache;
    static std::mutex PathCacheMutex;

    std::scoped_lock<std::mutex> lock(PathCacheMutex);
    auto anim_path_it = PathCache.find(path.u8string());

    if (anim_path_it != PathCache.end()) return anim_path_it->second;

    return PathCache.insert({path.u8string(), std::filesystem::absolute(path)}).first->second;
}

nemesis::AlterAnimGroup::AlterAnimGroup(nemesis::AlterAnimRepository& repo,
                                        const nlohmann::json& group_info) noexcept
    : Repository(repo)
{
    Id   = group_info["Id"].get<size_t>();
    Name = group_info["Name"].get<std::string>();

    Logger::Log("Alternate Animations Group: " + Name + " (" + std::to_string(Id) + ")");

    auto animation_list = group_info["Animations"].get<VecStr>();

    for (auto& anim_path : animation_list)
    {
        std::string path = nemesis::to_utf8_string(CanonizePath(repo.GetAnimationDirectory() / anim_path));
        AlterAnimList.emplace_back(
            AlterAnimMap.emplace(path, std::make_unique<nemesis::AlterAnim>(*this, anim_path))
                .first->second.get());
    }
}

void nemesis::AlterAnimGroup::AddPrefix(const std::filesystem::path& dir_path,
                                        const std::string& prefix,
                                        size_t slot_size)
{
    static UMap<std::filesystem::path, size_t> path_to_length;

    auto data_path = NemesisInfo::DataPath();
    auto proj_path = data_path / Repository.GetProjectDirectory();
    auto itr = path_to_length.find(proj_path);
    size_t length
        = itr != path_to_length.end()
              ? itr->second
              : path_to_length.insert({proj_path, PATH_TO_STRING(proj_path).length() + 1}).first->second;
    std::filesystem::path relative_dir = PATH_TO_STRING(dir_path).substr(length);

    for (auto& alter_anim : AlterAnimMap)
    {
        alter_anim.second->AddAlternateSlots(relative_dir, dir_path, prefix, slot_size);
    }

    Repository.AddPrefix(prefix);
}

const nemesis::AlterAnimRepository& nemesis::AlterAnimGroup::GetRepository() const noexcept
{
    return Repository;
}

VecStr nemesis::AlterAnimGroup::GetAnimations() const
{
    VecStr list;

    for (auto& alter_anim : AlterAnimMap)
    {
        list.emplace_back(alter_anim.first);
    }

    return list;
}

const nemesis::AlterAnim* nemesis::AlterAnimGroup::GetAlternateSet(const std::string& anim_path) const
{
    auto canon_anim_path = nemesis::to_utf8_string(CanonizePath(anim_path));

    for (auto& alter_anim : AlterAnimMap)
    {
        if (!nemesis::iequals(alter_anim.first, canon_anim_path)) continue;

        return alter_anim.second.get();
    }

    return nullptr;
}

const Vec<const nemesis::AlterAnim*>& nemesis::AlterAnimGroup::GetAlterAnimationList() const noexcept
{
    return AlterAnimList;
}

size_t nemesis::AlterAnimGroup::GetId() const noexcept
{
    return Id;
}

const std::string& nemesis::AlterAnimGroup::GetName() const noexcept
{
    return Name;
}

std::string nemesis::AlterAnimGroup::GetVariableName() const noexcept
{
    return "Nemesis_AA_" + Name;
}
