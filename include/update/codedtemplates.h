#pragma once

#include "update/templatepatch.h"

#include "update/animdata/animdatatemplatepatch.h"

#include "update/animsetdata/animsetdatatemplatepatch.h"

namespace nemesis
{
	struct CodedTemplates
    {
    private:
        std::string code;

        Vec<nemesis::TemplatePatch> patches;
        Vec<nemesis::AnimDataTemplatePatch> animdata_patches;
        Vec<nemesis::AnimSetDataTemplatePatch> animsetdata_patches;

        void AddCharcAnimDataTemplate(const std::string& character, const std::filesystem::path& path);
        void ValidateAnimDataTemplateFile(const std::filesystem::path& path);
        void ValidateAnimDataTemplateCharc(const std::string& character, const std::filesystem::path& path);

        void ParseFolder(const std::filesystem::path& path);
        void ParseAnimDataFolder(const std::filesystem::path& path);
        void ParseAnimSetDataFolder(const std::filesystem::path& path);
        void ParseBehaviorFolder(const std::filesystem::path& datapath, const std::filesystem::path& path);

    public:
        CodedTemplates(const std::filesystem::path& path);

        std::string_view GetCode() const;
        const Vec<nemesis::TemplatePatch>& GetPatches() const;
        const Vec<nemesis::AnimDataTemplatePatch>& GetADPatches() const;
        const Vec<nemesis::AnimSetDataTemplatePatch>& GetASDPatches() const;
    };
}
