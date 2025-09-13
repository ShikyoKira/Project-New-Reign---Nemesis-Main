#include "Core/ModClass.h"

#include "Utilities/Algorithm.h"
#include "Utilities/ThreadPool.h"

#include "Logger.h"

void nemesis::ModClass::ParseModFromMeshesDirectory(const std::filesystem::path& meshes_dir_path,
                                                    nemesis::ThreadPool& threadpool)
{
    if (!std::filesystem::exists(meshes_dir_path)) return;

    for (auto& entry : std::filesystem::directory_iterator(meshes_dir_path))
    {
        auto path = entry.path();

        if (!entry.is_directory())
        {
            if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".nemx"))) continue;

            auto node = nemesis::HkxNode::DeserializeHkxNodeFromFile(path, threadpool);
            NodeList.emplace_back(
                std::make_unique<nemesis::ModPatch<nemesis::HkxNode>>(*this, std::move(node)));
            continue;
        }

        if (nemesis::iequals(PATH_TO_STRING(path.stem()), LITERAL_PATH("animationdatasinglefile")))
        {
            ParseModAnimDataFromDirectory(path, threadpool);
            continue;
        }

        if (nemesis::iequals(PATH_TO_STRING(path.stem()), LITERAL_PATH("animationsetdatasinglefile")))
        {
            ParseModAnimSetDataFromDirectory(path, threadpool);
            continue;
        }

        ParseModHkxFromDirectory(path, threadpool);
    }
}

void nemesis::ModClass::ParseModHkxFromDirectory(const std::filesystem::path& dir_path,
                                                 nemesis::ThreadPool& threadpool)
{
    if (!std::filesystem::exists(dir_path)) return;

    for (auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.is_directory())
        {
            ParseModHkxFromDirectory(entry.path(), threadpool);
            continue;
        }

        auto node = nemesis::HkxNode::DeserializeHkxNodeFromFile(entry.path(), threadpool);
        NodeList.emplace_back(std::make_unique<nemesis::ModPatch<nemesis::HkxNode>>(*this, std::move(node)));
    }
}

void nemesis::ModClass::ParseModAnimDataFromDirectory(const std::filesystem::path& dir_path,
                                                      nemesis::ThreadPool& threadpool)
{
    for (auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.is_directory())
        {
            ParseModAnimDataFromDirectory(entry.path(), threadpool);
            continue;
        }

        std::filesystem::path path = entry.path();
        auto filename              = PATH_TO_STRING(path.stem());

        if (nemesis::iequals(filename, LITERAL_PATH("$header$")))
        {
            auto header = nemesis::AnimationDataProject::DeserializeHeaderFromFile(path, threadpool);
            AnimDataHeaderList.emplace_back(
                std::make_unique<nemesis::ModPatch<nemesis::AnimationDataProject::Headers>>(
                    *this, std::move(header)));
            continue;
        }

        std::smatch match;
        std::string search = nemesis::to_utf8_string(path.stem());

        if (std::regex_match(search, match, ModRgx))
        {
            if (match.str(1).empty())
            {
                auto motion_data = nemesis::AnimationDataMotionData::DeserializeFromFile(path, threadpool);
                MotionDataList.emplace_back(
                    std::make_unique<nemesis::ModPatch<nemesis::AnimationDataMotionData>>(
                        *this, std::move(motion_data)));
                continue;
            }

            auto clip_data = nemesis::AnimationDataClipData::DeserializeFromFile(path, threadpool);
            ClipDataList.emplace_back(std::make_unique<nemesis::ModPatch<nemesis::AnimationDataClipData>>(
                *this, std::move(clip_data)));
            continue;
        }

        if (is_only_number(filename))
        {
            auto motion_data = nemesis::AnimationDataMotionData::DeserializeFromFile(path, threadpool);
            MotionDataList.emplace_back(std::make_unique<nemesis::ModPatch<nemesis::AnimationDataMotionData>>(
                *this, std::move(motion_data)));
            continue;
        }

        auto clip_data = nemesis::AnimationDataClipData::DeserializeFromFile(path, threadpool);
        ClipDataList.emplace_back(
            std::make_unique<nemesis::ModPatch<nemesis::AnimationDataClipData>>(*this, std::move(clip_data)));
    }
}

void nemesis::ModClass::ParseModAnimSetDataFromDirectory(const std::filesystem::path& dir_path,
                                                         nemesis::ThreadPool& threadpool)
{
    if (!std::filesystem::exists(dir_path)) return;

    for (auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.is_directory())
        {
            ParseModAnimSetDataFromDirectory(entry.path(), threadpool);
            continue;
        }

        auto state = nemesis::AnimationSetDataState::DeserializeFromFile(entry.path(), threadpool);
        StateList.emplace_back(
            std::make_unique<nemesis::ModPatch<nemesis::AnimationSetDataState>>(*this, std::move(state)));
    }
}

nemesis::ModClass::ModClass(const std::filesystem::path& ini_file, nemesis::ThreadPool& threadpool)
{
    auto parent_path = ini_file.parent_path();
    Info.ReadFile(ini_file);
    Info.SetModCode(nemesis::to_lower_copy(nemesis::to_utf8_string(parent_path.stem())));
    ModRgx = "^(.+?~|)" + Info.GetModCode() + "\\$[0-9]+$";

    Logger::Log("Mod Class: " + Info.GetModCode());

    if (!std::filesystem::exists(parent_path)) return;

    InitializerFuture = std::async(
        [this, &threadpool, parent_path]()
        {
            for (auto& entry : std::filesystem::directory_iterator(parent_path))
            {
                if (!entry.is_directory()) continue;

                ParseModFromMeshesDirectory(entry.path(), threadpool);
            }
        });
}

void nemesis::ModClass::FinalizeInitialization()
{
    InitializerFuture.get();
}

const std::string& nemesis::ModClass::GetCode() const noexcept
{
    return Info.GetModCode();
}

const nemesis::ModInfo& nemesis::ModClass::GetModInfo() const
{
    return Info;
}

Vec<const nemesis::ModPatch<nemesis::HkxNode>*> nemesis::ModClass::GetNodeList() const
{
    if (NodeListCache) return *NodeListCache;

    NodeListCache = std::make_unique<Vec<const nemesis::ModPatch<nemesis::HkxNode>*>>();

    for (auto& node : NodeList)
    {
        NodeListCache->emplace_back(node.get());
    }

    return *NodeListCache;
}

Vec<const nemesis::ModPatch<nemesis::AnimationSetDataState>*> nemesis::ModClass::GetStateList() const
{
    if (StateListCache) return *StateListCache;

    StateListCache = std::make_unique<Vec<const nemesis::ModPatch<nemesis::AnimationSetDataState>*>>();

    for (auto& state : StateList)
    {
        StateListCache->emplace_back(state.get());
    }

    return *StateListCache;
}

Vec<const nemesis::ModPatch<nemesis::AnimationDataProject::Headers>*>
nemesis::ModClass::GetAnimDataHeaderList() const
{
    if (AnimDataHeaderListCache) return *AnimDataHeaderListCache;

    AnimDataHeaderListCache
        = std::make_unique<Vec<const nemesis::ModPatch<nemesis::AnimationDataProject::Headers>*>>();

    for (auto& header : AnimDataHeaderList)
    {
        AnimDataHeaderListCache->emplace_back(header.get());
    }

    return *AnimDataHeaderListCache;
}

Vec<const nemesis::ModPatch<nemesis::AnimationDataClipData>*> nemesis::ModClass::GetClipDataList() const
{
    if (ClipDataListCache) return *ClipDataListCache;

    ClipDataListCache = std::make_unique<Vec<const nemesis::ModPatch<nemesis::AnimationDataClipData>*>>();

    for (auto& clip_data : ClipDataList)
    {
        ClipDataListCache->emplace_back(clip_data.get());
    }

    return *ClipDataListCache;
}

Vec<const nemesis::ModPatch<nemesis::AnimationDataMotionData>*> nemesis::ModClass::GetMotionDataList() const
{
    if (MotionDataListCache) return *MotionDataListCache;

    MotionDataListCache = std::make_unique<Vec<const nemesis::ModPatch<nemesis::AnimationDataMotionData>*>>();

    for (auto& motion_data : MotionDataList)
    {
        MotionDataListCache->emplace_back(motion_data.get());
    }

    return *MotionDataListCache;
}
