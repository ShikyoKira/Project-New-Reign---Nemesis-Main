#include "Core/NObjectRepository.h"

#include <iostream>
#include <regex>

#include "Core/CacheManager.h"
#include "Core/CompilationManager.h"
#include "Core/FNISaaPexFile.h"
#include "Core/ModRepository.h"

#include "Core/Hkx/HkxBehavior.h"
#include "Core/Hkx/HkxCharacter.h"

#include "Core/AnimationData/AnimationDataSingleFile.h"

#include "Core/AnimationSetData/AnimationSetDataSingleFile.h"

#include "Utilities/StringExtension.h"
#include "Utilities/ThreadPool.h"

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

        if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".nemx"))) continue;

        if (nemesis::istarts_with(PATH_TO_STRING(path.parent_path().stem()), LITERAL_PATH("characters")))
        {
            auto character = nemesis::HkxCharacter::ParseFromFile(path, thread_pool);
            CharactersPathMap.insert(
                {nemesis::to_lower_copy(PATH_TO_STRING(character->GetFilePath())), character.get()});
            Characters.emplace_back(std::move(character));
            continue;
        }

        auto behavior = nemesis::HkxBehavior::ParseFromFile(path, thread_pool);
        BehaviorsPathMap.insert(
            {nemesis::to_lower_copy(PATH_TO_STRING(behavior->GetFilePath())), behavior.get()});
        Behaviors.emplace_back(std::move(behavior));
    }
}

void nemesis::NObjectRepository::PatchNodeList(
    const Vec<const nemesis::ModPatch<nemesis::HkxNode>*> node_list, nemesis::ThreadPool& thread_pool)
{
    size_t mod_dir_length = PATH_TO_STRING(NemesisInfo::ExeDirectory() / LITERAL_PATH("mods")).length();

    for (auto& node : node_list)
    {
        auto& mod_code = node->GetModClass().GetCode();
        size_t dir_length = mod_dir_length + mod_code.length() + 1;

        auto& filepath = node->GetFilePath();
        std::filesystem::path relative_path
            = PATH_TO_STRING(filepath.parent_path()).substr(dir_length + 1) + LITERAL_PATH(".nemx");

        auto* behavior = GetBehavior(relative_path);
        auto node_id   = nemesis::to_utf8_string(filepath.stem());

        if (behavior)
        {
            thread_pool.enqueue(
                [behavior, node_id, node, &mod_code]
                {
                    auto& m_node = node->GetContent();
                    auto* o_node = behavior->GetNodeById(node_id);

                    if (o_node)
                    {
                        o_node->MatchAndUpdate(mod_code, m_node);
                        behavior->AddModNode(mod_code);
                        return;
                    }

                    // Pre-creation of node is not required because nodes are sorted in hkxfile
                    behavior->AddModNode(mod_code, m_node.Clone());
                });
            continue;
        }

        auto character = GetCharacter(relative_path);

        if (!character) continue;

        thread_pool.enqueue(
            [character, node_id, node, &mod_code]
            {
                auto& m_node = node->GetContent();
                auto* o_node = character->GetNodeById(node_id);

                if (o_node)
                {
                    o_node->MatchAndUpdate(mod_code, m_node);
                    character->AddModNode(mod_code);
                    return;
                }

                // Pre-creation of node is not required because nodes are sorted in hkxfile
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
        std::string folder = nemesis::to_utf8_string(filepath.parent_path().stem());

        static std::regex rgx("^([^~]+)~([0-9]+)");
        std::smatch proj_match;

        if (!std::regex_match(folder, proj_match, rgx)) continue;

        auto proj_name = proj_match[1];
        size_t index   = std::stoul(proj_match[2]);

        auto* project  = AnimDataSingleFile->GetProject(proj_name, index);
        auto& mod_code = m_header->GetModClass().GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationDataProject>(proj_name);
            project       = uproject.get();
            AnimDataSingleFile->AddProject(std::move(uproject));
            AnimDataSingleFile->AddModNode(mod_code);
        }

        thread_pool.enqueue(
            [this, m_header, project, &mod_code]()
            {
                project->MatchAndUpdateHeader(mod_code, m_header->GetContent());
                AnimDataSingleFile->AddModNode(mod_code);
            });
    }
}

void nemesis::NObjectRepository::PatchClipData(
    const Vec<const nemesis::ModPatch<nemesis::AnimationDataClipData>*> clip_data_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_clip_data : clip_data_list)
    {
        auto filepath = m_clip_data->GetFilePath();
        std::string folder = nemesis::to_utf8_string(filepath.parent_path().stem());

        static std::regex rgx("^([^~]+)~([0-9]+)");
        std::smatch proj_match;

        if (!std::regex_match(folder, proj_match, rgx)) continue;

        auto proj_name = proj_match[1];
        size_t index   = std::stoul(proj_match[2]);

        auto* project  = AnimDataSingleFile->GetProject(proj_name, index);
        auto& mod_class = m_clip_data->GetModClass();
        auto& mod_code  = mod_class.GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationDataProject>(proj_name);
            project       = uproject.get();
            AnimDataSingleFile->AddProject(std::move(uproject));
            AnimDataSingleFile->AddModNode(mod_code);
        }

        auto& m_data    = m_clip_data->GetContent();
        auto* clip_data = project->GetClipData(m_data.GetName(), m_data.GetCode());

        if (clip_data)
        {
            thread_pool.enqueue(
                [this, clip_data, &mod_code, &m_data]
                {
                    clip_data->MatchAndUpdate(mod_code, m_data);
                    AnimDataSingleFile->AddModNode(mod_code);
                });
            return;
        }

        // Pre-create the object to lock in its memory slot to ensure the order
        // So that the output is deterministic
        auto* clip_data_ptr = &project->AddClipData(nullptr);
        thread_pool.enqueue(
            [this, filepath, &mod_class, &mod_code, &m_data, clip_data_ptr]
            {
                *clip_data_ptr = m_data.Clone(mod_class, filepath);
                AnimDataSingleFile->AddModNode(mod_code);
            });
    }
}

void nemesis::NObjectRepository::PatchMotionData(
    const Vec<const nemesis::ModPatch<nemesis::AnimationDataMotionData>*> motion_data_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_motion_data : motion_data_list)
    {
        auto filepath = m_motion_data->GetFilePath();
        std::string folder = nemesis::to_utf8_string(filepath.parent_path().stem());

        static std::regex rgx("^([^~]+)~([0-9]+)");
        std::smatch proj_match;

        if (!std::regex_match(folder, proj_match, rgx)) continue;

        auto proj_name  = proj_match[1];
        size_t index    = std::stoul(proj_match[2]);
        auto* project   = AnimDataSingleFile->GetProject(proj_name, index);
        auto& mod_class = m_motion_data->GetModClass();
        auto& mod_code  = mod_class.GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationDataProject>(proj_name);
            project       = uproject.get();
            AnimDataSingleFile->AddProject(std::move(uproject));
            AnimDataSingleFile->AddModNode(mod_code);
        }

        auto& m_data      = m_motion_data->GetContent();
        auto* motion_data = project->GetMotionData(m_data.GetCode());

        if (motion_data)
        {
            thread_pool.enqueue(
                [this, motion_data, &mod_code, &m_data]
                {
                    motion_data->MatchAndUpdate(mod_code, m_data);
                    AnimDataSingleFile->AddModNode(mod_code);
                });
            return;
        }

        // Pre-create the object to lock in its memory slot to ensure the order
        // So that the output is deterministic
        auto* motion_data_ptr = &project->AddMotionData(nullptr);
        thread_pool.enqueue(
            [this, filepath, &mod_class, &mod_code, &m_data, motion_data_ptr]
            {
                *motion_data_ptr = m_data.Clone(mod_class, filepath);
                AnimDataSingleFile->AddModNode(mod_code);
            });
    }
}

void nemesis::NObjectRepository::PatchStateData(
    const Vec<const nemesis::ModPatch<nemesis::AnimationSetDataState>*> statedata_list,
    nemesis::ThreadPool& thread_pool)
{
    for (auto& m_state_data : statedata_list)
    {
        auto filepath = m_state_data->GetFilePath();
        std::string proj_name = nemesis::to_utf8_string(filepath.parent_path().filename());
        nemesis::replace(proj_name, "~", "\\");

        auto* project  = AnimSetDataSingleFile->GetProject(proj_name);
        auto& mod_class = m_state_data->GetModClass();
        auto& mod_code  = mod_class.GetCode();

        if (!project)
        {
            auto uproject = std::make_unique<nemesis::AnimationSetDataProject>(proj_name);
            project       = uproject.get();
            AnimSetDataSingleFile->AddProject(std::move(uproject));
            AnimSetDataSingleFile->AddModNode(mod_code);
        }

        auto& m_data     = m_state_data->GetContent();
        auto* state_data = project->GetState(m_data.GetName());

        if (state_data)
        {
            thread_pool.enqueue(
                [this, state_data, &mod_code, &m_data]
                {
                    state_data->MatchAndUpdate(mod_code, m_data);
                    AnimSetDataSingleFile->AddModNode(mod_code);
                });
            return;
        }

        auto* state_data_ptr
            = &project->AddState(std::make_unique<nemesis::AnimationSetDataState>(m_data.GetName()));
        thread_pool.enqueue(
            [this, filepath, &mod_class, &mod_code, &m_data, state_data_ptr]
            {
                *state_data_ptr = m_data.Clone(mod_class, filepath);
                AnimSetDataSingleFile->AddModNode(mod_code);
            });
    }
}

nemesis::NObjectRepository::NObjectRepository(const std::filesystem::path& data_path)
{
    Logger::Log("Processing Core Objects...", true);

    nemesis::ThreadPool thread_pool;

    sf::path meshes_path = data_path / LITERAL_PATH("meshes");
    ParseHkxFilesFromDirectory(meshes_path, thread_pool);

    sf::path adsf_path = meshes_path / LITERAL_PATH("nemesis_animationdatasinglefile.txt");

    if (sf::exists(adsf_path))
    {
        AnimDataSingleFile = nemesis::AnimationDataSingleFile::ParseFromFile(adsf_path, thread_pool);
    }

    sf::path asdsf_path = meshes_path / LITERAL_PATH("nemesis_animationsetdatasinglefile.txt");

    if (sf::exists(asdsf_path))
    {
        AnimSetDataSingleFile = nemesis::AnimationSetDataSingleFile::ParseFromFile(asdsf_path, thread_pool);
    }

    thread_pool.join_all();
}

nemesis::HkxBehavior*
nemesis::NObjectRepository::GetBehavior(const std::filesystem::path& relative_path) noexcept
{
    auto fullpath = nemesis::to_lower_copy(PATH_TO_STRING(NemesisInfo::DataPath() / relative_path));
    auto itr      = BehaviorsPathMap.find(fullpath);

    if (itr != BehaviorsPathMap.end()) return itr->second;

    return nullptr;
}

const nemesis::HkxBehavior*
nemesis::NObjectRepository::GetBehavior(const std::filesystem::path& relative_path) const noexcept
{
    auto fullpath = nemesis::to_lower_copy(PATH_TO_STRING(NemesisInfo::DataPath() / relative_path));
    auto itr      = BehaviorsPathMap.find(fullpath);

    if (itr != BehaviorsPathMap.end()) return itr->second;

    return nullptr;
}

nemesis::HkxCharacter*
nemesis::NObjectRepository::GetCharacter(const std::filesystem::path& relative_path) noexcept
{
    auto fullpath = nemesis::to_lower_copy(PATH_TO_STRING(NemesisInfo::DataPath() / relative_path));
    auto itr      = CharactersPathMap.find(fullpath);

    if (itr != CharactersPathMap.end()) return itr->second;

    return nullptr;
}

const nemesis::HkxCharacter*
nemesis::NObjectRepository::GetCharacter(const std::filesystem::path& relative_path) const noexcept
{
    auto fullpath = nemesis::to_lower_copy(PATH_TO_STRING(NemesisInfo::DataPath() / relative_path));
    auto itr      = CharactersPathMap.find(fullpath);

    if (itr != CharactersPathMap.end()) return itr->second;

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
    Logger::Log("Patching Core Objects with Mod Objects...", true);

    auto mod_class_list = mod_repo.GetModClassList();
    nemesis::ThreadPool thread_pool;

    for (auto& mod_class : mod_class_list)
    {
        PatchNodeList(mod_class->GetNodeList(), thread_pool);
        PatchStateData(mod_class->GetStateList(), thread_pool);
        PatchHeaderList(mod_class->GetAnimDataHeaderList(), thread_pool);
        PatchClipData(mod_class->GetClipDataList(), thread_pool);
        PatchMotionData(mod_class->GetMotionDataList(), thread_pool);

        // 2 different mod class cannot share the same multithreading environment to ensure its order
        // So that the output is deterministic
        thread_pool.wait_for_all();
    }

    thread_pool.join_all();
}

void nemesis::NObjectRepository::Compile(nemesis::CompilationManager& manager,
                                         std::function<void(int, int)> prgs_callback) const
{
    Logger::Log("Compiling Core Objects...", true);

    manager.ClearCheckSum();
    nemesis::ThreadPool cthread_pool;
    nemesis::ThreadPool thread_pool;
    int TotalObjects = Characters.size() + Behaviors.size() + 1 + (AnimDataSingleFile != nullptr)
                       + (AnimSetDataSingleFile != nullptr);
    std::atomic<int> CompiledObjectCounter(0);

    for (auto& character : Characters)
    {
        auto& state = manager.CreateCompileState(character->GetFilePath());
        cthread_pool.priority_enqueue(
            character->GetSize(),
            [&character, &state, &CompiledObjectCounter, &prgs_callback, TotalObjects]()
            {
                character->ScheduleCompileFile(
                    state, NemesisInfo::OutputPlatform(), NemesisInfo::OutputVersion(), true);
                prgs_callback(++CompiledObjectCounter, TotalObjects);
            });
    }

    cthread_pool.join_all();
    const nemesis::HkxBehavior* build_info_bhv;

    for (auto& behavior : Behaviors)
    {
        if (nemesis::iequals(PATH_TO_STRING(behavior->GetTargetPath().filename()),
                             LITERAL_PATH("build_info.hkx")))
        {
            build_info_bhv = behavior.get();
            continue;
        }

        auto& state = manager.CreateCompileState(behavior->GetFilePath());
        thread_pool.priority_enqueue(
            behavior->GetSize(),
            [&behavior, &state, &CompiledObjectCounter, &prgs_callback, TotalObjects]()
            {
                behavior->ScheduleCompileFile(
                    state, NemesisInfo::OutputPlatform(), NemesisInfo::OutputVersion(), true);
                prgs_callback(++CompiledObjectCounter, TotalObjects);
            });
    }

    if (AnimDataSingleFile)
    {
        thread_pool.priority_enqueue(
            2000,
            [this,
             &adsf_state = manager.CreateCompileState(AnimDataSingleFile->GetFilePath()),
             &CompiledObjectCounter,
             &prgs_callback,
             TotalObjects]()
            {
                AnimDataSingleFile->ScheduleCompileFile(
                    adsf_state,
                    [&CompiledObjectCounter, &prgs_callback, TotalObjects]()
                    { prgs_callback(++CompiledObjectCounter, TotalObjects); });
            });
    }

    if (AnimSetDataSingleFile)
    {
        thread_pool.priority_enqueue(
            800,
            [this,
             &asdsf_state = manager.CreateCompileState(AnimSetDataSingleFile->GetFilePath()),
             &CompiledObjectCounter,
             &prgs_callback,
             TotalObjects]()
            {
                AnimSetDataSingleFile->ScheduleCompileFile(
                    asdsf_state,
                    [&CompiledObjectCounter, &prgs_callback, TotalObjects]()
                    { prgs_callback(++CompiledObjectCounter, TotalObjects); });
            });
    }

    thread_pool.join_all();

    std::future<void> build_info_future;

    if (build_info_bhv)
    {
        build_info_future = std::async(
            [&manager, &prgs_callback, &CompiledObjectCounter, TotalObjects, build_info_bhv]()
            {
                auto& state = manager.CreateCompileState(build_info_bhv->GetFilePath());
                build_info_bhv->CompileFile(
                    state, NemesisInfo::OutputPlatform(), NemesisInfo::OutputVersion(), true);
                prgs_callback(++CompiledObjectCounter, TotalObjects);
            });
    }

    std::future<void> pex_future = std::async(
        [&manager, &prgs_callback, &CompiledObjectCounter, TotalObjects]()
        {
            nemesis::FNISaaPexFile pex_file(NemesisInfo::ExeDirectory() / LITERAL_PATH("alternate_animations")
                                            / LITERAL_PATH("AlternateAnimationsScript.pex"));
            pex_file.Patch(manager);
            pex_file.OutputPexFile(NemesisInfo::DataPath() / LITERAL_PATH("Scripts")
                                   / LITERAL_PATH("Nemesis_aa2.pex"));
            prgs_callback(++CompiledObjectCounter, TotalObjects);
        });
    
    if (build_info_bhv)
    {
        build_info_future.get();
    }

    pex_future.get();

    for (auto& character : Characters)
    {
        character->WaitForCompleteCompilation();
    }

    for (auto& behavior : Behaviors)
    {
        behavior->WaitForCompleteCompilation();
    }

    if (AnimDataSingleFile)
    {
        AnimDataSingleFile->WaitForCompleteCompilation();
    }

    if (AnimSetDataSingleFile)
    {
        AnimSetDataSingleFile->WaitForCompleteCompilation();
    }

    Logger::Log("Total Files Compiled: " + std::to_string(CompiledObjectCounter), true);
}
