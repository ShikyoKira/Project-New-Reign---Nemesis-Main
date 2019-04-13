#include "registeranimation.h"
#include "functions\readtextfile.h"
#include "Nemesis Main GUI\src\utilities\wstrconvert.h"
#include <QtCore\QTextstream.h>
#include <QtCore\QFile.h>

using namespace std;

registerAnimation::registerAnimation(string curDirectory, string filename, getTemplate behaviortemplate, string behaviorpath, string behavior, bool fstP, bool isNemesis)
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

		if (nextpos == NOT_FOUND)
		{
			ErrorMessage(1081);
		}
		
		behaviorFile = behavior;
	}

	QFile file(QString::fromStdString(filepath));
	string line;

	if (file.open(QIODevice::ReadOnly))
	{
		int linecount = 0;
		int multiCount = -1;
		string previousShortline;
		string previousLine;
		string curAAprefix;
		unordered_map<string, bool> AAAnimFileExist;
		unordered_map<string, bool> AAprefixExist;
		QTextStream in(&file);

		while (!in.atEnd())
		{
			QString qline = in.readLine();
			string temp = qline.toStdString();
			string temp2;
			++linecount;
			bool hasInfo = false;

			for (auto& chr : temp)
			{
				if (chr == L'\'' || chr == L'\n' || chr == L'\0')
				{
					break;
				}
				else
				{
					temp2.push_back(chr);

					if (isalnum(chr))
					{
						hasInfo = true;
					}
				}
			}

			if (temp2.length() != 0 && hasInfo)
			{
				line = temp2;
				vecstr newAnimInfo;
				StringSplit(line, newAnimInfo);

				if (newAnimInfo[0].length() == 0)
				{
					ErrorMessage(1016, filename, linecount);
				}

				string lowerformat = boost::to_lower_copy(newAnimInfo[0]);

				if (lowerformat == "version")
				{
					if (boost::iequals(newAnimInfo[1].substr(0, 1), "V")) version = line.substr(line.find(newAnimInfo[1]));
					else version = "V" + line.substr(line.find(newAnimInfo[1]));
				}
				else if (lowerformat == "aaprefix")
				{
					if (string(line).substr(wordFind(line, "AAprefix") + 9).length() > 3 || newAnimInfo[1].length() != 3 || newAnimInfo.size() != 2)
					{
						ErrorMessage(4002, filename, linecount, line);
					}

					curAAprefix = newAnimInfo[1];
					string lowerPrefix = boost::to_lower_copy(curAAprefix);

					if (!AAprefixExist[lowerPrefix])
					{
						if (AAprefixExist.size() > 30)
						{
							ErrorMessage(4007, filename, linecount, modID, newAnimInfo[1]);
						}

						AAprefixExist[lowerPrefix] = true;
					}
				}
				else if (lowerformat == "aaset")
				{
					if (curAAprefix.length() == 0)
					{
						ErrorMessage(4006, filename, linecount, line);
					}

					if (newAnimInfo.size() != 3)
					{
						ErrorMessage(4003, filename, linecount, line);
					}

					string lowGroupName = boost::algorithm::to_lower_copy(newAnimInfo[1]);
					size_t groupSize = groupAA[lowGroupName].size();

					if (groupSize == 0)
					{
						ErrorMessage(4004, filename, linecount, line);
					}
					
					if (newAnimInfo[2].length() > 0)
					{
						if (!isOnlyNumber(newAnimInfo[2]))
						{
							ErrorMessage(4005, filename, linecount, line);
						}
					}

					int num = stoi(newAnimInfo[2]);
					string lowGN = fstP ? lowGroupName + "_1p*" : lowGroupName;
					AAGroupCount[curAAprefix][lowGN] = num;
					groupAAPrefix[lowGN].push_back(curAAprefix);

					for (unsigned int i = 0; i < groupSize; ++i)		// list of animations in the animation group
					{
						string lowAnim = groupAA[lowGroupName][i];
						string lowAnimPerson = (fstP ? lowAnim + "_1p*" : lowAnim);

						for (int j = 0; j < num; ++j)					// number of variation of the specific animation
						{
							string animFile = curAAprefix + to_string(j) + "_" + lowAnim;

							if (!isFileExist(curDirectory + animFile))
							{
								// mute this warning
								if (true == false)
								{
									WarningMessage(1003, modID, animFile);
								}

								alternateAnim[lowAnimPerson].push_back("x");
							}
							else
							{
								alternateAnim[lowAnimPerson].push_back(modID + "\\" + animFile);
								AAAnimFileExist[boost::to_lower_copy(fstP ? animFile + "_1p*" : animFile)] = true;
							}
						}

						if (alternateAnim[lowAnimPerson].size() > 128)
						{
							ErrorMessage(4008, filename, linecount, line);
						}
					}

					if (error) throw nemesis::exception();
				}
				else if (lowerformat == "t")
				{
					if (newAnimInfo.size() < 4 || newAnimInfo.size() % 2 != 0)
					{
						ErrorMessage(4010, filename, linecount);
					}

					string lowerAnimName = boost::algorithm::to_lower_copy(fstP ? newAnimInfo[1] + "_1p*" : newAnimInfo[1]) ;

					if (!AAAnimFileExist[lowerAnimName])
					{
						ErrorMessage(4011, filename, linecount, newAnimInfo[1]);
					}
					else
					{
						string originalAnim = lowerAnimName.substr(lowerAnimName.find("_") + 1);
						
						if (newAnimInfo.size() > 2) AAHasEvent[originalAnim].push_back(lowerAnimName);

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
				else if (lowerformat == "animvar")
				{
					size_t nextpos = 0;
					size_t num = count(line.begin(), line.end(), ' ');
					string name = newAnimInfo[1];
					string type = newAnimInfo[2];
					string value = newAnimInfo[3];

					if (newAnimInfo.size() > 4)
					{
						ErrorMessage(1023, filename, linecount, line);
					}

					if (!isOnlyNumber(value))
					{
						ErrorMessage(1030, filename, linecount, line);
					}
					
					var variable(type, value);

					if (!error)
					{
						AnimVar[name] = variable;
					}
					else
					{
						ErrorMessage(1018, filename, linecount, type);
					}
				}
				else if (newAnimInfo[0] == "MD")
				{
					if (newAnimInfo.size() != 5)
					{
						ErrorMessage(1089, filename, linecount);
					}

					for (unsigned int i = 1; i < newAnimInfo.size(); ++i)
					{
						if (!isOnlyNumber(newAnimInfo[i]))
						{
							ErrorMessage(1091, filename, linecount);
						}
					}

					if (animInfo[previousShortline].back()->motionData.size() > 0)
					{
						int timer = stoi(animInfo[previousShortline].back()->motionData.back().substr(0,
							animInfo[previousShortline].back()->motionData.back().find(" ")));

						if (timer >= stoi(newAnimInfo[1]))
						{
							ErrorMessage(1087, filename, linecount);
						}
					}

					animInfo[previousShortline].back()->motionData.push_back(newAnimInfo[1] + " " + newAnimInfo[2] + " " + newAnimInfo[3] + " " + newAnimInfo[4]);
				}
				else if (newAnimInfo[0] == "RD")
				{
					if (newAnimInfo.size() != 6)
					{
						ErrorMessage(1088, filename, linecount);
					}

					if (animInfo[previousShortline].back()->motionData.size() == 0)
					{
						ErrorMessage(1090, filename, linecount);
					}

					if (animInfo[previousShortline].back()->rotationData.size() > 0)
					{
						int timer = stoi(animInfo[previousShortline].back()->rotationData.back().substr(0,
							animInfo[previousShortline].back()->rotationData.back().find(" ")));

						if (timer >= stoi(newAnimInfo[1]))
						{
							ErrorMessage(1086, filename, linecount);
						}
					}

					animInfo[previousShortline].back()->rotationData.push_back(newAnimInfo[1] + " " + newAnimInfo[2] + " " + newAnimInfo[3] +
						" " + newAnimInfo[4] + " " + newAnimInfo[5]);
				}
				else if (newAnimInfo[0] == "+")
				{
					if (previousShortline.length() > 0)
					{
						if (behaviortemplate.templatelist[previousShortline])
						{
							bool isOExist = true;
							string anim = newAnimInfo[newAnimInfo.size() - 1];
							string number = boost::regex_replace(string(anim), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (isOnlyNumber(number) && anim.length() > number.length() && anim[anim.length() - number.length() - 1] == '/' &&
								anim.rfind(number) == anim.length() - number.length())
							{
								isOExist = false;
								OptionList* behaviorOption = &behaviortemplate.optionlist[previousShortline];

								if (behaviorOption->storelist.find("o") == behaviorOption->storelist.end() || !behaviorOption->storelist["o"])
								{
									if (behaviorOption->animObjectCount > 0)
									{
										isOExist = true;
									}
								}
							}

							if (newAnimInfo.size() > 3 && newAnimInfo[1][0] == '-')
							{
								animInfo[previousShortline].push_back(make_shared<animationInfo>(newAnimInfo, filename,
									behaviortemplate.optionlist[previousShortline], linecount, isOExist));
								
								animInfo[previousShortline].back()->addFilename(newAnimInfo[3]);

								if (!animInfo[previousShortline].back()->known && !isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[3]))
								{
									WarningMessage(1000, newAnimInfo[3]);
								}
							}
							else
							{
								animInfo[previousShortline].push_back(make_shared<animationInfo>(newAnimInfo, filename,
									behaviortemplate.optionlist[previousShortline], linecount, isOExist, true));
								
								animInfo[previousShortline].back()->addFilename(newAnimInfo[2]);

								if (!isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[2]))
								{
									WarningMessage(1000, newAnimInfo[2]);
								}
							}

							if (behaviortemplate.behaviortemplate.find(previousShortline + "_group") != behaviortemplate.behaviortemplate.end() &&
								behaviortemplate.behaviortemplate[previousShortline + "_group"].size() != 0)
							{
								animInfo[previousShortline].back()->optionPickedCount[previousShortline + "_group"] = 1;
							}

							if (behaviortemplate.behaviortemplate.find(previousShortline + "_master") != behaviortemplate.behaviortemplate.end() &&
								behaviortemplate.behaviortemplate[previousShortline + "_master"].size() != 0)
							{
								animInfo[previousShortline].back()->optionPickedCount[previousShortline + "_master"] = 1;
							}

							if (!isOExist)
							{
								ErrorMessage(1000, filename, linecount);
							}

							templateType[previousShortline]++;
							isMulti[previousShortline].push_back(multiCount);
							multiCount++;
						}
					}
					else
					{
						ErrorMessage(1065, filename, linecount);
					}
				}
				else if (behaviortemplate.templatelist[lowerformat])
				{
					string anim = newAnimInfo[newAnimInfo.size() - 1];
					bool isOExist = true;

					if (anim[anim.length() - 2] == '/' && (anim[anim.length() - 1] == '1' || anim[anim.length() - 1] == '2'))
					{
						isOExist = false;
					}

					if (newAnimInfo.size() > 3 && newAnimInfo[1][0] == '-')
					{
						animInfo[lowerformat].push_back(make_shared<animationInfo>(newAnimInfo, filename, behaviortemplate.optionlist[lowerformat], linecount,
							isOExist));
						animInfo[lowerformat].back()->addFilename(newAnimInfo[3]);

						if (!animInfo[lowerformat].back()->known && !isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[3]))
						{
							WarningMessage(1000, newAnimInfo[3]);
						}
					}
					else
					{
						animInfo[lowerformat].push_back(make_shared<animationInfo>(newAnimInfo, filename, behaviortemplate.optionlist[lowerformat], linecount,
							isOExist, true));
						animInfo[lowerformat].back()->addFilename(newAnimInfo[2]);

						if (!isFileExist(filepath.substr(0, filepath.find_last_of("\\") + 1) + newAnimInfo[2]))
						{
							WarningMessage(1000, newAnimInfo[2]);
						}
					}

					if (error) throw nemesis::exception();

					if (behaviortemplate.behaviortemplate.find(lowerformat + "_group") != behaviortemplate.behaviortemplate.end() &&
						behaviortemplate.behaviortemplate[lowerformat + "_group"].size() != 0)
					{
						animInfo[lowerformat].back()->optionPickedCount[lowerformat + "_group"] = 1;
					}

					if (behaviortemplate.behaviortemplate.find(lowerformat + "_master") != behaviortemplate.behaviortemplate.end() &&
						behaviortemplate.behaviortemplate[lowerformat + "_master"].size() != 0)
					{
						animInfo[lowerformat].back()->optionPickedCount[lowerformat + "_master"] = 1;
					}

					if (!isOExist)
					{
						ErrorMessage(1000, filename, linecount);
					}

					if (previousShortline.length() > 0)
					{
						if (behaviortemplate.optionlist[previousShortline].groupMin > multiCount)
						{
							ErrorMessage(1034, behaviortemplate.optionlist[previousShortline].groupMin, previousShortline, filename, linecount - 1);
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
										vecstr AnimInfo;
										StringSplit(previousLine, AnimInfo);
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

												if (curList[j].length() == (*rule)[i].length() ||
													(behaviortemplate.optionlist[previousShortline].addOn[(*rule)[i]].size() != 0 &&
														curList[j].length() > (*rule)[i].length()) || curList[j] == (*rule)[i] + number)
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
												ErrorMessage(1035, filename, linecount);
											}

											if (error) throw nemesis::exception();
										}
									}
									else
									{
										ErrorMessage(1035, filename, linecount);
									}
								}
							}
						}

						if (error) throw nemesis::exception();
					}

					if (behaviortemplate.optionlist[lowerformat].ruleOne.size() != 0 || behaviortemplate.optionlist[lowerformat].compulsory.size() != 0)
					{
						if (line[lowerformat.length() + 1] == '-')
						{
							vecstr AnimInfo;
							StringSplit(line, AnimInfo);
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

							size_t compulsorySize = behaviortemplate.optionlist[lowerformat].compulsory.size();
							vecstr* compulsory = &behaviortemplate.optionlist[lowerformat].compulsory;

							for (size_t i = 0; i < compulsorySize; ++i)
							{
								bool isMatched = false;

								for (unsigned int j = 0; j < curList.size(); ++j)
								{
									bool matching = false;
									string number = boost::regex_replace(string(curList[j]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (curList[j].length() == (*compulsory)[i].length() ||
										(behaviortemplate.optionlist[lowerformat].addOn[(*compulsory)[i]].size() != 0 &&
											curList[j].length() > (*compulsory)[i].length()) || curList[j] == (*compulsory)[i] + number)
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
									ErrorMessage(1035, filename, linecount);
								}
							}

							size_t ruleOneSize = behaviortemplate.optionlist[lowerformat].ruleOne.size();
							vecstr* rule = &behaviortemplate.optionlist[lowerformat].ruleOne;

							for (size_t i = 0; i < ruleOneSize; ++i)
							{
								bool isMatched = false;

								for (unsigned int j = 0; j < curList.size(); ++j)
								{
									bool matching = false;
									string number = boost::regex_replace(string(curList[j]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (curList[j].length() == (*rule)[i].length() ||
										(behaviortemplate.optionlist[lowerformat].addOn[(*rule)[i]].size() != 0 && curList[j].length() >(*rule)[i].length()) ||
										curList[j] == (*rule)[i] + number)
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
									ErrorMessage(1035, filename, linecount);
								}
							}
						}
						else
						{
							ErrorMessage(1035, filename, linecount);
						}
					}

					multiCount = 0;
					++templateType[lowerformat];
					isMulti[lowerformat].push_back(multiCount);
					previousShortline = lowerformat;
					++multiCount;
				}
				else
				{
					ErrorMessage(1016, filename, linecount);
				}

				if (error) throw nemesis::exception();

				previousLine = line;
			}
		}

		file.close();

		if (error) throw nemesis::exception();

		++linecount;

		if (behaviortemplate.optionlist[previousShortline].groupMin > multiCount)
		{
			ErrorMessage(1034, behaviortemplate.optionlist[previousShortline].groupMin, previousShortline, filename, linecount - 1);
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
						vecstr AnimInfo;
						StringSplit(line, AnimInfo);
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

								if (curList[j].length() == (*rule)[i].length() || (behaviortemplate.optionlist[previousShortline].addOn[(*rule)[i]].size() != 0 &&
									curList[j].length() >(*rule)[i].length()) || curList[j] == (*rule)[i] + number)
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
								ErrorMessage(1035, filename, linecount);
							}
						}
					}
					else
					{
						ErrorMessage(1035, filename, linecount);
					}
				}
			}
		}

		if (error) throw nemesis::exception();

		boost::filesystem::path file(filepath);
		interMsg(TextBoxMessage(1006) + ": " + (fstP ? file.parent_path().stem().string() + " (1st Person)" : file.parent_path().stem().string()));
	}
	else
	{
		ErrorMessage(1004, filename);
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
