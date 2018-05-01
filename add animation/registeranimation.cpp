#include "registeranimation.h"

using namespace std;

registerAnimation::registerAnimation(string curDirectory, string filename, getTemplate behaviortemplate, string behaviorpath, string behavior, bool isNemesis)
{
	if (isNemesis)
	{
		modID = filename.substr(8, filename.find("_List.txt") - 8);
	}
	else
	{
		modID = filename.substr(5, filename.find("_List.txt") - 5);
	}
	
	string filepath = curDirectory + filename;

	{
		size_t nextpos = wordFind(filepath, "\\animations\\", true);

		if(nextpos == -1)
		{
			cout << ">> ERROR(1081): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
			error = true;
			return;
		}
		
		behaviorFile = behavior;
	}

	FILE* newAnimationList;
	fopen_s(&newAnimationList, filepath.c_str(), "r");
	
	if (newAnimationList)
	{
		char line[2000];
		int linecount = 0;
		int multiCount = -1;
		string previousShortline;
		string previousLine;
		string curAAprefix;
		unordered_map<string, bool> AAAnimFileExist;
		unordered_map<string, bool> AAprefixExist;

		while (fgets(line, 2000, newAnimationList))
		{
			++linecount;

			if (line[0] != '\'' && line[0] != '\n' && strlen(line) != 0)
			{
				if (line[strlen(line) - 1] == '\n')
				{
					line[strlen(line) - 1] = '\0';
				}

				stringstream sstream(line);
				istream_iterator<string> ssbegin(sstream);
				istream_iterator<string> ssend;
				vecstr newAnimInfo(ssbegin, ssend);
				copy(newAnimInfo.begin(), newAnimInfo.end(), newAnimInfo.begin());

				if (newAnimInfo[0].length() == 0)
				{
					cout << "ERROR(1017): Unable to read line" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
					error = true;
					fclose(newAnimationList);
					return;
				}

				if (boost::iequals(newAnimInfo[0], "Version"))
				{
					if (boost::iequals(newAnimInfo[1].substr(0, 1), "V"))
					{
						version = newAnimInfo[1];
					}
					else
					{
						version = "V" + newAnimInfo[1];
					}
				}
				else if (boost::iequals(newAnimInfo[0], "AAprefix"))
				{
					if (string(line).substr(wordFind(line, "AAprefix") + 9).length() > 3 || newAnimInfo[1].length() != 3 || newAnimInfo.size() != 2)
					{
						cout << "ERROR(4002): Invalid AAprefix. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "AAset: " << line << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}

					curAAprefix = newAnimInfo[1];

					if (!AAprefixExist[curAAprefix])
					{
						if (AAprefixExist.size() > 30)
						{
							cout << "ERROR(4007): Alternate Animation has exceeded supported 30 mods limit. Please disable some mods that use alternate animation" << endl << "File: " << filename << "Line: " << linecount << "Mod: " << modID << endl << "AAprefix: " << newAnimInfo[1] << endl << endl;
							error = true;
							fclose(newAnimationList);
							return;
						}

						AAprefixExist[curAAprefix] = true;
					}
				}
				else if (boost::iequals(newAnimInfo[0], "AAset"))
				{
					if (curAAprefix.length() == 0)
					{
						cout << "ERROR(4006): Missing AAprefix. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "AAset: " << line << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}

					if (newAnimInfo.size() != 3 )
					{
						cout << "ERROR(4003): Invalid AAset. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "AAset: " << line << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}

					size_t groupSize = groupAA[newAnimInfo[1]].size();

					if (groupSize == 0)
					{
						cout << "ERROR(4004): Unknown AAset group detected. Registration required in \"alternate_animation_list.txt\" Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "AAset: " << line << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}
					
					if (newAnimInfo[2].length() > 0)
					{
						for (unsigned int i = 0; i < newAnimInfo[2].length(); ++i)
						{
							if (isalpha(newAnimInfo[2][i]) || !isalnum(newAnimInfo[2][i]))
							{
								cout << "ERROR(4005): Invalid AAset input. Only number is acceptable for the 3rd element. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "AAset: " << line << endl << endl;
								error = true;
								fclose(newAnimationList);
								return;
							}
						}
					}

					string lowGroupName = boost::algorithm::to_lower_copy(newAnimInfo[1]);
					int num = stoi(newAnimInfo[2]);
					AAGroupCount[curAAprefix][boost::algorithm::to_lower_copy(newAnimInfo[1])] = num;
					groupAAPrefix[boost::algorithm::to_lower_copy(newAnimInfo[1])].push_back(curAAprefix);

					for (unsigned int i = 0; i < groupSize; ++i)		// list of animations in the animation group
					{
						string lowAnim = groupAA[lowGroupName][i];

						for (int j = 0; j < num; ++j)					// number of variation of the specific animation
						{
							string animFile = curAAprefix + to_string(j) + "_" + lowAnim;

							if (!isFileExist(curDirectory + animFile))
							{
								cout << "WARNING: Alternate animation file doesn't exist. Current alternate animation file will not work" << endl << "Mod: " << modID << endl << "Animation File: " << animFile << endl << endl;
								alternateAnim[lowAnim].push_back("x");
							}
							else
							{
								alternateAnim[lowAnim].push_back(modID + "\\" + animFile);
								AAAnimFileExist[animFile] = true;
							}
						}

						if (alternateAnim[lowAnim].size() > 128)
						{
							cout << "ERROR(4008): Alternate Animation has exceeded supported 128 sets of animation limit. Please disable some mods that use alternate animation" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "AAset: " << line << endl << endl;
							error = true;
							fclose(newAnimationList);
							return;
						}
					}
				}
				else if (boost::iequals(newAnimInfo[0], "T"))
				{
					if (newAnimInfo.size() < 4 || newAnimInfo.size() % 2 != 0)
					{
						cout << "ERROR(4010): Missing input regarding Alternate Animation. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}

					if (!AAAnimFileExist[newAnimInfo[1]])
					{
						cout << "ERROR(4011): Alternate animation file has not been registered. Please contact the mod authork" << endl << "File: " << filename << endl << "Animation File: " << newAnimInfo[1] << endl << endl;
						error = true;
						return;
					}
					else
					{
						string lowerAnimName = boost::algorithm::to_lower_copy(newAnimInfo[1]);
						string originalAnim = lowerAnimName.substr(lowerAnimName.find("_") + 1);
						
						for (unsigned int j = 2; j < newAnimInfo.size(); ++j)
						{
							AAEvent[lowerAnimName].push_back(newAnimInfo[j]);

							if (j == 2)
							{
								AAHasEvent[originalAnim].push_back(lowerAnimName);
							}
						}
					}
				}
				else if (boost::iequals(newAnimInfo[0], "AnimVar"))
				{
					string strline(line);
					size_t nextpos = 0;
					size_t num = count(strline.begin(), strline.end(), ' ');
					string name;
					string type;
					string value;

					for (unsigned int i = 0; i < num; ++i)
					{
						size_t position = strline.find(" ", nextpos) + 1;
						nextpos = strline.find(" ", position);
						string section = strline.substr(position, nextpos - position);

						if (i == 0)
						{
							name = section;
						}
						else if (i == 1)
						{
							type = section;
						}
						else if (i == 2)
						{
							section.erase(remove_if(section.begin(), section.end(), isspace), section.end());
							string temp = boost::regex_replace(string(section), boost::regex("[^0-9]*([0-9]+(\\.([0-9]+)?)?).*"), string("\\1"));

							if (temp == section && isalnum(temp[0]) && !isalpha(temp[0]))
							{
								value = section;
							}
							else
							{
								cout << "ERROR(1030): Invalid element. Please enter valid numerical character" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "Element: " << strline << endl << endl;
								error = true;
								fclose(newAnimationList);
								return;
							}
						}
						else
						{
							cout << "ERROR(1023): Too many elements" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "Element: " << strline << endl << endl;
							error = true;
							fclose(newAnimationList);
							return;
						}
					}

					var variable(type, value);

					if (!error)
					{
						AnimVar[name] = variable;
					}
					else
					{
						cout << "ERROR(1018): Unknown Variable Type. Only \"BOOL\", \"INT32\" and \"REAL\" are supported" << endl << "File: " << filename << endl << "Line: " << linecount << endl << "Variable: " << type << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}
				}
				else if (newAnimInfo[0] == "MD")
				{
					if (newAnimInfo.size() != 5)
					{
						cout << "ERROR(1093): Motion data only accepts 4 numerical values. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}

					for (unsigned int i = 1; i < newAnimInfo.size(); ++i)
					{
						for (unsigned int j = 0; j < newAnimInfo[i].length(); ++j)
						{
							if ((isalpha(newAnimInfo[i][j]) || !isalnum(newAnimInfo[i][j])) && newAnimInfo[i][j] != ' ')
							{
								cout << "ERROR(1091): Invalid motion data character. Only number character is acceptable. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
								error = true;
								return;
							}
						}
					}

					if (animInfo[previousShortline].back()->motionData.size() > 0)
					{
						int timer = stoi(animInfo[previousShortline].back()->motionData.back().substr(0, animInfo[previousShortline].back()->motionData.back().find(" ")));

						if (timer >= stoi(newAnimInfo[1]))
						{
							cout << "ERROR(1092): Invalid motion data time frame. Previous motion data time frame must be before the current motion time frame" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
							error = true;
							return;
						}
					}

					animInfo[previousShortline].back()->motionData.push_back(newAnimInfo[1] + " " + newAnimInfo[2] + " " + newAnimInfo[3] + " " + newAnimInfo[4]);
				}
				else if (newAnimInfo[0] == "RD")
				{
					if (newAnimInfo.size() != 6)
					{
						cout << "ERROR(1093): Rotation data only accepts 5 numerical values. Please contact the mod author" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}

					if (animInfo[previousShortline].back()->motionData.size() == 0)
					{
						cout << "ERROR(1094): Missing motion data. Rotation data is only available right after at least 1 motion data has been attached to the animation" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}

					if (animInfo[previousShortline].back()->rotationData.size() > 0)
					{
						int timer = stoi(animInfo[previousShortline].back()->rotationData.back().substr(0, animInfo[previousShortline].back()->rotationData.back().find(" ")));

						if (timer >= stoi(newAnimInfo[1]))
						{
							cout << "ERROR(1093): Invalid rotation data time frame. Previous motion data time frame must be before the current rotation time frame" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
							error = true;
							return;
						}
					}

					animInfo[previousShortline].back()->rotationData.push_back(newAnimInfo[1] + " " + newAnimInfo[2] + " " + newAnimInfo[3] + " " + newAnimInfo[4] + " " + newAnimInfo[5]);
				}
				else if (newAnimInfo[0] == "+")
				{
					if (previousShortline.length() > 0)
					{
						if (behaviortemplate.templatelist[previousShortline])
						{
							string anim = newAnimInfo[newAnimInfo.size() - 1];
							bool isOExist = true;

							if (anim[anim.length() - 2] == '/' && (anim[anim.length() - 1] == '1' || anim[anim.length() - 1] == '2'))
							{
								isOExist = false;
							}

							if (newAnimInfo.size() > 3 && newAnimInfo[1][0] == '-')
							{
								animInfo[previousShortline].push_back(make_shared<animationInfo>(newAnimInfo, filename, behaviortemplate.optionlist[previousShortline], linecount, isOExist));

								if (error)
								{
									fclose(newAnimationList);
									return;
								}
								
								animInfo[previousShortline].back()->addFilename(newAnimInfo[3]);

								if (!isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[3]))
								{
									cout << "WARNING: Missing animation file" << endl << "File: " << newAnimInfo[3] << endl << endl;
								}
							}
							else
							{
								animInfo[previousShortline].push_back(make_shared<animationInfo>(newAnimInfo, filename, behaviortemplate.optionlist[previousShortline], linecount, isOExist, true));

								if (error)
								{
									fclose(newAnimationList);
									return;
								}
								
								animInfo[previousShortline].back()->addFilename(newAnimInfo[2]);

								if (!isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[2]))
								{
									cout << "WARNING: Missing animation file" << endl << "File: " << newAnimInfo[2] << endl << endl;
								}
							}

							if (behaviortemplate.behaviortemplate.find(previousShortline + "_group") != behaviortemplate.behaviortemplate.end() && behaviortemplate.behaviortemplate[previousShortline + "_group"].size() != 0)
							{
								animInfo[previousShortline].back()->optionPickedCount[previousShortline + "_group"] = 1;
							}

							if (behaviortemplate.behaviortemplate.find(previousShortline + "_master") != behaviortemplate.behaviortemplate.end() && behaviortemplate.behaviortemplate[previousShortline + "_master"].size() != 0)
							{
								animInfo[previousShortline].back()->optionPickedCount[previousShortline + "_master"] = 1;
							}

							if (!isOExist)
							{
								cout << "ERROR(1000): \"o\" option not found. \"o\" is required for AnimObject" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
								error = true;
								fclose(newAnimationList);
								return;
							}

							templateType[previousShortline]++;
							isMulti[previousShortline].push_back(multiCount);
							multiCount++;
						}
					}
					else
					{
						cout << "ERROR(1065): Missing animation type detected" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}
				}
				else if (behaviortemplate.templatelist[newAnimInfo[0]])
				{
					string anim = newAnimInfo[newAnimInfo.size() - 1];
					bool isOExist = true;

					if (anim[anim.length() - 2] == '/' && (anim[anim.length() - 1] == '1' || anim[anim.length() - 1] == '2'))
					{
						isOExist = false;
					}

					if (newAnimInfo.size() > 3 && newAnimInfo[1][0] == '-')
					{
						animInfo[newAnimInfo[0]].push_back(make_shared<animationInfo>(newAnimInfo, filename, behaviortemplate.optionlist[newAnimInfo[0]], linecount, isOExist));
						animInfo[newAnimInfo[0]].back()->addFilename(newAnimInfo[3]);

						if (!isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[3]))
						{
							cout << "WARNING: Missing animation file" << endl << "File: " << newAnimInfo[3] << endl << endl;
						}
					}
					else
					{
						animInfo[newAnimInfo[0]].push_back(make_shared<animationInfo>(newAnimInfo, filename, behaviortemplate.optionlist[newAnimInfo[0]], linecount, isOExist, true));
						animInfo[newAnimInfo[0]].back()->addFilename(newAnimInfo[2]);

						if (!isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[2]))
						{
							cout << "WARNING: Missing animation file" << endl << "File: " << newAnimInfo[2] << endl << endl;
						}
					}

					if (error)
					{
						fclose(newAnimationList);
						return;
					}

					if (behaviortemplate.behaviortemplate.find(newAnimInfo[0] + "_group") != behaviortemplate.behaviortemplate.end() && behaviortemplate.behaviortemplate[newAnimInfo[0] + "_group"].size() != 0)
					{
						animInfo[newAnimInfo[0]].back()->optionPickedCount[newAnimInfo[0] + "_group"] = 1;
					}

					if (behaviortemplate.behaviortemplate.find(newAnimInfo[0] + "_master") != behaviortemplate.behaviortemplate.end() && behaviortemplate.behaviortemplate[newAnimInfo[0] + "_master"].size() != 0)
					{
						animInfo[newAnimInfo[0]].back()->optionPickedCount[newAnimInfo[0] + "_master"] = 1;
					}

					if (!isOExist)
					{
						cout << "ERROR(1000): \"o\" option not found. \"o\" is required for AnimObject" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						fclose(newAnimationList);
						return;
					}

					if (previousShortline.length() > 0)
					{
						if (behaviortemplate.optionlist[previousShortline].groupMin > multiCount)
						{
							cout << "ERROR(1034): Insufficient animation in an animation group. At least " << behaviortemplate.optionlist[previousShortline].groupMin << " animations are required for " << previousShortline << " type animation. Add more animations using (+) in the group" << endl << "File: " << filename << endl << "Line: " << linecount - 1 << endl << endl;
							error = true;
							fclose(newAnimationList);
							return;
						}

						for (int i = 0; i < multiCount; ++i)
						{
							last[previousShortline].push_back(multiCount - 1);

							if (i == multiCount - 1 && i != 0)
							{
								if (behaviortemplate.optionlist[previousShortline].ruleTwo.size() != 0)
								{
									if (previousLine[2] == '-')
									{
										stringstream sstream(previousLine);
										istream_iterator<string> ssbegin(sstream);
										istream_iterator<string> ssend;
										vecstr AnimInfo(ssbegin, ssend);
										copy(AnimInfo.begin(), AnimInfo.end(), AnimInfo.begin());

										string templine = AnimInfo[1];
										templine = templine.substr(1);
										vecstr curList;
										size_t numOption = count(templine.begin(), templine.end(), ',') + 1;
										size_t nextOption;
										size_t pos = 0;

										for (unsigned int i = 0; i < numOption; ++i)
										{
											nextOption = templine.find(",", pos);

											if (nextOption != -1)
											{
												curList.push_back(templine.substr(pos, nextOption - pos));
												pos = nextOption + 1;
											}
											else
											{
												curList.push_back(templine.substr(pos));
											}
										}

										size_t ruleTwoSize = behaviortemplate.optionlist[previousShortline].ruleTwo.size();
										vecstr* rule = &behaviortemplate.optionlist[previousShortline].ruleTwo;

										for (size_t i = 0; i < ruleTwoSize; ++i)
										{
											bool isMatched = false;

											for (unsigned int j = 0; j < curList.size(); ++j)
											{
												bool matching = false;
												string number = boost::regex_replace(string(curList[j]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

												if (curList[j].length() == (*rule)[i].length() || (behaviortemplate.optionlist[previousShortline].addOn[(*rule)[i]].size() != 0 && curList[j].length() >(*rule)[i].length()) || curList[j] == (*rule)[i] + number)
												{
													for (unsigned int k = 0; k < (*rule)[i].length(); ++k)
													{
														if (k != (*rule)[i].length() - 1)
														{
															if (curList[j][k] == (*rule)[i][k])
															{
																matching = true;
															}
															else
															{
																matching = false;
																break;
															}
														}
														else
														{
															if (curList[j][k] == (*rule)[i][k])
															{
																matching = true;
															}

															break;
														}
													}
												}

												if (matching)
												{
													isMatched = true;
													break;
												}
											}

											if (!isMatched)
											{
												cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
												error = true;
												fclose(newAnimationList);
												return;
											}
										}
									}
									else
									{
										cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
										error = true;
										fclose(newAnimationList);
										return;
									}
								}
							}
						}
					}

					if (behaviortemplate.optionlist[newAnimInfo[0]].ruleOne.size() != 0 || behaviortemplate.optionlist[newAnimInfo[0]].compulsory.size() != 0)
					{
						if (line[newAnimInfo[0].length() + 1] == '-')
						{
							stringstream sstream(line);
							istream_iterator<string> ssbegin(sstream);
							istream_iterator<string> ssend;
							vecstr AnimInfo(ssbegin, ssend);
							copy(AnimInfo.begin(), AnimInfo.end(), AnimInfo.begin());

							string templine = AnimInfo[1];
							templine = templine.substr(1);
							vecstr curList;
							size_t numOption = count(templine.begin(), templine.end(), ',') + 1;
							size_t nextOption;
							size_t pos = 0;

							for (size_t i = 0; i < numOption; ++i)
							{
								nextOption = templine.find(",", pos);

								if (nextOption != -1)
								{
									curList.push_back(templine.substr(pos, nextOption - pos));
									pos = nextOption + 1;
								}
								else
								{
									curList.push_back(templine.substr(pos));
								}
							}

							size_t compulsorySize = behaviortemplate.optionlist[newAnimInfo[0]].compulsory.size();
							vecstr* compulsory = &behaviortemplate.optionlist[newAnimInfo[0]].compulsory;

							for (size_t i = 0; i < compulsorySize; ++i)
							{
								bool isMatched = false;

								for (unsigned int j = 0; j < curList.size(); ++j)
								{
									bool matching = false;
									string number = boost::regex_replace(string(curList[j]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (curList[j].length() == (*compulsory)[i].length() || (behaviortemplate.optionlist[newAnimInfo[0]].addOn[(*compulsory)[i]].size() != 0 && curList[j].length() >(*compulsory)[i].length()) || curList[j] == (*compulsory)[i] + number)
									{
										for (unsigned int k = 0; k < (*compulsory)[i].length(); ++k)
										{
											if (k != (*compulsory)[i].length() - 1)
											{
												if (curList[j][k] == (*compulsory)[i][k])
												{
													matching = true;
												}
												else
												{
													matching = false;
													break;
												}
											}
											else
											{
												if (curList[j][k] == (*compulsory)[i][k])
												{
													matching = true;
												}

												break;
											}
										}
									}

									if (matching)
									{
										isMatched = true;
										break;
									}
								}

								if (!isMatched)
								{
									cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
									error = true;
									fclose(newAnimationList);
									return;
								}
							}

							size_t ruleOneSize = behaviortemplate.optionlist[newAnimInfo[0]].ruleOne.size();
							vecstr* rule = &behaviortemplate.optionlist[newAnimInfo[0]].ruleOne;

							for (size_t i = 0; i < ruleOneSize; ++i)
							{
								bool isMatched = false;

								for (unsigned int j = 0; j < curList.size(); ++j)
								{
									bool matching = false;
									string number = boost::regex_replace(string(curList[j]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (curList[j].length() == (*rule)[i].length() || (behaviortemplate.optionlist[newAnimInfo[0]].addOn[(*rule)[i]].size() != 0 && curList[j].length() >(*rule)[i].length()) || curList[j] == (*rule)[i] + number)
									{
										for (unsigned int k = 0; k < (*rule)[i].length(); ++k)
										{
											if (k != (*rule)[i].length() - 1)
											{
												if (curList[j][k] == (*rule)[i][k])
												{
													matching = true;
												}
												else
												{
													matching = false;
													break;
												}
											}
											else
											{
												if (curList[j][k] == (*rule)[i][k])
												{
													matching = true;
												}

												break;
											}
										}
									}

									if (matching)
									{
										isMatched = true;
										break;
									}
								}

								if (!isMatched)
								{
									cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
									error = true;
									fclose(newAnimationList);
									return;
								}
							}
						}
						else
						{
							cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
							error = true;
							fclose(newAnimationList);
							return;
						}
					}

					multiCount = 0;
					templateType[newAnimInfo[0]]++;
					isMulti[newAnimInfo[0]].push_back(multiCount);
					previousShortline = newAnimInfo[0];
					multiCount++;
				}
				else
				{
					cout << "ERROR(1016): Unable to read line" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
					error = true;
					fclose(newAnimationList);
					return;
				}

				previousLine = line;
			}
		}

		++linecount;
		fclose(newAnimationList);

		if (behaviortemplate.optionlist[previousShortline].groupMin > multiCount)
		{
			cout << "ERROR(1034): Insufficient animation in an animation group. At least " << behaviortemplate.optionlist[previousShortline].groupMin << " animations are required for " << previousShortline << " type animation. Add more animations using (+) in the group" << endl << "File: " << filename << endl << "Line: " << linecount - 1 << endl << endl;
			error = true;
			return;
		}

		for (int i = 0; i < multiCount; ++i)
		{
			last[previousShortline].push_back(multiCount - 1);

			if (i == multiCount - 1 && i != 0)
			{
				if (behaviortemplate.optionlist[previousShortline].ruleTwo.size() != 0)
				{
					string tempPreviousShortline;

					if (multiCount > 1)
					{
						tempPreviousShortline = "+";
					}
					else
					{
						tempPreviousShortline = previousShortline;
					}

					if (line[tempPreviousShortline.length() + 1] == '-')
					{
						stringstream sstream(line);
						istream_iterator<string> ssbegin(sstream);
						istream_iterator<string> ssend;
						vecstr AnimInfo(ssbegin, ssend);
						copy(AnimInfo.begin(), AnimInfo.end(), AnimInfo.begin());

						string templine = AnimInfo[1];
						templine = templine.substr(1);
						vecstr curList;
						size_t numOption = count(templine.begin(), templine.end(), ',') + 1;
						size_t nextOption;
						size_t pos = 0;

						for (unsigned int i = 0; i < numOption; ++i)
						{
							nextOption = templine.find(",", pos);

							if (nextOption != -1)
							{
								curList.push_back(templine.substr(pos, nextOption - pos));
								pos = nextOption + 1;
							}
							else
							{
								curList.push_back(templine.substr(pos));
							}
						}

						size_t ruleTwoSize = behaviortemplate.optionlist[previousShortline].ruleTwo.size();
						vecstr* rule = &behaviortemplate.optionlist[previousShortline].ruleTwo;

						for (unsigned int i = 0; i < ruleTwoSize; ++i)
						{
							bool isMatched = false;

							for (unsigned int j = 0; j < curList.size(); ++j)
							{
								bool matching = false;
								string number = boost::regex_replace(string(curList[j]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								if (curList[j].length() == (*rule)[i].length() || (behaviortemplate.optionlist[previousShortline].addOn[(*rule)[i]].size() != 0 && curList[j].length() >(*rule)[i].length()) || curList[j] == (*rule)[i] + number)
								{
									for (unsigned int k = 0; k < (*rule)[i].length(); ++k)
									{
										if (k != (*rule)[i].length() - 1)
										{
											if (curList[j][k] == (*rule)[i][k])
											{
												matching = true;
											}
											else
											{
												matching = false;
												break;
											}
										}
										else
										{
											if (curList[j][k] == (*rule)[i][k])
											{
												matching = true;
											}

											break;
										}
									}
								}

								if (matching)
								{
									isMatched = true;
									break;
								}
							}

							if (!isMatched)
							{
								cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
								error = true;
								return;
							}
						}
					}
					else
					{
						cout << "ERROR(1035): Missing required option" << endl << "File: " << filename << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
				}
			}
		}
	}
	else
	{
		cout << "ERROR(1004): Unable to open file" << endl << "File: " << filename << endl << endl;
		error = true;
	}
}

void registerAnimation::clear()
{
	AnimVar.clear();
	templateType.clear();
	isMulti.clear();
	last.clear();
}

var::var(string inputtype, string inputvalue)
{
	if (boost::iequals(inputtype, "bool"))
	{
		var_type = "BOOL";
	}
	else if (boost::iequals(inputtype, "int32"))
	{
		var_type = "INT32";
	}
	else if (boost::iequals(inputtype, "real"))
	{
		var_type = "REAL";
	}
	else
	{
		error = true;
	}

	init_value = inputvalue;
}
