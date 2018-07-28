#include "dataunification.h"

#pragma warning(disable:4503)

using namespace std;

bool newAnimUpdateExt(string folderpath, string modcode, string behaviorfile, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, string>& lastUpdate)
{
	vecstr nodelist;
	read_directory(folderpath + "\\" + behaviorfile, nodelist);

	for (unsigned int k = 0; k < nodelist.size(); ++k)
	{
		boost::filesystem::path curfile(folderpath + "\\" + behaviorfile + "\\" + nodelist[k]);

		if (nodelist[k][0] == '#' && !boost::filesystem::is_directory(curfile))
		{
			string line;
			string filename = folderpath + "\\" + behaviorfile + "\\" + nodelist[k];

			saveLastUpdate(filename, lastUpdate);

			vecstr storeline;
			GetFunctionLines(filename, storeline);

			if (error)
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

bool newAnimDataUpdateExt(string folderpath, string modcode, string characterfile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, set<string>>& oriADH, unordered_map<string, string>& lastUpdate)
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
			stringstream sstream(filename);
			istream_iterator<string> ssbegin(sstream);
			istream_iterator<string> ssend;
			vecstr fileparts(ssbegin, ssend);
			copy(fileparts.begin(), fileparts.end(), fileparts.begin());

			while (true)
			{
				if (fileparts.size() < 3 && fileparts.size() != 0)
				{
					if (fileparts.size() == 1 && !isOnlyNumber(fileparts[0]))
					{
						if (fileparts[0][0] == '$' && (fileparts[0].back() == '$' || fileparts[0].find_last_of("$UC") == fileparts[0].length() - 1))
						{
							break;
						}
						else
						{
							ErrorMessage(2004, filepath);
							return false;
						}
					}
					else if (fileparts.size() == 2 && (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
					{
						ErrorMessage(2004, filepath);
						return false;
					}

					string line;
					vecstr storeline;
					bool isNew = false;

					if (newAnimData.find(characterfile) == newAnimData.end())
					{
						animDataChar.push_back(characterfile);
						isNew = true;
					}

					if (oriADH.find(characterfile) != oriADH.end())
					{
						if (oriADH[characterfile].find(filename) != oriADH[characterfile].end())
						{
							saveLastUpdate(filepath, lastUpdate);
						}
					}

					GetFunctionLines(filepath, storeline);

					if (error)
					{
						return false;
					}

					if (newAnimData[characterfile].find(filename) == newAnimData[characterfile].end())
					{
						if (fileparts.size() > 1)		// type 1
						{
							if (!isNew && animDataHeader[characterfile].size() > 0)
							{
								vecstr tempheaders;
								size_t infoline = animDataHeader[characterfile].size();
								tempheaders.reserve(infoline);

								for (unsigned int i = 0; i < infoline; ++i)
								{
									if (boost::iequals(animDataHeader[characterfile][i], "$info header$") || animDataHeader[characterfile][i].find(" ") == NOT_FOUND)
									{
										infoline = i;
										tempheaders.push_back(filename);
										break;
									}

									tempheaders.push_back(animDataHeader[characterfile][i]);
								}

								for (unsigned int i = infoline; i < animDataHeader[characterfile].size(); ++i)
								{
									tempheaders.push_back(animDataHeader[characterfile][i]);
								}

								animDataHeader[characterfile] = tempheaders;
							}
							else
							{
								animDataHeader[characterfile].push_back(filename);
							}
						}
						else		// type 2
						{
							if (!boost::iequals(filename, "$info header$"))
							{
								if (newAnimData[characterfile].find("$info header$") == newAnimData[characterfile].end())
								{
									if (!isFileExist(folderpath + "\\$info header$.txt"))
									{
										ErrorMessage(3012, filepath);
										return false;
									}
								}

								animDataHeader[characterfile].push_back(filename);
							}
							else
							{
								vecstr tempheaders;
								size_t infoline = animDataHeader[characterfile].size();
								tempheaders.reserve(infoline);

								for (unsigned int i = 0; i < infoline; ++i)
								{
									if (animDataHeader[characterfile][i].find(" ") == NOT_FOUND)
									{
										infoline = i;
										tempheaders.push_back(filename);
										break;
									}

									tempheaders.push_back(animDataHeader[characterfile][i]);
								}

								for (unsigned int i = infoline; i < animDataHeader[characterfile].size(); ++i)
								{
									tempheaders.push_back(animDataHeader[characterfile][i]);
								}

								animDataHeader[characterfile] = tempheaders;
							}
						}

						isNew = true;
					}

					if (!isNew)
					{
						vecstr originallines = newAnimData[characterfile][filename];

						if (storeline.back().length() != 0 && originallines.back().length() == 0)
						{
							storeline.push_back("");
						}

						bool isHeader = false;
						bool close = false;
						bool conditionOri = false;

						unordered_map<int, bool> conditionOpen;

						int linecount = 0;
						int conditionLvl = 0;
						int type;

						vecstr newlines;
						vecstr combinelines;

						combinelines.reserve(storeline.size() + originallines.size());

						if (boost::iequals(characterfile, "$header$"))
						{
							isHeader = true;
						}

						if (filename == "$header$" || filename == "$info header$")
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
							ErrorMessage(3006, characterfile, fileparts[0]);
							return false;
						}

						for (unsigned int k = 0; k < storeline.size(); ++k)
						{
							// condition function is not supported for animationsetdatasinglefile
							if (storeline[k].find("<!-- CONDITION") != NOT_FOUND)
							{
								ErrorMessage(1173, "animationsetdatasinglefile.txt", modcode, k + 1);
								return false;
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
										AD::position position = AnimDataPosition(storeline, characterfile, filename, modcode, linecount, type);

										if (error)
										{
											return false;
										}

										if (position != AD::behaviorfilelist && position != AD::eventnamelist && position != AD::motiondatalist && position != AD::rotationdatalist)
										{
											ErrorMessage(3018, modcode, characterfile, filename);
											return false;
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

						// for $header$
						if (isHeader && newlines.size() > 0)
						{
							combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
							newlines.clear();
						}

						combinelines.shrink_to_fit();
						newAnimData[characterfile][filename] = combinelines;
					}
					else
					{
						if (fileparts[0][0] == '$' && (fileparts[0].back() == '$' || fileparts[0].find_last_of("$UC") == fileparts[0].length() - 1))
						{
							ErrorMessage(2004, filepath);
							return false;
						}
					}
				}

				break;
			}
		}
	}

	return true;
}

bool newAnimDataSetUpdateExt(string folderpath, string modcode, string projectfile, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, unordered_map<string, set<string>>& oriASDH, unordered_map<string, string>& lastUpdate)
{
	vecstr datalist;
	vecstr headerfile;
	read_directory(folderpath, headerfile);
	string project = projectfile + "\\" + projectfile.substr(0, wordFind(projectfile, "data", true)) + ".txt";

	if (newAnimDataSet.find(project) == newAnimDataSet.end())
	{
		return true;
	}

	for (unsigned int k = 0; k < headerfile.size(); ++k)
	{
		string filename = folderpath + "\\" + headerfile[k];
		string lowerheader = boost::to_lower_copy(headerfile[k]);
		boost::filesystem::path curfile(filename);

		if (!boost::filesystem::is_directory(curfile) && curfile.stem().string() == "txt")
		{
			if (oriASDH.find(project) != oriASDH.end())
			{
				if (oriASDH[project].find(lowerheader) != oriASDH[project].end())
				{
					saveLastUpdate(filename, lastUpdate);
				}
			}

			vecstr storeline;
			GetFunctionLines(filename, storeline);

			if (error)
			{
				return false;
			}

			if (newAnimDataSet[project].find(lowerheader) != newAnimDataSet[project].end())
			{
				vecstr originallines = newAnimDataSet[project][lowerheader];
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
						ErrorMessage(1173, "animationsetdatasinglefile.txt", modcode, k + 1);
						return false;
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

				newAnimDataSet[project][lowerheader] = combinelines;
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
