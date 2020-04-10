#include "Global.h"

#include "debuglog.h"

#include "ui/Terminator.h"

#include "utilities/readtextfile.h"

#include "generate/animationdata.h"
#include "generate/animationdatatracker.h"
#include "generate/behaviorprocess.h"
#include "generate/behaviorsubprocess.h"
#include "generate/generator_utility.h"

using namespace std;

extern Terminator* p_terminate;
extern atomic<int> m_RunningThread;
extern atomic<int> behaviorRun;
extern mutex cv2_m;
extern condition_variable cv2;

struct invertInt
{
    short code = SHRT_MAX;

    invertInt()
    {}
    invertInt(short _code)
    {
        code = _code;
    }

    short& operator=(short _code)
    {
        code = _code;
        return code;
    }

    short operator++()
    {
        return code++;
    }

    short operator--()
    {
        return code--;
    }

    short& to_int()
    {
        return code;
    }
};

void BehaviorSub::AnimDataCompilation()
{
    try
    {
        ++m_RunningThread;

        try
        {
            CompilingAnimData();
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

void BehaviorSub::CompilingAnimData()
{
    string filepath          = directory + filelist[curList];
    string behaviorFile      = filelist[curList].substr(0, filelist[curList].find_last_of("."));
    string lowerBehaviorFile = nemesis::to_lower_copy(behaviorFile);

    vecstr projectList;                          // list of projects
    unordered_map<string, int> projectNameCount; // count the occurance of the project name
    vector<unique_ptr<AnimDataProject>> ADProject;
    unordered_map<string, int> nextProject;

    string project;
    //string header;

    process->newMilestone();

    {
        unordered_map<string, invertInt> uCode;                             // project, highest unique code
        unordered_map<string, unordered_map<string, string>> uniqueModCode; // project, mod code, unique code
        unordered_map<string, unordered_map<string, vecstr>> catalystMap;   // project, header, list of lines
        unordered_map<string, vecstr> animDataHeader;                       // project, list of headers
        unordered_map<string, vecstr> animDataInfo;                         // project, list of info headers

        {
            // read behavior file
            vecstr catalyst;
            vecstr newline;
            unordered_map<string, string> chosenLines;

            if (!GetFunctionLines(filepath, newline)) return;

            DebugLogging("Processing behavior: " + filepath + " (Check point 1, File extraction complete)");
            process->newMilestone();

            bool isOpen  = true;
            bool special = false;
            catalyst.reserve(newline.size());

            for (auto& line : newline)
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
                    else if (line.find("<!-- NEW ^", 0) != NOT_FOUND
                             || line.find("<!-- FOREACH ^") != NOT_FOUND)
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

                if (isOpen && !skip) { catalyst.push_back(line); }
            }

            newline.clear();
            int projectcounter = 0;
            bool isInfo        = false;
            int num            = 0;
            projectList.reserve(500);

            for (int i = 1; i < catalyst.size(); ++i)
            {
                if (catalyst[i].find(".txt") == NOT_FOUND)
                {
                    num = i;
                    break;
                }

                projectList.push_back(catalyst[i]);
            }

            projectList.shrink_to_fit();
            project       = projectList[0] + " " + to_string(++projectNameCount[projectList[0]]);
            string header = "$header$";
            animDataHeader[project].push_back(header);
            newline.reserve(20);
            newline.clear();

            process->newMilestone();

            // add picked behavior and remove not picked behavior
            // separation of all items for easier access and better compatibility
            for (unsigned int l = num; l < catalyst.size(); ++l)
            {
                string line = catalyst[l];

                if (line.find("<!-- *", 0) != NOT_FOUND)
                {
                    size_t tempint     = line.find("<!-- *") + 6;
                    string modID       = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
                    chosenLines[modID] = line;
                    continue;
                }
                else if (line.find("<!-- original -->", 0) != NOT_FOUND)
                {
                    if (chosenLines.size() == 0) ErrorMessage(1165);

                    line = behaviorLineChooser(line, chosenLines, behaviorPriority);
                    chosenLines.clear();
                }

                if (l + 3 < catalyst.size() && l > 2)
                {
                    bool empty = false;

                    if (catalyst[l - 1] == "") { empty = true; }
                    else
                    {
                        int next = -1;

                        while (l + next >= 0 && catalyst[l + next].find("<!--") != NOT_FOUND)
                        {
                            --next;
                        }

                        if (catalyst[l + next] == "") empty = true;
                    }

                    if (empty)
                    {
                        bool out = false;

                        if (isOnlyNumber(line))
                        {
                            int next = 1;

                            if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
                                ++next;

                            if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
                            {
                                ++next;

                                if (l + next < catalyst.size()
                                    && catalyst[l + next].find("<!--") != NOT_FOUND)
                                    ++next;

                                if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
                                {
                                    int nextnext = next + 1;

                                    if (l + next < catalyst.size()
                                        && catalyst[l + next].find("<!--") != NOT_FOUND)
                                        ++nextnext;

                                    if (catalyst[l + next] == "0"
                                        || (l + nextnext < catalyst.size()
                                            && catalyst[l + nextnext].find("\\") != NOT_FOUND))
                                    {
                                        newline.shrink_to_fit();
                                        catalystMap[project][header] = newline;
                                        string newproject            = projectList[++projectcounter];
                                        header                       = "$header$";
                                        project
                                            = newproject + " " + to_string(++projectNameCount[newproject]);
                                        animDataHeader[project].push_back(header);
                                        newline.reserve(20);
                                        newline.clear();
                                        isInfo = false;
                                        out    = true;
                                    }
                                }
                            }
                        }

                        if (!out)
                        {
                            if (!isInfo)
                            {
                                if (hasAlpha(line))
                                {
                                    if (isOnlyNumber(catalyst[l + 1])) // next anim header
                                    {
                                        newline.shrink_to_fit();
                                        catalystMap[project][header] = newline;
                                        newline.reserve(20);
                                        newline.clear();
                                        header = line + " " + catalyst[l + 1];
                                        animDataHeader[project].push_back(header);
                                    }
                                    else // new anim header added by mod
                                    {
                                        string number
                                            = boost::regex_replace(string(catalyst[l + 1]),
                                                                   boost::regex("[a-zA-Z]+[$]([0-9]+)"),
                                                                   string("\\1"));

                                        if (number != catalyst[l + 1] && isOnlyNumber(number))
                                        {
                                            string modcode  = catalyst[l + 1];
                                            catalyst[l + 1] = to_string(uCode[project].to_int());
                                            --uCode[project];
                                            uniqueModCode[project][modcode] = catalyst[l + 1];
                                            newline.shrink_to_fit();
                                            catalystMap[project][header] = newline;
                                            newline.reserve(20);
                                            newline.clear();
                                            header = line + " " + catalyst[l + 1];
                                            animDataHeader[project].push_back(header);
                                        }
                                    }
                                }
                                else if (isOnlyNumber(line)) // is info
                                {
                                    isInfo = true;
                                    newline.shrink_to_fit();
                                    catalystMap[project][header] = newline;
                                    ;
                                    newline.reserve(20);
                                    newline.clear();
                                    string number = boost::regex_replace(string(catalyst[++l]),
                                                                         boost::regex("[a-zA-Z]+[$]([0-9]+)"),
                                                                         string("\\1"));

                                    if (number != catalyst[l] && isOnlyNumber(number))
                                    {
                                        if (uniqueModCode[project].find(catalyst[l])
                                            != uniqueModCode[project].end())
                                        { catalyst[l] = uniqueModCode[project][catalyst[l]]; }
                                        else
                                        {
                                            WarningMessage(1024, catalyst[l]);
                                            short& refCode                      = uCode[project].to_int();
                                            uniqueModCode[project][catalyst[l]] = to_string(refCode);
                                            catalyst[l]                         = to_string(refCode);
                                            --refCode;
                                        }
                                    }

                                    line   = catalyst[l];
                                    header = line;
                                    animDataInfo[project].push_back(header);
                                }
                            }
                            else if (isOnlyNumber(line)) // next info
                            {
                                newline.shrink_to_fit();
                                catalystMap[project][header] = newline;
                                newline.reserve(20);
                                newline.clear();
                                header = line;
                                animDataInfo[project].push_back(header);
                            }
                            else // new info added by mod
                            {
                                string number = boost::regex_replace(
                                    string(line), boost::regex("[a-zA-Z]+[$]([0-9]+)"), string("\\1"));

                                if (number != line && isOnlyNumber(number))
                                {
                                    if (uniqueModCode[project].find(line) != uniqueModCode[project].end())
                                    { line = uniqueModCode[project][line]; }
                                    else
                                    {
                                        WarningMessage(1024, line);
                                        short& refCode               = uCode[project].to_int();
                                        uniqueModCode[project][line] = to_string(refCode);
                                        line                         = to_string(refCode);
                                        --refCode;
                                    }

                                    newline.shrink_to_fit();
                                    catalystMap[project][header] = newline;
                                    newline.reserve(20);
                                    newline.clear();
                                    header = line;
                                    animDataInfo[project].push_back(header);
                                }
                            }
                        }
                    }
                    else if (header == "$header$")
                    {
                        if (hasAlpha(line) && line.find("\\") == NOT_FOUND && l + 1 < catalyst.size())
                        {
                            if (isOnlyNumber(catalyst[l + 1])) // if it is unique code
                            {
                                newline.shrink_to_fit();
                                catalystMap[project][header] = newline;
                                newline.reserve(20);
                                newline.clear();
                                header = line + " " + catalyst[l + 1];
                                animDataHeader[project].push_back(header);
                            }
                            else
                            {
                                string number = boost::regex_replace(string(catalyst[l + 1]),
                                                                     boost::regex("[a-zA-Z]+[$]([0-9]+)"),
                                                                     string("\\1"));

                                if (number != catalyst[l + 1] && isOnlyNumber(number))
                                {
                                    string modcode  = catalyst[l + 1];
                                    short& refCode  = uCode[project].to_int();
                                    catalyst[l + 1] = to_string(refCode);
                                    --refCode;
                                    uniqueModCode[project][modcode] = catalyst[l + 1];
                                    newline.shrink_to_fit();
                                    catalystMap[project][header] = newline;
                                    newline.reserve(20);
                                    newline.clear();
                                    header = line + " " + catalyst[l + 1];
                                    animDataHeader[project].push_back(header);
                                }
                            }
                        }
                        else if (isOnlyNumber(catalyst[l - 1]) && catalyst[l - 1] == "0"
                                 && isOnlyNumber(line))
                        {
                            int next = 1;

                            if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
                            {
                                ++next;

                                if (l + next < catalyst.size()
                                    && catalyst[l + next].find("<!--") != NOT_FOUND)
                                    ++next;

                                if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
                                {
                                    int nextnext = next + 1;

                                    if (l + next < catalyst.size()
                                        && catalyst[l + next].find("<!--") != NOT_FOUND)
                                        ++nextnext;

                                    if (catalyst[l + next] == "0"
                                        || (l + nextnext < catalyst.size()
                                            && catalyst[l + nextnext].find("\\")
                                                   != NOT_FOUND)) // next project
                                    {
                                        newline.shrink_to_fit();
                                        catalystMap[project][header] = newline;
                                        string newproject            = projectList[++projectcounter];
                                        project
                                            = newproject + " " + to_string(++projectNameCount[newproject]);
                                        animDataHeader[project].push_back(header);
                                        newline.reserve(20);
                                        newline.clear();
                                        isInfo = false;
                                    }
                                }
                            }
                        }
                    }
                }

                newline.push_back(line);

                if (error) throw nemesis::exception();
            }

            if (newline.size() != 0)
            {
                if (newline.back().length() == 0) newline.pop_back();

                newline.shrink_to_fit();
                catalystMap[project][header] = newline;
                newline.clear();
            }
        }

        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 2, AnimData general processing complete)");
        process->newMilestone();

        unique_lock<mutex> ulock(cv2_m);

        if (behaviorRun > 0)
        {
            cv2.wait(ulock, [] { return behaviorRun == 0; });
        }

        // check for having newAnimation for the file
        if (BehaviorTemplate->grouplist.find(lowerBehaviorFile) != BehaviorTemplate->grouplist.end()
            && BehaviorTemplate->grouplist[lowerBehaviorFile].size() > 0)
        {
            unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;
            unordered_map<string, unordered_map<string, int>> ASDCount;

            for (auto& templateCode : BehaviorTemplate->grouplist[lowerBehaviorFile])
            {
                bool hasGroup    = false;
                bool hasMaster   = false;
                bool ignoreGroup = false;

                if (newAnimation.find(templateCode) != newAnimation.end()
                    && newAnimation[templateCode].size() != 0)
                {
                    for (auto& eachNewAnim : newAnimation[templateCode])
                    {
                        unordered_map<string, map<string, vecstr>> generatedAnimData;
                        eachNewAnim->GetAnimData(generatedAnimData);

                        for (auto& _animData : generatedAnimData)
                        {
                            project = _animData.first;
                            string projectplus;
                            unordered_map<string, string> isExist;

                            if (projectNameCount[project] == 0)
                            {
                                projectplus = project + " " + to_string(++projectNameCount[project]);
                                projectList.push_back(project);
                            }
                            else
                            {
                                projectplus = project + " " + to_string(projectNameCount[project]);
                            }

                            for (auto data : _animData.second)
                            {
                                size_t size = data.first.size();

                                if (data.second.size() > 0)
                                {
                                    string header = data.second[0];

                                    if (size > 2 && data.first[size - 3] == '$' && data.first[size - 2] == 'U'
                                        && data.first.back() == 'C') // info data
                                    {
                                        if (data.first[0] != '$') ErrorMessage(3006, project, data.first);

                                        if (isExist[header].length() == 0)
                                        {
                                            if (!isOnlyNumber(data.second[0]))
                                            {
                                                short& refCode = uCode[projectplus].to_int();
                                                header         = to_string(refCode);
                                                --refCode;
                                                isExist[header] = header;
                                                data.second[0]  = header;
                                            }

                                            animDataInfo[projectplus].push_back(header);
                                        }
                                    }
                                    else if (data.first[0] == '$' && data.first.back() == '$') // anim data
                                    {
                                        if (isExist[header].length() == 0)
                                        {
                                            if (!isOnlyNumber(data.second[1]))
                                            {
                                                short& refCode = uCode[projectplus].to_int();
                                                data.second[1] = to_string(refCode);
                                                --refCode;
                                                isExist[header] = data.second[1];
                                            }

                                            header.append(" " + data.second[1]);
                                            animDataHeader[projectplus].push_back(header);
                                        }
                                    }
                                    else
                                    {
                                        ErrorMessage(3003, data.first + ".txt");
                                    }

                                    if (catalystMap[projectplus][header].size() > 0)
                                        ErrorMessage(3024, templateCode, project, header);

                                    if (projectNameCount[project] != 1)
                                    {
                                        WarningMessage(1020, project, header);

                                        for (int i = 1; i <= projectNameCount[project]; ++i)
                                        {
                                            catalystMap[project + " " + to_string(i)][header] = data.second;
                                        }
                                    }
                                    else
                                    {
                                        catalystMap[projectplus][header] = data.second;
                                    }
                                }

                                if (error) throw nemesis::exception();
                            }
                        }

                        for (auto& project_itr : BehaviorTemplate->existingAnimDataHeader[templateCode])
                        {
                            string project = project_itr.first;

                            for (string header : project_itr.second)
                            {
                                if (catalystMap[project][header].size() == 0)
                                    ErrorMessage(5011, templateCode, project, header);

                                map<int, vecstr> extract;
                                eachNewAnim->existingASDProcess(catalystMap[project][header], extract);

                                if (error) throw nemesis::exception();

                                editExtract[project][header].push_back(extract);
                            }
                        }

                        if (error) throw nemesis::exception();
                    }
                }
                else
                {
                    for (auto& project_itr : BehaviorTemplate->existingAnimDataHeader[templateCode])
                    {
                        string project = project_itr.first;

                        for (string header : project_itr.second)
                        {
                            if (catalystMap[project][header].size() == 0)
                                ErrorMessage(5011, templateCode, project, header);

                            editExtract[project][header].push_back(map<int, vecstr>());

                            if (error) throw nemesis::exception();
                        }
                    }
                }

                if (error) throw nemesis::exception();
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

                    for (int i = 1; i <= projectNameCount[project.first]; ++i)
                    {
                        combineExtraction(catalystMap[project.first + " " + to_string(i)][header.first],
                                          combined,
                                          project.first,
                                          header.first);
                    }

                    if (error) throw nemesis::exception();
                }

                if (error) throw nemesis::exception();
            }
        }
        else
        {
            process->newMilestone();
        }

        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 3, AnimData general new animations complete)");
        process->newMilestone();
        DebugLogging("Processing behavior: " + filepath
                     + " (Check point 3.4, AnimData general new animations complete)");

        /* unsure of the function but is present in FNIS

		if (alternateAnim.size() > 0)
		{
			unordered_map<string, bool> isExist;

			for (auto it : alternateAnim)
			{
				for (auto iter : it.second)
				{
					if (!isExist[iter] && iter != "x")
					{
						isExist[iter] = true;
						string animFileName = std::filesystem::path(iter).filename().string();
						auto& ptr = charAnimDataInfo.find("defaultfemale");

						if (ptr != charAnimDataInfo.end())
						{
							auto& ptr2 = ptr->second.find(animFileName);

							if (ptr2 != ptr->second.end())
							{
								for (auto& header : ptr2->second->cliplist)
								{
									vecstr newlines;
									project = "DefaultFemale.txt 1";
									int& refCode = uCode[project].to_int();

									newlines.push_back(header);
									newlines.push_back(to_string(refCode));
									newlines.push_back("1");
									newlines.push_back("0");
									newlines.push_back("0");
									newlines.push_back("0");
									newlines.push_back("");

									--refCode;
									animDataHeader[project].push_back(header + to_string(animDataHeader[project].size()));
									catalystMap[project][header + to_string(animDataHeader[project].size() - 1)] = newlines;

									if (error) throw nemesis::exception();
								}
							}
						}

						ptr = charAnimDataInfo.find("defaultmale");

						if (ptr != charAnimDataInfo.end())
						{
							auto& ptr2 = ptr->second.find(animFileName);

							if (ptr2 != ptr->second.end())
							{
								for (auto& header : ptr2->second->cliplist)
								{
									vecstr newlines;
									project = "DefaultMale.txt 1";
									int& refCode = uCode[project].to_int();

									newlines.push_back(header);
									newlines.push_back(to_string(refCode));
									newlines.push_back("1");
									newlines.push_back("0");
									newlines.push_back("0");
									newlines.push_back("0");
									newlines.push_back("");

									--refCode;
									animDataHeader[project].push_back(header + to_string(animDataHeader[project].size()));
									catalystMap[project][header + to_string(animDataHeader[project].size() - 1)] = newlines;

									if (error) throw nemesis::exception();
								}
							}
						}
					}
				}

				if (error) throw nemesis::exception();
			}

			DebugLogging("Processing behavior: " + filepath + " (Check point 3.5, AnimData AA complete)");
		}*/

        process->newMilestone();

        try
        {
            for (string& curProject : projectList)
            {
                if (projectNameCount[curProject] > 1)
                    project = curProject + " " + to_string(++nextProject[curProject]);
                else
                    project = curProject + " 1";

                vecstr combined;

                for (string& header : animDataHeader[project])
                {
                    combined.reserve(combined.size() + catalystMap[project][header].size());

                    for (string& curline : catalystMap[project][header])
                    {
                        if (curline.find("//* delete this line *//") != NOT_FOUND) continue;

                        combined.push_back(curline);
                    }
                }

                for (string& header : animDataInfo[project])
                {
                    combined.reserve(combined.size() + catalystMap[project][header].size());

                    for (string& curline : catalystMap[project][header])
                    {
                        if (curline.find("//* delete this line *//") != NOT_FOUND) continue;

                        combined.push_back(curline);
                    }
                }

                if (combined.size() == 0) ErrorMessage(5017, curProject);

                if (error) throw nemesis::exception();

                try
                {
                    ADProject.push_back(make_unique<AnimDataProject>(combined, curProject, filepath));
                }
                catch (int)
                {
                    return;
                }

                if (error) throw nemesis::exception();
            }
        }
        catch (int)
        {
            return;
        }
    }

    DebugLogging("Processing behavior: " + filepath + " (Check point 4, AnimData format check complete)");
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

        for (string& project : projectList)
        {
            output << project + "\n";
        }

        for (auto& project : ADProject)
        {
            output << to_string(project->GetAnimTotalLine()) + "\n";
            output << project->projectActive + "\n";
            output << to_string(project->behaviorlist.size()) + "\n";

            for (auto& behavior : project->behaviorlist)
            {
                output << behavior + "\n";
            }

            output << project->childActive + "\n";

            if (project->childActive != "0")
            {
                for (auto& animdata : project->animdatalist)
                {
                    output << animdata.name + "\n";
                    output << animdata.uniquecode + "\n";
                    output << animdata.unknown1 + "\n";
                    output << animdata.unknown2 + "\n";
                    output << animdata.unknown3 + "\n";
                    output << to_string(animdata.eventname.size()) + "\n";

                    for (auto& eventname : animdata.eventname)
                    {
                        output << eventname + "\n";
                    }

                    output << "\n";

                    if (error) throw nemesis::exception();
                }

                output << to_string(project->GetInfoTotalLine()) + "\n";

                for (auto& infodata : project->infodatalist)
                {
                    output << infodata.uniquecode + "\n";
                    output << infodata.duration + "\n";
                    output << to_string(infodata.motiondata.size()) + "\n";

                    for (auto& motiondata : infodata.motiondata)
                    {
                        output << motiondata + "\n";
                    }

                    output << to_string(infodata.rotationdata.size()) + "\n";

                    for (auto& rotationdata : infodata.rotationdata)
                    {
                        output << rotationdata + "\n";
                    }

                    output << "\n";

                    if (error) throw nemesis::exception();
                }
            }

            if (error) throw nemesis::exception();
        }
    }
    else
    {
        ErrorMessage(1025, filepath);
    }

    DebugLogging("Processing behavior: " + filepath + " (Check point 5, AnimData output complete)");
    process->newMilestone();
}
