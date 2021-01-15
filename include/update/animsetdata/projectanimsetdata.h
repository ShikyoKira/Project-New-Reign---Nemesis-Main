#pragma once

#include <map>
#include <filesystem>

#include "utilities/alphanum.hpp"

#include "update/animsetdata/animsetdata.h"

struct ProjectAnimSetData
{
    using ModCode = std::string;
    using ProjectName = std::string;

private:
    class Parser
    {
    private:
        size_t index;

        std::string format;
        std::filesystem::path path;

        ProjectAnimSetData& host;
        const VecNstr& namelist;
        SPtr<nemesis::ConditionInfo> tobedeleted;

    public:
        Parser(ProjectAnimSetData& base, const VecNstr& namelist) noexcept;

        void SetFormat(const std::string& format) noexcept;
        void SetPath(std::filesystem::path path) noexcept;
        void SetStartIndex(const uint& startindex) noexcept;

        void AddAnimSetDataList();
        void ImportAnimSetDataList();

        size_t GetIndex() const noexcept;

    private:
        void PointingOriAnimSetData();
        void CloseAnimSetData();
    };

    UMap<std::string, size_t> setIndexMap;
    Vec<nemesis::LinkedVar<AnimSetData>> animSetData;

public:
    nemesis::LinkedVar<ProjectName> name;

    ProjectAnimSetData() {}
    ProjectAnimSetData(const ProjectName& _name, const VecNstr& storeline);

    nemesis::LinkedVar<AnimSetData>& AddAnimSetData(const nemesis::Line& line);
    nemesis::LinkedVar<AnimSetData>& AddAnimSetData(const nemesis::Line& line,
                                                    const nemesis::CondDetails& condition);
    nemesis::LinkedVar<AnimSetData>& AddAnimSetData(const nemesis::Line& line,
                                                    const nemesis::ConditionInfo& conditioninfo);

    void ImportData(std::filesystem::path path, size_t& num, const VecNstr& storeline);
    void ImportData(std::filesystem::path path, size_t& num, const VecNstr& storeline, const ModCode format);

    const std::vector<nemesis::LinkedVar<AnimSetData>>& getAnimSetData() const;
    SPtr<AnimSetData> find(const ProjectName& _name) const;
    SPtr<AnimSetData> find(const ProjectName& _name, const ModCode& modcode) const;

    void getlines(VecStr& storeline) const;
    void getname(VecStr& storeline) const;

private:
    uint getIndex(const ProjectName& _name) const;
    bool contains(const ProjectName& _name) const;
};

void getLinkedLines(const nemesis::LinkedVar<ProjectAnimSetData>& linkedpack,
                    VecStr& storeline,
                    VecStr& namelist);
