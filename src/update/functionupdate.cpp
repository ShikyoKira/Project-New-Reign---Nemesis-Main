#include "Global.h"

#include "debugmsg.h"

#include "utilities/lastupdate.h"
#include "utilities/readtextfile.h"
#include "utilities/stringsplit.h"

#include "update/functionupdate.h"

#pragma warning(disable : 4503)

using namespace std;

typedef vector<string> vecstr;

bool CombineAnimData(string filepath,
                     string filename,
                     string characterfile,
                     string modcode,
                     vecstr& newline,
                     vecstr& storeline,
                     vector<int> modEditCoordinate,
                     unordered_map<string, int> modEditLine,
                     unordered_map<int, int> NewCoordinate,
                     unordered_map<int, int> Pair,
                     unordered_map<string, string>& lastUpdate);
bool ClassCheck(vector<int>& modEditCoordinate,
                unordered_map<string, int>& modEditLine,
                unordered_map<int, int>& NewCoordinate,
                int linecount,
                vecstr& storeline,
                string filepath,
                string projectfile,
                string filename,
                string modcode);

bool GetFunctionEdits(string& line, vecstr storeline, int numline)
{
    if (numline < int(storeline.size()))
    {
        line = storeline[numline];
        return true;
    }

    error = true;
    return false;
}

vecstr GetFunctionEdits(string filename, vecstr storeline, int startline, int endline)
{
    vecstr storage;
    storage.reserve(endline);
    int combine = endline + startline;

    if (int(storeline.size()) > combine)
    {
        for (int i = startline; i < combine; ++i)
        {
            storage.push_back(storeline[i]);
        }
    }
    else
    {
        ErrorMessage(2002, filename, startline, startline + endline);
    }

    return storage;
}

bool NodeU::NodeUpdate(string modcode,
                       string behaviorfile,
                       string nodefile,
                       unique_ptr<map<string, vecstr, alphanum_less>>& newFile,
                       unique_ptr<SSMap>& stateID,
                       unique_ptr<SSMap>& parent,
                       unique_ptr<unordered_map<string, vecstr>>& statelist,
                       unordered_map<string, string>& lastUpdate
#if MULTITHREADED_UPDATE
                       ,
                       boost::atomic_flag& filelock,
                       boost::atomic_flag& stateLock,
                       boost::atomic_flag& parentLock
#endif
)
{
    if (behaviorPath[behaviorfile].empty()) ErrorMessage(2006, behaviorfile);

    string filecheck = boost::regex_replace(
                           string(nodefile), boost::regex(".+?([0-9]+)\\.[t|T][x|X][t|T]$"), string("\\1"))
                       + ".txt";
    string nodeID   = nodefile.substr(0, nodefile.find_last_of("."));
    string filename = "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile;

    if (!saveLastUpdate(nemesis::to_lower_copy(filename), lastUpdate)) return false;

    if (nemesis::iequals("#" + filecheck, nodefile))
    {
        vector<int> modEditCoordinate;
        unordered_map<string, string> modPath;
        unordered_map<string, vecstr> modEdits;
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

        int startoriline  = 0;
        int starteditline = 0;
        int variablecount;
        int eventcount;
        int attributecount;
        int characterpropertycount;

        vecstr storeline;
        FileReader BehaviorFormat(filename);

        if (BehaviorFormat.GetFile())
        {
            string rline;
            bool IsEventVariable = false;

            while (BehaviorFormat.GetLines(rline))
            {
                string line = rline;

                if (error) throw nemesis::exception();

                if (line.find("hkbBehaviorGraphStringData", 0) != NOT_FOUND
                    || line.find("hkbVariableValueSet", 0) != NOT_FOUND
                    || line.find("hkbBehaviorGraphData", 0) != NOT_FOUND)
                { IsEventVariable = true; }

                if (line.find("<hkobject name=\"", 0) != NOT_FOUND
                    && line.find("class=\"hkbStateMachine\" signature=\"", line.find("<hkobject name=\""))
                           != NOT_FOUND)
                { isSM = true; }
                else if (isSM && !originalopen && line.find("			#") != NOT_FOUND)
                {
                    vecstr curElements;
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
                    string stateIDStr = boost::regex_replace(
                        string(line),
                        boost::regex(".*<hkparam name=\"stateId\">([0-9]+)</hkparam>.*"),
                        string("\\1"));

                    if (stateIDStr != line)
                    {
#if MULTITHREADED_UPDATE
                        Lockless lock(stateLock);
#endif
                        (*stateID)[nodeID] = stateIDStr;
                    }
                }

                if ((line.find("<!-- MOD_CODE", 0) != NOT_FOUND) && (line.find("OPEN -->", 0) != NOT_FOUND)
                    && (!edited))
                {
                    edited        = true;
                    editline      = linecount;
                    starteditline = linecount;
                }
                else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
                {
                    edited       = false;
                    originalopen = true;
                    originalline = linecount;
                    startoriline = linecount;
                }
                else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
                {
                    edited = false;

                    if (originalopen)
                    {
                        int addLineCount = editline - originalline;

                        if (addLineCount > 0)
                        {
                            NewCoordinate[linecount]                = addLineCount;
                            modEditLine[to_string(linecount) + "R"] = Pair[originalline];
                        }

                        for (int i = startoriline; i < originalline; ++i)
                        {
                            modEditCoordinate.push_back(i);
                            modEditLine[to_string(i)] = Pair[i];
                        }

                        originalopen = false;
                    }
                    else
                    {
                        int addLineCount                        = editline - starteditline;
                        NewCoordinate[linecount]                = addLineCount;
                        modEditLine[to_string(linecount) + "R"] = coordinate - addLineCount;
                    }
                }
                else if (edited)
                {
                    if (IsEventVariable && line.find("numelements=", 0) != NOT_FOUND)
                    {
                        if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND
                            || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != NOT_FOUND)
                        {
                            eventcount = stoi(boost::regex_replace(
                                string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
                        }
                        else if (line.find("<hkparam name=\"attributeNames\" numelements=", 0) != NOT_FOUND
                                 || line.find("<hkparam name=\"attributeDefaults\" numelements=", 0)
                                        != NOT_FOUND)
                        {
                            attributecount = stoi(boost::regex_replace(
                                string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
                        }
                        else if (line.find("<hkparam name=\"variableNames\" numelements=", 0) != NOT_FOUND
                                 || line.find("<hkparam name=\"wordVariableValues\" numelements=", 0)
                                        != NOT_FOUND
                                 || line.find("<hkparam name=\"variableInfos\" numelements=", 0) != NOT_FOUND)
                        {
                            variablecount = stoi(boost::regex_replace(
                                string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
                        }
                        else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=", 0)
                                     != NOT_FOUND
                                 || line.find("<hkparam name=\"characterPropertyInfos\" numelements=", 0)
                                        != NOT_FOUND)
                        {
                            characterpropertycount = stoi(boost::regex_replace(
                                string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
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
        }
        else
        {
            ErrorMessage(2000, filename);
        }

        if (modEditLine.size() == 0)
        {
            WarningMessage(1017, filename);
            interMsg(warningMsges.back());
            return false;
        }

#if MULTITHREADED_UPDATE
        Lockless prelock(filelock);
        vecstr newline = (*newFile)[nodeID];
        prelock.Unlock();
#else
        vecstr newline = (*newFile)[nodeID];
#endif
        vecstr functionline;
        functionline.reserve(newline.size());
        linecount     = 0;
        int editcount = 0;
        bool skip     = false;

        if (newline.size() != 0)
        {
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

                if (line.find("<!-- NEW", 0) != NOT_FOUND) skip = true;

                if (line.find("<!-- *", 0) == NOT_FOUND && !skip)
                {
                    if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
                    {
                        if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND
                            || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != NOT_FOUND)
                        {
                            int tempint
                                = eventcount
                                  - stoi(boost::regex_replace(
                                      string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

                            if (line.find("<!-- EVENT numelement ", 0) != NOT_FOUND)
                            {
                                line.append(" <!-- EVENT numelement " + modcode + " +" + to_string(tempint)
                                            + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                            else
                            {
                                line.append("   				<!-- EVENT numelement " + modcode + " +"
                                            + to_string(tempint) + " $"
                                            + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                        }
                        else if (line.find("<hkparam name=\"attributeNames\" numelements=", 0) != NOT_FOUND)
                        {
                            int tempint
                                = attributecount
                                  - stoi(boost::regex_replace(
                                      string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

                            if (line.find("<!-- ATTRIBUTE numelement ", 0) != NOT_FOUND)
                            {
                                line.append(" <!-- ATTRIBUTE numelement " + modcode + " +"
                                            + to_string(tempint) + " $"
                                            + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                            else
                            {
                                line.append("   				<!-- ATTRIBUTE numelement " + modcode + " +"
                                            + to_string(tempint) + " $"
                                            + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                        }
                        else if (line.find("<hkparam name=\"variableNames\" numelements=", 0) != NOT_FOUND
                                 || line.find("<hkparam name=\"wordVariableValues\" numelements=", 0)
                                        != NOT_FOUND
                                 || line.find("<hkparam name=\"variableInfos\" numelements=", 0) != NOT_FOUND)
                        {
                            int tempint
                                = variablecount
                                  - stoi(boost::regex_replace(
                                      string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

                            if (line.find("<!-- VARIABLE numelement ", 0) != NOT_FOUND)
                            {
                                line.append(" <!-- VARIABLE numelement " + modcode + " +" + to_string(tempint)
                                            + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                            else
                            {
                                line.append("				<!-- VARIABLE numelement " + modcode + " +"
                                            + to_string(tempint) + " $"
                                            + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                        }
                        else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=", 0)
                                     != NOT_FOUND
                                 || line.find("<hkparam name=\"characterPropertyInfos\" numelements=", 0)
                                        != NOT_FOUND)
                        {
                            int tempint
                                = characterpropertycount
                                  - stoi(boost::regex_replace(
                                      string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

                            if (line.find("<!-- CHARACTER numelement ", 0) != NOT_FOUND)
                            {
                                line.append(" <!-- CHARACTER numelement " + modcode + " +"
                                            + to_string(tempint) + " $"
                                            + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                            else
                            {
                                line.append("				<!-- CHARACTER numelement " + modcode + " +"
                                            + to_string(tempint) + " $"
                                            + to_string(modEditLine[to_string(linecount)]) + " -->");
                            }
                        }
                        else if (line.find("numelements=\"", 0) != NOT_FOUND)
                        {
                            string templine;

                            if (!GetFunctionEdits(templine, storeline, modEditLine[to_string(linecount)]))
                            {
                                ErrorMessage(2005,
                                             "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile,
                                             modEditLine[to_string(linecount)]);
                            }

                            int difference
                                = stoi(boost::regex_replace(
                                      string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")))
                                  - stoi(boost::regex_replace(
                                      string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

                            if (line.find("<!-- numelement *", 0) != NOT_FOUND)
                                line.append(" <!-- numelement *" + modcode + "* +" + to_string(difference)
                                            + "-->");
                            else
                                line.append("					<!-- numelement *" + modcode + "* +"
                                            + to_string(difference) + "-->");
                        }
                        else
                        {
                            string templine;

                            if (!GetFunctionEdits(templine, storeline, modEditLine[to_string(linecount)]))
                            {
                                ErrorMessage(2005,
                                             "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile,
                                             modEditLine[to_string(linecount)]);
                            }

                            if (templine.find("			<hkobject>") != NOT_FOUND
                                || templine.find("			</hkobject>") != NOT_FOUND
                                || templine.find("			<hkparam>") != NOT_FOUND
                                || templine.find("			<hkparam>") != NOT_FOUND)
                            {
                                WarningMessage(1022,
                                               "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile,
                                               modEditLine[to_string(linecount)]);
                                interMsg(warningMsges.back());
                            }

                            templine.append("					<!-- *" + modcode + "* -->");
                            functionline.push_back(templine);

                            if (line.find("<!-- original -->", 0) == NOT_FOUND)
                                line.append("				<!-- original -->");
                        }

                        if (editcount != modEditCoordinate.size() - 1) editcount++;
                    }
                    else if (NewCoordinate[linecount] > 0)
                    {
                        functionline.push_back("<!-- NEW *" + modcode + "* -->");

                        vecstr storage
                            = GetFunctionEdits("mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile,
                                               storeline,
                                               modEditLine[to_string(linecount) + "R"],
                                               NewCoordinate[linecount]);

                        functionline.insert(functionline.end(), storage.begin(), storage.end());
                        functionline.push_back("<!-- CLOSE -->");
                    }

                    ++linecount;
                }

                if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND) skip = false;

                if (error) throw nemesis::exception();

                functionline.push_back(line);
            }
        }
        else
        {
            ErrorMessage(2001, nodeID);
        }

        if (bigger)
        {
            for (unsigned int i = newline.size(); i < storeline.size(); ++i)
            {
                if (NewCoordinate[linecount] > 0)
                {
                    functionline.push_back("<!-- NEW *" + modcode + "* -->");

                    vecstr storage
                        = GetFunctionEdits("mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile,
                                           storeline,
                                           modEditLine[to_string(linecount) + "R"],
                                           NewCoordinate[linecount]);
                    functionline.insert(functionline.end(), storage.begin(), storage.end());
                    functionline.push_back("<!-- CLOSE -->");
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
        vecstr storeline;

        if (!GetFunctionLines(filename, storeline)) return false;

        for (auto& line : storeline)
        {
            if (line.find("<hkparam name=\"stateId\">") != NOT_FOUND)
            {
                string stateIDStr
                    = boost::regex_replace(string(line),
                                           boost::regex(".*<hkparam name=\"stateId\">([0-9]+)</hkparam>.*"),
                                           string("\\1"));

                if (stateIDStr != line)
                {
#if MULTITHREADED_UPDATE
                    Lockless lock(stateLock);
#endif
                    (*stateID)[nodeID] = stateIDStr;
                }
            }
        }

#if MULTITHREADED_UPDATE
        Lockless lock(filelock);
#endif
        (*newFile)[nodeID] = storeline;
    }
    else if (nodefile != "__folder_managed_by_vortex")
    {
        ErrorMessage(2003, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile);
    }

    return true;
}

bool NodeU::FunctionUpdate(string modcode,
                           string behaviorfile,
                           string nodefile,
                           unique_ptr<map<string, vecstr, alphanum_less>>& newFile,
                           unique_ptr<SSMap>& stateID,
                           unique_ptr<SSMap>& parent,
                           unique_ptr<unordered_map<string, vecstr>>& statelist,
                           unordered_map<string, string>& lastUpdate
#if MULTITHREADED_UPDATE
                           ,
                           boost::atomic_flag& filelock,
                           boost::atomic_flag& stateLock,
                           boost::atomic_flag& parentLock
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
                    string characterfile,
                    string filepath,
                    MasterAnimData& animData,
                    bool isNewCharacter,
                    unordered_map<string, string>& lastUpdate,
                    bool& openAnim,
                    bool& openInfo)
{
    if (behaviorPath[nemesis::to_lower_copy(animdatafile)].empty()) ErrorMessage(2007, animdatafile);

    string filename = GetFileName(filepath);

    if (!saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate)) return false;

    if (isNewCharacter)
    {
        vecstr storeline;

        if (!GetFunctionLines(filepath, storeline, !nemesis::iequals(filename, "$header$"))) return false;

        // must not replace storeline with animData.newAnimData[characterfile][filename] for the total line will not get counted
        animData.newAnimData[characterfile][filename].insert(
            animData.newAnimData[characterfile][filename].end(), storeline.begin(), storeline.end());

        if (isOnlyNumber(filename)) // info data
        { animData.animDataInfo[characterfile].push_back(filename); }
        else if (filename.find("~") != NOT_FOUND) // anim data
        {
            string tempname
                = boost::regex_replace(string(filename), boost::regex("[^~]*~([0-9]+)"), string("\\1"));

            if (tempname == filename) ErrorMessage(2004, filepath);

            animData.animDataHeader[characterfile].push_back(filename);
        }
        else if (nemesis::iequals(filename, "$header$"))
        {
            animData.animDataHeader[characterfile].push_back(filename);
        }
        else
        {
            ErrorMessage(2004, filepath);
        }
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

        vecstr storeline;

        if (!GetFunctionLines(filepath, storeline)) return false;

        for (string& line : storeline)
        {
            if (error) throw nemesis::exception();

            if ((line.find("<!-- MOD_CODE", 0) != NOT_FOUND) && (line.find("OPEN -->", 0) != NOT_FOUND)
                && (!edited))
            {
                edited        = true;
                editline      = linecount;
                starteditline = linecount;
            }
            else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
            {
                edited       = false;
                originalopen = true;
                originalline = linecount;
                startoriline = linecount;
            }
            else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
            {
                edited = false;

                if (originalopen)
                {
                    int tempint                             = editline - originalline;
                    NewCoordinate[linecount]                = tempint;
                    modEditLine[to_string(linecount) + "R"] = coordinate - tempint - 2;

                    for (int i = startoriline; i < originalline; ++i)
                    {
                        modEditCoordinate.push_back(i);
                        modEditLine[to_string(i)] = Pair[i];
                    }

                    originalopen = false;
                }
                else
                {
                    int tempint                             = editline - starteditline;
                    NewCoordinate[linecount]                = tempint;
                    modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
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

        if (animData.newAnimData[characterfile].find(filename) != animData.newAnimData[characterfile].end())
        {
            if (NewCoordinate.size() == 0)
            {
                WarningMessage(1017, filepath);
                interMsg(warningMsges.back());
                return false;
            }

            CombineAnimData(filepath,
                            filename,
                            characterfile,
                            modcode,
                            animData.newAnimData[characterfile][filename],
                            storeline,
                            modEditCoordinate,
                            modEditLine,
                            NewCoordinate,
                            Pair,
                            lastUpdate);
        }
        else if (filename.find(modcode + "$") != NOT_FOUND)
        {
            string tempID = boost::regex_replace(
                string(filename), boost::regex("[^~]*~" + modcode + "[$]([0-9]+)"), string("\\1"));

            if (filename != tempID && isOnlyNumber(tempID)) // anim data
            {
                if (!GetFunctionLines(filepath, animData.newAnimData[characterfile][filename])) return false;

                if (!openAnim)
                {
                    animData.newAnimData[characterfile][filename].insert(
                        animData.newAnimData[characterfile][filename].begin(),
                        "<!-- NEW *" + modcode + "* -->");
                    openAnim = true;
                }

                animData.animDataHeader[characterfile].push_back(filename);
            }
            else
            {
                tempID = boost::regex_replace(
                    string(filename), boost::regex(modcode + "[$]([0-9]+)"), string("\\1"));

                if (filename != tempID && isOnlyNumber(tempID)) // info data
                {
                    if (!GetFunctionLines(filepath, animData.newAnimData[characterfile][filename]))
                        return false;

                    if (!openInfo)
                    {
                        animData.newAnimData[characterfile][filename].insert(
                            animData.newAnimData[characterfile][filename].begin(),
                            "<!-- NEW *" + modcode + "* -->");
                        openInfo = true;
                    }

                    animData.animDataInfo[characterfile].push_back(filename);
                }
                else
                {
                    ErrorMessage(2004, filepath);
                }
            }
        }
        else
        {
            string check
                = boost::regex_replace(string(filename), boost::regex("[^~]*~([0-9]+)"), string("\\1"));

            if (animData.newAnimData[characterfile].find(check) == animData.newAnimData[characterfile].end())
                ErrorMessage(2004, filepath);

            if (!GetFunctionLines(filepath, animData.newAnimData[characterfile][filename])) return false;

            if (!openAnim)
            {
                animData.newAnimData[characterfile][filename].insert(
                    animData.newAnimData[characterfile][filename].begin(), "<!-- NEW *" + modcode + "* -->");
                openAnim = true;
            }

            animData.animDataHeader[characterfile].push_back(filename);
        }
    }

    return true;
}

bool CombineAnimData(string filepath,
                     string filename,
                     string characterfile,
                     string modcode,
                     vecstr& newline,
                     vecstr& storeline,
                     vector<int> modEditCoordinate,
                     unordered_map<string, int> modEditLine,
                     unordered_map<int, int> NewCoordinate,
                     unordered_map<int, int> Pair,
                     unordered_map<string, string>& lastUpdate)
{
    vecstr functionline;
    vecstr headerline;
    vector<vector<int>> newchecker;

    int linecount = 0;
    int editcount = 0;
    int type;

    bool skip     = false;
    bool isHeader = false;
    bool bigger   = false;

    functionline.reserve(newline.size());

    if (!saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate)) return false;

    if (nemesis::iequals(characterfile, "$header$")) { isHeader = true; }
    else
    {
        if (nemesis::iequals(filename, "$header$"))
            type = 0;
        else if (hasAlpha(storeline[0]))
            type = 1;
        else if (isOnlyNumber(storeline[0]))
            type = 2;
        else
            ErrorMessage(3006, characterfile, filename);
    }

    for (auto& counter : NewCoordinate)
    {
        if (counter.first >= int(newline.size()))
        {
            bigger = true;
            break;
        }
    }

    for (auto line : newline)
    {
        if (error) throw nemesis::exception();

        if (line.find("<!-- NEW", 0) != NOT_FOUND) skip = true;

        if (line.find("<!-- *", 0) == NOT_FOUND && !skip)
        {
            if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
            {
                string templine;

                if (!GetFunctionEdits(templine, storeline, modEditLine[to_string(linecount)]))
                { ErrorMessage(2005, filepath, modEditLine[to_string(linecount)]); }

                if (!isHeader)
                {
                    int position = AnimDataPosition(storeline,
                                                    characterfile,
                                                    filename,
                                                    modcode,
                                                    filepath,
                                                    modEditLine[to_string(linecount)],
                                                    type);

                    if (error) throw nemesis::exception();

                    if (type == 0)
                    {
                        // behavior file count
                        if (position == 3) position = -1;
                    }
                    else if (type == 1)
                    {
                        // event name count
                        if (position == 6) position = -1;
                    }
                    else if (type == 2)
                    {
                        // motion data count or rotation data count
                        if (position == 3 || position == 5) position = -1;
                    }

                    if (position != -1)
                    {
                        templine.append("					<!-- *" + modcode + "* -->");
                        functionline.push_back(templine);

                        if (line.find("<!-- original -->", 0) == NOT_FOUND)
                            line.append("				<!-- original -->");

                        if (editcount != modEditCoordinate.size() - 1) editcount++;
                    }
                }
                else
                {
                    templine.append("					<!-- *" + modcode + "* -->");
                    functionline.push_back(templine);

                    if (line.find("<!-- original -->", 0) == NOT_FOUND)
                        line.append("				<!-- original -->");

                    if (editcount != modEditCoordinate.size() - 1) editcount++;
                }
            }
            else if (NewCoordinate[linecount] > 0)
            {
                if (isHeader)
                {
                    vecstr storage = GetFunctionEdits(filepath,
                                                      storeline,
                                                      modEditLine[to_string(linecount) + "R"],
                                                      NewCoordinate[linecount]);

                    if (!error)
                    {
                        headerline.push_back("<!-- NEW *" + modcode + "* -->");
                        headerline.insert(headerline.end(), storage.begin(), storage.end());
                        headerline.push_back("<!-- CLOSE -->");
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    vector<int> tempVI;
                    functionline.push_back("<!-- NEW *" + modcode + "* -->");
                    tempVI.push_back(functionline.size());
                    vecstr storage = GetFunctionEdits(filepath,
                                                      storeline,
                                                      modEditLine[to_string(linecount) + "R"],
                                                      NewCoordinate[linecount]);

                    if (!error)
                    {
                        functionline.insert(functionline.end(), storage.begin(), storage.end());
                        tempVI.push_back(functionline.size());
                        functionline.push_back("<!-- CLOSE -->");
                    }
                    else
                    {
                        return false;
                    }

                    newchecker.push_back(tempVI);
                }
            }

            ++linecount;
        }

        if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND) skip = false;

        functionline.push_back(line);
    }

    if (bigger)
    {
        for (unsigned int i = newline.size(); i < storeline.size(); ++i)
        {
            if (NewCoordinate[linecount] > 0)
            {
                if (isHeader)
                {
                    vecstr storage = GetFunctionEdits(filepath,
                                                      storeline,
                                                      modEditLine[to_string(linecount) + "R"],
                                                      NewCoordinate[linecount]);

                    if (!error)
                    {
                        headerline.push_back("<!-- NEW *" + modcode + "* -->");
                        headerline.insert(headerline.end(), storage.begin(), storage.end());
                        headerline.push_back("<!-- CLOSE -->");
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    vector<int> tempVI;
                    functionline.push_back("<!-- NEW *" + modcode + "* -->");
                    tempVI.push_back(functionline.size());
                    vecstr storage = GetFunctionEdits(filepath,
                                                      storeline,
                                                      modEditLine[to_string(linecount) + "R"],
                                                      NewCoordinate[linecount]);

                    if (!error)
                    {
                        functionline.insert(functionline.end(), storage.begin(), storage.end());
                        tempVI.push_back(functionline.size());
                        functionline.push_back("<!-- CLOSE -->");
                    }
                    else
                    {
                        return false;
                    }

                    newchecker.push_back(tempVI);
                }
            }

            ++linecount;
        }
    }

    if (isHeader && headerline.size() > 0)
        functionline.insert(functionline.end(), headerline.begin(), headerline.end());

    for (auto& chk : newchecker)
    {
        bool islast = false;
        using namespace AnimDataFormat;

        if (chk.size() != 2) ErrorMessage(3016, characterfile, filename);

        for (int j = chk[0]; j < chk[1]; ++j)
        {
            position curPosition
                = AnimDataPosition(functionline, characterfile, filename, modcode, filepath, j, type);

            if (error) throw nemesis::exception();

            if (curPosition != behaviorfilelist && curPosition != eventnamelist
                && curPosition != motiondatalist && curPosition != rotationdatalist)
            { ErrorMessage(3018, modcode, characterfile, filename); }
        }
    }

    functionline.shrink_to_fit();
    newline = functionline;
    return true;
}

bool AnimSetDataUpdate(string modcode,
                       string animdatasetfile,
                       string projectsource,
                       string projectfile,
                       string filepath,
                       MasterAnimSetData& animSetData,
                       bool isNewProject,
                       unordered_map<string, string>& lastUpdate)
{
    if (behaviorPath[nemesis::to_lower_copy(animdatasetfile)].empty()) ErrorMessage(2007, animdatasetfile);

    string filename  = GetFileName(filepath);
    string lowerfile = nemesis::to_lower_copy(filename);

    if (!saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate)) return false;

    if (isNewProject)
    {
        if (!GetFunctionLines(filepath, animSetData.newAnimSetData[projectfile][lowerfile], false))
            return false;
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

        vecstr storeline;

        if (!GetFunctionLines(filepath, storeline, false)) return false;

        for (string& line : storeline)
        {
            if (error) throw nemesis::exception();

            if ((line.find("<!-- MOD_CODE", 0) != NOT_FOUND) && (line.find("OPEN -->", 0) != NOT_FOUND)
                && (!edited))
            {
                edited        = true;
                editline      = linecount;
                starteditline = linecount;
            }
            else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
            {
                edited       = false;
                originalopen = true;
                originalline = linecount;
                startoriline = linecount;
            }
            else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
            {
                edited = false;

                if (originalopen)
                {
                    int tempint                             = editline - originalline;
                    NewCoordinate[linecount]                = tempint;
                    modEditLine[to_string(linecount) + "R"] = coordinate - tempint - 2;

                    for (int i = startoriline; i < originalline; ++i)
                    {
                        modEditCoordinate.push_back(i);
                        modEditLine[to_string(i)] = Pair[i];
                    }

                    originalopen = false;
                }
                else
                {
                    int tempint                             = editline - starteditline;
                    NewCoordinate[linecount]                = tempint;
                    modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
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

            vecstr newline = animSetData.newAnimSetData[projectfile][lowerfile];
            vecstr functionline;
            vecstr headerline;

            int editcount = 0;

            bool skip     = false;
            bool isHeader = false;

            functionline.reserve(newline.size());
            linecount = 0;

            if (nemesis::iequals(projectfile, "$header$")) { isHeader = true; }

            newline.push_back("12121332221223212");

            for (string& line : newline)
            {
                if (line.find("<!-- NEW", 0) != NOT_FOUND || line.find("<!-- FOREACH", 0) != NOT_FOUND)
                { skip = true; }

                if (line.find("<!-- *", 0) == NOT_FOUND && !skip)
                {
                    if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
                    {
                        string templine;

                        if (!GetFunctionEdits(templine, storeline, modEditLine[to_string(linecount)]))
                            ErrorMessage(2005, filepath, modEditLine[to_string(linecount)]);

                        using namespace ASDFormat;
                        position curPosition = ASDPosition(storeline,
                                                           projectfile,
                                                           filename,
                                                           modcode,
                                                           modEditLine[to_string(linecount)],
                                                           false);

                        if (curPosition != equipcount && curPosition != typecount
                            && curPosition != animpackcount && curPosition != attackcount
                            && curPosition != crc32count)
                        {
                            templine.append("					<!-- *" + modcode + "* -->");
                            functionline.push_back(templine);

                            if (line.find("<!-- original -->", 0) == NOT_FOUND)
                                line.append("				<!-- original -->");

                            if (editcount != modEditCoordinate.size() - 1) editcount++;
                        }
                    }
                    else if (NewCoordinate[linecount] > 0)
                    {
                        if (isHeader)
                        {
                            vecstr storage = GetFunctionEdits(filepath,
                                                              storeline,
                                                              modEditLine[to_string(linecount) + "R"],
                                                              NewCoordinate[linecount]);

                            if (!error)
                            {
                                headerline.push_back("<!-- NEW *" + modcode + "* -->");
                                headerline.insert(headerline.end(), storage.begin(), storage.end());
                                headerline.push_back("<!-- CLOSE -->");
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

                            functionline.push_back("<!-- NEW *" + modcode + "* -->");
                            vecstr storage = GetFunctionEdits(filepath,
                                                              storeline,
                                                              modEditLine[to_string(linecount) + "R"],
                                                              NewCoordinate[linecount]);

                            if (!error)
                            {
                                functionline.insert(functionline.end(), storage.begin(), storage.end());
                                functionline.push_back("<!-- CLOSE -->");
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }

                    ++linecount;
                }

                if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND) skip = false;

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
                    ErrorMessage(5022, projectfile, lowerheader);

                storeline.insert(storeline.begin(), "<!-- NEW *" + modcode + "* -->");
                storeline.push_back("<!-- CLOSE -->");
                animSetData.newAnimSetData[projectfile][lowerheader] = storeline;
            }
        }
    }

    if (error) throw nemesis::exception();

    return true;
}

bool ClassCheck(vector<int>& modEditCoordinate,
                unordered_map<string, int>& modEditLine,
                unordered_map<int, int>& NewCoordinate,
                int linecount,
                vecstr& storeline,
                string filepath,
                string projectfile,
                string filename,
                string modcode)
{
    unsigned int endline = modEditLine[to_string(linecount) + "R"] + NewCoordinate[linecount];
    bool attacknew       = false;
    bool islast          = false;

    if (ASDFormat::animpackname
        == ASDPosition(
            storeline, projectfile, filename, modcode, modEditLine[to_string(linecount) + "R"], true))
        attacknew = true;

    for (unsigned int k = modEditLine[to_string(linecount) + "R"]; k < endline; ++k)
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
                    ErrorMessage(5021, modcode, filepath, k + 1);
            }
            else
            {
                ErrorMessage(5021, modcode, filepath, k + 1);
            }
        }
    }

    return true;
}
