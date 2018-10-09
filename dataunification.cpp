#include "dataunification.h"
#include "lastupdate.h"

#pragma warning(disable:4503)

using namespace std;

void CombineAnimData(string filename, string characterfile, string modcode, string filepath, vecstr storeline, vecstr originallines, MasterAnimData& animData, bool isHeader);

bool newAnimUpdateExt(string folderpath, string modcode, string behaviorfile, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, string>& lastUpdate)
{
	vecstr nodelist;
	read_directory(folderpath + "\\" + behaviorfile, nodelist);

	for (unsigned int k = 0; k < nodelist.size(); ++k)
	{
		string path = folderpath + "\\" + behaviorfile + "\\" + nodelist[k];
		boost::filesystem::path curfile(path);

		if (nodelist[k][0] == '#' && !boost::filesystem::is_directory(curfile))
		{
			string line;
			string filename = folderpath + "\\" + behaviorfile + "\\" + nodelist[k];

			if (!saveLastUpdate(boost::to_lower_copy(filename), lastUpdate))
			{
				return false;
			}

			vecstr storeline;

			if (!GetFunctionLines(filename, storeline))
			{
				return false;
			}

			{
				vecstr newline;
				bool start = false;

				for (auto& curline : storeline)
				{
					if (curline.find("SERIALIZE_IGNORED") == NOT_FOUND)
					{
						if (curline.find("			#") != NOT_FOUND && newline.back().find("numelements=\"", 0) != NOT_FOUND)
						{
							start = true;
						}
						else if (start && curline.find("</hkparam>") != NOT_FOUND)
						{
							start = false;
						}

						if (start)
						{
							if (curline.find("			#") != NOT_FOUND)
							{
								stringstream sstream(curline);
								istream_iterator<string> ssbegin(sstream);
								istream_iterator<string> ssend;
								vector<string> curElements(ssbegin, ssend);
								copy(curElements.begin(), curElements.end(), curElements.begin());

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
			}

			string nodeID = nodelist[k].substr(0, nodelist[k].find_last_of("."));
			vecstr originallines = newFile[behaviorfile][nodeID];

			if (error)
			{
				return false;
			}

			if (originallines.size() == 0)
			{
				ErrorMessage(1170, modcode, curfile.string());
				return false;
			}

			bool close = false;
			unordered_map<int, bool> conditionOpen;
			bool conditionOri = false;
			int linecount = 0;
			int conditionLvl = 0;
			vecstr newlines;
			vecstr combinelines;

			for (unsigned int k = 0; k < storeline.size(); ++k)
			{
				if (storeline[k].find("<!-- CONDITION START ") != NOT_FOUND)
				{
					++conditionLvl;
					conditionOpen[conditionLvl] = true;
				}

				if (storeline[k].find("<!-- NEW", 0) == NOT_FOUND && !close)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
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
							else
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
							}
						}
					}
					else if (storeline[k].find("<!-- ", 0) != NOT_FOUND && storeline[k].find("numelements +", storeline[k].find("<!-- ")) != NOT_FOUND)
					{
						size_t position = storeline[k].find("<!-- ");
						string templine = storeline[k].substr(position, storeline[k].find("-->", position) - position + 3);

						if (originallines[linecount].find("<!-- ", 0) != NOT_FOUND && originallines[linecount].find("-->", 0) != NOT_FOUND)
						{
							originallines[linecount].append(" " + templine);
						}
						else
						{
							originallines[linecount].append("			" + templine);
						}
					}

					if (storeline[k].find("<!-- CONDITION END -->") != NOT_FOUND)
					{
						combinelines.push_back(storeline[k]);
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
						combinelines.push_back(storeline[k]);
					}
				}
				else if (close && storeline[k].find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
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
							else
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
							}
						}
					}

					combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
					combinelines.push_back(storeline[k]);
					newlines.clear();
					close = false;
				}
				else
				{
					close = true;
				}

				if (storeline[k].find("<!-- CONDITION -->") != NOT_FOUND)
				{
					conditionOri = true;
				}

				if (close)
				{
					newlines.push_back(storeline[k]);
				}
			}

			if (combinelines.size() != 0 && combinelines.back().length() != 0)
			{
				combinelines.push_back("");
			}

			newFile[behaviorfile][nodeID] = combinelines;
		}
	}

	return true;
}

bool animDataHeaderUpdate(string folderpath, string modcode, MasterAnimData& animData, unordered_map<string, string>& lastUpdate)
{
	vecstr storeline;

	if (!GetFunctionLines(folderpath, storeline))
	{
		return false;
	}

	if (!saveLastUpdate(boost::to_lower_copy(folderpath), lastUpdate))
	{
		return false;
	}

	CombineAnimData(folderpath, "$haeder$", modcode, GetFileDirectory(folderpath) + "\\$header$", storeline, animData.animDataChar, animData, true);

	if (error)
	{
		return false;
	}

	return true;
}

bool newAnimDataUpdateExt(string folderpath, string modcode, string characterfile, MasterAnimData& animData, unordered_map<string, string>& lastUpdate)
{
	vecstr headerlist;
	read_directory(folderpath, headerlist);

	for (unsigned int k = 0; k < headerlist.size(); ++k)
	{
		string filepath = folderpath + "\\" + headerlist[k];
		boost::filesystem::path curfile(filepath);

		if (!boost::filesystem::is_directory(curfile))
		{
			string filename = curfile.stem().string();

			while (true)
			{
				bool isinfo = false;

				if (filename[0] == '$' && (filename.back() == '$' || filename.find_last_of("$UC") == filename.length() - 1) && filename != "$header$")
				{
					break;
				}
				else
				{
					string tempname = boost::regex_replace(string(filename), boost::regex("[^~]*~([0-9]+)"), string("\\1"));

					if (tempname == filename)
					{
						// possibly info data
						if (isOnlyNumber(tempname))
						{
							isinfo = true;
						}
					}
					else
					{
						// possibly anim data
						if (!isOnlyNumber(tempname) || !hasAlpha(filename.substr(0, filename.find("~"))))
						{
							ErrorMessage(2004, filepath);
							return false;
						}
					}
				}

				string line;
				vecstr storeline;
				bool isNew = false;
				bool isHeader = false;
				string project;

				if (characterfile.find("~") != NOT_FOUND)
				{
					//check if project/characterfile has "~<num>" or not
					string tempproject = boost::regex_replace(string(characterfile), boost::regex("~([0-9]+)"), string("\\1"));

					if (tempproject == characterfile || !isOnlyNumber(tempproject))
					{
						project = project + ".txt~1";
					}
					else
					{
						project = characterfile.replace(characterfile.find_last_of("~"), 0, ".txt");
					}
				}
				else if (characterfile != "$header$")
				{
					project = characterfile + ".txt~1";
				}
				else
				{
					ErrorMessage(3022, filepath);
				}

				if (animData.newAnimData.find(project) == animData.newAnimData.end())
				{
					animData.animDataChar.push_back(project);
					isNew = true;
				}

				if (!saveLastUpdate(boost::to_lower_copy(filepath), lastUpdate))
				{
					return false;
				}

				if (!GetFunctionLines(filepath, storeline))
				{
					return false;
				}

				if (animData.newAnimData[project].find(filename) == animData.newAnimData[project].end())
				{
					if (!isinfo)		// anim data
					{
						animData.animDataHeader[project].push_back(filename);
					}
					else		// info data
					{
						animData.animDataInfo[project].push_back(filename);
					}

					isNew = true;
				}

				if (!isNew)
				{
					CombineAnimData(filename, characterfile, modcode, filepath, storeline, animData.newAnimData[project][filename], animData, false);
					
					if (error)
					{
						return false;
					}
				}
				else
				{
					if (filename[0] == '$' && (filename.back() == '$' || filename.find_last_of("$UC") == filename.length() - 1))
					{
						ErrorMessage(2004, filepath);
						return false;
					}
				}

				break;
			}
		}
	}

	return true;
}

bool newAnimDataSetUpdateExt(string folderpath, string modcode, string projectfile, MasterAnimSetData& animSetData, unordered_map<string, string>& lastUpdate)
{
	if (animSetData.newAnimSetData.find(projectfile) == animSetData.newAnimSetData.end())
	{
		return true;
	}

	vecstr datalist;
	vecstr headerfile;
	read_directory(folderpath, headerfile);

	for (unsigned int k = 0; k < headerfile.size(); ++k)
	{
		string filename = folderpath + "\\" + headerfile[k];
		boost::filesystem::path curfile(filename);

		if (!boost::filesystem::is_directory(curfile) && curfile.extension().string() == ".txt" && headerfile[k].length() > 0 && headerfile[k][0] != '$')
		{
			vecstr storeline;
			string lowerheader = boost::to_lower_copy(curfile.stem().string());

			if (!GetFunctionLines(filename, storeline))
			{
				return false;
			}

			if (!saveLastUpdate(boost::to_lower_copy(filename), lastUpdate))
			{
				return false;
			}

			if (animSetData.newAnimSetData[projectfile].find(lowerheader) != animSetData.newAnimSetData[projectfile].end())
			{
				vecstr originallines = animSetData.newAnimSetData[projectfile][lowerheader];
				bool close = false;
				unordered_map<int, bool> conditionOpen;
				bool conditionOri = false;
				int linecount = 0;
				int conditionLvl = 0;
				vecstr newlines;
				vecstr combinelines;

				for (unsigned int k = 0; k < storeline.size(); ++k)
				{
					// condition function is not supported for animationsetdatasinglefile
					if (storeline[k].find("<!-- CONDITION") != NOT_FOUND)
					{
						ErrorMessage(1173, filename, modcode, k + 1);
						return false;
					}

					if (storeline[k].find("<!-- CONDITION START ") != NOT_FOUND)
					{
						++conditionLvl;
						conditionOpen[conditionLvl] = true;
					}

					if (storeline[k].find("<!-- NEW", 0) == NOT_FOUND && !close)
					{
						if (linecount < int(originallines.size()) && originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
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
								else
								{
									combinelines.push_back(originallines[linecount]);
									++linecount;
								}
							}
						}

						if (storeline[k].find("<!-- CONDITION END -->") != NOT_FOUND)
						{
							combinelines.push_back(storeline[k]);
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
							combinelines.push_back(storeline[k]);
						}
					}
					else if (close && storeline[k].find("<!-- CLOSE -->", 0) != NOT_FOUND)
					{
						if (linecount < int(originallines.size()) && originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
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
								else
								{
									combinelines.push_back(originallines[linecount]);
									++linecount;
								}
							}
						}

						combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
						combinelines.push_back(storeline[k]);
						newlines.clear();
						close = false;
					}
					else
					{
						close = true;
					}

					if (storeline[k].find("<!-- CONDITION -->") != NOT_FOUND)
					{
						conditionOri = true;
					}

					if (close)
					{
						newlines.push_back(storeline[k]);
					}
				}

				animSetData.newAnimSetData[projectfile][lowerheader] = combinelines;
			}
		}
	}

	return true;
}

void behaviorJointsOutput()
{
	unordered_map<string, vector<string>> combinedBehaviorJoints;

	for (auto it = behaviorJoints.begin(); it != behaviorJoints.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			vecstr temp = it->second;

			while (!behaviorJoints[temp[i]].empty())
			{
				temp = behaviorJoints[temp[i]];
			}

			combinedBehaviorJoints[it->first] = temp;
			break;
		}
	}

	ofstream output("behavior_joints");

	if (output.is_open())
	{
		FunctionWriter fwriter(&output);

		for (auto it : combinedBehaviorJoints)
		{
			fwriter << it.first << "\n";

			for (unsigned int i = 0; i < it.second.size(); ++i)
			{
				fwriter << it.second[i] << "\n";
			}

			fwriter << "\n";
		}

		output.close();
	}
	else
	{
		ErrorMessage(2009, "behavior_joint");
		return;
	}
}

void CombineAnimData(string filename, string characterfile, string modcode, string filepath, vecstr storeline, vecstr originallines, MasterAnimData& animData, bool isHeader)
{
	if (storeline.back().length() != 0 && originallines.back().length() == 0)
	{
		storeline.push_back("");
	}

	bool close = false;
	bool conditionOri = false;

	unordered_map<int, bool> conditionOpen;

	int linecount = 0;
	int conditionLvl = 0;
	int type;
	int start = 0;

	vecstr newlines;
	vecstr combinelines;

	combinelines.reserve(storeline.size() + originallines.size());

	if (!isHeader)
	{
		if (filename == "$header$")
		{
			type = 0;
		}
		else if (hasAlpha(storeline[0]))
		{
			type = 1;
		}
		else if (isOnlyNumber(storeline[0]))
		{
			type = 2;
		}
		else
		{
			ErrorMessage(3006, characterfile, filename);
			return;
		}
	}
	else
	{
		start = 1;
		linecount = 1;
	}

	for (unsigned int k = start; k < storeline.size(); ++k)
	{
		// condition function is not supported for animationsetdatasinglefile
		if (storeline[k].find("<!-- CONDITION") != NOT_FOUND)
		{
			ErrorMessage(1173, filepath, modcode, k + 1);
			return;
		}

		if (storeline[k].find("<!-- CONDITION START ") != NOT_FOUND)
		{
			++conditionLvl;
			conditionOpen[conditionLvl] = true;
		}

		if (storeline[k].find("<!-- NEW", 0) == NOT_FOUND && !close)
		{
			if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
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
					else
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
					}
				}
			}

			if (storeline[k].find("<!-- CONDITION END -->") != NOT_FOUND)
			{
				combinelines.push_back(storeline[k]);
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
				combinelines.push_back(storeline[k]);
			}
		}
		else if (close && storeline[k].find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
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
					else
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
					}
				}
			}

			if (isHeader)
			{
				newlines.push_back(storeline[k]);
			}
			else
			{
				size_t startline = combinelines.size() + 1;
				combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
				combinelines.push_back(storeline[k]);
				newlines.clear();

				for (unsigned int j = startline; j < combinelines.size() - 1; ++j)
				{
					namespace AD = AnimDataFormat;
					AD::position position = AnimDataPosition(storeline, characterfile, filename, modcode, filepath, linecount, type);

					if (error)
					{
						return;
					}

					if (position != AD::behaviorfilelist && position != AD::eventnamelist && position != AD::motiondatalist && position != AD::rotationdatalist)
					{
						ErrorMessage(3018, modcode, characterfile, filename);
						return;
					}
				}
			}

			close = false;
		}
		else
		{
			close = true;
		}

		if (storeline[k].find("<!-- CONDITION -->") != NOT_FOUND)
		{
			conditionOri = true;
		}

		if (close)
		{
			newlines.push_back(storeline[k]);
		}
	}

	combinelines.shrink_to_fit();
	originallines = combinelines;
}
