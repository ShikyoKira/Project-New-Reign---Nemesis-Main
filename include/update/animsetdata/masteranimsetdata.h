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
        void SetPath(std::filesystem::path path) noexcept;

        void ImportProject();

    private:
        void PointingProject() noexcept;
        void PointingOriProject();
        void CloseProject();
    };

    std::string totalline;
    UMap<ProjectName, uint> projectIndexMap;

public:
    Vec<nemesis::LinkedVar<ProjectAnimSetData>> projects;

    bool contains(const ProjectName& projName) const;
    ProjectAnimSetDataPtr find(const ProjectName& projName);
    ProjectAnimSetDataPtr find(const ProjectName& projName, const ModCode& condition);
    ProjectAnimSetDataPtr find(const ProjectName& projName, bool& isNew, const ModCode& condition);

    ProjectAnimSetDataPtr& add(const nemesis::Line& line);
    ProjectAnimSetDataPtr& add(const ProjectName& name, size_t num);
    ProjectAnimSetDataPtr&
    add(const ProjectName& name, size_t num, const std::string& condition, nemesis::CondType type);
    ProjectAnimSetDataPtr& add(const ProjectName& name, size_t num, const nemesis::CondDetails& condition);
    ProjectAnimSetDataPtr& add(const ProjectName& name, const VecNstr& storeline);
    ProjectAnimSetDataPtr&
    add(const ProjectName& name, const VecNstr& storeline, const nemesis::CondDetails& condition);

    ProjectAnimSetDataPtr&
    add(const ProjectName& name, size_t num, const nemesis::ConditionInfo& conditioninfo);

    void projectListUpdate(const ModCode& modcode,
                           const std::string& filepath,
                           nemesis::CondCheckFunc condfunc);

    const std::string& SaveTemplateAs(const std::filesystem::path& filepath);
    void ReadTemplateLine(std::string& totalline);
    void ReadTemplateLines(VecStr& lines);

private:
    void getlines(VecStr& storeline);
    uint getIndex(const ProjectName& projName) const;
};
