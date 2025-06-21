#include "AlternateAnimation/AlterAnimRepository.h"

#include <fstream>
#include <iostream>

#include "Utilities/Algorithm.h"

#include "Logger.h"
#include "NemesisInfo.h"


using Json = nlohmann::json;

const std::filesystem::path& nemesis::AlterAnimRepository::CanonizePath(const std::filesystem::path& path)
{
    static UMap<std::wstring, std::filesystem::path> PathCache;
    static std::mutex PathCacheMutex;

    std::scoped_lock<std::mutex> lock(PathCacheMutex);
    auto anim_path_it = PathCache.find(path);

    if (anim_path_it != PathCache.end()) return anim_path_it->second;

    return PathCache.insert({path, std::filesystem::absolute(path)}).first->second;
}

nemesis::AlterAnimRepository::AlterAnimRepository(const std::filesystem::path& alter_anim_json)
{
    Logger::Log("Processing Alternate Animations...");

    Json aa_settings;
    std::fstream json_file(alter_anim_json);
    json_file >> aa_settings;

    ProjectDirectory = aa_settings["ProjectDirectory"].get<std::string>();
    AnimationDirectory = aa_settings["AnimationDirectory"].get<std::string>();

    auto characters = aa_settings["Characters"].get<VecStr>();
    auto behaviors  = aa_settings["Behaviors"].get<VecStr>();
    auto data_path  = NemesisInfo::DataPath();

    for (auto& character : characters)
    {
        Characters.emplace_back(CanonizePath(data_path / ProjectDirectory / character));
    }

    for (auto& behavior : behaviors)
    {
        Behaviors.emplace_back(CanonizePath(data_path / ProjectDirectory / behavior));
    }

    auto& elements = aa_settings["Groups"];

    for (Json::iterator it = elements.begin(); it != elements.end(); ++it)
    {
        auto& group_body = *it;
        auto& alter_anim_group
            = GroupList.emplace_back(std::make_unique<nemesis::AlterAnimGroup>(*this, group_body));
        GroupListCache.emplace_back(alter_anim_group.get());

        for (auto& anim : alter_anim_group->GetAnimations())
        {
            GroupAnimationMap[nemesis::to_lower_copy(CanonizePath(anim).string())] = alter_anim_group.get();
        }
    }
}

bool nemesis::AlterAnimRepository::IsSupportedBehavior(const std::filesystem::path& behavior_path) const
{
    auto canon_behavior_path = CanonizePath(behavior_path);

    for (auto& behavior : Behaviors)
    {
        if (!nemesis::iequals(canon_behavior_path, behavior)) continue;

        return true;
    }

    return false;
}

bool nemesis::AlterAnimRepository::IsSupportedCharacter(const std::filesystem::path& character_path) const
{
    auto canon_character_path = CanonizePath(character_path);

    for (auto& character : Characters)
    {
        if (!nemesis::iequals(canon_character_path, character)) continue;

        return true;
    }

    return false;
}

const std::filesystem::path& nemesis::AlterAnimRepository::GetProjectDirectory() const noexcept
{
    return ProjectDirectory;
}

const std::filesystem::path& nemesis::AlterAnimRepository::GetAnimationDirectory() const noexcept
{
    return AnimationDirectory;
}

void nemesis::AlterAnimRepository::AddPrefix(const std::string& name)
{
    std::scoped_lock<std::mutex> lock(PrefixListMutex);

    for (auto& prefix : PrefixList)
    {
        if (!nemesis::iequals(prefix.GetName(), name)) continue;

        return;
    }

    PrefixList.emplace_back(PrefixList.size() + 1, name);
}

const Vec<nemesis::AlterAnimPrefix>& nemesis::AlterAnimRepository::GetPrefixList() const noexcept
{
    return PrefixList;
}

const nemesis::AlterAnimPrefix* nemesis::AlterAnimRepository::GetPrefixById(size_t id) const noexcept
{
    std::scoped_lock<std::mutex> lock(PrefixListMutex);

    for (auto& prefix : PrefixList)
    {
        if (prefix.GetId() != id) continue;

        return &prefix;
    }

    return nullptr;
}

const nemesis::AlterAnimPrefix*
nemesis::AlterAnimRepository::GetPrefixByName(const std::string& name) const noexcept
{
    std::scoped_lock<std::mutex> lock(PrefixListMutex);

    for (auto& prefix : PrefixList)
    {
        if (!nemesis::iequals(prefix.GetName(), name)) continue;

        return &prefix;
    }

    return nullptr;
}

const Vec<const nemesis::AlterAnimGroup*>&
nemesis::AlterAnimRepository::GetAlterAnimGroupList() const noexcept
{
    return GroupListCache;
}

nemesis::AlterAnimGroup* nemesis::AlterAnimRepository::GetAlterAnimGroupByName(const std::string& group_name)
{
    for (auto& group : GroupList)
    {
        if (!nemesis::iequals(group->GetName(), group_name)) continue;

        return group.get();
    }

    return nullptr;
}

const nemesis::AlterAnimGroup*
nemesis::AlterAnimRepository::GetAlterAnimGroupByName(const std::string& group_name) const
{
    for (auto& group : GroupList)
    {
        if (!nemesis::iequals(group->GetName(), group_name)) continue;

        return group.get();
    }

    return nullptr;
}

const nemesis::AlterAnimGroup*
nemesis::AlterAnimRepository::GetAlterAnimGroupByAnimation(const std::string& anim_path) const
{
    auto itr = GroupAnimationMap.find(nemesis::to_lower_copy(CanonizePath(anim_path).string()));

    if (itr == GroupAnimationMap.end()) return nullptr;

    return itr->second;
}

Vec<std::filesystem::path> nemesis::AlterAnimRepository::GetAlterAnimationList() const
{
    std::scoped_lock<std::mutex> lock(AnimationListCacheMutex);

    if (!AnimationListCache.empty()) return AnimationListCache;

    for (auto& group : GroupList)
    {
        for (auto& aa : group->GetAlterAnimationList())
        {
            for (auto& aa_set : aa->GetAnimationSetList())
            {
                for (auto& slot : aa_set->GetSlots())
                {
                    if (!slot) continue;

                    AnimationListCache.emplace_back(*slot);
                }
            }
        }
    }

    return AnimationListCache;
}
