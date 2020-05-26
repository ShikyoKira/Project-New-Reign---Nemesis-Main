#pragma once

#include "utilities/writetextfile.h"

#include "update/animdata/animdataprojectcond.h"

struct MasterAnimData
{
    using ModCode           = std::string;
    using ProjectName       = std::string;
    using ProjectNameLinked = nemesis::LinkedVar<ProjectName>;
    using ProjectPtr        = std::shared_ptr<AnimDataProject_Condt>;
    using LinkedProjPair    = std::pair<ProjectNameLinked, ProjectPtr>;
    using ProjectData       = nemesis::LinkedVar<LinkedProjPair>;

private:
    std::unordered_map<ProjectName, unsigned int> projectIndexMap;
    std::unordered_map<ProjectName, unsigned int> projectCounter;

    void getprojectlines(const ProjectData& data, VecStr& output, VecStr& output2);

public:
    std::vector<ProjectData> projectlist;

    bool contains(const ProjectName& projName);
    unsigned int getIndex(const ProjectName& projName);

    ProjectPtr find(const ProjectName& projName, const ModCode& modcode = "original");

    ProjectPtr add(const ProjectName& projName,
                   size_t num             = 0,
                   const ModCode& modcode = "original",
                   nemesis::CondType type = nemesis::NONE);
    ProjectPtr add(const ProjectName& projName,
                   const VecStr& storeline,
                   const ModCode& modcode = "original",
                   nemesis::CondType type = nemesis::NONE);

    void projectListUpdate(const ModCode& modcode,
                           const std::string& filepath,
                           const VecStr& storeline,
                           bool isTemplate = false);

    VecStr getlines();
    std::string writelines(FileWriter& output);
};
