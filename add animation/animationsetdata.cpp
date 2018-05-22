#include "animationsetdata.h"

using namespace std;

AnimationDataProject::AnimationDataProject(int& startline, vecstr& animdatafile, string filename)
{
	// data list
	DataPackProcess(datalist, startline, animdatafile, filename);
	++startline;

	for (auto it = datalist.begin(); it != datalist.end(); ++it)
	{
		++startline;

		if (startline >= int(animdatafile.size()))
		{
			ErrorMessage(5019, filename);
			throw 1;
		}

		// equip list
		EquipPackProcess(it->second.equiplist, startline, animdatafile, filename, it->first);

		// type list
		TypePackProcess(it->second.typelist, startline, animdatafile, filename, it->first);

		// anim list
		AnimPackProcess(it->second.animlist, startline, animdatafile, filename, it->first);

		// crc3 list
		CRC32Process(it->second.crc32list, startline, animdatafile, filename, it->first);
	}
}

void DataPackProcess(map<string, datapack, alphanum_less<string>>& storeline, int& startline, vecstr& animdatafile, string filename)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, "Header");
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, startline);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		datapack newDataPack;
		storeline[animdatafile[i]] = newDataPack;

		if (i + 4 >= animdatafile.size())
		{
			ErrorMessage(5018, filename, "Header");
			throw 1;
		}

		if (boost::iequals(animdatafile[i + 1], "V3"))
		{
			startline = i;
			break;
		}
	}
}

void EquipPackProcess(std::vector<equip>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, startline);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (isOnlyNumber(animdatafile[i]))
		{
			startline = i;
			break;
		}

		equip tempEquip;
		tempEquip.name = animdatafile[i];
		storeline.push_back(tempEquip);

		if (i + 1 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
	}
}

void TypePackProcess(vector<typepack>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, header);
		throw 1;
	}

	int counter = 0;

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (isOnlyNumber(animdatafile[i]))
		{
			if (i + 1 < animdatafile.size() && hasAlpha(animdatafile[i + 1]))
			{
				startline = i;
				break;
			}
			else if (i + 4 < animdatafile.size() && animdatafile[i + 4] == "7891816")
			{
				startline = i;
				break;
			}
			else if (i + 2 < animdatafile.size() && boost::iequals(animdatafile[i + 2], "V3"))
			{
				startline = i;
				break;
			}
			else if (i + 2 == animdatafile.size())
			{
				startline = i;
				break;
			}
		}

		typepack tempTP;

		if (isOnlyNumber(animdatafile[i]) || !hasAlpha(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempTP.name = animdatafile[i];

		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempTP.equiptype1 = animdatafile[i];
		
		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempTP.equiptype2 = animdatafile[i];
		storeline.push_back(tempTP);

		if (i + 1 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
	}
}

void AnimPackProcess(vector<animpack>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, header);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (animdatafile[i] == "0")
		{
			if (i + 1 < animdatafile.size() && animdatafile[i + 1] == "V3")
			{
				startline = i;
				break;
			}
			else if (i + 2 < animdatafile.size() && animdatafile[i + 2].find(".txt") != NOT_FOUND)
			{
				startline = i;
				break;
			}
		}
		
		if (animdatafile[i] == "1")
		{
			if (i + 4 < animdatafile.size() && animdatafile[i + 4] == "V3")
			{
				startline = i;
				break;
			}
			else if (i + 5 < animdatafile.size() && animdatafile[i + 5].find(".txt") != NOT_FOUND)
			{
				startline = i;
				break;
			}
		}
		
		if (isOnlyNumber(animdatafile[i]))
		{
			if (i + 3 < animdatafile.size() && animdatafile[i + 3] == "7891816")
			{
				startline = i;
				break;
			}
			else if (i + 1 == animdatafile.size())
			{
				startline = i;
				break;
			}
		}

		animpack tempAP;
		tempAP.name = animdatafile[i++];
		tempAP.unknown = animdatafile[i++];

		if (i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}
		
		while (i < int(animdatafile.size()))
		{
			++i;

			if (animdatafile[i] == "0")
			{
				if (i + 1 < animdatafile.size() && animdatafile[i + 1] == "V3")
				{
					--i;
					break;
				}
				else if (i + 2 < animdatafile.size() && animdatafile[i + 2].find(".txt") != NOT_FOUND)
				{
					--i;
					break;
				}
			}
			
			if (animdatafile[i] == "1")
			{
				if (i + 4 < animdatafile.size() && animdatafile[i + 4] == "V3")
				{
					--i;
					break;
				}
				else if (i + 5 < animdatafile.size() && animdatafile[i + 5].find(".txt") != NOT_FOUND)
				{
					--i;
					break;
				}
			}
			
			if (isOnlyNumber(animdatafile[i]))
			{
				if (i + 3 < animdatafile.size() && animdatafile[i + 3] == "7891816")
				{
					--i;
					break;
				}
				else if (i + 1 == animdatafile.size())
				{
					startline = i;
					break;
				}
			}
			
			if (hasAlpha(animdatafile[i]))
			{
				bool out = false;

				for (int k = i + 1; k < i + 3; ++k)
				{
					if (!isOnlyNumber(animdatafile[k]))
					{
						out = false;
						break;
					}
					else
					{
						out = true;
					}
				}

				if (out)
				{
					if (i + 3 < animdatafile.size() && hasAlpha(animdatafile[i + 3]) && animdatafile[i + 3] != "V3")
					{
						--i;
						break;
					}
				}
			}

			attackdata attack;
			tempAP.attack.push_back(attack);
			tempAP.attack.back().data = animdatafile[i];
		}

		storeline.push_back(tempAP);

		if (i + 1 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
	}
}

void CRC32Process(vector<crc32>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, header);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (animdatafile[i] == "V3")
		{
			startline = i;
			break;
		}
		else if (i + 1 < animdatafile.size() && animdatafile[i + 1].find(".txt") != NOT_FOUND)
		{
			startline = i;
			break;
		}

		crc32 tempCRC32;

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempCRC32.filepath = animdatafile[i];

		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempCRC32.filename = animdatafile[i];

		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempCRC32.fileformat = animdatafile[i];
		storeline.push_back(tempCRC32);
		
		if (i + 1 >= int(animdatafile.size()))
		{
			startline = i;
		}
	}
}

void ASDCompilation(string directory, vecstr filelist, int curList, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior, getTemplate BehaviorTemplate, unordered_map<string, vector<shared_ptr<Furniture>>>& newAnimation, unordered_map<string, var> AnimVar)
{
	string filepath = directory + filelist[curList];
	string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
	string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

	vecstr projectList;
	int projectcounter = 0;
	int headercounter = 0;
	bool isOpen = true;
	bool special = false;
	string newMod;
	string project = "	";
	string header = project;
	string line;
	unordered_map<string, string> chosenLines;
	unordered_map<string, map<string, vecstr, alphanum_less<string>>> ASDPack;
	unordered_map<string, AnimationDataProject> ASDData;

	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time 2: " << duration / 1000 << " seconds" << endl;

	{
		// read behavior file
		vecstr catalyst;
		GetFunctionLines(filepath, catalyst, false);
		vecstr newline;
		vecstr storeline;

		while (catalyst.back().length() == 0)
		{
			catalyst.pop_back();
		}

		// check for error
		if (error)
		{
			return;
		}

		// add picked behavior and remove not picked behavior 
		// separation of all items for easier access and better compatibility
		for (unsigned int l = 0; l < catalyst.size(); ++l)
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
					else
					{
						newMod = modID;
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
					newMod.clear();

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

					storeline.push_back(line);
					break;
				}
			}

			if (error)
			{
				return;
			}
		}

		for (int i = 1; i < catalyst.size(); ++i)
		{
			if (isOnlyNumber(catalyst[i]))
			{
				break;
			}

			projectList.push_back(catalyst[i]);
			newline.push_back(catalyst[i]);
		}

		unordered_map<string, vecstr> animDataSetHeader;
		animDataSetHeader[project].push_back(header);

		time2 = boost::posix_time::microsec_clock::local_time();
		diff = time2 - time1;

		duration = double(diff.total_milliseconds());

		cout << "Processing time 3: " << duration / 1000 << " seconds" << endl;

		int num;

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
				else
				{
					newline.reserve(100);
					newline.clear();
					project = projectList[projectcounter];
					++projectcounter;
					headercounter = 0;
					animDataSetHeader[project].push_back("	");
					newline.push_back(storeline[i]);
					++i;
				}

				if (animDataSetHeader[project].size() != 1)
				{
					ErrorMessage(5005, filepath, i + 1);
					return;
				}

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
						{
							ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
							return;
						}

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
						ErrorMessage(5001, filepath, i + 1);
						return;
					}

					newline.push_back(storeline[i]);
					++i;
				}
			}
			else if (wordFind(storeline[i], "V3") != NOT_FOUND)
			{
				if (headercounter >= animDataSetHeader[project].size())
				{
					ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
					return;
				}

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

			newline.push_back(storeline[i]);
		}

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

				if (animDataSetHeader[project].size() != 1)
				{
					ErrorMessage(5005, filepath, i + 1);
					return;
				}

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
						{
							ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
							return;
						}

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
						ErrorMessage(5001, filepath, i + 1);
						return;
					}

					newline.push_back(storeline[i]);
					++i;
				}
			}
			else if (wordFind(storeline[i], "V3") != NOT_FOUND)
			{
				if (headercounter >= animDataSetHeader[project].size())
				{
					ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
					return;
				}

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

			newline.push_back(storeline[i]);
		}

		if (error)
		{
			return;
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

		time2 = boost::posix_time::microsec_clock::local_time();
		diff = time2 - time1;

		duration = double(diff.total_milliseconds());

		cout << "Processing time 4: " << duration / 1000 << " seconds" << endl;
	}
	
	// check for having newAnimation for the file
	if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
	{
		vecstr templateGroup = BehaviorTemplate.grouplist[lowerBehaviorFile];
		unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;	// project, header, list of extracts, startline, extractions; to get all edits done to master branch

		for (unsigned int j = 0; j < templateGroup.size(); ++j)
		{
			string templateCode = templateGroup[j];
			unordered_map<string, int> ASDCount;
			bool hasGroup = false;
			bool hasMaster = false;
			bool ignoreGroup = false;

			if (newAnimation.find(templateCode) != newAnimation.end() && newAnimation[templateCode].size() != 0)
			{
				for (unsigned int k = 0; k < newAnimation[templateCode].size(); ++k)
				{
					unordered_map<string, map<string, vecstr, alphanum_less<string>>> generatedASD;
					newAnimation[templateCode][k]->GetAnimSetData(generatedASD);

					if (error)
					{
						return;
					}

					for (auto it = generatedASD.begin(); it != generatedASD.end(); ++it)
					{
						string project = it->first;

						if (ASDPack.find(project) == ASDPack.end())
						{
							projectList.push_back(project);
						}

						for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
						{
							string header = iter->first.substr(1, iter->first.length() - 2);
							header.append("_" + to_string(++ASDCount[header]) + ".txt");

							if (ASDPack[project][header].size() > 0)
							{
								ErrorMessage(5012, templateCode, project, header);
								return;
							}

							ASDPack[project][header] = iter->second;
						}
					}

					for (auto it = BehaviorTemplate.existingASDHeader[templateCode].begin(); it != BehaviorTemplate.existingASDHeader[templateCode].end(); ++it)
					{
						string project = it->first;

						for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
						{
							string header = *iter;

							if (ASDPack[project][header].size() == 0)
							{
								ErrorMessage(5011, templateCode, project, header);
								return;
							}

							map<int, vecstr> extract;
							newAnimation[templateCode][k]->existingASDProcess(ASDPack[project][header], extract, 1);

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

				combineExtraction(ASDPack[project][header], combined, project, header);

				if (error)
				{
					return;
				}
			}
		}
	}

	for (unsigned int i = 0; i < projectList.size(); ++i)
	{
		int startline = 0;
		vecstr projectline;
		projectline.push_back(to_string(ASDPack[projectList[i]].size()));

		for (auto it = ASDPack[projectList[i]].begin(); it != ASDPack[projectList[i]].end(); ++it)
		{
			projectline.push_back(it->first);
		}

		for (auto it = ASDPack[projectList[i]].begin(); it != ASDPack[projectList[i]].end(); ++it)
		{
			projectline.insert(projectline.end(), it->second.begin(), it->second.end());
		}

		AnimationDataProject newProject(startline, projectline, filepath);
		ASDData[projectList[i]] = newProject;

		if (error)
		{
			return;
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
			string curProject = projectList[i];
			fwriter << to_string(ASDData[curProject].datalist.size()) << "\n";

			for (auto it = ASDData[curProject].datalist.begin(); it != ASDData[curProject].datalist.end(); ++it)
			{
				fwriter << it->first << "\n";
			}
			
			for (auto it = ASDData[curProject].datalist.begin(); it!= ASDData[curProject].datalist.end(); ++it)
			{
				fwriter << "V3" << "\n";
				fwriter << to_string(it->second.equiplist.size()) << "\n";

				for (unsigned int k = 0; k < it->second.equiplist.size(); ++k)
				{
					fwriter << it->second.equiplist[k].name << "\n";
				}

				fwriter << to_string(it->second.typelist.size()) << "\n";

				for (unsigned int k = 0; k < it->second.typelist.size(); ++k)
				{
					fwriter << it->second.typelist[k].name << "\n";
					fwriter << it->second.typelist[k].equiptype1 << "\n";
					fwriter << it->second.typelist[k].equiptype2 << "\n";
				}

				fwriter << to_string(it->second.animlist.size()) << "\n";

				for (unsigned int k = 0; k < it->second.animlist.size(); ++k)
				{
					fwriter << it->second.animlist[k].name << "\n";
					fwriter << it->second.animlist[k].unknown << "\n";
					fwriter << to_string(it->second.animlist[k].attack.size()) << "\n";

					for (unsigned int j = 0; j < it->second.animlist[k].attack.size(); ++j)
					{
						fwriter << it->second.animlist[k].attack[j].data << "\n";
					}
				}

				fwriter << to_string(it->second.crc32list.size()) << "\n";

				for (unsigned int k = 0; k < it->second.crc32list.size(); ++k)
				{
					fwriter << it->second.crc32list[k].filepath << "\n";
					fwriter << it->second.crc32list[k].filename << "\n";
					fwriter << it->second.crc32list[k].fileformat << "\n";
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

ASDFormat::position ASDPosition(vecstr animData, string project, string header, string modcode, int linecount, bool muteError)
{
	// has function
	bool isOpen = false;
	int functionstart = -1;
	int conditionOpen = 0;
	unordered_map<int, bool> isCondition;
	unordered_map<int, bool> isConditionOri;
	unordered_map<int, ASDFunct> marker;

	if (linecount >= int(animData.size()))
	{
		ErrorMessage(5010, modcode, project, header);
		return ASDFormat::xerror;
	}

	if (animData[linecount].find("<!-- ") != NOT_FOUND)
	{
		if (!muteError)
		{
			ErrorMessage(3007, modcode, linecount, header);
			return ASDFormat::xerror;
		}
	}

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
				if (!isOpen)
				{
					ErrorMessage(1171, modcode, i + 1);
					return ASDFormat::xerror;
				}

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
				if (isOpen)
				{
					ErrorMessage(1115, modcode, i + 1);
					return ASDFormat::xerror;
				}

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

	using namespace ASDFormat;

	if (linecount < 2 && functionstart < 1)
	{
		return ASDConvert(linecount + 1, muteError);
		// 1. V3
		// 2. equipcount
	}
	else
	{
		int id = 0;

		try
		{
			int curline = 0;
			int result = PositionLineCondition(curline, 0, linecount, animData, marker, modcode, header, true, muteError);

			if (result == -2)
			{
				if (!muteError)
				{
					ErrorMessage(5008, modcode, project, header);
				}

				return xerror;
			}
		}
		catch (double curID)
		{
			id = static_cast<int>(curID);
		}

		return ASDConvert(id, muteError);
	}

	if (!muteError)
	{
		ErrorMessage(5009, project, header);
	}

	return xerror;
}

ASDFormat::position ASDConvert(int position, bool muteError)
{
	using namespace ASDFormat;

	if (position == 1)
	{
		return V3;
	}
	else if (position == 2)
	{
		return equipcount;
	}
	else if (position == 3)
	{
		return equiplist;
	}
	else if (position == 4)
	{
		return typecount;
	}
	else if (position == 5)
	{
		return typelist;
	}
	else if (position == 6)
	{
		return animpackcount;
	}
	else if (position == 7)
	{
		return animpackname;
	}
	else if (position == 8)
	{
		return unknown3;
	}
	else if (position == 9)
	{
		return attackcount;
	}
	else if (position == 10)
	{
		return attacklist;
	}
	else if (position == 11)
	{
		return crc32count;
	}
	else if (position == 12)
	{
		return crc32list;
	}
	else if (position == 13)
	{
		return unknown1;
	}
	else if (position == 14)
	{
		return unknown2;
	}

	return xerror;
}

int PositionLineCondition(int& i, double curID, int linecount, vecstr animDataSet, unordered_map<int, ASDFunct>& marker, string modcode, string header, bool last, bool muteError)
{
	using namespace ASDFormat;
	double id = curID;
	int conditionOpen = marker[i].conditionOpen;
	bool jump = false;
	
	if (id < 3)
	{
		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (id == 2)
				{
					++id;
					break;
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
					else
					{
						++id;
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
						else
						{
							++id;
						}
					}
				}

				if (marker[i].isNew)
				{
					if (!muteError)
					{
						ErrorMessage(5007, modcode, i + 1, header);
					}

					return -1;
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}
	
	if (id == 3)
	{
		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (isOnlyNumber(animDataSet[i]))
				{
					++id;
					break;
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
					}
				}

				if (marker[i].isNew)
				{
					if (id != 3)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}

	if (id == 4)
	{
		if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
		{
			if (i == linecount)
			{
				throw id;
			}

			++id;
			++id;
			++i;
		}
		else
		{
			while (i < linecount + 1)
			{
				if (!marker[i].skip)
				{
					if (hasAlpha(animDataSet[i]))
					{
						jump = true;
						++id;
						break;
					}

					if (marker[i].isCondition)		// new condition
					{
						if (conditionOpen < marker[i].conditionOpen)
						{
							int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

							if (exit == -1 && !last)
							{
								return -1;
							}
						}
						else if (conditionOpen > marker[i].conditionOpen)
						{
							--i;
							return 0;
						}
					}
					else
					{
						if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
						{
							--i;
							return 0;
						}
						else
						{
							if (!last)
							{
								--i;
								return -1;
							}
						}
					}

					if (marker[i].isNew)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, i + 1, header);
						}

						return -1;
					}
				}
				else if (marker[i].nextCondition)		// next condition or close condition
				{
					id = curID;
				}

				++i;
			}
		}
	}

	if (id == 5 || id == 13 || id == 14)		// change this
	{
		if (id == 5 && jump)
		{
			jump = false;
			id = 14;
		}

		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (isOnlyNumber(animDataSet[i]) && id == 14)
				{
					if (animDataSet[i] == "0")
					{
						if (id == 14)
						{
							id = 6;
							break;
						}
					}
					else
					{
						int next = 1;

						if (animDataSet[i + next].find("<!--") != NOT_FOUND)
						{
							++next;
						}

						if (hasAlpha(animDataSet[i + next]))
						{
							++next;

							if (animDataSet[i + next].find("<!--") != NOT_FOUND)
							{
								++next;
							}

							if (isOnlyNumber(animDataSet[i + next]))
							{
								++next;

								if (animDataSet[i + next].find("<!--") != NOT_FOUND)
								{
									++next;
								}

								if (isOnlyNumber(animDataSet[i + next]))
								{
									++next;

									if (animDataSet[i + next].find("<!--") != NOT_FOUND)
									{
										++next;
									}

									if (hasAlpha(animDataSet[i + next]))
									{
										id = 6;
										break;
									}
								}
							}
						}
					}
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
					else
					{
						if (id == 14)
						{
							if (hasAlpha(animDataSet[i]))
							{
								id = 5;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 5)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 13;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 13)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 14;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else
						{
							if (!muteError)
							{
								ErrorMessage(5007, modcode, i + 1, header);
							}

							return -1;
						}
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
						else
						{
							if (id == 14)
							{
								if (hasAlpha(animDataSet[i]))
								{
									id = 5;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 5)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 13;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 13)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 14;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
				}

				if (marker[i].isNew)
				{
					if (id != 5 && id != 13 && id != 14)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}

	if (id == 6)
	{
		if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
		{
			if (i == linecount)
			{
				throw id;
			}

			id = 11;
			++i;
		}
		else
		{
			while (i < linecount + 1)
			{
				if (!marker[i].skip)
				{
					if (hasAlpha(animDataSet[i]))
					{
						jump = true;
						++id;
						break;
					}

					if (marker[i].isCondition)		// new condition
					{
						if (conditionOpen < marker[i].conditionOpen)
						{
							int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

							if (exit == -1 && !last)
							{
								return -1;
							}
						}
						else if (conditionOpen > marker[i].conditionOpen)
						{
							--i;
							return 0;
						}
						else
						{
							if (id != 6)
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
					else
					{
						if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
						{
							--i;
							return 0;
						}
						else
						{
							if (!last)
							{
								--i;
								return -1;
							}
							else
							{
								if (id != 6)
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
						}
					}

					if (marker[i].isNew)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
				else if (marker[i].nextCondition)		// next condition or close condition
				{
					id = curID;
				}

				++i;
			}
		}
	}

	if (id > 6 && id < 11)
	{
		if (id == 7 && jump)
		{
			jump = false;
			id = 10;
		}

		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (isOnlyNumber(animDataSet[i]) && id == 10)
				{
					if (animDataSet[i] == "0" && (i == animDataSet.size() - 1 || boost::iequals(animDataSet[i + 1], "V3")))
					{
						++id;
						break;
					}
					else
					{
						int next = 1;

						if (animDataSet[i + next].find("<!--") != NOT_FOUND)
						{
							++next;
						}

						if (isOnlyNumber(animDataSet[i + next]))
						{
							++next;

							if (animDataSet[i + next].find("<!--") != NOT_FOUND)
							{
								++next;
							}

							if (isOnlyNumber(animDataSet[i + next]))
							{
								++next;

								if (animDataSet[i + next].find("<!--") != NOT_FOUND)
								{
									++next;
								}

								if (isOnlyNumber(animDataSet[i + next]) && animDataSet[i + next] == "7891816")
								{
									++id;
									break;
								}
							}
						}
					}
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
					else
					{
						if (id == 10)
						{
							if (hasAlpha(animDataSet[i]))
							{
								int next = i + 1;

								while (true)
								{
									if (marker[next].skip)
									{
										++next;
									}
									else
									{
										break;
									}
								}

								if (isOnlyNumber(animDataSet[next]))
								{
									id = 7;
								}
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 7)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 8;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 8)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 9;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 9)
						{
							if (hasAlpha(animDataSet[i]))
							{
								id = 10;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else
						{
							if (!muteError)
							{
								ErrorMessage(5007, modcode, i + 1, header);
							}

							return -1;
						}
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
						else
						{
							if (id == 10)
							{
								if (hasAlpha(animDataSet[i]))
								{
									int next = i + 1;

									while (true)
									{
										if (marker[next].skip)
										{
											++next;
										}
										else
										{
											break;
										}
									}

									if (isOnlyNumber(animDataSet[next]))
									{
										id = 7;
									}
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 7)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 8;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 8)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 9;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 9)
							{
								if (hasAlpha(animDataSet[i]))
								{
									id = 10;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
				}

				if (marker[i].isNew)
				{
					if (id != 7 && id != 8 && id != 9 && id != 10)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}

	if (id == 11)
	{
		if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
		{
			if (i == linecount)
			{
				throw id;
			}

			++id;
		}
		else
		{
			while (i < linecount + 1)
			{
				if (i + 3 < animDataSet.size() && isOnlyNumber(animDataSet[i]))
				{
					int next = 1;

					if (animDataSet[i + next].find("<!--") != NOT_FOUND)
					{
						++next;
					}

					if (isOnlyNumber(animDataSet[i + next]))
					{
						++next;

						if (animDataSet[i + next].find("<!--") != NOT_FOUND)
						{
							++next;
						}

						if (animDataSet[i + next] == "7891816")
						{
							jump = true;
							++id;
							break;
						}
					}
				}

				if (!marker[i].skip)
				{
					if (marker[i].isCondition)		// new condition
					{
						if (conditionOpen < marker[i].conditionOpen)
						{
							int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

							if (exit == -1 && !last)
							{
								return -1;
							}
						}
						else if (conditionOpen > marker[i].conditionOpen)
						{
							--i;
							return 0;
						}
						else
						{
							if (id != 11)
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
					else
					{
						if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
						{
							--i;
							return 0;
						}
						else
						{
							if (!last)
							{
								--i;
								return -1;
							}
							else
							{
								if (id != 11)
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
						}
					}

					if (marker[i].isNew)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
				else if (marker[i].nextCondition)		// next condition or close condition
				{
					id = curID;
				}

				++i;
			}
		}
	}

	if (id == 12)
	{
		if (!isOnlyNumber(animDataSet[i]))
		{
			if (!muteError)
			{
				ErrorMessage(5007, modcode, i + 1, header);
			}

			return -1;
		}
	}

	if (id < 15)
	{
		throw id;
	}

	return -2;
}

void combineExtraction(vecstr& storeline, map<int, vecstr> extract, string project, string header)
{
	vecstr newline;
	bool newOpen = false;
	int condition = 0;

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		string line = storeline[i];

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			condition++;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
		{
			newOpen = true;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
		{
			newOpen = true;
		}
		else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
		{
			newOpen = true;
		}

		if (condition == 0 && !newOpen)
		{
			newline.push_back(storeline[i]);
		}

		if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			newOpen = false;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			condition--;
		}

		if (extract[i].size() > 0)
		{
			if (condition == 0 && !newOpen)
			{
				ErrorMessage(5013, project, header);
				return;
			}

			for (unsigned int k = 0; k < extract[i].size(); ++k)
			{
				newline.push_back(extract[i][k]);
			}
		}
	}

	storeline = newline;
}
