#include <functional>

#include "Global.h"

#include "utilities/writetextfile.h"
#include "utilities/conditionsyntax.h"

#include "update/animdata/animdatacond.h"

using namespace std;
namespace ns = nemesis::syntax;

void MasterAnimData::getprojectlines(const ProjectData& proj, VecStr& output, VecStr& output2)
{
    vector<pair<const string*, const nemesis::CondVar<LinkedProjPair>*>> modcodelist;

    auto& addToBoth = [&](const string& input)
    {
        output.push_back(input);
        output2.push_back(input);
    };

    auto& cancloseif = [&](const nemesis::CondVar<LinkedProjPair>& linkedpair, uint index) -> bool {
        if (&linkedpair != &proj.nestedcond.back())
        {
            switch (proj.nestedcond[index + 1].conditionType)
            {
                case nemesis::CondType::ELSEIF:
                case nemesis::CondType::ELSE:
                    return false;
            }
        }

        return true;
    };

    for (uint i = 0; i < proj.nestedcond.size(); ++i)
    {
        auto& cond = proj.nestedcond[i];

        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.emplace_back(
                    make_pair<const string*, const nemesis::CondVar<LinkedProjPair>*>(&cond.conditions, &cond));
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                addToBoth(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                addToBoth(ns::Close());
                break;
            }
            case nemesis::CondType::IF:
            {
                addToBoth(ns::If(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                if (cancloseif(cond, i)) addToBoth(ns::Close());

                break;
            }
            case nemesis::CondType::ELSEIF:
            {
                addToBoth(ns::ElseIf(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                if (cancloseif(cond, i)) addToBoth(ns::Close());

                break;
            }
            case nemesis::CondType::ELSE:
            {
                addToBoth(ns::Else());

                for (auto& each : cond.rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                addToBoth(ns::Close());

                break;
            }
        }
    }

    if (!modcodelist.empty())
    {
        if (proj.raw)
        {
            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.emplace_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                getLinkedLines(modcode.second->rawlist[0].raw->first, list.back().second);

                for (auto& each : list.back().second)
                {
                    each.append(ns::Spaces() + ns::Aster(*modcode.first));
                }
            }

            size_t max = 0;
            list.emplace_back(pair<string, VecStr>());
            list.back().first = "original";
            getLinkedLines(proj.raw->first, list.back().second);

            for (auto& line : list.back().second)
            {
                line.append(ns::Spaces() + ns::LowerOriginal());
            }

            for (auto& each : list)
            {
                max = max > each.second.size() ? max : each.second.size();
            }

            for (size_t i = 0; i < max; ++i)
            {
                for (auto& each : list)
                {
                    if (i >= each.second.size())
                    {
                        string constr = each.first == "original" ? ns::LowerOriginal()
                                                                 : ns::Aster(each.first);
                        output.push_back(ns::DeleteLine() + ns::Spaces() + constr);
                    }
                    else
                    {
                        output.push_back(each.second[i]);
                    }
                }
            }

            proj.raw->second->getlines(output2);
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                addToBoth(ns::ModCode(*modcode.first));

                for (auto& each : modcode.second->rawlist)
                {
                    getprojectlines(each, output, output2);
                }

                addToBoth(ns::Close());
            }
        }
    }
    else if (proj.raw)
    {
        getLinkedLines(proj.raw->first, output);
        proj.raw->second->getlines(output2);
    }
    else
    {
        addToBoth(ns::DeleteLine());
    }
}

bool MasterAnimData::contains(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(projName);

    return itr != projectIndexMap.end();
}

unsigned int MasterAnimData::getIndex(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(projName);

    if (itr == projectIndexMap.end()) ErrorMessage(3025);

    return itr->second;
}

MasterAnimData::ProjectPtr MasterAnimData::find(const ProjectName& projName, const ModCode& modcode) const
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
        projectlist.emplace_back(nemesis::CondVar(ref));
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
            projectlist.emplace_back(nemesis::CondVar(ref));
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
                                       bool isTemplate)
{
    size_t pos;
    VecNstr storeline;

    if (!GetFunctionLines(filepath, storeline, false)) return;

    if (isTemplate)
    {
        vector<VecStr> holdinglist;
        unique_ptr<ProjectData> condStringPtr;
        int open = 0;

        for (unsigned int i = storeline.size() - 1; i >= 0; --i)
        {
            const string& line = storeline[i];
            pos                = line.find(ns::ForEach());

            if (pos != NOT_FOUND && line.find(ns::EndSyntax(), pos) != NOT_FOUND)
            {
                pos += 14;
                string condition = line.substr(pos, line.find(ns::EndSyntax(), pos));

                if (condStringPtr != nullptr)
                {
                    condStringPtr = make_unique<ProjectData>(
                        nemesis::CondVar(*condStringPtr, condition, nemesis::CondType::FOREACH));
                }
                else
                {
                    condStringPtr = make_unique<ProjectData>();
                    condStringPtr->addCond(nemesis::CondType::FOREACH);
                    condStringPtr->backCond().conditions = condition;
                }

                for (auto projname : holdinglist.back())
                {
                    condStringPtr->backCond().rawlist.emplace_back(
                        std::pair<ProjectNameLinked, ProjectPtr>());
                }

                holdinglist.pop_back();

                if (holdinglist.size() == 0)
                {
                    projectlist.push_back(*condStringPtr);
                }
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
            {
                ++open;
                holdinglist.emplace_back(VecStr());
            }
            else if (holdinglist.size() > 0)
            {
                holdinglist.back().insert(holdinglist.back().begin(), line);
            }
        }

        return;
    }

    VecNstr edits;
    VecNstr origs;
    map<uint, string> tempISMap;
    map<string, uint> tempProjCounter;

    bool edited       = false;
    bool originalopen = false;

    for (auto& line : storeline)
    {
        if (!edited || originalopen)
        {
            size_t pos = line.find(".txt");

            if (pos != NOT_FOUND && line.find(ns::OpenComment()) == NOT_FOUND)
            {
                auto& templine = tempISMap[line.GetLineNumber()];
                templine       = line.substr(0, pos).ToString();
                templine.append("~");
                templine.append(to_string(++tempProjCounter[line.ToString()]));
            }
        }

        if (!edited)
        {
            pos    = line.find(ns::ModCode());
            edited = pos != NOT_FOUND && line.find(ns::EndModCodeSyntax(), pos) != NOT_FOUND;
        }
        else if (line.find(ns::Original()) != NOT_FOUND)
        {
            originalopen = true;
        }
        else if (line.find(ns::Close()) != NOT_FOUND)
        {
            edited       = false;
            originalopen = false;

            if (!origs.empty())
            {
                if (origs.size() > edits.size()) ErrorMessage(3026, modcode, filepath);

                for (unsigned int k = 0; k < origs.size(); ++k)
                {
                    auto& nestedcondt
                        = projectlist[getIndex(tempISMap[origs[k].GetLineNumber()])].raw->first.nestedcond;
                    bool exist = false;

                    for (auto& condt : nestedcondt)
                    {
                        if (condt.conditions == modcode && condt.conditionType == nemesis::CondType::MOD_CODE)
                        {
                            exist = true;
                            break;
                        }
                    }

                    if (!exist)
                    {
                        nestedcondt.emplace_back(nemesis::CondVar(
                            nemesis::LinkedVar(edits[k].ToString(), edits[k].GetLineNumber()),
                            modcode,
                            nemesis::CondType::MOD_CODE));
                    }
                }
            }

            if (edits.size() > origs.size())
            {
                for (unsigned int k = origs.size(); k < edits.size(); ++k)
                {
                    if (contains(edits[k].ToString())) continue;

                    add(edits[k], edits[k].GetLineNumber(), modcode, nemesis::CondType::MOD_CODE);
                }
            }

            edits.clear();
            origs.clear();
        }
        else if (originalopen)
        {
            origs.emplace_back(line);
        }
        else
        {
            edits.emplace_back(line);
        }
    }
}

void MasterAnimData::getlines(VecStr& lines)
{
    lines.clear();
    lines.reserve(170725);

    // project count
    lines.push_back(to_string(projectlist.size()));

    // project details
    VecStr projdetails;

    // project list
    for (auto& proj : projectlist)
    {
        if (!proj.raw)
        {
            int t = 10;
        }

        getprojectlines(proj, lines, projdetails);
    }

    lines.insert(lines.end(), projdetails.begin(), projdetails.end());
}

const string& MasterAnimData::SaveTemplateAs(const filesystem::path& filepath)
{
    FileWriter writer(filepath);

    if (!writer.is_open()) ErrorMessage(2009, writer.GetFilePath());

    totalline = filepath.string();
    ReadTemplateLine(totalline);
    writer << totalline;
    return totalline;
}

void MasterAnimData::ReadTemplateLine(std::string& totalline)
{
    VecStr adlines;
    getlines(adlines);

    for (auto& line : adlines)
    {
        totalline.append(line);
        totalline.push_back('\n');
    }
}

void MasterAnimData::ReadTemplateLines(VecStr& lines)
{
    getlines(lines);
}
