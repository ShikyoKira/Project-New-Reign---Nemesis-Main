#include "Core/NObjectRepository.h"

#include <regex>
#include <iostream>

#include "Core/FNISaaPexFile.h"
#include "Core/ModRepository.h"
#include "Core/CompilationManager.h"

#include "Core/Hkx/HkxBehavior.h"
#include "Core/Hkx/HkxCharacter.h"

#include "Core/AnimationData/AnimationDataSingleFile.h"

#include "Core/AnimationSetData/AnimationSetDataSingleFile.h"

#include "Utilities/ThreadPool.h"
#include "Utilities/StringExtension.h"

#include "Logger.h"
#include "NemesisInfo.h"


namespace sf = std::filesystem;

void nemesis::NObjectRepository::ParseHkxFilesFromDirectory(const std::filesystem::path& dir,
                                                            nemesis::ThreadPool& thread_pool)
{
    if (!std::filesystem::exists(dir)) return;

    for (auto& entry : sf::directory_iterator(dir))
    {
        const sf::path& path = entry.path();

        if (entry.is_directory())
        {
            ParseHkxFilesFromDirectory(path, thread_pool);
            continue;
        }

        if (path.extension().wstring() != L".nemx") continue;

        if (nemesis::istarts_with(path.parent_path().stem().wstring(), L"characters"))
        {
            auto character = nemesis::HkxCharacter::ParseFromFile(path, thread_pool);
            Characters.emplace_back(std::move(character));
            continue;
        }

        auto behavior = nemesis::HkxBehavior::ParseFromFile(path, thread_pool);
        Behaviors.emplace_back(std::move(behavior));
    }
}

void nemesis::NObjectRepository::PatchNodeList(
    const Vec<const nemesis::ModPatch<nemesis::HkxNode>*> node_list, nemesis::ThreadPool& thread_pool)
{
    for (auto& node : node_list)
    {
        std::filesystem::path filepath = node->GetFilePath();
        std::filesystem::path relative_path
            = filepath.parent_path().wstring().substr(
                  (NemesisInfo::ExeDirectory() / L"mods" / node->GetModClass().GetCode()).wstring().length() + 1)
              + L".hkx";

        auto behavior = GetBehavior(relative_path);
        auto mod_code = node->GetModClass().GetCode();

        if (behavior)
        {
            thread_pool.enqueue(
                [behavior, filepath, node, mod_code]
                {
                    auto& m_node = node->GetContent();
                    auto* o_node = behavior->GetNodeById(filepath.stem().string());

                    if (o_node)
                    {
                        o_node->MatchAndUpdate(mod_code, m_node);
                        return;
                    }

                    behavior->AddModNode(mod_code, m_node.Clone());
                });
            continue;
        }

        auto character = GetCharacter(relative_path);

        if (!character) continue;

        thread_pool.enqueue(
            [character, filepath, node, mod_code]
            {
                auto& m_node = node->GetContent();
                auto* o_node = character->GetNodeById(filepath.stem().string());

                if (o_node)
                {
                    o_node->MatchAndUpdate(mod_code, m_node);
                    return;
                }

                character->AddModNode(node->GetModClass().GetCode(), m_node.Clone());
            });
    }
}

void nemesis::NObjectRepository::PatchHeaderList(
    const Vec<const nemesis::ModPatch<nemesis::AnimationDataProject::Headers>*> header_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_header : header_list)
    {
        auto filepath = m_header->GetFilePath();
        auto folder   = filepath.parent_path().stem().string();

        static std::regex rgx("^([^~]+)~([0-9]+)");
        std::smatch proj_match;

        if (!std::regex_match(folder, proj_match, rgx)) continue;

        auto proj_name = proj_match[1];
        size_t index   = std::stoul(proj_match[2]);

        auto project  = AnimDataSingleFile->GetProject(proj_name, index);
        auto mod_code = m_header->GetModClass().GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationDataProject>(proj_name);
            project       = uproject.get();
            AnimDataSingleFile->AddProject(std::move(uproject));
        }

        thread_pool.enqueue([m_header, project, mod_code]()
                            { project->MatchAndUpdateHeader(mod_code, m_header->GetContent()); });
    }
}

void nemesis::NObjectRepository::PatchClipData(
    const Vec<const nemesis::ModPatch<nemesis::AnimationDataClipData>*> clip_data_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_clip_data : clip_data_list)
    {
        auto filepath = m_clip_data->GetFilePath();
        auto folder   = filepath.parent_path().stem().string();

        static std::regex rgx("^([^~]+)~([0-9]+)");
        std::smatch proj_match;

        if (!std::regex_match(folder, proj_match, rgx)) continue;

        auto proj_name = proj_match[1];
        size_t index   = std::stoul(proj_match[2]);

        auto project  = AnimDataSingleFile->GetProject(proj_name, index);
        auto mod_code = m_clip_data->GetModClass().GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationDataProject>(proj_name);
            project       = uproject.get();
            AnimDataSingleFile->AddProject(std::move(uproject));
        }

        auto& m_data   = m_clip_data->GetContent();
        auto clip_data = project->GetClipData(m_data.GetName(), m_data.GetCode());

        if (clip_data)
        {
            thread_pool.enqueue([clip_data, mod_code, &m_data]
                                { clip_data->MatchAndUpdate(mod_code, m_data); });
            return;
        }

        auto* clip_data_ptr = &project->AddClipData(nullptr);

        thread_pool.enqueue(
            [m_clip_data, &m_data, clip_data_ptr]
            { *clip_data_ptr = m_data.Clone(m_clip_data->GetModClass(), m_clip_data->GetFilePath()); });
    }
}

void nemesis::NObjectRepository::PatchMotionData(
    const Vec<const nemesis::ModPatch<nemesis::AnimationDataMotionData>*> motion_data_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_motion_data : motion_data_list)
    {
        auto filepath = m_motion_data->GetFilePath();
        auto folder   = filepath.parent_path().stem().string();

        static std::regex rgx("^([^~]+)~([0-9]+)");
        std::smatch proj_match;

        if (!std::regex_match(folder, proj_match, rgx)) continue;

        auto proj_name = proj_match[1];
        size_t index   = std::stoul(proj_match[2]);
        auto project   = AnimDataSingleFile->GetProject(proj_name, index);
        auto mod_code  = m_motion_data->GetModClass().GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationDataProject>(proj_name);
            project       = uproject.get();
            AnimDataSingleFile->AddProject(std::move(uproject));
        }

        auto& m_data     = m_motion_data->GetContent();
        auto motion_data = project->GetMotionData(m_data.GetCode());

        if (motion_data)
        {
            thread_pool.enqueue([motion_data, mod_code, &m_data]
                                { motion_data->MatchAndUpdate(mod_code, m_data); });
            return;
        }

        auto* motion_data_ptr = &project->AddMotionData(nullptr);

        thread_pool.enqueue(
            [m_motion_data, &m_data, motion_data_ptr]
            { *motion_data_ptr = m_data.Clone(m_motion_data->GetModClass(), m_motion_data->GetFilePath()); });
    }
}

void nemesis::NObjectRepository::PatchStateData(
    const Vec<const nemesis::ModPatch<nemesis::AnimationSetDataState>*> statedata_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_state_data : statedata_list)
    {
        auto filepath  = m_state_data->GetFilePath();
        auto proj_name = filepath.parent_path().filename().string();
        nemesis::replace(proj_name, "~", "\\");

        auto project  = AnimSetDataSingleFile->GetProject(proj_name);
        auto mod_code = m_state_data->GetModClass().GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationSetDataProject>(proj_name);
            project       = uproject.get();
            AnimSetDataSingleFile->AddProject(std::move(uproject));
        }

        auto& m_data    = m_state_data->GetContent();
        auto state_data = project->GetState(m_data.GetName());

        if (state_data)
        {
            thread_pool.enqueue([state_data, mod_code, &m_data]
                                { state_data->MatchAndUpdate(mod_code, m_data); });
            return;
        }

        auto* state_data_ptr = &project->AddState(std::make_unique<nemesis::AnimationSetDataState>(m_data.GetName()));

        thread_pool.enqueue(
            [m_state_data, &m_data, state_data_ptr]
            { *state_data_ptr = m_data.Clone(m_state_data->GetModClass(), m_state_data->GetFilePath()); });
    }
}

nemesis::NObjectRepository::NObjectRepository(const std::filesystem::path& data_path)
{
    Logger::Log("Processing Core Objects...", true);

    nemesis::ThreadPool thread_pool;

    sf::path meshes_path = data_path / L"meshes";
    ParseHkxFilesFromDirectory(meshes_path, thread_pool);

    AnimDataSingleFile = nemesis::AnimationDataSingleFile::ParseFromFile(
        meshes_path / L"nemesis_animationdatasinglefile.txt", thread_pool);
    AnimSetDataSingleFile = nemesis::AnimationSetDataSingleFile::ParseFromFile(
        meshes_path / L"nemesis_animationsetdatasinglefile.txt", thread_pool);

    thread_pool.join_all();
}

nemesis::HkxBehavior*
nemesis::NObjectRepository::GetBehavior(const std::filesystem::path& relative_path) noexcept
{
    for (auto& behavior : Behaviors)
    {
        if (!nemesis::iequals(behavior->GetRelativePath().wstring(), relative_path.wstring())) continue;

        return behavior.get();
    }

    return nullptr;
}

const nemesis::HkxBehavior*
nemesis::NObjectRepository::GetBehavior(const std::filesystem::path& relative_path) const noexcept
{
    for (auto& behavior : Behaviors)
    {
        if (!nemesis::iequals(behavior->GetRelativePath().wstring(), relative_path.wstring())) continue;

        return behavior.get();
    }

    return nullptr;
}

nemesis::HkxCharacter*
nemesis::NObjectRepository::GetCharacter(const std::filesystem::path& relative_path) noexcept
{
    for (auto& character : Characters)
    {
        if (!nemesis::iequals(character->GetRelativePath().wstring(), relative_path.wstring())) continue;

        return character.get();
    }

    return nullptr;
}

const nemesis::HkxCharacter*
nemesis::NObjectRepository::GetCharacter(const std::filesystem::path& relative_path) const noexcept
{
    for (auto& character : Characters)
    {
        if (!nemesis::iequals(character->GetRelativePath().wstring(), relative_path.wstring())) continue;

        return character.get();
    }

    return nullptr;
}

nemesis::AnimationDataSingleFile* nemesis::NObjectRepository::GetAnimDataSingleFile() noexcept
{
    return AnimDataSingleFile.get();
}

const nemesis::AnimationDataSingleFile* nemesis::NObjectRepository::GetAnimDataSingleFile() const noexcept
{
    return AnimDataSingleFile.get();
}

nemesis::AnimationSetDataSingleFile* nemesis::NObjectRepository::GetAnimSetDataSingleFile() noexcept
{
    return AnimSetDataSingleFile.get();
}

const nemesis::AnimationSetDataSingleFile*
nemesis::NObjectRepository::GetAnimSetDataSingleFile() const noexcept
{
    return AnimSetDataSingleFile.get();
}

void nemesis::NObjectRepository::Patch(const nemesis::ModRepository& mod_repo)
{
    Logger::Log(L"Patching Core Objects with Mod Objects...", true);

    auto mod_class_list = mod_repo.GetModClassList();
    nemesis::ThreadPool thread_pool;

    for (auto& mod_class : mod_class_list)
    {
        PatchNodeList(mod_class->GetNodeList(), thread_pool);
        PatchStateData(mod_class->GetStateList(), thread_pool);
        PatchHeaderList(mod_class->GetAnimDataHeaderList(), thread_pool);
        PatchClipData(mod_class->GetClipDataList(), thread_pool);
        PatchMotionData(mod_class->GetMotionDataList(), thread_pool);
    }

    thread_pool.join_all();
}

void nemesis::NObjectRepository::Compile(nemesis::CompilationManager& manager) const
{
    Logger::Log(L"Compiling Core Objects...", true);

    manager.ClearCheckSum();
    nemesis::ThreadPool cthread_pool;
    nemesis::ThreadPool thread_pool;

    for (auto& character : Characters)
    {
        auto& state = manager.CreateCompileState(character->GetFilePath());
        cthread_pool.enqueue(
            [&character, &state]()
            {
                character->ScheduleCompileFile(
                    state, NemesisInfo::OutputPlatform(), NemesisInfo::OutputVersion(), true);
            });
    }

    cthread_pool.join_all();
    const nemesis::HkxBehavior* build_info_bhv;

    for (auto& behavior : Behaviors)
    {
        if (nemesis::iequals(behavior->GetTargetPath().filename().wstring(), L"build_info.hkx"))
        {
            build_info_bhv = behavior.get();
            continue;
        }

        auto& state = manager.CreateCompileState(behavior->GetFilePath());
        thread_pool.enqueue([&behavior, &state]()
            {
                behavior->ScheduleCompileFile(
                    state, NemesisInfo::OutputPlatform(), NemesisInfo::OutputVersion(), true);
            });
    }

    auto& adsf_state = manager.CreateCompileState(AnimDataSingleFile->GetFilePath());
    thread_pool.enqueue([this, &adsf_state]() { AnimDataSingleFile->CompileFile(adsf_state); });

    auto& asdsf_state = manager.CreateCompileState(AnimSetDataSingleFile->GetFilePath());
    thread_pool.enqueue([this, &asdsf_state]() { AnimSetDataSingleFile->CompileFile(asdsf_state); });
    thread_pool.join_all();

    if (build_info_bhv != nullptr)
    {
        auto& state = manager.CreateCompileState(build_info_bhv->GetFilePath());
        build_info_bhv->ScheduleCompileFile(
            state, NemesisInfo::OutputPlatform(), NemesisInfo::OutputVersion(), true);
    }

    nemesis::FNISaaPexFile pex_file(NemesisInfo::ExeDirectory() / L"alternate_animations"
                                    / L"AlternateAnimationsScript.pex");
    pex_file.Patch(manager);
    pex_file.OutputPexFile(NemesisInfo::DataPath() / L"Scripts" / L"Nemesis_aa2.pex");

    for (auto& character : Characters)
    {
        character->WaitForCompleteCompilation();
    }

    for (auto& behavior : Behaviors)
    {
        behavior->WaitForCompleteCompilation();
    }

    Logger::Log("Compile Complete", true);
}
