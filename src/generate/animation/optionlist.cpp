#include "Global.h"

#include <unordered_set>


#include "debugmsg.h"

#include "utilities/readtextfile.h"
#include "utilities/regex.h"
#include "utilities/stringsplit.h"

#include "generate/animation/optionlist.h"

#pragma warning(disable : 4503)

using namespace std;

bool Debug = false;

OptionList::OptionList()
{}

OptionList::OptionList(string filepath, string format)
{
    templatecode = format;
    unordered_map<string, bool> isAddOn;
    unordered_map<string, VecStr> linked;
    FileReader input(filepath);

    if (input.GetFile())
    {
        int linecount       = 0;
        bool minDone        = false;
        bool ruleDone       = false;
        bool stateDone      = false;
        bool compulsoryDone = false;
        unordered_set<string> matcher;
        unordered_set<string> container;
        unordered_map<string, int> matchLine;
        unordered_map<string, bool> isElementExist;
        unordered_map<int, bool> isNumExist;
        string strline;

        while (input.GetLines(strline))
        {
            ++linecount;

            if (strline.length() != 0 && strline[0] != '\'')
            {
                VecStr AnimInfo;
                StringSplit(strline, AnimInfo);
                string lower = nemesis::to_lower_copy(AnimInfo[0]);

                if (lower == "rules")
                {
                    if (AnimInfo.size() > 3) ErrorMessage(1029, format, filepath, linecount);

                    if (!ruleDone)
                    {
                        string first  = AnimInfo[1];
                        string second = AnimInfo[2];

                        if (strline.find(AnimInfo[0] + " " + first + " " + second) != NOT_FOUND)
                        {
                            // first rule processing
                            __int64 reference = count(first.begin(), first.end(), ',');

                            if (reference != 0)
                            {
                                size_t nextpos = 0;
                                size_t pos     = 0;

                                for (int i = 0; i < reference + 1; ++i)
                                {
                                    if (i != reference)
                                    {
                                        nextpos = first.find(",", nextpos + 1);
                                        ruleOne.push_back(first.substr(pos, nextpos - pos));
                                        pos = nextpos + 1;
                                    }
                                    else
                                    {
                                        ruleOne.push_back(first.substr(pos));
                                    }
                                }
                            }
                            else if (first != "/")
                            {
                                ruleOne.push_back(first);
                            }

                            // second rule processing
                            reference = count(second.begin(), second.end(), ',');

                            if (reference != 0)
                            {
                                size_t nextpos = 0;
                                size_t pos     = 0;

                                for (int i = 0; i < reference + 1; ++i)
                                {
                                    if (i != reference)
                                    {
                                        nextpos = second.find(",", nextpos + 1);
                                        ruleTwo.push_back(second.substr(pos, nextpos - pos));
                                        pos = nextpos + 1;
                                    }
                                    else
                                    {
                                        ruleTwo.push_back(second.substr(pos));
                                    }
                                }
                            }
                            else if (second != "/")
                            {
                                ruleTwo.push_back(second);
                            }
                        }
                        else
                        {
                            ErrorMessage(1029, format, filepath, linecount);
                        }

                        ruleDone = true;
                    }
                    else
                    {
                        ErrorMessage(1040, format, filepath, linecount);
                    }
                }
                else if (lower == "state")
                {
                    if (AnimInfo.size() > 2) ErrorMessage(1013, format, filepath, linecount);

                    if (!stateDone)
                    {
                        string startState         = AnimInfo[1];
                        string filteredStartState = nemesis::regex_replace(
                            string(startState), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                        if (startState == filteredStartState)
                            startStateID = filteredStartState;
                        else
                            ErrorMessage(1013, format, filepath, linecount);

                        stateDone = true;
                    }
                    else
                    {
                        ErrorMessage(1041, format, filepath, linecount);
                    }
                }
                else if (lower == "minimum")
                {
                    if (AnimInfo.size() > 2) ErrorMessage(1194, format, filepath, linecount);

                    if (!minDone)
                    {
                        string number = nemesis::regex_replace(
                            string(AnimInfo[1]), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                        if (strline.find(AnimInfo[0] + " " + number) != NOT_FOUND)
                        {
                            groupMin = stoi(number);
                            minDone  = true;
                        }
                        else
                        {
                            ErrorMessage(1194, format, filepath, linecount);
                        }
                    }
                    else
                    {
                        ErrorMessage(1042, format, filepath, linecount);
                    }
                }
                else if (lower == "ignore_group")
                {
                    ignoreGroup = true;
                }
                else if (lower == "link")
                {
                    for (uint i = 1; i < AnimInfo.size(); ++i)
                    {
                        string currentTab;
                        VecStr linker;

                        for (uint j = 1; j < AnimInfo.size(); ++j)
                        {
                            if (i == j)
                            {
                                currentTab = AnimInfo[j];

                                if (linked[currentTab].size() != 0)
                                    ErrorMessage(1046, currentTab, format, filepath, linecount);
                            }
                            else
                            {
                                linker.push_back(AnimInfo[j]);
                            }
                        }

                        linked[currentTab] = linker;
                    }
                }
                else if (lower == "event")
                {
                    for (uint i = 0; i < strline.length(); ++i)
                    {
                        if (!isalnum(strline[i]) && strline[i] != '_' && strline[i] != '<'
                            && strline[i] != '>' && strline[i] != ' ')
                        { ErrorMessage(1036, format, filepath, linecount); }
                    }

                    if (AnimInfo.size() > 2) ErrorMessage(1195, format, filepath, linecount);

                    string templine = AnimInfo[1];

                    if (isElementExist[templine])
                        ErrorMessage(1063, templine, format, filepath, linecount, templine);

                    isElementExist[templine] = true;

                    if (templine.find("<") != NOT_FOUND
                        && templine.find(">", templine.find("<")) != NOT_FOUND)
                    {
                        __int64 opening = count(strline.begin(), strline.end(), '<');
                        __int64 closing = count(strline.begin(), strline.end(), '>');

                        if (opening == closing)
                        {
                            size_t nextpos = 0;
                            VecStr import;
                            import.reserve(10);

                            for (int i = 0; i < opening; ++i)
                            {
                                nextpos = templine.find("<", nextpos) + 1;

                                if (nextpos == NOT_FOUND) ErrorMessage(1037, format, filepath, linecount);

                                string element
                                    = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);

                                if (!nemesis::iequals(element, "main_anim_event"))
                                {
                                    matcher.insert(element);
                                    matchLine[element] = linecount;
                                }

                                import.push_back(element);
                                templine.replace(
                                    templine.find("<" + element + ">"), element.length() + 2, "$$");
                            }

                            import.shrink_to_fit();
                            eleEvent.push_back(import);
                            eleEventLine.push_back(templine);
                        }
                        else
                        {
                            ErrorMessage(1039, format, filepath, linecount);
                        }
                    }
                    else
                    {
                        VecStr import;
                        eleEvent.push_back(import);
                        eleEventLine.push_back(templine);
                    }
                }
                else if (lower == "event_group")
                {
                    for (uint i = 0; i < strline.length(); ++i)
                    {
                        if (!isalnum(strline[i]) && strline[i] != '_' && strline[i] != '<'
                            && strline[i] != '>' && strline[i] != ' ')
                        { ErrorMessage(1036, format, filepath, linecount); }
                    }

                    if (AnimInfo.size() > 3) ErrorMessage(1196, format, filepath, linecount);

                    bool isLast;

                    if (AnimInfo[1] == "F")
                        isLast = false;
                    else if (AnimInfo[1] == "L")
                        isLast = true;
                    else
                        ErrorMessage(1048, format, filepath, linecount);

                    string templine = AnimInfo[2];

                    if (isElementExist[templine]) ErrorMessage(1063, templine, format, filepath, linecount);

                    if (templine.find("<") != NOT_FOUND
                        && templine.find(">", templine.find("<")) != NOT_FOUND)
                    {
                        __int64 opening = count(strline.begin(), strline.end(), '<');
                        __int64 closing = count(strline.begin(), strline.end(), '>');

                        if (opening == closing)
                        {
                            size_t nextpos = 0;
                            VecStr import;
                            import.reserve(int(opening));

                            for (int i = 0; i < opening; ++i)
                            {
                                nextpos = templine.find("<", nextpos) + 1;

                                if (nextpos == NOT_FOUND) ErrorMessage(1037, format, filepath, linecount);

                                string element
                                    = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);

                                if (!nemesis::iequals(element, "main_anim_event"))
                                {
                                    matcher.insert(element);
                                    matchLine[element] = linecount;
                                }

                                import.push_back(element);
                                templine.replace(
                                    templine.find("<" + element + ">"), element.length() + 2, "$$");
                            }

                            if (isLast)
                            {
                                eleEventGroupL.push_back(import);
                                eleEventGroupLLine.push_back(templine);
                            }
                            else
                            {
                                eleEventGroupF.push_back(import);
                                eleEventGroupFLine.push_back(templine);
                            }
                        }
                        else
                        {
                            ErrorMessage(1039, format, filepath, linecount);
                        }
                    }
                    else
                    {
                        VecStr import;

                        if (isLast)
                        {
                            eleEventGroupL.push_back(import);
                            eleEventGroupLLine.push_back(templine);
                        }
                        else
                        {
                            eleEventGroupF.push_back(import);
                            eleEventGroupFLine.push_back(templine);
                        }
                    }
                }
                else if (lower == "variable")
                {
                    for (uint i = 0; i < strline.length(); ++i)
                    {
                        if (!isalnum(strline[i]) && strline[i] != '_' && strline[i] != '<'
                            && strline[i] != '>' && strline[i] != ' ')
                        { ErrorMessage(1043, format, filepath, linecount); }
                    }

                    if (AnimInfo.size() > 2) ErrorMessage(1197, format, filepath, linecount);

                    string templine = AnimInfo[1];

                    if (templine.find("<") != NOT_FOUND
                        && templine.find(">", templine.find("<")) != NOT_FOUND)
                    {
                        __int64 opening = count(strline.begin(), strline.end(), '<');
                        __int64 closing = count(strline.begin(), strline.end(), '>');

                        if (opening == closing)
                        {
                            size_t nextpos = 0;
                            VecStr import;
                            import.reserve(10);

                            for (int i = 0; i < opening; ++i)
                            {
                                nextpos = templine.find("<", nextpos) + 1;

                                if (nextpos == NOT_FOUND) ErrorMessage(1037, format, filepath, linecount);

                                string element
                                    = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);
                                matcher.insert(element);
                                matchLine[element] = linecount;
                                import.push_back(element);
                                templine.replace(
                                    templine.find("<" + element + ">"), element.length() + 2, "$$");
                            }

                            import.shrink_to_fit();
                            eleVar.push_back(import);
                            eleVarLine.push_back(templine);
                        }
                        else
                        {
                            ErrorMessage(1039, format, filepath, linecount);
                        }
                    }
                    else
                    {
                        VecStr import;
                        eleVar.push_back(import);
                        eleVarLine.push_back(templine);
                    }
                }
                else if (lower == "variable_group")
                {
                    for (uint i = 0; i < strline.length(); ++i)
                    {
                        if (!isalnum(strline[i]) && strline[i] != '_' && strline[i] != '<'
                            && strline[i] != '>' && strline[i] != ' ')
                        { ErrorMessage(1043, format, filepath, linecount); }
                    }

                    if (AnimInfo.size() > 3) ErrorMessage(1198, format, filepath, linecount);

                    bool isLast;

                    if (AnimInfo[1] == "F")
                        isLast = false;
                    else if (AnimInfo[1] == "L")
                        isLast = true;
                    else
                        ErrorMessage(1048, format, filepath, linecount);

                    string templine = AnimInfo[2];

                    if (templine.find("<") != NOT_FOUND
                        && templine.find(">", templine.find("<")) != NOT_FOUND)
                    {
                        __int64 opening = count(strline.begin(), strline.end(), '<');
                        __int64 closing = count(strline.begin(), strline.end(), '>');

                        if (opening == closing)
                        {
                            size_t nextpos = 0;
                            VecStr import;
                            import.reserve(10);

                            for (int i = 0; i < opening; ++i)
                            {
                                nextpos = templine.find("<", nextpos) + 1;

                                if (nextpos == NOT_FOUND) ErrorMessage(1037, format, filepath, linecount);

                                string element
                                    = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);
                                matcher.insert(element);
                                matchLine[element] = linecount;
                                import.push_back(element);
                                templine.replace(
                                    templine.find("<" + element + ">"), element.length() + 2, "$$");
                            }

                            import.shrink_to_fit();

                            if (isLast)
                            {
                                eleVarGroupL.push_back(import);
                                eleVarGroupLLine.push_back(templine);
                            }
                            else
                            {
                                eleVarGroupF.push_back(import);
                                eleVarGroupFLine.push_back(templine);
                            }
                        }
                        else
                        {
                            ErrorMessage(1039, format, filepath, linecount);
                        }
                    }
                    else
                    {
                        VecStr import;

                        if (isLast)
                        {
                            eleVarGroupL.push_back(import);
                            eleVarGroupLLine.push_back(templine);
                        }
                        else
                        {
                            eleVarGroupF.push_back(import);
                            eleVarGroupFLine.push_back(templine);
                        }
                    }
                }
                else if (lower == "compulsory")
                {
                    for (uint i = 0; i < strline.length(); ++i)
                    {
                        if (!isalpha(strline[i])) ErrorMessage(1044, format, filepath, linecount);
                    }

                    if (AnimInfo.size() > 2) ErrorMessage(1199, format, filepath, linecount);

                    if (!compulsoryDone)
                    {
                        string rule = AnimInfo[1];

                        // compulsory processing
                        __int64 reference = count(rule.begin(), rule.end(), ',');

                        size_t nextpos = 0;
                        size_t pos     = 0;

                        for (int i = 0; i < reference + 1; ++i)
                        {
                            if (i != reference)
                            {
                                nextpos = rule.find(",", nextpos + 1);
                                compulsory.push_back(rule.substr(pos, nextpos - pos));
                                pos = nextpos + 1;
                            }
                            else
                            {
                                compulsory.push_back(rule.substr(pos));
                            }
                        }

                        compulsoryDone = true;
                    }
                    else
                    {
                        ErrorMessage(1045, format, filepath, linecount);
                    }
                }
                else if (lower == "core")
                {
                    if (AnimInfo.size() < 2) ErrorMessage(1176, format, filepath, linecount);

                    core         = true;
                    coreBehavior = strline.substr(5);
                    break;
                }
                else if (lower == "add") // Add <option> <addon> <modifier>
                {
                    if (AnimInfo.size() > 4) ErrorMessage(1200, format, filepath, linecount);

                    if (AnimInfo[3].find("$$$") == NOT_FOUND)
                        ErrorMessage(1006, format, filepath, linecount, strline);

                    if (AnimInfo[3] == "$$$") ErrorMessage(1005, format, filepath, linecount, strline);

                    modAddOn[AnimInfo[1]][AnimInfo[2]] = AnimInfo[3];
                }
                else if (lower == "animobject")
                {
                    if (AnimInfo.size() > 2 || !isOnlyNumber(AnimInfo[1]))
                        ErrorMessage(1175, format, filepath, linecount, strline);

                    animObjectCount = stoi(AnimInfo[1]);
                }
                else
                {
                    string ID = nemesis::regex_replace(
                        string(lower), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                    if (AnimInfo[0] == "S" + ID)
                    {
                        if (ID == "0") ErrorMessage(1076, format, filepath, linecount);

                        if (AnimInfo.size() < 3)
                            ErrorMessage(1069, format, filepath, linecount);
                        else if (AnimInfo.size() > 3)
                            ErrorMessage(1070, format, filepath, linecount);

                        string functionID = nemesis::regex_replace(
                            string(AnimInfo[2]), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                        if (AnimInfo[2] != "#" + functionID) ErrorMessage(1071, format, filepath, linecount);

                        if (behaviorPath[nemesis::to_lower_copy(AnimInfo[1])].length() == 0)
                            ErrorMessage(1083, AnimInfo[1], format, filepath, linecount);

                        multiState[nemesis::to_lower_copy(AnimInfo[1])][stoi(ID)] = stoi(functionID);
                        isNumExist[stoi(ID)]                                      = true;
                    }
                    else
                    {
                        size_t lineplus = nemesis::regex_replace(string(strline),
                                                               nemesis::regex("[<\\s]*([^<\\s]+).*"),
                                                               string("\\1"))
                                              .length();

                        if (lineplus > 4) ErrorMessage(1011, format, filepath, linecount, AnimInfo[0]);

                        string tempOption = strline.substr(0, lineplus);
                        string recontext  = tempOption;

                        if ((nemesis::iequals(tempOption, templatecode) && strline == tempOption)
                            || nemesis::iequals(tempOption, "k") || nemesis::iequals(tempOption, "bsa"))
                        { ErrorMessage(1049, tempOption, format, filepath, linecount); }

                        if (strline.find("<", lineplus) != NOT_FOUND
                            && strline.find(">", lineplus + 1) != NOT_FOUND)
                        {
                            if (nemesis::iequals(tempOption, "animobject")
                                || nemesis::iequals(tempOption, "end"))
                            { ErrorMessage(1049, tempOption, format, filepath, linecount); }

                            if (tempOption == format) ErrorMessage(1064, format, filepath, linecount);

                            if (storelist[tempOption])
                                ErrorMessage(1177, format, filepath, linecount, tempOption);

                            storelist[tempOption] = true;
                            optionOrder.push_back(tempOption);
                            __int64 opening = count(strline.begin(), strline.end(), '<');
                            __int64 closing = count(strline.begin(), strline.end(), '>');

                            if (opening == closing)
                            {
                                addOn[tempOption].reserve(int(opening));
                                joint[tempOption].reserve(int(opening) - 1);
                                size_t nextpos = 0;

                                for (int i = 0; i < opening; ++i)
                                {
                                    nextpos = strline.find("<", nextpos) + 1;
                                    string addition
                                        = strline.substr(nextpos, strline.find(">", nextpos) - nextpos);
                                    string tempAddOn
                                        = nemesis::regex_replace(string(addition),
                                                               nemesis::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"),
                                                               string("\\1"));

                                    if (tempAddOn != addition)
                                    { ErrorMessage(1015, format, filepath, linecount); }
                                    else
                                    {
                                        if (Debug && isElementExist[addition] && addition != "variable"
                                            && addition != "event")
                                        { WarningMessage(1001, format, filepath, linecount); }

                                        isElementExist[addition] = true;
                                        addOn[tempOption].push_back(addition);
                                        container.insert(addition);
                                        isAddOn[tempOption] = true;
                                        recontext           = recontext + "<" + tempAddOn + ">";
                                    }

                                    if (i != opening - 1)
                                    {
                                        size_t pos      = strline.find(">", nextpos) + 1;
                                        string jointStr = strline.substr(pos, strline.find("<", pos) - pos);
                                        joint[tempOption].push_back(jointStr);
                                        recontext = recontext + jointStr;
                                    }

                                    if (error) throw nemesis::exception();
                                }

                                if (strline.length() > 2 && strline.substr(strline.length() - 2) == "[]")
                                {
                                    groupOption[tempOption] = true;
                                    recontext               = recontext + "[]";
                                }
                            }
                            else
                            {
                                ErrorMessage(1014, format, filepath, linecount);
                            }

                            boost::algorithm::erase_all(strline, " ");

                            if (recontext != strline) ErrorMessage(1033, format, filepath, linecount);
                        }
                        else
                        {
                            bool number = false;

                            for (uint i = 0; i < strline.length(); ++i)
                            {
                                if (!isalnum(strline[i]) && strline[i] != '[' && strline[i] != ']')
                                    ErrorMessage(1012, format, filepath, linecount);
                            }

                            if (nemesis::iequals(tempOption, "animobject") || tempOption == "D")
                                ErrorMessage(1049, tempOption, format, filepath, linecount);

                            if (tempOption == format) ErrorMessage(1064, format, filepath, strline);

                            if (storelist[tempOption])
                                ErrorMessage(1177, format, filepath, linecount, tempOption);

                            storelist[tempOption] = true;
                            optionOrder.push_back(tempOption);

                            if (strline.length() > 2 && strline.substr(strline.length() - 2) == "[]")
                                ErrorMessage(1022, format, filepath, linecount);

                            boost::algorithm::erase_all(strline, " ");

                            if (recontext != strline) ErrorMessage(1033, format, filepath, linecount);
                        }
                    }
                }
            }
        }

        for (uint i = 1; i < isNumExist.size(); ++i)
        {
            if (!isNumExist[i]) ErrorMessage(1024, isNumExist.size(), format, filepath, linecount);
        }

        // check if variable exist or not
        for (auto it = matcher.begin(); it != matcher.end(); ++it)
        {
            bool isMatched = false;

            for (auto iter = container.begin(); iter != container.end(); ++iter)
            {
                if (*it == *iter)
                {
                    isMatched = true;
                    break;
                }
            }

            if (!isMatched) ErrorMessage(1038, format, filepath, matchLine[*it], *it);
        }

        // check for validity of modAddOn
        for (auto it = modAddOn.begin(); it != modAddOn.end(); ++it)
        {
            // check if option exist
            if (isAddOn[it->first])
            {
                for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
                {
                    size_t addOnSize = addOn[it->first].size();
                    bool pass        = false;

                    for (uint i = 0; i < addOnSize; ++i)
                    {
                        if (iter->first == addOn[it->first][i])
                        {
                            pass = true;
                            break;
                        }
                    }

                    if (!pass) ErrorMessage(1007, it->first, iter->first, format, filepath, linecount);
                }
            }
            else
            {
                if (!storelist[it->first])
                {
                    WarningMessage(1025, it->first, format, filepath, linecount);
                    return;
                }
                else
                {
                    ErrorMessage(1010, format, filepath, linecount, it->first);
                }
            }

            if (error) throw nemesis::exception();
        }
    }
    else
    {
        ErrorMessage(1009, format, filepath);
    }

    if (eleEvent.size() != eleEventLine.size() || eleEventGroupF.size() != eleEventGroupFLine.size()
        || eleEventGroupL.size() != eleEventGroupLLine.size() || eleVar.size() != eleVarLine.size()
        || eleVarGroupF.size() != eleVarGroupFLine.size() || eleVarGroupL.size() != eleVarGroupLLine.size())
    { ErrorMessage(1047); }

    if (ignoreGroup)
    {
        if (eleEventGroupF.size() != 0 || eleEventGroupL.size() != 0) ErrorMessage(1080, format, filepath);

        if (eleVarGroupF.size() != 0 || eleVarGroupL.size() != 0) ErrorMessage(1080, format, filepath);
    }

    for (auto it = linked.begin(); it != linked.end(); ++it)
    {
        if (mixOptRegis[it->first].length() == 0)
        {
            string mixedOption = it->first;

            for (uint i = 0; i < it->second.size(); ++i)
            {
                mixedOption = mixedOption + "&" + it->second[i];
            }

            mixOptRegis[it->first] = mixedOption;
            mixOptRever[mixedOption].push_back(it->first);
            bool l_error = false;

            for (uint i = 0; i < it->second.size(); ++i)
            {
                if (groupOption[it->first] != groupOption[it->second[i]]
                    || addOn[it->first] != addOn[it->second[i]])
                    l_error = true;

                mixOptRegis[it->second[i]] = mixedOption;
                mixOptRever[mixedOption].push_back(it->second[i]);
            }

            if (l_error)
            {
                string errorElements;
                errorElements.append(it->first);

                for (uint i = 0; i < it->second.size(); ++i)
                {
                    errorElements.append("," + it->second[i]);
                }

                ErrorMessage(1062, format, filepath, errorElements);
            }
        }
    }

    unordered_map<string, bool> isDone;

    // linked option existence element matching
    for (auto it = linked.begin(); it != linked.end(); ++it)
    {
        if (!isDone[it->first])
        {
            for (uint i = 0; i < it->second.size(); ++i)
            {
                if (addOn[it->first].size() == addOn[it->second[i]].size())
                {
                    for (unsigned j = 0; j < addOn[it->first].size(); ++j)
                    {
                        if (addOn[it->first][j] != addOn[it->second[i]][j])
                            ErrorMessage(1053, format, filepath);
                    }

                    isDone[it->second[i]] = true;
                    isDone[it->first]     = true;
                }
                else
                {
                    ErrorMessage(1053, format, filepath);
                }
            }
        }
    }

    // throw error
    if (core)
    {
        if (ignoreGroup || groupMin != -1 || startStateID.length() != 0 || ruleOne.size() != 0
            || ruleTwo.size() != 0 || compulsory.size() != 0 || optionOrder.size() != 0
            || multiState.size() != 0 || storelist.size() != 0 || groupOption.size() != 0
            || mixOptRegis.size() != 0 || mixOptRever.size() != 0 || addOn.size() != 0 || joint.size() != 0
            || eleEvent.size() != 0 || eleEventGroupF.size() != 0 || eleEventGroupL.size() != 0
            || eleVar.size() != 0 || eleVarGroupF.size() != 0 || eleVarGroupL.size() != 0)
        { ErrorMessage(1084, format, filepath); }
    }

    // Warning message checking
    // Priority matching necessary checking
    if (Debug)
    {
        for (auto it = storelist.begin(); it != storelist.end(); ++it)
        {
            for (auto iter = storelist.begin(); iter != storelist.end(); ++iter)
            {
                if (it->first != iter->first && optionMatching(it->first, iter->first)
                    && (isAddOn[it->first] || isAddOn[iter->first]))
                { WarningMessage(1002, it->first, iter->first, filepath); }

                if (error) throw nemesis::exception();
            }

            if (error) throw nemesis::exception();
        }
    }
}

void OptionList::setDebug(bool isDebug)
{
    Debug = isDebug;
}

bool optionMatching(string option1, string option2)
{
    string shorter;
    string longer;

    if (option1.length() < option2.length())
    {
        shorter = option1;
        longer  = option2;
    }
    else
    {
        shorter = option2;
        longer  = option1;
    }

    for (uint i = 0; i < shorter.size(); ++i)
    {
        if (shorter[i] != longer[i]) { return false; }
    }

    return true;
}
