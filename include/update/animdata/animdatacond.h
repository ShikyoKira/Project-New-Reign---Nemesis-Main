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

private:
    std::string totalline;
    std::unordered_map<ProjectName, unsigned int> projectIndexMap;
    std::unordered_map<ProjectName, unsigned int> projectCounter;

    void getprojectlines(const ProjectData& data, VecStr& output, VecStr& output2);

public:
    std::vector<ProjectData> projectlist;

    bool contains(const ProjectName& projName) const;
    unsigned int getIndex(const ProjectName& projName) const;

    ProjectPtr find(const ProjectName& projName, const ModCode& modcode = "original") const;

    ProjectPtr add(const ProjectName& projName,
                   size_t num             = 0,
                   const ModCode& modcode = "original",
                   nemesis::CondType type = nemesis::CondType::NONE);
    ProjectPtr add(const ProjectName& projName,
                   const VecStr& storeline,
                   const ModCode& modcode = "original",
                   nemesis::CondType type = nemesis::CondType::NONE);

    void projectListUpdate(const ModCode& modcode,
                           const std::string& filepath,
                           bool isTemplate = false);

    const std::string& SaveTemplateAs(const std::filesystem::path& filepath);
    void ReadTemplateLine(std::string& totalline);
    void ReadTemplateLines(VecStr& lines);

private:
    void getlines(VecStr& lines);
};
