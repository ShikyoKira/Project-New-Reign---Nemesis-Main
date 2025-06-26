#include <fstream>

#include "ExAnimation/ExAnimationProject.h"

#include "nlohmann/json.hpp"

#include "Utilities/Algorithm.h"

#include "Logger.h"
#include "NemesisInfo.h"

using Json   = nlohmann::json;
namespace sf = std::filesystem;

const std::filesystem::path& nemesis::ExAnimationProject::CanonizePath(const std::filesystem::path& path)
{
    static UMap<std::filesystem::path, std::filesystem::path> PathCache;
    static std::mutex PathCacheMutex;

    std::scoped_lock<std::mutex> lock(PathCacheMutex);
    auto anim_path_it = PathCache.find(path);

    if (anim_path_it != PathCache.end()) return anim_path_it->second;

    return PathCache.insert({path, sf::absolute(path)}).first->second;
}

void nemesis::ExAnimationProject::PopulateProjectDataTo(nemesis::ExAnimationProject& project,
                                                        const std::filesystem::path& json_path)
{
    Json json_project;
    std::fstream json_file(json_path);
    json_file >> json_project;

    project.Name = json_project["Name"].get<std::string>();
    Logger::Log("ExAnimation Project Name: " + project.Name);

    project.WorkingDirectory   = json_path.parent_path();
    project.AnimationDirectory = json_project["AnimationDirectory"].get<std::string>();

    VecStr characters = json_project["Characters"].get<VecStr>();
    VecStr behaviors  = json_project["Behaviors"].get<VecStr>();

    for (auto& character : characters)
    {
        auto& path = project.Characters.emplace_back(project.WorkingDirectory / std::move(character));
        project.CanonCharacters.insert(CanonizePath(nemesis::to_lower_copy(path)));
    }

    for (auto& behavior : behaviors)
    {
        auto& path = project.Behaviors.emplace_back(project.WorkingDirectory / std::move(behavior));
        project.CanonBehaviors.insert(CanonizePath(nemesis::to_lower_copy(path)));
    }

    project.LoadExAnim();
}

void nemesis::ExAnimationProject::LoadExAnim()
{
    auto anim_dir = WorkingDirectory / AnimationDirectory;

    if (!sf::exists(anim_dir) || !sf::is_directory(anim_dir))
    {
        throw std::runtime_error("Invalid Animation directory for ExAnimation repository");
    }

    const sf::path pcea_dir      = anim_dir / LITERAL_PATH("Nemesis_ExAnimation");
    const sf::path pcea_anim_dir = NemesisInfo::PatchOutputPath(pcea_dir / LITERAL_PATH("ExAnimations"));

    sf::remove_all(pcea_anim_dir);
    sf::create_directories(pcea_anim_dir);

    for (auto entry : sf::directory_iterator(pcea_dir))
    {
        auto path = entry.path();

        if (nemesis::iequals(PATH_TO_STRING(path.stem()), LITERAL_PATH("exanimations"))) continue;

        auto ex_anim = std::make_unique<nemesis::ExAnimationPack>(
            path,
            AnimationDirectory,
            AnimationDirectory / LITERAL_PATH("Nemesis_ExAnimation") / LITERAL_PATH("ExAnimations"));

        for (auto& request : ex_anim->GetRequestList())
        {
            // Required for VFS to work consistently
            std::ofstream dst(pcea_anim_dir / request->GetExAnimPath().filename(),
                              std::ios::in | std::ios::binary);
            std::ifstream src(pcea_dir / path.filename() / request->GetAnimPath().filename(),
                              std::ios::out | std::ios::binary);
            dst << src.rdbuf();
        }

        auto itr = ExAnimMap.find(ex_anim->GetOrder());

        if (itr != ExAnimMap.end())
        {
            itr->second = std::move(ex_anim);
            continue;
        }

        ExAnimMap.insert({ex_anim->GetOrder(), std::move(ex_anim)});
    }
}

bool nemesis::ExAnimationProject::MatchBehavior(const std::filesystem::path& hkxfile_path) const
{
    return CanonBehaviors.find(nemesis::to_lower_copy(CanonizePath(hkxfile_path))) != CanonBehaviors.end();
}

bool nemesis::ExAnimationProject::MatchCharacter(const std::filesystem::path& hkxfile_path) const
{
    return CanonCharacters.find(nemesis::to_lower_copy(CanonizePath(hkxfile_path))) != CanonCharacters.end();
}

const std::string& nemesis::ExAnimationProject::GetName() const noexcept
{
    return Name;
}

Vec<const nemesis::ExAnimationRequest*>
nemesis::ExAnimationProject::GetExAnimationRequest(const std::filesystem::path& anim_path) const
{
    Vec<const nemesis::ExAnimationRequest*> request_list;
    auto canon_anim_path = CanonizePath(anim_path);

    for (auto& each : ExAnimMap)
    {
        auto* request = each.second->GetExAnimRequest(canon_anim_path);

        if (!request) continue;

        request_list.emplace_back(request);
    }

    return request_list;
}

Vec<const nemesis::ExAnimationPack*> nemesis::ExAnimationProject::GetExAnimationPackList() const
{
    Vec<const nemesis::ExAnimationPack*> list;

    for (auto& each : ExAnimMap)
    {
        list.emplace_back(each.second.get());
    }

    return list;
}

UPtr<nemesis::LinkedExAnimList>
nemesis::ExAnimationProject::GetLinkedExAnim(const std::filesystem::path& anim_path) const
{
    auto requests = GetExAnimationRequest(anim_path);

    if (requests.empty()) return nullptr;

    UPtr<nemesis::LinkedExAnimList> linked_ex_anim
        = std::make_unique<nemesis::LinkedExAnimList>(*requests.front());

    for (auto it = requests.rbegin() + 1; it != requests.rend(); ++it)
    {
        linked_ex_anim->Add(**it);
    }

    return linked_ex_anim;
}

Vec<const std::filesystem::path*> nemesis::ExAnimationProject::GetExAnimationList() const
{
    Vec<const sf::path*> anim_list;

    for (auto& each : ExAnimMap)
    {
        for (auto& request : each.second->GetRequestList())
        {
            anim_list.emplace_back(&request->GetExAnimPath());
        }
    }

    return anim_list;
}

UPtr<nemesis::ExAnimationProject>
nemesis::ExAnimationProject::ParseExAnimationProject(const std::filesystem::path& json_path)
{
    auto ex_anim_project = std::make_unique<nemesis::ExAnimationProject>();
    PopulateProjectDataTo(*ex_anim_project, json_path);
    return ex_anim_project;
}

UPtr<nemesis::ExAnimationProject>
nemesis::ExAnimationProject::ParseExAnimationProject(const std::filesystem::path& json_path,
                                                     nemesis::ThreadPool& thread_pool)
{
    auto ex_anim_project = std::make_unique<nemesis::ExAnimationProject>();
    thread_pool.enqueue([proj_ptr = ex_anim_project.get(), json_path]
                        { PopulateProjectDataTo(*proj_ptr, json_path); });
    return ex_anim_project;
}
