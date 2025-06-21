#pragma once

#include "Core/LineStream.h"
#include "Core/SubTemplateObject.h"

#include "Core/Template/TemplateObject.h"

#include "Core/Hkx/HkxPlatform.h"

#include "Enums/HavokVersion.h"
#include "Enums/PlatformType.h"

namespace nemesis
{
    struct HkxNode;
    struct SemanticManager;

    struct HkxFile : public nemesis::NObject
    {
    protected:
        Map<std::string, nemesis::HkxNode*> NodeMap;

        nemesis::HkxNode* RootNode;

        Map<std::string, UPtr<nemesis::NObject>> NewNodes;
        UPtr<nemesis::CollectionObject> RegularNodes;

        UMap<std::string, SPtr<nemesis::TemplateObject>> TemplateMap;

        USetStr ModInUsedList;
        mutable std::mutex ModInUsedListMutex;

        std::filesystem::path FilePath;
        std::filesystem::path TargetPath;
        std::filesystem::path CachedFilePath;
        std::filesystem::path RelativePath;

        mutable std::mutex NodeMutex;
        mutable std::mutex TemplateMutex;

        mutable Vec<std::future<void>> CompileFuture;
        mutable std::mutex CompileFutureMutex;

        DeqNstr CompileAllTemplates(nemesis::CompileState& state) const;
        DeqNstr CompileAllSubTemplates(nemesis::CompileState& state) const;

        std::future<void> CompileToHkx(const std::filesystem::path& hkx_path,
                                       const std::string& contents,
                                       nemesis::CompileState& state,
                                       nemesis::PlatformType platform,
                                       nemesis::HavokVersion version,
                                       bool include_xml,
                                       const UMap<size_t, Pair<size_t, std::filesystem::path>>& modded_lines,
                                       std::function<void()> callback) const;

    public:
        std::filesystem::path CompileFile(nemesis::CompileState& state,
                                          nemesis::PlatformType platform,
                                          nemesis::HavokVersion version) const;
        void CompileFileAsXml(const std::filesystem::path& filepath, nemesis::CompileState& state) const;
        void CompileFileAsHkx(const std::filesystem::path& filepath,
                              nemesis::CompileState& state,
                              nemesis::PlatformType platform,
                              nemesis::HavokVersion version,
                              bool include_xml) const;
        std::filesystem::path ScheduleCompileFile(nemesis::CompileState& state,
                                                  nemesis::PlatformType platform,
                                                  nemesis::HavokVersion version,
                                                  bool include_xml) const;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                   nemesis::CompileState& state,
                                   nemesis::PlatformType platform,
                                   nemesis::HavokVersion version,
                                   bool include_xml) const;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                   nemesis::CompileState& state,
                                   nemesis::PlatformType platform,
                                   nemesis::HavokVersion version,
                                   bool include_xml,
                                   std::function<void()> callback) const;
        void WaitForCompleteCompilation() const;

        void AddTemplate(const SPtr<nemesis::TemplateObject>& templt_obj);

        nemesis::HkxNode* AddModNode(const std::string& modcode, UPtr<nemesis::HkxNode>&& node);

        nemesis::HkxNode* GetNodeById(const std::string& node_id);
        const nemesis::HkxNode* GetNodeById(const std::string& node_id) const;

        const std::filesystem::path& GetFilePath() const noexcept;
        const std::filesystem::path& GetTargetPath() const noexcept;
        const std::filesystem::path& GetCachedFilePath() const noexcept;
        const std::filesystem::path& GetRelativePath() const noexcept;

        bool IsSameAsCached(nemesis::CompileState& state) const;

    protected:
        static bool TryGetValueInHkcString(const std::string& line, std::string& value);
    };
}
