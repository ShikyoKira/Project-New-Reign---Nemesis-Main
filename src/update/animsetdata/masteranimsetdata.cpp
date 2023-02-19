#include "Global.h"

#include "utilities/writetextfile.h"
#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"

#include "update/animsetdata/masteranimsetdata.h"

using namespace std;

namespace sf = std::filesystem;

MasterAnimSetData::ModPatch::Project::AnimSetData::AnimSetData(const std::string& code, const std::filesystem::path& path)
    : file(path)
{
    name = nemesis::to_lower_copy(path.filename().string());
    size_t pos = name.find(nemesis::to_lower_copy(code) + "$");

    if (pos != 0) return;

    name = name.substr(pos + code.length() + 1);
    isnew = true;
}

void MasterAnimSetData::ModPatch::Project::AddAnimSetData(const std::string& code,
                                                          const std::filesystem::path& path)
{
    asdlist.emplace_back(code, path);
}

void MasterAnimSetData::ModPatch::Project::AddHeader(const std::filesystem::path& path)
{
    header = std::make_unique<nemesis::File>(path);
}

MasterAnimSetData::ModPatch::Project::Project(const std::string& code, const sf::path& path)
{
    name = nemesis::to_lower_copy(path.filename().string()) + ".txt";
    std::replace_if(
        name.begin(), name.end(), [&](const char& ch) { return ch == '~'; }, '\\');

    VecStr contents;
    read_directory(path, contents);

    for (auto& each : contents)
    {
        auto each_path = path / each;

        if (sf::is_directory(each_path)) continue;

        if (each == MasterAnimSetData::ModPatch::header_file)
        {
            AddHeader(each_path);
            continue;
        }

        AddAnimSetData(code, each_path);
    }
}

void MasterAnimSetData::ModPatch::AddHeader(const sf::path& path)
{
    if (!sf::exists(path)) return;

    if (sf::is_directory(path)) return;

    header = std::make_unique<nemesis::File>(path);
}

void MasterAnimSetData::ModPatch::AddProject(const sf::path& path)
{
    projectlist.emplace_back(code, path);
}

MasterAnimSetData::ModPatch::ModPatch(const std::string& code, const sf::path& path)
    : code(nemesis::to_lower_copy(code))
{
    VecStr projlist;
    read_directory(path, projlist);

    for (auto& each : projlist)
    {
        auto proj_path = path / each;

        if (!sf::is_directory(proj_path)) continue;

        if (each == header_str)
        {
            AddHeader(proj_path / header_file);
            continue;
        }

        AddProject(proj_path);
    }
}

MasterAnimSetData::Parser::Parser(MasterAnimSetData& base, const VecNstr& storeline) noexcept
    : host(base)
    , storeline(storeline)
{
}

void MasterAnimSetData::Parser::SetFormat(const string& format) noexcept
{
    this->format = format;
}

void MasterAnimSetData::Parser::SetPath(const sf::path& path) noexcept
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
            else if (cscope.Back().GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriProject();
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted().shared_from_this();
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

void MasterAnimSetData::ModPatcher::AddHeaderModPatch()
{
    if (!patch.header) return;

    corefiles.emplace_back(patch.header->GetFilePath());
    master_asd.projectListUpdate(patch.code, patch.header->GetFilePath(), nemesis::CondDetails::modCheck);
}

void MasterAnimSetData::ModPatcher::AddProjectModPatch(const MasterAnimSetData::ModPatch::Project& project)
{
    if (error) throw nemesis::exception();

    if (!master_asd.contains(project.name)) ErrorMessage(5000, "$header$.txt", project.name, patch.code);

    auto projptr = master_asd.find(project.name, patch.code).get();
    AddProjectHeaderModPatch(projptr, project);

    for (auto& asd : project.asdlist)
    {
        AddAnimSetData(projptr, asd);
    }
}

void MasterAnimSetData::ModPatcher::AddProjectHeaderModPatch(
    ProjectAnimSetData* projectptr, const MasterAnimSetData::ModPatch::Project& project)
{
    if (!project.header) return;

    auto& file        = *project.header;
    size_t num        = 1;
    VecNstr lines     = file.GetLines();
    sf::path filepath = file.GetFilePath();

    if (patch.header)
    {
        projectptr->ImportData(filepath, num, lines);
        return;
    }

    projectptr->ImportData(filepath, num, lines, patch.code);
}

void MasterAnimSetData::ModPatcher::AddAnimSetData(
    ProjectAnimSetData* projectptr, const MasterAnimSetData::ModPatch::Project::AnimSetData& asd)
{
    sf::path filepath = asd.file.GetFilePath();
    VecNstr lines     = asd.file.GetLines();
    size_t num        = 2;

    corefiles.emplace_back(asd.file);

    if (asd.isnew)
    {
        projectptr->find(asd.name, patch.code)->ImportData(filepath, num, lines, patch.code);
        return;
    }

    projectptr->find(asd.name)->ImportData(filepath, num, lines, patch.code);
}

MasterAnimSetData::ModPatcher::ModPatcher(MasterAnimSetData& master_asd,
                                          Vec<nemesis::File>& corefiles,
                                          const ModPatch& patch)
    : master_asd(master_asd)
    , patch(patch)
    , corefiles(corefiles)
{
    AddHeaderModPatch();

    for (auto& project: patch.projectlist)
    {
        AddProjectModPatch(project);
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

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const nemesis::Line& line)
{
    projectIndexMap[nemesis::to_lower_copy(line).ToString()] = projects.size();
    projects.emplace_back(nemesis::LinkedVar<ProjectAnimSetData>(ProjectAnimSetData()));
    projects.back().raw->name = line.ToString();
    return projects.back().raw;
}

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const ProjectName& name, size_t num)
{
    add(name);
    projects.back().raw->name.linecount = num;
    return projects.back().raw;
}

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const ProjectName& name,
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

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const ProjectName& name, size_t num, const nemesis::CondDetails& condition)
{
    return add(name, num, condition.condition, condition.type);
}

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const ProjectName& name, const VecNstr& storeline)
{
    auto proj = find(name);

    if (proj) return proj;

    proj = std::make_shared<ProjectAnimSetData>(name, storeline);
    add(name, 0);
    projects.back().raw = proj;
    return proj;
}

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const ProjectName& name,
                                                 const VecNstr& storeline,
                                                 const nemesis::CondDetails& condition)
{
    auto proj = find(name, condition.condition);

    if (proj) return proj;

    proj = std::make_shared<ProjectAnimSetData>(name, storeline);
    add(name, 0, condition);
    auto& curcond              = projects.back().backCond();
    curcond.rawlist.back().raw = proj;
    return curcond.rawlist.back().raw;
}

SPtr<ProjectAnimSetData> MasterAnimSetData::add(const ProjectName& name, size_t num, const nemesis::ConditionInfo& conditioninfo)
{
    return add(name, num, conditioninfo.GetCondition(), conditioninfo.GetType());
}

void MasterAnimSetData::projectListUpdate(const string& modcode,
                                          const sf::path& filepath,
                                          nemesis::CondCheckFunc condfunc)
{
    VecNstr storeline;
    GetFileLines(filepath, storeline, false);

    Parser parser(*this, storeline);
    parser.SetFormat(modcode);
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

const std::string& MasterAnimSetData::SaveTemplateAs(const sf::path& filepath)
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

void MasterAnimSetData::AddModPatch(Vec<nemesis::File>& corefiles,
                                                  const MasterAnimSetData::ModPatch& patch)
{
    ModPatcher patcher(*this, corefiles, patch);
}

size_t MasterAnimSetData::getIndex(const ProjectName& projName) const
{
    auto itr = projectIndexMap.find(nemesis::to_lower_copy(projName));

    if (itr == projectIndexMap.end()) ErrorMessage(5023, projName);

    return itr->second;
}
