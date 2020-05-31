#include "Global.h"

#include <QtCore/QFile.h>
#include <QtCore/QTextstream.h>

#include "debuglog.h"

#include "utilities/regex.h"
#include "utilities/algorithm.h"
#include "utilities/stringsplit.h"
#include "utilities/readtextfile.h"

#include "generate/animation/registeranimation.h"

using namespace std;

namespace sf = std::filesystem;

bool ruleCheck(VecStr rules, VecStr curList, TemplateInfo& behaviortemplate, string lowerformat)
{
    for (auto& rule : rules)
    {
        bool isMatched = false;

        for (auto& cur : curList)
        {
            bool matching = false;
            string number
                = nemesis::regex_replace(string(cur), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

            if (cur.length() == rule.length()
                || (behaviortemplate.optionlist[lowerformat].addOn[rule].size() != 0
                    && cur.length() > rule.length())
                || cur == rule + number)
            {
                for (uint k = 0; k < rule.length(); ++k)
                {
                    if (k != rule.length() - 1)
                    {
                        if (cur[k] == rule[k]) 
                        {
                            matching = true; 
                        }
                        else
                        {
                            matching = false;
                            break;
                        }
                    }
                    else
                    {
                        if (cur[k] == rule[k]) matching = true;

                        break;
                    }
                }
            }

            if (matching)
            {
                isMatched = true;
                break;
            }
        }

        if (!isMatched) return false;

        if (error) throw nemesis::exception();
    }

    return true;
}

registerAnimation::registerAnimation(sf::path curDirectory,
                                     sf::path filename,
                                     TemplateInfo behaviortemplate,
                                     sf::path bhvrPath,
                                     bool fstP,
                                     bool isNemesis)
{
    int linecount   = isNemesis ? 8 : 5;
    modID           = filename.string().substr(linecount, filename.string().find("_List.txt") - linecount);
    wstring filepath = curDirectory.wstring() + filename.wstring();
    linecount       = wordFind(filepath, L"\\animations\\", true);

    if (linecount == NOT_FOUND) ErrorMessage(1081);

    behaviorFile = bhvrPath;
    QFile qfile(QString::fromStdWString(filepath));
    string line;

    if (!qfile.open(QIODevice::ReadOnly)) ErrorMessage(1004, filename);

    linecount      = 0;
    int multiCount = -1;
    string previousShortline;
    string previousLine;
    string curAAprefix;
    unordered_map<string, bool> AAAnimFileExist;
    unordered_map<string, bool> AAprefixExist;
    QTextStream in(&qfile);

    while (!in.atEnd())
    {
        if (error) throw nemesis::exception();

        QString qline = in.readLine();
        string temp   = qline.toStdString();
        string temp2;
        ++linecount;
        bool hasInfo = false;

        for (auto& chr : temp)
        {
            if (chr == L'\'' || chr == L'\n' || chr == L'\0') break;

            temp2.push_back(chr);

            if (isalnum(chr)) hasInfo = true;
        }

        if (temp2.length() != 0 && hasInfo)
        {
            line = temp2;
            VecStr newAnimInfo;
            StringSplit(line, newAnimInfo);

            if (newAnimInfo[0].length() == 0) ErrorMessage(1016, filename, linecount);

            string lowerformat = nemesis::to_lower_copy(newAnimInfo[0]);

            if (lowerformat == "version")
            {
                if (nemesis::iequals(newAnimInfo[1].substr(0, 1), "V"))
                    version = line.substr(line.find(newAnimInfo[1]));
                else
                    version = "V" + line.substr(line.find(newAnimInfo[1]));
            }
            else if (lowerformat == "aaprefix")
            {
                if (string(line).substr(wordFind(line, "AAprefix") + 9).length() > 3
                    || newAnimInfo[1].length() != 3 || newAnimInfo.size() != 2)
                {
                    ErrorMessage(4002, filename, linecount, line);
                }

                curAAprefix        = newAnimInfo[1];
                string lowerPrefix = nemesis::to_lower_copy(curAAprefix);

                if (!AAprefixExist[lowerPrefix])
                {
                    if (AAprefixExist.size() > 30)
                    {
                        ErrorMessage(4007, filename, linecount, modID, newAnimInfo[1]);
                    }

                    AAprefixExist[lowerPrefix] = true;
                }
            }
            else if (lowerformat == "aaset")
            {
                if (curAAprefix.length() == 0) ErrorMessage(4006, filename, linecount, line);

                if (newAnimInfo.size() != 3) ErrorMessage(4003, filename, linecount, line);

                string lowGroupName = nemesis::to_lower_copy(newAnimInfo[1]);
                size_t groupSize    = groupAA[lowGroupName].size();

                if (groupSize == 0) ErrorMessage(4004, filename, linecount, line);

                if (newAnimInfo[2].length() > 0 && !isOnlyNumber(newAnimInfo[2]))
                {
                    ErrorMessage(4005, filename, linecount, line);
                }

                int num                          = stoi(newAnimInfo[2]);
                string lowGN                     = fstP ? lowGroupName + "_1p*" : lowGroupName;
                AAGroupCount[curAAprefix][lowGN] = num;
                groupAAPrefix[lowGN].push_back(curAAprefix);

                for (uint i = 0; i < groupSize; ++i) // list of animations in the animation group
                {
                    string lowAnim       = groupAA[lowGroupName][i];
                    string lowAnimPerson = (fstP ? lowAnim + "_1p*" : lowAnim);

                    for (int j = 0; j < num; ++j) // number of variation of the specific animation
                    {
                        string animFile = curAAprefix + to_string(j) + "_" + lowAnim;

                        if (!isFileExist(curDirectory.wstring() + nemesis::transform_to<wstring>(animFile)))
                        {
                            WarningMessage(1003, modID, animFile);
                            alternateAnim[lowAnimPerson].push_back("x");
                        }
                        else
                        {
                            alternateAnim[lowAnimPerson].push_back(modID + "\\" + animFile);
                            AAAnimFileExist[nemesis::to_lower_copy(fstP ? animFile + "_1p*" : animFile)]
                                = true;
                        }
                    }

                    if (alternateAnim[lowAnimPerson].size() > 128)
                        ErrorMessage(4008, filename, linecount, line);
                }

                if (error) throw nemesis::exception();
            }
            else if (lowerformat == "t")
            {
                if (newAnimInfo.size() < 4 || newAnimInfo.size() % 2 != 0)
                    ErrorMessage(4010, filename, linecount);

                string lowerAnimName
                    = nemesis::to_lower_copy(fstP ? newAnimInfo[1] + "_1p*" : newAnimInfo[1]);

                if (!AAAnimFileExist[lowerAnimName])
                {
                    ErrorMessage(4011, filename, linecount, newAnimInfo[1]);
                }
                else
                {
                    string originalAnim = lowerAnimName.substr(lowerAnimName.find("_") + 1);

                    if (newAnimInfo.size() > 2) AAHasEvent[originalAnim].push_back(lowerAnimName);

                    for (uint j = 2; j < newAnimInfo.size(); ++j)
                    {
                        AAEvent[lowerAnimName].push_back(newAnimInfo[j]);

                        if (j == 2) AAHasEvent[originalAnim].push_back(lowerAnimName);
                    }
                }
            }
            else if (lowerformat == "animvar")
            {
                size_t nextpos = 0;
                string name    = newAnimInfo[1];
                string type    = newAnimInfo[2];
                string value   = newAnimInfo[3];

                if (newAnimInfo.size() > 4) ErrorMessage(1023, filename, linecount, line);

                if (!isOnlyNumber(value)) ErrorMessage(1030, filename, linecount, line);

                bool failed = false;

                var variable(type, value, failed);

                if (failed) ErrorMessage(1018, filename, linecount, type);

                AnimVar[name] = variable;
            }
            else if (newAnimInfo[0] == "MD")
            {
                if (previousShortline.length() == 0) ErrorMessage(1143, filename, linecount);

                if (newAnimInfo.size() != 5) ErrorMessage(1089, filename, linecount);

                for (uint i = 1; i < newAnimInfo.size(); ++i)
                {
                    if (!isOnlyNumber(newAnimInfo[i])) ErrorMessage(1091, filename, linecount);
                }

                double timer = stod(newAnimInfo[1]);

                if (animInfo[previousShortline].back()->motionData.size() > 0)
                {
                    string* strptr = &animInfo[previousShortline].back()->motionData.back();

                    if (stod(strptr->substr(0, strptr->find(" "))) >= timer)
                    {
                        ErrorMessage(1087, filename, linecount);
                    }
                }

                shared_ptr<AnimationInfo> animInfo_ptr = animInfo[previousShortline].back();
                animInfo_ptr->motionData.push_back(newAnimInfo[1] + " " + newAnimInfo[2] + " "
                                                   + newAnimInfo[3] + " " + newAnimInfo[4]);
                animInfo_ptr->optionPicked["motion"] = true;

                if (animInfo_ptr->duration < timer
                    && (animInfo_ptr->optionPicked.find("D") == animInfo_ptr->optionPicked.end()
                        || !animInfo_ptr->optionPicked["D"]))
                {
                    animInfo_ptr->duration    = timer;
                    animInfo_ptr->hasDuration = true;
                }
            }
            else if (newAnimInfo[0] == "RD")
            {
                if (previousShortline.length() == 0) ErrorMessage(1143, filename, linecount);

                if (newAnimInfo.size() != 6) ErrorMessage(1088, filename, linecount);

                for (uint i = 1; i < newAnimInfo.size(); ++i)
                {
                    if (!isOnlyNumber(newAnimInfo[i])) ErrorMessage(1088, filename, linecount);
                }

                if (animInfo[previousShortline].back()->motionData.size() == 0)
                {
                    ErrorMessage(1090, filename, linecount);
                }

                double timer = stod(newAnimInfo[1]);

                if (animInfo[previousShortline].back()->rotationData.size() > 0)
                {
                    string* strptr = &animInfo[previousShortline].back()->rotationData.back();

                    if (stod(strptr->substr(0, strptr->find(" "))) >= timer)
                    {
                        ErrorMessage(1086, filename, linecount);
                    }
                }

                shared_ptr<AnimationInfo> animInfo_ptr = animInfo[previousShortline].back();
                animInfo_ptr->rotationData.push_back(newAnimInfo[1] + " " + newAnimInfo[2] + " "
                                                     + newAnimInfo[3] + " " + newAnimInfo[4] + " "
                                                     + newAnimInfo[5]);
                animInfo_ptr->optionPicked["rotation"] = true;

                if (animInfo_ptr->duration < timer
                    && (animInfo_ptr->optionPicked.find("D") == animInfo_ptr->optionPicked.end()
                        || !animInfo_ptr->optionPicked["D"]))
                {
                    animInfo_ptr->duration    = timer;
                    animInfo_ptr->hasDuration = true;
                }
            }
            else if (newAnimInfo[0] == "+")
            {
                if (previousShortline.length() > 0)
                {
                    if (behaviortemplate.templatelist[previousShortline])
                    {
                        bool isOExist = true;
                        string anim   = newAnimInfo[newAnimInfo.size() - 1];
                        string number = nemesis::regex_replace(
                            string(anim), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

                        if (isOnlyNumber(number) && anim.length() > number.length()
                            && anim[anim.length() - number.length() - 1] == '/'
                            && anim.rfind(number) == anim.length() - number.length())
                        {
                            isOExist                   = false;
                            OptionList* behaviorOption = &behaviortemplate.optionlist[previousShortline];

                            if (behaviorOption->storelist.find("o") == behaviorOption->storelist.end()
                                || !behaviorOption->storelist["o"])
                            {
                                if (behaviorOption->animObjectCount > 0) isOExist = true;
                            }
                        }

                        if (newAnimInfo.size() > 3 && newAnimInfo[1].length() > 0 && newAnimInfo[1][0] == '-')
                        {
                            animInfo[previousShortline].push_back(
                                make_shared<AnimationInfo>(newAnimInfo,
                                                           filename,
                                                           behaviortemplate.optionlist[previousShortline],
                                                           linecount,
                                                           isOExist));
                            animInfo[previousShortline].back()->addFilename(newAnimInfo[3]);

                            if (!animInfo[previousShortline].back()->known
                                && !isFileExist(filepath.substr(0, filepath.find_last_of(L"\\") + 1)
                                                + nemesis::transform_to<wstring>(newAnimInfo[3])))
                            {
                                WarningMessage(1000, newAnimInfo[3]);
                            }
                        }
                        else
                        {
                            animInfo[previousShortline].push_back(
                                make_shared<AnimationInfo>(newAnimInfo,
                                                           filename,
                                                           behaviortemplate.optionlist[previousShortline],
                                                           linecount,
                                                           isOExist,
                                                           true));
                            animInfo[previousShortline].back()->addFilename(newAnimInfo[2]);

                            if (!isFileExist(filepath.substr(0, filepath.find_last_of(L"\\") + 1)
                                             + nemesis::transform_to<wstring>(newAnimInfo[2])))
                            {
                                WarningMessage(1000, newAnimInfo[2]);
                            }
                        }

                        if (behaviortemplate.behaviortemplate.find(previousShortline + "_group")
                                != behaviortemplate.behaviortemplate.end()
                            && behaviortemplate.behaviortemplate[previousShortline + "_group"].size() != 0)
                        {
                            animInfo[previousShortline]
                                .back()
                                ->optionPickedCount[previousShortline + "_group"]
                                = 1;
                        }

                        if (behaviortemplate.behaviortemplate.find(previousShortline + "_master")
                                != behaviortemplate.behaviortemplate.end()
                            && behaviortemplate.behaviortemplate[previousShortline + "_master"].size() != 0)
                        {
                            animInfo[previousShortline]
                                .back()
                                ->optionPickedCount[previousShortline + "_master"]
                                = 1;
                        }

                        if (!isOExist) ErrorMessage(1000, filename, linecount);

                        templateType[previousShortline]++;
                        isMulti[previousShortline].push_back(multiCount);
                        multiCount++;
                    }
                }
                else
                {
                    ErrorMessage(1065, filename, linecount);
                }
            }
            else if (behaviortemplate.templatelist[lowerformat])
            {
                string anim   = newAnimInfo[newAnimInfo.size() - 1];
                bool isOExist = true;

                if (anim[anim.length() - 2] == '/'
                    && (anim[anim.length() - 1] == '1' || anim[anim.length() - 1] == '2'))
                    isOExist = false;

                // Has option
                if (newAnimInfo.size() > 3 && newAnimInfo[1].length() > 0 && newAnimInfo[1][0] == '-')
                {
                    animInfo[lowerformat].push_back(
                        make_shared<AnimationInfo>(newAnimInfo,
                                                   filename,
                                                   behaviortemplate.optionlist[lowerformat],
                                                   linecount,
                                                   isOExist));
                    animInfo[lowerformat].back()->addFilename(newAnimInfo[3]);

                    if (!animInfo[lowerformat].back()->known
                        && !isFileExist(filepath.substr(0, filepath.find_last_of(L"\\") + 1) + nemesis::transform_to<wstring>(newAnimInfo[3])))
                    {
                        WarningMessage(1000, newAnimInfo[3]);
                    }
                }
                else
                {
                    animInfo[lowerformat].push_back(
                        make_shared<AnimationInfo>(newAnimInfo,
                                                   filename,
                                                   behaviortemplate.optionlist[lowerformat],
                                                   linecount,
                                                   isOExist,
                                                   true));
                    animInfo[lowerformat].back()->addFilename(newAnimInfo[2]);

                    if (!isFileExist(filepath.substr(0, filepath.find_last_of(L"\\") + 1)
                                     + nemesis::transform_to<wstring>(newAnimInfo[2])))
                    {
                        WarningMessage(1000, newAnimInfo[2]);
                    }
                }

                if (error) throw nemesis::exception();

                if (behaviortemplate.behaviortemplate.find(lowerformat + "_group")
                        != behaviortemplate.behaviortemplate.end()
                    && behaviortemplate.behaviortemplate[lowerformat + "_group"].size() != 0)
                {
                    animInfo[lowerformat].back()->optionPickedCount[lowerformat + "_group"] = 1;
                }

                if (behaviortemplate.behaviortemplate.find(lowerformat + "_master")
                        != behaviortemplate.behaviortemplate.end()
                    && behaviortemplate.behaviortemplate[lowerformat + "_master"].size() != 0)
                {
                    animInfo[lowerformat].back()->optionPickedCount[lowerformat + "_master"] = 1;
                }

                if (!isOExist) ErrorMessage(1000, filename, linecount);

                if (previousShortline.length() > 0)
                {
                    if (behaviortemplate.optionlist[previousShortline].groupMin > multiCount)
                    {
                        ErrorMessage(1034,
                                     behaviortemplate.optionlist[previousShortline].groupMin,
                                     previousShortline,
                                     filename,
                                     linecount - 1);
                    }

                    for (int i = 0; i < multiCount; ++i)
                    {
                        last[previousShortline].push_back(multiCount - 1);

                        if (i == multiCount - 1 && i != 0)
                        {
                            if (behaviortemplate.optionlist[previousShortline].ruleTwo.size() != 0)
                            {
                                VecStr AnimInfo;
                                StringSplit(previousLine, AnimInfo);

                                if (AnimInfo.size() > 3 && AnimInfo[1].length() > 0 && AnimInfo[1][0] == '-')
                                {
                                    string templine = AnimInfo[1];
                                    templine        = templine.substr(1);
                                    VecStr curList;
                                    size_t numOption = count(templine.begin(), templine.end(), ',') + 1;
                                    size_t nextOption;
                                    size_t pos = 0;

                                    for (uint i = 0; i < numOption; ++i)
                                    {
                                        nextOption = templine.find(",", pos);

                                        if (nextOption != -1)
                                        {
                                            curList.push_back(templine.substr(pos, nextOption - pos));
                                            pos = nextOption + 1;
                                        }
                                        else
                                        {
                                            curList.push_back(templine.substr(pos));
                                        }
                                    }

                                    if (!ruleCheck(behaviortemplate.optionlist[previousShortline].ruleTwo,
                                                   curList,
                                                   behaviortemplate,
                                                   previousShortline))
                                    {
                                        ErrorMessage(1035, filename, linecount);
                                    }
                                }
                                else
                                {
                                    ErrorMessage(1035, filename, linecount);
                                }
                            }
                        }
                    }

                    if (error) throw nemesis::exception();
                }

                if (behaviortemplate.optionlist[lowerformat].ruleOne.size() != 0
                    || behaviortemplate.optionlist[lowerformat].compulsory.size() != 0)
                {
                    if (newAnimInfo.size() > 3 && newAnimInfo[1].length() > 0 && newAnimInfo[1][0] == '-')
                    {
                        string templine = newAnimInfo[1];
                        templine        = templine.substr(1);
                        VecStr curList;
                        size_t numOption = count(templine.begin(), templine.end(), ',') + 1;
                        size_t nextOption;
                        size_t pos = 0;

                        for (size_t i = 0; i < numOption; ++i)
                        {
                            nextOption = templine.find(",", pos);

                            if (nextOption != -1)
                            {
                                curList.push_back(templine.substr(pos, nextOption - pos));
                                pos = nextOption + 1;
                            }
                            else
                            {
                                curList.push_back(templine.substr(pos));
                            }
                        }

                        if (!ruleCheck(behaviortemplate.optionlist[lowerformat].compulsory,
                                       curList,
                                       behaviortemplate,
                                       lowerformat))
                        {
                            ErrorMessage(1035, filename, linecount);
                        }

                        if (!ruleCheck(behaviortemplate.optionlist[lowerformat].ruleOne,
                                       curList,
                                       behaviortemplate,
                                       lowerformat))
                        {
                            ErrorMessage(1035, filename, linecount);
                        }
                    }
                    else
                    {
                        ErrorMessage(1035, filename, linecount);
                    }
                }

                multiCount = 0;
                ++templateType[lowerformat];
                isMulti[lowerformat].push_back(multiCount);
                previousShortline = lowerformat;
                ++multiCount;
            }
            else
            {
                ErrorMessage(1016, filename, linecount);
            }

            if (error) throw nemesis::exception();

            previousLine = line;
        }

        if (error) throw nemesis::exception();
    }

    qfile.close();

    if (error) throw nemesis::exception();

    ++linecount;

    if (behaviortemplate.optionlist[previousShortline].groupMin > multiCount)
    {
        ErrorMessage(1034,
                     behaviortemplate.optionlist[previousShortline].groupMin,
                     previousShortline,
                     filename,
                     linecount - 1);
    }

    for (int i = 0; i < multiCount; ++i)
    {
        last[previousShortline].push_back(multiCount - 1);

        if (i == multiCount - 1 && i != 0)
        {
            if (behaviortemplate.optionlist[previousShortline].ruleTwo.size() != 0)
            {
                string tempPreviousShortline = multiCount > 1 ? "+" : previousShortline;

                if (line[tempPreviousShortline.length() + 1] == '-')
                {
                    VecStr AnimInfo;
                    StringSplit(line, AnimInfo);
                    string templine = AnimInfo[1];
                    templine        = templine.substr(1);
                    VecStr curList;
                    size_t numOption = count(templine.begin(), templine.end(), ',') + 1;
                    size_t nextOption;
                    size_t pos = 0;

                    for (uint i = 0; i < numOption; ++i)
                    {
                        nextOption = templine.find(",", pos);

                        if (nextOption != -1)
                        {
                            curList.push_back(templine.substr(pos, nextOption - pos));
                            pos = nextOption + 1;
                        }
                        else
                        {
                            curList.push_back(templine.substr(pos));
                        }
                    }

                    if (!ruleCheck(behaviortemplate.optionlist[previousShortline].ruleTwo,
                                   curList,
                                   behaviortemplate,
                                   previousShortline))
                    {
                        ErrorMessage(1035, filename, linecount);
                    }
                }
                else
                {
                    ErrorMessage(1035, filename, linecount);
                }
            }
        }
    }

    if (error) throw nemesis::exception();

    std::filesystem::path pfile(filepath);
    wstring msg = L": "
                  + (fstP ? pfile.parent_path().stem().wstring() + L" (1st Person)"
                          : pfile.parent_path().stem().wstring());
    interMsg(TextBoxMessage(1006) + msg);
    DebugLogging(nemesis::transform_to<wstring>(EngTextBoxMessage(1006)) + msg);
}

void registerAnimation::clear()
{
	AnimVar.clear();
	templateType.clear();
	isMulti.clear();
	last.clear();
}

var::var(string inputtype, string inputvalue, bool& failed)
{
	nemesis::to_lower(inputtype);

	if (inputtype == "bool") var_type = "BOOL";
	else if (inputtype == "int32") var_type = "INT32";
	else if (inputtype == "real") var_type = "REAL";
	else failed = true;

	init_value = inputvalue;
}
