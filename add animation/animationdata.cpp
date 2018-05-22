#include "animationdata.h"

#pragma warning(disable:4503)

using namespace std;

AnimDataProject::AnimDataProject(vecstr animdatafile, string project, string filepath, string modcode)
{
	int startline = 0;
	BehaviorListProcess(*this, startline, animdatafile, project, modcode);

	if (startline >= animdatafile.size())
	{
		return;
	}

	AnimDataProcess(animdatalist, startline, animdatafile, project, modcode);

	if (startline >= animdatafile.size())
	{
		return;
	}

	InfoDataProcess(infodatalist, startline, animdatafile, project, modcode);
}

void BehaviorListProcess(AnimDataProject& storeline, int& startline, vecstr& animdatafile, string project, string modcode)
{
	if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(3005, project, "Header");
		throw 1;
	}

	int i = startline + 1;

	if (i + 4 >= animdatafile.size())
	{
		ErrorMessage(3021, project);
		throw 1;
	}

	storeline.unknown1 = animdatafile[i++];
	++i;

	while (!isOnlyNumber(animdatafile[i]))
	{
		if (hasAlpha(animdatafile[i]))
		{
			storeline.behaviorlist.push_back(animdatafile[i++]);
		}
		else
		{
			ErrorMessage(3005, project, "Header");
			throw 1;
		}
	}

	storeline.unknown2 = animdatafile[i++];
	startline = i;

	if (i < animdatafile.size() && !hasAlpha(animdatafile[i]))
	{
		ErrorMessage(3005, project, "Header");
		throw 1;
	}
}

void AnimDataProcess(vector<AnimDataPack>& storeline, int& startline, vecstr& animdatafile, string project, string modcode)
{
	for (int i = startline; i < int(animdatafile.size()); ++i)
	{
		if (!hasAlpha(animdatafile[i]))
		{
			ErrorMessage(3014, project, animdatafile[i]);
			throw 1;
		}

		string name = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		string uniquecode = animdatafile[i++];
		AnimDataPack curAP;
		curAP.name = name;
		curAP.uniquecode = uniquecode;

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		curAP.unknown1 = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		curAP.unknown2 = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		curAP.unknown3 = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i++]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		if (animdatafile[i].length() != 0 && hasAlpha(animdatafile[i]))
		{
			while (animdatafile[i].length() != 0)
			{
				curAP.eventname.push_back(animdatafile[i++]);
			}
		}

		if (animdatafile[i++].length() != 0)
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		storeline.push_back(curAP);

		if (isOnlyNumber(animdatafile[i]))
		{
			startline = i;
			break;
		}
		else
		{
			--i;
		}		
	}
}

void InfoDataProcess(vector<InfoDataPack>& storeline, int& startline, vecstr& animdatafile, string project, string modcode)
{
	unordered_map<string, bool> isExist;

	for (int i = startline; i < int(animdatafile.size()); ++i)
	{
		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3020, project, animdatafile[i]);
			throw 1;
		}

		InfoDataPack curIP;
		string uniquecode = animdatafile[i++];
		curIP.uniquecode = uniquecode;

		if (isExist[uniquecode])
		{
			ErrorMessage(3022, project, uniquecode);
			throw 1;
		}

		isExist[uniquecode] = true;

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		curIP.duration = animdatafile[i];

		if (!isOnlyNumber(animdatafile[++i]))
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		++i;

		while (count(animdatafile[i].begin(), animdatafile[i].end(), ' ') != 0)
		{
			curIP.motiondata.push_back(animdatafile[i++]);
		}

		if (!isOnlyNumber(animdatafile[i++]))
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		while (count(animdatafile[i].begin(), animdatafile[i].end(), ' ') != 0)
		{
			curIP.rotationdata.push_back(animdatafile[i++]);
		}

		if (animdatafile[i].length() != 0)
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		storeline.push_back(curIP);
	}
}

int AnimDataProject::GetAnimTotalLine()
{
	int counter = 3 + int(behaviorlist.size());

	for (auto& it : animdatalist)
	{
		counter += 7 + it.eventname.size();
	}

	return counter;
}

int AnimDataProject::GetInfoTotalLine()
{
	int counter = 0;

	for (auto& it : infodatalist)
	{
		counter += 5 + int(it.motiondata.size()) + int(it.rotationdata.size());
	}

	return counter;
}

void AnimDataCompilation(string directory, vecstr filelist, int curList, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior, getTemplate BehaviorTemplate, vector<unique_ptr<registerAnimation>>& animationList, unordered_map<string, vector<shared_ptr<Furniture>>>& newAnimation)
{
	string filepath = directory + filelist[curList];
	string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
	string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

	vecstr projectList;		// list of projects
	unordered_map<string, int> projectNameCount;	// count the occurance of the project name
	vector<AnimDataProject> ADProject;
	unordered_map<string, int> nextProject;

	string project;
	string header;

	// timer
	boost::posix_time::ptime time2;
	boost::posix_time::time_duration diff;
	double duration;

	{
		unordered_map<string, int> uCode;		// project, highest unique code
		unordered_map<string, unordered_map<string, vecstr>> catalystMap;	// project, header, list of lines
		unordered_map<string, vecstr> animDataHeader;						// project, list of headers
		unordered_map<string, vecstr> animDataInfo;							// project, list of info headers

		{
			// read behavior file
			vecstr catalyst;
			GetFunctionLines(filepath, catalyst);
			vecstr newline;
			unordered_map<string, string> chosenLines;

			// check for error
			if (error)
			{
				return;
			}

			time2 = boost::posix_time::microsec_clock::local_time();
			diff = time2 - time1;

			duration = double(diff.total_milliseconds());

			cout << "Processing time 2: " << duration / 1000 << " seconds" << endl;

			int projectcounter = 0;
			bool isOpen = true;
			bool special = false;
			bool isInfo = false;
			int num = 0;
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
			project = projectList[0] + " " + to_string(++projectNameCount[projectList[0]]);
			header = "$header$";
			animDataHeader[project].push_back(header);
			newline.reserve(20);
			newline.clear();

			time2 = boost::posix_time::microsec_clock::local_time();
			diff = time2 - time1;

			duration = double(diff.total_milliseconds());

			cout << "Processing time 3: " << duration / 1000 << " seconds" << endl;

			// add picked behavior and remove not picked behavior 
			// separation of all items for easier access and better compatibility
			for (unsigned int l = num; l < catalyst.size(); ++l)
			{
				string line = catalyst[l];
				bool skip = false;

				if (line.find("<!-- ", 0) != NOT_FOUND)
				{
					if (line.find("<!-- NEW *", 0) != NOT_FOUND)
					{
						size_t tempint = line.find("<!-- NEW *", 0) + 10;
						string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

						if (!chosenBehavior[modID])
						{
							isOpen = false;
						}

						skip = true;
					}
					else if (line.find("<!-- NEW ^", 0) != NOT_FOUND)
					{
						special = true;
					}
					else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
					{
						isOpen = true;

						if (!special)
						{
							skip = true;
						}
						else
						{
							special = false;
						}
					}
				}

				if (isOpen && !skip)
				{
					while (true)
					{
						if (line.find("<!-- *", 0) != NOT_FOUND)
						{
							size_t tempint = line.find("<!-- *") + 6;
							string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
							chosenLines[modID] = line;
							break;
						}
						else if (line.find("<!-- original -->", 0) != NOT_FOUND)
						{
							if (chosenLines.size() != 0)
							{
								line = behaviorLineChooser(line, chosenLines, behaviorPriority);
								chosenLines.clear();
							}
							else
							{
								ErrorMessage(1165);
								return;
							}
						}

						if (l + 3 < catalyst.size() && l > 2)
						{
							if (catalyst[l - 1] == "")
							{
								bool out = false;

								if (isOnlyNumber(line))
								{
									int next = 1;

									if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
									{
										++next;
									}

									if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
									{
										++next;

										if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
										{
											++next;
										}

										if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
										{
											int nextnext = next + 1;

											if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
											{
												++nextnext;
											}

											if (catalyst[l + next] == "0" || (l + nextnext < catalyst.size() && catalyst[l + nextnext].find("\\") != NOT_FOUND))
											{
												newline.shrink_to_fit();
												catalystMap[project][header] = newline;
												string newproject = projectList[++projectcounter];
												header = "$header$";
												project = newproject + " " + to_string(++projectNameCount[newproject]);
												animDataHeader[project].push_back(header);
												newline.reserve(20);
												newline.clear();
												isInfo = false;
												out = true;
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
											if (isOnlyNumber(catalyst[l + 1]))
											{
												int number = stoi(catalyst[l + 1]);
												newline.shrink_to_fit();
												catalystMap[project][header] = newline;
												newline.reserve(20);
												newline.clear();
												header = line + " " + catalyst[l + 1];
												animDataHeader[project].push_back(header);

												if (number >= uCode[project])
												{
													uCode[project] = number;
												}
											}
										}
										else if (isOnlyNumber(line))
										{
											isInfo = true;
											newline.shrink_to_fit();
											catalystMap[project][header] = newline;;
											newline.reserve(20);
											newline.clear();
											header = catalyst[++l];
											line = catalyst[l];
											animDataInfo[project].push_back(header);
										}
									}
									else if (isOnlyNumber(line))
									{
										newline.shrink_to_fit();
										catalystMap[project][header] = newline;
										newline.reserve(20);
										newline.clear();
										header = line;
										animDataInfo[project].push_back(header);
									}
								}
							}
							else if (header == "$header$")
							{
								if (hasAlpha(line) && line.find("\\") == NOT_FOUND && l + 1 < catalyst.size())
								{
									if (isOnlyNumber(catalyst[l + 1]))	// if it is unique code
									{
										int number = stoi(catalyst[l + 1]);
										newline.shrink_to_fit();
										catalystMap[project][header] = newline;
										newline.reserve(20);
										newline.clear();
										header = line + " " + catalyst[l + 1];
										animDataHeader[project].push_back(header);

										if (number >= uCode[project])
										{
											uCode[project] = number;
										}
									}
								}
								else if (isOnlyNumber(catalyst[l - 1]) && catalyst[l - 1] == "0" && isOnlyNumber(line))
								{
									int next = 1;

									if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
									{
										++next;

										if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
										{
											++next;
										}

										if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
										{
											int nextnext = next + 1;

											if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
											{
												++nextnext;
											}

											if (catalyst[l + next] == "0" || (l + nextnext < catalyst.size() && catalyst[l + nextnext].find("\\") != NOT_FOUND))
											{
												newline.shrink_to_fit();
												catalystMap[project][header] = newline;
												string newproject = projectList[++projectcounter];
												project = newproject + " " + to_string(++projectNameCount[newproject]);
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
						break;
					}
				}

				if (error)
				{
					return;
				}
			}

			if (newline.size() != 0)
			{
				if (newline.back().length() == 0)
				{
					newline.pop_back();
				}

				newline.shrink_to_fit();
				catalystMap[project][header] = newline;
				newline.clear();
			}
		}

		time2 = boost::posix_time::microsec_clock::local_time();
		diff = time2 - time1;

		duration = double(diff.total_milliseconds());

		cout << "Processing time 4: " << duration / 1000 << " seconds" << endl;

		// check for having newAnimation for the file
		if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
		{
			vecstr templateGroup = BehaviorTemplate.grouplist[lowerBehaviorFile];
			unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;
			unordered_map<string, unordered_map<string, int>> ASDCount;

			for (unsigned int j = 0; j < templateGroup.size(); ++j)
			{
				string templateCode = templateGroup[j];
				bool hasGroup = false;
				bool hasMaster = false;
				bool ignoreGroup = false;

				if (newAnimation.find(templateCode) != newAnimation.end() && newAnimation[templateCode].size() != 0)
				{
					for (unsigned int k = 0; k < newAnimation[templateCode].size(); ++k)
					{
						unordered_map<string, unordered_map<string, vecstr>> generatedAnimData;
						newAnimation[templateCode][k]->GetAnimData(generatedAnimData);

						if (error)
						{
							return;
						}

						for (auto it = generatedAnimData.begin(); it != generatedAnimData.end(); ++it)
						{
							project = it->first;
							unordered_map<string, string> isExist;

							for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
							{
								if (iter->first.find_last_of("$UC") != iter->first.length() - 1)
								{
									if (iter->first[0] == '$' && iter->first.back() == '$')
									{
										header = iter->first.substr(1, iter->first.length() - 2);
										header.append("~" + to_string(++ASDCount[project][header]));
										string uniquecode;

										if (isExist[header].length() == 0)
										{
											uniquecode = to_string(++uCode[project]);
											isExist[header] = uniquecode;
										}
										else
										{
											uniquecode = isExist[header];
										}

										iter->second[1] = uniquecode;
										animDataHeader[project].push_back(header);
									}
									else
									{
										ErrorMessage(3021, iter->first + ".txt");
										return;
									}
								}
								else
								{
									if (isExist[header].length() == 0)
									{
										header = to_string(++uCode[project]);
										isExist[header] = header;
									}
									else
									{
										header = isExist[header];
									}

									iter->second[0] = header;
									animDataInfo[project].push_back(header);
								}

								if (catalystMap[project][header].size() > 0)
								{
									ErrorMessage(5012, templateCode, project, header);
									return;
								}

								if (projectNameCount[project] != 1)
								{
									WarningMessage(1020, project, header);

									for (int i = 1; i <= projectNameCount[project]; ++i)
									{
										catalystMap[project + to_string(i)][header] = iter->second;
									}
								}
								else
								{
									catalystMap[project + "1"][header] = iter->second;
								}
							}
						}

						for (auto it = BehaviorTemplate.existingAnimDataHeader[templateCode].begin(); it != BehaviorTemplate.existingAnimDataHeader[templateCode].end(); ++it)
						{
							project = it->first;

							for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
							{
								header = *iter;

								if (catalystMap[project][header].size() == 0)
								{
									ErrorMessage(5011, templateCode, project, header);
									return;
								}

								map<int, vecstr> extract;
								newAnimation[templateCode][k]->existingASDProcess(catalystMap[project][header], extract);

								if (error)
								{
									return;
								}

								editExtract[project][header].push_back(extract);
							}
						}

						if (error)
						{
							return;
						}
					}
				}
			}

			for (auto it = editExtract.begin(); it != editExtract.end(); ++it)
			{
				string project = it->first;

				for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
				{
					string header = iter->first;
					map<int, vecstr> combined;
					int totalline = 0;

					for (unsigned int i = 0; iter->second.size(); ++i)
					{
						vecstr newline;
						string line;

						for (auto itera = iter->second[i].begin(); itera != iter->second[i].end(); ++itera)
						{
							combined[itera->first].insert(combined[itera->first].end(), itera->second.begin(), itera->second.end());
							totalline += int(itera->second.size());
						}
					}

					if (totalline == 0)
					{
						ErrorMessage(5014, project, header);
						return;
					}

					for (int i = 1; i <= projectNameCount[project]; ++i)
					{
						combineExtraction(catalystMap[project + " " + to_string(i)][header], combined, project, header);
					}

					if (error)
					{
						return;
					}
				}
			}
		}

		{
			// unsure of the function but is present in FNIS
			unordered_map<string, bool> isExist;

			for (auto& it : alternateAnim)
			{
				for (auto& iter : it.second)
				{
					header = GetFileName(iter);

					if (!isExist[header] && header != "x")
					{
						vecstr newlines;
						isExist[header] = true;
						project = "DefaultFemale.txt 1";

						newlines.push_back(header);
						newlines.push_back(to_string(++uCode[project]));
						newlines.push_back("1");
						newlines.push_back("0");
						newlines.push_back("0");
						newlines.push_back("0");
						newlines.push_back("");

						animDataHeader[project].push_back(header);
						catalystMap[project][header] = newlines;

						project = "DefaultMale.txt 1";
						newlines[1] = to_string(++uCode[project]);
						animDataHeader[project].push_back(header);
						catalystMap[project][header] = newlines;
					}
				}
			}
		}

		for (unsigned int i = 0; i < projectList.size(); ++i)
		{
			if (projectNameCount[project] > 1)
			{
				project = projectList[i] + to_string(++nextProject[project]);
			}
			else
			{
				project = projectList[i] + " 1";
			}

			vecstr combined;

			for (unsigned int j = 0; j < animDataHeader[project].size(); ++j)
			{
				header = animDataHeader[project][j];
				combined.insert(combined.end(), catalystMap[project][header].begin(), catalystMap[project][header].end());
			}

			for (unsigned int j = 0; j < animDataInfo[project].size(); ++j)
			{
				header = animDataInfo[project][j];
				combined.insert(combined.end(), catalystMap[project][header].begin(), catalystMap[project][header].end());
			}

			if (combined.size() == 0)
			{
				ErrorMessage(5017, projectList[i]);
				return;
			}

			AnimDataProject newProject(combined, projectList[i], filepath);
			ADProject.push_back(newProject);
		}
	}

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 5: " << duration / 1000 << " seconds" << endl;
	
	// final output	
#ifndef DEBUG
	string filename = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
	string filename = behaviorPath[lowerBehaviorFile];
#endif

	FolderCreate(filename.substr(0, filename.find_last_of(filename)));
	ofstream output(filename + ".txt");

	if (output.is_open())
	{
		FunctionWriter fwriter(&output);
		fwriter << to_string(projectList.size()) << "\n";

		for (unsigned int i = 0; i < projectList.size(); ++i)
		{
			fwriter << projectList[i] << "\n";
		}

		for (unsigned int i = 0; i < projectList.size(); ++i)
		{
			project = projectList[i];
			fwriter << to_string(ADProject[i].GetAnimTotalLine()) << "\n";
			fwriter << ADProject[i].unknown1 << "\n";
			fwriter << to_string(ADProject[i].behaviorlist.size()) << "\n";

			for (unsigned int j = 0; j < ADProject[i].behaviorlist.size(); ++j)
			{
				fwriter << ADProject[i].behaviorlist[j] << "\n";
			}

			fwriter << ADProject[i].unknown2 << "\n";

			if (ADProject[i].unknown2 != "0")
			{
				for (unsigned int j = 0; j < ADProject[i].animdatalist.size(); ++j)
				{
					fwriter << ADProject[i].animdatalist[j].name << "\n";
					fwriter << ADProject[i].animdatalist[j].uniquecode << "\n";
					fwriter << ADProject[i].animdatalist[j].unknown1 << "\n";
					fwriter << ADProject[i].animdatalist[j].unknown2 << "\n";
					fwriter << ADProject[i].animdatalist[j].unknown3 << "\n";
					fwriter << to_string(ADProject[i].animdatalist[j].eventname.size()) << "\n";

					for (unsigned int k = 0; k < ADProject[i].animdatalist[j].eventname.size(); ++k)
					{
						fwriter << ADProject[i].animdatalist[j].eventname[k] << "\n";
					}

					fwriter << "\n";
				}

				fwriter << to_string(ADProject[i].GetInfoTotalLine()) << "\n";

				for (unsigned int j = 0; j < ADProject[i].infodatalist.size(); ++j)
				{
					fwriter << ADProject[i].infodatalist[j].uniquecode << "\n";
					fwriter << ADProject[i].infodatalist[j].duration << "\n";
					fwriter << to_string(ADProject[i].infodatalist[j].motiondata.size()) << "\n";

					for (unsigned int k = 0; k < ADProject[i].infodatalist[j].motiondata.size(); ++k)
					{
						fwriter << ADProject[i].infodatalist[j].motiondata[k] << "\n";
					}

					fwriter << to_string(ADProject[i].infodatalist[j].rotationdata.size()) << "\n";

					for (unsigned int k = 0; k < ADProject[i].infodatalist[j].rotationdata.size(); ++k)
					{
						fwriter << ADProject[i].infodatalist[j].rotationdata[k] << "\n";
					}

					fwriter << "\n";
				}
			}
		}

		output.close();
	}
	else
	{
		ErrorMessage(1025, filename);
		return;
	}

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 6: " << duration / 1000 << " seconds" << endl;

}

AnimDataFormat::position AnimDataPosition(vecstr animData, string character, string header, string modcode, int linecount, int type, bool muteError)
{
	// has function
	bool isOpen = false;
	int functionstart = -1;
	int conditionOpen = 0;
	unordered_map<int, bool> isCondition;
	unordered_map<int, bool> isConditionOri;
	unordered_map<int, AnimDataFunct> marker;

	for (unsigned int i = 0; i < animData.size(); ++i)
	{
		if (animData[i].find("<!-- ") != NOT_FOUND)
		{
			if (functionstart == -1)
			{
				functionstart = i;
			}

			marker[i].skip = true;

			if (animData[i].find("<!-- CLOSE -->") != NOT_FOUND)
			{
				isOpen = false;
			}
			else if (animData[i].find("<!-- CONDITION END -->") != NOT_FOUND)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = false;
				--conditionOpen;
			}
		}

		if (isOpen)
		{
			marker[i].isNew = true;
		}

		if (isCondition[conditionOpen])
		{
			marker[i].isCondition = true;
			marker[i].conditionOpen = conditionOpen;

		}
		else if (isConditionOri[conditionOpen])
		{
			marker[i].isConditionOri = true;
			marker[i].conditionOpen = conditionOpen;
		}

		if (animData[i].find("<!-- ") != NOT_FOUND)
		{
			if (animData[i].find("<!-- NEW") != NOT_FOUND)
			{
				isOpen = true;
			}
			else if (animData[i].find("<!-- CONDITION START ^") != NOT_FOUND)
			{
				++conditionOpen;
				isCondition[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION -->") != NOT_FOUND)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION ^") != NOT_FOUND)
			{
				marker[i].nextCondition = true;
			}
		}
	}

	using namespace AnimDataFormat;

	if (linecount < int(animData.size()))
	{
		if (type == 0)
		{
			if (linecount < 3 && functionstart == 0)
			{
				return AnimDataConvert(type, linecount + 1, muteError);
				// 1. total line count
				// 2. unknown number
				// 3. behavior file count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != NOT_FOUND)
				{
					if (!muteError)
					{
						ErrorMessage(3007, modcode, linecount, header);
						return xerror;
					}
				}

				int id = 0;
				unordered_map<int, int> subid;

				for (int i = 0; i < linecount + 1; ++i)
				{
					if (!marker[i].skip)
					{
						if (marker[i].isCondition)
						{
							++subid[marker[i].conditionOpen];
						}
						else
						{
							if (marker[i].conditionOpen > 1)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
							}
							else
							{
								subid[1] = 0;
								++id;

								if (id > 3)
								{
									break;
								}
							}
						}

						if (marker[i].isNew)
						{
							int tempid = id;

							for (auto it = subid.begin(); it != subid.end(); ++it)
							{
								tempid = tempid + it->second;
							}

							if (tempid != 4)
							{
								if (!muteError)
								{
									ErrorMessage(3007, modcode, linecount, header);
									return xerror;
								}
							}
						}
					}
					else if (marker[i].nextCondition)
					{
						subid[marker[i].conditionOpen] = 0;
					}
				}

				for (auto it = subid.begin(); it != subid.end(); ++it)
				{
					id = id + it->second;
				}

				if (id < 4)
				{
					return AnimDataConvert(type, id, muteError);
				}
			}

			if (isOnlyNumber(animData[linecount]))
			{
				return unknown5;
				// 5. unknown number
			}
			else
			{
				return behaviorfilelist;
				// 4. behavior file list
			}
		}
		else if (type == 1)
		{
			if (linecount < 6 && functionstart > 5)
			{
				return AnimDataConvert(type, linecount + 1, muteError);
				// 1. animation name (clip)
				// 2. unique code
				// 3. unknown number (1)
				// 4. unknown number (0)
				// 5. unknown number (0)
				// 6. event name count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != NOT_FOUND)
				{
					if (!muteError)
					{
						ErrorMessage(3007, modcode, linecount, header);
						return xerror;
					}
				}

				int id = 0;
				unordered_map<int, int> subid;

				for (int i = 0; i < linecount + 1; ++i)
				{
					if (!marker[i].skip)
					{
						if (marker[i].isCondition)
						{
							++subid[marker[i].conditionOpen];
						}
						else
						{
							if (marker[i].conditionOpen > 1)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
							}
							else
							{
								subid[1] = 0;
								++id;

								if (id > 6)
								{
									break;
								}
							}
						}

						if (marker[i].isNew)
						{
							int tempid = id;

							for (auto it = subid.begin(); it != subid.end(); ++it)
							{
								tempid = tempid + it->second;
							}

							if (tempid != 7)
							{
								ErrorMessage(3007, modcode, linecount, header);
								return xerror;
							}
						}
					}
					else if (marker[i].nextCondition)
					{
						subid[marker[i].conditionOpen] = 0;
					}
				}

				for (auto it = subid.begin(); it != subid.end(); ++it)
				{
					id = id + it->second;
				}

				if (id < 7)
				{
					return AnimDataConvert(type, id, muteError);
				}
			}

			if (linecount == int(animData.size()) - 1)
			{
				return space;
				// 8. <space>
			}
			else if (hasAlpha(animData[linecount]))
			{
				return eventnamelist;
				// 7. event name list
			}
		}
		else if (type == 2)
		{
			int nextline = 3;

			if (linecount < 3 && functionstart > 2)
			{
				return AnimDataConvert(type, linecount + 1, muteError);
				// 1. unique code
				// 2. total animation (clip) length
				// 3. motion data count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != NOT_FOUND)
				{
					if (!muteError)
					{
						ErrorMessage(3007, modcode, linecount, header);
						return xerror;
					}
				}

				int id = 0;
				unordered_map<int, int> subid;

				for (int i = 0; i < linecount + 1; ++i)
				{
					if (!marker[i].skip)
					{
						if (marker[i].isCondition)
						{
							++subid[marker[i].conditionOpen];
						}
						else
						{
							if (marker[i].conditionOpen > 1)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
							}
							else
							{
								subid[1] = 0;
								++id;

								if (id > 3)
								{
									nextline = i;
									break;
								}
							}
						}

						if (marker[i].isNew)
						{
							int tempid = id;

							for (auto it = subid.begin(); it != subid.end(); ++it)
							{
								tempid = tempid + it->second;
							}

							if (tempid != 4 && tempid != 6)
							{
								if (!muteError)
								{
									ErrorMessage(3007, modcode, linecount, header);
									return xerror;
								}
							}
						}
					}
					else if (marker[i].nextCondition)
					{
						subid[marker[i].conditionOpen] = 0;
					}
				}

				for (auto it = subid.begin(); it != subid.end(); ++it)
				{
					id = id + it->second;
				}

				if (id < 4)
				{
					return AnimDataConvert(type, id, muteError);
				}
			}

			if (linecount == int(animData.size()) - 1)
			{
				return space;
				// 7. <space>
			}

			int id = 4;
			bool nextplus = false;
			unordered_map<int, int> subid;
			
			for (int i = nextline; i < linecount + 1; ++i)
			{
				if (!marker[i].skip)
				{
					if (marker[i].isCondition)
					{
						if (isOnlyNumber(animData[i]) && count(animData[i].begin(), animData[i].end(), ' ') == 0)
						{
							++subid[marker[i].conditionOpen];

							if (marker.find(i + 1) != marker.end())
							{
								if (marker[i + 1].conditionOpen == marker[i].conditionOpen)
								{
									nextplus = true;
								}
							}
						}
						else if (nextplus)
						{
							++subid[marker[i].conditionOpen];
							nextplus = false;
						}
					}
					else
					{
						if (marker[i].conditionOpen > 1)
						{
							if (isOnlyNumber(animData[i]) && count(animData[i].begin(), animData[i].end(), ' ') == 0)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];

								if (marker.find(i + 1) != marker.end())
								{
									if (marker[i + 1].conditionOpen == marker[i].conditionOpen)
									{
										nextplus = true;
									}
								}
							}
							else if (nextplus)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
								nextplus = false;
							}
						}
						else
						{
							if (isOnlyNumber(animData[i]) && count(animData[i].begin(), animData[i].end(), ' ') == 0)
							{
								subid[1] = 0;
								++id;

								if (marker.find(i + 1) == marker.end())
								{
									nextplus = true;
								}
							}
							else if (nextplus)
							{
								subid[1] = 0;
								++id;
								nextplus = false;
							}
						}
					}

					if (marker[i].isNew)
					{
						int tempid = id;

						for (auto it = subid.begin(); it != subid.end(); ++it)
						{
							tempid = tempid + it->second;
						}

						if (tempid != 4 && tempid != 6)
						{
							ErrorMessage(3007, modcode, linecount, header);
							return xerror;
						}
					}
				}
				else if (marker[i].nextCondition)
				{
					subid[marker[i].conditionOpen] = 0;
				}
			}

			for (auto it = subid.begin(); it != subid.end(); ++it)
			{
				id = id + it->second;
			}

			if (id < 7 && id > 3)
			{
				return AnimDataConvert(type, id, muteError);
				// 4. motion data list
				// 5. rotation data count
				// 6. rotation data list
			}
		}
	}

	if (!muteError)
	{
		ErrorMessage(3005, character, header);
	}

	return xerror;
}

AnimDataFormat::position AnimDataConvert(int type, int position, bool muteError)
{
	using namespace AnimDataFormat;

	if (type == 1)
	{
		if (position == 1)
		{
			return totallinecount;
		}
		else if (position == 2)
		{
			return unknown2;
		}
		else if (position == 3)
		{
			return behaviorfilecount;
		}
		else if (position == 4)
		{
			return behaviorfilelist;
		}
		else if (position == 5)
		{
			return unknown5;
		}
	}
	else if (type == 2)
	{
		if (position == 1)
		{
			return animationname;
		}
		else if (position == 2)
		{
			return uniquecode;
		}
		else if (position == 3)
		{
			return unknown3;
		}
		else if (position == 4)
		{
			return unknown4;
		}
		else if (position == 5)
		{
			return unknown5;
		}
		else if (position == 6)
		{
			return eventnamecount;
		}
		else if (position == 7)
		{
			return eventnamelist;
		}
		else if (position == 8)
		{
			return space;
		}
	}
	else
	{
		if (position == 1)
		{
			return uniquecode;
		}
		else if (position == 2)
		{
			return totalcliplength;
		}
		else if (position == 3)
		{
			return motiondatacount;
		}
		else if (position == 4)
		{
			return motiondatalist;
		}
		else if (position == 5)
		{
			return rotationdatacount;
		}
		else if (position == 6)
		{
			return rotationdatalist;
		}
		else if (position == 7)
		{
			return space;
		}
	}

	return xerror;
}
