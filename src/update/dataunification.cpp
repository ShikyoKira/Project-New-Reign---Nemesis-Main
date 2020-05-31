/*
COMBINE NEW ANIMATION JOINT
OBJECTIVES:
- Add connection from template to existing node
- Avoid conflicting with other templates by adding full address as well as checking if same data has been changed
- Check for error prior to actual patching
*/

#include "update/dataunification.h"

#include "utilities/lastupdate.h"
#include "utilities/stringsplit.h"

#pragma warning(disable:4503)

using namespace std;

atomic_flag newAnimAdditionLock{};

void CombineAnimData(string filename, string characterfile, string modcode, string filepath, VecStr storeline, MasterAnimData& animData, bool isHeader);

bool newAnimUpdateExt(string folderpath, string modcode, string behaviorfile, map<string, VecStr, alphanum_less>& newFile, map<string, VecStr>& newAnimAddition,
                      unordered_map<wstring, wstring>& lastUpdate)
{
	VecStr nodelist;
	read_directory(folderpath + "\\" + behaviorfile, nodelist);

	for (unsigned int k = 0; k < nodelist.size(); ++k)
	{
		string path = folderpath + "\\" + behaviorfile + "\\" + nodelist[k];
		std::filesystem::path curfile(path);

		if (std::filesystem::is_directory(curfile)) continue;

		string filename = folderpath + "\\" + behaviorfile + "\\" + nodelist[k];
		VecStr storeline;

		saveLastUpdate(nemesis::to_lower_copy(filename), lastUpdate);

		if (!GetFunctionLines(filename, storeline)) return false;

		if (nodelist[k][0] != '#')
		{
			Lockless lock(newAnimAdditionLock);
			newAnimAddition[nemesis::to_lower_copy(curfile.string())] = storeline;
			continue;
		}

		VecStr newline;
		bool start = false;
		unsigned int row = 0;
		unsigned int scope = 0;

		for (auto& curline : storeline)
		{
			++row;

			if (curline.find("SERIALIZE_IGNORED") == NOT_FOUND)
			{
				if (curline.find("<!-- NEW", 0) != NOT_FOUND || curline.find("<!-- FOREACH", 0) != NOT_FOUND)
				{
					++scope;
				}
				else if (curline.find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					if (scope == 0) ErrorMessage(1171, modcode, path, row);

					--scope;
				}

				if (curline.find("			#") != NOT_FOUND && newline.back().find("numelements=\"", 0) != NOT_FOUND) start = true;
				else if (start && curline.find("</hkparam>") != NOT_FOUND) start = false;

				if (start)
				{
					if (curline.find("			#") != NOT_FOUND)
					{
						VecStr curElements;
						StringSplit(curline, curElements);

						for (auto& element : curElements)
						{
							newline.push_back("				" + element);
						}
					}
					else
					{
						newline.push_back(curline);
					}
				}
				else
				{
					newline.push_back(curline);
				}
			}
		}

		storeline = newline;

		string nodeID = nodelist[k].substr(0, nodelist[k].find_last_of("."));
		VecStr originallines = newFile[nodeID];

		if (originallines.size() == 0) ErrorMessage(1170, modcode, curfile.string());

		bool close = false;
		unordered_map<int, bool> conditionOpen;
		bool conditionOri = false;
		int linecount = 0;
		int conditionLvl = 0;
		scope = 0;
		VecStr newlines;
		VecStr combinelines;

		for (unsigned int i = 0; i < storeline.size(); ++i)
		{
			string& curline = storeline[i];

			if (curline.find("<!-- CONDITION START ") != NOT_FOUND)
			{
				++conditionLvl;
				conditionOpen[conditionLvl] = true;
			}

			if (!close)
			{
				if (curline.find("<!-- NEW", 0) == NOT_FOUND && curline.find("<!-- FOREACH", 0) == NOT_FOUND)
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
					else if (curline.find("<!-- ", 0) != NOT_FOUND && curline.find("numelements +", curline.find("<!-- ")) != NOT_FOUND)
					{
						size_t position = curline.find("<!-- ");
						string templine = curline.substr(position, curline.find("-->", position) - position + 3);

						if (originallines[linecount].find("<!-- ", 0) != NOT_FOUND && originallines[linecount].find("-->", 0) != NOT_FOUND) originallines[linecount].append(" " + templine);
						else originallines[linecount].append("			" + templine);
					}

					if (curline.find("<!-- CONDITION END -->") != NOT_FOUND)
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
			else if (curline.find("<!-- CLOSE -->", 0) != NOT_FOUND)
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

		if (combinelines.size() != 0 && combinelines.back().length() != 0) combinelines.push_back("");

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

	for (unsigned int k = 0; k < headerlist.size(); ++k)
	{
		string filepath = folderpath + "\\" + headerlist[k];
		std::filesystem::path curfile(filepath);

		if (std::filesystem::is_directory(curfile)) continue;

		string filename = curfile.stem().string();
		VecStr storeline;

		saveLastUpdate(nemesis::to_lower_copy(filepath), lastUpdate);

		if (!GetFunctionLines(filepath, storeline)) return false;

		if (filename[0] == '$' && (filename.back() == '$' || (filename.length() > 3 && filename.rfind("$UC") == filename.length() - 3)) && filename != "$header$")
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

bool newAnimDataSetUpdateExt(string folderpath, string modcode, string projectfile, MasterAnimSetData& animSetData, map<string, VecStr>& newAnimAddition,
                             unordered_map<wstring, wstring>& lastUpdate)
{
	if (animSetData.newAnimSetData.find(projectfile) == animSetData.newAnimSetData.end()) return true;

	VecStr datalist;
	VecStr headerfile;
	read_directory(folderpath, headerfile);

	for (unsigned int k = 0; k < headerfile.size(); ++k)
	{
		string filename = folderpath + "\\" + headerfile[k];
		std::filesystem::path curfile(filename);

		if (std::filesystem::is_directory(curfile) || !nemesis::iequals(curfile.extension().string(), ".txt") || headerfile[k].length() == 0) continue;

		saveLastUpdate(nemesis::to_lower_copy(filename), lastUpdate);

		VecStr storeline;
		string lowerheader = nemesis::to_lower_copy(curfile.stem().string());

		if (!GetFunctionLines(filename, storeline)) return false;

		if (headerfile[k][0] == '$')
		{
			Lockless lock(newAnimAdditionLock);
			newAnimAddition[nemesis::to_lower_copy(curfile.string())] = storeline;
			continue;
		}

		if (animSetData.newAnimSetData[projectfile].find(lowerheader) != animSetData.newAnimSetData[projectfile].end())
		{
			VecStr originallines = animSetData.newAnimSetData[projectfile][lowerheader];
			bool close = false;
			unordered_map<int, bool> conditionOpen;
			bool conditionOri = false;
			int linecount = 0;
			int conditionLvl = 0;
			VecStr newlines;
			VecStr combinelines;

			for (string& line : storeline)
			{
				// condition function is not supported for animationsetdatasinglefile
				if (line.find("<!-- CONDITION") != NOT_FOUND)
				{
					ErrorMessage(1173, filename, modcode, k + 1);
				}

				if (line.find("<!-- CONDITION START ") != NOT_FOUND)
				{
					++conditionLvl;
					conditionOpen[conditionLvl] = true;
				}

				if (line.find("<!-- NEW", 0) == NOT_FOUND && !close)
				{
					if (linecount < int(originallines.size()) && (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND ||
						originallines[linecount].find("<!-- FOREACH", 0) != NOT_FOUND))
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						combinelines.push_back(originallines[linecount]);
						++linecount;

						while (true)
						{
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								break;
							}

							combinelines.push_back(originallines[linecount]);
							++linecount;
						}
					}

					if (line.find("<!-- CONDITION END -->") != NOT_FOUND)
					{
						combinelines.push_back(line);
						conditionOri = false;
						conditionOpen[conditionLvl] = false;
						--conditionLvl;
					}
					else if (linecount < int(originallines.size()) && (conditionOri || !conditionOpen[conditionLvl]))
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
					}
					else
					{
						combinelines.push_back(line);
					}
				}
				else if (close && line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					if (linecount < int(originallines.size()) && (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND
						|| originallines[linecount].find("<!-- FOREACH", 0) != NOT_FOUND))
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						combinelines.push_back(originallines[linecount]);
						++linecount;

						while (true)
						{
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								break;
							}

							combinelines.push_back(originallines[linecount]);
							++linecount;
						}
					}

					combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
					combinelines.push_back(line);
					newlines.clear();
					close = false;
				}
				else
				{
					close = true;
				}

				if (line.find("<!-- CONDITION -->") != NOT_FOUND) conditionOri = true;

				if (error) throw nemesis::exception();

				if (close) newlines.push_back(line);
			}

			animSetData.newAnimSetData[projectfile][lowerheader] = combinelines;
		}
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

			while (!behaviorJoints[temp[i].data()].empty())
			{
				temp = behaviorJoints[temp[i].data()];
			}

			combinedBehaviorJoints[it->first] = temp;
			break;
		}

		if (error) throw nemesis::exception();
	}

	FileWriter output(L"cache\\behavior_joints");

	if (output.is_open())
	{
		for (auto it : combinedBehaviorJoints)
		{
			output << it.first << "\n";

			for (unsigned int i = 0; i < it.second.size(); ++i)
			{
				output << it.second[i] << "\n";
			}

			output << "\n";
		}

		if (error) throw nemesis::exception();
	}
	else
	{
		ErrorMessage(2009, "behavior_joint");
	}
}

void CombineAnimData(string filename, string characterfile, string modcode, string filepath, VecStr storeline, MasterAnimData& animData, bool isHeader)
{
	if (storeline.back().length() != 0)
	{
		storeline.push_back("");
	}

	int linecount = 0;
	int conditionLvl = 0;
	int type;
	int start = 0;

	if (!isHeader)
	{
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
