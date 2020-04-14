#include "Global.h"

#include "utilities/algorithm.h"

#include "generate/animation/optionlist.h"
#include "generate/animation/templateinfo.h"

#include "utilities/regex.h"

#pragma warning(disable : 4503)

using namespace std;
namespace sf = filesystem;

TemplateInfo::TemplateInfo()
{
    string templateDirectory = "behavior templates\\";
    string newpath;
    VecStr codelist;
    set<string> corelist;
    read_directory(templateDirectory, codelist);

    for (auto& code : codelist)
    {
        if (nemesis::to_lower_copy(code) != code) ErrorMessage(1191, code, nemesis::to_lower_copy(code));

        if (code.find(".", 0) == NOT_FOUND)
        {
            newpath = templateDirectory + code;
            sf::path FOF(newpath);

            if (code == "t" || code == "aaprefix" || code == "aaset" || code == "md" || code == "rd"
                || code == "+")
                ErrorMessage(3009, code);

            if (sf::is_directory(FOF))
            {
                VecStr folderlist;
                read_directory(newpath, folderlist);
                bool isCore        = false;
                bool isOptionExist = false;
                bool registered    = false;
                unordered_map<string, unordered_map<int, bool>>
                    isStateJoint; // behavior, node(function) ID, true/false; is this node(function) joining the animation template with the main branch?
                vector<sf::path> pathVector;

                for (auto& folder : folderlist)
                {
                    newpath = templateDirectory + code + "\\" + folder;
                    sf::path FOF2(newpath);

                    if (nemesis::iequals(folder, "option_list.txt") && !sf::is_directory(FOF2))
                    {
                        OptionList option(newpath, code);
                        option.setDebug(debug);
                        optionlist[code]   = option;
                        templatelist[code] = true;
                        isOptionExist      = true;

                        if (option.core)
                        {
                            isCore          = true;
                            string corefile = nemesis::to_lower_copy(option.coreBehavior);
                            corelist.insert(corefile);
                            coreBehaviorCode[corefile] = code;
                        }
                    }

                    pathVector.push_back(FOF2);
                }

                for (uint l = 0; l < pathVector.size(); ++l)
                {
                    if (sf::is_directory(pathVector[l]))
                    {
                        VecStr filelist;
                        newpath = pathVector[l].string();
                        read_directory(newpath, filelist);
                        string behaviorFolder      = pathVector[l].stem().string();
                        string lowerBehaviorFolder = nemesis::to_lower_copy(behaviorFolder);
                        bool noGroup               = true;

                        if (isCore)
                        {
                            if (behaviorJoints[lowerBehaviorFolder].size() == 0)
                                ErrorMessage(1182, code, templateDirectory + code + "\\option_list.txt");

                            behaviorJoints[nemesis::to_lower_copy(optionlist[code].coreBehavior)]
                                = behaviorJoints[lowerBehaviorFolder];
                        }

                        for (uint i = 0; i < filelist.size(); ++i)
                        {
                            newpath = templateDirectory + code + "\\" + behaviorFolder + "\\" + filelist[i];
                            sf::path file(newpath);
                            string lowerfilename = nemesis::to_lower_copy(filelist[i]);

                            if (!sf::is_directory(file))
                            {
                                if (nemesis::iequals(lowerfilename, code + "_group.txt"))
                                {
                                    if (behaviortemplate[code + "_group"][lowerBehaviorFolder].size() == 0)
                                    {
                                        noGroup = false;

                                        if (!GetFunctionLines(
                                                newpath,
                                                behaviortemplate[code + "_group"][lowerBehaviorFolder]))
                                            return;
                                    }
                                    else
                                    {
                                        ErrorMessage(1019, newpath);
                                    }
                                }
                                else if (nemesis::iequals(lowerfilename, code + "_master.txt"))
                                {
                                    if (behaviortemplate[code + "_master"][lowerBehaviorFolder].size() == 0)
                                    {
                                        noGroup = false;

                                        if (!GetFunctionLines(
                                                newpath,
                                                behaviortemplate[code + "_master"][lowerBehaviorFolder]))
                                            return;
                                    }
                                    else
                                    {
                                        ErrorMessage(1019, newpath);
                                    }
                                }
                                else if (nemesis::iequals(lowerfilename, code + ".txt"))
                                {
                                    registered = true;
                                    grouplist[lowerBehaviorFolder].insert(code);

                                    if (behaviortemplate[code][lowerBehaviorFolder].size() == 0)
                                    {
                                        if (!GetFunctionLines(newpath,
                                                              behaviortemplate[code][lowerBehaviorFolder]))
                                            return;
                                    }
                                    else
                                    {
                                        ErrorMessage(1019, newpath);
                                    }
                                }
                                else if (lowerfilename[0] == '#')
                                {
                                    string number = nemesis::regex_replace(string(lowerfilename),
                                                                         nemesis::regex("[^0-9]*([0-9]+).*"),
                                                                         string("\\1"));

                                    if (lowerfilename == "#" + number + ".txt" && isOnlyNumber(number))
                                    {
                                        existingFunctionID[code][lowerBehaviorFolder].push_back(stoi(number));
                                    }

                                    VecStr storeline;

                                    if (!GetFunctionLines(newpath, storeline)) return;

                                    bool isJoint        = false;
                                    bool isStateMachine = false;

                                    for (string line : storeline)
                                    {
                                        if (line.find("class=\"hkbStateMachine\" signature=\"") != NOT_FOUND)
                                        {
                                            isStateMachine = true; 
                                        }

                                        if (isStateMachine && line.find("<!-- FOREACH ^" + code) != NOT_FOUND)
                                        {
                                            if (!isJoint) isJoint = true;
                                        }
                                        else if (isStateMachine && line.find("<!-- CLOSE -->") != NOT_FOUND)
                                        {
                                            if (isJoint) isJoint = false;
                                        }

                                        if (isStateMachine && line.find("#" + code) != NOT_FOUND)
                                        {
                                            if (isJoint)
                                            {
                                                isStateJoint[lowerBehaviorFolder][stoi(number)] = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            else if (lowerBehaviorFolder == "animationdatasinglefile")
                            {
                                if (sf::is_directory(newpath))
                                {
                                    VecStr headerlist;
                                    read_directory(newpath, headerlist);
                                    grouplist[lowerBehaviorFolder].insert(code);
                                    string project = filelist[i] + ".txt";

                                    for (uint j = 0; j < headerlist.size(); ++j)
                                    {
                                        string header
                                            = headerlist[j].substr(0, headerlist[j].find_last_of("."));

                                        if (header[0] == '$'
                                            && (header.back() == '$'
                                                || (header.length() > 3
                                                    && header.rfind("$UC") == header.length() - 3)))
                                        {
                                            if (animdatatemplate[code][project][header].size() == 0)
                                            {
                                                if (!GetFunctionLines(newpath + "\\" + headerlist[j],
                                                                      animdatatemplate[code][project][header],
                                                                      true))
                                                    return;
                                            }
                                            else
                                            {
                                                ErrorMessage(1019, newpath);
                                            }
                                        }
                                        else
                                        {
                                            existingAnimDataHeader[code][project].insert(header);
                                        }
                                    }
                                }

                                noGroup = false;
                            }
                            else if (lowerBehaviorFolder == "animationsetdatasinglefile")
                            {
                                if (sf::is_directory(newpath) && filelist[i].find("~") != NOT_FOUND)
                                {
                                    VecStr headerlist;
                                    read_directory(newpath, headerlist);
                                    grouplist[lowerBehaviorFolder].insert(code);
                                    string project = filelist[i] + ".txt";

                                    while (project.find("~") != NOT_FOUND)
                                    {
                                        project.replace(project.find("~"), 1, "\\");
                                    }

                                    for (auto& curheader : headerlist)
                                    {
                                        sf::path thisfile(newpath + "\\" + curheader);

                                        if (!sf::is_directory(thisfile)
                                            && nemesis::iequals(thisfile.extension().string(), ".txt"))
                                        {
                                            string header = thisfile.stem().string();

                                            if (header[0] == '$' && header.back() == '$')
                                            {
                                                if (asdtemplate[code][project][header].size() == 0)
                                                {
                                                    if (!GetFunctionLines(thisfile.string(),
                                                                          asdtemplate[code][project][header],
                                                                          false))
                                                        return;
                                                }
                                                else
                                                {
                                                    ErrorMessage(1019, newpath);
                                                }
                                            }
                                            else
                                            {
                                                existingASDHeader[code][project].insert(header + ".txt");
                                            }
                                        }
                                    }
                                }

                                noGroup = false;
                            }
                        }

                        if (optionlist[code].multiState[lowerBehaviorFolder].size() > 1)
                        {
                            if (isStateJoint[lowerBehaviorFolder].size() == 0)
                                ErrorMessage(1074, templateDirectory + code);
                            else if (isStateJoint[lowerBehaviorFolder].size()
                                     != optionlist[code].multiState[lowerBehaviorFolder].size())
                                ErrorMessage(1073, templateDirectory + code);

                            for (auto it = optionlist[code].multiState[lowerBehaviorFolder].begin();
                                 it != optionlist[code].multiState[lowerBehaviorFolder].end();
                                 ++it)
                            {
                                if (isStateJoint[lowerBehaviorFolder][it->second])
                                    mainBehaviorJoint[code][lowerBehaviorFolder][it->first] = it->second;
                                else
                                    ErrorMessage(1075, templateDirectory + code);
                            }
                        }
                        else if (lowerBehaviorFolder != "animationdatasinglefile"
                                 && lowerBehaviorFolder != "animationsetdatasinglefile")
                        {
                            if (optionlist[code].multiState[lowerBehaviorFolder].size() == 1)
                                WarningMessage(1008, templateDirectory + code + "\\option_list.txt");

                            if (isStateJoint[lowerBehaviorFolder].size() > 1)
                                ErrorMessage(1072, templateDirectory + code);
                            else if (isStateJoint[lowerBehaviorFolder].size() == 0)
                                ErrorMessage(1074, templateDirectory + code);

                            mainBehaviorJoint[code][lowerBehaviorFolder][0]
                                = isStateJoint[lowerBehaviorFolder].begin()->first;
                        }

                        // Error checking
                        if (noGroup)
                        {
                            if (!optionlist[code].ignoreGroup)
                            {
                                if (optionlist[code].groupMin != -1 || optionlist[code].ruleOne.size() != 0
                                    || optionlist[code].ruleTwo.size() != 0)
                                    ErrorMessage(1061,
                                                 code,
                                                 templateDirectory + code + "\\" + behaviorFolder + "\\"
                                                     + code + "_group.txt");

                                if (behaviortemplate[code + "_master"].size() != 0)
                                    ErrorMessage(1085,
                                                 templateDirectory + code + "\\" + behaviorFolder + "\\"
                                                     + code + "_group.txt");
                            }
                        }
                        else if (optionlist[code].ignoreGroup)
                        {
                            ErrorMessage(1079, code, templateDirectory + code + "\\option_list.txt");
                        }

                        if (error) throw nemesis::exception();

                        filelist.clear();
                    }
                }

                if (!isOptionExist && registered) ErrorMessage(1021, newpath);

                if (error) throw nemesis::exception();

                folderlist.clear();
            }
        }
    }

    for (auto& animlist : grouplist)
    {
        // match behavior file, is the behavior file a core behavior?
        if (corelist.find(animlist.first) != corelist.end())
        {
            for (auto& animcode : animlist.second)
            {
                coreTemplate[animcode] = animlist.first;
            }
        }
    }
}
