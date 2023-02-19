#include <functional>

#include "Global.h"

#include "utilities/writetextfile.h"
#include "utilities/conditionsyntax.h"

#include "update/animdata/animdatacond.h"

using namespace std;
namespace ns = nemesis::syntax;
namespace sf = std::filesystem;

MasterAnimData::ModPatch::Project::Project(const std::filesystem::path& projdir)
{
    directory  = projdir;
    name       = projdir.stem().string();
    size_t pos = name.find_last_of("~");

    if (pos == NOT_FOUND) return;

    name.replace(pos, 0, ".txt");
}

void MasterAnimData::ModPatch::Project::AddHeader(const std::filesystem::path& headerpath)
{
    if (sf::is_directory(headerpath)) return;

    header = std::make_unique<nemesis::File>(headerpath);
}

void MasterAnimData::ModPatch::Project::AddUniqueCodeList(const VecStr& uniquecodelist)
{
    for (auto& uniquecode : uniquecodelist)
    {
        auto path = directory / uniquecode;

        if (sf::is_directory(path)) continue;

        this->uniquecodelist.emplace_back(path);
    }
}

void MasterAnimData::ModPatch::AddHeader(const std::string& code, const std::filesystem::path& headerdir)
{
    auto fullpath = headerdir / MasterAnimData::header_file;

    if (!sf::exists(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

    header = std::make_unique<nemesis::File>(fullpath);
}

void MasterAnimData::ModPatch::AddProject(const std::string& code, const std::filesystem::path& projdir)
{
    VecStr uniquecodelist;
    read_directory(projdir, uniquecodelist);
    auto headerpath = projdir / MasterAnimData::header_file;
    auto& project   = projectlist.emplace_back(projdir);
    
    if (sf::exists(headerpath))
    {
        project.AddHeader(headerpath);
        auto itr = std::remove_if(uniquecodelist.begin(), uniquecodelist.end(), [&](std::string& uniquecode) {
            return nemesis::iequals(uniquecode, MasterAnimData::header_file.data());
        });
        uniquecodelist.erase(itr, uniquecodelist.end());
    }

    project.AddUniqueCodeList(uniquecodelist);
}

MasterAnimData::ModPatch::ModPatch(const std::string& code, const std::filesystem::path& path)
    : code(code)
{
    VecStr projfiles;
    read_directory(path, projfiles);
    auto headerdir = path / MasterAnimData::header_str;

    if (sf::exists(headerdir))
    {
        AddHeader(code, headerdir);
        auto itr = std::remove_if(projfiles.begin(), projfiles.end(), [&](const std::string& projfile) {
            return nemesis::iequals(projfile, MasterAnimData::header_str.data());
        });
        projfiles.erase(itr, projfiles.end());
    }

    for (auto& projfile : projfiles)
    {
        auto projdir = path / projfile;

        if (!sf::is_directory(projdir)) continue;

        AddProject(code, projdir);
    }
}

void MasterAnimData::ModPatcher::AddHeader()
{
    if (!patch.header) return;

    auto& header = *patch.header;
    corefiles.emplace_back(header);
    master_ad.projectListUpdate(patch.code, header.GetFilePath(), false);
}

void MasterAnimData::ModPatcher::AddProject(const ModPatch::Project& project)
{
    if (error) throw nemesis::exception();

    if (!master_ad.contains(project.name)) ErrorMessage(3011, MasterAnimData::header_file, project.name, patch.code);

    AddProjectHeader(project);
    AddUniqueCodes(project);
}

void MasterAnimData::ModPatcher::AddProjectHeader(const ModPatch::Project& project)
{
    if (!project.header) return;

    auto projindex = master_ad.getIndex(project.name);
    auto& proj     = master_ad.projectlist[projindex];

    auto& path = *project.header;
    VecNstr lines = path.GetLines();

    corefiles.emplace_back(path);

    if (!patch.header)
    {
        proj.raw->second->modify(patch.code, lines);
        return;
    }

    for (auto& cond : proj.nestedcond)
    {
        if (cond.conditions != patch.code) continue;

        if (cond.rawlist[0].raw->second) continue;

        lines.insert(lines.begin(), "");
        cond.rawlist[0].raw->second = make_shared<AnimDataProject_Condt>(lines);
        return;
    }

    ErrorMessage(2025);
}

void MasterAnimData::ModPatcher::AddUniqueCodes(const ModPatch::Project& project)
{
    auto projindex = master_ad.getIndex(project.name);
    auto& proj     = master_ad.projectlist[projindex];

    if (!proj.raw)
    {
        MasterAnimData::ProjectPtr projptr = master_ad.GetConditionedProject(proj, patch.code);

        if (!projptr) ErrorMessage(3021, project.name);

        for (auto& uniquecode : project.uniquecodelist)
        {
            if (!master_ad.AddConditionedPatch(projptr, uniquecode.GetFilePath())) continue;

            corefiles.emplace_back(uniquecode);
        }

        return;
    }

    auto& curproj = proj.raw->second;

    for (auto& uniquecode : project.uniquecodelist)
    {
        if (!curproj->AddPatch(uniquecode.GetFilePath(), patch.code, project.name)) continue;

        corefiles.emplace_back(uniquecode);
    }
}

MasterAnimData::ModPatcher::ModPatcher(MasterAnimData& master_ad,
                                       Vec<nemesis::File>& corefiles,
                                       const ModPatch& patch)
    : master_ad(master_ad)
    , corefiles(corefiles)
    , patch(patch)
{
}

void MasterAnimData::ModPatcher::Patch()
{
    AddHeader();

    for (auto& project : patch.projectlist)
    {
        AddProject(project);
    }
}

void MasterAnimData::getprojectlines(const ProjectData& proj, VecStr& output, VecStr& output2)
{
    vector<pair<const string*, const nemesis::CondVar<LinkedProjPair>*>> modcodelist;

    auto addToBoth = [&](const string& input)
    {
        output.push_back(input);
        output2.push_back(input);
    };

    auto cancloseif = [&](const nemesis::CondVar<LinkedProjPair>& linkedpair, size_t index) -> bool {
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

    for (size_t i = 0; i < proj.nestedcond.size(); ++i)
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

size_t MasterAnimData::getIndex(const ProjectName& projName) const
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

    if (modcode == "original")
    {
        projectlist.push_back(ref);
        projectlist.back().raw->first           = projName;
        projectlist.back().raw->first.linecount = num;
        projectlist.back().raw->second          = make_unique<AnimDataProject_Condt>();
        return projectlist.back().raw->second;
    }

    projectlist.emplace_back(nemesis::CondVar(ref));
    projectlist.back().linecount      = num;
    auto& curcond                     = projectlist.back().backCond();
    curcond.conditions                = modcode;
    curcond.conditionType             = type;
    curcond.rawlist.back().linecount  = num;
    curcond.rawlist.back().raw->first = projName;
    return curcond.rawlist.back().raw->second;
}

MasterAnimData::ProjectPtr MasterAnimData::add(const ProjectName& projName,
                                               const VecNstr& storeline,
                                               const ModCode& modcode,
                                               nemesis::CondType type)
{
    ProjectPtr proj = find(projName, modcode);

    if (proj) return proj;

    if (modcode == "original")
    {
        add(projName, 0, modcode, type);
        projectlist.back().raw->second = make_unique<AnimDataProject_Condt>(storeline);
        return projectlist.back().raw->second;
    }

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

void MasterAnimData::projectListUpdate(const ModCode& modcode,
                                       const std::filesystem::path& filepath,
                                       bool isTemplate)
{
    if (isTemplate)
    {
        UpdateTemplateProjectList(filepath);
        return;
    }

    UpdateBaseProjectList(modcode, filepath);
}

void MasterAnimData::UpdateBaseProjectList(const ModCode& modcode, const std::filesystem::path& filepath)
{
    VecNstr storeline;

    if (!GetFileLines(filepath, storeline, false)) return;

    VecNstr edits;
    VecNstr origs;
    map<size_t, string> tempISMap;
    map<string, size_t> tempProjCounter;

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
            auto pos = line.find(ns::ModCode());
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

                for (size_t k = 0; k < origs.size(); ++k)
                {
                    auto& nestedcondt
                        = projectlist[getIndex(tempISMap[origs[k].GetLineNumber()])].raw->first.nestedcond;
                    bool exist = false;

                    for (auto& condt : nestedcondt)
                    {
                        if (condt.conditions != modcode) continue;

                        if (condt.conditionType != nemesis::CondType::MOD_CODE) continue;

                        exist = true;
                        break;
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
                for (size_t k = origs.size(); k < edits.size(); ++k)
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

void MasterAnimData::UpdateTemplateProjectList(const std::filesystem::path& filepath)
{
    VecNstr storeline;

    if (!GetFileLines(filepath, storeline, false)) return;

    Vec<VecStr> holdinglist;
    UPtr<ProjectData> condStringPtr;

    for (size_t i = storeline.size() - 1; i >= 0; --i)
    {
        auto& line = storeline[i];
        auto pos   = line.find(ns::ForEach());

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
                condStringPtr->backCond().rawlist.emplace_back(std::pair<ProjectNameLinked, ProjectPtr>());
            }

            holdinglist.pop_back();

            if (holdinglist.empty())
            {
                projectlist.push_back(*condStringPtr);
            }
        }
        else if (line.find(ns::Close()) != NOT_FOUND)
        {
            holdinglist.emplace_back();
        }
        else if (!holdinglist.empty())
        {
            holdinglist.back().insert(holdinglist.back().begin(), line.ToString());
        }
    }
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

void MasterAnimData::AddModPatch(Vec<nemesis::File>& corefiles, const MasterAnimData::ModPatch& modpatch)
{
    if (projectlist.empty()) ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");

    ModPatcher patcher(*this, corefiles, modpatch);
    patcher.Patch();
}
//
//bool MasterAnimData::AddHeaderPatch(Vec<nemesis::File>& corefiles,
//                                    const MasterAnimData::ModPatch::Project& project,
//                                    const MasterAnimData::ModPatch& modpatch)
//{
//    if (!project.header) return false;
//
//    auto projindex = getIndex(project.name);
//    auto& proj     = projectlist[projindex];
//
//    VecNstr lines;
//    auto& path = *project.header;
//
//    if (!GetFileLines(path.GetFilePath(), lines, false)) return false;
//
//    corefiles.emplace_back(path);
//
//    if (!modpatch.header)
//    {
//        proj.raw->second->modify(modpatch.code, lines);
//        return true;
//    }
//
//    for (auto& cond : proj.nestedcond)
//    {
//        if (cond.conditions != modpatch.code) continue;
//
//        if (cond.rawlist[0].raw->second) continue;
//
//        lines.insert(lines.begin(), "");
//        cond.rawlist[0].raw->second = make_shared<AnimDataProject_Condt>(lines);
//        return true;
//    }
//
//    ErrorMessage(2025);
//    return false;
//}
//
//void MasterAnimData::AddProjectHeaderPatch(Vec<nemesis::File>& corefiles,
//                                           const MasterAnimData::ModPatch::Project& project,
//                                           const MasterAnimData::ModPatch& modpatch)
//{
//    auto projindex = getIndex(project.name);
//    auto& proj     = projectlist[projindex];
//
//    if (!proj.raw)
//    {
//        MasterAnimData::ProjectPtr projptr = GetConditionedProject(proj, modpatch.code);
//
//        if (!projptr) ErrorMessage(3021, project.name);
//
//        for (auto& uniquecode : project.uniquecodelist)
//        {
//            if (!AddConditionedPatch(projptr, uniquecode.GetFilePath())) continue;
//
//            corefiles.emplace_back(uniquecode);
//        }
//
//        return;
//    }
//
//    auto& curproj = proj.raw->second;
//
//    for (auto& uniquecode : project.uniquecodelist)
//    {
//        if (!curproj->AddPatch(uniquecode.GetFilePath(), modpatch.code, project.name)) continue;
//
//        corefiles.emplace_back(uniquecode);
//    }
//}

MasterAnimData::ProjectPtr MasterAnimData::GetConditionedProject(ProjectData& project, const std::string& code)
{
    for (auto& cond : project.nestedcond)
    {
        if (cond.conditions != code) continue;

        for (auto& each : cond.rawlist)
        {
            if (!each.raw) continue;

            return each.raw->second;
        }
    }

    return nullptr;
}

bool MasterAnimData::AddConditionedPatch(ProjectPtr projptr, const std::filesystem::path& uniquecode_path)
{
    std::string filename = uniquecode_path.stem().string();

    VecNstr lines;

    if (!GetFileLines(uniquecode_path, lines, false)) return false;

    if (isOnlyNumber(filename)) // info data
    {
        projptr->iadd(filename, "original", lines, 1);
        return true;
    }

    nemesis::smatch sm;

    if (!nemesis::regex_search(filename, sm, nemesis::regex("[^~]*~[0-9]+")))
    {
        ErrorMessage(2004, uniquecode_path);
    }

    projptr->aadd(filename, "original", lines, 1);
    return true;
}

//
//void MasterAnimData::AddModPatch(const std::string& code, const std::filesystem::path& projdir)
//{
//    if (projectlist.empty()) ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");
//
//    VecStr projfiles;
//    read_directory(projdir, projfiles);
//    auto headerdir = projdir / MasterAnimData::header_str;
//
//    if (sf::exists(headerdir))
//    {
//        AddAnimDataHeaderModPatch(code, headerdir);
//        std::remove_if(projfiles.begin(), projfiles.end(), [&](const std::string& projfile) {
//            return nemesis::iequals(projfile, MasterAnimData::header_str.data());
//        });
//    }
//
//    for (auto& projfile : projfiles)
//    {
//        if (projfile != MasterAnimData::header_str)
//        {
//            size_t pos = projfile.find_last_of("~");
//
//            if (pos != NOT_FOUND) projfile.replace(pos, 0, ".txt");
//        }
//
//        AddAnimDataProjModPatch(code, projfile, path);
//    }
//}

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
