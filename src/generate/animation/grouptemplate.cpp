#include "Global.h"

#include "utilities/compute.h"
#include "utilities/stringsplit.h"

#include "generate/animation/grouptemplate.h"
#include "generate/animation/nodejoint.h"
#include "generate/animation/singletemplate.h"
#include "generate/animation/templatetree.h"

#pragma warning(disable : 4503)

using namespace std;

void multiChoice(std::string& line,
                 std::string filename,
                 std::vector<std::vector<std::unordered_map<std::string, bool>>> masterOptionPicked,
                 std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo,
                 int numline,
                 std::string format,
                 std::string masterformat,
                 AnimationUtility utility);
bool singleCondition(string condition,
                     string filename,
                     vector<vector<unordered_map<string, bool>>> curOptionPicked,
                     vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                     int numline,
                     string format,
                     string masterformat,
                     AnimationUtility utility);
bool andLoop(string condition,
             string filename,
             vector<vector<unordered_map<string, bool>>> curOptionPicked,
             vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
             int numline,
             string format,
             string masterformat,
             AnimationUtility utility);
bool andOrParenthesis(size_t c_and,
                      size_t c_or,
                      string condition,
                      string filename,
                      vector<vector<unordered_map<string, bool>>> curOptionPicked,
                      vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                      int numline,
                      string format,
                      string masterformat,
                      AnimationUtility utility);
bool andParenthesis(string condition,
                    string filename,
                    vector<vector<unordered_map<string, bool>>> curOptionPicked,
                    vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                    int numline,
                    string format,
                    string masterformat,
                    AnimationUtility utility);
bool newCondition(std::string condition,
                  std::string filename,
                  std::vector<std::vector<std::unordered_map<std::string, bool>>> optionPicked,
                  std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo,
                  int numline,
                  std::string format,
                  std::string masterformat,
                  AnimationUtility utility);
bool conditionProcess(std::string condition,
                      std::string masterformat,
                      std::string format,
                      std::string filename,
                      int numline,
                      std::vector<std::vector<std::unordered_map<std::string, bool>>> curOptionPicked,
                      std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo,
                      bool isNot,
                      AnimationUtility utility);
int formatGroupReplace(string& curline,
                       string oriline,
                       int point,
                       string filename,
                       string format,
                       shared_ptr<master> subFunctionIDs,
                       vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                       int linecount,
                       int groupMulti,
                       int optionMulti,
                       int animMulti,
                       string multiOption,
                       bool& innerError);
void OutputCheckGroup(string format,
                      string behaviorFile,
                      shared_ptr<VecStr> generatedlines,
                      proc& process,
                      condset* curset,
                      bool& elementCatch,
                      bool isMaster,
                      int& openRange,
                      size_t& elementLine,
                      int& counter,
                      int groupCount,
                      ID& eventid,
                      ID& variableid);
void processing(string& line, shared_ptr<NodePackedParameters> parameters);
bool specialCondition(string condition,
                      string filename,
                      vector<vector<unordered_map<string, bool>>> curOptionPicked,
                      vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                      int numline,
                      string format,
                      string masterformat,
                      AnimationUtility utility);

GroupTemplate::GroupTemplate(VecStr grouptemplateformat, shared_ptr<AnimTemplate> n_grouptemplate)
{
    templatelines = grouptemplateformat;
    grouptemplate = n_grouptemplate;
}

void GroupTemplate::getFunctionLines(shared_ptr<VecStr> functionline,
                                     string behaviorFile,
                                     string formatname,
                                     vector<int>& stateID,
                                     shared_ptr<master> newSubFunctionIDs,
                                     vector<vector<shared_ptr<AnimationInfo>>> newGroupAnimInfo,
                                     int nFunctionID,
                                     ImportContainer& import,
                                     ID newEventID,
                                     ID newVariableID,
                                     string masterFormat,
                                     NewAnimLock& animLock,
                                     int groupCount)
{
    bool isMaster;

    if (groupCount == -1)
    {
        filename = masterFormat + "_master";
        isMaster = true;
    }
    else
    {
        filename = masterFormat + "_group";
        isMaster = false;
    }

    format         = formatname;
    strID          = to_string(nFunctionID);
    nextFunctionID = nFunctionID;
    newImport      = const_cast<ImportContainer*>(&import);
    atomicLock     = const_cast<NewAnimLock*>(&animLock);
    subFunctionIDs = newSubFunctionIDs;
    groupAnimInfo  = newGroupAnimInfo;
    eventid        = newEventID;
    variableid     = newVariableID;

    bool elementCatch  = false;
    bool norElement    = false;
    bool negative      = false;
    bool isEnd         = false;
    int openRange      = 0;
    int counter        = 0;
    size_t elementLine = 0;

    vector<int> fixedStateID = stateID;
    unordered_map<int, bool> IsConditionOpened;
    vector<vector<unordered_map<string, bool>>> masterOptionPicked;

    for (auto& groupInfo : groupAnimInfo)
    {
        vector<unordered_map<string, bool>> curGroupInfo;

        for (auto& animInfo : groupInfo)
        {
            curGroupInfo.push_back(animInfo->optionPicked);
        }

        if (curGroupInfo.size() > 0) { masterOptionPicked.push_back(curGroupInfo); }
    }

    while (strID.length() < 4)
    {
        strID = "0" + strID;
    }

    IsConditionOpened[0] = true;
    functionline->reserve(templatelines.size() + 20 * memory);

    proc process               = grouptemplate->process;
    process.format             = format;
    process.masterformat       = masterFormat;
    process.behaviorFile       = behaviorFile;
    process.masterOptionPicked = &masterOptionPicked;
    process.fixedStateID       = fixedStateID;
    process.masterFunction     = subFunctionIDs;
    process.animLock           = atomicLock;
    process.furnitureCount     = groupCount;
    process.groupMulti         = isMaster ? -1 : groupCount - 1;
    process.isGroup            = true;
    process.isMaster           = isMaster;
    process.generatedlines     = functionline;
    process.strID              = &strID;
    process.negative           = &negative;
    process.isEnd              = &isEnd;
    process.elementCatch       = &elementCatch;
    process.openRange          = &openRange;
    process.counter            = &counter;
    process.elementLine        = &elementLine;
    process.eventid            = &eventid;
    process.variableid         = &variableid;
    process.IDExist            = &IDExist;
    process.newImport          = newImport;
    process.curGroup           = this;
    process.norElement         = &norElement;
    process.zeroEvent          = zeroEvent;
    process.zeroVariable       = zeroVariable;

    // output
    OutputCheckGroup(masterFormat,
                     behaviorFile,
                     functionline,
                     process,
                     &grouptemplate->lines,
                     elementCatch,
                     isMaster,
                     openRange,
                     elementLine,
                     counter,
                     groupCount,
                     eventid,
                     variableid);

    // OutputGroupBackup(functionline, format, masterFormat, behaviorFile, groupCount, templatelines, IsConditionOpened, masterOptionPicked, fixedStateID);

    IsConditionOpened[0] = false;

    for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
    {
        if (it->second) { ErrorMessage(1120, format, filename); }
    }

    if (functionline->size() != 0)
    {
        if (!functionline->back().empty()) { functionline->push_back(""); }
        else if (functionline->size() > 1)
        {
            while (functionline->back().empty())
            {
                if ((*functionline)[functionline->size() - 2].empty()) { functionline->pop_back(); }
                else
                {
                    break;
                }
            }
        }
    }

    functionline->shrink_to_fit();
    return;
}

void GroupTemplate::OutputGroupBackup(shared_ptr<VecStr> functionline,
                                      string format,
                                      string masterFormat,
                                      string behaviorFile,
                                      int groupCount,
                                      VecStr templatelines,
                                      unordered_map<int, bool>& IsConditionOpened,
                                      vector<vector<unordered_map<string, bool>>> masterOptionPicked,
                                      vector<int> fixedStateID)
{
    unordered_map<string, string> IDExist;
    VecStr tempstore;
    VecStr templateID;
    string multiOption;
    bool negative   = false;
    bool newOpen    = false;
    bool multi      = false;
    bool norElement = false;
    bool open       = false;
    bool skip       = false;
    bool freeze     = false; // mainly used by CONDITION to freeze following CONDITION

    int curGroup       = groupCount - 1;
    int openRange      = 0;
    int condition      = 0;
    int counter        = 0;
    int order          = -2;
    int groupOrder     = -2;
    size_t elementLine = -1;

    for (uint i = 0; i < templatelines.size(); ++i)
    {
        bool uniqueskip   = false;
        bool elementCatch = false;
        string line       = templatelines[i];

        if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND && !multi)
        {
            condition++;

            if (!freeze && ((newOpen && !skip) || !newOpen))
            {
                if (!IsConditionOpened[condition])
                {
                    if (isPassed(condition, IsConditionOpened))
                    {
                        size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
                        string conditionLine  = line.substr(
                            optionPosition, line.find("^ -->", optionPosition) - optionPosition);
                        AnimationUtility utility;
                        utility.originalCondition = conditionLine;
                        utility.currentProcess    = this;
                        utility.hasGroup          = true;
                        utility.groupMulti        = curGroup;

                        if (newCondition(conditionLine,
                                         filename,
                                         masterOptionPicked,
                                         groupAnimInfo,
                                         i + 1,
                                         format,
                                         masterFormat,
                                         utility))
                        {
                            skip                         = false;
                            IsConditionOpened[condition] = true;
                        }
                        else
                        {
                            skip = true;
                        }

                        if (error) throw nemesis::exception();
                    }
                }
                else
                {
                    skip   = true;
                    freeze = true;
                }
            }

            uniqueskip = true;
        }
        else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) { ErrorMessage(1119, format, filename, i + 1); }

            if (!freeze && ((newOpen && !skip) || !newOpen))
            {
                if (!IsConditionOpened[condition])
                {
                    if (isPassed(condition, IsConditionOpened))
                    {
                        size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
                        string option
                            = line.substr(optionPosition, line.find("^", optionPosition) - optionPosition);
                        AnimationUtility utility;
                        utility.originalCondition = line;
                        utility.currentProcess    = this;
                        utility.hasGroup          = true;
                        utility.groupMulti        = curGroup;

                        if (newCondition(option,
                                         filename,
                                         masterOptionPicked,
                                         groupAnimInfo,
                                         i + 1,
                                         format,
                                         masterFormat,
                                         utility))
                        {
                            skip                         = false;
                            IsConditionOpened[condition] = true;
                            uniqueskip                   = true;
                        }
                        else
                        {
                            skip = true;
                        }

                        if (error) throw nemesis::exception();
                    }
                }
                else
                {
                    skip   = true;
                    freeze = true;
                }
            }

            uniqueskip = true;
        }
        else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) { ErrorMessage(1119, format, filename, i + 1); }

            if (!freeze && ((newOpen && !skip) || !newOpen))
            {
                if (!IsConditionOpened[condition])
                {
                    if (isPassed(condition, IsConditionOpened))
                    {
                        skip                         = false;
                        IsConditionOpened[condition] = true;
                    }
                    else
                    {
                        skip = true;
                    }
                }
                else
                {
                    skip   = true;
                    freeze = true;
                }
            }

            uniqueskip = true;
        }
        else if (line.find("<!-- NEW ^" + masterFormat + "^ -->", 0) != NOT_FOUND
                 || line.find("<!-- NEW ^" + masterFormat + "_group^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) { ErrorMessage(1116, format, filename, i + 1); }

            ErrorMessage(1164, format, filename, i + 1);
        }
        else if (line.find("<!-- FOREACH ^" + masterFormat + "^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) { ErrorMessage(1116, format, filename, i + 1); }

            if (IsConditionOpened[condition])
            {
                if (groupCount != -1)
                {
                    if (!open)
                    {
                        open        = true;
                        multi       = true;
                        order       = -1;
                        multiOption = masterFormat;
                    }
                    else
                    {
                        ErrorMessage(1115, format, filename, i + 1);
                    }
                }
                else
                {
                    ErrorMessage(1159, format, filename, i + 1);
                }
            }

            newOpen    = true;
            uniqueskip = true;
        }
        else if (line.find("<!-- FOREACH ^" + masterFormat + "_group^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) { ErrorMessage(1116, format, filename, i + 1); }

            if (IsConditionOpened[condition])
            {
                if (groupCount == -1)
                {
                    if (!open)
                    {
                        open        = true;
                        multi       = true;
                        groupOrder  = -1;
                        multiOption = masterFormat + "_group";
                    }
                    else
                    {
                        ErrorMessage(1115, format, filename, i + 1);
                    }
                }
                else
                {
                    ErrorMessage(1160, format, filename, i + 1);
                }
            }

            newOpen    = true;
            uniqueskip = true;
        }
        else if (line.find("<!-- NEW ^" + masterFormat + "_master^ -->", 0) != NOT_FOUND
                 || line.find("<!-- FOREACH ^" + masterFormat + "_master^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) { ErrorMessage(1116, format, filename, i + 1); }

            ErrorMessage(1163, format, filename, i + 1);
        }
        else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) { ErrorMessage(1116, format, filename, i + 1); }

            if (IsConditionOpened[condition])
            {
                if (!open)
                {
                    string curOption = getOption(line);
                    bool isNot       = false;

                    if (curOption[0] == '!')
                    {
                        isNot     = true;
                        curOption = curOption.substr(1);
                    }

                    VecStr optionInfo = GetOptionInfo(
                        curOption, masterFormat, filename, i + 1, groupAnimInfo, false, true, groupCount);

                    if (error) throw nemesis::exception();

                    if (optionInfo[3].find("AnimObject") != NOT_FOUND)
                    { ErrorMessage(1129, format, filename, i + 1); }

                    if (!groupAnimInfo[stoi(optionInfo[1])][stoi(optionInfo[2])]->optionPicked[curOption])
                    {
                        // clear group number
                        curOption = nemesis::regex_replace(string(optionInfo[2]),
                                                         nemesis::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"),
                                                         string("\\1"));

                        if (groupAnimInfo[stoi(optionInfo[1])][stoi(optionInfo[2])]->optionPicked[curOption])
                        {
                            if (isNot) { skip = true; }
                            else
                            {
                                open = true;
                            }
                        }
                        else
                        {
                            // Check if current condition accepts other options that are linked
                            if (isNot) { open = true; }
                            else
                            {
                                skip = true;
                            }
                        }
                    }
                    else
                    {
                        if (isNot) { skip = true; }
                        else
                        {
                            open = true;
                        }
                    }
                }
                else
                {
                    ErrorMessage(1116, format, filename, i + 1);
                }
            }

            newOpen    = true;
            uniqueskip = true;
        }
        else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) { ErrorMessage(1116, format, filename, i + 1); }

            if (IsConditionOpened[condition])
            {
                if (!open)
                {
                    string curOption = getOption(line);
                    bool isNot       = false;

                    if (curOption[0] == '!')
                    {
                        isNot     = true;
                        curOption = curOption.substr(1);
                    }

                    VecStr optionInfo = GetOptionInfo(
                        curOption, masterFormat, filename, i + 1, groupAnimInfo, true, true, curGroup);

                    if (error) throw nemesis::exception();

                    if (!groupAnimInfo[stoi(optionInfo[1])][stoi(optionInfo[2])]->optionPicked[optionInfo[3]])
                    {
                        // Check if current condition accepts other options that are linked
                        if (isNot)
                        {
                            tempstore.reserve(templatelines.size() / 5);
                            open        = true;
                            multi       = true;
                            multiOption = optionInfo[3];

                            if (optionInfo[2].length() == 0) { order = -1; }
                            else
                            {
                                order = stoi(optionInfo[2]);
                            }
                        }
                        else
                        {
                            skip = true;
                        }
                    }
                    else
                    {
                        if (isNot) { skip = true; }
                        else
                        {
                            tempstore.reserve(templatelines.size() / 5);
                            open        = true;
                            multi       = true;
                            multiOption = optionInfo[3];

                            if (optionInfo[2].length() == 0) { order = -1; }
                            else
                            {
                                order = stoi(optionInfo[2]);
                            }
                        }
                    }
                }
                else
                {
                    ErrorMessage(1115, format, filename, i + 1);
                }
            }

            newOpen    = true;
            uniqueskip = true;
        }
        else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
        {
            if (!newOpen) { ErrorMessage(1171, format, filename, i + 1); }

            newOpen    = false;
            uniqueskip = true;
        }
        else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi)
        {
            uniqueskip = true;
        }

        if (!skip && !freeze && !uniqueskip)
        {
            while (true)
            {
                if (multi)
                {
                    tempstore.push_back(line);
                    break;
                }

                if (line.find("MID$", 0) != NOT_FOUND)
                {
                    int counter = sameWordCount(line, "MID$");

                    for (int k = 0; k < counter; ++k)
                    {
                        size_t MIDposition = line.find("MID$");
                        string ID          = nemesis::regex_replace(string(line.substr(MIDposition)),
                                                         nemesis::regex("[^0-9]*([0-9]+).*"),
                                                         string("\\1"));
                        string oldID       = "MID$" + ID;

                        if (line.find(oldID, MIDposition) != NOT_FOUND)
                        {
                            if (groupCount != -1
                                && subFunctionIDs->grouplist[curGroup]->functionIDs.find(oldID)
                                       != subFunctionIDs->grouplist[curGroup]->functionIDs.end())
                            {
                                ID = subFunctionIDs->grouplist[curGroup]->functionIDs[oldID];
                                subFunctionIDs->grouplist[curGroup]->functionIDs.erase(oldID);
                                IDExist[oldID] = ID;
                            }
                            else
                            {
                                if (IDExist[oldID].length() > 0) { ID = IDExist[oldID]; }
                                else
                                {
                                    IDExist[oldID] = strID;
                                    ID             = strID;
                                    newID();
                                }
                            }

                            string templine = line;
                            templine.replace(templine.find("MID$", MIDposition), 3, format);
                            templine = templine.substr(templine.find(format + "$", MIDposition),
                                                       format.length() + oldID.length() - 3);
                            line.replace(line.find("MID$"), oldID.length(), ID);

                            if (groupCount == -1)
                            {
                                // master
                                subFunctionIDs->functionIDs[templine] = ID;
                            }
                            else
                            {
                                // group
                                subFunctionIDs->grouplist[curGroup]->functionIDs[templine] = ID;
                            }
                        }
                    }
                }

                // set state ID
                if (line.find("$(S", 0) != NOT_FOUND)
                {
                    string templine = line.substr(line.find("$(S"));
                    string ID       = nemesis::regex_replace(
                        string(templine), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));
                    int intID;

                    if (line.find("$(S" + ID + "+") == NOT_FOUND)
                    {
                        ID    = "";
                        intID = 0;
                    }
                    else
                    {
                        intID = stoi(ID) - 1;

                        if (intID >= int(fixedStateID.size()) || intID < 0)
                        {
                            ErrorMessage(
                                1168, format, filename, i + 1, templine.substr(0, templine.find(")") + 1));
                        }
                    }

                    if (line.find("$(S" + ID + "+") != NOT_FOUND)
                    { stateReplacer(line, filename, ID, intID, fixedStateID[intID], i + 1, curGroup); }
                }

                if (groupCount != -1)
                {
                    // set animation ID
                    while (line.find("$%$", 0) != NOT_FOUND)
                    {
                        line.replace(line.find("$%$"), 3, to_string(groupCount));
                    }

                    // multi choice selection
                    if (line.find("$MC$", 0) != NOT_FOUND)
                    {
                        AnimationUtility utility;
                        utility.originalCondition = line;
                        utility.currentProcess    = this;
                        utility.hasGroup          = true;
                        utility.groupMulti        = curGroup;
                        multiChoice(line,
                                    filename,
                                    masterOptionPicked,
                                    groupAnimInfo,
                                    i + 1,
                                    format,
                                    masterFormat,
                                    utility);

                        if (error) throw nemesis::exception();
                    }

                    if (line.find("%") != NOT_FOUND)
                    {
                        __int64 counter = count(line.begin(), line.end(), '%');

                        for (int p = 0; p < counter; ++p)
                        {
                            line.replace(line.find("%"), 1, to_string(0));
                        }
                    }
                }

                // compute numelements
                if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND
                    && line.find("</hkparam>") == NOT_FOUND
                    && line.find("<!-- COMPUTE -->", line.find("numelements=\"")) != NOT_FOUND)
                {
                    if (!norElement)
                    {
                        norElement      = true;
                        elementCatch    = true;
                        string templine = line.substr(0, line.find("<hkparam name=\"", 0));
                        openRange       = count(templine.begin(), templine.end(), '\t');
                    }
                    else
                    {
                        ErrorMessage(1136, format, filename, i + 1);
                    }
                }
                else if (line.find("</hkparam>") != NOT_FOUND && norElement)
                {
                    string templine = line.substr(0, line.find("</hkparam>"));
                    __int64 range   = count(templine.begin(), templine.end(), '\t');

                    if (openRange == range)
                    {
                        string oldElement;

                        if ((*functionline)[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
                        {
                            size_t position = (*functionline)[elementLine].find("numelements=\"") + 13;
                            oldElement      = (*functionline)[elementLine].substr(
                                position, (*functionline)[elementLine].find("\">", position) - position);
                        }
                        else
                        {
                            oldElement = "$elements$";
                        }

                        if (oldElement != to_string(counter))
                        {
                            (*functionline)[elementLine].replace(
                                (*functionline)[elementLine].find(oldElement),
                                oldElement.length(),
                                to_string(counter));
                        }

                        norElement  = false;
                        counter     = 0;
                        elementLine = -1;
                    }
                }

                if (norElement)
                {
                    string templine = line;

                    if (templine.find("<hkobject>") != NOT_FOUND)
                    {
                        templine      = templine.substr(0, templine.find("<hkobject>"));
                        __int64 range = count(templine.begin(), templine.end(), '\t');

                        if (range == openRange + 1) { counter++; }
                    }
                    else if (templine.find("\t\t\t#") != NOT_FOUND)
                    {
                        templine          = templine.substr(0, templine.find("#", 0));
                        __int64 reference = count(templine.begin(), templine.end(), '\t');

                        if (reference == openRange + 1)
                        {
                            __int64 number = count(line.begin(), line.end(), '#');
                            counter += int(number);
                        }
                    }
                }

                if ((line.find("#" + masterFormat + "$") != NOT_FOUND
                     || line.find("#" + masterFormat + "_group$") != NOT_FOUND))
                {
                    VecStr generator;
                    size_t nextpos = -1;

                    if (line.find("\t\t\t#") != NOT_FOUND) { StringSplit(line, generator); }
                    else
                    {
                        int ref = sameWordCount(line, "#" + masterFormat);

                        for (int j = 0; j < ref; ++j)
                        {
                            nextpos         = line.find("#" + masterFormat, nextpos + 1);
                            string templine = line.substr(nextpos);
                            string ID       = nemesis::regex_replace(
                                string(templine), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));
                            templine = line.substr(nextpos, line.find(ID, nextpos) - nextpos);
                            generator.push_back(templine);
                        }
                    }

                    nextpos = 0;

                    for (uint p = 0; p < generator.size(); p++)
                    {
                        string ID = generator[p];

                        if (ID.find("#" + masterFormat + "$") != NOT_FOUND && multiOption == masterFormat)
                        {
                            nextpos       = line.find("#" + masterFormat + "$", nextpos) + 1;
                            string tempID = line.substr(nextpos);
                            string curID  = nemesis::regex_replace(
                                string(tempID), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));
                            curID = masterFormat + "$" + curID;

                            if (tempID.find(curID, 0) != NOT_FOUND && nextpos == line.find(curID))
                            {
                                if (subFunctionIDs->grouplist[curGroup]->singlelist[0]->format[curID].length()
                                    == 0)
                                { ErrorMessage(2017, format, filename, i + 1); }

                                line.replace(
                                    nextpos,
                                    curID.length(),
                                    subFunctionIDs->grouplist[curGroup]->singlelist[0]->format[curID]);
                            }
                            else
                            {
                                ErrorMessage(2018, format, filename, i + 1, curID);
                            }
                        }
                        else if (ID.find("#" + masterFormat + "_group") != NOT_FOUND
                                 && multiOption == masterFormat + "_group")
                        {
                            nextpos       = line.find("#" + masterFormat + "_group$", nextpos) + 1;
                            string tempID = line.substr(nextpos);
                            string curID  = nemesis::regex_replace(
                                string(tempID), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));
                            curID = multiOption + "$" + curID;

                            if (tempID.find(curID, 0) != NOT_FOUND && nextpos == line.find(curID))
                            {
                                for (uint k = 0; k < subFunctionIDs->grouplist.size();
                                     ++k) // number of variation
                                {
                                    if (subFunctionIDs->grouplist[k]->functionIDs[curID].length() == 0)
                                    { ErrorMessage(2017, format, filename, i + 1); }

                                    line.replace(nextpos,
                                                 curID.length(),
                                                 subFunctionIDs->grouplist[k]->functionIDs[curID]);
                                }
                            }
                            else
                            {
                                ErrorMessage(2018, format, filename, i + 1, curID);
                            }
                        }
                    }
                }

                if (line.find("$") != NOT_FOUND && groupCount != -1)
                { processing(line, filename, masterFormat, i + 1, eventid, variableid, curGroup); }

                if (error) throw nemesis::exception();

                size_t pos = line.find("(");

                if (pos != NOT_FOUND && line.find(")", pos) != NOT_FOUND)
                {
                    string templine = line.substr(pos);
                    int nextpos     = openEndBracket(templine, '(', ')', format, filename, i + 1) + 1;
                    templine        = templine.substr(0, nextpos);
                    string oldline  = templine;
                    nemesis::calculate(templine, format, filename, i + 1);

                    if (oldline != templine) line.replace(pos, oldline.length(), templine);
                }

                if (line.find("<hkparam name=\"animationName\">") != NOT_FOUND)
                {
                    pos             = line.find("animationName\">") + 15;
                    string animPath = line.substr(pos, line.find("</hkparam>", pos) - pos);
                    nemesis::to_lower(animPath);
                    addUsedAnim(behaviorFile, animPath);
                }
                else if (line.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
                {
                    negative = true;
                }
                else if (negative && line.find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
                {
                    if (line.find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
                    {
                        size_t pos = line.find("<hkparam name=\"relativeToEndOfClip\">") + 36;
                        line.replace(pos, line.find("</hkparam>", pos) - pos, "true");
                    }

                    negative = false;
                }

                if (error) throw nemesis::exception();

                functionline->push_back(line);

                if (elementCatch) { elementLine = functionline->size() - 1; }

                break;
            }
        }

        if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND && IsConditionOpened[condition])
        {
            if (skip) { skip = false; }
            else
            {
                if (multi)
                {
                    int size;
                    int tempOrder;
                    int groupSize;

                    if (groupOrder == -2)
                    {
                        groupOrder = curGroup;
                        groupSize  = groupCount;
                    }
                    else if (groupOrder == -1)
                    {
                        groupOrder = 0;
                        groupSize  = int(subFunctionIDs->grouplist.size());
                    }
                    else
                    {
                        groupSize = groupOrder + 1;
                    }

                    for (int groupMulti = groupOrder; groupMulti < groupSize; ++groupMulti)
                    {
                        if (order == -2)
                        {
                            tempOrder = 0;
                            size      = 1;
                        }
                        else if (order == -1)
                        {
                            tempOrder = 0;
                            size      = int(subFunctionIDs->grouplist[groupMulti]->singlelist.size());
                        }
                        else
                        {
                            tempOrder = order;
                            size      = order + 1;
                        }

                        for (int animMulti = tempOrder; animMulti < size;
                             ++animMulti) // each animation in a group
                        {
                            int optionMulti = 0;

                            while (true)
                            {
                                if (groupCount == -1 && multiOption == masterFormat + "_group") {}
                                else if (optionMulti >= groupAnimInfo[curGroup][animMulti]
                                                            ->optionPickedCount[multiOption])
                                {
                                    break;
                                }

                                bool skip2   = false;
                                bool freeze2 = false;

                                for (uint l = 0; l < tempstore.size(); ++l) // part lines need to add
                                {
                                    string curLine   = tempstore[l];
                                    bool uniqueskip2 = false;
                                    size_t linecount = i + 1 + l - int(tempstore.size());

                                    if (curLine.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
                                    {
                                        condition++;

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    size_t optionPosition
                                                        = curLine.find("<!-- CONDITION START ^") + 22;
                                                    string conditionLine
                                                        = curLine.substr(optionPosition,
                                                                         curLine.find("^ -->", optionPosition)
                                                                             - optionPosition);
                                                    AnimationUtility utility;
                                                    utility.originalCondition = conditionLine;
                                                    utility.currentProcess    = this;
                                                    utility.hasGroup          = true;
                                                    utility.groupMulti        = groupMulti;
                                                    utility.animMulti         = animMulti;
                                                    utility.optionMulti       = optionMulti;
                                                    utility.multiOption       = multiOption;

                                                    if (newCondition(conditionLine,
                                                                     filename,
                                                                     masterOptionPicked,
                                                                     groupAnimInfo,
                                                                     linecount,
                                                                     format,
                                                                     masterFormat,
                                                                     utility))
                                                    {
                                                        skip2                        = false;
                                                        IsConditionOpened[condition] = true;
                                                    }
                                                    else
                                                    {
                                                        skip2 = true;
                                                    }

                                                    if (error)
                                                    {
                                                        functionline->shrink_to_fit();
                                                        return;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION ^", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0)
                                        { ErrorMessage(1119, format, filename, linecount); }

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    size_t optionPosition
                                                        = curLine.find("<!-- CONDITION ^") + 16;
                                                    string option
                                                        = curLine.substr(optionPosition,
                                                                         curLine.find("^ -->", optionPosition)
                                                                             - optionPosition);
                                                    AnimationUtility utility;
                                                    utility.originalCondition = option;
                                                    utility.currentProcess    = this;
                                                    utility.hasGroup          = true;
                                                    utility.groupMulti        = groupMulti;
                                                    utility.animMulti         = animMulti;
                                                    utility.optionMulti       = optionMulti;
                                                    utility.multiOption       = multiOption;

                                                    if (newCondition(option,
                                                                     filename,
                                                                     masterOptionPicked,
                                                                     groupAnimInfo,
                                                                     linecount,
                                                                     format,
                                                                     masterFormat,
                                                                     utility))
                                                    {
                                                        skip2                        = false;
                                                        IsConditionOpened[condition] = true;
                                                        uniqueskip2                  = true;
                                                    }
                                                    else
                                                    {
                                                        skip2 = true;
                                                    }

                                                    if (error)
                                                    {
                                                        functionline->shrink_to_fit();
                                                        return;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION -->", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0)
                                        { ErrorMessage(1119, format, filename, linecount); }

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    skip2                        = false;
                                                    IsConditionOpened[condition] = true;
                                                    uniqueskip2                  = true;
                                                    size_t conditionPosition
                                                        = curLine.find("<!-- CONDITION") + 14;
                                                    string replacement1
                                                        = curLine.substr(0, conditionPosition + 1);
                                                    string replacement2 = curLine.substr(conditionPosition);
                                                    functionline->push_back(replacement1 + "START"
                                                                            + replacement2);
                                                }
                                                else
                                                {
                                                    skip2 = true;
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
                                    {
                                        uniqueskip2 = true;
                                    }

                                    if (curLine.find("<!-- ") != NOT_FOUND
                                        && curLine.find(" -->", curLine.find("<!-- ")) != NOT_FOUND
                                        && curLine.find("$MC$") == NOT_FOUND)
                                    {
                                        bool isNot    = false;
                                        size_t pos    = curLine.find("<!-- ") + 5;
                                        string option = curLine.substr(pos, curLine.find(" -->") - pos);

                                        if (option[0] == '!')
                                        {
                                            isNot  = true;
                                            option = option.substr(1);
                                        }

                                        if (option == "L") { option = to_string(groupAnimInfo.size() - 1); }

                                        if (option == "F") { option = "0"; }

                                        if (isOnlyNumber(option))
                                        {
                                            if (to_string(animMulti) == option && isNot) { break; }
                                            else if (to_string(animMulti) != option && !isNot)
                                            {
                                                break;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }

                                    if (!skip2 && !freeze2 && !uniqueskip2)
                                    {
                                        if (curLine.find("$") != NOT_FOUND)
                                        {
                                            // set animation ID
                                            while (curLine.find("$%$", 0) != NOT_FOUND)
                                            {
                                                curLine.replace(
                                                    curLine.find("$%$"), 3, to_string(groupCount));
                                            }

                                            if (curLine.find("MID$", 0) != NOT_FOUND)
                                            {
                                                int counter = sameWordCount(curLine, "MID$");

                                                for (int k = 0; k < counter; ++k)
                                                {
                                                    size_t MIDposition = curLine.find("MID$");
                                                    string ID          = nemesis::regex_replace(
                                                        string(curLine.substr(MIDposition)),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    string oldID = "MID$" + ID;

                                                    if (curLine.find(oldID, MIDposition) != NOT_FOUND)
                                                    {
                                                        if (groupCount != -1
                                                            && subFunctionIDs->grouplist[curGroup]
                                                                       ->functionIDs.find(oldID)
                                                                   != subFunctionIDs->grouplist[curGroup]
                                                                          ->functionIDs.end())
                                                        {
                                                            ID = subFunctionIDs->grouplist[curGroup]
                                                                     ->functionIDs[oldID];
                                                            subFunctionIDs->grouplist[curGroup]
                                                                ->functionIDs.erase(oldID);
                                                            IDExist[oldID] = ID;
                                                        }
                                                        else
                                                        {
                                                            if (IDExist[oldID].length() > 0)
                                                            { ID = IDExist[oldID]; }
                                                            else
                                                            {
                                                                IDExist[oldID] = strID;
                                                                ID             = strID;
                                                                newID();
                                                            }
                                                        }

                                                        string templine = curLine;
                                                        templine.replace(
                                                            templine.find("MID$", MIDposition), 3, format);
                                                        templine = templine.substr(
                                                            templine.find(format + "$", MIDposition),
                                                            format.length() + oldID.length() - 3);
                                                        curLine.replace(
                                                            curLine.find("MID$"), oldID.length(), ID);

                                                        if (groupCount == -1)
                                                        {
                                                            // master
                                                            subFunctionIDs->functionIDs[templine] = ID;
                                                        }
                                                        else
                                                        {
                                                            // group
                                                            subFunctionIDs->grouplist[curGroup]
                                                                ->functionIDs[templine]
                                                                = ID;
                                                        }
                                                    }
                                                }
                                            }

                                            // set state ID
                                            if (curLine.find("$(S", 0) != NOT_FOUND)
                                            {
                                                string templine = curLine.substr(curLine.find("$(S"));
                                                string ID
                                                    = nemesis::regex_replace(string(templine),
                                                                           nemesis::regex("[^0-9]*([0-9]+).*"),
                                                                           string("\\1"));
                                                int intID;

                                                if (curLine.find("$(S" + ID + "+") == NOT_FOUND)
                                                {
                                                    ID    = "";
                                                    intID = 0;
                                                }
                                                else
                                                {
                                                    intID = stoi(ID) - 1;

                                                    if (intID >= int(fixedStateID.size()) || intID < 0)
                                                    { ErrorMessage(1127, format, filename, linecount); }
                                                }

                                                if (curLine.find("$(S" + ID + "+") != NOT_FOUND)
                                                {
                                                    stateReplacer(curLine,
                                                                  filename,
                                                                  ID,
                                                                  intID,
                                                                  fixedStateID[intID],
                                                                  linecount,
                                                                  curGroup);
                                                }
                                            }

                                            // multi choice selection
                                            if (curLine.find("$MC$", 0) != NOT_FOUND)
                                            {
                                                AnimationUtility utility;
                                                utility.originalCondition = curLine;
                                                utility.currentProcess    = this;
                                                utility.hasGroup          = true;
                                                utility.groupMulti        = groupMulti;
                                                utility.animMulti         = animMulti;
                                                utility.optionMulti       = optionMulti;
                                                utility.multiOption       = multiOption;

                                                multiChoice(curLine,
                                                            filename,
                                                            masterOptionPicked,
                                                            groupAnimInfo,
                                                            linecount,
                                                            format,
                                                            masterFormat,
                                                            utility);
                                            }
                                        }

                                        if (error) throw nemesis::exception();

                                        if (curLine.find("%") != NOT_FOUND)
                                        {
                                            __int64 counter = count(curLine.begin(), curLine.end(), '%');

                                            for (int p = 0; p < counter; ++p)
                                            {
                                                curLine.replace(curLine.find("%"), 1, to_string(optionMulti));
                                            }
                                        }

                                        if (norElement)
                                        {
                                            string templine = curLine;

                                            if (templine.find("<hkobject>") != NOT_FOUND)
                                            {
                                                templine = templine.substr(0, templine.find("<hkobject>"));
                                                __int64 range = count(templine.begin(), templine.end(), '\t');

                                                if (range == openRange + 1) { counter++; }
                                            }
                                            else if (templine.find("\t\t\t#") != NOT_FOUND)
                                            {
                                                templine = templine.substr(0, templine.find("#", 0));
                                                __int64 reference
                                                    = count(templine.begin(), templine.end(), '\t');

                                                if (reference == openRange + 1)
                                                {
                                                    __int64 number
                                                        = count(curLine.begin(), curLine.end(), '#');
                                                    counter += int(number);
                                                }
                                            }
                                        }

                                        if (curLine.find("\t\t\t#") != NOT_FOUND
                                            && (curLine.find("#" + masterFormat + "$") != NOT_FOUND
                                                || curLine.find("#" + masterFormat + "_group$") != NOT_FOUND))
                                        {
                                            VecStr generator;
                                            StringSplit(curLine, generator);
                                            size_t nextpos = 0;

                                            for (uint p = 0; p < generator.size(); p++)
                                            {
                                                string ID = generator[p];

                                                if (ID.find("#" + masterFormat + "$") != NOT_FOUND
                                                    && multiOption == masterFormat)
                                                {
                                                    nextpos
                                                        = curLine.find("#" + masterFormat + "$", nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = masterFormat + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->grouplist[groupMulti]
                                                                ->singlelist[animMulti]
                                                                ->format[curID]
                                                                .length()
                                                            == 0)
                                                        { ErrorMessage(2017, format, filename, linecount); }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->grouplist[groupMulti]
                                                                            ->singlelist[animMulti]
                                                                            ->format[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2018, format, filename, linecount, curID);
                                                    }
                                                }
                                                else if (ID.find("#" + masterFormat + "_group") != NOT_FOUND
                                                         && multiOption == masterFormat + "_group")
                                                {
                                                    nextpos = curLine.find("#" + multiOption, nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = multiOption + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->grouplist[groupMulti]
                                                                ->functionIDs[curID]
                                                                .length()
                                                            == 0)
                                                        { ErrorMessage(2017, format, filename, linecount); }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->grouplist[groupMulti]
                                                                            ->functionIDs[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2018, format, filename, linecount, curID);
                                                    }
                                                }
                                            }
                                        }

                                        if (curLine.find("$") != NOT_FOUND)
                                        {
                                            processing(curLine,
                                                       filename,
                                                       masterFormat,
                                                       linecount,
                                                       eventid,
                                                       variableid,
                                                       curGroup,
                                                       optionMulti,
                                                       animMulti,
                                                       multiOption);
                                        }

                                        if (error) throw nemesis::exception();

                                        size_t pos = curLine.find("(");

                                        if (pos != NOT_FOUND && curLine.find(")", pos) != NOT_FOUND)
                                        {
                                            string templine = curLine.substr(pos);
                                            int nextpos     = openEndBracket(
                                                              templine, '(', ')', format, filename, linecount)
                                                          + 1;

                                            if (error)
                                            {
                                                functionline->shrink_to_fit();
                                                return;
                                            }

                                            templine       = templine.substr(0, nextpos);
                                            string oldline = templine;
                                            nemesis::calculate(templine, format, filename, linecount);

                                            if (oldline != templine)
                                            { curLine.replace(pos, oldline.length(), templine); }
                                        }

                                        if (curLine.find("<hkparam name=\"animationName\">") != NOT_FOUND)
                                        {
                                            pos = curLine.find("animationName\">") + 15;
                                            string animPath
                                                = curLine.substr(pos, curLine.find("</hkparam>", pos) - pos);
                                            nemesis::to_lower(animPath);
                                            addUsedAnim(behaviorFile, animPath);
                                        }
                                        else if (curLine.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
                                        {
                                            negative = true;
                                        }
                                        else if (negative
                                                 && curLine.find("<hkparam name=\"relativeToEndOfClip\">")
                                                        != NOT_FOUND)
                                        {
                                            if (curLine.find(
                                                    "<hkparam name=\"relativeToEndOfClip\">true</hkparam>")
                                                == NOT_FOUND)
                                            {
                                                size_t pos
                                                    = curLine.find("<hkparam name=\"relativeToEndOfClip\">")
                                                      + 36;
                                                curLine.replace(
                                                    pos, curLine.find("</hkparam>", pos) - pos, "true");
                                            }

                                            negative = false;
                                        }

                                        functionline->push_back(curLine);
                                    }

                                    if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0)
                                        { ErrorMessage(1118, format, filename, linecount); }

                                        if (freeze2 && IsConditionOpened[condition]) { freeze2 = false; }

                                        if (isPassed(condition, IsConditionOpened)) { skip2 = false; }
                                        else
                                        {
                                            skip2 = true;
                                        }

                                        IsConditionOpened[condition] = false;
                                        condition--;
                                    }
                                }

                                if (groupCount == -1 && multiOption == masterFormat + "_group") { break; }

                                ++optionMulti;
                            }
                        }

                        order = -2;
                    }
                }

                tempstore.clear();
            }

            multi = false;
            open  = false;
        }
        else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) { ErrorMessage(1118, format, filename, i + 1); }

            if ((newOpen && !skip) || !newOpen)
            {
                if (freeze && IsConditionOpened[condition]) { freeze = false; }

                if (isPassed(condition, IsConditionOpened)) { skip = false; }
                else
                {
                    skip = true;
                }

                IsConditionOpened[condition] = false;
            }

            condition--;
        }
    }

    if (newOpen) { ErrorMessage(1116, format, filename, templatelines.size()); }
}

VecStr
ExistingFunction::groupExistingFunctionProcess(int curFunctionID,
                                               VecStr existingFunctionLines,
                                               shared_ptr<master> newSubFunctionIDs,
                                               vector<vector<shared_ptr<AnimationInfo>>> newGroupAnimInfo,
                                               string curformat,
                                               ImportContainer& import,
                                               ID newEventID,
                                               ID newVariableID,
                                               int& nFunctionID,
                                               bool hasMaster,
                                               bool hasGroup,
                                               SetStr templateGroup,
                                               bool ignoreGroup)
{
    VecStr newFunctionLines;

    VecStr tempstore;
    VecStr empty;

    newImport      = const_cast<ImportContainer*>(&import);
    nextFunctionID = const_cast<int*>(&nFunctionID);
    format         = curformat;
    strID          = to_string(nFunctionID);
    subFunctionIDs = newSubFunctionIDs;
    groupAnimInfo  = newGroupAnimInfo;
    eventid        = newEventID;
    variableid     = newVariableID;
    m_hasGroup     = hasGroup;

    string multiOption;
    bool formatOpen      = false;
    bool skipTemplate    = false;
    bool newOpen         = false;
    bool functionNot     = false;
    bool isElement       = false;
    bool multi           = false;
    bool open            = false;
    bool skip            = false;
    bool freeze          = false; // mainly used by CONDITION to freeze following CONDITION
    int condition        = 0;
    __int64 openRange    = 0;
    __int64 elementCount = 0;
    int counter          = 0;
    int order            = -2;
    int groupOrder       = -2;
    int elementLine      = 0;
    unordered_map<int, bool> IsConditionOpened;
    unordered_map<string, bool> otherAnimType;
    vector<vector<unordered_map<string, bool>>> masterOptionPicked;

    string IDFileName = to_string(curFunctionID);

    while (IDFileName.length() < 4)
    {
        IDFileName = "0" + IDFileName;
    }

    IDFileName = "#" + IDFileName;

    for (auto& groupInfo : groupAnimInfo)
    {
        vector<unordered_map<string, bool>> curGroupInfo;

        for (auto& animInfo : groupInfo)
        {
            curGroupInfo.push_back(animInfo->optionPicked);
        }

        if (curGroupInfo.size() > 0) { masterOptionPicked.push_back(curGroupInfo); }
    }

    while (strID.length() < 4)
    {
        strID = "0" + strID;
    }

    for (auto& templatecode : templateGroup)
    {
        if (templatecode != format)
        {
            string lowercode                     = nemesis::to_lower_copy(templatecode);
            otherAnimType[lowercode]             = true;
            otherAnimType[lowercode + "_group"]  = true;
            otherAnimType[lowercode + "_master"] = true;
        }
    }

    IsConditionOpened[0] = true;
    newFunctionLines.reserve(existingFunctionLines.size() + 20 * memory);

    for (uint i = 0; i < existingFunctionLines.size(); ++i)
    {
        bool uniqueskip   = false;
        bool elementCatch = false;
        string line       = existingFunctionLines[i];

        if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND && !multi)
        {
            condition++;

            if (!skipTemplate && ((newOpen && !skip) || !newOpen))
            {
                if (!freeze)
                {
                    if (!IsConditionOpened[condition])
                    {
                        if (isPassed(condition, IsConditionOpened))
                        {
                            size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
                            string conditionLine  = line.substr(
                                optionPosition, line.find("^ -->", optionPosition) - optionPosition);

                            if (conditionLine.find("[") == NOT_FOUND)
                            {
                                if (format != conditionLine) skip = true;
                            }
                            else
                            {
                                AnimationUtility utility;
                                utility.originalCondition = conditionLine;
                                utility.currentProcess    = this;
                                utility.isExisting        = true;
                                utility.hasGroup          = hasGroup;

                                if (newCondition(conditionLine,
                                                 IDFileName,
                                                 masterOptionPicked,
                                                 groupAnimInfo,
                                                 i + 1,
                                                 format,
                                                 format,
                                                 utility))
                                {
                                    skip                         = false;
                                    IsConditionOpened[condition] = true;
                                }
                                else
                                {
                                    skip = true;
                                }

                                if (error) throw nemesis::exception();
                            }
                        }
                    }
                    else
                    {
                        skip = true;
                    }
                }

                uniqueskip = true;
            }
        }
        else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) ErrorMessage(1119, format, IDFileName, i + 1);

            if (!skipTemplate && ((newOpen && !skip) || !newOpen))
            {
                if (!freeze)
                {
                    if (!IsConditionOpened[condition])
                    {
                        if (isPassed(condition, IsConditionOpened))
                        {
                            size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
                            string option         = line.substr(optionPosition,
                                                        line.find("^", optionPosition) - optionPosition);

                            if (option.find("[") == NOT_FOUND)
                            {
                                if (format != option) { skip = true; }
                            }
                            else
                            {
                                AnimationUtility utility;
                                utility.originalCondition = option;
                                utility.currentProcess    = this;
                                utility.isExisting        = true;
                                utility.hasGroup          = hasGroup;

                                if (newCondition(option,
                                                 IDFileName,
                                                 masterOptionPicked,
                                                 groupAnimInfo,
                                                 i + 1,
                                                 format,
                                                 format,
                                                 utility))
                                {
                                    skip                         = false;
                                    IsConditionOpened[condition] = true;
                                }
                                else
                                {
                                    skip = true;
                                }

                                if (error) throw nemesis::exception();
                            }
                        }
                    }
                    else
                    {
                        skip   = true;
                        freeze = true;
                    }
                }

                uniqueskip = true;
            }
        }
        else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) ErrorMessage(1119, format, IDFileName, i + 1);

            if (!skipTemplate && ((newOpen && !skip) || !newOpen))
            {
                if (!freeze)
                {
                    if (!IsConditionOpened[condition])
                    {
                        if (isPassed(condition, IsConditionOpened))
                        {
                            skip                         = false;
                            IsConditionOpened[condition] = true;
                        }
                        else
                        {
                            skip = true;
                        }
                    }
                    else
                    {
                        skip   = true;
                        freeze = true;
                    }
                }

                uniqueskip = true;
            }
        }
        else if (line.find("<!-- FOREACH ^" + format, 0) != NOT_FOUND)
        {
            if (newOpen) ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());

            if (line.find("<!-- FOREACH ^" + format + "^ -->", 0) != NOT_FOUND)
            {
                if (IsConditionOpened[condition])
                {
                    if (!hasGroup)
                    {
                        if (!open)
                        {
                            if (groupAnimInfo.size() > 0)
                            {
                                if (ignoreGroup)
                                    groupOrder = -1;
                                else
                                    order = -1;

                                open        = true;
                                multi       = true;
                                multiOption = format;
                                formatOpen  = true;
                            }
                            else
                            {
                                skip = true;
                            }
                        }
                        else
                        {
                            ErrorMessage(1115, format, IDFileName, i + 1);
                        }
                    }
                    else
                    {
                        ErrorMessage(1161, format, IDFileName, i + 1);
                    }
                }
            }
            else if (line.find("<!-- FOREACH ^" + format + "_group^ -->", 0) != NOT_FOUND)
            {
                if (IsConditionOpened[condition])
                {
                    if (hasGroup)
                    {
                        if (!open)
                        {
                            if (groupAnimInfo.size() > 0)
                            {
                                open        = true;
                                multi       = true;
                                groupOrder  = -1;
                                multiOption = format + "_group";
                                formatOpen  = true;
                            }
                            else
                            {
                                skip = true;
                            }
                        }
                        else
                        {
                            ErrorMessage(1115, format, IDFileName, i + 1);
                        }
                    }
                    else
                    {
                        ErrorMessage(1167, format, IDFileName, i + 1);
                    }
                }

                newOpen    = true;
                uniqueskip = true;
            }
            else if (line.find("<!-- FOREACH ^" + format + "_master^ -->", 0) != NOT_FOUND)
            {
                if (IsConditionOpened[condition])
                {
                    if (hasMaster)
                    {
                        if (!open)
                        {
                            if (groupAnimInfo.size() > 0)
                            {
                                open        = true;
                                multi       = true;
                                groupOrder  = -2;
                                multiOption = format + "_master";
                                formatOpen  = true;
                            }
                            else
                            {
                                skip = true;
                            }
                        }
                        else
                        {
                            ErrorMessage(1115, format, IDFileName, i + 1);
                        }
                    }
                    else
                    {
                        ErrorMessage(1162, format, IDFileName, i + 1);
                    }
                }

                newOpen    = true;
                uniqueskip = true;
            }

            newOpen    = true;
            uniqueskip = true;
            formatOpen = true;
        }
        else if (line.find("<!-- NEW ^" + format + "^ -->", 0) != NOT_FOUND
                 || line.find("<!-- NEW ^" + format + "_group^ -->", 0) != NOT_FOUND
                 || line.find("<!-- NEW ^" + format + "_master^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());

            ErrorMessage(1164, format, IDFileName, i + 1);
        }
        else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
        {
            if (newOpen) ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());

            newOpen        = true;
            size_t pos     = line.find("<!-- NEW ^") + 10;
            string checker = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));

            if (!otherAnimType[checker])
            {
                uniqueskip = true;

                if (IsConditionOpened[condition])
                {
                    if (!open)
                    {
                        string curOption = getOption(line);

                        if (curOption.find("[") == NOT_FOUND)
                        {
                            if (format != curOption)
                                skip = true;
                            else
                                open = true;
                        }
                        else
                        {
                            bool isNot = false;

                            if (curOption[0] == '!')
                            {
                                isNot     = true;
                                curOption = curOption.substr(1);
                            }

                            if (curOption.find(format + "[") != NOT_FOUND && curOption.find("]") != NOT_FOUND)
                            {
                                int pos           = 0;
                                VecStr formatInfo = GetOptionInfo(
                                    curOption, format, IDFileName, i + 1, groupAnimInfo, false, true);

                                if (formatInfo[2].find("AnimObject") != NOT_FOUND)
                                    ErrorMessage(1129, format, IDFileName, i + 1);

                                if (masterOptionPicked[stoi(formatInfo[1])][stoi(formatInfo[2])]
                                                      [formatInfo[3]])
                                {
                                    if (isNot)
                                        skip = true;
                                    else
                                        open = true;
                                }
                                else
                                {
                                    if (isNot)
                                        open = true;
                                    else
                                        skip = true;
                                }
                            }
                        }
                    }
                    else
                    {
                        ErrorMessage(1116, format, IDFileName, i + 1);
                    }
                }
            }
            else
            {
                skipTemplate = true;
            }
        }
        else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
        {
            if (newOpen) ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());

            newOpen        = true;
            size_t pos     = line.find("<!-- FOREACH ^") + 14;
            string checker = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));

            if (!otherAnimType[checker])
            {
                uniqueskip = true;

                if (IsConditionOpened[condition])
                {
                    if (!open)
                    {
                        string curOption = getOption(line);

                        if (curOption.find("[") == NOT_FOUND)
                        {
                            if (format != curOption) { skip = true; }
                            else
                            {
                                if (ignoreGroup)
                                    groupOrder = -1;
                                else
                                    order = -1;

                                open        = true;
                                multi       = true;
                                multiOption = format;
                                formatOpen  = true;
                            }
                        }
                        else
                        {
                            if (curOption[0] != '!')
                            {
                                if (curOption.find(format + "[") != NOT_FOUND
                                    && curOption.find("]") != NOT_FOUND)
                                {
                                    int pos           = 0;
                                    VecStr formatInfo = GetOptionInfo(
                                        curOption, format, IDFileName, i + 1, groupAnimInfo, true, true);

                                    open       = true;
                                    multi      = true;
                                    groupOrder = formatInfo[1].length() == 0 ? -1 : stoi(formatInfo[1]);
                                    order      = formatInfo[2].length() == 0 ? -1 : stoi(formatInfo[2]);
                                }
                            }
                        }
                    }
                    else
                    {
                        ErrorMessage(1115, format, IDFileName, i + 1);
                    }
                }
            }
            else
            {
                skipTemplate = true;
            }
        }
        else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
        {
            if (!newOpen) { ErrorMessage(1171, format, IDFileName, i + 1); }

            newOpen = false;

            if (skipTemplate) { skipTemplate = false; }
            else
            {
                uniqueskip = true;
            }
        }
        else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi && !skipTemplate
                 && ((newOpen && !skip) || !newOpen))
        {
            uniqueskip = true;
        }

        if (error) throw nemesis::exception();

        if (!uniqueskip && !skip && !freeze)
        {
            while (true)
            {
                if (skipTemplate)
                {
                    newFunctionLines.push_back(line);
                    break;
                }

                if (multi)
                {
                    tempstore.push_back(line);
                    break;
                }

                // set animation ID
                while (line.find("$%$", 0) != NOT_FOUND)
                {
                    line.replace(line.find("$%$"), 3, "0");
                }

                // multi choice selection
                if (line.find("$MC$", 0) != NOT_FOUND)
                {
                    AnimationUtility utility;
                    utility.originalCondition = line;
                    utility.currentProcess    = this;
                    utility.isExisting        = true;
                    utility.hasGroup          = hasGroup;

                    multiChoice(
                        line, IDFileName, masterOptionPicked, groupAnimInfo, i + 1, format, format, utility);
                }

                // compute numelements
                if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND
                    && line.find("</hkparam>") == NOT_FOUND)
                {
                    if (!isElement)
                    {
                        isElement       = true;
                        elementCatch    = true;
                        string templine = line.substr(0, line.find("<hkparam name=\"", 0));
                        openRange       = count(templine.begin(), templine.end(), '\t');
                    }
                    else
                    {
                        ErrorMessage(2019, format, IDFileName, i + 1);
                    }
                }
                else if (line.find("</hkparam>") != NOT_FOUND && isElement)
                {
                    string templine = line.substr(0, line.find("</hkparam>"));
                    __int64 range   = count(templine.begin(), templine.end(), '\t');

                    if (openRange == range)
                    {
                        string oldElement;

                        if (newFunctionLines[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
                        {
                            size_t position = newFunctionLines[elementLine].find("numelements=\"") + 13;
                            oldElement      = newFunctionLines[elementLine].substr(
                                position, newFunctionLines[elementLine].find("\">", position) - position);
                        }
                        else
                        {
                            oldElement = "$elements$";
                        }

                        if (oldElement != to_string(elementCount))
                        {
                            newFunctionLines[elementLine].replace(
                                newFunctionLines[elementLine].find(oldElement),
                                oldElement.length(),
                                to_string(elementCount));
                        }

                        isElement    = false;
                        elementCount = 0;
                        elementLine  = NULL;
                    }
                }

                if (isElement)
                {
                    string templine = line;

                    if (templine.find("<hkobject>") != NOT_FOUND)
                    {
                        templine      = templine.substr(0, templine.find("<hkobject>"));
                        __int64 range = count(templine.begin(), templine.end(), '\t');

                        if (range == openRange + 1) elementCount++;
                    }
                    else if (templine.find("\t\t\t#") != NOT_FOUND)
                    {
                        templine          = templine.substr(0, templine.find("#", 0));
                        __int64 reference = count(templine.begin(), templine.end(), '\t');

                        if (reference == openRange + 1)
                        {
                            __int64 number = count(line.begin(), line.end(), '#');
                            elementCount += number;
                        }
                    }
                }

                if (error) throw nemesis::exception();

                if (line.find("$") != NOT_FOUND && !hasGroup)
                { processing(line, IDFileName, curFunctionID, i + 1, eventid, variableid, 0); }

                size_t pos = line.find("(");

                if (pos != NOT_FOUND && line.find(")", pos) != NOT_FOUND)
                {
                    string templine = line.substr(pos);
                    int nextpos     = openEndBracket(templine, '(', ')', format, IDFileName, i + 1) + 1;
                    templine        = templine.substr(0, nextpos);
                    string oldline  = templine;
                    nemesis::calculate(templine, format, IDFileName, i + 1);

                    if (oldline != templine) line.replace(pos, oldline.length(), templine);
                }

                if (error) throw nemesis::exception();

                newFunctionLines.push_back(line);

                if (elementCatch) elementLine = newFunctionLines.size() - 1;

                break;
            }
        }

        if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
        {
            if (skip) { skip = false; }
            else
            {
                if (multi)
                {
                    int size;
                    int tempOrder;
                    int groupSize;

                    if (groupOrder == -2)
                    {
                        groupOrder = 0;
                        groupSize  = 1;
                    }
                    else if (groupOrder == -1)
                    {
                        groupOrder = 0;
                        groupSize  = int(groupAnimInfo.size());
                    }
                    else
                    {
                        groupSize = groupOrder + 1;
                    }

                    for (int groupMulti = groupOrder; groupMulti < groupSize; ++groupMulti)
                    {
                        if (order == -2)
                        {
                            tempOrder = 0;
                            size      = 1;
                        }
                        else if (order == -1)
                        {
                            tempOrder = 0;
                            size      = int(subFunctionIDs->grouplist[groupMulti]->singlelist.size());
                        }
                        else
                        {
                            tempOrder = order;
                            size      = order + 1;
                        }

                        for (int animMulti = tempOrder; animMulti < size;
                             ++animMulti) // each animation in a group
                        {
                            int repeatCount;

                            if (formatOpen)
                                repeatCount = 1;
                            else
                                repeatCount
                                    = groupAnimInfo[groupMulti][animMulti]->optionPickedCount[multiOption];

                            for (int optionMulti = 0; optionMulti < repeatCount;
                                 ++optionMulti) // option with different add-ons
                            {
                                bool skip2   = false;
                                bool freeze2 = false;

                                for (uint l = 0; l < tempstore.size(); ++l) // part lines need to add
                                {
                                    string curLine   = tempstore[l];
                                    size_t linecount = i + 1 + l - int(tempstore.size());
                                    bool uniqueskip2 = false;

                                    if (curLine.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
                                    {
                                        condition++;

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    size_t optionPosition
                                                        = curLine.find("<!-- CONDITION START ^") + 22;
                                                    string conditionLine
                                                        = curLine.substr(optionPosition,
                                                                         curLine.find("^ -->", optionPosition)
                                                                             - optionPosition);
                                                    AnimationUtility utility;
                                                    utility.originalCondition = conditionLine;
                                                    utility.currentProcess    = this;
                                                    utility.isExisting        = true;
                                                    utility.hasGroup          = hasGroup;
                                                    utility.groupMulti        = groupMulti;
                                                    utility.animMulti         = animMulti;
                                                    utility.optionMulti       = optionMulti;
                                                    utility.multiOption       = multiOption;

                                                    if (newCondition(conditionLine,
                                                                     IDFileName,
                                                                     masterOptionPicked,
                                                                     groupAnimInfo,
                                                                     linecount,
                                                                     format,
                                                                     format,
                                                                     utility))
                                                    {
                                                        skip2                        = false;
                                                        IsConditionOpened[condition] = true;
                                                    }
                                                    else
                                                    {
                                                        skip2 = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                skip2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION ^", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0)
                                        { ErrorMessage(1119, format, IDFileName, linecount); }

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    size_t optionPosition
                                                        = curLine.find("<!-- CONDITION ^") + 16;
                                                    string option = curLine.substr(
                                                        optionPosition,
                                                        curLine.find("^", optionPosition) - optionPosition);
                                                    AnimationUtility utility;
                                                    utility.originalCondition = option;
                                                    utility.currentProcess    = this;
                                                    utility.isExisting        = true;
                                                    utility.hasGroup          = hasGroup;
                                                    utility.groupMulti        = groupMulti;
                                                    utility.animMulti         = animMulti;
                                                    utility.optionMulti       = optionMulti;
                                                    utility.multiOption       = multiOption;

                                                    if (newCondition(option,
                                                                     IDFileName,
                                                                     masterOptionPicked,
                                                                     groupAnimInfo,
                                                                     linecount,
                                                                     format,
                                                                     format,
                                                                     utility))
                                                    {
                                                        skip2                        = false;
                                                        IsConditionOpened[condition] = true;
                                                    }
                                                    else
                                                    {
                                                        skip2 = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION -->", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0)
                                        { ErrorMessage(1119, format, IDFileName, linecount); }

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    skip2                        = false;
                                                    IsConditionOpened[condition] = true;
                                                }
                                                else
                                                {
                                                    skip2 = true;
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
                                    {
                                        uniqueskip2 = true;
                                    }

                                    if (error) throw nemesis::exception();

                                    if (!skip2 && !freeze2 && !uniqueskip2)
                                    {
                                        if (curLine.find("<hkparam name=\"") != NOT_FOUND
                                            && curLine.find("numelements=\"") != NOT_FOUND
                                            && curLine.find("</hkparam>") == NOT_FOUND
                                            && curLine.find("<!-- COMPUTE -->", 0) != NOT_FOUND)
                                        { ErrorMessage(1140, format, IDFileName, linecount); }

                                        if (curLine.find("\t\t\t#") != NOT_FOUND
                                            && (curLine.find("#" + format + "$") != NOT_FOUND
                                                || curLine.find("#" + format + "_group$") != NOT_FOUND
                                                || curLine.find("#" + format + "_master") != NOT_FOUND))
                                        {
                                            VecStr generator;
                                            StringSplit(curLine, generator);
                                            size_t nextpos = 0;

                                            for (auto& ID : generator)
                                            {
                                                if (ID.find("#" + format + "$") != NOT_FOUND
                                                    && multiOption == format)
                                                {
                                                    nextpos = curLine.find("#" + format + "$", nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = format + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->grouplist[groupMulti]
                                                                ->singlelist[animMulti]
                                                                ->format[curID]
                                                                .length()
                                                            == 0)
                                                        {
                                                            ErrorMessage(
                                                                2011, format, IDFileName, linecount, curID);
                                                        }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->grouplist[groupMulti]
                                                                            ->singlelist[animMulti]
                                                                            ->format[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2012, format, IDFileName, linecount, curID);
                                                    }
                                                }
                                                else if (ID.find("#" + format + "_group") != NOT_FOUND
                                                         && multiOption == format + "_group")
                                                {
                                                    nextpos = curLine.find("#" + multiOption, nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = multiOption + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->grouplist[groupMulti]
                                                                ->functionIDs[curID]
                                                                .length()
                                                            == 0)
                                                        {
                                                            ErrorMessage(
                                                                2011, format, IDFileName, linecount, curID);
                                                        }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->grouplist[groupMulti]
                                                                            ->functionIDs[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2012, format, IDFileName, linecount, curID);
                                                    }
                                                }
                                                else if (ID.find("#" + format + "_master") != NOT_FOUND
                                                         && multiOption == format + "_master")
                                                {
                                                    nextpos = curLine.find("#" + multiOption, nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = multiOption + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->functionIDs[curID].length() == 0)
                                                        {
                                                            ErrorMessage(
                                                                2011, format, IDFileName, linecount, curID);
                                                        }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->functionIDs[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2012, format, IDFileName, linecount, curID);
                                                    }
                                                }
                                            }
                                        }

                                        if (isElement)
                                        {
                                            string templine = curLine;

                                            if (templine.find("<hkobject>") != NOT_FOUND)
                                            {
                                                templine = templine.substr(0, templine.find("<hkobject>"));
                                                __int64 range = count(templine.begin(), templine.end(), '\t');

                                                if (range == openRange + 1) elementCount++;
                                            }
                                            else if (templine.find("\t\t\t#") != NOT_FOUND)
                                            {
                                                templine = templine.substr(0, templine.find("#", 0));
                                                __int64 reference
                                                    = count(templine.begin(), templine.end(), '\t');

                                                if (reference == openRange + 1)
                                                {
                                                    __int64 number
                                                        = count(curLine.begin(), curLine.end(), '#');
                                                    elementCount += number;
                                                }
                                            }
                                        }

                                        if (curLine.find("%") != NOT_FOUND)
                                        {
                                            __int64 counter = count(curLine.begin(), curLine.end(), '%');

                                            for (int p = 0; p < counter; ++p)
                                            {
                                                curLine.replace(curLine.find("%"), 1, to_string(l));
                                            }
                                        }

                                        // multi choice selection
                                        if (curLine.find("$MC$", 0) != NOT_FOUND)
                                        {
                                            AnimationUtility utility;
                                            utility.originalCondition = curLine;
                                            utility.currentProcess    = this;
                                            utility.isExisting        = true;
                                            utility.hasGroup          = hasGroup;
                                            utility.groupMulti        = groupMulti;
                                            utility.animMulti         = animMulti;
                                            utility.optionMulti       = optionMulti;
                                            utility.multiOption       = multiOption;

                                            multiChoice(curLine,
                                                        IDFileName,
                                                        masterOptionPicked,
                                                        groupAnimInfo,
                                                        linecount,
                                                        format,
                                                        format,
                                                        utility);
                                        }

                                        // set animation ID
                                        while (curLine.find("$%$", 0) != NOT_FOUND)
                                        {
                                            curLine.replace(curLine.find("$%$"), 3, "0");
                                        }

                                        if (error) throw nemesis::exception();

                                        if (curLine.find("$") != NOT_FOUND)
                                        {
                                            processing(curLine,
                                                       IDFileName,
                                                       curFunctionID,
                                                       linecount,
                                                       eventid,
                                                       variableid,
                                                       groupMulti,
                                                       optionMulti,
                                                       animMulti,
                                                       multiOption);
                                        }

                                        size_t pos = curLine.find("(");

                                        if (pos != NOT_FOUND && curLine.find(")", pos) != NOT_FOUND)
                                        {
                                            string templine = curLine.substr(pos);
                                            int nextpos
                                                = openEndBracket(
                                                      templine, '(', ')', format, IDFileName, linecount)
                                                  + 1;
                                            templine       = templine.substr(0, nextpos);
                                            string oldline = templine;
                                            nemesis::calculate(templine, format, IDFileName, linecount);

                                            if (oldline != templine)
                                                curLine.replace(pos, oldline.length(), templine);
                                        }

                                        if (curLine.find("<!-- NEW ^" + format, 0) == NOT_FOUND
                                            && curLine.find("<!-- FOREACH ^" + format, 0) == NOT_FOUND
                                            && curLine.find("<!-- CLOSE -->", 0) == NOT_FOUND)
                                            newFunctionLines.push_back(curLine);
                                    }

                                    if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0) ErrorMessage(1118, format, IDFileName, linecount);
                                        if (freeze2 && IsConditionOpened[condition]) freeze2 = false;

                                        skip2 = !isPassed(condition, IsConditionOpened);
                                        IsConditionOpened[condition] = false;
                                        condition--;
                                    }

                                    if (error) throw nemesis::exception();
                                }
                            }
                        }
                    }

                    groupOrder = -2;
                }

                tempstore.clear();
                tempstore.reserve(existingFunctionLines.size() / 10);
            }

            open       = false;
            multi      = false;
            formatOpen = false;
        }
        else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) { ErrorMessage(1118, format, IDFileName, i + 1); }

            if (!skipTemplate && ((newOpen && !skip) || !newOpen))
            {
                if (freeze && IsConditionOpened[condition]) freeze = false;

                skip                         = !isPassed(condition, IsConditionOpened);
                IsConditionOpened[condition] = false;
            }

            condition--;
        }

        if (error) return empty;
    }

    IsConditionOpened[0] = false;

    if (newOpen) ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());

    for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
    {
        if (it->second) ErrorMessage(1120, format, IDFileName);
    }

    if (newFunctionLines.size() != 0)
    {
        if (!newFunctionLines.back().empty()) { newFunctionLines.push_back(""); }
        else if (newFunctionLines.size() > 1)
        {
            while (newFunctionLines.back().empty())
            {
                if (!newFunctionLines[newFunctionLines.size() - 2].empty()) break;

                newFunctionLines.pop_back();
            }
        }
    }

    newFunctionLines.shrink_to_fit();
    return newFunctionLines;
}

void ExistingFunction::outPutExistingFunction(VecStr& existingFunctionLines,
                                              VecStr& newFunctionLines,
                                              bool isGroup,
                                              bool isMaster,
                                              bool ignoreGroup,
                                              string IDFileName,
                                              vector<vector<unordered_map<string, bool>>>& masterOptionPicked,
                                              unordered_map<string, bool>& otherAnimType,
                                              int order,
                                              int groupOrder,
                                              bool& isElement,
                                              int& elementLine,
                                              __int64& openRange,
                                              string& multiOption,
                                              int& elementCount,
                                              int curFunctionID,
                                              int curLine,
                                              uint scopeSize)
{
    bool multi             = false;
    bool skipTemplate      = false;
    bool open              = false;
    bool skip              = false;
    bool freeze            = false;
    bool formatOpen        = false;
    uint condition         = 0;
    uint scope             = 0;

    VecStr tempstore;
    unordered_map<int, bool> IsConditionOpened;
    IsConditionOpened[0] = true;

    for (uint i = curLine; i < scopeSize; ++i)
    {
        bool uniqueskip   = false;
        bool elementCatch = false;
        string line       = existingFunctionLines[i];

        if (!isMaster)
        {
            if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND && !multi)
            {
                condition++;

                if (!skipTemplate && ((scope > 0 && !skip) || scope == 0))
                {
                    if (!freeze)
                    {
                        if (!IsConditionOpened[condition])
                        {
                            if (isPassed(condition, IsConditionOpened))
                            {
                                size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
                                string conditionLine  = line.substr(
                                    optionPosition, line.find("^ -->", optionPosition) - optionPosition);

                                if (conditionLine.find("[") == NOT_FOUND)
                                {
                                    if (format != conditionLine) skip = true;
                                }
                                else
                                {
                                    AnimationUtility utility;
                                    utility.originalCondition = conditionLine;
                                    utility.currentProcess    = this;
                                    utility.isExisting        = true;
                                    utility.hasGroup          = isGroup;

                                    if (newCondition(conditionLine,
                                                     IDFileName,
                                                     masterOptionPicked,
                                                     groupAnimInfo,
                                                     i + 1,
                                                     format,
                                                     format,
                                                     utility))
                                    {
                                        skip                         = false;
                                        IsConditionOpened[condition] = true;
                                    }
                                    else
                                    {
                                        skip = true;
                                    }

                                    if (error) throw nemesis::exception();
                                }
                            }
                        }
                        else
                        {
                            skip = true;
                        }
                    }

                    uniqueskip = true;
                }
            }
            else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND && !multi)
            {
                if (condition == 0) ErrorMessage(1119, format, IDFileName, i + 1);

                if (!skipTemplate && ((scope > 0 && !skip) || scope == 0))
                {
                    if (!freeze)
                    {
                        if (!IsConditionOpened[condition])
                        {
                            if (isPassed(condition, IsConditionOpened))
                            {
                                size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
                                string option         = line.substr(optionPosition,
                                                            line.find("^", optionPosition) - optionPosition);

                                if (option.find("[") == NOT_FOUND)
                                {
                                    if (format != option) skip = true;
                                }
                                else
                                {
                                    AnimationUtility utility;
                                    utility.originalCondition = option;
                                    utility.currentProcess    = this;
                                    utility.isExisting        = true;
                                    utility.hasGroup          = isGroup;

                                    if (newCondition(option,
                                                     IDFileName,
                                                     masterOptionPicked,
                                                     groupAnimInfo,
                                                     i + 1,
                                                     format,
                                                     format,
                                                     utility))
                                    {
                                        skip                         = false;
                                        IsConditionOpened[condition] = true;
                                    }
                                    else
                                    {
                                        skip = true;
                                    }

                                    if (error) throw nemesis::exception();
                                }
                            }
                        }
                        else
                        {
                            skip   = true;
                            freeze = true;
                        }
                    }

                    uniqueskip = true;
                }
            }
            else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND && !multi)
            {
                if (condition == 0) ErrorMessage(1119, format, IDFileName, i + 1);

                if (!skipTemplate && ((scope > 0 && !skip) || scope == 0))
                {
                    if (!freeze)
                    {
                        if (!IsConditionOpened[condition])
                        {
                            if (isPassed(condition, IsConditionOpened))
                            {
                                skip                         = false;
                                IsConditionOpened[condition] = true;
                            }
                            else
                            {
                                skip = true;
                            }
                        }
                        else
                        {
                            skip   = true;
                            freeze = true;
                        }
                    }

                    uniqueskip = true;
                }
            }
            else if (line.find("<!-- FOREACH ^" + format + "^ -->", 0) != NOT_FOUND)
            {
                if (IsConditionOpened[condition])
                {
                    if (!isGroup)
                    {
                        if (!open)
                        {
                            if (groupAnimInfo.size() > 0)
                            {
                                if (ignoreGroup)
                                    groupOrder = -1;
                                else
                                    order = -1;

                                open        = true;
                                multi       = true;
                                multiOption = format;
                                formatOpen  = true;
                            }
                            else
                            {
                                skip = true;
                            }
                        }
                        else
                        {
                            ErrorMessage(1115, format, IDFileName, i + 1);
                        }
                    }
                    else
                    {
                        ErrorMessage(1161, format, IDFileName, i + 1);
                    }
                }

                ++scope;
                uniqueskip = true;
            }
            else if (line.find("<!-- FOREACH ^" + format + "_group^ -->", 0) != NOT_FOUND)
            {
                if (IsConditionOpened[condition])
                {
                    if (isGroup)
                    {
                        if (!open)
                        {
                            if (groupAnimInfo.size() > 0)
                            {
                                open        = true;
                                multi       = true;
                                groupOrder  = -1;
                                multiOption = format + "_group";
                                formatOpen  = true;
                            }
                            else
                            {
                                skip = true;
                            }
                        }
                        else
                        {
                            ErrorMessage(1115, format, IDFileName, i + 1);
                        }
                    }
                    else
                    {
                        ErrorMessage(1167, format, IDFileName, i + 1);
                    }
                }

                ++scope;
                uniqueskip = true;
            }
            else if (line.find("<!-- NEW ^" + format + "^ -->", 0) != NOT_FOUND
                     || line.find("<!-- NEW ^" + format + "_group^ -->", 0) != NOT_FOUND
                     || line.find("<!-- NEW ^" + format + "_master^ -->", 0) != NOT_FOUND)
            {
                ErrorMessage(1164, format, IDFileName, i + 1);
            }
            else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
            {
                ++scope;
                size_t pos     = line.find("<!-- NEW ^") + 10;
                string checker = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));

                if (!otherAnimType[checker])
                {
                    uniqueskip = true;

                    if (IsConditionOpened[condition])
                    {
                        if (!open)
                        {
                            string curOption = getOption(line);

                            if (curOption.find("[") == NOT_FOUND)
                            {
                                if (format != curOption)
                                    skip = true;
                                else
                                    open = true;
                            }
                            else
                            {
                                bool isNot = false;

                                if (curOption[0] == '!')
                                {
                                    isNot     = true;
                                    curOption = curOption.substr(1);
                                }

                                if (curOption.find(format + "[") != NOT_FOUND
                                    && curOption.find("]") != NOT_FOUND)
                                {
                                    int pos           = 0;
                                    VecStr formatInfo = GetOptionInfo(
                                        curOption, format, IDFileName, i + 1, groupAnimInfo, false, true);

                                    if (formatInfo[2].find("AnimObject") != NOT_FOUND)
                                        ErrorMessage(1129, format, IDFileName, i + 1);

                                    if (masterOptionPicked[stoi(formatInfo[1])][stoi(formatInfo[2])]
                                                          [formatInfo[3]])
                                    {
                                        if (isNot)
                                            skip = true;
                                        else
                                            open = true;
                                    }
                                    else
                                    {
                                        if (isNot)
                                            open = true;
                                        else
                                            skip = true;
                                    }
                                }
                            }
                        }
                        else
                        {
                            ErrorMessage(1116, format, IDFileName, i + 1);
                        }
                    }
                }
                else
                {
                    skipTemplate = true;
                }
            }
            else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
            {
                ++scope;
                size_t pos     = line.find("<!-- FOREACH ^") + 14;
                string checker = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));

                if (!otherAnimType[checker])
                {
                    uniqueskip = true;

                    if (IsConditionOpened[condition])
                    {
                        if (!open)
                        {
                            string curOption = getOption(line);

                            if (curOption.find("[") == NOT_FOUND)
                            {
                                if (format != curOption) { skip = true; }
                                else
                                {
                                    if (ignoreGroup)
                                        groupOrder = -1;
                                    else
                                        order = -1;

                                    open        = true;
                                    multi       = true;
                                    multiOption = format;
                                    formatOpen  = true;
                                }
                            }
                            else
                            {
                                bool isNot = false;

                                if (curOption[0] == '!')
                                {
                                    isNot     = true;
                                    curOption = curOption.substr(1);
                                }

                                if (curOption.find(format + "[") != NOT_FOUND
                                    && curOption.find("]") != NOT_FOUND)
                                {
                                    int pos           = 0;
                                    VecStr formatInfo = GetOptionInfo(
                                        curOption, format, IDFileName, i + 1, groupAnimInfo, true, true);

                                    if (isNot) { skip = true; }
                                    else
                                    {
                                        open       = true;
                                        multi      = true;
                                        groupOrder = formatInfo[1].length() == 0 ? -1 : stoi(formatInfo[1]);
                                        order      = formatInfo[2].length() == 0 ? -1 : stoi(formatInfo[2]);
                                    }
                                }
                            }
                        }
                        else
                        {
                            ErrorMessage(1115, format, IDFileName, i + 1);
                        }
                    }
                }
                else
                {
                    skipTemplate = true;
                }
            }
            else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
            {
                if (scope == 0) ErrorMessage(1171, format, IDFileName, i + 1);

                --scope;

                if (skipTemplate)
                    skipTemplate = false;
                else
                    uniqueskip = true;
            }
            else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi && !skipTemplate
                     && ((scope > 0 && !skip) || scope == 0))
            {
                uniqueskip = true;
            }
        }
        else if (line.find("<!-- FOREACH ^" + format + "_master^ -->", 0) != NOT_FOUND)
        {
            if (IsConditionOpened[condition])
            {
                if (isMaster)
                {
                    if (!open)
                    {
                        if (groupAnimInfo.size() > 0)
                        {
                            open        = true;
                            multi       = true;
                            groupOrder  = -2;
                            multiOption = format + "_master";
                            formatOpen  = true;
                        }
                        else
                        {
                            skip = true;
                        }
                    }
                    else
                    {
                        ErrorMessage(1115, format, IDFileName, i + 1);
                    }
                }
                else
                {
                    ErrorMessage(1162, format, IDFileName, i + 1);
                }
            }

            ++scope;
            uniqueskip = true;
        }
        else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
        {
            ++scope;
            size_t pos     = line.find("<!-- FOREACH ^") + 14;
            string checker = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));

            string templine = line;
            pos             = templine.find("[");
            VecStr optionInfo;
            optionInfo.push_back(templine.substr(0, pos));
            templine = templine.substr(templine.find("[", pos) + 1);

            while (true)
            {
                pos = templine.find("]");
                optionInfo.push_back(templine.substr(0, pos));
                size_t optionLength = optionInfo.back().length() + 1;

                if (templine.length() <= optionLength || templine[optionLength] != '[') break;

                templine = templine.substr(templine.find("[") + 1);
            }

            if (optionInfo.size() == 3 || otherAnimType[checker]) { skipTemplate = true; }
        }
        else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
        {
            if (scope == 0) ErrorMessage(1171, format, IDFileName, i + 1);

            --scope;

            if (skipTemplate)
                skipTemplate = false;
            else
                uniqueskip = true;
        }

        if (error) throw nemesis::exception();

        if (!uniqueskip && !skip && !freeze)
        {
            while (true)
            {
                if (skipTemplate)
                {
                    newFunctionLines.push_back(line);
                    break;
                }

                if (multi)
                {
                    tempstore.push_back(line);
                    break;
                }

                // set animation ID
                while (line.find("$%$", 0) != NOT_FOUND)
                {
                    line.replace(line.find("$%$"), 3, "0");
                }

                // multi choice selection
                if (line.find("$MC$", 0) != NOT_FOUND)
                {
                    AnimationUtility utility;
                    utility.originalCondition = line;
                    utility.currentProcess    = this;
                    utility.isExisting        = true;
                    utility.hasGroup          = isGroup;

                    multiChoice(
                        line, IDFileName, masterOptionPicked, groupAnimInfo, i + 1, format, format, utility);
                }

                // compute numelements
                if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND
                    && line.find("</hkparam>") == NOT_FOUND)
                {
                    if (!isElement)
                    {
                        isElement       = true;
                        elementCatch    = true;
                        string templine = line.substr(0, line.find("<hkparam name=\"", 0));
                        openRange       = count(templine.begin(), templine.end(), '\t');
                    }
                    else
                    {
                        ErrorMessage(2019, format, IDFileName, i + 1);
                    }
                }
                else if (line.find("</hkparam>") != NOT_FOUND && isElement)
                {
                    string templine = line.substr(0, line.find("</hkparam>"));
                    __int64 range   = count(templine.begin(), templine.end(), '\t');

                    if (openRange == range)
                    {
                        string oldElement;

                        if (newFunctionLines[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
                        {
                            size_t position = newFunctionLines[elementLine].find("numelements=\"") + 13;
                            oldElement      = newFunctionLines[elementLine].substr(
                                position, newFunctionLines[elementLine].find("\">", position) - position);
                        }
                        else
                        {
                            oldElement = "$elements$";
                        }

                        if (oldElement != to_string(elementCount))
                        {
                            newFunctionLines[elementLine].replace(
                                newFunctionLines[elementLine].find(oldElement),
                                oldElement.length(),
                                to_string(elementCount));
                        }

                        isElement    = false;
                        elementCount = 0;
                        elementLine  = NULL;
                    }
                }

                if (isElement)
                {
                    string templine = line;

                    if (templine.find("<hkobject>") != NOT_FOUND)
                    {
                        templine      = templine.substr(0, templine.find("<hkobject>"));
                        __int64 range = count(templine.begin(), templine.end(), '\t');

                        if (range == openRange + 1) elementCount++;
                    }
                    else if (templine.find("\t\t\t#") != NOT_FOUND)
                    {
                        templine          = templine.substr(0, templine.find("#", 0));
                        __int64 reference = count(templine.begin(), templine.end(), '\t');

                        if (reference == openRange + 1)
                        {
                            __int64 number = count(line.begin(), line.end(), '#');
                            elementCount += static_cast<int>(number);
                        }
                    }
                }

                if (error) throw nemesis::exception();

                if (line.find("$") != NOT_FOUND && !isGroup)
                { processing(line, IDFileName, curFunctionID, i + 1, eventid, variableid, 0); }

                size_t pos = line.find("(");

                if (pos != NOT_FOUND && line.find(")", pos) != NOT_FOUND)
                {
                    string templine = line.substr(pos);
                    int nextpos     = openEndBracket(templine, '(', ')', format, IDFileName, i + 1) + 1;
                    templine        = templine.substr(0, nextpos);
                    string oldline  = templine;
                    nemesis::calculate(templine, format, IDFileName, i + 1);

                    if (oldline != templine) line.replace(pos, oldline.length(), templine);
                }

                if (error) throw nemesis::exception();

                newFunctionLines.push_back(line);

                if (elementCatch) elementLine = newFunctionLines.size() - 1;

                break;
            }
        }

        if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND && scope == 0)
        {
            if (skip) { skip = false; }
            else if (!isMaster)
            {
                if (multi)
                {
                    int size;
                    int tempOrder;
                    int groupSize;

                    if (groupOrder == -2)
                    {
                        groupOrder = 0;
                        groupSize  = 1;
                    }
                    else if (groupOrder == -1)
                    {
                        groupOrder = 0;
                        groupSize  = int(groupAnimInfo.size());
                    }
                    else
                    {
                        groupSize = groupOrder + 1;
                    }

                    for (int groupMulti = groupOrder; groupMulti < groupSize; ++groupMulti)
                    {
                        if (order == -2)
                        {
                            tempOrder = 0;
                            size      = 1;
                        }
                        else if (order == -1)
                        {
                            tempOrder = 0;
                            size      = int(subFunctionIDs->grouplist[groupMulti]->singlelist.size());
                        }
                        else
                        {
                            tempOrder = order;
                            size      = order + 1;
                        }

                        for (int animMulti = tempOrder; animMulti < size;
                             ++animMulti) // each animation in a group
                        {
                            int repeatCount;

                            if (formatOpen)
                                repeatCount = 1;
                            else
                                repeatCount
                                    = groupAnimInfo[groupMulti][animMulti]->optionPickedCount[multiOption];

                            for (int optionMulti = 0; optionMulti < repeatCount;
                                 ++optionMulti) // option with different add-ons
                            {
                                bool skip2   = false;
                                bool freeze2 = false;

                                for (uint l = 0; l < tempstore.size(); ++l) // part lines need to add
                                {
                                    string curLine   = tempstore[l];
                                    size_t linecount = i + 1 + l - int(tempstore.size());
                                    bool uniqueskip2 = false;

                                    if (curLine.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
                                    {
                                        condition++;

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    size_t optionPosition
                                                        = curLine.find("<!-- CONDITION START ^") + 22;
                                                    string conditionLine
                                                        = curLine.substr(optionPosition,
                                                                         curLine.find("^ -->", optionPosition)
                                                                             - optionPosition);
                                                    AnimationUtility utility;
                                                    utility.originalCondition = conditionLine;
                                                    utility.currentProcess    = this;
                                                    utility.isExisting        = true;
                                                    utility.hasGroup          = isGroup;
                                                    utility.groupMulti        = groupMulti;
                                                    utility.animMulti         = animMulti;
                                                    utility.optionMulti       = optionMulti;
                                                    utility.multiOption       = multiOption;

                                                    if (newCondition(conditionLine,
                                                                     IDFileName,
                                                                     masterOptionPicked,
                                                                     groupAnimInfo,
                                                                     linecount,
                                                                     format,
                                                                     format,
                                                                     utility))
                                                    {
                                                        skip2                        = false;
                                                        IsConditionOpened[condition] = true;
                                                    }
                                                    else
                                                    {
                                                        skip2 = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                skip2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION ^", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0) ErrorMessage(1119, format, IDFileName, linecount);

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    size_t optionPosition
                                                        = curLine.find("<!-- CONDITION ^") + 16;
                                                    string option = curLine.substr(
                                                        optionPosition,
                                                        curLine.find("^", optionPosition) - optionPosition);
                                                    AnimationUtility utility;
                                                    utility.originalCondition = option;
                                                    utility.currentProcess    = this;
                                                    utility.isExisting        = true;
                                                    utility.hasGroup          = isGroup;
                                                    utility.groupMulti        = groupMulti;
                                                    utility.animMulti         = animMulti;
                                                    utility.optionMulti       = optionMulti;
                                                    utility.multiOption       = multiOption;

                                                    if (newCondition(option,
                                                                     IDFileName,
                                                                     masterOptionPicked,
                                                                     groupAnimInfo,
                                                                     linecount,
                                                                     format,
                                                                     format,
                                                                     utility))
                                                    {
                                                        skip2                        = false;
                                                        IsConditionOpened[condition] = true;
                                                    }
                                                    else
                                                    {
                                                        skip2 = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION -->", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0) ErrorMessage(1119, format, IDFileName, linecount);

                                        if (!freeze2)
                                        {
                                            if (!IsConditionOpened[condition])
                                            {
                                                if (isPassed(condition, IsConditionOpened))
                                                {
                                                    skip2                        = false;
                                                    IsConditionOpened[condition] = true;
                                                }
                                                else
                                                {
                                                    skip2 = true;
                                                }
                                            }
                                            else
                                            {
                                                skip2   = true;
                                                freeze2 = true;
                                            }
                                        }

                                        uniqueskip2 = true;
                                    }
                                    else if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
                                    {
                                        uniqueskip2 = true;
                                    }

                                    if (error) throw nemesis::exception();

                                    if (!skip2 && !freeze2 && !uniqueskip2)
                                    {
                                        if (curLine.find("<hkparam name=\"") != NOT_FOUND
                                            && curLine.find("numelements=\"") != NOT_FOUND
                                            && curLine.find("</hkparam>") == NOT_FOUND
                                            && curLine.find("<!-- COMPUTE -->", 0) != NOT_FOUND)
                                        { ErrorMessage(1140, format, IDFileName, linecount); }

                                        if (curLine.find("\t\t\t#") != NOT_FOUND
                                            && (curLine.find("#" + format + "$") != NOT_FOUND
                                                || curLine.find("#" + format + "_group$") != NOT_FOUND
                                                || curLine.find("#" + format + "_master") != NOT_FOUND))
                                        {
                                            VecStr generator;
                                            StringSplit(curLine, generator);
                                            size_t nextpos = 0;

                                            for (auto& ID : generator)
                                            {
                                                if (ID.find("#" + format + "$") != NOT_FOUND
                                                    && multiOption == format)
                                                {
                                                    nextpos = curLine.find("#" + format + "$", nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = format + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->grouplist[groupMulti]
                                                                ->singlelist[animMulti]
                                                                ->format[curID]
                                                                .length()
                                                            == 0)
                                                        {
                                                            ErrorMessage(
                                                                2011, format, IDFileName, linecount, curID);
                                                        }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->grouplist[groupMulti]
                                                                            ->singlelist[animMulti]
                                                                            ->format[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2012, format, IDFileName, linecount, curID);
                                                    }
                                                }
                                                else if (ID.find("#" + format + "_group") != NOT_FOUND
                                                         && multiOption == format + "_group")
                                                {
                                                    nextpos = curLine.find("#" + multiOption, nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = multiOption + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->grouplist[groupMulti]
                                                                ->functionIDs[curID]
                                                                .length()
                                                            == 0)
                                                        {
                                                            ErrorMessage(
                                                                2011, format, IDFileName, linecount, curID);
                                                        }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->grouplist[groupMulti]
                                                                            ->functionIDs[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2012, format, IDFileName, linecount, curID);
                                                    }
                                                }
                                                else if (ID.find("#" + format + "_master") != NOT_FOUND
                                                         && multiOption == format + "_master")
                                                {
                                                    nextpos = curLine.find("#" + multiOption, nextpos) + 1;
                                                    string tempID = curLine.substr(nextpos);
                                                    string curID  = nemesis::regex_replace(
                                                        string(tempID),
                                                        nemesis::regex("[^0-9]*([0-9]+).*"),
                                                        string("\\1"));
                                                    curID = multiOption + "$" + curID;

                                                    if (tempID.find(curID, 0) != NOT_FOUND
                                                        && nextpos == curLine.find(curID))
                                                    {
                                                        if (subFunctionIDs->functionIDs[curID].length() == 0)
                                                        {
                                                            ErrorMessage(
                                                                2011, format, IDFileName, linecount, curID);
                                                        }

                                                        curLine.replace(nextpos,
                                                                        curID.length(),
                                                                        subFunctionIDs->functionIDs[curID]);
                                                    }
                                                    else
                                                    {
                                                        ErrorMessage(
                                                            2012, format, IDFileName, linecount, curID);
                                                    }
                                                }
                                            }
                                        }

                                        if (isElement)
                                        {
                                            string templine = curLine;

                                            if (templine.find("<hkobject>") != NOT_FOUND)
                                            {
                                                templine = templine.substr(0, templine.find("<hkobject>"));
                                                __int64 range = count(templine.begin(), templine.end(), '\t');

                                                if (range == openRange + 1) elementCount++;
                                            }
                                            else if (templine.find("\t\t\t#") != NOT_FOUND)
                                            {
                                                templine = templine.substr(0, templine.find("#", 0));
                                                __int64 reference
                                                    = count(templine.begin(), templine.end(), '\t');

                                                if (reference == openRange + 1)
                                                {
                                                    __int64 number
                                                        = count(curLine.begin(), curLine.end(), '#');
                                                    elementCount += static_cast<int>(number);
                                                }
                                            }
                                        }

                                        if (curLine.find("%") != NOT_FOUND)
                                        {
                                            __int64 counter = count(curLine.begin(), curLine.end(), '%');

                                            for (int p = 0; p < counter; ++p)
                                            {
                                                curLine.replace(curLine.find("%"), 1, to_string(l));
                                            }
                                        }

                                        // multi choice selection
                                        if (curLine.find("$MC$", 0) != NOT_FOUND)
                                        {
                                            AnimationUtility utility;
                                            utility.originalCondition = curLine;
                                            utility.currentProcess    = this;
                                            utility.isExisting        = true;
                                            utility.hasGroup          = isGroup;
                                            utility.groupMulti        = groupMulti;
                                            utility.animMulti         = animMulti;
                                            utility.optionMulti       = optionMulti;
                                            utility.multiOption       = multiOption;

                                            multiChoice(curLine,
                                                        IDFileName,
                                                        masterOptionPicked,
                                                        groupAnimInfo,
                                                        linecount,
                                                        format,
                                                        format,
                                                        utility);
                                        }

                                        // set animation ID
                                        while (curLine.find("$%$", 0) != NOT_FOUND)
                                        {
                                            curLine.replace(curLine.find("$%$"), 3, "0");
                                        }

                                        if (error) throw nemesis::exception();

                                        if (curLine.find("$") != NOT_FOUND)
                                        {
                                            processing(curLine,
                                                       IDFileName,
                                                       curFunctionID,
                                                       linecount,
                                                       eventid,
                                                       variableid,
                                                       groupMulti,
                                                       optionMulti,
                                                       animMulti,
                                                       multiOption);
                                        }

                                        size_t pos = curLine.find("(");

                                        if (pos != NOT_FOUND && curLine.find(")", pos) != NOT_FOUND)
                                        {
                                            string templine = curLine.substr(pos);
                                            int nextpos
                                                = openEndBracket(
                                                      templine, '(', ')', format, IDFileName, linecount)
                                                  + 1;
                                            templine       = templine.substr(0, nextpos);
                                            string oldline = templine;
                                            nemesis::calculate(templine, format, IDFileName, linecount);

                                            if (oldline != templine)
                                                curLine.replace(pos, oldline.length(), templine);
                                        }

                                        if (curLine.find("<!-- NEW ^" + format, 0) == NOT_FOUND
                                            && curLine.find("<!-- FOREACH ^" + format, 0) == NOT_FOUND
                                            && curLine.find("<!-- CLOSE -->", 0) == NOT_FOUND)
                                            newFunctionLines.push_back(curLine);
                                    }

                                    if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
                                    {
                                        if (condition == 0) ErrorMessage(1118, format, IDFileName, linecount);
                                        if (freeze2 && IsConditionOpened[condition]) freeze2 = false;

                                        skip2 = !isPassed(condition, IsConditionOpened);
                                        IsConditionOpened[condition] = false;
                                        condition--;
                                    }

                                    if (error) throw nemesis::exception();
                                }
                            }
                        }
                    }

                    groupOrder = -2;
                }

                tempstore.clear();
                tempstore.reserve(existingFunctionLines.size() / 10);
            }
            else
            {}

            open       = false;
            multi      = false;
            formatOpen = false;
        }
        else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi)
        {
            if (condition == 0) ErrorMessage(1118, format, IDFileName, i + 1);

            if (!skipTemplate && ((scope > 0 && !skip) || scope == 0))
            {
                if (freeze && IsConditionOpened[condition]) freeze = false;

                skip                         = !isPassed(condition, IsConditionOpened);
                IsConditionOpened[condition] = false;
            }

            condition--;
        }

        if (error) return;
    }

    if (scope != 0) ErrorMessage(1116, format, IDFileName, scopeSize);

    for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
    {
        if (it->second) ErrorMessage(1120, format, IDFileName);
    }
}

void GroupTemplate::stateReplacer(
    string& line, string filename, string statenum, int ID, int stateID, int linecount, int groupMulti)
{
    int count = sameWordCount(line, "$(S" + statenum + "+");

    for (int i = 0; i < count; ++i)
    {
        string number = nemesis::regex_replace(
            string(line.substr(line.find("$(S" + statenum + "+") + statenum.length() + 4)),
            nemesis::regex("[^0-9]*([0-9]+).*"),
            string("\\1"));
        string state = "$(S" + statenum + "+" + number + ")$";

        if (line.find(state, 0) != NOT_FOUND)
        {
            size_t stateposition = line.find(state, 0);

            if (state == line.substr(stateposition, line.find(")$", stateposition) - stateposition + 2))
            {
                size_t stateLength = state.length();
                state              = state.substr(1, state.length() - 2);
                state.replace(1, 1 + statenum.length(), to_string(stateID));
                nemesis::calculate(state, format, filename, linecount);
                int stateint = stoi(state);

                if (groupMulti > -1)
                    subFunctionIDs->grouplist[groupMulti]->functionIDs["(S" + statenum + "+" + number + ")"]
                        = state; // group
                else
                    subFunctionIDs->functionIDs["(S" + statenum + "+" + number + ")"] = state; // master

                line.replace(stateposition, stateLength, state);
            }
            else
            {
                ErrorMessage(1137, format, filename, linecount, state);
            }
        }

        if (error) throw nemesis::exception();
    }
}

void GroupTemplate::processing(string& line,
                               string filename,
                               string masterFormat,
                               int linecount,
                               ID eventid,
                               ID variableid,
                               int groupMulti,
                               int optionMulti,
                               int animMulti,
                               string multiOption)
{
    __int64 counter = count(line.begin(), line.end(), '$') / 2;
    size_t curPos   = -1;

    for (int i = 0; i < counter; ++i)
    {
        bool isChange    = false;
        curPos           = line.find("$", curPos + 1);
        string change    = line.substr(curPos, line.find("$", curPos + 1) - curPos + 1);
        string oldChange = change;
        change           = change.substr(1, change.length() - 2);
        curPos           = line.find("$", curPos + 1);

        while (true)
        {
            if (change.find("(") != NOT_FOUND && change.find("L", change.find("(")) != NOT_FOUND
                && change.find(")", change.find("(")) != NOT_FOUND)
            {
                __int64 maths = count(change.begin(), change.end(), '(');

                if (maths != 0 && maths == count(change.begin(), change.end(), ')'))
                {
                    size_t nextpos = change.find("(");

                    for (__int64 j = 0; j < maths; ++j)
                    {
                        string equation = change.substr(nextpos, change.find(")", nextpos) - 1);
                        string number   = "";
                        string ID       = "";

                        if (equation.find("(S", 0) != NOT_FOUND)
                        {
                            ID = nemesis::regex_replace(
                                string(equation), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                            if (change.find("(S" + ID + "+") == NOT_FOUND) ID = "";

                            number = nemesis::regex_replace(string(equation.substr(3 + ID.length())),
                                                          nemesis::regex("[^0-9]*([0-9]+).*"),
                                                          string("\\1"));
                        }

                        if (equation != "(S" + ID + "+" + number + ")")
                        {
                            size_t equationLength = equation.length();

                            if (equation.find("L") != NOT_FOUND)
                            {
                                __int64 maths2 = count(change.begin(), change.end(), 'L');

                                for (__int64 k = 0; k < maths2; ++k)
                                {
                                    equation.replace(
                                        equation.find("L"), 1, to_string(groupAnimInfo.size() - 1));
                                }
                            }

                            nemesis::calculate(equation, format, filename, linecount);

                            if (stoi(equation) > int(groupAnimInfo.size() - 1) || stoi(equation) < 0)
                            {
                                ErrorMessage(1148, format, filename, linecount, change);
                                // ErrorMessage(1155, groupAnimInfo.size() - 1, format, filename, linecount, change);		REPLACED
                            }

                            change.replace(nextpos, equationLength, equation);
                            isChange = true;
                        }

                        nextpos = change.find("(", nextpos + 1);
                    }
                }
            }

            if (change.find("[FilePath]") != NOT_FOUND)
            {
                if (change.find(masterFormat + "[][FilePath]", 0) != NOT_FOUND)
                {
                    change.replace(
                        change.find(masterFormat + "[][FilePath]"),
                        13 + masterFormat.length(),
                        subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FilePath"]);
                    isChange = true;
                }

                if (change.find(masterFormat + "[F][FilePath]", 0) != NOT_FOUND)
                {
                    change.replace(change.find(masterFormat + "[F][FilePath]"),
                                   13 + masterFormat.length(),
                                   subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FilePath"]);
                    isChange = true;
                }

                if (change.find(masterFormat + "[N][FilePath]", 0) != NOT_FOUND)
                    ErrorMessage(1056, format, filename, linecount, line);
                if (change.find(masterFormat + "[B][FilePath]", 0) != NOT_FOUND)
                    ErrorMessage(1056, format, filename, linecount, line);

                if (change.find(masterFormat + "[L][FilePath]", 0) != NOT_FOUND)
                {
                    change.replace(
                        change.find(masterFormat + "[L][FilePath]"),
                        13 + masterFormat.length(),
                        subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FilePath"]);
                    isChange = true;
                }

                if (change.find(masterFormat + "[", 0) != NOT_FOUND)
                {
                    string number
                        = nemesis::regex_replace(string(change.substr(change.find(masterFormat + "[", 0))),
                                               nemesis::regex("[^0-9]*([0-9]+).*"),
                                               string("\\1"));

                    if (change.find(masterFormat + "[" + number + "][FilePath]", 0) != NOT_FOUND)
                    {
                        if (static_cast<uint>(stoi(number)) >= groupAnimInfo[groupMulti].size())
                        {
                            change.replace(change.find(masterFormat + "[" + number + "][FilePath]"),
                                           8 + masterFormat.length() + number.length(),
                                           subFunctionIDs->grouplist[groupMulti]
                                               ->singlelist[stoi(number)]
                                               ->format["FilePath"]);
                            isChange = true;
                        }
                        else
                        {
                            ErrorMessage(1148, format, filename, linecount, change);
                        }
                    }
                }

                if (error) throw nemesis::exception();
            }

            if (change.find("[FileName]") != NOT_FOUND)
            {
                if (change.find(masterFormat + "[][FileName]", 0) != NOT_FOUND)
                {
                    if (multiOption != masterFormat || animMulti == -1)
                        ErrorMessage(1052, format, filename, linecount, line);

                    change.replace(
                        change.find(masterFormat + "[][FileName]"),
                        12 + masterFormat.length(),
                        subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FileName"]);
                }

                if (change.find(masterFormat + "[F][FileName]", 0) != NOT_FOUND)
                {
                    change.replace(change.find(masterFormat + "[F][FileName]"),
                                   13 + masterFormat.length(),
                                   subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FileName"]);
                    isChange = true;
                }

                if (change.find(masterFormat + "[N][FileName]", 0) != NOT_FOUND)
                    ErrorMessage(1056, format, filename, linecount, line);
                if (change.find(masterFormat + "[B][FileName]", 0) != NOT_FOUND)
                    ErrorMessage(1056, format, filename, linecount, line);

                if (change.find(masterFormat + "[L][FileName]", 0) != NOT_FOUND)
                {
                    change.replace(
                        change.find(masterFormat + "[L][FileName]"),
                        13 + masterFormat.length(),
                        subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FileName"]);
                    isChange = true;
                }

                if (change.find(masterFormat + "[", 0) != NOT_FOUND)
                {
                    string number
                        = nemesis::regex_replace(string(change.substr(change.find(masterFormat + "[", 0))),
                                               nemesis::regex("[^0-9]*([0-9]+).*"),
                                               string("\\1"));

                    if (change.find(masterFormat + "[" + number + "][FileName]", 0) != NOT_FOUND)
                    {
                        if (static_cast<uint>(stoi(number)) >= groupAnimInfo[groupMulti].size())
                        {
                            change.replace(change.find(masterFormat + "[" + number + "][FileName]"),
                                           8 + masterFormat.length() + number.length(),
                                           subFunctionIDs->grouplist[groupMulti]
                                               ->singlelist[stoi(number)]
                                               ->format["FileName"]);
                            isChange = true;
                        }
                        else
                        {
                            ErrorMessage(1148, format, filename, linecount, change);
                        }
                    }
                }

                if (error) throw nemesis::exception();
            }

            if (change.find("main_anim_event") != NOT_FOUND)
            {
                size_t pos = change.find("{" + masterFormat + "[][main_anim_event]}", 0);

                if (pos != NOT_FOUND)
                {
                    if (multiOption != masterFormat || animMulti == -1)
                        ErrorMessage(1052, format, filename, linecount, line);

                    change.replace(pos,
                                   21 + masterFormat.length(),
                                   subFunctionIDs->grouplist[groupMulti]
                                       ->singlelist[animMulti]
                                       ->format["main_anim_event"]);
                    isChange = true;
                }
                else
                {
                    pos = change.find(masterFormat + "[][main_anim_event]", 0);

                    if (pos != NOT_FOUND)
                    {
                        if (multiOption != masterFormat || animMulti == -1)
                            ErrorMessage(1052, format, filename, linecount, line);

                        change.replace(pos,
                                       19 + masterFormat.length(),
                                       subFunctionIDs->grouplist[groupMulti]
                                           ->singlelist[animMulti]
                                           ->format["main_anim_event"]);
                        isChange = true;
                    }
                }

                pos = change.find("{" + masterFormat + "[F][main_anim_event]}", 0);

                if (pos != NOT_FOUND)
                {
                    change.replace(
                        pos,
                        22 + masterFormat.length(),
                        subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["main_anim_event"]);
                    isChange = true;
                }
                else
                {
                    pos = change.find(masterFormat + "[F][main_anim_event]", 0);

                    if (pos != NOT_FOUND)
                    {
                        change.replace(
                            pos,
                            20 + masterFormat.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["main_anim_event"]);
                        isChange = true;
                    }
                }

                if (change.find(masterFormat + "[N][main_anim_event]", 0) != NOT_FOUND)
                    ErrorMessage(1056, format, filename, linecount, line);
                if (change.find(masterFormat + "[B][main_anim_event]", 0) != NOT_FOUND)
                    ErrorMessage(1056, format, filename, linecount, line);

                pos = change.find("{" + masterFormat + "[L][main_anim_event]}", 0);

                if (pos != NOT_FOUND)
                {
                    change.replace(
                        pos,
                        22 + masterFormat.length(),
                        subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["main_anim_event"]);
                    isChange = true;
                }
                else
                {
                    pos = change.find(masterFormat + "[L][main_anim_event]", 0);

                    if (pos != NOT_FOUND)
                    {
                        change.replace(pos,
                                       20 + masterFormat.length(),
                                       subFunctionIDs->grouplist[groupMulti]
                                           ->singlelist.back()
                                           ->format["main_anim_event"]);
                        isChange = true;
                    }
                }

                if (error) throw nemesis::exception();

                if (change.find(masterFormat + "[", 0) != NOT_FOUND)
                {
                    string number = nemesis::regex_replace(
                        string(change),
                        nemesis::regex(masterFormat + "\\[([0-9]+)\\]\\[main_anim_event\\].*"),
                        string("\\1"));

                    if (number != change)
                    {
                        if (uint(stoi(number)) >= groupAnimInfo[groupMulti].size())
                        {
                            pos = change.find("{" + masterFormat + "[" + number + "][main_anim_event]}", 0);

                            if (pos != NOT_FOUND)
                            {
                                change.replace(pos,
                                               21 + masterFormat.length() + number.length(),
                                               subFunctionIDs->grouplist[groupMulti]
                                                   ->singlelist[stoi(number)]
                                                   ->format["main_anim_event"]);
                                isChange = true;
                            }
                            else
                            {
                                pos = change.find(masterFormat + "[" + number + "][main_anim_event]", 0);

                                if (pos != NOT_FOUND)
                                {
                                    change.replace(pos,
                                                   19 + masterFormat.length() + number.length(),
                                                   subFunctionIDs->grouplist[groupMulti]
                                                       ->singlelist[stoi(number)]
                                                       ->format["main_anim_event"]);
                                    isChange = true;
                                }
                            }
                        }
                        else
                        {
                            ErrorMessage(1148, format, filename, linecount, change);
                        }
                    }
                }
            }

            size_t pos      = change.find(masterFormat + "[", 0);
            bool innerError = false;

            if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
            {
                formatGroupReplace(change,
                                   change,
                                   0,
                                   filename,
                                   masterFormat,
                                   subFunctionIDs,
                                   groupAnimInfo,
                                   linecount,
                                   groupMulti,
                                   optionMulti,
                                   animMulti,
                                   multiOption,
                                   innerError);
                isChange = true;

                if (innerError) ErrorMessage(2014, format, filename, linecount);
                if (error) throw nemesis::exception();
            }

            pos = change.find(masterFormat + "_group[", 0);

            if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
            {
                formatGroupReplace(change,
                                   change,
                                   0,
                                   filename,
                                   masterFormat,
                                   subFunctionIDs,
                                   groupAnimInfo,
                                   linecount,
                                   groupMulti,
                                   optionMulti,
                                   animMulti,
                                   multiOption,
                                   innerError);
                isChange = true;

                if (innerError) ErrorMessage(2014, format, filename, linecount);
                if (error) throw nemesis::exception();
            }

            if (multiOption == masterFormat)
            {
                string tempchange = change;
                pos               = change.find("[");

                if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
                {
                    formatGroupReplace(change,
                                       change,
                                       0,
                                       filename,
                                       masterFormat,
                                       subFunctionIDs,
                                       groupAnimInfo,
                                       linecount,
                                       groupMulti,
                                       optionMulti,
                                       animMulti,
                                       multiOption,
                                       innerError);

                    if (innerError) ErrorMessage(2014, format, filename, linecount);
                    if (error) throw nemesis::exception();
                }
                else if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change].length()
                         > 0)
                {
                    change = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change];
                }

                if (change != tempchange) isChange = true;
            }

            if (error) throw nemesis::exception();

            size_t position = change.find("eventID[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                eventIDReplacer(change, format, filename, eventid, zeroEvent, linecount);
                isChange = true;

                if (error) throw nemesis::exception();
            }

            position = change.find("variableID[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                variableIDReplacer(change, format, filename, variableid, zeroVariable, linecount);
                isChange = true;

                if (error) throw nemesis::exception();
            }

            position = change.find("crc32[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                CRC32Replacer(change, format, filename, linecount);
                isChange = true;
            }

            position = change.find("import[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                size_t nextpos          = change.find("import[");
                string importer         = change.substr(nextpos, change.find_last_of("]") - nextpos + 1);
                __int64 bracketCount    = count(importer.begin(), importer.end(), '[');
                __int64 altBracketCount = count(importer.begin(), importer.end(), ']');

                if (IDExist[importer].length() == 0)
                {
                    if (bracketCount != altBracketCount)
                        ErrorMessage(2013, format, filename, linecount, importer);

                    size_t pos  = importer.find("[") + 1;
                    string file = importer.substr(pos, importer.find("]", pos) - pos);
                    string keyword;
                    string tempID;

                    if (bracketCount > 1)
                    {
                        pos                = importer.find("[", pos);
                        string tempKeyword = importer.substr(pos, importer.find_last_of("]") + 1 - pos);
                        int openBrack      = 0;

                        for (auto& curChar : tempKeyword)
                        {
                            if (curChar == '[') { ++openBrack; }
                            else if (curChar == ']')
                            {
                                --openBrack;

                                if (openBrack == 0) keyword.append("!~^!");
                            }
                            else
                            {
                                keyword = keyword + curChar;
                            }
                        }

                        pos = keyword.rfind("!~^!");

                        if (openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4)
                            ErrorMessage(2013, format, filename, linecount, importer);
                        else
                            keyword = keyword.substr(0, keyword.length() - 4);
                    }
                    else
                    {
                        keyword = "";
                    }

                    if ((*newImport)[file][keyword].length() > 0) { tempID = (*newImport)[file][keyword]; }
                    else
                    {
                        tempID                      = strID;
                        IDExist[importer]           = tempID;
                        (*newImport)[file][keyword] = tempID;
                        newID();
                    }

                    change.replace(nextpos, importer.length(), tempID);
                    isChange = true;
                }
                else
                {
                    change.replace(nextpos, importer.length(), IDExist[importer]);
                    isChange = true;
                }
            }

            if (isChange) { line.replace(line.find(oldChange), oldChange.length(), change); }

            break;
        }
    }
}

void ExistingFunction::processing(string& line,
                                  string filename,
                                  int curFunctionID,
                                  int linecount,
                                  ID eventid,
                                  ID variableid,
                                  int groupMulti,
                                  int optionMulti,
                                  int animMulti,
                                  string multiOption)
{
    __int64 counter = count(line.begin(), line.end(), '$') / 2;
    size_t curPos   = 0;

    for (int i = 0; i < counter; ++i)
    {
        bool isChange    = false;
        curPos           = line.find("$", curPos + 1);
        string change    = line.substr(curPos, line.find("$", curPos + 1) - curPos + 1);
        string oldChange = change;
        change           = change.substr(1, change.length() - 2);
        curPos           = line.find("$", curPos + 1);

        while (true)
        {
            // order equation
            if (change.find("(") != NOT_FOUND && change.find("L", change.find("(")) != NOT_FOUND
                && change.find(")", change.find("(")) != NOT_FOUND)
            {
                __int64 maths = count(change.begin(), change.end(), '(');

                if (maths != 0 && maths == count(change.begin(), change.end(), ')'))
                {
                    size_t nextpos = change.find("(");

                    for (__int64 j = 0; j < maths; ++j)
                    {
                        string equation = change.substr(nextpos, change.find(")", nextpos) - 1);
                        string number   = "";
                        string ID       = "";

                        if (equation.find("(S", 0) != NOT_FOUND)
                        {
                            ID = nemesis::regex_replace(
                                string(equation), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                            if (change.find("(S" + ID + "+") == NOT_FOUND) ID = "";

                            number = nemesis::regex_replace(string(equation.substr(3 + ID.length())),
                                                          nemesis::regex("[^0-9]*([0-9]+).*"),
                                                          string("\\1"));
                        }

                        if (equation != "(S" + ID + "+" + number + ")")
                        {
                            size_t equationLength = equation.length();

                            if (equation.find("L") != NOT_FOUND)
                            {
                                __int64 maths2 = count(change.begin(), change.end(), 'L');

                                for (__int64 k = 0; k < maths2; ++k)
                                {
                                    equation.replace(equation.find("L"),
                                                     1,
                                                     to_string(groupAnimInfo[groupMulti].size() - 1));
                                }
                            }

                            nemesis::calculate(equation, format, filename, linecount);

                            if (stoi(equation) > int(groupAnimInfo[groupMulti].size() - 1)
                                || stoi(equation) < 0)
                                ErrorMessage(1148, format, filename, linecount, change);

                            change.replace(nextpos, equationLength, equation);
                            isChange = true;
                        }

                        nextpos = change.find("(", nextpos + 1);
                    }
                }
            }

            if (groupMulti > -1)
            {
                if (change.find("[FilePath]") != NOT_FOUND)
                {
                    if (change.find(format + "[][FilePath]", 0) != NOT_FOUND)
                    {
                        change.replace(
                            change.find(format + "[][FilePath]"),
                            13 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FilePath"]);
                        isChange = true;
                    }

                    if (change.find(format + "[F][FilePath]", 0) != NOT_FOUND)
                    {
                        change.replace(
                            change.find(format + "[F][FilePath]"),
                            13 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FilePath"]);
                        isChange = true;
                    }

                    if (change.find(format + "[N][FilePath]", 0) != NOT_FOUND)
                        ErrorMessage(1056, format, filename, linecount, line);
                    if (change.find(format + "[B][FilePath]", 0) != NOT_FOUND)
                        ErrorMessage(1056, format, filename, linecount, line);

                    if (change.find(format + "[L][FilePath]", 0) != NOT_FOUND)
                    {
                        change.replace(
                            change.find(format + "[L][FilePath]"),
                            13 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FilePath"]);
                        isChange = true;
                    }

                    if (change.find(format + "[", 0) != NOT_FOUND)
                    {
                        string number
                            = nemesis::regex_replace(string(change.substr(change.find(format + "[", 0))),
                                                   nemesis::regex("[^0-9]*([0-9]+).*"),
                                                   string("\\1"));

                        if (change.find(format + "[" + number + "][FilePath]", 0) != NOT_FOUND)
                        {
                            if (uint(stoi(number)) >= groupAnimInfo[groupMulti].size())
                            {
                                change.replace(change.find(format + "[" + number + "][FilePath]"),
                                               8 + format.length() + number.length(),
                                               subFunctionIDs->grouplist[groupMulti]
                                                   ->singlelist[stoi(number)]
                                                   ->format["FilePath"]);
                                isChange = true;
                            }
                            else
                            {
                                ErrorMessage(1148, format, filename, linecount, change);
                            }
                        }
                    }
                }

                if (change.find("[FileName]") != NOT_FOUND)
                {
                    if (change.find(format + "[][FileName]", 0) != NOT_FOUND)
                    {
                        if (multiOption != format || animMulti == -1)
                        { ErrorMessage(1052, format, filename, linecount, line); }

                        change.replace(
                            change.find(format + "[][FileName]"),
                            12 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FileName"]);
                    }

                    if (change.find(format + "[F][FileName]", 0) != NOT_FOUND)
                    {
                        change.replace(
                            change.find(format + "[F][FileName]"),
                            13 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FileName"]);
                        isChange = true;
                    }

                    if (change.find(format + "[N][FileName]", 0) != NOT_FOUND)
                        ErrorMessage(1056, format, filename, linecount, line);
                    if (change.find(format + "[B][FileName]", 0) != NOT_FOUND)
                        ErrorMessage(1056, format, filename, linecount, line);

                    if (change.find(format + "[L][FileName]", 0) != NOT_FOUND)
                    {
                        change.replace(
                            change.find(format + "[L][FileName]"),
                            13 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FileName"]);
                        isChange = true;
                    }

                    if (change.find(format + "[", 0) != NOT_FOUND)
                    {
                        string number
                            = nemesis::regex_replace(string(change.substr(change.find(format + "[", 0))),
                                                   nemesis::regex("[^0-9]*([0-9]+).*"),
                                                   string("\\1"));

                        if (change.find(format + "[" + number + "][FileName]", 0) != NOT_FOUND)
                        {
                            if (uint(stoi(number)) >= groupAnimInfo[groupMulti].size())
                            {
                                change.replace(change.find(format + "[" + number + "][FileName]"),
                                               8 + format.length() + number.length(),
                                               subFunctionIDs->grouplist[groupMulti]
                                                   ->singlelist[stoi(number)]
                                                   ->format["FileName"]);
                                isChange = true;
                            }
                            else
                            {
                                ErrorMessage(1148, format, filename, linecount, change);
                            }
                        }
                    }

                    if (error) throw nemesis::exception();
                }

                if (change.find("main_anim_event") != NOT_FOUND)
                {
                    size_t pos = change.find("[" + format + "[][main_anim_event]]", 0);

                    if (pos != NOT_FOUND
                        && (change.find("eventID[" + format + "[][main_anim_event]]") == NOT_FOUND
                            || change.find("eventID[" + format + "[][main_anim_event]]") != pos - 7))
                    {
                        if (multiOption != format || animMulti == -1)
                        { ErrorMessage(1052, format, filename, linecount, line); }

                        change.replace(pos,
                                       21 + format.length(),
                                       subFunctionIDs->grouplist[groupMulti]
                                           ->singlelist[animMulti]
                                           ->format["main_anim_event"]);
                        isChange = true;
                    }
                    else
                    {
                        pos = change.find(format + "[][main_anim_event]", 0);

                        if (pos != NOT_FOUND)
                        {
                            if (multiOption != format || animMulti == -1)
                            { ErrorMessage(1052, format, filename, linecount, line); }

                            change.replace(pos,
                                           19 + format.length(),
                                           subFunctionIDs->grouplist[groupMulti]
                                               ->singlelist[animMulti]
                                               ->format["main_anim_event"]);
                            isChange = true;
                        }
                    }

                    pos = change.find("[" + format + "[F][main_anim_event]]", 0);

                    if (pos != NOT_FOUND
                        && (change.find("eventID[" + format + "[F][main_anim_event]]") == NOT_FOUND
                            || change.find("eventID[" + format + "[F][main_anim_event]]") != pos - 7))
                    {
                        change.replace(
                            pos,
                            22 + format.length(),
                            subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["main_anim_event"]);
                        isChange = true;
                    }
                    else
                    {
                        pos = change.find(format + "[F][main_anim_event]", 0);

                        if (pos != NOT_FOUND)
                        {
                            change.replace(pos,
                                           20 + format.length(),
                                           subFunctionIDs->grouplist[groupMulti]
                                               ->singlelist[0]
                                               ->format["main_anim_event"]);
                            isChange = true;
                        }
                    }

                    if (change.find(format + "[N][main_anim_event]", 0) != NOT_FOUND)
                        ErrorMessage(1056, format, filename, linecount, line);
                    if (change.find(format + "[B][main_anim_event]", 0) != NOT_FOUND)
                        ErrorMessage(1056, format, filename, linecount, line);

                    pos = change.find("[" + format + "[L][main_anim_event]]", 0);

                    if (pos != NOT_FOUND
                        && (change.find("eventID[" + format + "[L][main_anim_event]]") == NOT_FOUND
                            || change.find("eventID[" + format + "[L][main_anim_event]]") != pos - 7))
                    {
                        change.replace(pos,
                                       22 + format.length(),
                                       subFunctionIDs->grouplist[groupMulti]
                                           ->singlelist.back()
                                           ->format["main_anim_event"]);
                        isChange = true;
                    }
                    else
                    {
                        pos = change.find(format + "[L][main_anim_event]", 0);

                        if (pos != NOT_FOUND)
                        {
                            change.replace(pos,
                                           20 + format.length(),
                                           subFunctionIDs->grouplist[groupMulti]
                                               ->singlelist.back()
                                               ->format["main_anim_event"]);
                            isChange = true;
                        }
                    }

                    if (change.find(format + "[", 0) != NOT_FOUND)
                    {
                        string number = nemesis::regex_replace(
                            string(change),
                            nemesis::regex(format + "\\[([0-9]+)\\]\\[main_anim_event\\].*"),
                            string("\\1"));

                        if (number != change)
                        {
                            if (uint(stoi(number)) >= groupAnimInfo[groupMulti].size())
                            {
                                pos = change.find("[" + format + "[" + number + "][main_anim_event]]", 0);

                                if (pos != NOT_FOUND
                                    && (change.find("eventID[" + format + "[" + number
                                                    + "][main_anim_event]]")
                                            == NOT_FOUND
                                        || change.find("eventID[" + format + "[" + number
                                                       + "][main_anim_event]]")
                                               != pos - 7))
                                {
                                    change.replace(pos,
                                                   21 + format.length(),
                                                   subFunctionIDs->grouplist[groupMulti]
                                                       ->singlelist[stoi(number)]
                                                       ->format["main_anim_event"]);
                                    isChange = true;
                                }
                                else
                                {
                                    pos = change.find(format + "[" + number + "][main_anim_event]", 0);

                                    if (pos != NOT_FOUND)
                                    {
                                        change.replace(pos,
                                                       19 + format.length() + number.length(),
                                                       subFunctionIDs->grouplist[groupMulti]
                                                           ->singlelist[stoi(number)]
                                                           ->format["main_anim_event"]);
                                        isChange = true;
                                    }
                                }
                            }
                            else
                            {
                                ErrorMessage(1148, format, filename, linecount, change);
                            }
                        }
                    }

                    if (!m_hasGroup)
                    {
                        pos = change.find("{main_anim_event}", 0);

                        if (pos != NOT_FOUND)
                        {
                            change.replace(pos,
                                           17,
                                           subFunctionIDs->grouplist[groupMulti]
                                               ->singlelist[animMulti]
                                               ->format["main_anim_event"]);
                            isChange = true;
                        }
                        else
                        {
                            pos = change.find("main_anim_event", 0);

                            if (pos != NOT_FOUND)
                            {
                                change.replace(pos,
                                               15,
                                               subFunctionIDs->grouplist[groupMulti]
                                                   ->singlelist[animMulti]
                                                   ->format["main_anim_event"]);
                                isChange = true;
                            }
                        }
                    }

                    if (error) throw nemesis::exception();
                }

                size_t pos      = change.find(format + "[", 0);
                bool innerError = false;

                if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
                {
                    formatGroupReplace(change,
                                       change,
                                       0,
                                       filename,
                                       format,
                                       subFunctionIDs,
                                       groupAnimInfo,
                                       linecount,
                                       groupMulti,
                                       optionMulti,
                                       animMulti,
                                       multiOption,
                                       innerError);
                    isChange = true;

                    if (innerError) ErrorMessage(2014, format, filename, linecount);

                    if (error) throw nemesis::exception();
                }

                if (change.find(format + "_group[][", 0) != NOT_FOUND
                    && change.find("]", change.find(format + "_group[][")) != NOT_FOUND)
                {
                    formatGroupReplace(change,
                                       change,
                                       0,
                                       filename,
                                       format,
                                       subFunctionIDs,
                                       groupAnimInfo,
                                       linecount,
                                       groupMulti,
                                       optionMulti,
                                       animMulti,
                                       multiOption,
                                       innerError);
                    isChange = true;

                    if (innerError) ErrorMessage(2014, format, filename, linecount);

                    if (error) throw nemesis::exception();
                }

                if (multiOption == format)
                {
                    string tempchange = change;
                    pos               = change.find("[");

                    if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
                    {
                        formatGroupReplace(change,
                                           change,
                                           0,
                                           filename,
                                           format,
                                           subFunctionIDs,
                                           groupAnimInfo,
                                           linecount,
                                           groupMulti,
                                           optionMulti,
                                           animMulti,
                                           multiOption,
                                           innerError);

                        if (innerError) ErrorMessage(2014, format, filename, linecount);

                        if (error) throw nemesis::exception();
                    }
                    else if (subFunctionIDs->grouplist[groupMulti]
                                 ->singlelist[animMulti]
                                 ->format[change]
                                 .length()
                             > 0)
                    {
                        change = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change];
                    }

                    if (change != tempchange) isChange = true;
                }

                if (error) throw nemesis::exception();
            }
            else
            {
                for (auto it = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format.begin();
                     it != subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format.end();
                     ++it)
                {
                    if (change.find(it->first) != NOT_FOUND)
                    {
                        int counter = sameWordCount(change, it->first);

                        for (int j = 0; j < counter; ++j)
                        {
                            change.replace(change.find(it->first), it->first.length(), it->second);
                            isChange = true;
                        }

                        if (change.length() == it->second.length()) { break; }
                    }
                }
            }

            size_t position = change.find("eventID[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                eventIDReplacer(change, format, filename, eventid, zeroEvent, linecount);
                isChange = true;
            }

            position = change.find("variableID[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                variableIDReplacer(change, format, filename, variableid, zeroVariable, linecount);
                isChange = true;
            }

            position = change.find("crc32[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                CRC32Replacer(change, format, filename, linecount);
                isChange = true;
            }

            if (error) throw nemesis::exception();

            position = change.find("import[");

            if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
            {
                size_t nextpos          = change.find("import[");
                string importer         = change.substr(nextpos, change.find_last_of("]") - nextpos + 1);
                __int64 bracketCount    = count(importer.begin(), importer.end(), '[');
                __int64 altBracketCount = count(importer.begin(), importer.end(), ']');

                if (IDExist[importer].length() == 0)
                {
                    if (bracketCount != altBracketCount)
                        ErrorMessage(2013, format, filename, linecount, importer);

                    size_t pos  = importer.find("[") + 1;
                    string file = importer.substr(pos, importer.find("]", pos) - pos);
                    string keyword;
                    string tempID;

                    if (bracketCount > 1)
                    {
                        pos                = importer.find("[", pos);
                        string tempKeyword = importer.substr(pos, importer.find_last_of("]") + 1 - pos);
                        int openBrack      = 0;

                        for (auto& curChar : tempKeyword)
                        {
                            if (curChar == '[') { ++openBrack; }
                            else if (curChar == ']')
                            {
                                --openBrack;

                                if (openBrack == 0) keyword.append("!~^!");
                            }
                            else
                            {
                                keyword = keyword + curChar;
                            }
                        }

                        pos = keyword.rfind("!~^!");

                        if (openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4)
                            ErrorMessage(2013, format, filename, linecount, importer);
                        else
                            keyword = keyword.substr(0, keyword.length() - 4);
                    }
                    else
                    {
                        keyword = "";
                    }

                    if ((*newImport)[file][keyword].length() > 0) { tempID = (*newImport)[file][keyword]; }
                    else
                    {
                        tempID                      = strID;
                        IDExist[importer]           = tempID;
                        (*newImport)[file][keyword] = tempID;
                        newID();
                    }

                    change.replace(nextpos, importer.length(), tempID);
                    isChange = true;
                }
                else
                {
                    change.replace(nextpos, importer.length(), IDExist[importer]);
                    isChange = true;
                }
            }

            if (change.find("MD") != NOT_FOUND) ErrorMessage(1096, format, filename, linecount);
            if (change.find("RD") != NOT_FOUND) ErrorMessage(1097, format, filename, linecount);
            if (isChange) line.replace(line.find(oldChange), oldChange.length(), change);

            break;
        }
    }
}

void multiChoice(string& line,
                 string filename,
                 vector<vector<unordered_map<string, bool>>> masterOptionPicked,
                 vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                 int numline,
                 string format,
                 string masterformat,
                 AnimationUtility utility)
{
    if (line.find("<!-- ", 0) != NOT_FOUND)
    {
        size_t nextposition = 0;
        int choicecount     = 0;

        while (true)
        {
            if (line.find("<!-- ", nextposition) != NOT_FOUND)
            {
                choicecount++;
                nextposition = line.find("<!-- ", nextposition) + 1;
            }
            else
            {
                break;
            }
        }

        if (line.find("*", 0) != NOT_FOUND || line.find("%", 0) != NOT_FOUND)
            ErrorMessage(2015, format, filename, numline);

        bool none    = true;
        nextposition = 0;

        for (int i = 0; i < choicecount; ++i)
        {
            VecStr opt;
            vector<char> storechar;
            nextposition   = line.find("<!-- ", nextposition) + 5;
            string tempstr = line.substr(nextposition, line.find(" ", nextposition) - nextposition);

            if (line.find("<!-- " + tempstr + " -->", 0) == NOT_FOUND)
            {
                if (newCondition(tempstr,
                                 filename,
                                 masterOptionPicked,
                                 groupAnimInfo,
                                 numline,
                                 format,
                                 masterformat,
                                 utility))
                {
                    nextposition  = line.find(" ", nextposition) + 1;
                    string output = line.substr(nextposition, line.find(" ", nextposition) - nextposition);
                    line.replace(line.find("$MC$", 0), 4, output);
                    none = false;
                    break;
                }
            }
            else if (i == choicecount - 1)
            {
                line.replace(line.find("$MC$", 0), 4, tempstr);
                none = false;
                break;
            }
            else
            {
                ErrorMessage(2016, format, filename, numline);
            }
        }

        if (none) line.replace(line.find("$MC$", 0), 4, "null");
    }
    else
    {
        line.replace(line.find("$MC$"), 4, "null");
    }

    line = line.substr(0, line.find("</hkparam>") + 10);
}

bool specialCondition(string condition,
                      string filename,
                      vector<vector<unordered_map<string, bool>>> curOptionPicked,
                      vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                      int numline,
                      string format,
                      string masterformat,
                      AnimationUtility utility)
{
    bool isNot;
    size_t pos;

    if (condition.find("!=") != NOT_FOUND)
    {
        if (condition.find("==") != NOT_FOUND || sameWordCount(condition, "!=") > 1)
            ErrorMessage(1124, format, filename, numline, utility.originalCondition);

        isNot = true;
        pos   = condition.find("!=");
    }
    else if (condition.find("==") != NOT_FOUND)
    {
        if (condition.find("!=") != NOT_FOUND || sameWordCount(condition, "==") > 1)
            ErrorMessage(1124, format, filename, numline, utility.originalCondition);

        isNot = false;
        pos   = condition.find("==");
    }
    else
    {
        ErrorMessage(1125, format, filename, numline, utility.originalCondition);
    }

    if (error) throw nemesis::exception();

    bool oneTime;
    string oriCondition1 = condition.substr(1, pos - 1);
    string oriCondition2 = condition.substr(pos + 2);
    VecStr optionInfo1;
    VecStr optionInfo2;

    // <optionA != optionB>*
    // only need to fulfill the condition once
    if (condition.back() == '*')
    {
        oneTime = true;
        oriCondition2.pop_back();
        oriCondition2.pop_back();
    }
    else
    {
        oneTime = false;
        oriCondition2.pop_back();
    }

    if (oriCondition1.length() > 0)
        optionInfo1 = GetOptionInfo(oriCondition1,
                                    masterformat,
                                    filename,
                                    numline,
                                    groupAnimInfo,
                                    true,
                                    false,
                                    utility.groupMulti,
                                    utility.animMulti,
                                    utility.optionMulti,
                                    utility.multiOption);

    if (oriCondition2.length() > 0)
        optionInfo2 = GetOptionInfo(oriCondition2,
                                    masterformat,
                                    filename,
                                    numline,
                                    groupAnimInfo,
                                    true,
                                    false,
                                    utility.groupMulti,
                                    utility.animMulti,
                                    utility.optionMulti,
                                    utility.multiOption);

    if (error) throw nemesis::exception();

    int groupMulti1;
    int groupMulti2;
    int endgroupMulti1;
    int endgroupMulti2;

    if (optionInfo1.size() > 0)
    {
        if (optionInfo1[1].length() == 0)
        {
            groupMulti1    = 0;
            endgroupMulti1 = groupAnimInfo.size();
        }
        else
        {
            groupMulti1    = stoi(optionInfo1[1]);
            endgroupMulti1 = groupMulti1 + 1;
        }
    }
    else
    {
        groupMulti1    = 0;
        endgroupMulti1 = 1;
    }

    if (optionInfo2.size() > 0)
    {
        if (optionInfo2[1].length() == 0)
        {
            groupMulti2    = 0;
            endgroupMulti2 = groupAnimInfo.size();
        }
        else
        {
            groupMulti2    = stoi(optionInfo2[1]);
            endgroupMulti2 = groupMulti2 + 1;
        }
    }
    else
    {
        groupMulti2    = 0;
        endgroupMulti2 = 1;
    }

    while (groupMulti1 < endgroupMulti1)
    {
        while (groupMulti2 < endgroupMulti2)
        {
            int animMulti1;
            int animMulti2;
            int endAnimMulti1;
            int endAnimMulti2;

            if (optionInfo1.size() > 0)
            {
                if (optionInfo1[2].length() == 0)
                {
                    animMulti1    = 0;
                    endAnimMulti1 = groupAnimInfo.size();
                }
                else
                {
                    animMulti1    = stoi(optionInfo1[2]);
                    endAnimMulti1 = animMulti1 + 1;
                }
            }
            else
            {
                animMulti1    = 0;
                endAnimMulti1 = 1;
            }

            if (optionInfo2.size() > 0)
            {
                if (optionInfo2[2].length() == 0)
                {
                    animMulti2    = 0;
                    endAnimMulti2 = groupAnimInfo.size();
                }
                else
                {
                    animMulti2    = stoi(optionInfo2[2]);
                    endAnimMulti2 = animMulti2 + 1;
                }
            }
            else
            {
                animMulti2    = 0;
                endAnimMulti2 = 1;
            }

            unordered_map<int, unordered_map<int, unordered_map<int, string>>> history1;
            unordered_map<int, unordered_map<int, unordered_map<int, string>>> history2;

            while (animMulti1 < endAnimMulti1)
            {
                while (animMulti2 < endAnimMulti2)
                {
                    int optionMulti1;
                    int optionMulti2;
                    int endMulti1;
                    int endMulti2;

                    if (optionInfo1.size() > 0)
                    {
                        if (optionInfo1[3].find("@AnimObject/") != NOT_FOUND)
                        {
                            optionMulti1 = 0;
                            endMulti1    = groupAnimInfo[groupMulti1][animMulti1]
                                            ->optionPickedCount[optionInfo1[3].substr(1)];
                        }
                        else if (optionInfo1.size() > 4)
                        {
                            unordered_map<string, VecStr>* addOnPtr
                                = &groupAnimInfo[groupMulti1][animMulti1]->addOn;

                            if (addOnPtr->find(optionInfo1[3]) != addOnPtr->end()
                                && (*addOnPtr)[optionInfo1[3]].size() > 0)
                            {
                                unordered_map<string, VecStr>* groupAdditionPtr
                                    = &groupAnimInfo[groupMulti1][animMulti1]->groupAddition[optionInfo1[3]];

                                if (groupAdditionPtr->find(optionInfo1[4]) != groupAdditionPtr->end()
                                    && (*groupAdditionPtr)[optionInfo1[4]].size() > 0)
                                {
                                    if (optionInfo1.size() == 5 || optionInfo1[5].length() == 0)
                                    {
                                        optionMulti1 = 0;
                                        endMulti1    = (*groupAdditionPtr)[optionInfo1[4]].size();
                                    }
                                    else
                                    {
                                        optionMulti1 = stoi(optionInfo1[5]);
                                        endMulti1    = optionMulti1 + 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            optionMulti1 = 0;
                            endMulti1    = 1;
                        }
                    }
                    else
                    {
                        optionMulti1 = 0;
                        endMulti1    = 1;
                    }

                    if (optionInfo2.size() > 0)
                    {
                        if (optionInfo2[3].find("@AnimObject/") != NOT_FOUND)
                        {
                            optionMulti2 = 0;
                            endMulti2    = groupAnimInfo[groupMulti2][animMulti2]
                                            ->optionPickedCount[optionInfo2[3].substr(1)];
                        }
                        else if (optionInfo2.size() > 4)
                        {
                            unordered_map<string, VecStr>* addOnPtr
                                = &groupAnimInfo[groupMulti2][animMulti2]->addOn;

                            if (addOnPtr->find(optionInfo2[3]) != addOnPtr->end()
                                && (*addOnPtr)[optionInfo2[3]].size() > 0)
                            {
                                unordered_map<string, VecStr>* groupAdditionPtr
                                    = &groupAnimInfo[groupMulti2][animMulti2]->groupAddition[optionInfo2[3]];

                                if (groupAdditionPtr->find(optionInfo2[4]) != groupAdditionPtr->end()
                                    && (*groupAdditionPtr)[optionInfo2[4]].size() > 0)
                                {
                                    if (optionInfo2.size() == 5 || optionInfo2[5].length() == 0)
                                    {
                                        optionMulti2 = 0;
                                        endMulti2    = (*groupAdditionPtr)[optionInfo2[4]].size();
                                    }
                                    else
                                    {
                                        optionMulti2 = stoi(optionInfo2[5]);
                                        endMulti2    = optionMulti2 + 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            optionMulti2 = 0;
                            endMulti2    = 1;
                        }

                        if (error) throw nemesis::exception();
                    }
                    else
                    {
                        optionMulti2 = 0;
                        endMulti2    = 1;
                    }

                    if (optionInfo1.size() > 0 && optionInfo2.size() > 0
                        && (optionMulti1 != optionMulti2 || endMulti1 != endMulti2))
                    {
                        if (!isNot)
                            return false;
                        else if (oneTime)
                            return true;
                    }
                    else
                    {
                        unordered_set<string> condlist1;
                        unordered_set<string> condlist2;
                        endMulti1 = max(endMulti1, endMulti2);

                        while (optionMulti1 < endMulti1)
                        {
                            string condition1;
                            string condition2;

                            if (oriCondition1.length() != 0)
                            {
                                condition1 = "$" + masterformat + "_group[][][" + optionInfo1[3] + "]$";

                                if (history1[groupMulti1][animMulti1][optionMulti1].length() == 0)
                                {
                                    if (utility.isExisting)
                                    {
                                        if (utility.currentProcess)
                                        {
                                            utility.currentProcess->processing(
                                                condition1,
                                                filename,
                                                stoi(filename.substr(1)),
                                                numline,
                                                utility.currentProcess->eventid,
                                                utility.currentProcess->variableid,
                                                groupMulti1,
                                                optionMulti1,
                                                animMulti1,
                                                masterformat);
                                        }
                                        else
                                        {
                                            utility.groupMulti             = groupMulti1;
                                            utility.animMulti              = animMulti1;
                                            utility.optionMulti            = optionMulti1;
                                            utility.nodeProcess->linecount = numline;
                                            processing(condition1, utility.nodeProcess);
                                        }
                                    }
                                    else if (utility.currentProcess)
                                    {
                                        utility.currentProcess->processing(condition1,
                                                                           filename,
                                                                           masterformat,
                                                                           numline,
                                                                           utility.currentProcess->eventid,
                                                                           utility.currentProcess->variableid,
                                                                           groupMulti1,
                                                                           optionMulti1,
                                                                           animMulti1,
                                                                           masterformat);
                                    }
                                    else
                                    {
                                        utility.groupMulti             = groupMulti1;
                                        utility.animMulti              = animMulti1;
                                        utility.optionMulti            = optionMulti1;
                                        utility.nodeProcess->linecount = numline;
                                        processing(condition1, utility.nodeProcess);
                                    }

                                    history1[groupMulti1][animMulti1][optionMulti1] = condition1;
                                }
                                else
                                {
                                    condition1 = history1[groupMulti1][animMulti1][optionMulti1];
                                }

                                if (error) throw nemesis::exception();

                                if (condition.length() == 0) ErrorMessage(1172, format, filename, numline);
                            }

                            if (oriCondition2.length() != 0)
                            {
                                condition2 = "$" + masterformat + "_group[][][" + optionInfo2[3] + "]$";

                                if (history2[groupMulti2][animMulti2][optionMulti1].length() == 0)
                                {
                                    if (utility.isExisting)
                                    {
                                        if (utility.currentProcess)
                                        {
                                            utility.currentProcess->processing(
                                                condition2,
                                                filename,
                                                stoi(filename.substr(1)),
                                                numline,
                                                utility.currentProcess->eventid,
                                                utility.currentProcess->variableid,
                                                groupMulti2,
                                                optionMulti1,
                                                animMulti2,
                                                utility.multiOption);
                                        }
                                        else
                                        {
                                            utility.groupMulti             = groupMulti2;
                                            utility.animMulti              = animMulti2;
                                            utility.optionMulti            = optionMulti1;
                                            utility.nodeProcess->linecount = numline;
                                            processing(condition2, utility.nodeProcess);
                                        }
                                    }
                                    else if (utility.currentProcess)
                                    {
                                        utility.currentProcess->processing(condition2,
                                                                           filename,
                                                                           masterformat,
                                                                           numline,
                                                                           utility.currentProcess->eventid,
                                                                           utility.currentProcess->variableid,
                                                                           groupMulti2,
                                                                           optionMulti1,
                                                                           animMulti2,
                                                                           utility.multiOption);
                                    }
                                    else
                                    {
                                        utility.groupMulti             = groupMulti2;
                                        utility.animMulti              = animMulti2;
                                        utility.optionMulti            = optionMulti1;
                                        utility.nodeProcess->linecount = numline;
                                        processing(condition2, utility.nodeProcess);
                                    }

                                    history2[groupMulti2][animMulti2][optionMulti1] = condition2;
                                }
                                else
                                {
                                    condition2 = history2[groupMulti2][animMulti2][optionMulti1];
                                }

                                if (condition.length() == 0) ErrorMessage(1172, format, filename, numline);
                            }

                            condlist1.insert(condition1);
                            condlist2.insert(condition2);

                            if (error) throw nemesis::exception();

                            ++optionMulti1;
                        }

                        if (condlist1 != condlist2)
                        {
                            if (!isNot)
                                return false;
                            else if (oneTime)
                                return true;
                        }
                        else
                        {
                            if (isNot)
                                return false;
                            else if (oneTime)
                                return true;
                        }
                    }

                    ++animMulti2;

                    if (error) throw nemesis::exception();
                }

                animMulti2
                    = optionInfo2.size() == 0 || optionInfo2[2].length() == 0 ? 0 : stoi(optionInfo2[2]);
                ++animMulti1;
            }

            ++groupMulti2;
        }

        groupMulti2 = optionInfo2.size() == 0 || optionInfo2[1].length() == 0 ? 0 : stoi(optionInfo2[1]);
        ++groupMulti1;
    }

    return true;
}

bool singleCondition(string condition,
                     string filename,
                     vector<vector<unordered_map<string, bool>>> curOptionPicked,
                     vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                     int numline,
                     string format,
                     string masterformat,
                     AnimationUtility utility)
{
    if (condition.find("<") == 0
        && (condition.find(">") == condition.length() - 1 || condition.find(">*") == condition.length() - 2)
        && (condition.find("!=") != NOT_FOUND || condition.find("==") != NOT_FOUND))
    {
        return specialCondition(
            condition, filename, curOptionPicked, groupAnimInfo, numline, format, masterformat, utility);
    }
    else
    {
        bool isNot            = false;
        string conditionOrder = condition;

        if (conditionOrder[0] == '!')
        {
            isNot          = true;
            conditionOrder = conditionOrder.substr(1);
        }

        return conditionProcess(conditionOrder,
                                masterformat,
                                format,
                                filename,
                                numline,
                                curOptionPicked,
                                groupAnimInfo,
                                isNot,
                                utility);
    }
}

bool andLoop(string condition,
             string filename,
             vector<vector<unordered_map<string, bool>>> curOptionPicked,
             vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
             int numline,
             string format,
             string masterformat,
             AnimationUtility utility)
{
    size_t lastpos = 0;
    string nextCondition;

    while (condition.find("&", lastpos) != NOT_FOUND)
    {
        string nextCondition = condition.substr(lastpos, condition.find("&", lastpos) - lastpos);

        // bool1 & (bool2 | bool3) & bool4 | bool5...
        // --------^
        if (nextCondition[0] == '(')
        {
            size_t c_or  = 0;
            size_t backB = 0;

            for (uint i = 0; i < nextCondition.size(); ++i)
            {
                if (nextCondition[i] == '(') { ++c_or; }
                else if (nextCondition[i] == ')')
                {
                    --c_or;

                    if (c_or == 0)
                    {
                        backB = i;
                        break;
                    }
                }
            }

            if (c_or != 0) ErrorMessage(1106, format, filename, numline, utility.originalCondition);
            if (!newCondition(nextCondition.substr(1, backB - 1),
                              filename,
                              curOptionPicked,
                              groupAnimInfo,
                              numline,
                              format,
                              masterformat,
                              utility))
                return false;

            lastpos = condition.find("&", backB + 1) + 1;
        }
        else
        {
            if (!singleCondition(nextCondition,
                                 filename,
                                 curOptionPicked,
                                 groupAnimInfo,
                                 numline,
                                 format,
                                 masterformat,
                                 utility))
                return false;

            lastpos = condition.find("&", lastpos) + 1;
        }
    }

    if (error) throw nemesis::exception();

    nextCondition = condition.substr(lastpos);

    if (nextCondition[0] == '(')
    {
        size_t c_or  = 0;
        size_t backB = 0;

        for (uint i = 0; i < nextCondition.size(); ++i)
        {
            if (nextCondition[i] == '(') { ++c_or; }
            else if (nextCondition[i] == ')')
            {
                --c_or;

                if (c_or == 0)
                {
                    backB = i;
                    break;
                }
            }
        }

        if (c_or != 0) ErrorMessage(1106, format, filename, numline, utility.originalCondition);

        return newCondition(nextCondition.substr(1, backB - 1),
                            filename,
                            curOptionPicked,
                            groupAnimInfo,
                            numline,
                            format,
                            masterformat,
                            utility);
    }
    else
    {
        return singleCondition(
            nextCondition, filename, curOptionPicked, groupAnimInfo, numline, format, masterformat, utility);
    }
}

bool andOrParenthesis(size_t c_and,
                      size_t c_or,
                      string condition,
                      string filename,
                      vector<vector<unordered_map<string, bool>>> curOptionPicked,
                      vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                      int numline,
                      string format,
                      string masterformat,
                      AnimationUtility utility)
{
    size_t parent = condition.find("(");

    // PARENT not exist / PARENT after OR
    // bool1 | (bool2 & bool3)...
    // ------^
    if (parent == NOT_FOUND || parent > c_or)
    {
        // bool1 & bool2 | (bool3...
        // ^-----------^
        string conditionGroup = condition.substr(0, c_or);

        // bool1 & bool2 | (bool3...
        //				   ^----->
        string nextCondition = condition.substr(c_or + 1);

        // bool1 & bool2 | bool3... (AND before OR)
        // --------------^------>
        if (c_and < c_or)
        {
            return andLoop(conditionGroup,
                           filename,
                           curOptionPicked,
                           groupAnimInfo,
                           numline,
                           format,
                           masterformat,
                           utility)
                   || newCondition(nextCondition,
                                   filename,
                                   curOptionPicked,
                                   groupAnimInfo,
                                   numline,
                                   format,
                                   masterformat,
                                   utility);
        }

        // bool1 | bool2 & bool3... (AND after OR)
        // ------^-------------->
        else
        {
            return singleCondition(conditionGroup,
                                   filename,
                                   curOptionPicked,
                                   groupAnimInfo,
                                   numline,
                                   format,
                                   masterformat,
                                   utility)
                   || newCondition(nextCondition,
                                   filename,
                                   curOptionPicked,
                                   groupAnimInfo,
                                   numline,
                                   format,
                                   masterformat,
                                   utility);
        }
    }

    // PARENT before OR
    // bool1 & (bool2 | bool3) & bool4 | bool5...
    // --------^
    else
    {
        return andParenthesis(
            condition, filename, curOptionPicked, groupAnimInfo, numline, format, masterformat, utility);
    }
}

bool andParenthesis(string condition,
                    string filename,
                    vector<vector<unordered_map<string, bool>>> curOptionPicked,
                    vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                    int numline,
                    string format,
                    string masterformat,
                    AnimationUtility utility)
{
    size_t c_or  = 0;
    size_t inner = 0;

    for (uint i = 0; i < condition.length(); ++i)
    {
        if (condition[i] == '(')
            ++c_or;
        else if (condition[i] == ')')
            --c_or;

        if (c_or == 0)
        {
            if (i + 1 == condition.length())
            {
                inner = i + 1;
                break;
            }
            else if (condition[i + 1] == '|')
            {
                inner = i;
                break;
            }
        }
    }

    if (c_or != 0) { ErrorMessage(1106, format, filename, numline, utility.originalCondition); }

    // ... & (bool1 | bool2) & bool3 | (bool4 & ...
    //		 ^---------------------^
    string conditionGroup = condition.substr(0, inner);

    // ... & (bool1 | bool2) & bool3
    // -----------------------------
    if (inner == condition.length())
    {
        return andLoop(
            conditionGroup, filename, curOptionPicked, groupAnimInfo, numline, format, masterformat, utility);
    }

    // ... & (bool1 | bool2) & bool3 | bool4...
    // ------------------------------^
    else
    {
        // ... & (bool1 | bool2) & bool3 | bool4...
        // ------------------------------------->
        return newCondition(conditionGroup,
                            filename,
                            curOptionPicked,
                            groupAnimInfo,
                            numline,
                            format,
                            masterformat,
                            utility)
               || newCondition(condition.substr(inner + 2),
                               filename,
                               curOptionPicked,
                               groupAnimInfo,
                               numline,
                               format,
                               masterformat,
                               utility);
    }
}

bool newCondition(string condition,
                  string filename,
                  vector<vector<unordered_map<string, bool>>> curOptionPicked,
                  vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                  int numline,
                  string format,
                  string masterformat,
                  AnimationUtility utility)
{
    if (condition[0] == '(')
    {
        if (condition.find(")") == NOT_FOUND
            || count(condition.begin(), condition.end(), '(')
                   != count(condition.begin(), condition.end(), ')'))
        { ErrorMessage(1105, format, filename, numline); }

        size_t c_and = 0;
        size_t c_or  = 0;
        size_t backB = 0;

        for (uint i = 0; i < condition.size(); ++i)
        {
            if (condition[i] == '(') { ++c_or; }
            else if (condition[i] == ')')
            {
                --c_or;

                if (c_or == 0)
                {
                    backB = i - 1;
                    break;
                }
            }
        }

        // (bool1 & bool2) | bool3 ...
        // --------------^
        string inHouse = condition.substr(1, backB);
        bool inHouseResult;

        // if first condition has parentesis
        // ((bool1 & bool2) | bool 3)
        // -^
        if (inHouse[0] == '(')
        {
            inHouseResult = newCondition(
                inHouse, filename, curOptionPicked, groupAnimInfo, numline, format, masterformat, utility);
        }
        else
        {
            c_and = inHouse.find("&");
            c_or  = inHouse.find("|");

            if (c_and == NOT_FOUND)
            {
                // AND & OR not exist
                if (c_or == NOT_FOUND)
                {
                    inHouseResult = singleCondition(inHouse,
                                                    filename,
                                                    curOptionPicked,
                                                    groupAnimInfo,
                                                    numline,
                                                    format,
                                                    masterformat,
                                                    utility);
                }

                // OR exist but not AND
                else
                {
                    inHouseResult = singleCondition(inHouse.substr(0, c_or),
                                                    filename,
                                                    curOptionPicked,
                                                    groupAnimInfo,
                                                    numline,
                                                    format,
                                                    masterformat,
                                                    utility)
                                    || newCondition(inHouse.substr(c_or + 1),
                                                    filename,
                                                    curOptionPicked,
                                                    groupAnimInfo,
                                                    numline,
                                                    format,
                                                    masterformat,
                                                    utility);
                }
            }

            // AND exist but not OR
            else if (c_or == NOT_FOUND)
            {
                inHouseResult = andLoop(inHouse,
                                        filename,
                                        curOptionPicked,
                                        groupAnimInfo,
                                        numline,
                                        format,
                                        masterformat,
                                        utility);
            }

            // AND & OR exist
            else
            {
                inHouseResult = andOrParenthesis(c_and,
                                                 c_or,
                                                 inHouse,
                                                 filename,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 numline,
                                                 format,
                                                 masterformat,
                                                 utility);
            }
        }

        if (error) throw nemesis::exception();
        if (backB + 2 >= condition.length()) return inHouseResult;

        string outHouse = condition.substr(backB + 2);

        if (outHouse.length() > 0 && outHouse[0] != '|' && outHouse[0] != '&')
            ErrorMessage(1106, format, filename, numline, utility.originalCondition);

        // ... | (bool1 & bool2)...
        // ----^
        if (outHouse[0] == '|')
        {
            return inHouseResult
                   || newCondition(outHouse.substr(1),
                                   filename,
                                   curOptionPicked,
                                   groupAnimInfo,
                                   numline,
                                   format,
                                   masterformat,
                                   utility);
        }

        c_and = outHouse.find("&");
        c_or  = outHouse.find("|");

        if (c_and == NOT_FOUND)
        {
            // AND & OR not exist
            if (c_or == NOT_FOUND) { return inHouseResult; }

            // OR exist but not AND
            else
            {
                // should have been covered
                ErrorMessage(1106, format, filename, numline, utility.originalCondition);
            }
        }

        // AND exist but not OR
        else if (c_or == NOT_FOUND)
        {
            return inHouseResult
                   && andLoop(outHouse.substr(c_and + 1),
                              filename,
                              curOptionPicked,
                              groupAnimInfo,
                              numline,
                              format,
                              masterformat,
                              utility);
        }

        // AND & OR exist
        else
        {
            c_and         = outHouse.find("&", 1);
            c_or          = outHouse.find("|", 1);
            size_t parent = outHouse.find("(", 1);

            // PARENT not exist / PARENT after OR
            // ... | bool2 & bool3 | (bool4...
            // ------^
            if (parent == NOT_FOUND || (c_or != NOT_FOUND && parent > c_or))
            {
                // ... & bool1 & bool2 | bool3... (AND before OR)
                //	     ^-----------^
                string conditionGroup = outHouse.substr(1, c_or - 1);

                // ... & bool1 & bool2 | bool3... (AND before OR)
                // ----^
                if (outHouse[0] == '&')
                {
                    if (inHouseResult
                        && andLoop(conditionGroup,
                                   filename,
                                   curOptionPicked,
                                   groupAnimInfo,
                                   numline,
                                   format,
                                   masterformat,
                                   utility))
                        return true;
                }

                // ... | bool1 & bool2 | bool3... (AND after OR)
                // ----^
                else if (outHouse[0] == '|')
                {
                    if (inHouseResult
                        || andLoop(conditionGroup,
                                   filename,
                                   curOptionPicked,
                                   groupAnimInfo,
                                   numline,
                                   format,
                                   masterformat,
                                   utility))
                        return true;
                }
                else
                {
                    ErrorMessage(1106, format, filename, numline, utility.originalCondition);
                }

                // ... | bool1 & bool2 | bool3...
                //						 ^---->
                return newCondition(outHouse.substr(c_or + 1),
                                    filename,
                                    curOptionPicked,
                                    groupAnimInfo,
                                    numline,
                                    format,
                                    masterformat,
                                    utility);
            }

            // PARENT exist
            else
            {
                // ... & (bool1 | bool2) | bool3...
                // ----^
                if (outHouse[0] == '&')
                {
                    // ... & (bool1 | bool2) | bool3...
                    // ----< check if false
                    if (!inHouseResult) return false;

                    // ... & (bool1 | bool2) & bool3...
                    //		 ^---------------------->
                    // ... & bool1 | (bool2 & bool3)...
                    //		 ^---------------------->
                    return andParenthesis(outHouse.substr(1),
                                          filename,
                                          curOptionPicked,
                                          groupAnimInfo,
                                          numline,
                                          format,
                                          masterformat,
                                          utility);
                }
                else
                {
                    ErrorMessage(1106, format, filename, numline, utility.originalCondition);
                }
            }
        }
    }
    else
    {
        size_t c_and = condition.find("&");
        size_t c_or  = condition.find("|");

        if (c_and == NOT_FOUND)
        {
            // AND & OR not exist
            if (c_or == NOT_FOUND)
            {
                return singleCondition(condition,
                                       filename,
                                       curOptionPicked,
                                       groupAnimInfo,
                                       numline,
                                       format,
                                       masterformat,
                                       utility);
            }

            // OR exist but not AND
            else
            {
                return singleCondition(condition.substr(0, c_or),
                                       filename,
                                       curOptionPicked,
                                       groupAnimInfo,
                                       numline,
                                       format,
                                       masterformat,
                                       utility)
                       || newCondition(condition.substr(c_or + 1),
                                       filename,
                                       curOptionPicked,
                                       groupAnimInfo,
                                       numline,
                                       format,
                                       masterformat,
                                       utility);
            }
        }

        // AND exist but not OR
        else if (c_or == NOT_FOUND)
            return andLoop(
                condition, filename, curOptionPicked, groupAnimInfo, numline, format, masterformat, utility);

        // AND & OR exist
        else
            return andOrParenthesis(c_and,
                                    c_or,
                                    condition,
                                    filename,
                                    curOptionPicked,
                                    groupAnimInfo,
                                    numline,
                                    format,
                                    masterformat,
                                    utility);
    }

    return false;
}

namespace backup
{
    bool newCondition(string condition,
                      string filename,
                      vector<vector<unordered_map<string, bool>>> curOptionPicked,
                      vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                      int numline,
                      string format,
                      string masterformat,
                      AnimationUtility utility)
    {
        if (condition[0] == '(')
        {
            size_t backB   = condition.find(")", 0);
            string inHouse = condition.substr(1, backB - 1);
            size_t x       = inHouse.find("&");
            size_t y       = inHouse.find("|");

            bool inHouseResult;

            if (x == NOT_FOUND && y == NOT_FOUND)
            {
                bool isNot = false;

                if (inHouse[0] == '!')
                {
                    isNot   = true;
                    inHouse = inHouse.substr(1);
                }

                inHouseResult = conditionProcess(inHouse,
                                                 masterformat,
                                                 format,
                                                 filename,
                                                 numline,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 isNot,
                                                 utility);
            }
            else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
            {
                string firstCondition  = inHouse.substr(0, inHouse.find("|"));
                string secondCondition = inHouse.substr(inHouse.find("|") + 1);
                bool isNot             = false;

                if (firstCondition[0] == '!')
                {
                    isNot          = true;
                    firstCondition = firstCondition.substr(1);
                }

                inHouseResult = conditionProcess(firstCondition,
                                                 masterformat,
                                                 format,
                                                 filename,
                                                 numline,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 isNot,
                                                 utility);

                if (!inHouseResult
                    && backup::newCondition(secondCondition,
                                            filename,
                                            curOptionPicked,
                                            groupAnimInfo,
                                            numline,
                                            format,
                                            masterformat,
                                            utility))
                { inHouseResult = true; }
            }
            else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
            {
                string firstCondition      = inHouse.substr(0, inHouse.find("&"));
                string tempSecondCondition = inHouse.substr(inHouse.find("&") + 1);
                string secondCondition;
                bool isNot = false;

                if (firstCondition[0] == '!')
                {
                    isNot          = true;
                    firstCondition = firstCondition.substr(1);
                }

                size_t x = tempSecondCondition.find("&");
                size_t y = tempSecondCondition.find("|");

                if ((x == NOT_FOUND || x > y) && y != NOT_FOUND)
                {
                    secondCondition     = tempSecondCondition.substr(0, tempSecondCondition.find("|"));
                    tempSecondCondition = tempSecondCondition.substr(tempSecondCondition.find("|") + 1);

                    if (conditionProcess(firstCondition,
                                         masterformat,
                                         format,
                                         filename,
                                         numline,
                                         curOptionPicked,
                                         groupAnimInfo,
                                         isNot,
                                         utility))
                    {
                        if (error) throw nemesis::exception();

                        inHouseResult = backup::newCondition(secondCondition,
                                                             filename,
                                                             curOptionPicked,
                                                             groupAnimInfo,
                                                             numline,
                                                             format,
                                                             masterformat,
                                                             utility);
                    }
                    else
                    {
                        inHouseResult = false;
                    }

                    if (error) throw nemesis::exception();

                    if (!inHouseResult
                        && backup::newCondition(tempSecondCondition,
                                                filename,
                                                curOptionPicked,
                                                groupAnimInfo,
                                                numline,
                                                format,
                                                masterformat,
                                                utility))
                    { inHouseResult = true; }
                }
                else
                {
                    secondCondition = tempSecondCondition;

                    if (conditionProcess(firstCondition,
                                         masterformat,
                                         format,
                                         filename,
                                         numline,
                                         curOptionPicked,
                                         groupAnimInfo,
                                         isNot,
                                         utility))
                    {
                        if (error) throw nemesis::exception();

                        inHouseResult = backup::newCondition(secondCondition,
                                                             filename,
                                                             curOptionPicked,
                                                             groupAnimInfo,
                                                             numline,
                                                             format,
                                                             masterformat,
                                                             utility);
                    }
                    else
                    {
                        inHouseResult = false;
                    }

                    if (error) throw nemesis::exception();
                }
            }
            else
            {
                ErrorMessage(1105, format, filename, numline);
            }

            string outHouse = condition.substr(backB + 1);

            x = outHouse.find("&");
            y = outHouse.find("|");

            if (x == NOT_FOUND && y == NOT_FOUND) { return inHouseResult; }
            else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
            {
                string secondCondition = outHouse.substr(outHouse.find("|") + 1);

                if (inHouseResult
                    || backup::newCondition(secondCondition,
                                            filename,
                                            curOptionPicked,
                                            groupAnimInfo,
                                            numline,
                                            format,
                                            masterformat,
                                            utility))
                    return true;
            }
            else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
            {
                string secondCondition = inHouse.substr(inHouse.find("&") + 1);

                if (inHouseResult
                    && backup::newCondition(secondCondition,
                                            filename,
                                            curOptionPicked,
                                            groupAnimInfo,
                                            numline,
                                            format,
                                            masterformat,
                                            utility))
                    return true;
            }
            else
            {
                ErrorMessage(1106, format, filename, numline, condition);
            }
        }
        else
        {
            size_t x = condition.find("&");
            size_t y = condition.find("|");

            if (x == NOT_FOUND && y == NOT_FOUND)
            {
                string conditionOrder = condition;
                bool isNot            = false;

                if (conditionOrder[0] == '!')
                {
                    isNot          = true;
                    conditionOrder = conditionOrder.substr(1);
                }

                return conditionProcess(conditionOrder,
                                        masterformat,
                                        format,
                                        filename,
                                        numline,
                                        curOptionPicked,
                                        groupAnimInfo,
                                        isNot,
                                        utility);
            }
            else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
            {
                string firstCondition  = condition.substr(0, condition.find("|"));
                string secondCondition = condition.substr(condition.find("|") + 1);
                bool isNot             = false;

                if (firstCondition[0] == '!')
                {
                    isNot          = true;
                    firstCondition = firstCondition.substr(1);
                }

                if (conditionProcess(firstCondition,
                                     masterformat,
                                     format,
                                     filename,
                                     numline,
                                     curOptionPicked,
                                     groupAnimInfo,
                                     isNot,
                                     utility))
                    return true;
                if (error) throw nemesis::exception();
                if (backup::newCondition(secondCondition,
                                         filename,
                                         curOptionPicked,
                                         groupAnimInfo,
                                         numline,
                                         format,
                                         masterformat,
                                         utility))
                    return true;
            }
            else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
            {
                string firstCondition  = condition.substr(0, condition.find("&"));
                string secondCondition = condition.substr(condition.find("&") + 1);
                bool isNot             = false;

                if (firstCondition[0] == '!')
                {
                    isNot          = true;
                    firstCondition = firstCondition.substr(1);
                }

                if (y != NOT_FOUND)
                {
                    if (secondCondition[0] == '(')
                    {
                        int position;
                        int openCounter = 0;

                        for (uint i = 0; i < secondCondition.size(); ++i)
                        {
                            if (secondCondition[i] == '(') { openCounter++; }
                            else if (secondCondition[i] == ')')
                            {
                                openCounter--;

                                if (openCounter == 0)
                                {
                                    position = i + 1;
                                    break;
                                }
                            }
                        }

                        string thirdCondition = secondCondition.substr(position);
                        secondCondition       = secondCondition.substr(1, position - 2);

                        if (thirdCondition.length() == 0)
                        {
                            if (conditionProcess(firstCondition,
                                                 masterformat,
                                                 format,
                                                 filename,
                                                 numline,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 isNot,
                                                 utility))
                            {
                                if (error) throw nemesis::exception();
                                if (backup::newCondition(secondCondition,
                                                         filename,
                                                         curOptionPicked,
                                                         groupAnimInfo,
                                                         numline,
                                                         format,
                                                         masterformat,
                                                         utility))
                                    return true;
                            }

                            if (error) throw nemesis::exception();
                        }
                        else
                        {
                            char logic     = thirdCondition[0];
                            thirdCondition = thirdCondition.substr(1);

                            if (logic == '&')
                            {
                                if (conditionProcess(firstCondition,
                                                     masterformat,
                                                     format,
                                                     filename,
                                                     numline,
                                                     curOptionPicked,
                                                     groupAnimInfo,
                                                     isNot,
                                                     utility))
                                {
                                    if (error) throw nemesis::exception();
                                    if (backup::newCondition(secondCondition,
                                                             filename,
                                                             curOptionPicked,
                                                             groupAnimInfo,
                                                             numline,
                                                             format,
                                                             masterformat,
                                                             utility)
                                        && backup::newCondition(thirdCondition,
                                                                filename,
                                                                curOptionPicked,
                                                                groupAnimInfo,
                                                                numline,
                                                                format,
                                                                masterformat,
                                                                utility))
                                        return true;
                                }
                            }
                            else if (logic == '|')
                            {
                                if (conditionProcess(firstCondition,
                                                     masterformat,
                                                     format,
                                                     filename,
                                                     numline,
                                                     curOptionPicked,
                                                     groupAnimInfo,
                                                     isNot,
                                                     utility))
                                {
                                    if (backup::newCondition(secondCondition,
                                                             filename,
                                                             curOptionPicked,
                                                             groupAnimInfo,
                                                             numline,
                                                             format,
                                                             masterformat,
                                                             utility))
                                        return true;
                                }

                                if (backup::newCondition(thirdCondition,
                                                         filename,
                                                         curOptionPicked,
                                                         groupAnimInfo,
                                                         numline,
                                                         format,
                                                         masterformat,
                                                         utility))
                                    return true;
                            }
                            else
                            {
                                ErrorMessage(1101, format, filename, numline);
                            }
                        }
                    }
                    else
                    {
                        x = secondCondition.find("&");
                        y = secondCondition.find("|");

                        if (x == NOT_FOUND && y == NOT_FOUND)
                        {
                            if (conditionProcess(firstCondition,
                                                 masterformat,
                                                 format,
                                                 filename,
                                                 numline,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 isNot,
                                                 utility))
                            {
                                if (error) throw nemesis::exception();

                                bool isAnotherNot = false;

                                if (secondCondition[0] == '!')
                                {
                                    isAnotherNot    = true;
                                    secondCondition = secondCondition.substr(1);
                                }

                                if (conditionProcess(secondCondition,
                                                     masterformat,
                                                     format,
                                                     filename,
                                                     numline,
                                                     curOptionPicked,
                                                     groupAnimInfo,
                                                     isAnotherNot,
                                                     utility))
                                    return true;
                            }
                        }
                        else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
                        {
                            size_t position       = secondCondition.find("|") + 1;
                            string thirdCondition = secondCondition.substr(position);
                            secondCondition       = secondCondition.substr(0, position - 1);

                            if (conditionProcess(firstCondition,
                                                 masterformat,
                                                 format,
                                                 filename,
                                                 numline,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 isNot,
                                                 utility))
                            {
                                if (backup::newCondition(secondCondition,
                                                         filename,
                                                         curOptionPicked,
                                                         groupAnimInfo,
                                                         numline,
                                                         format,
                                                         masterformat,
                                                         utility))
                                    return true;
                            }

                            if (backup::newCondition(thirdCondition,
                                                     filename,
                                                     curOptionPicked,
                                                     groupAnimInfo,
                                                     numline,
                                                     format,
                                                     masterformat,
                                                     utility))
                                return true;
                        }
                        else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
                        {
                            size_t position       = secondCondition.find("&") + 1;
                            string thirdCondition = secondCondition.substr(position);
                            secondCondition       = secondCondition.substr(0, position - 1);

                            if (conditionProcess(firstCondition,
                                                 masterformat,
                                                 format,
                                                 filename,
                                                 numline,
                                                 curOptionPicked,
                                                 groupAnimInfo,
                                                 isNot,
                                                 utility))
                            {
                                if (backup::newCondition(secondCondition,
                                                         filename,
                                                         curOptionPicked,
                                                         groupAnimInfo,
                                                         numline,
                                                         format,
                                                         masterformat,
                                                         utility)
                                    && backup::newCondition(thirdCondition,
                                                            filename,
                                                            curOptionPicked,
                                                            groupAnimInfo,
                                                            numline,
                                                            format,
                                                            masterformat,
                                                            utility))
                                    return true;
                            }
                        }
                        else
                        {
                            ErrorMessage(1103, format, filename, numline);
                        }
                    }
                }
                else
                {
                    return conditionProcess(firstCondition,
                                            masterformat,
                                            format,
                                            filename,
                                            numline,
                                            curOptionPicked,
                                            groupAnimInfo,
                                            isNot,
                                            utility);
                }
            }
            else
            {
                ErrorMessage(1102, format, filename, numline);
            }
        }

        return false;
    }
} // namespace backup

VecStr GetOptionInfo(string line,
                     string format,
                     string filename,
                     int numline,
                     vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                     bool allowNoFixAnim,
                     bool isCondition,
                     int groupMulti,
                     int animMulti,
                     int optionMulti,
                     string multiOption)
{
    int lastGroup = groupAnimInfo.size() - 1;
    int limiter   = isCondition ? 4 : 5;
    VecStr optionInfo;

    if (line.find(format + "_group[") != NOT_FOUND && groupAnimInfo.size() != 0
        && groupMulti < int(groupAnimInfo.size()))
    {
        if (sameWordCount(line, format + "_group[") > 1) ErrorMessage(1157, format, filename, numline, line);
        if (error) throw nemesis::exception();

        string templine = line;
        size_t pos      = templine.find(format + "_group[");
        optionInfo.push_back(templine.substr(pos, format.length()));
        templine = templine.substr(templine.find("[", pos) + 1);

        while (true)
        {
            pos = templine.find("]");
            optionInfo.push_back(templine.substr(0, pos));
            size_t optionLength = optionInfo.back().length() + 1;

            if (templine.length() <= optionLength || templine[optionLength] != '[') break;

            templine = templine.substr(templine.find("[") + 1);
        }

        optionInfo[1] = optionOrderProcess(
            optionInfo[1], format, filename, numline, groupMulti, lastGroup, isCondition, allowNoFixAnim);

        if (error) throw nemesis::exception();
        if (optionInfo[1] != "" && !isOnlyNumber(optionInfo[1]))
            ErrorMessage(1121, format, filename, numline, line);

        int curGroup = stoi(optionInfo[1]);

        if (optionInfo.size() > 3)
        {
            optionInfo[2] = optionOrderProcess(optionInfo[2],
                                               format,
                                               filename,
                                               numline,
                                               animMulti,
                                               groupAnimInfo[groupMulti].size() - 1,
                                               isCondition,
                                               allowNoFixAnim);

            if (error) throw nemesis::exception();
            if (optionInfo[2] != "" && !isOnlyNumber(optionInfo[2]))
                ErrorMessage(1121, format, filename, numline, line);

            int curOrder = stoi(optionInfo[2]);

            if (optionInfo[3].back() == '*')
            {
                string option = optionInfo[3].substr(0, optionInfo[3].length() - 1);
                SSMap* mapPtr = &groupAnimInfo[curGroup][curOrder]->mixOptRegis;

                if (mapPtr->find(option) != mapPtr->end() && (*mapPtr)[option].length() != 0)
                    optionInfo[3] = (*mapPtr)[option];
                else
                    ErrorMessage(1109, format, filename, numline, line);
            }

            optionLimiter(optionInfo,
                          line,
                          format,
                          filename,
                          numline,
                          groupAnimInfo,
                          3,
                          isCondition,
                          curGroup,
                          curOrder,
                          limiter,
                          optionMulti);
        }
        else if (isOnlyNumber(optionInfo[2]))
        {
            ErrorMessage(1057, format, filename, numline, line);
        }

        if (error) throw nemesis::exception();
    }
    else if (line.find(format + "[") != NOT_FOUND && groupAnimInfo.size() != 0
             && groupMulti < int(groupAnimInfo.size()))
    {
        nonGroupOptionInfo(optionInfo,
                           line,
                           format,
                           filename,
                           numline,
                           groupAnimInfo,
                           allowNoFixAnim,
                           isCondition,
                           groupMulti,
                           animMulti,
                           optionMulti,
                           multiOption);
        optionLimiter(optionInfo,
                      line,
                      format,
                      filename,
                      numline,
                      groupAnimInfo,
                      3,
                      isCondition,
                      stoi(optionInfo[1]),
                      stoi(optionInfo[2]),
                      limiter,
                      optionMulti);
    }
    else if (multiOption == format && format.find("_group") == NOT_FOUND)
    {
        nonGroupOptionInfo(optionInfo,
                           line,
                           format,
                           filename,
                           numline,
                           groupAnimInfo,
                           allowNoFixAnim,
                           isCondition,
                           groupMulti,
                           animMulti,
                           optionMulti,
                           multiOption);

        if (optionInfo.back().find("import[") == NOT_FOUND && optionInfo.back().find("crc32[") == NOT_FOUND
            && optionInfo.back().find("eventID[") == NOT_FOUND
            && optionInfo.back().find("variableID[") == NOT_FOUND)
        {
            optionLimiter(optionInfo,
                          line,
                          format,
                          filename,
                          numline,
                          groupAnimInfo,
                          3,
                          isCondition,
                          stoi(optionInfo[1]),
                          stoi(optionInfo[2]),
                          limiter,
                          optionMulti);
        }
    }
    else
    {
        ErrorMessage(1192, format, filename, numline, line);
    }

    return optionInfo;
}

void nonGroupOptionInfo(VecStr& optionInfo,
                        string line,
                        string format,
                        string filename,
                        int numline,
                        vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                        bool allowNoFixAnim,
                        bool isCondition,
                        int groupMulti,
                        int animMulti,
                        int optionMulti,
                        string multiOption)
{
    string templine = line;
    size_t pos      = templine.find(format + "[");
    optionInfo.push_back(format + "_group");
    templine = templine.substr(templine.find("[", pos) + 1);

    if (groupMulti == -1) ErrorMessage(1056, format, filename, numline, line);

    optionInfo.push_back(to_string(groupMulti));

    if (line.find(format + "[") == NOT_FOUND && line.find(format + "_group[") == NOT_FOUND
        && multiOption == format)
    {
        if (animMulti == -1) ErrorMessage(1056, format, filename, numline, line);

        optionInfo.push_back(to_string(animMulti));
        optionInfo.push_back(line);
        return;
    }

    while (true)
    {
        pos = templine.find("]");
        optionInfo.push_back(templine.substr(0, pos));
        size_t optionLength = optionInfo.back().length() + 1;

        if (templine.length() <= optionLength || templine[optionLength] != '[') break;

        templine = templine.substr(templine.find("[") + 1);
    }

    optionInfo[2] = optionOrderProcess(optionInfo[2],
                                       format,
                                       filename,
                                       numline,
                                       animMulti,
                                       groupAnimInfo[groupMulti].size() - 1,
                                       isCondition,
                                       allowNoFixAnim);

    if (error) throw nemesis::exception();

    if (!isOnlyNumber(optionInfo[2])) ErrorMessage(1057, format, filename, numline, line);

    int curOrder = stoi(optionInfo[2]);

    if (optionInfo[3].back() == '*')
    {
        string option = optionInfo[3].substr(0, optionInfo[3].length() - 1);
        SSMap* mapPtr = &groupAnimInfo[groupMulti][curOrder]->mixOptRegis;

        if (mapPtr->find(option) != mapPtr->end() && (*mapPtr)[option].length() != 0)
            optionInfo.back() = (*mapPtr)[option];
        else
            ErrorMessage(1109, format, filename, numline, line);
    }
}

void optionLimiter(VecStr optionInfo,
                   string line,
                   string format,
                   string filename,
                   int numline,
                   vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                   int open,
                   bool isCondition,
                   int curGroup,
                   int curOrder,
                   int limiter,
                   int optionMulti)
{
    if (!isCondition && int(optionInfo.size()) > open + 1)
    {
        if (optionInfo.size() == open + 3)
        {
            bool isPassed = true;

            if (optionInfo[open + 2].length() == 0)
            {
                if (optionMulti == -1) ErrorMessage(1060, format, filename, numline, line);

                optionInfo[open + 2] = to_string(optionMulti);
            }
            else if (!isOnlyNumber(optionInfo[open + 2]))
            {
                ErrorMessage(1055, format, filename, numline, line);
            }
        }

        if (groupAnimInfo[curGroup][curOrder]->groupAddition[optionInfo[open]][optionInfo[open + 1]].size()
            != 0)
            limiter++;
    }

    if (!isCondition && optionInfo.size() > 3
        && (optionInfo[3] == "main_anim_event" || optionInfo[3] == "[main_anim_event]"
            || optionInfo[3] == "FilePath" || optionInfo[3].find("@AnimObject") != NOT_FOUND
            || optionInfo[3].find("(S+") != NOT_FOUND))
        limiter--;

    if (int(optionInfo.size()) != limiter) ErrorMessage(1054, limiter - 1, format, filename, numline, line);

    /* testing for allowing order condition
	if (optionInfo[open][0] == '^')
	{
		ErrorMessage(1149, format, filename, numline, line);
	}
	*/

    if (error) throw nemesis::exception();
}

string optionOrderProcess(string line,
                          string format,
                          string filename,
                          int numline,
                          int groupMulti,
                          int lastOrder,
                          bool isCondition,
                          bool allowNoFixAnim)
{
    string newline = line;

    if (newline.length() == 0)
    {
        if (!allowNoFixAnim) { ErrorMessage(1056, format, filename, numline, line); }
        else if (groupMulti == -1)
        {
            if (!isCondition) ErrorMessage(1056, format, filename, numline, line);
        }
        else
        {
            newline = to_string(groupMulti);
        }
    }
    else if (newline == "F")
        newline = "0";
    else if (newline == "L")
        newline = to_string(lastOrder);
    else if (newline == "N")
        newline = lastOrder - 1 == groupMulti ? to_string(groupMulti) : to_string(groupMulti + 1);
    else if (newline == "B")
        newline = groupMulti == 0 ? to_string(groupMulti) : to_string(groupMulti - 1);
    else if (newline[0] == '(' && newline[newline.length() - 1] == ')')
    {
        int Fnum = sameWordCount(newline, "F");
        int Nnum = sameWordCount(newline, "N");
        int Bnum = sameWordCount(newline, "B");
        int Lnum = sameWordCount(newline, "L");

        for (int i = 0; i < Fnum; ++i)
        {
            newline.replace(newline.find("F"), 1, "0");
        }

        if (lastOrder == groupMulti)
        {
            for (int i = 0; i < Nnum; ++i)
            {
                newline.replace(newline.find("N"), 1, to_string(groupMulti));
            }
        }
        else
        {
            for (int i = 0; i < Nnum; ++i)
            {
                newline.replace(newline.find("N"), 1, to_string(groupMulti + 1));
            }
        }

        if (groupMulti == 0)
        {
            for (int i = 0; i < Bnum; ++i)
            {
                newline.replace(newline.find("B"), 1, to_string(groupMulti));
            }
        }
        else
        {
            for (int i = 0; i < Bnum; ++i)
            {
                newline.replace(newline.find("B"), 1, to_string(groupMulti - 1));
            }
        }

        for (int i = 0; i < Lnum; ++i)
        {
            newline.replace(newline.find("L"), 1, to_string(lastOrder));
        }

        nemesis::calculate(newline, format, filename, numline);
    }
    else
    {
        string templine = newline;
        templine        = templine + "a";
        string newtempline
            = nemesis::regex_replace(string(templine), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

        if (newtempline == templine) ErrorMessage(1055, format, filename, numline, line);
        if (stoi(newline) > int(lastOrder)) ErrorMessage(1148, format, filename, numline, line);
    }

    if (error) throw nemesis::exception();

    return newline;
}

bool conditionProcess(string condition,
                      string masterformat,
                      string format,
                      string filename,
                      int numline,
                      vector<vector<unordered_map<string, bool>>> curOptionPicked,
                      vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                      bool isNot,
                      AnimationUtility utility)
{
    if (condition.find(masterformat + "_group[][") != NOT_FOUND)
    {
        int GOG = 0;

        while (GOG < int(groupAnimInfo.size()))
        {
            int formatGroup = 0;
            int groupEnd    = 1;
            bool conditionResult;

            if (condition.find(masterformat + "_group[][]") != NOT_FOUND)
            { groupEnd = int(groupAnimInfo[GOG].size()); }
            else
            {
                groupEnd      = int(condition.find(masterformat + "_group[][")) + masterformat.length() + 9;
                string number = condition.substr(groupEnd, condition.find("]", groupEnd) - groupEnd);
                optionOrderProcess(number,
                                   masterformat,
                                   filename,
                                   numline,
                                   groupAnimInfo[GOG].size() - 1,
                                   groupAnimInfo[GOG].size() - 1,
                                   true,
                                   true);

                if (number.length() > 0)
                {
                    formatGroup = stoi(number);
                    groupEnd    = formatGroup + 1;
                }
                else
                {
                    formatGroup = 0;
                    groupEnd    = int(groupAnimInfo[GOG].size());
                }
            }

            while (formatGroup < groupEnd)
            {
                VecStr optionInfo = GetOptionInfo(condition,
                                                  masterformat,
                                                  filename,
                                                  numline,
                                                  groupAnimInfo,
                                                  true,
                                                  true,
                                                  GOG,
                                                  formatGroup,
                                                  utility.optionMulti);

                if (error) throw nemesis::exception();

                if (curOptionPicked[stoi(optionInfo[1])][stoi(optionInfo[2])][optionInfo[3]])
                    conditionResult = !isNot;
                else
                    conditionResult = isNot;

                if (!conditionResult) return false;

                ++formatGroup;
            }

            ++GOG;
        }
    }

    if (error) throw nemesis::exception();

    if (condition.find(masterformat + "[][") != NOT_FOUND)
    {
        if (utility.groupMulti == -1) ErrorMessage(1056, masterformat, filename, numline, condition);

        int formatGroup = 0;
        bool conditionResult;

        while (formatGroup < int(groupAnimInfo[utility.groupMulti].size()))
        {
            VecStr optionInfo = GetOptionInfo(condition,
                                              masterformat,
                                              filename,
                                              numline,
                                              groupAnimInfo,
                                              true,
                                              true,
                                              utility.groupMulti,
                                              formatGroup,
                                              utility.optionMulti);

            if (utility.multiOption == masterformat && optionInfo.back()[0] == '^'
                && optionInfo.back().back() == '^' && optionInfo.back().length() > 2)
            {
                string conditionOrder;

                if (isalpha(optionInfo.back()[1]))
                {
                    conditionOrder = nemesis::regex_replace(
                        string(optionInfo.back()), nemesis::regex("\\^([A-Za-z]+)\\^"), string("\\1"));

                    if (nemesis::iequals(conditionOrder, "last"))
                        conditionResult = utility.animMulti == groupAnimInfo.size() - 1 ? !isNot : isNot;
                    else if (nemesis::iequals(conditionOrder, "first"))
                        conditionOrder = "0";
                    else
                        ErrorMessage(1138, format, filename, numline, condition);
                }
                else
                {
                    conditionOrder = optionInfo.back().substr(1, optionInfo.back().length() - 2);

                    if (!isOnlyNumber(conditionOrder))
                        ErrorMessage(1138, format, filename, numline, condition);
                }

                conditionResult = utility.animMulti == stoi(conditionOrder) ? !isNot : isNot;
            }
            else
            {
                conditionResult = curOptionPicked[stoi(optionInfo[1])][stoi(optionInfo[2])][optionInfo[3]]
                                      ? !isNot
                                      : isNot;
            }

            if (!conditionResult) return false;

            ++formatGroup;
        }
    }
    else
    {
        VecStr optionInfo = GetOptionInfo(condition,
                                          masterformat,
                                          filename,
                                          numline,
                                          groupAnimInfo,
                                          false,
                                          true,
                                          utility.groupMulti,
                                          utility.animMulti,
                                          utility.optionMulti,
                                          utility.multiOption);

        if (utility.multiOption == masterformat && optionInfo.back()[0] == '^'
            && optionInfo.back().back() == '^' && optionInfo.back().length() > 2)
        {
            string conditionOrder;

            if (isalpha(optionInfo.back()[1]))
            {
                conditionOrder = nemesis::regex_replace(
                    string(optionInfo.back()), nemesis::regex("\\^([A-Za-z]+)\\^"), string("\\1"));

                if (nemesis::iequals(conditionOrder, "last"))
                    return utility.animMulti == groupAnimInfo.size() - 1 ? !isNot : isNot;
                else if (nemesis::iequals(conditionOrder, "first"))
                    conditionOrder = "0";
                else
                    ErrorMessage(1138, format, filename, numline, condition);
            }
            else
            {
                conditionOrder = optionInfo.back().substr(1, optionInfo.back().length() - 2);

                if (!isOnlyNumber(conditionOrder)) ErrorMessage(1138, format, filename, numline, condition);
            }

            return utility.animMulti == stoi(conditionOrder) ? !isNot : isNot;
        }
        else if (curOptionPicked[stoi(optionInfo[1])][stoi(optionInfo[2])][optionInfo[3]])
        {
            return !isNot;
        }
        else
        {
            return isNot;
        }
    }

    if (error) throw nemesis::exception();

    return true;
}

inline void GroupTemplate::newID()
{
    ++nextFunctionID;

    if (nextFunctionID == 9216) ++nextFunctionID;

    strID = to_string(nextFunctionID);

    while (strID.length() < 4)
    {
        strID = "0" + strID;
    }
}

void GroupTemplate::setZeroEvent(string eventname)
{
    zeroEvent = eventname;
}

void GroupTemplate::setZeroVariable(string variablename)
{
    zeroVariable = variablename;
}

inline void ExistingFunction::newID()
{
    ++(*nextFunctionID);

    if (*nextFunctionID == 9216) ++(*nextFunctionID);

    strID = to_string(*nextFunctionID);

    while (strID.length() < 4)
    {
        strID = "0" + strID;
    }
}

void ExistingFunction::setZeroEvent(string eventname)
{
    zeroEvent = eventname;
}

void ExistingFunction::setZeroVariable(string variablename)
{
    zeroVariable = variablename;
}

int formatGroupReplace(string& curline,
                       string oriline,
                       int point,
                       string filename,
                       string format,
                       shared_ptr<master> subFunctionIDs,
                       vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
                       int linecount,
                       int groupMulti,
                       int optionMulti,
                       int animMulti,
                       string multiOption,
                       bool& innerError)
{
    int open = 0;
    int curGroup;
    int curAnim;
    int curPoint = curline.length();

    if (animMulti > -1 && groupMulti > -1)
    {
        for (uint i = point; i < curline.length(); ++i)
        {
            if (curline[i] == '[')
            {
                ++open;

                if (open > 1)
                {
                    i = formatGroupReplace(curline,
                                           oriline,
                                           curGroup,
                                           filename,
                                           format,
                                           subFunctionIDs,
                                           groupAnimInfo,
                                           linecount,
                                           groupMulti,
                                           optionMulti,
                                           animMulti,
                                           multiOption,
                                           innerError);
                    --open;

                    if (innerError || error) throw nemesis::exception();
                }

                curGroup = i + 1;
            }
            else if (curline[i] == ']')
            {
                --open;

                if (open < 0) break;

                curPoint = i + 1;
            }
        }
    }
    else
    {
        for (uint i = point; i < curline.length(); ++i)
        {
            if (curline[i] == '[')
            {
                ++open;

                if (open > 1)
                {
                    i = formatGroupReplace(curline,
                                           oriline,
                                           curGroup,
                                           filename,
                                           format,
                                           subFunctionIDs,
                                           groupAnimInfo,
                                           linecount,
                                           groupMulti,
                                           optionMulti,
                                           animMulti,
                                           multiOption,
                                           innerError);
                    --open;

                    if (innerError || error) throw nemesis::exception();
                }

                curGroup = i + 1;
            }
            else if (curline[i] == ']')
            {
                --open;

                if (open < 0) break;

                curPoint = i + 1;
            }
        }
    }

    if (open > 0) { ErrorMessage(1057, format + "_group", filename, linecount, oriline); }

    if (curline != oriline)
    {
        if (curline.find("[") == NOT_FOUND)
            return 0;
        else if (curline.find(format + "[") == NOT_FOUND && curline.find(format + "_group[") == NOT_FOUND)
            return 0;
    }

    string originalLine = curline.substr(point, curPoint - point);
    VecStr groupline    = GetOptionInfo(originalLine,
                                     format,
                                     filename,
                                     linecount,
                                     groupAnimInfo,
                                     true,
                                     false,
                                     groupMulti,
                                     animMulti,
                                     optionMulti,
                                     multiOption);

    if (error) throw nemesis::exception();

    string input;
    string output;
    curGroup = stoi(groupline[1]);
    curAnim  = animMulti;

    if (isOnlyNumber(groupline[2]))
    {
        open    = 4;
        curAnim = stoi(groupline[2]);
        input   = groupline[3];
    }
    else
    {
        open  = 3;
        input = groupline[2];
    }

    if (groupline.back().find("@AnimObject/") != NOT_FOUND)
    {
        if (optionMulti == -1) ErrorMessage(1128, format + "_group", filename, linecount);
        if (groupline.size() < 5) groupline.push_back(to_string(optionMulti));

        for (uint d = open; d < groupline.size(); ++d)
        {
            input.append("[" + groupline[d] + "]");
        }
    }
    else if (groupline.back().length() == 0)
    {
        if (optionMulti == -1) ErrorMessage(1128, format + "_group", filename, linecount);

        groupline.back().append(to_string(optionMulti));

        for (uint d = open; d < groupline.size() - 1; ++d)
        {
            input = input + "[" + groupline[d] + "]";
        }
    }
    else
    {
        for (uint d = open; d < groupline.size(); ++d)
        {
            input = input + "[" + groupline[d] + "]";
        }
    }

    if (open == 4)
    {
        unordered_map<string, string>* mapPtr
            = &subFunctionIDs->grouplist[stoi(groupline[1])]->singlelist[stoi(groupline[2])]->format;
        shared_ptr<AnimationInfo> animInfo = groupAnimInfo[stoi(groupline[1])][stoi(groupline[2])];

        if (mapPtr->find(input) != mapPtr->end() && (*mapPtr)[input].length() > 0)
        { output = (*mapPtr)[input]; }
        else if (animInfo->addOn.find(groupline[3]) != animInfo->addOn.end()
                 && animInfo->addOn[groupline[3]].size() > 0 && optionMulti > -1)
        {
            addOnReplacer(input,
                          filename,
                          animInfo->addOn,
                          animInfo->addition,
                          animInfo->groupAddition,
                          animInfo->mixOptRegis,
                          optionMulti,
                          format,
                          linecount);
        }
        else if (multiOption == format && groupMulti != -1 && animMulti != -1)
        {
            for (uint i = point; i < curline.length(); ++i)
            {
                if (curline[i] == '[') { curGroup = i + 1; }
                else if (curline[i] == ']')
                {
                    string templine = curline.substr(curGroup, i - curGroup);

                    if (templine.find("[") == NOT_FOUND && templine.find("]") == NOT_FOUND)
                    {
                        unordered_map<std::string, std::string>* formatOut
                            = &subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format;

                        if (formatOut->find(templine) != formatOut->end()
                            && (*formatOut)[templine].length() > 0)
                        {
                            curline.replace(
                                curline.find(templine), templine.length(), (*formatOut)[templine]);
                        }
                        else
                        {
                            for (auto& match : *formatOut)
                            {
                                if (templine.find(match.first) != NOT_FOUND)
                                    curline.replace(curline.find(templine), templine.length(), match.second);
                            }
                        }
                    }
                }
            }

            return point;
        }
    }
    else
    {
        output = subFunctionIDs->grouplist[stoi(groupline[1])]->functionIDs[input];
    }

    if (output.length() == 0 || groupline.size() < 3)
    {
        if (curline.find(format + "[") != NOT_FOUND || curline.find(format + "_group[") != NOT_FOUND)
            ErrorMessage(1158, format + "_group", filename, linecount, curline);
    }
    else
    {
        curline.replace(point, originalLine.length(), output);
    }

    if (error) throw nemesis::exception();

    return point;
}

void OutputCheckGroup(string format,
                      string behaviorFile,
                      shared_ptr<VecStr> generatedlines,
                      proc& process,
                      condset* curset,
                      bool& elementCatch,
                      bool isMaster,
                      int& openRange,
                      size_t& elementLine,
                      int& counter,
                      int groupCount,
                      ID& eventid,
                      ID& variableid)
{
    for (auto& curstack : curset->lines)
    {
        bool uniqueskip = false;
        bool hasProcess = false;
        elementCatch    = false;
        string line;
        VecStr lineblocks;

        if (curstack.nestedcond.size() == 0)
        {
            hasProcess = curstack.hasProcess;

            if (hasProcess)
                lineblocks = curstack.lineblocks;
            else
                line = curstack.line;
        }
        else
        {
            size_t condcount = 0;

            while (condcount < curstack.nestedcond.size())
            {
                condset& curcond = curstack.nestedcond[condcount];

                // FOREACH
                if (curcond.isMulti)
                {
                    int groupMulti = -2;
                    int animMulti  = -2;

                    if (curcond.n_conditions->isMultiTrue(&process,
                                                          format,
                                                          behaviorFile,
                                                          curstack.linecount,
                                                          animMulti,
                                                          true,
                                                          isMaster,
                                                          groupMulti))
                    {
                        // master
                        if (groupMulti == -1)
                        {
                            groupMulti          = process.groupMulti;
                            string oldcond      = process.multiOption;
                            process.multiOption = curcond.conditions;
                            size_t size         = process.curGroup->groupAnimInfo.size();

                            for (size_t m_groupMulti = 0; m_groupMulti < size; ++m_groupMulti)
                            {
                                process.groupMulti = m_groupMulti;
                                OutputCheckGroup(format,
                                                 behaviorFile,
                                                 generatedlines,
                                                 process,
                                                 &curcond,
                                                 elementCatch,
                                                 isMaster,
                                                 openRange,
                                                 elementLine,
                                                 counter,
                                                 groupCount,
                                                 eventid,
                                                 variableid);
                            }

                            process.groupMulti  = groupMulti;
                            process.multiOption = oldcond;
                            break;
                        }

                        // group
                        else
                        {
                            size_t a_size;
                            size_t oldGroup     = process.groupMulti;
                            size_t oldAnim      = process.animMulti;
                            size_t oldOption    = process.optionMulti;
                            string oldcond      = process.multiOption;
                            process.multiOption = curcond.conditions;
                            process.groupMulti  = groupMulti;

                            if (animMulti == -2)
                            {
                                animMulti = 0;
                                a_size    = 1;
                            }
                            else if (animMulti == -1)
                            {
                                animMulti = 0;
                                a_size    = process.curGroup->groupAnimInfo[groupMulti].size();
                            }
                            else
                            {
                                a_size = animMulti + 1;
                            }

                            for (size_t m_animMulti = animMulti; m_animMulti < a_size; ++m_animMulti)
                            {
                                process.animMulti = m_animMulti;

                                for (int m_optionMulti = 0;
                                     m_optionMulti < process.curGroup->groupAnimInfo[groupMulti][m_animMulti]
                                                         ->optionPickedCount[curcond.conditions];
                                     ++m_optionMulti)
                                {
                                    process.optionMulti = m_optionMulti;
                                    OutputCheckGroup(format,
                                                     behaviorFile,
                                                     generatedlines,
                                                     process,
                                                     &curcond,
                                                     elementCatch,
                                                     isMaster,
                                                     openRange,
                                                     elementLine,
                                                     counter,
                                                     groupCount,
                                                     eventid,
                                                     variableid);
                                }
                            }

                            process.groupMulti  = oldGroup;
                            process.animMulti   = oldAnim;
                            process.optionMulti = oldOption;
                            process.multiOption = oldcond;
                            break;
                        }
                    }
                }

                // CONDITION
                else
                {
                    if (!curcond.n_conditions
                        || curcond.n_conditions->isTrue(&process,
                                                        format,
                                                        behaviorFile,
                                                        curstack.linecount,
                                                        true,
                                                        isMaster,
                                                        curcond.n_conditions))
                    {
                        OutputCheckGroup(format,
                                         behaviorFile,
                                         generatedlines,
                                         process,
                                         &curcond,
                                         elementCatch,
                                         isMaster,
                                         openRange,
                                         elementLine,
                                         counter,
                                         groupCount,
                                         eventid,
                                         variableid);
                        break;
                    }
                }

                ++condcount;
            }

            uniqueskip = true;
        }

        if (!uniqueskip)
        {
            while (true)
            {
                if (hasProcess)
                {
                    process.numline = curstack.linecount;
                    process.line    = &line;
                    process.blocksCompile(lineblocks);

                    if (error) throw nemesis::exception();
                }

                generatedlines->push_back(line);

                if (elementCatch) elementLine = generatedlines->size() - 1;

                break;
            }
        }

        if (error) throw nemesis::exception();
    }
}
