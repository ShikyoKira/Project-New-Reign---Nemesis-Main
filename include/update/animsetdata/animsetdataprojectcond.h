#pragma once

#include <map>
#include <filesystem>

#include "utilities/alphanum.hpp"

#include "update/animsetdata/animsetdatacond.h"

struct ProjectAnimSetData
{
    using ModCode = std::string;
    using ProjectName = std::string;

private:
    std::unordered_map<std::string, size_t> setIndexMap;
    std::vector<nemesis::LinkedVar<AnimSetData>> animSetData;

public:
    nemesis::LinkedVar<ProjectName> name;

    ProjectAnimSetData() {}
    ProjectAnimSetData(const ProjectName& _name, const VecNstr& storeline);

    nemesis::LinkedVar<AnimSetData>& addAnimSetData(const nemesis::Line& line);
    nemesis::LinkedVar<AnimSetData>& addAnimSetData(const nemesis::Line& line,
                                                    const nemesis::CondDetails& condition);
    void importAnimSetData(std::filesystem::path filepath, const VecNstr& _namelist, size_t& index);
    void importAnimSetData(std::filesystem::path filepath,
                           const VecNstr& _namelist,
                           size_t& index,
                           nemesis::CondCheckFunc condfunc,
                           const ModCode& format);
    const std::vector<nemesis::LinkedVar<AnimSetData>>& getAnimSetData() const;
    std::shared_ptr<AnimSetData> find(const ProjectName& _name) const;
    std::shared_ptr<AnimSetData> find(const ProjectName& _name, const ModCode& modcode) const;

    void getlines(VecStr& storeline) const;
    void getname(VecStr& storeline) const;

private:
    uint getIndex(const ProjectName& _name) const;
    bool contains(const ProjectName& _name) const;
};

void getLinkedLines(const nemesis::LinkedVar<ProjectAnimSetData>& linkedpack,
                    VecStr& storeline,
                    VecStr& namelist);
