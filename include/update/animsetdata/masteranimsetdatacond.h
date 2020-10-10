#pragma once

#include "update/animsetdata/animsetdataprojectcond.h"

struct MasterAnimSetData
{
    using ProjectName           = std::string;
    using ModCode               = std::string;
    using ProjectAnimSetDataPtr = std::shared_ptr<ProjectAnimSetData>;

    VecStr projectList; // order of the project
    std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>>
        newAnimSetData; // project, header, vector<string>; memory to access each node

private:
    std::unordered_map<ProjectName, uint> projectIndexMap;

public:
    std::vector<nemesis::LinkedVar<ProjectAnimSetData>> projects;

    bool contains(const ProjectName& projName) const;
    ProjectAnimSetDataPtr find(const ProjectName& projName);
    ProjectAnimSetDataPtr find(const ProjectName& projName, const ModCode& condition);
    ProjectAnimSetDataPtr find(const ProjectName& projName, bool& isNew, const ModCode& condition);

    ProjectAnimSetDataPtr& add(const nemesis::Line& line);
    ProjectAnimSetDataPtr& add(const ProjectName& name, size_t num);
    ProjectAnimSetDataPtr& add(const ProjectName& name, size_t num, const nemesis::CondDetails& condition);
    ProjectAnimSetDataPtr& add(const ProjectName& name, const VecNstr& storeline);
    ProjectAnimSetDataPtr&
    add(const ProjectName& name, const VecNstr& storeline, const nemesis::CondDetails& condition);
    void projectListUpdate(const ModCode& modcode,
                           const std::string& filepath,
                           const VecNstr& storeline,
                           nemesis::CondCheckFunc condfunc);

    void getlines(VecStr& storeline);

private:
    uint getIndex(const ProjectName& projName) const;
};
