#include "Global.h"

#include "debuglog.h"

#include "ui/Terminator.h"

#include "utilities/readtextfile.h"

#include "generate/animationdata.h"
#include "generate/behaviorprocess.h"
#include "generate/behaviorsubprocess.h"
#include "generate/generator_utility.h"

using namespace std;

extern Terminator* p_terminate;
extern atomic<int> m_RunningThread;

void BehaviorSub::ASDCompilation()
{
    try
    {
        ++m_RunningThread;

        try
        {
            CompilingASD();
        }
        catch (exception& ex)
        {
            ErrorMessage(6002, filelist[curList], ex.what());
        }
    }
    catch (nemesis::exception&)
    {
        // resolved exception
    }
    catch (...)
    {
        try
        {
            ErrorMessage(6002, filelist[curList], "New animation: Unknown");
        }
        catch (nemesis::exception&)
        {
            // resolved exception
        }
    }

    process->EndAttempt();
}

void BehaviorSub::CompilingASD()
{
    string filepath          = directory + filelist[curList];
    string behaviorFile      = filelist[curList].substr(0, filelist[curList].find_last_of("."));
    string lowerBehaviorFile = nemesis::to_lower_copy(behaviorFile);

    vecstr projectList;
    int projectcounter = 0;
    int headercounter  = 0;
    bool isOpen        = true;
    bool special       = false;
    string project     = "	";
    string header      = project;
    unordered_map<string, string> chosenLines;
    unordered_map<string, map<string, vecstr, alphanum_less>> ASDPack;
    unordered_map<string, unique_ptr<AnimationDataProject>> ASDData;

    process->newMilestone();

    {
        // read behavior file
        vecstr catalyst;
        vecstr newline;
        vecstr storeline;

        if (!GetFunctionLines(filepath, catalyst, false)) return;

        while (catalyst.back().length() == 0)
        {
            catalyst.pop_back();
        }

        DebugLogging("Processing behavior: " + filepath + " (Check point 1, File extraction complete)");
        process->newMilestone();

        // add picked behavior and remove not picked behavior
        // separation of all items for easier access and better compatibility
        for (string line : catalyst)
        {
            bool skip = false;

            if (line.find("<!-- ", 0) != NOT_FOUND)
            {
                if (line.find("<!-- NEW *", 0) != NOT_FOUND)
                {
                    size_t tempint = line.find("<!-- NEW *", 0) + 10;
                    string modID   = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

                    if (!chosenBehavior[modID]) isOpen = false;

                    skip = true;
                }
                else if (line.find("<!-- NEW ^", 0) != NOT_FOUND || line.find("<!-- FOREACH ^") != NOT_FOUND)
                {
                    special = true;
                }
                else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
                {
                    isOpen = true;

                    if (!special)
                        skip = true;
                    else
                        special = false;
                }
            }

            if (isOpen && !skip)
            {
                if (line.find("<!-- *", 0) != NOT_FOUND)
                {
                    size_t tempint     = line.find("<!-- *") + 6;
                    string modID       = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
                    chosenLines[modID] = line;
                }
                else
                {
                    if (line.find("<!-- original -->", 0) != NOT_FOUND)
                    {
                        if (chosenLines.size() != 0)
                        {
                            line = behaviorLineChooser(line, chosenLines, behaviorPriority);
                            chosenLines.clear();
                        }
                        else
                        {
                            ErrorMessage(1165);
                        }
                    }

                    if (line.find("//* delete this line *//") == NOT_FOUND) storeline.push_back(line);
                }
            }

            if (error) throw nemesis::exception();
        }

        for (int i = 1; i < storeline.size(); ++i)
        {
            if (isOnlyNumber(storeline[i])) break;

            projectList.push_back(storeline[i]);
            newline.push_back(storeline[i]);

            if (error) throw nemesis::exception();
        }

        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 2, AnimSetData mod selection complete)");
        unordered_map<string, vecstr> animDataSetHeader;
        animDataSetHeader[project].push_back(header);

        process->newMilestone();

        int num;
        string line;

        for (unsigned int i = projectList.size() + 1; i < storeline.size(); ++i)
        {
            line = storeline[i];

            if (i != storeline.size() - 1 && wordFind(storeline[i + 1], ".txt") != NOT_FOUND)
            {
                if (i != projectList.size() + 1)
                {
                    num = i;
                    break;
                }

                newline.reserve(100);
                newline.clear();
                project = projectList[projectcounter];
                ++projectcounter;
                headercounter = 0;
                animDataSetHeader[project].push_back("	");
                newline.push_back(storeline[i]);
                ++i;

                if (animDataSetHeader[project].size() != 1) ErrorMessage(5005, filepath, i + 1);

                while (i < storeline.size())
                {
                    if (wordFind(storeline[i], ".txt") != NOT_FOUND)
                    {
                        string curHeader = storeline[i];
                        animDataSetHeader[project].push_back(curHeader);
                    }
                    else if (wordFind(storeline[i], "V3") != NOT_FOUND)
                    {
                        if (headercounter >= animDataSetHeader[project].size())
                            ErrorMessage(5015, filepath, i + 1);

                        header = animDataSetHeader[project][headercounter];
                        ++headercounter;

                        if (header != "	" || project == "	")
                        {
                            newline.shrink_to_fit();
                            ASDPack[project][header] = newline;
                        }

                        newline.reserve(100);
                        newline.clear();
                        break;
                    }
                    else
                    {
                        ErrorMessage(5020, filepath, i + 1);
                    }

                    newline.push_back(storeline[i]);
                    ++i;
                }
            }
            else if (wordFind(storeline[i], "V3") != NOT_FOUND)
            {
                if (headercounter >= animDataSetHeader[project].size()) ErrorMessage(5015, filepath, i + 1);

                header = animDataSetHeader[project][headercounter];
                ++headercounter;

                if (header != "	" || project == "	")
                {
                    newline.shrink_to_fit();
                    ASDPack[project][header] = newline;
                }

                newline.reserve(100);
                newline.clear();
            }

            if (error) throw nemesis::exception();

            newline.push_back(storeline[i]);
        }

        process->newMilestone();

        for (int i = num; i < storeline.size(); ++i)
        {
            line = storeline[i];

            if (i != storeline.size() - 1 && wordFind(storeline[i + 1], ".txt") != NOT_FOUND)
            {
                header = animDataSetHeader[project][headercounter];

                if (header != "	" || project == "	")
                {
                    newline.shrink_to_fit();
                    ASDPack[project][header] = newline;
                }

                newline.reserve(100);
                newline.clear();
                project = projectList[projectcounter];
                ++projectcounter;
                headercounter = 0;
                animDataSetHeader[project].push_back("	");
                newline.push_back(storeline[i]);
                ++i;

                if (animDataSetHeader[project].size() != 1) ErrorMessage(5005, filepath, i + 1);

                while (i < storeline.size())
                {
                    if (wordFind(storeline[i], ".txt") != NOT_FOUND)
                    {
                        string curHeader = storeline[i];
                        animDataSetHeader[project].push_back(curHeader);
                    }
                    else if (wordFind(storeline[i], "V3") != NOT_FOUND)
                    {
                        if (headercounter >= animDataSetHeader[project].size())
                            ErrorMessage(5015, filepath, i + 1);

                        header = animDataSetHeader[project][headercounter];
                        ++headercounter;

                        if (header != "	" || project == "	")
                        {
                            newline.shrink_to_fit();
                            ASDPack[project][header] = newline;
                        }

                        newline.reserve(100);
                        newline.clear();
                        break;
                    }
                    else
                    {
                        ErrorMessage(5020, filepath, i + 1);
                    }

                    newline.push_back(storeline[i]);
                    ++i;
                }
            }
            else if (wordFind(storeline[i], "V3") != NOT_FOUND)
            {
                if (headercounter >= animDataSetHeader[project].size() - 1)
                    ErrorMessage(5015, filepath, i + 1);

                header = animDataSetHeader[project][headercounter];
                ++headercounter;

                if (header != "	" || project == "	")
                {
                    newline.shrink_to_fit();
                    ASDPack[project][header] = newline;
                }

                newline.reserve(100);
                newline.clear();
            }

            if (error) throw nemesis::exception();

            newline.push_back(storeline[i]);
        }

        if (newline.size() != 0)
        {
            header = animDataSetHeader[project][headercounter];

            if (header != "	" || project == "	")
            {
                newline.shrink_to_fit();
                ASDPack[project][header] = newline;
            }
        }

        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 3, AnimSetData general processing complete)");
        process->newMilestone();

        // check for having newAnimation for the file
        if (BehaviorTemplate->grouplist.find(lowerBehaviorFile) != BehaviorTemplate->grouplist.end()
            && BehaviorTemplate->grouplist[lowerBehaviorFile].size() > 0)
        {
            unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>>
                editExtract; // project, header, list of extracts, startline, extractions;
                             // to get all edits done to master branch

            for (auto& templateCode : BehaviorTemplate->grouplist[lowerBehaviorFile])
            {
                unordered_map<string, unordered_map<string, int>> ASDCount;
                bool hasGroup    = false;
                bool hasMaster   = false;
                bool ignoreGroup = false;

                if (newAnimation.find(templateCode) != newAnimation.end()
                    && newAnimation[templateCode].size() != 0)
                {
                    for (unsigned int k = 0; k < newAnimation[templateCode].size(); ++k)
                    {
                        unordered_map<string, map<string, vecstr, alphanum_less>> generatedASD;
                        newAnimation[templateCode][k]->GetAnimSetData(generatedASD);

                        for (auto it = generatedASD.begin(); it != generatedASD.end(); ++it)
                        {
                            string interproject = it->first;

                            if (ASDPack.find(interproject) == ASDPack.end())
                                projectList.push_back(interproject);

                            for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
                            {
                                string interheader = iter->first.substr(1, iter->first.length() - 2);
                                interheader.append("_" + to_string(++ASDCount[interproject][interheader])
                                                   + ".txt");

                                if (ASDPack[interproject][interheader].size() > 0)
                                    ErrorMessage(5012, templateCode, interproject, interheader);

                                if (error) throw nemesis::exception();

                                ASDPack[interproject][interheader] = iter->second;
                            }
                        }

                        DebugLogging(
                            "Processing behavior: " + filepath
                            + " (Check point 3.5, Mod code: " + templateCode + ", Existing AnimData"
                            + to_string(
                                BehaviorTemplate->existingFunctionID[templateCode][lowerBehaviorFile].size())
                            + ")");

                        for (auto it = BehaviorTemplate->existingASDHeader[templateCode].begin();
                             it != BehaviorTemplate->existingASDHeader[templateCode].end();
                             ++it)
                        {
                            string interproject = it->first;

                            for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
                            {
                                string interheader = *iter;

                                if (ASDPack[interproject][interheader].size() == 0)
                                    ErrorMessage(5011, templateCode, interproject, interheader);

                                map<int, vecstr> extract;
                                newAnimation[templateCode][k]->existingASDProcess(
                                    ASDPack[interproject][interheader], extract, vector<int>(1));
                                editExtract[interproject][interheader].push_back(extract);

                                if (error) throw nemesis::exception();
                            }
                        }

                        DebugLogging(
                            "Processing behavior: " + filepath
                            + " (Check point 3.5, Mod code: " + templateCode + ", Existing AnimData"
                            + to_string(
                                BehaviorTemplate->existingFunctionID[templateCode][lowerBehaviorFile].size())
                            + " COMPLETE)");
                    }
                }
                else
                {
                    for (auto& proj : BehaviorTemplate->existingASDHeader[templateCode])
                    {
                        string interproject = proj.first;

                        for (string interheader : proj.second)
                        {
                            if (ASDPack[interproject][interheader].size() == 0)
                                ErrorMessage(5011, templateCode, interproject, interheader);

                            editExtract[interproject][interheader].push_back(*new map<int, vecstr>());

                            if (error) throw nemesis::exception();
                        }
                    }
                }
            }

            process->newMilestone();

            for (auto& project : editExtract)
            {
                for (auto& header : project.second)
                {
                    map<int, vecstr> combined;
                    int totalline = 0;

                    for (auto& list : header.second)
                    {
                        for (auto& data : list)
                        {
                            combined[data.first].insert(
                                combined[data.first].end(), data.second.begin(), data.second.end());
                            totalline += int(data.second.size());
                        }
                    }

                    if (totalline == 0 && header.second.size() > 0 && header.second[0].size() > 0)
                        ErrorMessage(5014, project.first, header.first);

                    combineExtraction(
                        ASDPack[project.first][header.first], combined, project.first, header.first);

                    if (error) throw nemesis::exception();
                }
            }
        }
        else
        {
            process->newMilestone();
        }

        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 4, AnimSetData new animations complete)");
        process->newMilestone();

        for (string& curProject : projectList)
        {
            int startline = 0;
            vecstr projectline;
            projectline.push_back(to_string(ASDPack[curProject].size()));

            for (auto it : ASDPack[curProject])
            {
                if (it.first.find("<!--") != NOT_FOUND) continue;

                projectline.push_back(it.first);
            }

            for (auto it : ASDPack[curProject])
            {
                for (string& each : it.second)
                {
                    if (each.find("<!--") != NOT_FOUND) continue;

                    projectline.push_back(each);
                }
            }

            if (error) throw nemesis::exception();

            ASDData[curProject]
                = make_unique<AnimationDataProject>(startline, projectline, filepath, curProject);

            if (error) throw nemesis::exception();
        }

        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 5, AnimSetData format check complete)");
        process->newMilestone();

        // final output
#ifdef DEBUG
        filepath = "new_behaviors\\"
                   + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
        filepath = behaviorPath[lowerBehaviorFile];
#endif

        if (!FolderCreate(GetFileDirectory(filepath))) return;

        FileWriter output(filepath + ".txt");

        if (output.is_open())
        {
            output << to_string(projectList.size()) + "\n";

            for (string& curProject : projectList)
            {
                output << curProject + "\n";
            }

            for (string& curProject : projectList)
            {
                output << to_string(ASDData[curProject]->datalist.size()) + "\n";

                for (auto& data : ASDData[curProject]->datalist)
                {
                    output << data.first + "\n";
                }

                for (auto& data : ASDData[curProject]->datalist)
                {
                    output << "V3\n";
                    output << to_string(data.second.equiplist.size()) + "\n";

                    for (auto& equip : data.second.equiplist)
                    {
                        output << equip.name + "\n";
                    }

                    output << to_string(data.second.typelist.size()) + "\n";

                    for (auto& type : data.second.typelist)
                    {
                        output << type.name + "\n";
                        output << type.equiptype1 + "\n";
                        output << type.equiptype2 + "\n";
                    }

                    output << to_string(data.second.animlist.size()) + "\n";

                    for (auto& anim : data.second.animlist)
                    {
                        output << anim.eventname + "\n";
                        output << anim.unknown + "\n";
                        output << to_string(anim.attackClip.size()) + "\n";

                        for (auto& atk : anim.attackClip)
                        {
                            output << atk.data + "\n";
                        }
                    }

                    output << to_string(data.second.crc32list.size()) + "\n";

                    for (auto& crc32 : data.second.crc32list)
                    {
                        output << crc32.filepath + "\n";
                        output << crc32.filename + "\n";
                        output << crc32.fileformat + "\n";
                    }

                    if (error) throw nemesis::exception();
                }

                if (error) throw nemesis::exception();
            }
        }
        else
        {
            ErrorMessage(1025, filepath);
        }

        DebugLogging("Processing behavior: " + filepath + " (Check point 6, AnimSetData output complete)");
        process->newMilestone();
    }
}
