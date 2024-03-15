#include "debugmsg.h"

#include "utilities/lastupdate.h"
#include "utilities/readtextfile.h"
#include "utilities/stringextension.h"
#include "utilities/conditionsyntax.h"

#include "update/functionupdate.h"

#include "update/animsetdata/masteranimsetdata.h"

#include "update/patch.h"

#pragma warning(disable : 4503)

using namespace std;
namespace ns = nemesis::syntax;

typedef vector<string> VecStr;

bool ClassCheck(vector<int>& modEditCoordinate,
                unordered_map<string, int>& modEditLine,
                unordered_map<int, int>& NewCoordinate,
                int linecount,
                VecStr& storeline,
                string filepath,
                string projectfile,
                string filename,
                string modcode);
void addAnimDataPack(const string& projectfile,
                     const string& filename,
                     const string& modcode,
                     const MasterAnimData::ProjectPtr& projData,
                     const VecNstr& storeline);
void addInfoDataPack(const string& filepath,
                     const string& filename,
                     const string& modcode,
                     const MasterAnimData::ProjectPtr& projData,
                     const VecNstr& storeline);

bool GetFunctionEdits(nemesis::Line& line, VecNstr storeline, int numline)
{
    if (numline < int(storeline.size()))
    {
        line = storeline[numline];
        return true;
    }

    error = true;
    return false;
}

VecNstr GetFunctionEdits(const string& filename, VecNstr storeline, int startline, int endline)
{
    VecNstr storage;
    storage.reserve(endline);
    int combine = endline + startline;

    if (int(storeline.size()) <= combine) ErrorMessage(2002, filename, startline, startline + endline);

    storage.insert(storage.end(), storeline.begin() + startline, storeline.begin() + combine);
    return storage;
}

bool NodeU::NodeUpdate(string modcode,
                       string behaviorfile,
                       string nodefile,
                       unique_ptr<map<string, VecNstr, alphanum_less>>& newFile,
                       unique_ptr<UMapStr2>& stateID,
                       unique_ptr<UMapStr2>& parent,
                       unique_ptr<unordered_map<string, VecStr>>& statelist,
                       unordered_map<wstring, wstring>& lastUpdate
#if MULTITHREADED_UPDATE
                       ,
                       atomic_flag& filelock,
                       atomic_flag& stateLock,
                       atomic_flag& parentLock
#endif
)
{
    if (behaviorPath[nemesis::transform_to<wstring>(nemesis::to_lower_copy(behaviorfile))].empty())
    {
        ErrorMessage(2006, behaviorfile + ".hkx");
    }

    string filecheck = nemesis::regex_replace(
                           string(nodefile), nemesis::regex(".+?([0-9]+)\\.[t|T][x|X][t|T]$"), string("$1"))
                       + ".txt";
    string nodeID   = nodefile.substr(0, nodefile.rfind("."));
    string filepath = "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile;

    saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate);

    if (nemesis::iequals("#" + filecheck, nodefile))
    {
        vector<int> modEditCoordinate;
        unordered_map<string, string> modPath;
        unordered_map<string, VecStr> modEdits;
        unordered_map<string, int> modEditLine;
        unordered_map<int, int> NewCoordinate;
        unordered_map<int, int> Pair;

        int curline      = 0;
        int editline     = 0;
        int originalline = 0;
        int linecount    = 0;
        int coordinate   = 0;

        bool edited       = false;
        bool originalopen = false;
        bool bigger       = false;
        bool isSM         = false;

        size_t startoriline = 0;
        size_t starteditline = 0;
        int variablecount;
        int eventcount;
        int attributecount;
        int characterpropertycount;

        VecNstr storeline;
        FileReader BehaviorFormat(filepath);

        if (!BehaviorFormat.TryGetFile()) ErrorMessage(2000, filepath, BehaviorFormat.ErrorMessage());

        nemesis::Line rline;
        bool IsEventVariable = false;

        while (BehaviorFormat.TryGetLines(rline))
        {
            string line = rline;

            if (error) throw nemesis::exception();

            if (line.find("hkbBehaviorGraphStringData") != NOT_FOUND
                || line.find("hkbVariableValueSet") != NOT_FOUND
                || line.find("hkbBehaviorGraphData") != NOT_FOUND)
            {
                IsEventVariable = true;
            }

            if (line.find("<hkobject name=\"") != NOT_FOUND
                && line.find("class=\"hkbStateMachine\" signature=\"", line.find("<hkobject name=\""))
                       != NOT_FOUND)
            {
                isSM = true;
            }
            else if (isSM && !originalopen && line.find("\t\t\t#") != NOT_FOUND)
            {
                VecStr curElements;
                StringSplit(line, curElements);

                if (isSM)
                {
                    for (auto& element : curElements)
                    {
#if MULTITHREADED_UPDATE
                        Lockless lock(parentLock);
#endif
                        (*statelist)[nodeID].push_back(element);
                        (*parent)[element] = nodeID;
                    }
                }
            }
            else if (!originalopen && line.find("<hkparam name=\"stateId\">") != NOT_FOUND)
            {
                constexpr std::string_view openparam = "<hkparam name=\"stateId\">";
                constexpr std::string_view closeparam = "</hkparam>";
                string stateIDStr = std::string(nemesis::between(line, openparam, closeparam));

                if (isOnlyNumber(stateIDStr))
                {
#if MULTITHREADED_UPDATE
                    Lockless lock(stateLock);
#endif
                    (*stateID)[nodeID] = stateIDStr;
                }
            }

            if (line.find(ns::ModCode(modcode)) != NOT_FOUND && !edited)
            {
                edited        = true;
                editline      = linecount;
                starteditline = linecount;
            }
            else if (line.find(ns::ModOriginal()) != NOT_FOUND)
            {
                edited       = false;
                originalopen = true;
                originalline = linecount;
                startoriline = linecount;
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
            {
                edited = false;

                if (originalopen)
                {
                    int addLineCount = editline - originalline;

                    if (addLineCount > 0)
                    {
                        NewCoordinate[linecount]                     = addLineCount;
                        modEditLine[std::to_string(linecount) + "R"] = Pair[originalline];
                    }

                    for (int i = startoriline; i < originalline; ++i)
                    {
                        modEditCoordinate.push_back(i);
                        modEditLine[std::to_string(i)] = Pair[i];
                    }

                    originalopen = false;
                }
                else
                {
                    int addLineCount                             = editline - starteditline;
                    NewCoordinate[linecount]                     = addLineCount;
                    modEditLine[std::to_string(linecount) + "R"] = coordinate - addLineCount;
                }
            }
            else if (edited)
            {
                if (IsEventVariable && line.find("numelements=") != NOT_FOUND)
                {
                    if (line.find("<hkparam name=\"eventNames\" numelements=") != NOT_FOUND
                        || line.find("<hkparam name=\"eventInfos\" numelements=") != NOT_FOUND)
                    {
                        eventcount = stoi(nemesis::regex_replace(
                            string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));
                    }
                    else if (line.find("<hkparam name=\"attributeNames\" numelements=") != NOT_FOUND
                             || line.find("<hkparam name=\"attributeDefaults\" numelements=") != NOT_FOUND)
                    {
                        attributecount = stoi(nemesis::regex_replace(
                            string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));
                    }
                    else if (line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND
                             || line.find("<hkparam name=\"wordVariableValues\" numelements=") != NOT_FOUND
                             || line.find("<hkparam name=\"variableInfos\" numelements=") != NOT_FOUND)
                    {
                        variablecount = stoi(nemesis::regex_replace(
                            string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));
                    }
                    else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=")
                                 != NOT_FOUND
                             || line.find("<hkparam name=\"characterPropertyInfos\" numelements=")
                                    != NOT_FOUND)
                    {
                        characterpropertycount = stoi(nemesis::regex_replace(
                            string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));
                    }
                }

                Pair[editline] = coordinate;
                ++editline;
            }
            else if (!edited)
            {
                ++linecount;
                ++originalline;
            }

            if (error) throw nemesis::exception();

            storeline.push_back(line);
            coordinate++;
        }

        if (modEditLine.empty())
        {
            WarningMessage(1017, filepath);
            interMsg(warningMsges.back());
            return false;
        }

#if MULTITHREADED_UPDATE
        Lockless prelock(filelock);
        VecStr newline = (*newFile)[nodeID];
        prelock.Unlock();
#else
        VecNstr newline = (*newFile)[nodeID];
#endif
        VecNstr functionline;
        functionline.reserve(newline.size());
        linecount     = 0;
        int editcount = 0;
        bool skip     = false;

        if (newline.empty()) ErrorMessage(2001, nodeID);

        for (auto& counter : NewCoordinate)
        {
            if (counter.first >= int(newline.size()))
            {
                bigger = true;
                break;
            }
        }

        for (string& line : newline)
        {
            if (error) throw nemesis::exception();

            if (line.find(ns::ModCode()) != NOT_FOUND) skip = true;

            if (line.find(ns::Aster()) == NOT_FOUND && !skip)
            {
                if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
                {
                    if (line.find("<hkparam name=\"eventNames\" numelements=") != NOT_FOUND
                        || line.find("<hkparam name=\"eventInfos\" numelements=") != NOT_FOUND)
                    {
                        int tempint = eventcount
                                      - stoi(nemesis::regex_replace(
                                          string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));

                        if (line.find("<!-- EVENT numelement ") != NOT_FOUND)
                        {
                            line.append(" <!-- EVENT numelement " + modcode + " +" + std::to_string(tempint)
                                        + " $" + std::to_string(modEditLine[std::to_string(linecount)])
                                        + " -->");
                        }
                        else
                        {
                            line.append("   				<!-- EVENT numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                    }
                    else if (line.find("<hkparam name=\"attributeNames\" numelements=") != NOT_FOUND)
                    {
                        int tempint = attributecount
                                      - stoi(nemesis::regex_replace(
                                          string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));

                        if (line.find("<!-- ATTRIBUTE numelement ") != NOT_FOUND)
                        {
                            line.append(" <!-- ATTRIBUTE numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                        else
                        {
                            line.append("   				<!-- ATTRIBUTE numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                    }
                    else if (line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND
                             || line.find("<hkparam name=\"wordVariableValues\" numelements=") != NOT_FOUND
                             || line.find("<hkparam name=\"variableInfos\" numelements=") != NOT_FOUND)
                    {
                        int tempint = variablecount
                                      - stoi(nemesis::regex_replace(
                                          string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));

                        if (line.find("<!-- VARIABLE numelement ") != NOT_FOUND)
                        {
                            line.append(" <!-- VARIABLE numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                        else
                        {
                            line.append("				<!-- VARIABLE numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                    }
                    else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=")
                                 != NOT_FOUND
                             || line.find("<hkparam name=\"characterPropertyInfos\" numelements=")
                                    != NOT_FOUND)
                    {
                        int tempint = characterpropertycount
                                      - stoi(nemesis::regex_replace(
                                          string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));

                        if (line.find("<!-- CHARACTER numelement ") != NOT_FOUND)
                        {
                            line.append(" <!-- CHARACTER numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                        else
                        {
                            line.append("				<!-- CHARACTER numelement " + modcode + " +"
                                        + std::to_string(tempint) + " $"
                                        + std::to_string(modEditLine[std::to_string(linecount)]) + " -->");
                        }
                    }
                    else if (line.find("numelements=\"") != NOT_FOUND)
                    {
                        nemesis::Line templine;

                        if (!GetFunctionEdits(templine, storeline, modEditLine[std::to_string(linecount)]))
                        {
                            ErrorMessage(2005, filepath, modEditLine[std::to_string(linecount)]);
                        }

                        int difference
                            = stoi(nemesis::regex_replace(
                                  templine.ToString(), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")))
                              - stoi(nemesis::regex_replace(
                                  string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("$1")));

                        if (line.find("<!-- numelement *") != NOT_FOUND)
                            line.append(" <!-- numelement *" + modcode + "* +" + std::to_string(difference)
                                        + "-->");
                        else
                            line.append("					<!-- numelement *" + modcode + "* +"
                                        + std::to_string(difference) + "-->");
                    }
                    else
                    {
                        nemesis::Line templine;

                        if (!GetFunctionEdits(templine, storeline, modEditLine[std::to_string(linecount)]))
                        {
                            ErrorMessage(2005, filepath, modEditLine[std::to_string(linecount)]);
                        }

                        if (templine.find("			<hkobject>") != NOT_FOUND
                            || templine.find("			</hkobject>") != NOT_FOUND
                            || templine.find("			<hkparam>") != NOT_FOUND
                            || templine.find("			<hkparam>") != NOT_FOUND)
                        {
                            WarningMessage(1022, filepath, modEditLine[std::to_string(linecount)]);
                            interMsg(warningMsges.back());
                        }

                        templine.append(ns::Spaces().append(ns::Aster(modcode)));
                        functionline.push_back(templine);

                        if (line.find(ns::LowerOriginal()) == NOT_FOUND)
                        {
                            line.append(ns::Spaces().append(ns::LowerOriginal()));
                        }
                    }

                    if (editcount != modEditCoordinate.size() - 1) editcount++;
                }
                else if (NewCoordinate[linecount] > 0)
                {
                    functionline.push_back(ns::ModCode(modcode));

                    VecNstr storage = GetFunctionEdits(filepath,
                                                      storeline,
                                                      modEditLine[std::to_string(linecount) + "R"],
                                                      NewCoordinate[linecount]);

                    functionline.insert(functionline.end(), storage.begin(), storage.end());
                    functionline.push_back(ns::Close());
                }

                ++linecount;
            }

            if (line.find(ns::Close()) != NOT_FOUND) skip = false;

            if (error) throw nemesis::exception();

            functionline.push_back(line);
        }

        if (bigger)
        {
            for (size_t i = newline.size(); i < storeline.size(); ++i)
            {
                if (NewCoordinate[linecount] > 0)
                {
                    functionline.push_back(ns::ModCode(modcode));

                    VecNstr storage = GetFunctionEdits(filepath,
                                                      storeline,
                                                      modEditLine[std::to_string(linecount) + "R"],
                                                      NewCoordinate[linecount]);
                    functionline.insert(functionline.end(), storage.begin(), storage.end());
                    functionline.push_back(ns::Close());
                }

                ++linecount;
            }
        }

        functionline.shrink_to_fit();
#if MULTITHREADED_UPDATE
        Lockless lock(filelock);
#endif
        (*newFile)[nodeID] = functionline;
    }
    else if (nemesis::iequals(nodefile, "#" + modcode + "$" + filecheck))
    {
        VecNstr storeline;

        if (!GetFileLines(filepath, storeline)) return false;

        for (auto& line : storeline)
        {
            constexpr std::string_view openparam  = "<hkparam name=\"stateId\">";

            if (line.find(openparam) == NOT_FOUND) continue;

            constexpr std::string_view closeparam = "</hkparam>";
            string stateIDStr = std::string(nemesis::between(line, openparam, closeparam));

            if (!isOnlyNumber(stateIDStr)) continue;

#if MULTITHREADED_UPDATE
            Lockless lock(stateLock);
#endif
            (*stateID)[nodeID] = stateIDStr;
        }

#if MULTITHREADED_UPDATE
        Lockless lock(filelock);
#endif
        (*newFile)[nodeID] = storeline;
    }
    else if (nodefile != "__folder_managed_by_vortex")
    {
        ErrorMessage(2003, filepath);
    }

    return true;
}

bool NodeU::FunctionUpdate(string modcode,
                           string behaviorfile,
                           string nodefile,
                           unique_ptr<map<string, VecNstr, alphanum_less>>& newFile,
                           unique_ptr<UMapStr2>& stateID,
                           unique_ptr<UMapStr2>& parent,
                           unique_ptr<unordered_map<string, VecStr>>& statelist,
                           unordered_map<wstring, wstring>& lastUpdate
#if MULTITHREADED_UPDATE
                           ,
                           atomic_flag& filelock,
                           atomic_flag& stateLock,
                           atomic_flag& parentLock
#endif
)
{
    bool result = false;

    try
    {
        result = NodeUpdate(modcode,
                            behaviorfile,
                            nodefile,
                            newFile,
                            stateID,
                            parent,
                            statelist,
                            lastUpdate
#if MULTITHREADED_UPDATE
                            ,
                            filelock,
                            stateLock,
                            parentLock
#endif
        );
    }
    catch (nemesis::exception&)
    {
        throw nemesis::exception();
    }
    catch (const std::exception& ex)
    {
        throw ex;
    }

    return result;
}

bool AnimDataUpdate(string modcode,
                    string animdatafile,
                    string projectfile,
                    string filepath,
                    MasterAnimData& animData,
                    bool isNewProject,
                    unordered_map<wstring, wstring>& lastUpdate)
{
    if (behaviorPath[nemesis::transform_to<wstring>(nemesis::to_lower_copy(animdatafile))].empty())
    {
        ErrorMessage(2007, animdatafile);
    }

    VecNstr storeline;
    string filename = GetFileName(filepath);

    saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate);

    if (isNewProject)
    {
        if (!GetFileLines(filepath, storeline, !nemesis::iequals(filename, "$header$"))) return false;

        // must not replace storeline with animData.newAnimData[projectfile][filename] for the total line will not get counted
        if (nemesis::iequals(filename, "$header$"))
        {
            auto& proj = animData.projectlist[animData.getIndex(projectfile)];

            for (auto& cond : proj.nestedcond)
            {
                if (cond.conditions != modcode) continue;

                if (cond.rawlist[0].raw->second) continue;

                storeline.insert(storeline.begin(), "");
                cond.rawlist[0].raw->second = make_shared<AnimDataProject_Condt>(storeline);
                return true;
            }

            string fullpath = filesystem::path(filepath).parent_path().parent_path().string() + "\\$header$\\$header$.txt";

            if (!isFileExist(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

            animData.projectListUpdate(modcode, fullpath, false);

            for (auto& cond : animData.projectlist[animData.getIndex(projectfile)].nestedcond)
            {
                if (cond.conditions != modcode) continue;

                for (auto& each : cond.rawlist)
                {
                    if (!each.raw) continue;

                    storeline.insert(storeline.begin(), "");
                    each.raw->second = make_shared<AnimDataProject_Condt>(storeline);
                    return true;
                }
            }

            ErrorMessage(2025);
        }

        MasterAnimData::ProjectPtr projptr;

        for (auto& cond : animData.projectlist[animData.getIndex(projectfile)].nestedcond)
        {
            if (cond.conditions != modcode) continue;

            for (auto& each : cond.rawlist)
            {
                if (!each.raw) continue;

                projptr = each.raw->second;
                break;
            }

            if (projptr) break;
        }

        if (!projptr) ErrorMessage(3021, projectfile);

        if (isOnlyNumber(filename)) // info data
        {
            projptr->iadd(filename, "original", storeline, 1);
        }
        else if (filename.find("~") != NOT_FOUND) // anim data
        {
            string tempname
                = nemesis::regex_replace(string(filename), nemesis::regex("[^~]*~([0-9]+)"), string("$1"));

            if (tempname == filename) ErrorMessage(2004, filepath);

            projptr->aadd(filename, "original", storeline, 1);
        }
        else
        {
            ErrorMessage(2004, filepath);
        }

        return true;
    }

    if (nemesis::iequals(projectfile, "$header$")) // header file
    {
        animData.projectListUpdate(modcode, filepath);
        return true;
    }

    SPtr<AnimDataProject_Condt> projData = animData.find(projectfile);

    if (projData == nullptr)
    {
        interMsg("ERROR missing project file. File: " + projectfile);
        error = true;
        throw nemesis::exception();
    }

    VecNstr templines;

    if (!GetFileLines(filepath, templines)) return false;

    if (nemesis::iequals(filename, "$header$")) // project header
    {
        projData->modify(modcode, templines);
        return true;
    }

    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;

    storeline.reserve(templines.size());

    for (string& line : templines)
    {
        if (line.find("<!--") != NOT_FOUND)
        {
            if (!edited && line.find(ns::ModCode(modcode)) != NOT_FOUND)
            {
                edited = true;
            }
            else if (line.find(ns::ModOriginal()) != NOT_FOUND)
            {
                originalopen = true;
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
            {
                originalopen = false;
                edited       = false;
            }

            continue;
        }

        if (!originalopen) storeline.push_back(line);
    }

    if (filename.find(modcode + "$") == NOT_FOUND)
    {
        // anim data
        if (!isOnlyNumber(filename))
        {
            // anim data
            if (!hasAlpha(storeline[0])) ErrorMessage(3006, projectfile, filename);

            string check
                = nemesis::regex_replace(string(filename), nemesis::regex("^([^~]+)~[0-9]+$"), string("$1"));

            auto* curptr = projData->afindlist(check);

            if (check == filename || projData->ifindlist(filename.substr(check.length() + 1)) == nullptr)
            {
                ErrorMessage(2004, filepath);
            }

            if (!curptr)
            {
                auto pair = make_pair(std::string(storeline[0]),
                                      nemesis::LinkedVar(AnimDataPack_Condt(storeline)));
                auto condpair = nemesis::CondVar(pair, modcode, nemesis::CondType::MOD_CODE);
                projData->animdatalist.push_back(condpair);
                return true;
            }

            curptr->addCond(nemesis::LinkedVar(AnimDataPack_Condt(storeline)), modcode, nemesis::CondType::MOD_CODE);
        }
        else
        {
            // info data confirmation
            if (!isOnlyNumber(storeline[0].ToString())) ErrorMessage(3006, projectfile, filename);

            auto* curptr = projData->ifindlist(filename);

            if (curptr == nullptr) ErrorMessage(2004, filepath);

            curptr->addCond(nemesis::LinkedVar(InfoDataPack_Condt(storeline)), modcode, nemesis::CondType::MOD_CODE);
        }
    }
    else
    {
        string tempID = nemesis::regex_replace(
            string(filename), nemesis::regex("[^~]*~" + modcode + "[$]([0-9]+)"), string("$1"));

        if (filename != tempID && isOnlyNumber(tempID)) // anim data
        {
            addAnimDataPack(projectfile, filename, modcode, projData, storeline);
        }
        else
        {
            addInfoDataPack(filepath, filename, modcode, projData, storeline);
        }
    }

    return true;
}

void addAnimDataPack(const string& projectfile,
                     const string& filename,
                     const string& modcode,
                     const MasterAnimData::ProjectPtr& projData,
                     const VecNstr& storeline)
{
    // anim data
    if (!hasAlpha(storeline[0])) ErrorMessage(3006, projectfile, filename);

    nemesis::LinkedVar<AnimDataPack_Condt>* animDataPtr = projData->afindlist(filename);

    // new anim data
    if (animDataPtr == nullptr)
    {
        auto pair = make_pair(std::string(storeline[0]), nemesis::LinkedVar(AnimDataPack_Condt(storeline)));
        auto condpair = nemesis::CondVar(pair, modcode, nemesis::CondType::MOD_CODE);
        projData->animdatalist.push_back(condpair);
    }
    // existing anim data
    else
    {
        auto shptr = nemesis::CondVar(AnimDataPack_Condt(storeline));
        //(*animDataPtr).addCond(make_shared<AnimDataPack_Condt>(storeline), modcode, nemesis::CondType::MOD_CODE); does the same thing as line below
        (*animDataPtr).nestedcond.push_back(shptr);
    }
}

void addInfoDataPack(const string& filepath,
                     const string& filename,
                     const string& modcode,
                     const MasterAnimData::ProjectPtr& projData,
                     const VecNstr& storeline)
{
    string tempID
        = nemesis::regex_replace(string(filename), nemesis::regex(modcode + "[$]([0-9]+)"), string("$1"));

    // info data
    if (filename == tempID || !isOnlyNumber(tempID)) ErrorMessage(2004, filepath);

    nemesis::LinkedVar<InfoDataPack_Condt>* infoDataPtr = projData->ifindlist(filename);

    // new info data
    if (infoDataPtr == nullptr)
    {
        auto pair = make_pair(std::string(storeline[0]), nemesis::LinkedVar(InfoDataPack_Condt(storeline)));
        auto condpair = nemesis::CondVar(pair, modcode, nemesis::CondType::MOD_CODE);
        projData->infodatalist.push_back(condpair);
    }
    // existing info data
    else
    {
        auto shptr = nemesis::CondVar(InfoDataPack_Condt(storeline), modcode, nemesis::CondType::MOD_CODE);
        //(*infoDataPtr).addCond(make_shared<InfoDataPack_Condt>(storeline), modcode, nemesis::CondType::MOD_CODE); does the same thing as line below
        (*infoDataPtr).nestedcond.push_back(shptr);
    }
}

bool AnimSetDataUpdate(string modcode,
                       string animdatasetfile,
                       string projectsource,
                       string projectfile,
                       filesystem::path filepath,
                       MasterAnimSetData& animSetData,
                       bool isNewProject,
                       unordered_map<wstring, wstring>& lastUpdate)
{
    if (behaviorPath[nemesis::transform_to<wstring>(nemesis::to_lower_copy(animdatasetfile))].empty())
    {
        ErrorMessage(2007, animdatasetfile);
    }

    string filename  = filepath.stem().string();
    string lowerfile = nemesis::to_lower_copy(filename);

    saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate);

    VecNstr storeline;

    if (!GetFileLines(filepath, storeline, false)) return false;

    #if REMOVE
    if (isNewProject)
    {
        animSetData.newAnimSetData[projectfile][lowerfile] = storeline;
    }
    else
    {
        vector<int> modEditCoordinate;
        unordered_map<string, int> modEditLine;
        unordered_map<int, int> NewCoordinate;
        unordered_map<int, int> Pair;

        bool edited       = false;
        bool originalopen = false;

        int coordinate    = 0;
        int startoriline  = 0;
        int starteditline = 0;
        int editline      = 0;
        int originalline  = 0;
        int linecount     = 0;

        for (string& line : storeline)
        {
            if (error) throw nemesis::exception();

            if (line.find(ns::ModCode(modcode)) != NOT_FOUND && !edited)
            {
                edited        = true;
                editline      = linecount;
                starteditline = linecount;
            }
            else if (line.find(ns::ModOriginal()) != NOT_FOUND)
            {
                edited       = false;
                originalopen = true;
                originalline = linecount;
                startoriline = linecount;
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
            {
                edited = false;

                if (originalopen)
                {
                    int tempint                                  = editline - originalline;
                    NewCoordinate[linecount]                     = tempint;
                    modEditLine[std::to_string(linecount) + "R"] = coordinate - tempint - 2;

                    for (size_t i = startoriline; i < originalline; ++i)
                    {
                        modEditCoordinate.push_back(i);
                        modEditLine[std::to_string(i)] = Pair[i];
                    }

                    originalopen = false;
                }
                else
                {
                    int tempint                                  = editline - starteditline;
                    NewCoordinate[linecount]                     = tempint;
                    modEditLine[std::to_string(linecount) + "R"] = coordinate - tempint;
                }
            }
            else if (edited)
            {
                Pair[editline] = coordinate;
                ++editline;
            }
            else if (!edited)
            {
                ++linecount;
                ++originalline;
            }

            coordinate++;
        }

        if (error) throw nemesis::exception();

        if (animSetData.newAnimSetData[projectfile].find(lowerfile)
            != animSetData.newAnimSetData[projectfile].end())
        {
            if (NewCoordinate.size() == 0)
            {
                WarningMessage(1017, filepath);
                interMsg(warningMsges.back());
                return false;
            }

            VecStr newline = animSetData.newAnimSetData[projectfile][lowerfile];
            VecStr functionline;
            VecStr headerline;

            int editcount = 0;

            bool skip     = false;
            bool isHeader = false;

            functionline.reserve(newline.size());
            linecount = 0;

            if (nemesis::iequals(projectfile, "$header$"))
            {
                isHeader = true;
            }

            newline.push_back("12121332221223212");

            for (string& line : newline)
            {
                if (line.find("<!-- NEW") != NOT_FOUND || line.find("<!-- FOREACH") != NOT_FOUND)
                {
                    skip = true;
                }

                if (line.find("<!-- *") == NOT_FOUND && !skip)
                {
                    if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
                    {
                        string templine;

                        if (!GetFunctionEdits(templine, storeline, modEditLine[std::to_string(linecount)]))
                        {
                            ErrorMessage(2005, filepath, modEditLine[std::to_string(linecount)]);
                        }

                        using namespace ASDFormat;
                        position curPosition = ASDPosition(storeline,
                                                           projectfile,
                                                           filename,
                                                           modcode,
                                                           modEditLine[std::to_string(linecount)],
                                                           false);

                        if (curPosition != equipcount && curPosition != typecount
                            && curPosition != animpackcount && curPosition != attackcount
                            && curPosition != crc32count)
                        {
                            templine.append("\t\t\t\t\t<!-- *" + modcode + "* -->");
                            functionline.push_back(templine);

                            if (line.find("\t<!-- original -->") == NOT_FOUND)
                            {
                                line.append("\t\t\t\t\t<!-- original -->");
                            }

                            if (editcount != modEditCoordinate.size() - 1) editcount++;
                        }
                    }
                    else if (NewCoordinate[linecount] > 0)
                    {
                        if (isHeader)
                        {
                            VecStr storage = GetFunctionEdits(filepath,
                                                              storeline,
                                                              modEditLine[std::to_string(linecount) + "R"],
                                                              NewCoordinate[linecount]);

                            if (!error)
                            {
                                headerline.push_back(ns::ModCode(modcode));
                                headerline.insert(headerline.end(), storage.begin(), storage.end());
                                headerline.push_back(ns::Close());
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            if (!ClassCheck(modEditCoordinate,
                                            modEditLine,
                                            NewCoordinate,
                                            linecount,
                                            storeline,
                                            filepath,
                                            projectfile,
                                            filename,
                                            modcode))
                                return false;

                            functionline.push_back(ns::ModCode(modcode));
                            VecStr storage = GetFunctionEdits(filepath,
                                                              storeline,
                                                              modEditLine[std::to_string(linecount) + "R"],
                                                              NewCoordinate[linecount]);

                            if (!error)
                            {
                                functionline.insert(functionline.end(), storage.begin(), storage.end());
                                functionline.push_back(ns::Close());
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }

                    ++linecount;
                }

                if (line.find(ns::Close()) != NOT_FOUND) skip = false;

                functionline.push_back(line);
            }

            functionline.pop_back();

            if (isHeader && headerline.size() > 0)
                functionline.insert(functionline.end(), headerline.begin(), headerline.end());

            functionline.shrink_to_fit();
            animSetData.newAnimSetData[projectfile][lowerfile] = functionline;
        }
        else
        {
            size_t pos = lowerfile.find(modcode + "$");

            if (pos == 0)
            {
                string lowerheader = lowerfile.substr(modcode.length() + 1);

                if (animSetData.newAnimSetData[projectfile].find(lowerheader)
                    != animSetData.newAnimSetData[projectfile].end())
                {
                    ErrorMessage(5022, projectfile, lowerheader);
                }

                storeline.insert(storeline.begin(), ns::ModCode(modcode));
                storeline.push_back(ns::Close());
                animSetData.newAnimSetData[projectfile][lowerheader] = storeline;
            }
        }
    }
    #else
    if (error) throw nemesis::exception();

    size_t num = 2;
    string lowername = nemesis::to_lower_copy(filepath.filename().string());
    auto masterProj = animSetData.find(projectfile, modcode);
    size_t pos = lowername.find(nemesis::to_lower_copy(modcode) + "$");

    if (pos == 0)
    {
        masterProj->find(lowername.substr(pos + modcode.length() + 1), modcode)
            ->ImportData(filepath, num, storeline, modcode);
    }
    else
    {
        masterProj->find(lowername)->ImportData(filepath, num, storeline, modcode);
    }
    #endif

    return true;
}

bool ClassCheck(vector<int>& modEditCoordinate,
                unordered_map<string, int>& modEditLine,
                unordered_map<int, int>& NewCoordinate,
                int linecount,
                VecStr& storeline,
                string filepath,
                string projectfile,
                string filename,
                string modcode)
{
    size_t endline = modEditLine[std::to_string(linecount) + "R"] + NewCoordinate[linecount];
    bool attacknew       = false;
    bool islast          = false;

    if (ASDFormat::animpackname
        == ASDPosition(
            storeline, projectfile, filename, modcode, modEditLine[std::to_string(linecount) + "R"], true))
    {
        attacknew = true;
    }

    for (size_t k = modEditLine[std::to_string(linecount) + "R"]; k < endline; ++k)
    {
        if (error) throw nemesis::exception();

        using namespace ASDFormat;

        position curPosition;

        if (!islast)
        {
            curPosition = ASDPosition(storeline, projectfile, filename, modcode, k, false);

            if (error) throw nemesis::exception();

            if (curPosition == crc32list) islast = true;
        }
        else
        {
            curPosition = crc32list;
        }

        if (curPosition != equiplist && curPosition != typelist && curPosition != attacklist
            && curPosition != crc32list)
        {
            if (attacknew)
            {
                if ((curPosition != animpackname && curPosition != unknown3 && curPosition != attackcount))
                {
                    ErrorMessage(5021, modcode, filepath, k + 1);
                }
            }
            else
            {
                ErrorMessage(5021, modcode, filepath, k + 1);
            }
        }
    }

    return true;
}
