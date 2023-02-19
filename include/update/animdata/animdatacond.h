#pragma once

#include "update/animdata/animdataprojectcond.h"

struct MasterAnimData
{
    using ModCode           = std::string;
    using ProjectName       = std::string;
    using ProjectNameLinked = nemesis::LinkedVar<ProjectName>;
    using ProjectPtr        = std::shared_ptr<AnimDataProject_Condt>;
    using LinkedProjPair    = std::pair<ProjectNameLinked, ProjectPtr>;
    using ProjectData       = nemesis::LinkedVar<LinkedProjPair>;

    struct ModPatch
    {
        struct Project
        {
            std::filesystem::path directory;

            std::string name;
            UPtr<nemesis::File> header = nullptr;
            Vec<nemesis::File> uniquecodelist;

            Project(const std::filesystem::path& projdir);

            void AddHeader(const std::filesystem::path& headerpath);
            void AddUniqueCodeList(const VecStr& uniquecodelist);
        };

        std::string code;
        UPtr<nemesis::File> header = nullptr;
        Vec<Project> projectlist;

    private:
        void AddHeader(const std::string& code, const std::filesystem::path& headerdir);
        void AddProject(const std::string& code, const std::filesystem::path& projdir);

    public:
        ModPatch(const std::string& code, const std::filesystem::path& path);
    };

private:
    class ModPatcher
    {
        MasterAnimData& master_ad;
        Vec<nemesis::File>& corefiles;
        const ModPatch& patch;

        void AddHeader();
        void AddProject(const ModPatch::Project& project);
        void AddProjectHeader(const ModPatch::Project& project);
        void AddUniqueCodes(const ModPatch::Project& project);

    public:
        ModPatcher(MasterAnimData& master_ad, Vec<nemesis::File>& corefiles, const ModPatch& patch);

        void Patch();
    };

    std::string totalline;
    UMap<ProjectName, size_t> projectIndexMap;
    UMap<ProjectName, size_t> projectCounter;

    void getprojectlines(const ProjectData& data, VecStr& output, VecStr& output2);

public:
    Vec<ProjectData> projectlist;

    bool contains(const ProjectName& projName) const;
    size_t getIndex(const ProjectName& projName) const;

    MasterAnimData::ProjectPtr find(const ProjectName& projName, const ModCode& modcode = "original") const;

    MasterAnimData::ProjectPtr add(const ProjectName& projName,
                                   size_t num             = 0,
                                   const ModCode& modcode = "original",
                                   nemesis::CondType type = nemesis::CondType::NONE);
    MasterAnimData::ProjectPtr add(const ProjectName& projName,
                                   const VecNstr& storeline,
                                   const ModCode& modcode = "original",
                                   nemesis::CondType type = nemesis::CondType::NONE);

    void projectListUpdate(const ModCode& modcode, const std::filesystem::path& filepath,
                           bool isTemplate = false);
    void UpdateBaseProjectList(const ModCode& modcode, const std::filesystem::path& filepath);
    void UpdateTemplateProjectList(const std::filesystem::path& filepath);

    const std::string& SaveTemplateAs(const std::filesystem::path& filepath);
    void ReadTemplateLine(std::string& totalline);
    void ReadTemplateLines(VecStr& lines);

    void AddModPatch(Vec<nemesis::File>& corefiles, const MasterAnimData::ModPatch& modpatch);
    //bool AddHeaderPatch(Vec<nemesis::File>& corefiles,
    //                    const MasterAnimData::ModPatch::Project& project,
    //                    const MasterAnimData::ModPatch& modpatch);
    //void AddProjectHeaderPatch(Vec<nemesis::File>& corefiles,
    //                           const MasterAnimData::ModPatch::Project& project,
    //                           const MasterAnimData::ModPatch& modpatch);

    MasterAnimData::ProjectPtr GetConditionedProject(ProjectData& project, const std::string& code);

    bool AddConditionedPatch(ProjectPtr projptr, const std::filesystem::path& uniquecode_path);


    static constexpr std::string_view header_str  = "$header$";
    static constexpr std::string_view header_file = "$header$.txt";

private:
    void getlines(VecStr& lines);
};
