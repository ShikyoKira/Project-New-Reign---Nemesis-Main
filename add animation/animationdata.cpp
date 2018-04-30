#include "animationdata.h"

#pragma warning(disable:4503)

using namespace std;

void AnimDataCompilation(string directory, vecstr filelist, int curList, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior, getTemplate BehaviorTemplate, vector<unique_ptr<registerAnimation>>& animationList, unordered_map<string, vector<shared_ptr<Furniture>>>& newAnimation, unordered_map<string, var> AnimVar, unordered_map<string, vector<string>> modAnimBehavior)
{
	string filepath = directory + filelist[curList];
	string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
	string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

	int curID = 0;

	unordered_map<string, unordered_map<string, vecstr>> catalystMap;
	unordered_map<string, vecstr> animDataHeader;
	vecstr animDataChar;

	// read behavior file
	vecstr catalyst = GetFunctionLines(filepath);

	// check for error
	if (error)
	{
		return;
	}

	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time 2: " << duration / 1000 << " seconds" << endl;

	int infoline = 0;
	int num = 0;
	size_t openRange = 0;
	bool isOpen = true;
	bool special = false;
	bool newinfo = false;
	bool isInfo = false;
	bool end = false;
	string newMod;
	string character = "$header$";
	string header = character;
	vecstr newline;
	unordered_map<string, string> chosenLines;
	unordered_map<string, vecstr> newAnimList = newAnimationList();
	unordered_map<string, unordered_map<int, bool>> hasAddClip;

	{
		string strnum = boost::regex_replace(string(catalyst[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (!isOnlyNumber(strnum) || stoi(strnum) < 10)
		{
			cout << "ERROR(3014): Wrong nemesis_animationdatasinglefile detected. Abort processing. Please reinstall Nemesis" << endl << endl;
			error = true;
			return;
		}

		num = stoi(strnum) + 1;
	}

	newline.reserve(num);

	for (int i = 0; i < num; ++i)
	{
		newline.push_back(catalyst[i]);
	}

	animDataChar.push_back(character);
	animDataHeader[character].push_back(header);
	catalystMap[character][header] = newline;;
	newline.reserve(20);
	newline.clear();

	for (unsigned int j = num; j < catalyst.size(); ++j)
	{
		if (wordFind(catalyst[j], "Characters") == 0)
		{
			character = GetFileName(boost::algorithm::to_lower_copy(catalyst[j]));
			animDataChar.push_back(character);
			header = "$header$";
			animDataHeader[character].push_back(header);
			break;
		}
	}

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 3: " << duration / 1000 << " seconds" << endl;

	// add picked behavior and remove not picked behavior 
	// separation of all items for easier access and better compatibility
	for (unsigned int l = num; l < catalyst.size(); l++)
	{
		string line = catalyst[l];
		bool skip = false;
		
		if (line.find("<!-- ", 0) != string::npos)
		{
			if (line.find("<!-- NEW *", 0) != string::npos)
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
			else if (line.find("<!-- NEW ^", 0) != string::npos)
			{
				special = true;
			}
			else if (line.find("<!-- CLOSE -->", 0) != string::npos)
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
				if (line.find("<!-- *", 0) != string::npos)
				{
					size_t tempint = line.find("<!-- *") + 6;
					string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
					chosenLines[modID] = line;
					break;
				}
				else if (line.find("<!-- original -->", 0) != string::npos)
				{
					if (chosenLines.size() != 0)
					{
						line = behaviorLineChooser(line, chosenLines, behaviorPriority);
						chosenLines.clear();
					}
					else
					{
						cout << ">> ERROR(1165): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
						error = true;
						return;
					}
				}

				if (!end)
				{
					if (l + 3 < catalyst.size() && l > 2)
					{
						if (catalyst[l - 1] == "")
						{
							if (catalyst[l + 3].find("Behaviors") != string::npos && catalyst[l + 3].find(".hkx") == catalyst[l + 3].length() - 4)
							{
								newline.shrink_to_fit();
								catalystMap[character][header] = newline;
								newline.reserve(20);
								newline.clear();
								isInfo = false;

								if (catalyst[l + 3].find("Behaviors\\Behavior00.hkx") != string::npos)
								{
									end = true;
									character = "$end$";
									header = character;
									animDataChar.push_back(character);
									animDataHeader[character].push_back(header);
								}
								else
								{
									for (unsigned int j = l + 4; j < catalyst.size(); ++j)
									{
										if (wordFind(catalyst[j], "Characters") == 0)
										{
											character = GetFileName(boost::algorithm::to_lower_copy(catalyst[j]));
											animDataChar.push_back(character);
											header = "$header$";
											animDataHeader[character].push_back(header);
											break;
										}
									}
								}
							}
							else if (wordFind(catalyst[l - 3], "Characters") == 0)
							{
								if (hasAlpha(line))
								{
									if (isOnlyNumber(catalyst[l + 1]))
									{
										newline.shrink_to_fit();
										catalystMap[character][header] = newline;
										newline.reserve(20);
										newline.clear();
										header = line + " " + catalyst[l + 1];
										animDataHeader[character].push_back(header);
									}
								}
							}
							else if (!isInfo)
							{
								if (wordFind(catalyst[l], "$CLIP$") != -1)
								{
									newline.shrink_to_fit();
									catalystMap[character][header] = newline;;
									newline.reserve(20);
									newline.clear();
									header = "$CLIP$";
									hasAddClip[character][animDataHeader[character].size()] = true;
									animDataHeader[character].push_back(header);
								}
								else if (isOnlyNumber(line))
								{
									isInfo = true;
									newinfo = true;
									newline.shrink_to_fit();
									catalystMap[character][header] = newline;;
									newline.reserve(20);
									newline.clear();
									infoline = animDataHeader[character].size();
									header = "$info header$";
									animDataHeader[character].push_back(header);

								}
								else if (hasAlpha(line))
								{
									if (isOnlyNumber(catalyst[l + 1]))
									{
										newline.shrink_to_fit();
										catalystMap[character][header] = newline;
										newline.reserve(20);
										newline.clear();
										header = line + " " + catalyst[l + 1];
										animDataHeader[character].push_back(header);
									}
								}
							}
							else
							{
								if (wordFind(catalyst[l], "$MDRD$") != -1)
								{
									newline.shrink_to_fit();
									catalystMap[character][header] = newline;;
									newline.reserve(20);
									newline.clear();
									header = "$MDRD$";
									animDataHeader[character].push_back(header);
								}
								else if (isOnlyNumber(line))
								{
									newline.shrink_to_fit();
									catalystMap[character][header] = newline;
									newline.reserve(20);
									newline.clear();
									header = line;
									animDataHeader[character].push_back(header);
								}
							}
						}
						else if (wordFind(catalyst[l - 3], "Characters") == 0)
						{
							if (hasAlpha(line) && l + 1 != catalyst.size())
							{
								if (isOnlyNumber(catalyst[l + 1]))
								{
									newline.shrink_to_fit();
									catalystMap[character][header] = newline;
									newline.reserve(20);
									newline.clear();
									header = line + " " + catalyst[l + 1];
									animDataHeader[character].push_back(header);
								}
							}
						}
					}

					if (newinfo)
					{
						newinfo = false;
						newline.push_back(line);
						newline.shrink_to_fit();
						catalystMap[character][header] = newline;
						newline.reserve(20);
						newline.clear();
						header = catalyst[l + 1];
						animDataHeader[character].push_back(header);
					}
					else
					{
						newline.push_back(line);
					}
				}
				else
				{
					newline.push_back(line);
				}

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
		catalystMap[character][header] = newline;
		newline.clear();
	}

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 4: " << duration / 1000 << " seconds" << endl;

	for (unsigned int i = 0; i < animDataChar.size(); ++i)
	{
		character = animDataChar[i];

		if (character != "$end$")
		{
			vector<clipinfo> data;
			bool start = false;
			bool hasNewClip = false;
			int lastID = 0;
			int infoline = 0;

			if (hasAddClip.find(character) != hasAddClip.end())
			{
				hasNewClip = true;
			}

			for (unsigned int j = 0; j < animDataHeader[character].size(); ++j)
			{
				header = animDataHeader[character][j];
				newline.reserve(catalystMap[character][header].size() + memory * 5);
				int type = 0;

				if (header == "$header$")
				{
					type = 1;
				}
				else if (header == "$info header$")
				{
					infoline = j;
				}
				else if (header.find(" ") != string::npos)
				{
					type = 2;
				}
				else if (isOnlyNumber(header))
				{
					type = 3;
				}

				if (type > 1 && hasNewClip)
				{
					for (unsigned int p = 0; p < catalystMap[character][header].size(); ++p)
					{
						if (AnimDataPosition(catalystMap[character][header], character, header, "not available", p, type, true) == AnimDataFormat::uniquecode)
						{
							int tempID = stoi(catalystMap[character][header][p]);

							if (tempID <= lastID)
							{
								lastID = tempID + 1;
							}

							break;
						}
					}
				}
				
				if (header == "$CLIP$")
				{
					for (unsigned int k = 0; k < animationList.size(); ++k)
					{
						for (unsigned int l = 0; l < newAnimList[character].size(); ++l)
						{
							if (animationList[k]->animInfo.find(newAnimList[character][l]) != animationList[k]->animInfo.end())
							{
								for (unsigned int j = 0; j < animationList[k]->animInfo[newAnimList[character][l]].size(); ++j)
								{
									if (animationList[k]->animInfo[newAnimList[character][l]][j]->motionData.size() != 0)
									{
										data.push_back(*new clipinfo);
										string templine = animationList[k]->animInfo[newAnimList[character][l]][j]->motionData.back();
										data.back().duration = stoi(templine.substr(0, templine.find(" ")));
										data.back().motiondata = animationList[k]->animInfo[newAnimList[character][l]][j]->motionData;
										data.back().rotationdata = animationList[k]->animInfo[newAnimList[character][l]][j]->rotationData;

										vecstr tempclip = animationList[k]->animInfo[newAnimList[character][l]][j]->clipname;

										// format:
										// animation clip name
										// unique code
										// constant 1
										// constant 0
										// constant 0
										// event name count

										if (tempclip.size() > 0)
										{
											for (unsigned int p = 0; p < tempclip.size(); ++p)
											{
												data.back().uniqueID.push_back(lastID);		// record unique code
												newline.push_back(tempclip[p]);
												newline.push_back(to_string(lastID));
												newline.push_back("1");
												newline.push_back("0");
												newline.push_back("0");
												newline.push_back("0");
												newline.push_back("");		// end
												++lastID;
											}
										}
										else
										{
											string clipname = newAnimList[character][l] + "_" + animationList[k]->animInfo[newAnimList[character][l]][j]->mainAnimEvent;

											data.back().uniqueID.push_back(lastID);		// record unique code
											newline.push_back(clipname);
											newline.push_back(to_string(lastID));
											newline.push_back("1");
											newline.push_back("0");
											newline.push_back("0");
											newline.push_back("0");
											newline.push_back("");		// end
											++lastID;
										}
									}
								}
							}
						}
					}

					// add AA anim data; personally have no idea why it is done but it is done in FNIS so i keep this feature in to ensure that generated file
					// will be as close as to FNIS generated file
					for (auto it = alternateAnim.begin(); it != alternateAnim.end(); ++it)
					{
						for (unsigned int p = 0; p < it->second.size(); ++p)
						{
							if (it->second[p] != "x")
							{
								newline.push_back(GetFileName(it->second[p]));
								newline.push_back("1");
								newline.push_back("0");
								newline.push_back("0");
								newline.push_back("0");
								newline.push_back("");
							}
						}
					}

					newline.shrink_to_fit();
					catalystMap[character][header] = newline;
					newline.clear();
				}
				else if (header == "$MDRD$")
				{
					// format:
					// unique code
					// clip length
					// motion data count
					// motion data list
					// rotation data count
					// rotation data list

					for (unsigned int l = 0; l < data.size(); ++l)
					{
						for (unsigned int k = 0; k < data[l].uniqueID.size(); ++k)
						{
							newline.push_back(to_string(data[l].uniqueID[k]));			// unique code
							newline.push_back(to_string(data[l].duration));				// clip length
							newline.push_back(to_string(data[l].motiondata.size()));	// motion data count

							for (unsigned int k = 0; k < data[l].motiondata.size(); ++k)
							{
								newline.push_back(data[l].motiondata[k]);		// motion data list
							}

							newline.push_back(to_string(data[l].rotationdata.size()));	// rotation data list

							for (unsigned int k = 0; k < data[l].rotationdata.size(); ++k)
							{
								newline.push_back(data[l].rotationdata[k]);		// rotation data list
							}

							newline.push_back("");
						}
					}

					// AA animation doesn't have animation driven function hence omitted
					if (false != false)
					{
						for (auto it = alternateAnim.begin(); it != alternateAnim.end(); ++it)
						{
							// empty
						}
					}

					newline.shrink_to_fit();
					catalystMap[character][header] = newline;
					newline.clear();
				}
				else
				{
					bool startCount = false;
					AnimDataFormat::position pos;
					int elementline;
					int counter;

					for (unsigned int p = 0; p < catalystMap[character][header].size(); ++p)
					{
						string line = catalystMap[character][header][p];

						if (line.find("$elements$") != string::npos)
						{
							if (startCount)
							{
								cout << "ERROR(3015): Multiple $elements$ running concurrently detected. Please contact the template creator" << endl << "Character: " << character << endl << "Header: " << header << endl << "Line: " << p + 1 << endl << endl;
								error = true;
								return;
							}

							pos = AnimDataPosition(catalystMap[character][header], character, header, "not available", p, type);

							if (pos != AnimDataFormat::behaviorfilecount && pos != AnimDataFormat::eventnamecount && pos != AnimDataFormat::motiondatacount && pos != AnimDataFormat::rotationdatacount)
							{
								cout << "ERROR(3016): Current line cannot be used for $elements$. Please contact the template creator" << endl << "Character: " << character << endl << "Header: " << header << endl << "Line: " << p + 1 << endl << endl;
								error = true;
								return;
							}

							elementline = p;
							startCount = true;
							counter = 0;
						}

						if (startCount)
						{
							if (p != AnimDataPosition(catalystMap[character][header], character, header, "not available", p, type, true))
							{
								startCount = false;
								catalystMap[character][header][elementline] = to_string(counter);
							}
							else
							{
								++counter;
							}
						}

						newline.push_back(line);
					}

					newline.shrink_to_fit();
					catalystMap[character][header] = newline;
					newline.clear();
				}
			}

			if (catalystMap[character].find("$info header$") != catalystMap[character].end())
			{
				int counter = 0;

				for (unsigned int j = 0; j < infoline; ++j)
				{
					counter = counter + int(catalystMap[character][animDataHeader[character][j]].size());
				}

				catalystMap[character]["$header$"][0] = to_string(counter - 1);
				counter = 0;

				for (unsigned int j = infoline; j < animDataHeader[character].size(); ++j)
				{
					counter = counter + int(catalystMap[character][animDataHeader[character][j]].size());
				}

				catalystMap[character]["$info header$"][0] = to_string(counter - 1);
			}
			else
			{
				int counter = 0;

				for (unsigned int j = 0; j < animDataHeader[character].size(); ++j)
				{
					counter = counter + int(catalystMap[character][animDataHeader[character][j]].size());
				}

				catalystMap[character]["$header$"][0] = to_string(counter - 1);
			}
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

		for (unsigned int i = 0; i < animDataChar.size(); ++i)
		{
			string character = animDataChar[i];

			for (unsigned int j = 0; j < animDataHeader[character].size(); ++j)
			{
				string header = animDataHeader[character][j];

				for (unsigned int k = 0; k < catalystMap[character][header].size(); ++k)
				{
					fwriter << catalystMap[character][header][k] << "\n";
				}
			}
		}

		output.close();
	}
	else
	{
		cout << "ERROR(1025): Fail to output xml file" << endl << "File: " << filename << endl << endl;
		error = true;
		return;
	}

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 6: " << duration / 1000 << " seconds" << endl;

}

unordered_map<string, vecstr> newAnimationList()
{
	unordered_map<string, vecstr> list;				// character hkx file, list of templatecode
	vecstr templatecode;
	vecstr behaviorlist;
	string curdirectory = "behavior templates\\";
	read_directory(curdirectory, templatecode);

	for (unsigned int i = 0; i < templatecode.size(); ++i)
	{
		curdirectory = curdirectory + templatecode[i];
		boost::filesystem::path curTemplate(curdirectory);

		if (boost::filesystem::is_directory(curTemplate))
		{
			read_directory(curdirectory, behaviorlist);

			for (unsigned int j = 0; j < behaviorlist.size(); ++j)
			{
				boost::filesystem::path curBehavior(curdirectory + behaviorlist[j]);

				if (boost::filesystem::is_directory(curBehavior))
				{
					string lowercase = boost::algorithm::to_lower_copy(behaviorlist[j]);

					for (unsigned int k = 0; k < behaviorJoints[lowercase].size(); ++k)
					{
						list[behaviorJoints[lowercase][k]].push_back(templatecode[i]);
					}
				}
			}
		}
	}

	return list;
}

AnimDataFormat::position AnimDataPosition(vecstr animData, string character, string header, string modcode, int linecount, int type, bool silentError)
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
		if (animData[i].find("<!-- ") != string::npos)
		{
			if (functionstart == -1)
			{
				functionstart = i;
			}

			marker[i].skip = true;

			if (animData[i].find("<!-- CLOSE -->") != string::npos)
			{
				isOpen = false;
			}
			else if (animData[i].find("<!-- CONDITION END -->") != string::npos)
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

		if (animData[i].find("<!-- ") != string::npos)
		{
			if (animData[i].find("<!-- NEW") != string::npos)
			{
				isOpen = true;
			}
			else if (animData[i].find("<!-- CONDITION START ^") != string::npos)
			{
				++conditionOpen;
				isCondition[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION -->") != string::npos)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION ^") != string::npos)
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
			if (linecount < 3 && functionstart > 2)
			{
				return AnimDataConvert(type, linecount + 1, silentError);
				// 1. total line count
				// 2. unknown number
				// 3. behavior file count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != string::npos)
				{
					if (!silentError)
					{
						cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
						error = true;
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
								if (!silentError)
								{
									cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
									error = true;
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
					return AnimDataConvert(type, id, silentError);
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
				return AnimDataConvert(type, linecount + 1, silentError);
				// 1. animation name (clip)
				// 2. unique code
				// 3. unknown number (1)
				// 4. unknown number (0)
				// 5. unknown number (0)
				// 6. event name count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != string::npos)
				{
					if (!silentError)
					{
						cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
						error = true;
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
								cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
								error = true;
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
					return AnimDataConvert(type, id, silentError);
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
				return AnimDataConvert(type, linecount + 1, silentError);
				// 1. unique code
				// 2. total animation (clip) length
				// 3. motion data count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != string::npos)
				{
					if (!silentError)
					{
						cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
						error = true;
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
								if (!silentError)
								{
									cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
									error = true;
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
					return AnimDataConvert(type, id, silentError);
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
							cout << "ERROR(3007): Invalid function call. Function call cannot be use in this line for it is not in line with the format. Please contact the template creator" << endl << "Template: " << modcode << endl << "Line: " << linecount << endl << endl;
							error = true;
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
				return AnimDataConvert(type, id, silentError);
				// 4. motion data list
				// 5. rotation data count
				// 6. rotation data list
			}
		}
	}

	if (!silentError)
	{
		cout << "ERROR(3005): Invalid format for \"animationdatasinglefile.txt\" detected. Please contact the template creator" << endl << "Character: " << character << endl << "Header: " << header << endl << endl;
		error = true;
	}

	return xerror;
}

AnimDataFormat::position AnimDataConvert(int type, int position, bool silentError)
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
