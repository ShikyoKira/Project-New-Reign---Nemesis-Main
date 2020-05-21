#include "update/animdata/animdatacond.h"
#include "Global.h"
#include "utilities/writetextfile.h"

using namespace std;

void MasterAnimData::getprojectlines(const ProjectData& proj, VecStr& output, VecStr& output2)
{
    vector<pair<const string*, const nemesis::CondVar<LinkedProjPair>*>> modcodelist;

    for (auto& cond : proj.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(
                    make_pair<const string*, const nemesis::CondVar<LinkedProjPair>*>(&cond.conditions, &cond));
                break;
            }
            case nemesis::FOREACH:
            {
                output.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");
                output2.push_back(output.back());

                for (auto& each : cond.rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                output.push_back("<!-- CLOSE -->");
                output2.push_back(output.back());
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        if (proj.raw)
        {
            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.push_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                getlinkedline(modcode.second->rawlist[0].raw->first, list.back().second);

                for (auto& each : list.back().second)
                {
                    each.append("\t\t\t\t\t<!-- *" + *modcode.first + "* -->");
                }
            }

            size_t max = 0;
            list.push_back(pair<string, VecStr>());
            list.back().first = "original";
            getlinkedline(proj.raw->first, list.back().second);

            for (auto& line : list.back().second)
            {
                line.append("\t\t\t\t\t<!-- original -->");
            }

            for (auto& each : list)
            {
                max = max > each.second.size() ? max : each.second.size();
            }

            for (size_t i = 0; i < max; ++i)
            {
                for (size_t k = 0; k < list.size(); ++k)
                {
                    if (i >= list[k].second.size())
                    {
                        output.push_back(
                            "//* delete this line *//\t\t\t\t\t<!-- "
                            + (list[k].first == "original" ? "original" : "*" + list[k].first + "*")
                            + " -->");
                    }
                    else
                    {
                        output.push_back(list[k].second[i]);
                    }
                }
            }

            proj.raw->second->getlines(output2);
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                output.push_back("<!-- NEW *" + *modcode.first + "* -->");
                output2.push_back("<!-- NEW *" + *modcode.first + "* -->");

                for (auto& each : modcode.second->rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                output.push_back("<!-- CLOSE -->");
                output2.push_back("<!-- CLOSE -->");
            }
        }
    }
    else if (proj.raw)
    {
        getlinkedline(proj.raw->first, output);
        proj.raw->second->getlines(output2);
    }
    else
    {
        output.push_back("//* delete this line *//");
        output2.push_back("//* delete this line *//");
    }
}

bool MasterAnimData::contains(const ProjectName& projName)
{
    auto itr = projectIndexMap.find(projName);

    return itr != projectIndexMap.end();
}

unsigned int MasterAnimData::getIndex(const ProjectName& projName)
{
    auto itr = projectIndexMap.find(projName);

    if (itr == projectIndexMap.end()) ErrorMessage(3025);

    return itr->second;
}

MasterAnimData::ProjectPtr MasterAnimData::find(const ProjectName& projName, const ModCode& modcode)
{
    if (modcode == "original") return projectlist[getIndex(projName)].raw->second;

    auto itr = projectIndexMap.find(projName);

    if (itr == projectIndexMap.end() || !projectlist[itr->second].raw) return nullptr;

    return projectlist[itr->second].raw->second;
}

MasterAnimData::ProjectPtr
MasterAnimData::add(const ProjectName& projName, size_t num, const ModCode& modcode, nemesis::CondType type)
{
    projectIndexMap[projName + "~" + to_string(++projectCounter[projName])] = projectlist.size();
    auto ref = ProjectData(pair<ProjectNameLinked, ProjectPtr>(), num);

    if (modcode != "original")
    {
        projectlist.push_back(nemesis::CondVar(ref));
        projectlist.back().linecount       = num;
        auto& curcond                      = projectlist.back().backCond();
        curcond.conditions                 = modcode;
        curcond.conditionType              = type;
        curcond.rawlist.back().linecount   = num;
        curcond.rawlist.back().raw->first  = projName;
        return curcond.rawlist.back().raw->second;
    }

    projectlist.push_back(ref);
    projectlist.back().raw->first           = projName;
    projectlist.back().raw->first.linecount = num;
    projectlist.back().raw->second          = make_unique<AnimDataProject_Condt>();
    return projectlist.back().raw->second;
}

MasterAnimData::ProjectPtr MasterAnimData::add(const ProjectName& projName,
                                               const VecStr& storeline,
                                               const ModCode& modcode,
                                               nemesis::CondType type)
{
    ProjectPtr proj = find(projName, modcode);

    if (!proj)
    {
        if (modcode == "original")
        {
            add(projName, 0, modcode, type);
            projectlist.back().raw->second = make_unique<AnimDataProject_Condt>(storeline);
            proj                           = projectlist.back().raw->second;
        }
        else
        {
            projectIndexMap[projName + "~" + to_string(++projectCounter[projName])] = projectlist.size();
            auto ref = ProjectData(pair<ProjectNameLinked, ProjectPtr>(), 0);
            projectlist.push_back(nemesis::CondVar(ref));
            projectlist.back().linecount       = 0;
            auto& curcond                      = projectlist.back().backCond();
            curcond.conditions                 = modcode;
            curcond.conditionType              = type;
            curcond.rawlist.back().linecount   = 0;
            curcond.rawlist.back().raw->first  = projName;
            curcond.rawlist.back().raw->second = make_shared<AnimDataProject_Condt>(storeline);
            return curcond.rawlist.back().raw->second;
        }
    }

    return proj;
}

void MasterAnimData::projectListUpdate(const ModCode& modcode,
                                       const string& filepath,
                                       const VecStr& storeline,
                                       bool isTemplate)
{
    size_t pos;

    if (isTemplate)
    {
        vector<VecStr> holdinglist;
        vector<ProjectNameLinked*> curPoint;
        unique_ptr<ProjectData> condStringPtr;
        int open = 0;

        for (unsigned int i = storeline.size() - 1; i >= 0; --i)
        {
            const string& line = storeline[i];
            pos                = line.find("<!-- FOREACH ^");

            if (pos != NOT_FOUND && line.find("^ -->", pos) != NOT_FOUND)
            {
                pos += 14;
                string condition = line.substr(pos, line.find("^ -->", pos));

                if (condStringPtr != nullptr)
                {
                    condStringPtr = make_unique<ProjectData>(
                        nemesis::CondVar(*condStringPtr, condition, nemesis::FOREACH));
                }
                else
                {
                    condStringPtr = make_unique<ProjectData>();
                    condStringPtr->addCond(nemesis::FOREACH);
                    condStringPtr->backCond().conditions = condition;
                }

                for (auto projname : holdinglist.back())
                {
                    condStringPtr->backCond().rawlist.push_back(std::pair<ProjectNameLinked, ProjectPtr>());
                }

                holdinglist.pop_back();

                if (holdinglist.size() == 0)
                {
                    projectlist.push_back(*condStringPtr);
                }
            }
            else if (line.find("<!-- CLOSE -->") != NOT_FOUND)
            {
                ++open;
                holdinglist.push_back(VecStr());
            }
            else if (holdinglist.size() > 0)
            {
                holdinglist.back().insert(holdinglist.back().begin(), line);
            }
        }

        return;
    }

    vector<pair<string, uint>> edits;
    vector<pair<string, uint>> origs;

    bool edited       = false;
    bool originalopen = false;

    for (unsigned int i = 1; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        if (!edited)
        {
            pos    = line.find("<!-- MOD_CODE ~");
            edited = pos != NOT_FOUND && line.find("~ OPEN -->", pos) != NOT_FOUND;
        }
        else if (line.find("<!-- ORIGINAL -->") != NOT_FOUND)
        {
            originalopen = true;
        }
        else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
        {
            edited       = false;
            originalopen = false;

            if (origs.size() > 0)
            {
                if (origs.size() > edits.size()) ErrorMessage(3026, modcode, filepath);

                for (unsigned int k = 0; k < origs.size(); ++k)
                {
                    auto& nestedcondt = projectlist[getIndex(origs[k].first)].raw->first.nestedcond;
                    nestedcondt.push_back(nemesis::CondVar(
                        nemesis::LinkedVar(edits[k].first, edits[k].second), modcode, nemesis::MOD_CODE));
                }
            }

            if (edits.size() > origs.size())
            {
                for (unsigned int k = origs.size(); k < edits.size(); ++k)
                {
                    add(edits[k].first, edits[k].second, modcode, nemesis::MOD_CODE);
                }
            }

            edits.clear();
            origs.clear();
        }
        else if (originalopen)
        {
            origs.push_back(make_pair(line, i));
        }
        else
        {
            edits.push_back(make_pair(line, i));
        }
    }
}

VecStr MasterAnimData::getlines()
{
    VecStr storeline;
    storeline.reserve(170725);

    // project count
    storeline.push_back(to_string(projectlist.size()));

    // project details
    VecStr projdetails;

    // project list
    for (auto& proj : projectlist)
    {
        if (!proj.raw)
        {
            int t = 10;
        }

        getprojectlines(proj, storeline, projdetails);
    }

    storeline.insert(storeline.end(), projdetails.begin(), projdetails.end());
    return storeline;
}

string MasterAnimData::writelines(FileWriter& output) 
{
    string total = output.GetFilePath().string() + "\n";

    for (auto& line : getlines())
    {
        output << line + "\n";
        total.append(line + "\n");
    }

    return total;
}
