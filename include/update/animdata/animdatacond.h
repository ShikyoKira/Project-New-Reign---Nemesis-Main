#pragma once

#include "utilities/writetextfile.h"

#include "update/animdata/animdataprojectcond.h"

struct MasterAnimData
{
    using ModCode           = std::string;
    using ProjectName       = std::string;
    using ProjectNameLinked = nemesis::LinkedVar<ProjectName>;
    using ProjectPtr        = std::shared_ptr<AnimDataProject_Condt>;
    using ProjectData       = nemesis::LinkedVar<std::pair<ProjectNameLinked, ProjectPtr>>;

private:
    std::unordered_map<ProjectName, unsigned int> projectIndexMap;
    std::unordered_map<ProjectName, unsigned int> projectCounter;

    void getprojectlines(VecStr& output);

    void conditionLoop(const ProjectData& data, VecStr& output);

public:
    std::vector<ProjectData> projectlist;

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

#if HIDE
    struct AnimDataList : VecStr
    {
    private:
        size_t TotalLines = 0;

    public:
        size_t length()
        {
            return TotalLines;
        }

        void clear()
        {
            TotalLines = 0;
            VecStr::clear();
        }

        void push_back(std::string line)
        {
            if (line.find("<!--") == NOT_FOUND) TotalLines++;

            VecStr::emplace_back(line);
        }

        void pop_back()
        {
            if (VecStr::back().find("<!--") == NOT_FOUND) TotalLines--;

            VecStr::pop_back();
        }

        void insert(VecStr::iterator pos, const std::string& line)
        {
            if (line.find("<!--") == NOT_FOUND) TotalLines--;

            VecStr::insert(pos, line);
        }

        void insert(VecStr::iterator pos, VecStr::iterator whr, VecStr::iterator til)
        {
            for (auto whe = whr; whe < til; ++whe)
            {
                if (whe->find("<!--") == NOT_FOUND) TotalLines++;
            }

            VecStr::insert(pos, whr, til);
        }
    };

    std::unordered_map<std::string, std::unordered_map<std::string, AnimDataList>>
        newAnimData;     // character, unique code, vector<string>; memory to access each node
    VecStr animDataChar; // order of the character
    std::unordered_map<std::string, VecStr> animDataHeader; // order of the character's header
    std::unordered_map<std::string, VecStr> animDataInfo;   // order of the character's info data
#endif
};
