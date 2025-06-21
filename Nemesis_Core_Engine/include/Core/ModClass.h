#pragma once

#include <regex>

#include "Core/ModInfo.h"
#include "Core/ModPatch.h"

#include "Core/Hkx/HkxNode.h"

#include "Core/AnimationData/AnimationDataProject.h"

#include "Core/AnimationSetData/AnimationSetDataState.h"

namespace nemesis
{
    struct ThreadPool;

    struct ModClass
    {
    private:
        nemesis::ModInfo Info;
        std::regex ModRgx;

        Vec<UPtr<nemesis::ModPatch<nemesis::HkxNode>>> NodeList;
        Vec<UPtr<nemesis::ModPatch<nemesis::AnimationSetDataState>>> StateList;
        Vec<UPtr<nemesis::ModPatch<nemesis::AnimationDataProject::Headers>>> AnimDataHeaderList;
        Vec<UPtr<nemesis::ModPatch<nemesis::AnimationDataClipData>>> ClipDataList;
        Vec<UPtr<nemesis::ModPatch<nemesis::AnimationDataMotionData>>> MotionDataList;

        mutable UPtr<Vec<const nemesis::ModPatch<nemesis::HkxNode>*>> NodeListCache;
        mutable UPtr<Vec<const nemesis::ModPatch<nemesis::AnimationSetDataState>*>> StateListCache;
        mutable UPtr<Vec<const nemesis::ModPatch<nemesis::AnimationDataProject::Headers>*>>
            AnimDataHeaderListCache;
        mutable UPtr<Vec<const nemesis::ModPatch<nemesis::AnimationDataClipData>*>> ClipDataListCache;
        mutable UPtr<Vec<const nemesis::ModPatch<nemesis::AnimationDataMotionData>*>> MotionDataListCache;

        void ParseModFromMeshesDirectory(const std::filesystem::path& meshes_dir_path,
                                         nemesis::ThreadPool& threadpool);
        void ParseModHkxFromDirectory(const std::filesystem::path& dir_path, nemesis::ThreadPool& threadpool);
        void ParseModAnimDataFromDirectory(const std::filesystem::path& dir_path,
                                           nemesis::ThreadPool& threadpool);
        void ParseModAnimSetDataFromDirectory(const std::filesystem::path& dir_path,
                                              nemesis::ThreadPool& threadpool);

    public:
        ModClass(const std::filesystem::path& ini_file, nemesis::ThreadPool& threadpool);

        std::string GetCode() const noexcept;
        const nemesis::ModInfo& GetModInfo() const;
        Vec<const nemesis::ModPatch<nemesis::HkxNode>*> GetNodeList() const;
        Vec<const nemesis::ModPatch<nemesis::AnimationSetDataState>*> GetStateList() const;
        Vec<const nemesis::ModPatch<nemesis::AnimationDataProject::Headers>*> GetAnimDataHeaderList() const;
        Vec<const nemesis::ModPatch<nemesis::AnimationDataClipData>*> GetClipDataList() const;
        Vec<const nemesis::ModPatch<nemesis::AnimationDataMotionData>*> GetMotionDataList() const;
    };
}
