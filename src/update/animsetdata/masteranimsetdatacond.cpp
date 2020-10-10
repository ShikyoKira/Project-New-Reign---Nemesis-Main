#include "Global.h"

#include "update/animsetdata/masteranimsetdatacond.h"

using namespace std;

bool MasterAnimSetData::contains(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    return itr != projectIndexMap.end();
}

shared_ptr<ProjectAnimSetData> MasterAnimSetData::find(const ProjectName& projName)
{
    return projects[getIndex(projName)].raw;
}

shared_ptr<ProjectAnimSetData> MasterAnimSetData::find(const ProjectName& projName, const ModCode& condition)
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    if (itr == projectIndexMap.end()) return nullptr;

    if (!projects[itr->second].raw)
    {
        for (auto& cond : projects[itr->second].nestedcond)
        {
            if (cond.conditions != condition) continue;

            auto& proj = cond.rawlist[0].raw;

            if (!proj) continue;

            return proj;
        }

        return nullptr;
    }

    return projects[itr->second].raw;
}

shared_ptr<ProjectAnimSetData>
MasterAnimSetData::find(const ProjectName& projName, bool& isNew, const ModCode& modcode)
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    if (itr == projectIndexMap.end()) return nullptr;

    if (!projects[itr->second].raw)
    {
        for (auto& cond : projects[itr->second].nestedcond)
        {
            if (cond.conditions != modcode) continue;

            auto& proj = cond.rawlist[0].raw;

            if (!proj) continue;

            isNew = true;
            return proj;
        }

        return nullptr;
    }

    return projects[itr->second].raw;
}

shared_ptr<ProjectAnimSetData>& MasterAnimSetData::add(const nemesis::Line& line)
{
    projectIndexMap[nemesis::to_lower_copy(line)] = projects.size();
    projects.push_back(nemesis::LinkedVar<ProjectAnimSetData>(ProjectAnimSetData()));
    projects.back().raw->name           = line;
    projects.back().raw->name.linecount = line.linenum;
    return projects.back().raw;
}

shared_ptr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name, size_t num)
{
    projectIndexMap[nemesis::to_lower_copy(name)] = projects.size();
    projects.push_back(nemesis::LinkedVar<ProjectAnimSetData>(ProjectAnimSetData()));
    projects.back().raw->name           = name;
    projects.back().raw->name.linecount = num;
    return projects.back().raw;
}

shared_ptr<ProjectAnimSetData>&
MasterAnimSetData::add(const ProjectName& name, size_t num, const nemesis::CondDetails& condition)
{
    projectIndexMap[nemesis::to_lower_copy(name)] = projects.size();
    projects.push_back(nemesis::CondVar(nemesis::LinkedVar<ProjectAnimSetData>(ProjectAnimSetData())));
    projects.back().linecount        = num;
    auto& curcond                    = projects.back().backCond();
    curcond.conditions               = condition.condition;
    curcond.conditionType            = condition.type;
    curcond.rawlist.back().linecount = num;
    curcond.rawlist.back().raw->name = name;
    return curcond.rawlist.back().raw;
}

shared_ptr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name, const VecNstr& storeline)
{
    auto proj = find(name);

    if (proj) return proj;

    proj = make_shared<ProjectAnimSetData>(name, storeline);
    add(name, 0);
    projects.back().raw = proj;
    return proj;
}

shared_ptr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name,
                                                       const VecNstr& storeline,
                                                       const nemesis::CondDetails& condition)
{
    auto proj = find(name, condition.condition);

    if (proj) return proj;

    proj = make_shared<ProjectAnimSetData>(name, storeline);
    projectIndexMap[nemesis::to_lower_copy(name)] = projects.size();
    auto ref = nemesis::LinkedVar<ProjectAnimSetData>();
    projects.push_back(nemesis::CondVar(ref));
    projects.back().linecount        = 0;
    auto& curcond                    = projects.back().backCond();
    curcond.conditions               = condition.condition;
    curcond.conditionType            = condition.type;
    curcond.rawlist.back().linecount = 0;
    curcond.rawlist.back().raw       = proj;
    return curcond.rawlist.back().raw;
    
}

void MasterAnimSetData::projectListUpdate(const ModCode& format,
                                          const string& filepath,
                                          const VecNstr& storeline,
                                          nemesis::CondCheckFunc condfunc)
{
    bool original = false;
    uint index    = 0;
    deque<nemesis::Line>* edits;
    vector<nemesis::CondDetails> condtype;

    for (unsigned int i = 1; i < storeline.size(); ++i)
    {
        const auto& line = storeline[i];

        if (condfunc(filepath, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.size() == 0 && line.find("<!--") == NOT_FOUND)
        {
            ++index;
            continue;
        }

        if (nemesis::CondDetails::originalScope(filepath, format, line, condtype))
        {
            original = true;
        }
        else if (nemesis::CondDetails::closeScope(filepath, format, line, condtype))
        {
            for (auto& edit : *edits)
            {
                add(edit, edit.linenum, condtype.back());
            }

            original = false;
            condtype.pop_back();
            edits = condtype.size() > 0 ? &condtype.back().contents : nullptr;
        }
        else if (original)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            projects[index++].raw->name.addCond(
                nemesis::LinkedVar(static_cast<string>(vline)), condtype.back(), vline.linenum);
        }
        else if (edits)
        {
            edits->push_back(line);
        }
    }
}

void MasterAnimSetData::getlines(VecStr& storeline)
{
    VecStr tempstore;
    storeline.push_back(to_string(projects.size()));

    for (auto& each : projects)
    {
        getLinkedLines(each, tempstore, storeline);
    }

    storeline.insert(storeline.end(), tempstore.begin(), tempstore.end());
}

uint MasterAnimSetData::getIndex(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    if (itr == projectIndexMap.end()) ErrorMessage(5023, projName);

    return itr->second;
}
