#include "Global.h"

#include "utilities/writetextfile.h"
#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"

#include "update/animsetdata/masteranimsetdata.h"

using namespace std;

MasterAnimSetData::Parser::Parser(MasterAnimSetData& base, const VecNstr& storeline) noexcept
    : host(base)
    , storeline(storeline)
{
}

void MasterAnimSetData::Parser::SetFormat(const string& format) noexcept
{
    this->format = format;
}

void MasterAnimSetData::Parser::SetPath(std::filesystem::path path) noexcept
{
    this->path = path;
}

void MasterAnimSetData::Parser::ImportProject()
{
    index    = 0;

    //bool original = false;
    //deque<nemesis::Line>* edits;
    //vector<nemesis::CondDetails> condtype;
    //nemesis::CondCheckFunc condfunc;

    tobedeleted = nullptr;
    nemesis::ConditionScope cscope(format, path);

    for (const auto& line : storeline)
    {
        auto conditioninfo = cscope.TryGetConditionInfo(line);

        if (!conditioninfo)
        {
            if (cscope.Empty())
            {
                PointingProject();
            }
            else if (cscope.Back()->GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriProject();
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted();
                }

                CloseProject();
                tobedeleted = nullptr;
                break;
            }
        }


        //if (condfunc(path, format, line, condtype))
        //{
        //    edits = &condtype.back().contents;
        //    continue;
        //}

        //if (condtype.empty() && line.find(nemesis::syntax::OpenComment()) == NOT_FOUND)
        //{
        //    ++index;
        //    continue;
        //}

        //if (nemesis::CondDetails::originalScope(path, format, line, condtype))
        //{
        //    original = true;
        //}
        //else if (nemesis::CondDetails::closeScope(path, format, line, condtype))
        //{
        //    for (auto& edit : *edits)
        //    {
        //        host.add(edit, edit.GetLineNumber(), condtype.back());
        //    }

        //    original = false;
        //    condtype.pop_back();
        //    edits = !condtype.empty() ? &condtype.back().contents : nullptr;
        //}
        //else if (original)
        //{
        //    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
        //    host.projects[index++].raw->name.addCond(
        //        nemesis::LinkedVar(vline.ToString()), condtype.back(), vline.GetLineNumber());
        //}
        //else if (edits)
        //{
        //    edits->emplace_back(line);
        //}
    }
}

void MasterAnimSetData::Parser::PointingProject() noexcept
{
    ++index;
}

void MasterAnimSetData::Parser::PointingOriProject()
{
    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(&tobedeleted->GetRefContents());
    nemesis::LinkedVar linkedline(vline.ToString());
    host.projects[index++].raw->name.addCond(linkedline, tobedeleted->GetType(), vline.GetLineNumber());
}

void MasterAnimSetData::Parser::CloseProject()
{
    for (auto& edit : tobedeleted->GetContents())
    {
        host.add(edit, edit.GetLineNumber(), *tobedeleted);
    }
}

bool MasterAnimSetData::contains(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    return itr != projectIndexMap.end();
}

SPtr<ProjectAnimSetData> MasterAnimSetData::find(const ProjectName& projName)
{
    return projects[getIndex(projName)].raw;
}

SPtr<ProjectAnimSetData> MasterAnimSetData::find(const ProjectName& projName, const ModCode& condition)
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

SPtr<ProjectAnimSetData>
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

SPtr<ProjectAnimSetData>& MasterAnimSetData::add(const nemesis::Line& line)
{
    projectIndexMap[nemesis::to_lower_copy(line).ToString()] = projects.size();
    projects.emplace_back(nemesis::LinkedVar<ProjectAnimSetData>(ProjectAnimSetData()));
    projects.back().raw->name = line.ToString();
    return projects.back().raw;
}

SPtr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name, size_t num)
{
    add(name);
    projects.back().raw->name.linecount = num;
    return projects.back().raw;
}

SPtr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name,
                                                 size_t num,
                                                 const std::string& condition,
                                                 nemesis::CondType type)
{
    projectIndexMap[nemesis::to_lower_copy(name)] = projects.size();
    projects.emplace_back(nemesis::CondVar(nemesis::LinkedVar<ProjectAnimSetData>(ProjectAnimSetData())));
    projects.back().linecount        = num;
    auto& curcond                    = projects.back().backCond();
    curcond.conditions               = condition;
    curcond.conditionType            = type;
    curcond.rawlist.back().linecount = num;
    curcond.rawlist.back().raw->name = name;
    return curcond.rawlist.back().raw;
}

SPtr<ProjectAnimSetData>&
MasterAnimSetData::add(const ProjectName& name, size_t num, const nemesis::CondDetails& condition)
{
    return add(name, num, condition.condition, condition.type);
}

SPtr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name, const VecNstr& storeline)
{
    auto proj = find(name);

    if (proj) return proj;

    proj = make_shared<ProjectAnimSetData>(name, storeline);
    add(name, 0);
    projects.back().raw = proj;
    return proj;
}

SPtr<ProjectAnimSetData>& MasterAnimSetData::add(const ProjectName& name,
                                                 const VecNstr& storeline,
                                                 const nemesis::CondDetails& condition)
{
    auto proj = find(name, condition.condition);

    if (proj) return proj;

    proj = make_shared<ProjectAnimSetData>(name, storeline);
    add(name, 0, condition);
    auto& curcond              = projects.back().backCond();
    curcond.rawlist.back().raw = proj;
    return curcond.rawlist.back().raw;
}

SPtr<ProjectAnimSetData>&
MasterAnimSetData::add(const ProjectName& name, size_t num, const nemesis::ConditionInfo& conditioninfo)
{
    return add(name, num, conditioninfo.GetCondition(), conditioninfo.GetType());
}

void MasterAnimSetData::projectListUpdate(const ModCode& format,
                                          const string& filepath,
                                          nemesis::CondCheckFunc condfunc)
{
    VecNstr storeline;
    GetFunctionLines(filepath, storeline, false);

    Parser parser(*this, storeline);
    parser.SetFormat(format);
    parser.SetPath(filepath);

    parser.ImportProject();
}

void MasterAnimSetData::getlines(VecStr& storeline)
{
    VecStr tempstore;
    storeline.emplace_back(to_string(projects.size()));

    for (auto& each : projects)
    {
        getLinkedLines(each, tempstore, storeline);
    }

    storeline.insert(storeline.end(), tempstore.begin(), tempstore.end());
}

const std::string& MasterAnimSetData::SaveTemplateAs(const std::filesystem::path& filepath)
{
    FileWriter writer(filepath);

    if (!writer.is_open()) ErrorMessage(2009, writer.GetFilePath());

    totalline = filepath.string();
    ReadTemplateLine(totalline);
    writer << totalline;
    return totalline;
}

void MasterAnimSetData::ReadTemplateLine(std::string& totalline)
{
    VecStr asdlines;
    getlines(asdlines);

    for (auto& line : asdlines)
    {
        totalline.append(line);
        totalline.push_back('\n');
    }
}

void MasterAnimSetData::ReadTemplateLines(VecStr& lines)
{
    getlines(lines);
}

uint MasterAnimSetData::getIndex(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    if (itr == projectIndexMap.end()) ErrorMessage(5023, projName);

    return itr->second;
}
