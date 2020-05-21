#include <boost/regex.hpp>

#include "utilities/conditions.h"
#include "utilities/stringsplit.h"

#include "generate/animation/newanimation.h"
#include "generate/animation/singletemplate.h"

using namespace std;

void stateInstall(string line,
                  string change,
                  string format,
                  string behaviorFile,
                  int numline,
                  size_t curPos,
                  string animOrder,
                  bool isMC,
                  map<int, vector<nemesis::scope>>& lineblocks,
                  proc& process,
                  void (proc::*func)(nemesis::scope, VecStr&));
void mainAnimEventInstall(string format,
                          string behaviorFile,
                          string change,
                          int numline,
                          size_t curPos,
                          nemesis::regex expr,
                          bool isGroup,
                          bool isMaster,
                          proc& process);
void ProcessFunction(string change,
                     string line,
                     string format,
                     string behaviorFile,
                     string multiOption,
                     bool &isEnd,
                     int numline,
                     size_t curPos,
                     OptionList &optionlist,
                     map<int, vector<shared_ptr<nemesis::scope>>> &lineblocks,
                     vector<AddOnInfo> &addInfo,
                     bool &isTrueMulti,
                     bool isGroup = false,
                     bool isMaster = false,
                     bool isMC = true,
                     proc *process = nullptr);

void AnimTemplate::ExamineTemplate(
    string n_format, string n_file, VecStr templatelines, bool isGroup, bool isMaster, OptionList optionlist)
{
    bool isCore     = optionlist.core;
    bool isEnd      = false;
    bool open       = false;
    bool multi      = false;
    bool newOpen    = false;
    bool norElement = false;
    bool negative   = false;
    int openOrder   = -2;
    int condition   = 0;
    int openRange   = 0;
    format          = n_format;
    behaviorFile    = n_file;
    string elementline;
    unordered_map<int, VecStr> conditionStore;
    unordered_map<int, string> openConditions;
    vector<nemesis::CondVar<string>*> generatedlines;
    generatedlines.push_back(&lines);

    if (isCore)
    {
        for (unsigned int i = 0; i < templatelines.size(); ++i)
        {
            bool uniqueskip = false;
            string line     = templatelines[i];
            Process(line,
                    "",
                    norElement,
                    isEnd,
                    isGroup,
                    isMaster,
                    openRange,
                    i + 1,
                    optionlist,
                    generatedlines.back());

            if (error) throw nemesis::exception();
        }
    }
    else
    {
        for (unsigned int i = 0; i < templatelines.size(); ++i)
        {
            bool uniqueskip = false;
            string line     = templatelines[i];
            nemesis::smatch match;

            if (nemesis::regex_search(
                    line, match, nemesis::regex(".*<!-- CONDITION START \\^(.+?)\\^ -->.*")))
            {
                condition++;
                string multiOption;

                for (auto it = generatedlines.rbegin(); it != generatedlines.rend(); ++it) {
                    if ((*it)->isMulti) multiOption = (*it)->conditions;
                }

                generatedlines.back()->rawlist.push_back(nemesis::LinkedVar<string>());
                generatedlines.back()->rawlist.back().nestedcond.push_back(nemesis::CondVar<string>());
                generatedlines.push_back(&generatedlines.back()->rawlist.back().nestedcond.back());
                generatedlines.back()->conditions    = match[1];
                generatedlines.back()->next          = make_shared<nemesis::Condt>(match[1],
                                                                          format,
                                                                          behaviorFile,
                                                                          match[1],
                                                                          multiOption,
                                                                          i + 1,
                                                                          isGroup,
                                                                          isMaster,
                                                                          optionlist);
                generatedlines.back()->linenum       = i + 1;
                generatedlines.back()->conditionType = nemesis::CONDITION_START;
                uniqueskip                           = true;
            }
            else if (nemesis::regex_search(line, match, nemesis::regex(".*<!-- CONDITION \\^(.+?)\\^ -->.*")))
            {
                if (condition == 0) ErrorMessage(1119, format, behaviorFile, i + 1);

                string multiOption;

                for (auto it = generatedlines.rbegin(); it != generatedlines.rend(); ++it) {
                    if ((*it)->isMulti) multiOption = (*it)->conditions;
                }

                generatedlines.pop_back();
                generatedlines.back()->rawlist.back().nestedcond.push_back(nemesis::CondVar<string>());
                generatedlines.push_back(&generatedlines.back()->rawlist.back().nestedcond.back());
                generatedlines.back()->conditions    = match[1];
                generatedlines.back()->next          = make_shared<nemesis::Condt>(match[1],
                                                                          format,
                                                                          behaviorFile,
                                                                          match[1],
                                                                          multiOption,
                                                                          i + 1,
                                                                          isGroup,
                                                                          isMaster,
                                                                          optionlist);
                generatedlines.back()->linenum       = i + 1;
                generatedlines.back()->conditionType = nemesis::CONDITION;
                uniqueskip                           = true;
            }
            else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
            {
                if (condition == 0) ErrorMessage(1119, format, behaviorFile, i + 1);

                generatedlines.pop_back();
                generatedlines.back()->rawlist.back().nestedcond.push_back(nemesis::CondVar<string>());
                generatedlines.push_back(&generatedlines.back()->rawlist.back().nestedcond.back());
                generatedlines.back()->conditionType = nemesis::CONDITION_DEFAULT;
                uniqueskip                           = true;
            }
            else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
            {
                if (newOpen) ErrorMessage(1116, format, behaviorFile, i + 1);

                string multiOption;

                for (auto it = generatedlines.rbegin(); it != generatedlines.rend(); ++it) {
                    if ((*it)->isMulti) multiOption = (*it)->conditions;
                }

                generatedlines.back()->rawlist.push_back(nemesis::LinkedVar<string>());
                generatedlines.back()->rawlist.back().nestedcond.push_back(nemesis::CondVar<string>());
                generatedlines.push_back(&generatedlines.back()->rawlist.back().nestedcond.back());
                generatedlines.back()->conditions = getOption(line);
                generatedlines.back()->next = make_shared<nemesis::Condt>(generatedlines.back()->conditions,
                                                                          format,
                                                                          behaviorFile,
                                                                          generatedlines.back()->conditions,
                                                                          multiOption,
                                                                          i + 1,
                                                                          isGroup,
                                                                          isMaster,
                                                                          optionlist);
                generatedlines.back()->linenum       = i + 1;
                generatedlines.back()->conditionType = nemesis::NEW;
                newOpen                              = true;
                uniqueskip                           = true;
            }
            else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
            {
                if (newOpen) ErrorMessage(1116, format, behaviorFile, i + 1);

                string multiOption;

                for (auto it = generatedlines.rbegin(); it != generatedlines.rend(); ++it) {
                    if ((*it)->isMulti) multiOption = (*it)->conditions;
                }

                generatedlines.back()->rawlist.push_back(nemesis::LinkedVar<string>());
                generatedlines.back()->rawlist.back().nestedcond.push_back(nemesis::CondVar<string>());
                generatedlines.push_back(&generatedlines.back()->rawlist.back().nestedcond.back());
                generatedlines.back()->conditions = getOption(line);

                if (isMaster && generatedlines.back()->conditions == format
                    && multiOption != format + "_group")
                    ErrorMessage(1202, format, behaviorFile, i + 1);

                generatedlines.back()->next = make_shared<nemesis::Condt>(generatedlines.back()->conditions,
                                                                          format,
                                                                          behaviorFile,
                                                                          generatedlines.back()->conditions,
                                                                          multiOption,
                                                                          i + 1,
                                                                          isGroup,
                                                                          isMaster,
                                                                          optionlist);
                generatedlines.back()->linenum       = i + 1;
                generatedlines.back()->isMulti       = true;
                generatedlines.back()->conditionType = nemesis::FOREACH;

                newOpen    = true;
                uniqueskip = true;
            }
            else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
            {
                if (isMaster) ErrorMessage(1202, format, behaviorFile, i + 1);

                if (newOpen) ErrorMessage(1116, format, behaviorFile, i + 1);

                string multiOption;

                for (auto it = generatedlines.rbegin(); it != generatedlines.rend(); ++it) {
                    if ((*it)->isMulti) multiOption = (*it)->conditions;
                }

                generatedlines.back()->rawlist.push_back(nemesis::LinkedVar<string>());
                generatedlines.back()->rawlist.back().nestedcond.push_back(nemesis::CondVar<string>());
                generatedlines.push_back(&generatedlines.back()->rawlist.back().nestedcond.back());
                generatedlines.back()->conditions = nemesis::regex_replace(
                    string(line), nemesis::regex(".*<!-- NEW ORDER (.+?) -->.*"), string("\\1"));
                generatedlines.back()->next = make_shared<nemesis::Condt>(generatedlines.back()->conditions,
                                                                          format,
                                                                          behaviorFile,
                                                                          generatedlines.back()->conditions,
                                                                          multiOption,
                                                                          i + 1,
                                                                          isGroup,
                                                                          isMaster,
                                                                          optionlist);
                generatedlines.back()->linenum       = i + 1;
                generatedlines.back()->isOrder       = true;
                generatedlines.back()->conditionType = nemesis::NEW_ORDER;
                newOpen                              = true;
                uniqueskip                           = true;
            }
            else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
            {
                if (!newOpen) ErrorMessage(1171, format, behaviorFile, i + 1);

                generatedlines.pop_back();
                newOpen    = false;
                uniqueskip = true;
            }
            else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
            {
                --condition;
                generatedlines.pop_back();
                uniqueskip = true;
            }

            if (!uniqueskip)
            {
                string multiOption;

                for (auto it = generatedlines.rbegin(); it != generatedlines.rend(); ++it) {
                    if ((*it)->isMulti) multiOption = (*it)->conditions;
                }

                Process(line,
                        multiOption,
                        norElement,
                        isEnd,
                        isGroup,
                        isMaster,
                        openRange,
                        i + 1,
                        optionlist,
                        generatedlines.back());
            }

            if (error) throw nemesis::exception();
        }
    }

    if (generatedlines.size() != 1) ErrorMessage(1122, format, behaviorFile);

    if (newOpen) ErrorMessage(1116, format, behaviorFile, templatelines.size() + 1);

    if (condition > 0) ErrorMessage(1145, format, behaviorFile);

    size = templatelines.size();
}

void AnimTemplate::Process(const string& line,
                           const string& multiOption,
                           bool& norElement,
                           bool& isEnd,
                           bool isGroup,
                           bool isMaster,
                           int& openRange,
                           int numline,
                           OptionList& optionlist,
                           nemesis::CondVar<string>* generatedlines)
{
    bool hasProcess = false;

    // compute numelements
    if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND
        && line.find("</hkparam>") == NOT_FOUND
        && line.find("<!-- COMPUTE -->", line.find("numelements=\"")) != NOT_FOUND)
    {
        if (!norElement)
        {
            norElement      = true;
            hasProcess      = true;
            string templine = line.substr(0, line.find("<hkparam name=\"", 0));
            openRange       = count(templine.begin(), templine.end(), '\t');
            process.installBlock(nemesis::scope(0, 0, vector<int>{openRange}, &proc::compute),
                                 numline);
        }
        else
        {
            ErrorMessage(1136, format, behaviorFile, numline);
        }
    }
    else if (norElement && line.find("</hkparam>") != NOT_FOUND)
    {
        string templine = line.substr(0, line.find("</hkparam>"));

        if (openRange == count(templine.begin(), templine.end(), '\t'))
        {
            hasProcess = true;
            norElement = false;
            process.installBlock(nemesis::scope(0, 0, &proc::rangeCompute), numline);
        }
    }

    if (norElement)
    {
        string templine = line;

        if (templine.find("<hkobject>") != NOT_FOUND)
        {
            templine = templine.substr(0, templine.find("<hkobject>"));

            if (openRange + 1 == count(templine.begin(), templine.end(), '\t'))
            {
                hasProcess = true;
                process.installBlock(nemesis::scope(0, 99999, &proc::upCounter), numline);
            }
        }
        else if (templine.find("\t\t\t#") != NOT_FOUND)
        {
            templine = templine.substr(0, templine.find("#", 0));

            if (openRange + 1 == count(templine.begin(), templine.end(), '\t'))
            {
                hasProcess = true;
                process.installBlock(nemesis::scope(0, 99999, &proc::upCounterPlus), numline);
            }
        }
    }

    if (line.find("$") != NOT_FOUND)
    {
        nemesis::regex exp("(?<!MID)(?<!\\$MC)(?<!" + format + "_master)(?<!" + format
                           + "_group)(?<!\\$%)\\$(?!%\\$)(?!MC\\$)(?!elements\\$)(.+?)(?<!MID)(?<!\\$MC)(?<!"
                           + format + "_master)(?<!" + format
                           + "_group)(?<!\\$%)\\$(?!%\\$)(?!MC\\$)(?!elements\\$)");

        for (nemesis::regex_iterator itr(line, exp); itr != nemesis::regex_iterator(); ++itr)
        {
            bool isChange = false;
            string change = itr->str(1);
            size_t curPos = itr->position();
            process.brackets[numline].push_back(itr->position());
            process.brackets[numline].push_back(itr->position() + itr->str().length() - 1);
            map<int, vector<shared_ptr<nemesis::scope>>> dummy1;
            vector<AddOnInfo> dummy2;
            bool dummy3;
            ProcessFunction(change,
                            line,
                            format,
                            behaviorFile,
                            multiOption,
                            isEnd,
                            numline,
                            curPos + 1,
                            optionlist,
                            dummy1,
                            dummy2,
                            dummy3,
                            isGroup,
                            isMaster,
                            false,
                            &process);
            hasProcess = true;
        }

        size_t pos = line.find("$%$");

        if (pos != NOT_FOUND)
        {
            while (pos != NOT_FOUND)
            {
                process.installBlock(nemesis::scope(pos, pos + 3, &proc::animCount), numline);
                pos = line.find("$%$", pos + 1);
            }

            hasProcess = true;
        }

        pos = line.find("$MC$");

        // multi choice selection
        if (pos != NOT_FOUND)
        {
            vector<nemesis::MultiChoice> m_conditions;
            process.hasMC[numline] = true;

            for (auto itr = nemesis::regex_iterator(line,
                                                    nemesis::regex("[\\s]+<!-- (.+?) -->[\\s]*?"));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                string output = itr->str(1);
                pos           = itr->position(1);
                VecStr curElements;
                StringSplit(output, curElements);

                if (curElements.size() == 1)
                {
                    m_conditions.push_back(nemesis::MultiChoice("",
                                                                format,
                                                                behaviorFile,
                                                                multiOption,
                                                                numline,
                                                                isGroup,
                                                                isMaster,
                                                                optionlist,
                                                                pos,
                                                                pos + output.length()));
                }
                else if (curElements.size() > 1)
                {
                    pos = pos + output.length();
                    m_conditions.push_back(nemesis::MultiChoice(curElements[0],
                                                                format,
                                                                behaviorFile,
                                                                multiOption,
                                                                numline,
                                                                isGroup,
                                                                isMaster,
                                                                optionlist,
                                                                pos - curElements.back().length(),
                                                                pos));
                }
            }

            for (auto itr = nemesis::regex_iterator(line, nemesis::regex("\\$MC\\$"));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                pos = itr->position();
                process.installBlock(nemesis::scope(pos,
                                                    pos + itr->str().length(),
                                                    &proc::multiChoiceRegis),
                                     numline,
                                     m_conditions);
                hasProcess = true;
            }
        }

        // get group node ID
        if (isGroup)
        {
            for (auto itr = nemesis::regex_iterator(line, nemesis::regex(format + "\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                string ID = itr->str(1);
                pos       = itr->position();
                process.installBlock(nemesis::scope(pos,
                                                    pos + itr->str().length(),
                                                    VecStr{ID, line},
                                                    &proc::IDRegisAnim),
                                     numline);
                hasProcess = true;
            }

            for (auto itr = nemesis::regex_iterator(line,
                                                    nemesis::regex(format + "_group\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                string ID = itr->str(1);
                pos       = itr->position();
                process.installBlock(nemesis::scope(pos,
                                                    pos + itr->str().length(),
                                                    VecStr{ID, line},
                                                    &proc::IDRegisGroup),
                                     numline);
                hasProcess = true;
            }
        }
        else
        {
            for (auto itr = nemesis::regex_iterator(line,
                                                    nemesis::regex(format + "_group\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                string ID = itr->str(1);
                pos       = itr->position();
                process.installBlock(nemesis::scope(pos,
                                                    pos + itr->str().length(),
                                                    VecStr{ID},
                                                    &proc::groupIDRegis),
                                     numline);
                hasProcess = true;
            }
        }

        pos = line.find("MID$");

        // set function ID
        if (pos != NOT_FOUND)
        {
            void (proc::*func)(nemesis::scope, VecStr&);

            if (isMaster)
                func = &proc::IDRegisMaster;
            else if (isGroup)
                func = &proc::IDRegisGroup;
            else
                func = &proc::IDRegis;

            for (auto itr = nemesis::regex_iterator(line, nemesis::regex("MID\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                pos = itr->position();
                process.installBlock(nemesis::scope(pos,
                                                    pos + itr->str().length(),
                                                    VecStr{itr->str(1), line},
                                                    func),
                                     numline);
                hasProcess = true;
            }
        }
    }

    if (isEnd)
    {
        for (auto itr = nemesis::regex_iterator(
                 line, nemesis::regex("<hkparam name\\=\"relativeToEndOfClip\">(.+?)<\\/hkparam>"));
             itr != nemesis::regex_iterator();
             ++itr) {
            isEnd      = false;
            hasProcess = true;
            size_t pos = itr->position(1);
            process.installBlock(nemesis::scope(pos,
                                                pos + itr->str(1).length(),
                                                &proc::relativeNegative),
                                 numline);
        }

        for (auto itr = nemesis::regex_iterator(
                 line, nemesis::regex("<hkparam name\\=\"localTime\">(.+?)<\\/hkparam>"));
             itr != nemesis::regex_iterator();
             ++itr) {
            hasProcess = true;
            size_t pos = itr->position(1);
            process.installBlock(nemesis::scope(pos,
                                                pos + itr->str(1).length(),
                                                &proc::localNegative),
                                 numline);
        }
    }

    for (auto itr = nemesis::regex_iterator(
             line, nemesis::regex("<hkparam name\\=\"animationName\">(.+?)<\\/hkparam>"));
         itr != nemesis::regex_iterator();
         ++itr) {
        hasProcess = true;
        size_t pos = itr->position(1);
        process.installBlock(nemesis::scope(pos, pos + itr->str(1).length(), &proc::regisAnim),
                             numline);
    }

    for (auto itr = nemesis::regex_iterator(
             line, nemesis::regex("<hkparam name\\=\"behaviorName\">(.+?)<\\/hkparam>"));
         itr != nemesis::regex_iterator();
         ++itr) {
        hasProcess = true;
        size_t pos = itr->position(1);
        process.installBlock(nemesis::scope(pos, pos + itr->str(1).length(), &proc::regisBehavior),
                             numline);
    }

    auto ls       = nemesis::LinkedVar(line, hasProcess, numline);
    ls.raw        = make_shared<string>(line);
    ls.hasProcess = hasProcess;
    ls.linecount  = numline;
    generatedlines->rawlist.push_back(ls);
}

AddOnInfo::AddOnInfo(string n_h, string n_a)
{
    header   = n_h;
    addition = n_a;
}

AddOnInfo::AddOnInfo(string n_h, string n_a, int n_om)
{
    header      = n_h;
    addition    = n_a;
    optionMulti = n_om;
}

string getOption(string curline)
{
    return nemesis::regex_replace(
        string(curline), nemesis::regex(".*<!-- (?:FOREACH|NEW) \\^(.+?)\\^ -->.*"), string("\\1"));
}

void stateInstall(string line,
                  string change,
                  string format,
                  string behaviorFile,
                  int numline,
                  size_t curPos,
                  string animOrder,
                  bool isMC,
                  map<int, vector<shared_ptr<nemesis::scope>>>& lineblocks,
                  proc& process,
                  void (proc::*func)(nemesis::scope, VecStr&))
{
    int intID;
    nemesis::regex expr(format + "\\[" + animOrder + "\\]\\[\\(S([0-9]*)\\+([0-9]+)\\)\\]");

    for (auto itr = nemesis::regex_iterator(change, expr); itr != nemesis::regex_iterator(); ++itr) {
        string ID     = itr->str(1);
        string number = itr->str(2);
        size_t post   = curPos + itr->position();

        if (!isOnlyNumber(number))
        {
            ErrorMessage(1152, format, behaviorFile, numline, itr->str());
        }

        if (ID.length() == 0)
        {
            intID = 0;
        }
        else
        {
            intID = stoi(ID) - 1;
        }

        if (format + "[" + animOrder + "][(S" + ID + "+" + number + ")]"
            == line.substr(post, itr->str().length()))
        {
            if (animOrder.length() > 0 && isOnlyNumber(animOrder))
            {
                string full = itr->str();
                nemesis::scope blok(post,
                                    post + full.length(),
                                    vector<int>{intID, stoi(number), stoi(animOrder)},
                                    VecStr{full},
                                    func);

                if (isMC)
                {
                    lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                }
                else
                {
                    process.installBlock(blok, numline);
                }
            }
            else
            {
                nemesis::scope blok(post, post + itr->str().length(), vector<int>{intID, stoi(number)}, func);

                if (isMC)
                {
                    lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                }
                else
                {
                    process.installBlock(blok, numline);
                }
            }
        }
        else
        {
            ErrorMessage(1137, format, behaviorFile, numline, itr->str());
        }

        if (error) throw nemesis::exception();
    }
}

void mainAnimEventInstall(string format,
                          string behaviorFile,
                          string change,
                          int numline,
                          size_t curPos,
                          nemesis::regex expr,
                          bool isGroup,
                          bool isMaster,
                          proc& process)
{
    for (auto itr = nemesis::regex_iterator(change, expr); itr != nemesis::regex_iterator(); ++itr) {
        bool num     = false;
        string first = itr->str(1);
        size_t post  = curPos + itr->position();
        void (proc::*func)(nemesis::scope, VecStr&);

        if (first.length() == 0)
        {
            func = isGroup ? &proc::MAEMultiMaster : &proc::MAEMultiGroup;
        }
        else if (first == "F")
        {
            func = isGroup ? &proc::MAEFirstMaster : &proc::MAEFirstGroup;
        }
        else if (first == "N")
        {
            if (isMaster)
            {
                ErrorMessage(1056, format + "_master", behaviorFile, numline, change);
            }
            else if (isGroup)
            {
                ErrorMessage(1056, format + "_group", behaviorFile, numline, change);
            }

            func = &proc::MAENextGroup;
        }
        else if (first == "B")
        {
            if (isMaster)
            {
                ErrorMessage(1056, format + "_master", behaviorFile, numline, change);
            }
            else if (isGroup)
            {
                ErrorMessage(1056, format + "_group", behaviorFile, numline, change);
            }
                
            func = &proc::MAEBackGroup;
        }
        else if (first == "L")
        {
            func = isGroup ? &proc::MAELastMaster : &proc::MAELastGroup;
        }
        else
        {
            num = true;
        }

        if (num)
        {
            isGroup ? process.installBlock(nemesis::scope(post,
                                                          post + itr->str().length(),
                                                          vector<int>{stoi(first)},
                                                          VecStr{itr->str()},
                                                          &proc::MAENumMaster),
                                           numline)
                    : process.installBlock(nemesis::scope(post,
                                                          post + itr->str().length(),
                                                          vector<int>{stoi(first)},
                                                          VecStr{itr->str()},
                                                          &proc::MAENumGroup),
                                           numline);
        }
        else
        {
            isGroup ? process.installBlock(nemesis::scope(post,
                                                          post + itr->str().length(),
                                                          VecStr{itr->str()},
                                                          func),
                                           numline)
                    : process.installBlock(nemesis::scope(post, post + itr->str().length(), func),
                                           numline);
        }
    }
}

void ProcessFunction(string change,
                     string line,
                     string format,
                     string behaviorFile,
                     string multiOption,
                     bool &isEnd,
                     int numline,
                     size_t curPos,
                     OptionList &optionlist,
                     map<int, vector<shared_ptr<nemesis::scope>>> &lineblocks,
                     vector<AddOnInfo> &addInfo,
                     bool &isTrueMulti,
                     bool isGroup,
                     bool isMaster,
                     bool isMC,
                     proc *process)
{
    proc tempProc;
    if (process == nullptr)
        process = &tempProc; //To avoid checking for null everywhere

    if (isMaster && multiOption != format + "_group") {
        if (change.find(format + "[") != NOT_FOUND) ErrorMessage(1204, format, behaviorFile, numline, change);

        for (auto itr = nemesis::regex_iterator(change,
                                                nemesis::regex(format + "_group\\[(.*?)\\]"));
             itr != nemesis::regex_iterator();
             ++itr) {
            ErrorMessage(1201, format, behaviorFile, numline);
        }
    } else if (!isGroup && change.find(format + "_group[") != NOT_FOUND)
        ErrorMessage(1204, format, behaviorFile, numline, change);

    string shortcut = isMaster ? format + "_group\\[\\]" : format;

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

                    if (change.find("(S" + ID + "+") == NOT_FOUND)
                    {
                        ID = "";
                    }

                    number = nemesis::regex_replace(string(equation.substr(3 + ID.length())),
                                                    nemesis::regex("[^0-9]*([0-9]+).*"),
                                                    string("\\1"));
                }

                if (equation != "(S" + ID + "+" + number + ")" && isOnlyNumber(number))
                {
                    nemesis::scope blok(
                        curPos + nextpos, curPos + nextpos + equation.length(), &proc::computation);

                    if (isMC)
                    {
                        lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                    }
                    else
                    {
                        process->installBlock(blok, numline);
                    }
                }

                if (equation.find("N") != NOT_FOUND || equation.find("B") != NOT_FOUND)
                {
                }

                if (error) throw nemesis::exception();

                nextpos = change.find("(", nextpos + 1);
            }
        }
    }

    if (change.find("END", 0) != NOT_FOUND)
    {
        nemesis::regex expr(shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[END\\]");

        for (auto itr = nemesis::regex_iterator(change, expr); itr != nemesis::regex_iterator();
             ++itr) {
            bool number  = false;
            string first = itr->str(1);
            void (proc::*func)(nemesis::scope, VecStr&);

            if (first.length() == 0)
            {
                isTrueMulti = true;
                func        = isGroup ? &proc::endMultiMaster : &proc::endMultiGroup;
            }
            else if (first == "F")
            {
                func = isGroup ? &proc::endFirstMaster : &proc::endFirstGroup;
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::endNextGroup;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::endBackGroup;
            }
            else if (first == "L")
            {
                func = isGroup ? &proc::endLastMaster : &proc::endLastGroup;
            }
            else
            {
                number = true;
                func   = isGroup ? &proc::endNumMaster : &proc::endNumGroup;
            }

            shared_ptr<nemesis::scope> blok;

            if (number)
            {
                blok = make_shared<nemesis::scope>(curPos + itr->position(),
                                                   curPos + itr->position() + itr->str().length(),
                                                   vector<int>{stoi(first)},
                                                   VecStr{change},
                                                   func);
            }
            else
            {
                blok = make_shared<nemesis::scope>(curPos + itr->position(),
                                                   curPos + itr->position() + itr->str().length(),
                                                   VecStr{change},
                                                   func);
            }

            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        expr = nemesis::regex("(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!"
                              + shortcut + "\\[\\d\\d\\]\\[)(END)");

        for (auto itr = nemesis::regex_iterator(change, expr); itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            nemesis::scope blok(curPos + itr->position(),
                                curPos + itr->position() + itr->str().length(),
                                isGroup ? &proc::endMultiMaster : &proc::endSingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }

        if (error) throw nemesis::exception();

        isEnd = true;
    }

    // set state ID
    if (change.find("(S", 0) != NOT_FOUND)
    {
        string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[\\(S([0-9]*)\\+([0-9]+)\\)\\]";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            string first = itr->str(1);
            void (proc::*func)(nemesis::scope, VecStr&);

            if (first.length() == 0)
            {
                isTrueMulti = true;
                func        = isGroup ? &proc::stateMultiMaster : &proc::stateMultiGroup;
            }
            else if (first == "F")
            {
                func = isGroup ? &proc::stateFirstMaster : &proc::stateFirstGroup;
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::stateNextGroup;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::stateBackGroup;
            }
            else if (first == "L")
            {
                func = isGroup ? &proc::stateLastMaster : &proc::stateLastGroup;
            }
            else
            {
                func = isGroup ? &proc::stateNumMaster : &proc::stateNumGroup;
            }

            int intID;
            string ID     = itr->str(2);
            string number = itr->str(3);
            size_t post   = curPos + itr->position();
            string full   = itr->str();

            if (!isOnlyNumber(number))
            {
                ErrorMessage(1152, format, behaviorFile, numline, full);
            }

            ID.length() == 0 ? intID = 0 : intID = stoi(ID) - 1;
            string compare                       = line.substr(post, full.length());

            if (format + "[" + first + "][(S" + ID + "+" + number + ")]" == compare
                || format + "_group[][" + first + "][(S" + ID + "+" + number + ")]" == compare)
            {
                shared_ptr<nemesis::scope> blok
                    = first.length() > 0 && isOnlyNumber(first)
                          ? make_shared<nemesis::scope>(post,
                                                        post + full.length(),
                                                        vector<int>{intID, stoi(number), stoi(first)},
                                                        VecStr{full},
                                                        func)
                          : make_shared<nemesis::scope>(post,
                                                        post + full.length(),
                                                        vector<int>{intID, stoi(number)},
                                                        VecStr{full},
                                                        func);
                isMC ? lineblocks[blok->size].push_back(blok)
                     : process->installBlock(*blok, numline);
            }
            else
            {
                ErrorMessage(1137, format, behaviorFile, numline, full);
            }

            if (error) throw nemesis::exception();
        }

        if (isMaster)
        {
            expstr = shortcut + "\\[\\(S([0-9]*)\\+([0-9]+)\\)\\]";

            for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
                 itr != nemesis::regex_iterator();
                 ++itr) {
                int intID;
                string ID                = itr->str(1);
                string number            = itr->str(2);
                size_t post              = curPos + itr->position();
                string full              = itr->str();
                ID.length() == 0 ? intID = 0 : intID = stoi(ID) - 1;
                shared_ptr<nemesis::scope> blok
                    = make_shared<nemesis::scope>(post,
                                                  post + full.length(),
                                                  vector<int>{intID, stoi(number)},
                                                  VecStr{full},
                                                  &proc::stateMultiMasterToGroup);
                isMC ? lineblocks[blok->size].push_back(blok)
                     : process->installBlock(*blok, numline);

                if (error) throw nemesis::exception();
            }
        }

        expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut
                 + "\\[\\d\\d\\]\\[)(?<!" + shortcut + "\\[)\\(S([0-9]*)\\+([0-9]+)\\)";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            int intID;
            size_t post                 = curPos + itr->position();
            string first                = itr->str(1);
            first.length() == 0 ? intID = 0 : intID = stoi(first) - 1;
            nemesis::scope blok(
                post, post + itr->str().length(), vector<int>{intID, stoi(itr->str(2))}, &proc::stateSingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }
    }

    if (change.find("FilePath", 0) != NOT_FOUND)
    {
        string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[FilePath\\]";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            bool number  = false;
            size_t post  = curPos + itr->position();
            string first = itr->str(1);
            void (proc::*func)(nemesis::scope, VecStr&);

            if (first.length() == 0)
            {
                isTrueMulti = true;
                func        = isGroup ? &proc::filepathMultiMaster : &proc::filepathMultiGroup;
            }
            else if (first == "F")
            {
                func = isGroup ? &proc::filepathFirstMaster : &proc::filepathFirstGroup;
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::filepathNextGroup;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::filepathBackGroup;
            }
            else if (first == "L")
            {
                func = isGroup ? &proc::filepathLastMaster : &proc::filepathLastGroup;
            }
            else
            {
                number = true;
                func   = isGroup ? &proc::filepathNumMaster : &proc::filepathNumGroup;
            }

            shared_ptr<nemesis::scope> blok;
            number ? blok = make_shared<nemesis::scope>(
                         post, post + itr->str().length(), vector<int>{stoi(first)}, VecStr{change}, func)
                   : blok = make_shared<nemesis::scope>(post, post + itr->str().length(), func);
            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut
                 + "\\[\\d\\d\\]\\[)(FilePath)";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post = curPos + itr->position();
            nemesis::scope blok(post, post + 8, isGroup ? &proc::filepathMultiMaster : &proc::filepathSingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }
    }

    if (change.find("FileName", 0) != NOT_FOUND)
    {
        string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[FileName\\]";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            bool number  = false;
            size_t post  = curPos + itr->position();
            string first = itr->str(1);
            void (proc::*func)(nemesis::scope, VecStr&);

            if (first.length() == 0)
            {
                isTrueMulti = true;
                func        = isGroup ? &proc::filenameMultiMaster : &proc::filenameMultiGroup;
            }
            else if (first == "F")
            {
                func = isGroup ? &proc::filenameFirstMaster : &proc::filenameFirstGroup;
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }

                func = &proc::filenameNextGroup;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }

                func = &proc::filenameBackGroup;
            }
            else if (first == "L")
            {
                func = isGroup ? &proc::filenameLastMaster : &proc::filenameLastGroup;
            }
            else
            {
                number = true;
                func   = isGroup ? &proc::filenameNumMaster : &proc::filenameNumGroup;
            }

            shared_ptr<nemesis::scope> blok;
            number ? blok = make_shared<nemesis::scope>(
                         post, post + itr->str().length(), vector<int>{stoi(first)}, VecStr{change}, func)
                   : blok = make_shared<nemesis::scope>(post, post + itr->str().length(), func);
            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut
                 + "\\[\\d\\d\\]\\[)(FileName)";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post = curPos + itr->position();
            nemesis::scope blok(post, post + 8, isGroup ? &proc::filenameMultiMaster : &proc::filenameSingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }

        if (error) throw nemesis::exception();
    }

    if (change.find("Path", 0) != NOT_FOUND)
    {
        string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[Path\\]";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            size_t post = curPos + itr->position();
            nemesis::scope blok(post, post + itr->str().length(), &proc::pathSingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }

        expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut
                 + "\\[\\d\\d\\]\\[)(Path)";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            size_t post = curPos + itr->position();
            nemesis::scope blok(post, post + 4, &proc::pathSingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }

        if (error) throw nemesis::exception();
    }

    if (change.find("@AnimObject/", 0) != NOT_FOUND)
    {
        int counter   = 0;
        string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[@AnimObject\\/([0-9]+)\\](\\[[0-9]+\\]|)";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            string first       = itr->str(1);
            string second      = itr->str(2);
            string optionMulti = itr->str(3);
            string full        = itr->str();
            size_t post        = curPos + itr->position();
            void (proc::*func)(nemesis::scope, VecStr&);
            vector<int> container;
            ++counter;
            string templine;
            bool empty = true;

            if (full.length() == 0)
            {
                string errorformat = format;

                if (isMaster)
                {
                    errorformat.append("_master");
                }
                else if (isGroup)
                {
                    errorformat.append("_group");
                }

                ErrorMessage(1157, errorformat, behaviorFile, numline, change);
            }

            if (first.length() == 0)
            {
                isTrueMulti = true;

                if (isGroup)
                {
                    func = empty ? &proc::AOMultiMasterA : &proc::AOMultiMasterB;
                }
                else
                {
                    func = empty ? &proc::AOMultiGroupA : &proc::AOMultiGroupB;
                }
            }
            else if (first == "F")
            {
                if (isGroup)
                {
                    func = empty ? &proc::AOFirstMasterA : &proc::AOFirstMasterB;
                }
                else
                {
                    func = empty ? &proc::AOFirstGroupA : &proc::AOFirstGroupB;
                }
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }

                func = empty ? &proc::AONextGroupA : &proc::AONextGroupB;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }

                func = empty ? &proc::AOBackGroupA : &proc::AOBackGroupB;
            }
            else if (first == "L")
            {
                if (isGroup)
                {
                    func = empty ? &proc::AOLastMasterA : &proc::AOLastMasterB;
                }
                else
                {
                    func = empty ? &proc::AOLastGroupA : &proc::AOLastGroupB;
                }
            }
            else
            {
                container.push_back(stoi(first));

                if (isGroup)
                {
                    func = empty ? &proc::AONumMasterA : &proc::AONumMasterB;
                }
                else
                {
                    func = empty ? &proc::AONumGroupA : &proc::AONumGroupB;
                }
            }

            container.push_back(stoi(second));

            if (optionMulti.length() != 0)
            {
                empty = false;
                optionMulti.pop_back();
                optionMulti = optionMulti.substr(1);
                container.push_back(stoi(optionMulti));
            }

            nemesis::scope blok(post, post + full.length(), container, VecStr{change}, func);

            if (isMC)
            {
                lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                empty ? addInfo.push_back(AddOnInfo("@AnimObject/" + second, ""))
                      : addInfo.push_back(AddOnInfo("@AnimObject/" + second, "", stoi(optionMulti)));
            }
            else
            {
                process->installBlock(blok, numline);
            }

            if (error) throw nemesis::exception();
        }

        expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut
                 + "\\[\\d\\d\\]\\[)@AnimObject\\/([0-9]+)(\\[[0-9]+\\]|)";

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex(expstr));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post     = curPos + itr->position();
            string templine = itr->str(2);

            if (templine.length() == 0)
            {
                nemesis::scope blok(post,
                                    post + itr->str().length(),
                                    vector<int>{stoi(itr->str(1))},
                                    isGroup ? &proc::AOMultiMasterA : &proc::AOSingleA);

                if (isMC)
                {
                    lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                    addInfo.push_back(AddOnInfo("@AnimObject/" + itr->str(1), ""));
                }
                else
                {
                    process->installBlock(blok, numline);
                }
            }
            else
            {
                templine.pop_back();
                templine = templine.substr(1);
                nemesis::scope blok(post,
                                    post + itr->str().length(),
                                    vector<int>{stoi(itr->str(1)), stoi(templine)},
                                    isGroup ? &proc::AOMultiMasterB : &proc::AOSingleB);

                if (isMC)
                {
                    lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                    addInfo.push_back(AddOnInfo("@AnimObject/" + itr->str(1), "", stoi(templine)));
                }
                else
                {
                    process->installBlock(blok, numline);
                }
            }
        }

        if (error) throw nemesis::exception();
    }

    if (change.find("main_anim_event", 0) != NOT_FOUND)
    {
        mainAnimEventInstall(format,
                             behaviorFile,
                             change,
                             numline,
                             curPos,
                             nemesis::regex("\\{" + shortcut
                                            + "\\[(F|N|B|L|[0-9]*)\\]\\[main_anim_event\\]\\}"),
                             isGroup,
                             isMaster,
                             *process);
        mainAnimEventInstall(format,
                             behaviorFile,
                             change,
                             numline,
                             curPos,
                             nemesis::regex(
                                 "(?<!\\{)" + shortcut
                                 + "\\[(F|N|B|L|[0-9]*)\\]\\[main_anim_event\\](?=[^\\}]|$)"),
                             isGroup,
                             isMaster,
                             *process);

        for (auto itr = nemesis::regex_iterator(change, nemesis::regex("(\\{main_anim_event\\})"));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post = curPos + itr->position();
            nemesis::scope blok(
                post, post + itr->str().length(), isGroup ? &proc::MAEMultiMaster : &proc::MAESingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }

        for (auto itr = nemesis::regex_iterator(
                 change,
                 nemesis::regex("(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut
                                + "\\[\\]\\[)(?<!" + shortcut
                                + "\\[\\d\\d\\]\\[)(?<!\\{)(main_anim_event)(?=[^\\}]|$)"));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post = curPos + itr->position();
            nemesis::scope blok(
                post, post + itr->str().length(), isGroup ? &proc::MAEMultiMaster : &proc::MAESingle);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }
    }

    size_t pos = change.find("[");

    if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
    {
        for (auto it = optionlist.addOn.begin(); it != optionlist.addOn.end(); ++it)
        {
            for (auto& addname : it->second)
            {
                // include other anim group
                // cont here

                for (auto itr = nemesis::regex_iterator(change,
                                                        nemesis::regex(
                                                            format + "\\[(F|N|B|L|[0-9]*)\\]\\["
                                                            + it->first + "(\\*|)\\]\\[" + addname
                                                            + "\\](\\[[0-9]+\\]|)"));
                     itr != nemesis::regex_iterator();
                     ++itr) {
                    bool number = false;
                    string header;
                    string first  = itr->str(1);
                    size_t addpos = curPos + itr->position();
                    void (proc::*func)(nemesis::scope, VecStr&);

                    if (first.length() == 0)
                    {
                        isTrueMulti = true;
                        func        = isGroup ? &proc::addOnMultiMaster : &proc::addOnMultiGroup;
                    }
                    else if (first == "F")
                    {
                        func = isGroup ? &proc::addOnFirstMaster : &proc::addOnFirstGroup;
                    }
                    else if (first == "N")
                    {
                        if (isMaster)
                        {
                            ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                        }
                        else if (isGroup)
                        {
                            ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                        }
                
                        func = &proc::addOnNextGroup;
                    }
                    else if (first == "B")
                    {
                        if (isMaster)
                        {
                            ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                        }
                        else if (isGroup)
                        {
                            ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                        }
                
                        func = &proc::addOnBackGroup;
                    }
                    else if (first == "L")
                    {
                        func = isGroup ? &proc::addOnLastMaster : &proc::addOnLastGroup;
                    }
                    else
                    {
                        number = true;
                        func   = isGroup ? &proc::addOnNumMaster : &proc::addOnNumGroup;
                    }

                    if (itr->str(2).length() == 0)
                    {
                        header = it->first;
                    }
                    else
                    {
                        header = optionlist.mixOptRegis[it->first];
                    }

                    if (number)
                    {
                        nemesis::scope blok(addpos,
                                            addpos + itr->str().length(),
                                            vector<int>{stoi(first)},
                                            VecStr{header, addname, change},
                                            func);

                        if (isMC)
                        {
                            lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                            string o_multi = itr->str(3);

                            if (o_multi.length() > 0)
                            {
                                addInfo.push_back(AddOnInfo(header, addname, stoi(o_multi)));
                            }
                            else
                            {
                                addInfo.push_back(AddOnInfo(header, addname));
                            }
                        }
                        else
                        {
                            process->installBlock(blok, numline);
                        }
                    }
                    else
                    {
                        nemesis::scope blok(
                            addpos, addpos + itr->str().length(), VecStr{header, addname, change}, func);

                        if (isMC)
                        {
                            lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                            string o_multi = itr->str(3);

                            if (o_multi.length() > 0)
                            {
                                addInfo.push_back(AddOnInfo(header, addname, stoi(o_multi)));
                            }
                            else
                            {
                                addInfo.push_back(AddOnInfo(header, addname));
                            }
                        }
                        else
                        {
                            process->installBlock(blok, numline);
                        }
                    }
                }

                for (auto itr = nemesis::regex_iterator(change,
                                                        nemesis::regex(it->first + "(\\*|)\\["
                                                                       + addname
                                                                       + "\\](\\[[0-9]+\\]|)"));
                     itr != nemesis::regex_iterator();
                     ++itr) {
                    if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

                    string header;
                    string first  = itr->str(1);
                    size_t addpos = curPos + itr->position();

                    if (first.length() == 0)
                    {
                        header = it->first;
                    }
                    else
                    {
                        header = optionlist.mixOptRegis[it->first];
                    }

                    nemesis::scope blok(addpos,
                                        addpos + itr->str().length(),
                                        VecStr{header, addname, change},
                                        isGroup ? &proc::addOnMultiMaster : &proc::addOnSingle);

                    if (isMC)
                    {
                        lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok));
                        string o_multi = itr->str(2);

                        if (o_multi.length() > 0)
                        {
                            addInfo.push_back(AddOnInfo(header, addname, stoi(o_multi)));
                        }
                        else
                        {
                            addInfo.push_back(AddOnInfo(header, addname));
                        }
                    }
                    else
                    {
                        process->installBlock(blok, numline);
                    }
                }
            }
        }
    }

    if (change.find("LastState") != NOT_FOUND)
    {
        for (auto itr = nemesis::regex_iterator(change, nemesis::regex("LastState([0-9]*)"));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isMaster) ErrorMessage(1206, format + "_master", behaviorFile, numline, itr->str());
            if (isGroup) ErrorMessage(1206, format + "_group", behaviorFile, numline, itr->str());

            size_t post = curPos + itr->position();
            nemesis::scope blok(post, post + itr->str().length(), VecStr{itr->str(1)}, &proc::lastState);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
        }
    }

    if (change.find("eventID[") != NOT_FOUND)
    {
        pos = change.find("eventID[");

        while (pos != NOT_FOUND)
        {
            size_t eventpos   = pos + 8;
            size_t curcounter = 1;
            size_t post       = curPos + pos;

            for (unsigned int ch = eventpos; ch < change.length(); ++ch)
            {
                if (change[ch] == '[')
                    ++curcounter;
                else if (change[ch] == ']')
                {
                    --curcounter;

                    if (curcounter == 0)
                    {
                        ++eventpos;
                        eventpos += curPos;
                        break;
                    }
                }

                ++eventpos;
            }

            nemesis::scope blok(
                post, eventpos, vector<int>{int(post + 8), int(eventpos - 2)}, &proc::eventID);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
            pos = change.find("eventID[", pos + 1);
        }
    }

    if (change.find("variableID[") != NOT_FOUND)
    {
        pos = change.find("variableID[");

        while (pos != NOT_FOUND)
        {
            size_t varpos     = pos + 11;
            size_t curcounter = 1;
            size_t post       = curPos + pos;

            for (unsigned int ch = varpos; ch < change.length(); ++ch)
            {
                if (change[ch] == '[')
                    ++curcounter;
                else if (change[ch] == ']')
                {
                    --curcounter;

                    if (curcounter == 0)
                    {
                        ++varpos;
                        varpos += curPos;
                        break;
                    }
                }

                ++varpos;
            }

            nemesis::scope blok(
                post, varpos, vector<int>{int(post + 11), int(varpos - 2)}, &proc::variableID);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
            pos = change.find("variableID[", pos + 1);
        }
    }

    if (change.find("animOrder[") != NOT_FOUND)
    {
        pos = change.find("animOrder[");

        while (pos != NOT_FOUND)
        {
            size_t animpos    = pos + 10;
            size_t curcounter = 1;
            size_t post       = curPos + pos;

            for (unsigned int ch = animpos; ch < change.length(); ++ch)
            {
                if (change[ch] == '[')
                    ++curcounter;
                else if (change[ch] == ']')
                {
                    --curcounter;

                    if (curcounter == 0)
                    {
                        ++animpos;
                        animpos += curPos;
                        break;
                    }
                }

                ++animpos;
            }

            nemesis::scope blok(post, animpos, {int(post + 10), int(animpos - 2)}, &proc::animOrder);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
            pos = change.find("animOrder[", pos + 1);
        }
    }

    if (change.find("crc32[") != NOT_FOUND)
    {
        __int64 bracketCount    = count(change.begin(), change.end(), '[');
        __int64 altBracketCount = count(change.begin(), change.end(), ']');

        if (bracketCount != altBracketCount)
        {
            ErrorMessage(1181, isGroup ? format + "_group" : format, behaviorFile, numline, change);
        }

        pos = -1;
        pos = change.find("crc32[");

        while (pos != NOT_FOUND)
        {
            size_t encode     = pos + 6;
            size_t curcounter = 1;

            for (unsigned int ch = encode; ch < change.length(); ++ch)
            {
                if (change[ch] == '[')
                    ++curcounter;
                else if (change[ch] == ']')
                {
                    --curcounter;

                    if (curcounter == 0)
                    {
                        ++encode;
                        break;
                    }
                }

                ++encode;
            }

            nemesis::scope blok(curPos + pos, curPos + encode, &proc::crc32);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
            pos = change.find("crc32[", pos + 1);
        }
    }

    if (change.find("import[") != NOT_FOUND)
    {
        pos = change.find("import[");

        while (pos != NOT_FOUND)
        {
            string import;
            size_t curcounter = 0;
            size_t post       = curPos + pos;

            for (unsigned int ch = pos; ch < change.length(); ++ch)
            {
                if (change[ch] == '[')
                    ++curcounter;
                else if (change[ch] == ']')
                {
                    --curcounter;

                    if (curcounter == 0)
                    {
                        if (ch == change.length() - 1 || change[ch + 1] != '[')
                        {
                            import.push_back(change[ch]);
                            break;
                        }
                    }
                }

                import.push_back(change[ch]);
            }

            __int64 bracketCount    = count(import.begin(), import.end(), '[');
            __int64 altBracketCount = count(import.begin(), import.end(), ']');

            if (bracketCount != altBracketCount)
            {
                ErrorMessage(1139, isGroup ? format + "_group" : format, behaviorFile, numline, import);
            }

            nemesis::scope blok(post, post + import.length(), &proc::import);
            isMC ? lineblocks[blok.size].push_back(make_shared<nemesis::scope>(blok))
                 : process->installBlock(blok, numline);
            pos = change.find("import[", pos + 1);
        }
    }

    if (change.find("MD", 0) != NOT_FOUND)
    {
        for (auto itr = nemesis::regex_iterator(change,
                                                nemesis::regex(format
                                                               + "\\[(F|N|B|L|[0-9]*)\\]\\[MD\\]"));
             itr != nemesis::regex_iterator();
             ++itr) {
            bool number  = false;
            string first = itr->str(1);
            size_t post  = curPos + itr->position();
            void (proc::*func)(nemesis::scope, VecStr&);

            if (first.length() == 0)
            {
                isTrueMulti = true;
                func        = isGroup ? &proc::motionDataMultiMaster : &proc::motionDataMultiGroup;
            }
            else if (first == "F")
            {
                func = isGroup ? &proc::motionDataFirstMaster : &proc::motionDataFirstGroup;
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::motionDataNextGroup;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::motionDataBackGroup;
            }
            else if (first == "L")
            {
                func = isGroup ? &proc::motionDataLastMaster : &proc::motionDataLastGroup;
            }
            else
            {
                number = true;
                func   = isGroup ? &proc::motionDataNumMaster : &proc::motionDataNumGroup;
            }

            shared_ptr<nemesis::scope> blok
                = number
                      ? make_shared<nemesis::scope>(
                          post, post + itr->str().length(), vector<int>{stoi(first)}, VecStr{change}, func)
                      : make_shared<nemesis::scope>(post, post + itr->str().length(), VecStr{change}, func);
            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        for (auto itr = nemesis::regex_iterator(change,
                                                nemesis::regex("(?<!" + format
                                                               + "\\[[F|N|B|L|\\d]\\]\\[)(?<!"
                                                               + format + "\\[\\]\\[)(?<!" + format
                                                               + "\\[\\d\\d\\]\\[)(MD)"));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post                     = curPos + itr->position();
            shared_ptr<nemesis::scope> blok = make_shared<nemesis::scope>(
                post,
                post + itr->str().length(),
                VecStr{change},
                isGroup ? &proc::motionDataMultiMaster : &proc::motionDataSingle);
            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        if (error) throw nemesis::exception();
    }

    if (change.find("RD", 0) != NOT_FOUND)
    {
        for (auto itr = nemesis::regex_iterator(change,
                                                nemesis::regex(format
                                                               + "\\[(F|N|B|L|[0-9]*)\\]\\[RD\\]"));
             itr != nemesis::regex_iterator();
             ++itr) {
            bool number  = false;
            string first = itr->str(1);
            size_t post  = curPos + itr->position();
            void (proc::*func)(nemesis::scope, VecStr&);

            if (first.length() == 0)
            {
                isTrueMulti = true;
                func        = isGroup ? &proc::rotationDataMultiMaster : &proc::rotationDataMultiGroup;
            }
            else if (first == "F")
            {
                func = isGroup ? &proc::rotationDataFirstMaster : &proc::rotationDataFirstGroup;
            }
            else if (first == "N")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::rotationDataNextGroup;
            }
            else if (first == "B")
            {
                if (isMaster)
                {
                    ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
                }
                else if (isGroup)
                {
                    ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
                }
                
                func = &proc::rotationDataBackGroup;
            }
            else if (first == "L")
            {
                func = isGroup ? &proc::rotationDataLastMaster : &proc::rotationDataLastGroup;
            }
            else
            {
                number = true;
                func   = isGroup ? &proc::rotationDataNumMaster : &proc::rotationDataNumGroup;
            }

            shared_ptr<nemesis::scope> blok
                = number ? make_shared<nemesis::scope>(
                      post, post + itr->str().length(), vector<int>{stoi(first)}, VecStr{change}, func)
                         : make_shared<nemesis::scope>(post, post + itr->str().length(), func);
            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        for (auto itr = nemesis::regex_iterator(change,
                                                nemesis::regex("(?<!" + format
                                                               + "\\[[F|N|B|L|\\d]\\]\\[)(?<!"
                                                               + format + "\\[\\]\\[)(?<!" + format
                                                               + "\\[\\d\\d\\]\\[)(RD)"));
             itr != nemesis::regex_iterator();
             ++itr) {
            if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

            size_t post                     = curPos + itr->position();
            shared_ptr<nemesis::scope> blok = make_shared<nemesis::scope>(
                post,
                post + itr->str().length(),
                VecStr{change},
                isGroup ? &proc::rotationDataMultiMaster : &proc::rotationDataSingle);
            isMC ? lineblocks[blok->size].push_back(blok) : process->installBlock(*blok, numline);
        }

        if (error) throw nemesis::exception();
    }
}
