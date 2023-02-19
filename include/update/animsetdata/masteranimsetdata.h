#pragma once

#include "update/animsetdata/projectanimsetdata.h"

struct MasterAnimSetData
{
    using ProjectName           = std::string;
    using ModCode               = std::string;
    using ProjectAnimSetDataPtr = SPtr<ProjectAnimSetData>;
    using LinkedProject         = nemesis::LinkedVar<ProjectAnimSetData>;

    VecStr projectList; // order of the project
    std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>>
        newAnimSetData; // project, header, vector<string>; memory to access each node

    struct ModPatch
    {
        struct Project
        {
            struct AnimSetData
            {
                std::string name;
                nemesis::File file;
                bool isnew = false;

                AnimSetData(const std::string& code, const std::filesystem::path& path);
            };

            std::string name;
            UPtr<nemesis::File> header = nullptr;
            Vec<AnimSetData> asdlist;

        private:
            void AddAnimSetData(const std::string& code, const std::filesystem::path& path);
            void AddHeader(const std::filesystem::path& path);

        public:
            Project(const std::string& code, const std::filesystem::path& projdir);
        };

        std::string code;

        UPtr<nemesis::File> header = nullptr;
        Vec<Project> projectlist;

    private:
        void AddHeader(const std::filesystem::path& path);
        void AddProject(const std::filesystem::path& path);

    public:
        ModPatch(const std::string& code, const std::filesystem::path& path);

    private:
        static constexpr std::string_view header_str  = "$header$";
        static constexpr std::string_view header_file = "$header$.txt";
    };

private:
    class Parser
    {
    private:
        size_t index;

        std::string format;
        std::filesystem::path path;

        MasterAnimSetData& host;
        const VecNstr& storeline;
        SPtr<nemesis::ConditionInfo> tobedeleted;


    public:
        Parser(MasterAnimSetData& base, const VecNstr& storeline) noexcept;

        void SetFormat(const std::string& format) noexcept;
        void SetPath(const std::filesystem::path& path) noexcept;

        void ImportProject();

    private:
        void PointingProject() noexcept;
        void PointingOriProject();
        void CloseProject();
    };

    class ModPatcher
    {
    private:
        MasterAnimSetData& master_asd;
        const ModPatch& patch;
        Vec<nemesis::File>& corefiles;

        void AddHeaderModPatch();
        void AddProjectModPatch(const MasterAnimSetData::ModPatch::Project& project);
        void AddProjectHeaderModPatch(ProjectAnimSetData* projectptr,
                                      const MasterAnimSetData::ModPatch::Project& project);
        void AddAnimSetData(ProjectAnimSetData* projectptr,
                            const MasterAnimSetData::ModPatch::Project::AnimSetData& asd);

    public:
        ModPatcher(MasterAnimSetData& master_asd, Vec<nemesis::File>& corefiles, const ModPatch& patch);
    };

    std::string totalline;
    UMap<ProjectName, size_t> projectIndexMap;

public:
    Vec<nemesis::LinkedVar<ProjectAnimSetData>> projects;

    bool contains(const ProjectName& projName) const;
    ProjectAnimSetDataPtr find(const ProjectName& projName);
    ProjectAnimSetDataPtr find(const ProjectName& projName, const ModCode& condition);
    ProjectAnimSetDataPtr find(const ProjectName& projName, bool& isNew, const ModCode& condition);

    ProjectAnimSetDataPtr add(const nemesis::Line& line);
    ProjectAnimSetDataPtr add(const ProjectName& name, size_t num);
    ProjectAnimSetDataPtr add(const ProjectName& name, size_t num, const std::string& condition, nemesis::CondType type);
    ProjectAnimSetDataPtr add(const ProjectName& name, size_t num, const nemesis::CondDetails& condition);
    ProjectAnimSetDataPtr add(const ProjectName& name, const VecNstr& storeline);
    ProjectAnimSetDataPtr add(const ProjectName& name, const VecNstr& storeline, const nemesis::CondDetails& condition);
    ProjectAnimSetDataPtr add(const ProjectName& name, size_t num, const nemesis::ConditionInfo& conditioninfo);

    void projectListUpdate(const std::string& modcode,
                           const std::filesystem::path& filepath,
                           nemesis::CondCheckFunc condfunc);

    const std::string& SaveTemplateAs(const std::filesystem::path& filepath);
    void ReadTemplateLine(std::string& totalline);
    void ReadTemplateLines(VecStr& lines);

    void AddModPatch(Vec<nemesis::File>& corefiles, const MasterAnimSetData::ModPatch& patch);

private:
    void getlines(VecStr& storeline);
    size_t getIndex(const ProjectName& projName) const;
};
