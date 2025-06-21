#include "ExAnimation/ExAnimationRepository.h"

#include <iostream>

#include "Utilities/Algorithm.h"
#include "Utilities/ThreadPool.h"

#include "Logger.h"


void nemesis::ExAnimationRepository::AddAllProjectsFromFolderRecursively(const std::filesystem::path& dir,
                                                                         nemesis::ThreadPool& thread_pool)
{
    if (!std::filesystem::exists(dir)) return;

    for (auto entry : std::filesystem::directory_iterator(dir))
    {
        auto path = entry.path();

        if (entry.is_directory())
        {
            AddAllProjectsFromFolderRecursively(path, thread_pool);
            continue;
        }

        if (!nemesis::iequals(path.filename().wstring(), L"exanimation.json")) continue;

        auto project = nemesis::ExAnimationProject::ParseExAnimationProject(path, thread_pool);
        ProjectList.emplace_back(std::move(project));
    }
}

nemesis::ExAnimationRepository::ExAnimationRepository(const std::filesystem::path& meshes_dir)
{
    Logger::Log("Processing ExAnimation...", true);

    nemesis::ThreadPool thread_pool;
    AddAllProjectsFromFolderRecursively(meshes_dir, thread_pool);
    thread_pool.join_all();
}

const nemesis::ExAnimationProject*
nemesis::ExAnimationRepository::GetProjectByBehavior(const std::filesystem::path& hkxfile_path) const
{
    for (auto& project : ProjectList)
    {
        if (!project->MatchBehavior(hkxfile_path)) continue;

        return project.get();
    }

    return nullptr;
}

const nemesis::ExAnimationProject*
nemesis::ExAnimationRepository::GetProjectByCharacter(const std::filesystem::path& hkxfile_path) const
{
    for (auto& project : ProjectList)
    {
        if (!project->MatchCharacter(hkxfile_path)) continue;

        return project.get();
    }

    return nullptr;

}

Vec<const std::filesystem::path*>
nemesis::ExAnimationRepository::GetExAnimationList(const std::filesystem::path& character_filepath) const
{
    auto proj = GetProjectByCharacter(character_filepath);

    if (!proj) return Vec<const std::filesystem::path*>();

    return proj->GetExAnimationList();
}

Vec<const nemesis::ExAnimationProject*> nemesis::ExAnimationRepository::GetProjectList() const
{
    Vec<const nemesis::ExAnimationProject*> project_list;

    for (auto& project : ProjectList)
    {
        project_list.emplace_back(project.get());
    }

    return project_list;
}

void nemesis::ExAnimationRepository::OutputExAnimationInfo() const
{
    for (auto& project : ProjectList)
    {
        std::cout << "ExAnimation Project Name: " << project->GetName() << std::endl;

        for (auto& pack : project->GetExAnimationPackList())
        {
            std::cout << "ExAnimations: " << pack->GetName() << " (" << pack->GetOrder() << ")" << std::endl;
        }

        std::cout << "\n" << std::endl;
    }
}
