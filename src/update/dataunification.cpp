/*
COMBINE NEW ANIMATION JOINT
OBJECTIVES:
- Add connection from template to existing node
- Avoid conflicting with other templates by adding full address as well as checking if same data has been changed
- Check for error prior to actual patching
*/

#include "update/dataunification.h"

#include "update/animsetdata/masteranimsetdata.h"

#include "utilities/lastupdate.h"
#include "utilities/stringextension.h"
#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"
#include "utilities/hkxelementsplit.h"

#pragma warning(disable:4503)

using namespace std;
namespace ns = nemesis::syntax;

atomic_flag newAnimAdditionLock{};

void CombineAnimData(string filename, string characterfile, string modcode, string filepath, VecStr storeline, MasterAnimData& animData, bool isHeader);

bool newAnimUpdateExt(string folderpath, string modcode, string behaviorfile, map<string, VecStr, alphanum_less>& newFile, map<string, VecStr>& newAnimAddition,
                      unordered_map<wstring, wstring>& lastUpdate)
{
	VecStr nodelist;
    string dir = folderpath + "\\" + behaviorfile;
    read_directory(dir, nodelist);

	for (auto& node : nodelist)
	{
        string path = dir + "\\" + node;
        std::filesystem::path curfile(path);

		if (std::filesystem::is_directory(curfile)) continue;

		const string& filename = path;
		VecStr storeline;

		saveLastUpdate(nemesis::to_lower_copy(filename), lastUpdate);

		if (!GetFunctionLines(filename, storeline)) return false;

		if (node[0] != '#')
		{
			Lockless lock(newAnimAdditionLock);
			newAnimAddition[nemesis::to_lower_copy(curfile.string())] = storeline;
			continue;
		}

        nemesis::ConditionScope cscope(modcode, curfile);
        nemesis::HkxElementSplit hesplit;
		bool start = false;
		unsigned int row = 0;
        unsigned int ifscope = 0;
        unsigned int foreachscope = 0;

		for (int i = 0; i < storeline.size(); ++i)
		{
            auto& curline = storeline[i];

			if (curline.find("SERIALIZE_IGNORED") != NOT_FOUND)
            {
                storeline.erase(storeline.begin() + i);
                continue;
            }

            nemesis::Line nline(curline);
            auto conditioninfo = cscope.TryGetConditionInfo(nline);

			if (conditioninfo || !cscope.Empty()) continue;

            hesplit.SplitValidation(nline);

			if (!hesplit.IsStart()) continue;

			storeline.erase(storeline.begin() + i--);

            for (auto& each : StringSplit(nline, '\n'))
            {
                storeline.insert(storeline.begin() + ++i, each);
            }
		}

		string nodeID        = node.substr(0, node.find_last_of("."));
		VecStr originallines = newFile[nodeID];

		if (originallines.empty()) ErrorMessage(1170, modcode, curfile.string());

		bool close = false;
		unordered_map<int, bool> conditionOpen;
		bool conditionOri = false;
		int linecount = 0;
		int conditionLvl = 0;
		int scope = 0;
		VecStr newlines;
		VecStr combinelines;

		for (unsigned int i = 0; i < storeline.size(); ++i)
		{
			string& curline = storeline[i];

			if (curline.find(ns::If()) != NOT_FOUND)
			{
				++conditionLvl;
				conditionOpen[conditionLvl] = true;
			}

			if (!close)
			{
				if (curline.find(ns::If()) == NOT_FOUND && curline.find(ns::ForEach(), 0) == NOT_FOUND)
				{
					if (originallines[linecount].find(ns::If(), 0) != NOT_FOUND || originallines[linecount].find(ns::ForEach(), 0) != NOT_FOUND)
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						size_t oriScope = 1;

						while (oriScope != 0)
						{
							if (originallines[linecount].find(ns::If()) != NOT_FOUND || originallines[linecount].find(ns::ForEach()) != NOT_FOUND) ++oriScope;
							if (originallines[linecount].find(ns::Close()) != NOT_FOUND) --oriScope;

							combinelines.push_back(originallines[linecount]);
							++linecount;

							if (linecount == originallines.size() && oriScope != 0) ErrorMessage(1114, modcode, curfile.string(), i + 1);
						}
					}
                    else if (curline.find(ns::OpenComment()) != NOT_FOUND
                             && curline.find("numelements +", curline.find(ns::OpenComment())) != NOT_FOUND)
					{
						size_t position = curline.find("<!-- ");
						string templine = curline.substr(position, curline.find("-->", position) - position + 3);

						if (originallines[linecount].find(ns::OpenComment(), 0) != NOT_FOUND
                            && originallines[linecount].find(ns::CloseComment()) != NOT_FOUND)
                            originallines[linecount].append(" " + templine);
						else originallines[linecount].append("			" + templine);
					}

					if (curline.find(ns::EndIf()) != NOT_FOUND)
					{
						combinelines.push_back(curline);
						conditionOri = false;
						conditionOpen[conditionLvl] = false;
						--conditionLvl;
					}
					else if (conditionOri || !conditionOpen[conditionLvl])
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
					}
					else
					{
						combinelines.push_back(curline);
					}
				}
				else
				{
					++scope;
					close = true;
				}
			}
			else if (curline.find(ns::Close()) != NOT_FOUND)
			{
				--scope;

				if (scope == 0)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND || originallines[linecount].find("<!-- FOREACH", 0) != NOT_FOUND)
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						size_t oriScope = 1;

						while (oriScope != 0)
						{
							if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND || originallines[linecount].find("<!-- FOREACH", 0) != NOT_FOUND) ++oriScope;
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND) --oriScope;

							combinelines.push_back(originallines[linecount]);
							++linecount;

							if (linecount == originallines.size() && oriScope != 0) ErrorMessage(1114, modcode, curfile.string(), i + 1);
						}
					}

					combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
					combinelines.push_back(curline);
					newlines.clear();
					close = false;
				}
			}
			else if (curline.find("<!-- NEW", 0) != NOT_FOUND || curline.find("<!-- FOREACH", 0) != NOT_FOUND)
			{
				++scope;
			}

			if (curline.find("<!-- CONDITION -->") != NOT_FOUND) conditionOri = true;

			if (error) throw nemesis::exception();

			if (close) newlines.push_back(curline);
		}

		if (scope != 0) ErrorMessage(1114, modcode, curfile.string(), storeline.size());

		if (!combinelines.empty() && combinelines.back().length() != 0) combinelines.emplace_back("");

		newFile[nodeID] = combinelines;
	}

	return true;
}

bool animDataHeaderUpdate(string folderpath,
                          string modcode,
                          MasterAnimData& animData,
                          unordered_map<wstring, wstring>& lastUpdate)
{
	VecStr storeline;

	if (!GetFunctionLines(folderpath, storeline)) return false;

	saveLastUpdate(nemesis::to_lower_copy(folderpath), lastUpdate);

	CombineAnimData(folderpath, "$haeder$", modcode, GetFileDirectory(folderpath) + "\\$header$", storeline, animData, true);

	if (error) throw nemesis::exception();

	return true;
}

bool newAnimDataUpdateExt(string folderpath, string modcode, string characterfile, MasterAnimData& animData, map<string, VecStr>& newAnimAddition,
	unordered_map<wstring, wstring>& lastUpdate)
{
	VecStr headerlist;
	read_directory(folderpath, headerlist);

	for (auto& header : headerlist)
	{
        string filepath = folderpath + "\\" + header;
		std::filesystem::path curfile(filepath);

		if (std::filesystem::is_directory(curfile)) continue;

		string filename = curfile.stem().string();
		VecStr storeline;

		saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate);

		if (!GetFunctionLines(filepath, storeline)) return false;

		if (filename[0] == '$'
            && (filename.back() == '$'
                || (filename.length() > 3 && filename.rfind("$UC") == filename.length() - 3))
            && filename != "$header$")
		{
			Lockless lock(newAnimAdditionLock);
			newAnimAddition[nemesis::to_lower_copy(curfile.string())] = storeline;
			continue;
		}

		bool isinfo = false;
        string tempname
            = nemesis::regex_replace(string(filename), nemesis::regex("[^~]*~([0-9]+)"), string("\\1"));

		if (tempname == filename)
		{
			// possibly info data
			if (isOnlyNumber(tempname)) isinfo = true;
		}
		// possibly anim data
		else if (!isOnlyNumber(tempname) || !hasAlpha(filename.substr(0, filename.find("~"))))
		{
			ErrorMessage(2004, filepath);
		}

		bool isNew = false;
		bool isHeader = false;
		string project;

		if (characterfile.find("~") != NOT_FOUND)
		{
			//check if project/characterfile has "~<num>" or not
            string tempproject
                = nemesis::regex_replace(string(characterfile), nemesis::regex("~([0-9]+)"), string("\\1"));
			project = (tempproject == characterfile || !isOnlyNumber(tempproject)) ? project + ".txt~1" : characterfile.replace(characterfile.find_last_of("~"), 0, ".txt");
		}
		else if (characterfile != "$header$")
		{
			project = characterfile + ".txt~1";
		}
		else
		{
			ErrorMessage(3022, filepath);
		}

		if (error) throw nemesis::exception();
	}

	return true;
}

bool newAnimDataSetUpdateExt(string folderpath, string templatecode, string projectfile, MasterAnimSetData& animSetData, map<string, VecStr>& newAnimAddition,
                             unordered_map<wstring, wstring>& lastUpdate)
{
	VecStr datalist;
    VecStr headerfiles;
    read_directory(folderpath, headerfiles);

	for (auto& file : headerfiles)
	{
        string filename = folderpath + "\\" + file;
		std::filesystem::path filepath(filename);

        if (std::filesystem::is_directory(filepath)) continue;

		if (!nemesis::iequals(filepath.extension().string(), ".txt")) continue;

		saveLastUpdate(nemesis::to_lower_copy(filename), lastUpdate);

		VecNstr storeline;
        string lowerheader = nemesis::to_lower_copy(filepath.stem().string());

		if (!GetFunctionLines(filename, storeline)) return false;

        size_t num       = 2;
        string lowername = nemesis::to_lower_copy(filepath.stem().string());
        auto& masterProj = animSetData.find(projectfile, templatecode);

        if (lowername.length() > 2 && lowername.front() == '$' && lowername.back() == '$') continue;

        masterProj->find(lowername + filepath.extension().string())
            ->ImportDataTemplate(filepath, num, storeline, templatecode);
	}

	return true;
}

void behaviorJointsOutput()
{
	unordered_map<string, VecStr> combinedBehaviorJoints;

	for (auto it = behaviorJoints.begin(); it != behaviorJoints.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			VecStr temp = it->second;

			while (!behaviorJoints[temp[i]].empty())
			{
				temp = behaviorJoints[temp[i]];
			}

			combinedBehaviorJoints[it->first] = temp;
			break;
		}

		if (error) throw nemesis::exception();
	}

	FileWriter output(L"cache\\behavior_joints");

	if (!output.is_open())
    {
        ErrorMessage(2009, "behavior_joint");
	}

    for (auto& it : combinedBehaviorJoints)
    {
        output << it.first << "\n";

        for (auto& i : it.second)
        {
            output << i << "\n";
        }

        output << "\n";
    }

    if (error) throw nemesis::exception();
}

void CombineAnimData(string filename, string characterfile, string modcode, string filepath, VecStr storeline, MasterAnimData& animData, bool isHeader)
{
	if (storeline.back().length() != 0)
	{
        storeline.emplace_back("");
	}

	int linecount = 0;
	int conditionLvl = 0;
	int type;
	int start = 0;

	if (!isHeader)
	{
		// continue here

		if (filename == "$header$")
		{

		}
		else if (hasAlpha(storeline[0]))
		{

		}
		else if (isOnlyNumber(storeline[0]))
		{

		}
		else
		{
			ErrorMessage(3006, characterfile, filename);
		}
	}
	else
	{
		start = 1;
		linecount = 1;
	}
}
