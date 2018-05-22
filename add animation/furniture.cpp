#include "furniture.h"
#include <boost\crc.hpp>

#pragma warning(disable:4503)

using namespace std;

string ZeroEvent;

Furniture::Furniture(unordered_map<string, vecstr> furnitureformat, string formatname, int furniturecount, string curfilepath, animationInfo& animationinfo)
{
	// import registerAnimation information
	addOn = animationinfo.addOn;
	linkedOption = animationinfo.linkedOption;
	optionPicked = animationinfo.optionPicked;
	optionPickedCount = animationinfo.optionPickedCount;
	addition = animationinfo.addition;
	groupAddition = animationinfo.groupAddition;
	mixOptRegis = animationinfo.mixOptRegis;
	mixOptRever = animationinfo.mixOptRever;
	eventID = animationinfo.eventID;
	variableID = animationinfo.variableID;
	hasDuration = animationinfo.hasDuration;
	duration = animationinfo.duration;
	mainAnimEvent = animationinfo.mainAnimEvent;
	filename = animationinfo.filename;
	AnimObject = animationinfo.AnimObject;
	known = animationinfo.known;

	format = formatname;
	furniturelines = furnitureformat;
	furnitureCount = furniturecount;
	filepath = curfilepath;
}

vecstr Furniture::GetFurnitureLine(string behaviorFile, int& nFunctionID, ImportContainer& import, id eventid, id variableid, vector<int>& stateID, int stateCountMultiplier, bool hasGroup, bool isCore)
{
	vecstr generatedlines;
	vecstr recorder;

	newImport = const_cast<ImportContainer*>(&import);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	lastState = const_cast<vector<int>*>(&stateID);

	bool skip = false; // mainly used by NEW
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	bool open = false;
	bool norElement = false;
	bool multi = false;
	bool newOpen = false;
	int openOrder = -2;
	int condition = 0;
	vector<int> fixedStateID = stateID;
	__int64 openRange = 0;
	__int64 counter = 0;
	string multiOption;
	size_t elementLine = -1;
	unordered_map<int, bool> IsConditionOpened;
	vector<unordered_map<string, bool>> groupOptionPicked;

	strID = to_string(*nextFunctionID);

	for (unsigned int i = 0; i < 4 - strID.length(); ++i)
	{
		strID = "0" + strID;
	}

	groupOptionPicked.reserve(groupAnimInfo.size());

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	IsConditionOpened[0] = true;
	generatedlines.reserve(furniturelines[behaviorFile].size() + 10 * memory);

	for (unsigned int i = 0; i < furniturelines[behaviorFile].size(); ++i)
	{
		bool uniqueskip = false;
		bool elementCatch = false;
		string line = furniturelines[behaviorFile][i];

		if (!isCore)
		{
			if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
			{
				condition++;

				if (!freeze)
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
							string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
							animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);

							if (newCondition(conditionLine, generatedlines, groupOptionPicked, i + 1, utility))
							{
								skip = false;
								IsConditionOpened[condition] = true;
							}
							else
							{
								skip = true;
							}

							if (error)
							{
								generatedlines.shrink_to_fit();
								return generatedlines;
							}
						}
					}
					else
					{
						skip = true;
						freeze = true;
					}
				}

				uniqueskip = true;
			}
			else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
			{
				if (condition == 0)
				{
					ErrorMessage(1119, format, i + 1);
					generatedlines.shrink_to_fit();
					return generatedlines;
				}

				if (!freeze)
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
							string option = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
							animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);

							if (newCondition(option, generatedlines, groupOptionPicked, i + 1, utility))
							{
								skip = false;
								IsConditionOpened[condition] = true;
							}
							else
							{
								skip = true;
							}

							if (error)
							{
								generatedlines.shrink_to_fit();
								return generatedlines;
							}
						}
					}
					else
					{
						skip = true;
						freeze = true;
					}
				}

				uniqueskip = true;
			}
			else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
			{
				if (condition == 0)
				{
					ErrorMessage(1119, format, i + 1);
					generatedlines.shrink_to_fit();
					return generatedlines;
				}

				if (!freeze)
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							skip = false;
							IsConditionOpened[condition] = true;
							uniqueskip = true;
							size_t conditionPosition = line.find("<!-- CONDITION") + 14;
							string replacement1 = line.substr(0, conditionPosition + 1);
							string replacement2 = line.substr(conditionPosition);
							generatedlines.push_back(replacement1 + "START" + replacement2);
						}
						else
						{
							skip = true;
						}
					}
					else
					{
						skip = true;
						freeze = true;
					}
				}

				uniqueskip = true;
			}
			else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
			{
				if (newOpen)
				{
					ErrorMessage(1116, format, i + 1);
					return generatedlines;
				}

				if (IsConditionOpened[condition])
				{
					if (!open)
					{
						string curOption = getOption(furniturelines[behaviorFile][i], false);
						bool isNot = false;

						if (curOption[0] == '!')
						{
							isNot = true;
							curOption = curOption.substr(1);
						}

						vecstr optionInfo = GetOptionInfo(curOption, format, i + 1, lastOrder, groupAnimInfo, false, true, false, order);

						if (error)
						{
							generatedlines.shrink_to_fit();
							return generatedlines;
						}

						if (optionInfo[2] != "AnimObject")
						{
							if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[optionInfo[2]])
							{
								if (isNot)
								{
									skip = true;

								}
								else
								{
									open = true;
								}
							}
							else
							{
								// clear group number
								string previous = optionInfo[2];
								string templine = boost::regex_replace(string(optionInfo[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

								if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[templine])
								{
									if (isNot)
									{
										skip = true;
									}
									else
									{
										open = true;
									}
								}
								else
								{
									string ID = boost::regex_replace(string(previous), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									// animobject bypass
									if (previous == "AnimObject/" + ID)
									{
										if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[previous])
										{
											if (isNot)
											{
												skip = true;
											}
											else
											{
												open = true;
											}
										}
										else
										{
											if (isNot)
											{
												open = true;
											}
											else
											{
												skip = true;
											}
										}
									}
									else
									{
										if (isNot)
										{
											open = true;
										}
										else
										{
											skip = true;
										}
									}
								}
							}
						}
						else
						{
							ErrorMessage(1150, format, i + 1);
							skip = true;
						}
					}
					else
					{
						ErrorMessage(1116, format, i + 1);
						generatedlines.shrink_to_fit();
						return generatedlines;
					}

					if (error)
					{
						generatedlines.shrink_to_fit();
						return generatedlines;
					}
				}

				newOpen = true;
				uniqueskip = true;
			}
			else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
			{
				if (newOpen)
				{
					ErrorMessage(1116, format, i + 1);
					return generatedlines;
				}

				if (IsConditionOpened[condition])
				{
					if (!open)
					{
						string curOption = getOption(line, true);
						bool isNot = false;

						if (curOption[0] == '!')
						{
							isNot = true;
							curOption = curOption.substr(1);
						}

						while (true)
						{
							vecstr optionInfo = GetOptionInfo(curOption, format, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true, false, order);
							string animID = boost::regex_replace(string(curOption), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (error)
							{
								generatedlines.shrink_to_fit();
								return generatedlines;
							}

							if (optionInfo[2] != "AnimObject/" + animID)
							{
								if (optionInfo[1].length() == 0)
								{
									recorder.reserve(furniturelines[behaviorFile].size() / 5);
									open = true;
									multi = true;
									openOrder = -1;
									multiOption = optionInfo[2];
								}
								else
								{
									if (!groupOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
									{
										// animobject bypass
										if (optionInfo[2] == "AnimObject")
										{
											if (isNot)
											{
												skip = true;
											}
											else
											{
												recorder.reserve(furniturelines[behaviorFile].size() / 5);
												open = true;
												multi = true;
												multiOption = optionInfo[2];
												openOrder = stoi(optionInfo[1]);
											}
										}
										else
										{
											// Check if current condition accepts other options that are linked
											if (isNot)
											{
												recorder.reserve(furniturelines[behaviorFile].size() / 5);
												open = true;
												multi = true;
												multiOption = optionInfo[2];

												if (optionInfo[1].length() == 0)
												{
													openOrder = -1;
												}
												else
												{
													openOrder = stoi(optionInfo[1]);
												}
											}
											else
											{
												skip = true;
											}
										}
									}
									else
									{
										if (isNot)
										{
											skip = true;
										}
										else
										{
											recorder.reserve(furniturelines[behaviorFile].size() / 5);
											open = true;
											multi = true;
											multiOption = optionInfo[2];
											openOrder = stoi(optionInfo[1]);
										}
									}
								}
							}
							else
							{
								WarningMessage(1005, format, i + 1);
								skip = true;
							}

							break;
						}
					}
					else
					{
						ErrorMessage(1115, format, i + 1);
						generatedlines.shrink_to_fit();
						return generatedlines;
					}
				}

				newOpen = true;
				uniqueskip = true;
			}
			else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
			{
				if (newOpen)
				{
					ErrorMessage(1116, format, i + 1);
					return generatedlines;
				}

				if (IsConditionOpened[condition])
				{
					if (!open)
					{
						size_t orderPosition = line.find("<!-- NEW ORDER ") + 15;
						string curOrder = line.substr(orderPosition, line.find(" -->", orderPosition) - orderPosition);
						bool isNot = false;

						if (curOrder[0] == '!')
						{
							isNot = true;
							curOrder = curOrder.substr(1);
						}

						bool number = false;
						bool word = false;
						bool unknown = false;

						for (unsigned int j = 0; j < curOrder.size(); ++j)
						{
							if (isalpha(curOrder[j]))
							{
								word = true;
							}
							else if (isdigit(curOrder[j]))
							{
								number = true;
							}
							else
							{
								unknown = true;
							}
						}

						if (word && number)
						{
							ErrorMessage(1110, format, i + 1);
							generatedlines.shrink_to_fit();
							return generatedlines;
						}
						else if (unknown)
						{
							ErrorMessage(1111, format, i + 1);
							generatedlines.shrink_to_fit();
							return generatedlines;
						}
						else if (word)
						{
							if (boost::iequals(curOrder, "last"))
							{
								if (!isLastOrder)
								{
									if (!isNot)
									{
										skip = true;
									}
								}
								else if (isNot)
								{
									skip = true;
								}
							}
							else if (boost::iequals(curOrder, "first"))
							{
								if (order != 0)
								{
									if (!isNot)
									{
										skip = true;
									}
								}
								else if (isNot)
								{
									skip = true;
								}
							}
							else
							{
								ErrorMessage(1112, format, i + 1);
								generatedlines.shrink_to_fit();
								return generatedlines;
							}
						}
						else if (number)
						{
							if (order != stoi(curOrder))
							{
								if (!isNot)
								{
									skip = true;
								}
							}
							else if (isNot)
							{
								skip = true;
							}
						}
						else
						{
							ErrorMessage(1113, format, i + 1);
							generatedlines.shrink_to_fit();
							return generatedlines;
						}
					}
					else
					{
						ErrorMessage(1114, format, i + 1);
						generatedlines.shrink_to_fit();
						return generatedlines;
					}
				}

				newOpen = true;
				uniqueskip = true;
			}
			else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
			{
				if (!newOpen)
				{
					ErrorMessage(1171, format, i + 1);
					generatedlines.shrink_to_fit();
					return generatedlines;
				}

				newOpen = false;
				uniqueskip = true;
			}
			else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
			{
				uniqueskip = true;
			}
		}

		if (!skip && !uniqueskip && !freeze)
		{
			while (true)
			{
				if (multi)
				{
					recorder.push_back(line);
					break;
				}

				if (!hasDuration && isEnd && line.find("\t\t<hkparam name=\"relativeToEndOfClip\">false</hkparam>", 0) != NOT_FOUND)
				{
					line.replace(line.find("\t\t<hkparam name=\"relativeToEndOfClip\">false</hkparam>") + 38, 5, "true");
					isEnd = false;
				}

				// compute numelements
				if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND && line.find("<!-- COMPUTE -->", line.find("numelements=\"")) != NOT_FOUND)
				{
					if (!norElement)
					{
						norElement = true;
						elementCatch = true;
						string templine = line.substr(0, line.find("<hkparam name=\"", 0));
						openRange = count(templine.begin(), templine.end(), '\t');
					}
					else
					{
						ErrorMessage(1136, format, i + 1);
						generatedlines.shrink_to_fit();
						return generatedlines;
					}
				}
				else if (line.find("</hkparam>") != NOT_FOUND && norElement)
				{
					string templine = line.substr(0, line.find("</hkparam>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						string oldElement;

						if (generatedlines[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
						{
							size_t position = generatedlines[elementLine].find("numelements=\"") + 13;
							oldElement = generatedlines[elementLine].substr(position, generatedlines[elementLine].find("\">", position) - position);
						}
						else
						{
							oldElement = "$elements$";
						}

						if (oldElement != to_string(counter))
						{
							generatedlines[elementLine].replace(generatedlines[elementLine].find(oldElement), oldElement.length(), to_string(counter));
						}

						norElement = false;
						counter = 0;
						elementLine = -1;
					}
				}

				if (norElement)
				{
					string templine = line;

					if (templine.find("<hkobject>") != NOT_FOUND)
					{
						templine = templine.substr(0, templine.find("<hkobject>"));
						__int64 range = count(templine.begin(), templine.end(), '\t');

						if (range == openRange + 1)
						{
							counter++;
						}
					}
					else if (templine.find("\t\t\t#") != NOT_FOUND)
					{
						templine = templine.substr(0, templine.find("#", 0));
						__int64 reference = count(templine.begin(), templine.end(), '\t');

						if (reference == openRange + 1)
						{
							__int64 number = count(line.begin(), line.end(), '#');
							counter += number;
						}
					}
				}

				if (line.find("$") != NOT_FOUND)
				{
					// set animation ID
					if (line.find("$%$", 0) != NOT_FOUND)
					{
						line.replace(line.find("$%$"), 3, to_string(furnitureCount));
					}

					// multi choice selection
					if (line.find("$MC$", 0) != NOT_FOUND)
					{
						animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);
						multiChoice(line, generatedlines, groupOptionPicked, i + 1, utility);

						if (error)
						{
							generatedlines.shrink_to_fit();
							return generatedlines;
						}
					}

					// set function ID
					if (line.find("MID$", 0) != NOT_FOUND)
					{
						int counter = sameWordCount(line, "MID$");

						for (int k = 0; k < counter; ++k)
						{
							size_t MIDposition = line.find("MID$");
							string ID = boost::regex_replace(string(line.substr(MIDposition)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							string oldID = "MID$" + ID;


							if (line.find(oldID, MIDposition) != NOT_FOUND)
							{
								if (IDExist[oldID].length() > 0)
								{
									ID = IDExist[oldID];
								}
								else
								{
									IDExist[oldID] = strID;
									ID = strID;
									newID();
								}

								string templine = line;
								templine.replace(templine.find("MID$", MIDposition), 3, format);
								templine = templine.substr(templine.find(format + "$", MIDposition), format.length() + oldID.length() - 3);
								subFunctionIDs[templine] = ID;
								line.replace(line.find("MID$", MIDposition), oldID.length(), ID);
							}
							else
							{
								ErrorMessage(1130, format, i + 1);
								generatedlines.shrink_to_fit();
								return generatedlines;
							}
						}
					}

					processing(line, generatedlines, format, i + 1, eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);

					if (error)
					{
						generatedlines.shrink_to_fit();
						return generatedlines;
					}

					if (line.length() == 0)
					{
						ErrorMessage(1172, format, i + 1);
						generatedlines.shrink_to_fit();
						return generatedlines;
					}
				}

				// get animation file to check for duplicates and whether the animation has been registered or not
				if (line.find("<hkparam name=\"animationName\">") != NOT_FOUND)
				{
					size_t pos = line.find("animationName\">") + 15;
					string animPath = line.substr(pos, line.find("</hkparam>", pos) - pos);
					boost::algorithm::to_lower(animPath);
					addUsedAnim(behaviorFile, animPath);
				}
				else if (line.find("<hkparam name=\"behaviorName\">") != NOT_FOUND)
				{
					size_t pos = line.find("behaviorName\">") + 14;
					string behaviorName = line.substr(pos, line.find("</hkparam>", pos) - pos);
					boost::algorithm::to_lower(behaviorName);
					behaviorJoints[behaviorName].push_back(behaviorFile);
				}

				generatedlines.push_back(line);

				if (elementCatch)
				{
					elementLine = generatedlines.size() - 1;
				}

				break;
			}
		}

		if (!isCore)
		{
			if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND && IsConditionOpened[condition])
			{
				if (skip)
				{
					skip = false;
				}
				else
				{
					if (multi)
					{
						int size;

						if (openOrder == -2)
						{
							openOrder = 0;
							size = 1;
						}
						else if (openOrder == -1)
						{
							openOrder = 0;
							size = int(groupAnimInfo.size());
						}
						else
						{
							size = openOrder + 1;
						}

						for (int animMulti = openOrder; animMulti < size; ++animMulti)
						{
							for (int optionMulti = 0; optionMulti < groupAnimInfo[animMulti]->optionPickedCount[multiOption]; ++optionMulti)
							{
								for (unsigned int k = 0; k < recorder.size(); ++k)
								{
									string newline = recorder[k];

									if (newline.find("$") != NOT_FOUND)
									{
										// set animation ID
										if (newline.find("$%$", 0) != NOT_FOUND)
										{
											newline.replace(newline.find("$%$"), 3, to_string(furnitureCount));
										}

										// multi choice selection
										if (newline.find("$MC$", 0) != NOT_FOUND)
										{
											animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup, optionMulti, animMulti, multiOption);
											multiChoice(newline, generatedlines, groupOptionPicked, i + 1, utility);

											if (error)
											{
												generatedlines.shrink_to_fit();
												return generatedlines;
											}
										}

										// set function ID
										if (newline.find("MID$", 0) != NOT_FOUND)
										{
											int counter = sameWordCount(newline, "MID$");

											for (int k = 0; k < counter; ++k)
											{
												size_t MIDposition = newline.find("MID$");
												string ID = boost::regex_replace(string(newline.substr(MIDposition)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
												string oldID = "MID$" + ID;


												if (newline.find(oldID, MIDposition) != NOT_FOUND)
												{
													if (IDExist[oldID].length() > 0)
													{
														ID = IDExist[oldID];
													}
													else
													{
														IDExist[oldID] = strID;
														ID = strID;
														newID();
													}

													string templine = newline;
													templine.replace(templine.find("MID$", MIDposition), 3, format);
													templine = templine.substr(templine.find(format + "$", MIDposition), format.length() + oldID.length() - 3);
													subFunctionIDs[templine] = ID;
													newline.replace(newline.find("MID$", MIDposition), oldID.length(), ID);
												}
												else
												{
													ErrorMessage(1130, format, i + 1);
													generatedlines.shrink_to_fit();
													return generatedlines;
												}
											}
										}
									}

									if (newline.find("%", 0) != NOT_FOUND)
									{
										newline.replace(newline.find("%", 0), 1, to_string(optionMulti));
									}

									if (isEnd)
									{
										if (newline.find("\t\t<hkparam name=\"relativeToEndOfClip\">false</hkparam>", 0) != NOT_FOUND)
										{
											newline.replace(newline.find("\t\t<hkparam name=\"relativeToEndOfClip\">false</hkparam>") + 38, 5, "true");
											isEnd = false;
										}
									}

									// compute numelements
									if (norElement)
									{
										string templine = newline;

										if (templine.find("<hkobject>") != NOT_FOUND)
										{
											templine = templine.substr(0, templine.find("<hkobject>"));
											__int64 range = count(templine.begin(), templine.end(), '\t');

											if (range == openRange + 1)
											{
												counter++;
											}
										}
										else if (templine.find("\t\t\t#") != NOT_FOUND)
										{
											templine = templine.substr(0, templine.find("#", 0));
											__int64 reference = count(templine.begin(), templine.end(), '\t');

											if (reference == openRange + 1)
											{
												__int64 number = count(newline.begin(), newline.end(), '#');
												counter += number;
											}
										}
									}

									if (newline.find("$") != NOT_FOUND)
									{
										processing(newline, generatedlines, format, i + 1 - int(recorder.size()) + k, eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup, optionMulti, animMulti, multiOption);

										if (error)
										{
											generatedlines.shrink_to_fit();
											return generatedlines;
										}

										if (line.length() == 0)
										{
											ErrorMessage(1172, format, i + 1);
											generatedlines.shrink_to_fit();
											return generatedlines;
										}
									}

									// get animation file to check for duplicates and whether the animation has been registered or not
									if (newline.find("<hkparam name=\"animationName\">") != NOT_FOUND)
									{
										size_t pos = newline.find("animationName\">") + 15;
										string animPath = newline.substr(pos, newline.find("</hkparam>", pos) - pos);
										boost::algorithm::to_lower(animPath);
										addUsedAnim(behaviorFile, animPath);
									}
									else if (newline.find("<hkparam name=\"behaviorName\">") != NOT_FOUND)
									{
										size_t pos = newline.find("behaviorName\">") + 14;
										string behaviorName = newline.substr(pos, newline.find("</hkparam>", pos) - pos);
										boost::algorithm::to_lower(behaviorName);
										behaviorJoints[behaviorName].push_back(behaviorFile);
									}

									generatedlines.push_back(newline);
								}
							}
						}

						multiOption.clear();
						openOrder = -2;
					}

					recorder.clear();
				}

				multi = false;
				open = false;
			}
			else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
			{
				if (condition == 0)
				{
					ErrorMessage(1118, format, i + 1);
					generatedlines.shrink_to_fit();
					return generatedlines;
				}

				if (freeze && IsConditionOpened[condition])
				{
					// generatedlines.push_back(line);
					freeze = false;
				}

				if (isPassed(condition, IsConditionOpened))
				{
					skip = false;
				}
				else
				{
					skip = true;
				}

				IsConditionOpened[condition] = false;
				condition--;
			}
		}

		if (error)
		{
			generatedlines.shrink_to_fit();
			return generatedlines;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, furniturelines[behaviorFile].size());
		return generatedlines;
	}

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			ErrorMessage(1145, format);
			generatedlines.shrink_to_fit();
			return generatedlines;
			break;
		}
	}

	if (condition != 0)
	{
		ErrorMessage(1122, format);
		generatedlines.shrink_to_fit();
		return generatedlines;
	}

	for (auto it = addOn.begin(); it != addOn.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			string optionName;

			if (addition[it->first][it->second[i]].length() != 0)
			{
				optionName = it->first;

				if (hasGroup)
				{
					subFunctionIDs[format + "[" + to_string(order) + "][" + it->first + "][" + it->second[i] + "]"] = addition[optionName][it->second[i]];
				}
				else
				{
					subFunctionIDs[optionName + "[" + it->second[i] + "]"] = addition[optionName][it->second[i]];
				}
			}
			else
			{
				if (mixOptRegis[it->first].length() != 0)
				{
					optionName = mixOptRegis[it->first];

					for (int k = 0; k < optionPickedCount[optionName]; ++k)
					{
						for (unsigned int j = 0; j < groupAddition[optionName][it->second[i]].size(); ++j)
						{
							if (hasGroup)
							{
								subFunctionIDs[format + "[" + to_string(order) + "][" + optionName + "][" + it->second[i] + "][" + to_string(j) + "]"] = groupAddition[optionName][it->second[i]][j];
							}
							else
							{
								subFunctionIDs[optionName + "[" + it->second[i] + "][" + to_string(j) + "]"] = groupAddition[optionName][it->second[i]][j];
							}
						}
					}
				}
				else
				{
					optionName = it->first;

					for (unsigned int j = 0; j < groupAddition[optionName][it->second[i]].size(); ++j)
					{
						if (hasGroup)
						{
							subFunctionIDs[format + "[" + to_string(order) + "][" + optionName + "][" + it->second[i] + "][" + to_string(j) + "]"] = groupAddition[optionName][it->second[i]][j];
						}
						else
						{
							subFunctionIDs[optionName + "[" + it->second[i] + "][" + to_string(j) + "]"] = groupAddition[optionName][it->second[i]][j];
						}
					}
				}

			}
		}
	}

	if (AnimObject.size() != 0)
	{
		for (auto it = AnimObject.begin(); it != AnimObject.end(); ++it)
		{
			if (hasGroup)
			{
				subFunctionIDs[format + "[" + to_string(order) + "][@AnimObject/" + to_string(it->first) + "]"] = it->second;
			}
			else
			{
				subFunctionIDs["@AnimObject/" + to_string(it->first)] = it->second;
			}
		}
	}

	if (fixedStateID.size() > 1)
	{
		for (unsigned int i = 0; i < fixedStateID.size(); ++i)
		{
			if (hasGroup)
			{
				for (int j = 0; j < stateCountMultiplier; ++j)
				{
					subFunctionIDs[format + "[" + to_string(order) + "][(S" + to_string(i + 1) + "+" + to_string(j) + "]"] = to_string(fixedStateID[i] + j);
				}
			}
			else
			{
				for (int j = 0; j < stateCountMultiplier; ++j)
				{
					subFunctionIDs["(S" + to_string(i + 1) + "+" + to_string(j) + ")"] = to_string(fixedStateID[i] + j);
				}
			}
		}
	}
	else
	{
		if (hasGroup)
		{
			for (int j = 0; j < stateCountMultiplier; ++j)
			{
				subFunctionIDs[format + "[" + to_string(order) + "][(S+" + to_string(j) + ")]"] = to_string(fixedStateID[0] + j);
			}
		}
		else
		{
			for (int j = 0; j < stateCountMultiplier; ++j)
			{
				subFunctionIDs["(S+" + to_string(j) + ")"] = to_string(fixedStateID[0] + j);
			}
		}
	}

	if (hasGroup)
	{
		subFunctionIDs[format + "[" + to_string(order) + "][main_anim_event]"] = mainAnimEvent;
		subFunctionIDs[format + "[" + to_string(order) + "][FilePath]"] = filepath + filename;
		subFunctionIDs[format + "[" + to_string(order) + "][FileName]"] = filename.substr(0, filename.find_last_of("."));
	}
	else
	{
		subFunctionIDs["main_anim_event"] = mainAnimEvent;
		subFunctionIDs["FilePath"] = filepath + filename;
		subFunctionIDs["FileName"] = filename.substr(0, filename.find_last_of("."));
	}

	if (generatedlines.size() != 0 && generatedlines.back().length() != 0)
	{
		generatedlines.push_back("");
	}

	generatedlines.shrink_to_fit();
	return generatedlines;
}

string Furniture::GetFilePath()
{
	return filepath + filename;
}

vecstr Furniture::GetEventID()
{
	return eventID;
}

vecstr Furniture::GetVariableID()
{
	return variableID;
}

SSMap Furniture::GetNewIDs()
{
	return subFunctionIDs;
}

SSMap Furniture::GetMixOpt()
{
	return mixOptRegis;
}

vector<shared_ptr<animationInfo>> Furniture::GetGroupAnimInfo()
{
	return groupAnimInfo;
}

ImportContainer Furniture::GetAddition()
{
	return addition;
}

unordered_map<string, unordered_map<string, vecstr>> Furniture::GetGroupAddition()
{
	return groupAddition;
}

inline void Furniture::newID()
{
	(*nextFunctionID)++;
	strID = to_string(*nextFunctionID);

	for (unsigned int i = 0; i < 4 - strID.length(); ++i)
	{
		strID = "0" + strID;
	}
}

void Furniture::addGroupAnimInfo(vector<shared_ptr<animationInfo>> animInfo)
{
	if (animInfo.size() != lastOrder + 1)
	{
		ErrorMessage(1147);
		return;
	}

	groupAnimInfo = animInfo;
}

void Furniture::storeAnimObject(vecstr animobjects, string listFilename, int linecount)
{
	size_t position;

	for (unsigned int i = 0; i < animobjects.size(); ++i)
	{
		position = animobjects[i].find("/");

		string ObjectName = animobjects[i].substr(0, animobjects[i].find("/", position));
		int temp = stoi(animobjects[i].substr(position + 1, 2));

		if (temp != 1 && temp != 2)
		{
			ErrorMessage(1144, listFilename, linecount);
			return;
		}

		optionPicked["AnimObject/" + to_string(temp)] = true;
		AnimObject[temp] = ObjectName;
	}
}

void Furniture::setZeroEvent(string eventname)
{
	ZeroEvent = eventname;
}

void Furniture::setLastOrder(int curLastOrder)
{
	lastOrder = curLastOrder;

	if (order == curLastOrder)
	{
		isLastOrder = true;
	}
}

void Furniture::setOrder(int curOrder)
{
	order = curOrder;
}

int Furniture::getOrder()
{
	return order;
}

bool Furniture::isLast()
{
	return isLastOrder;
}

bool Furniture::isKnown()
{
	return known;
}

void Furniture::multiChoice(string& line, vecstr& storeline, vector<unordered_map<string, bool>> groupOptionPicked, int numline, animationutility utility)
{
	if (line.find("<!-- ", 0) != NOT_FOUND)
	{
		size_t nextposition = 0;
		int choicecount = 0;

		while (true)
		{
			if (line.find("<!--", nextposition) != NOT_FOUND)
			{
				++choicecount;
				nextposition = line.find("<!-- ", nextposition) + 1;
			}
			else
			{
				break;
			}
		}

		if (line.find("*", 0) != NOT_FOUND || line.find("@", 0) != NOT_FOUND || line.find("%", 0) != NOT_FOUND)
		{
			ErrorMessage(1100, format, numline);
			return;
		}

		nextposition = 0;

		for (int i = 0; i < choicecount; ++i)
		{
			vecstr opt;
			vector<char> storechar;
			nextposition = line.find("<!-- ", nextposition) + 5;
			string tempstr = line.substr(nextposition, line.find(" ", nextposition) - nextposition);

			if (i == choicecount - 1 && tempstr.find("$") != NOT_FOUND)
			{
				tempstr = line.substr(nextposition, line.find(" -->", nextposition) - nextposition);
			}

			if (line.find("<!-- " + tempstr + " -->", 0) == NOT_FOUND)
			{
				if (newCondition(tempstr, storeline, groupOptionPicked, numline, utility))
				{
					if (error)
					{
						return;
					}

					nextposition = line.find(" ", nextposition) + 1;
					string output = line.substr(nextposition, line.find(" -->", nextposition) - nextposition);
					line.replace(line.find("$MC$", 0), 4, output);
					line = line.substr(0, line.find("</hkparam>") + 10);
					return;
				}

				if (error)
				{
					return;
				}
			}
			else if (i == choicecount - 1)
			{
				line.replace(line.find("$MC$", 0), 4, tempstr);
				line = line.substr(0, line.find("</hkparam>") + 10);
				return;
			}
			else
			{
				ErrorMessage(1104, format, numline);
				return;
			}
		}

		line.replace(line.find("$MC$", 0), 4, "null");
	}
	else
	{
		line.replace(line.find("$MC$", 0), 4, "null");
	}
}

bool Furniture::newCondition(string condition, vecstr& storeline, vector<unordered_map<string, bool>> groupOptionPicked, int numline, animationutility utility)
{
	if (condition[0] == '(')
	{
		size_t x = 0;
		size_t y = 0;
		size_t backB = 0;

		for (unsigned int i = 0; i < condition.size(); ++i)
		{
			if (condition[i] == '(')
			{
				y++;
			}
			else if (condition[i] == ')')
			{
				y--;

				if (y == 0)
				{
					backB = i;
					break;
				}
			}
		}
		
		string inHouse = condition.substr(1, backB - 1);
		x = inHouse.find("&");
		y = inHouse.find("|");

		bool inHouseResult;

		if (x == NOT_FOUND && y == NOT_FOUND)
		{
			if (inHouse.find("<") == 0 && inHouse.find(">") == inHouse.length() - 1 && (inHouse.find("!=") != NOT_FOUND || inHouse.find("==") != NOT_FOUND))
			{
				inHouseResult = specialCondition(inHouse, storeline, numline, utility);

				if (error)
				{
					return false;
				}
			}
			else
			{
				bool isNot = false;

				if (inHouse[0] == '!')
				{
					isNot = true;
					inHouse = inHouse.substr(1);
				}

				vecstr optionInfo = GetOptionInfo(inHouse, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

				if (error)
				{
					return false;
				}

				inHouseResult = GetFirstCondition(inHouse, optionInfo, numline, groupOptionPicked, isNot);

				if (error)
				{
					return false;
				}
			}
		}
		else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
		{
			string firstCondition = inHouse.substr(0, inHouse.find("|"));
			string secondCondition = inHouse.substr(inHouse.find("|") + 1);
			
			if (firstCondition.find("<") == 0 && firstCondition.find(">") == firstCondition.length() - 1 && (firstCondition.find("!=") != NOT_FOUND || firstCondition.find("==") != NOT_FOUND))
			{
				inHouseResult = specialCondition(firstCondition, storeline, numline, utility);

				if (error)
				{
					return false;
				}
			}
			else
			{
				bool isNot = false;

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}

				vecstr optionInfo = GetOptionInfo(firstCondition, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

				if (error)
				{
					return false;
				}

				inHouseResult = GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot);

				if (error)
				{
					return false;
				}
			}

			if (!inHouseResult && newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
			{
				inHouseResult = true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string firstCondition = inHouse.substr(0, inHouse.find("&"));
			string tempSecondCondition = inHouse.substr(inHouse.find("&") + 1);
			string secondCondition;

			if (firstCondition.find("<") == 0 && firstCondition.find(">") == firstCondition.length() - 1 && (firstCondition.find("!=") != NOT_FOUND || firstCondition.find("==") != NOT_FOUND))
			{
				size_t x = tempSecondCondition.find("&");
				size_t y = tempSecondCondition.find("|");

				// bool1 & bool2 | bool3 ....
				if ((x == NOT_FOUND || x > y) && y != NOT_FOUND)
				{
					secondCondition = tempSecondCondition.substr(0, tempSecondCondition.find("|"));
					tempSecondCondition = tempSecondCondition.substr(tempSecondCondition.find("|") + 1);

					if (specialCondition(firstCondition, storeline, numline, utility))
					{
						inHouseResult = newCondition(secondCondition, storeline, groupOptionPicked, numline, utility);
					}
					else
					{
						inHouseResult = false;
					}
					
					if (!inHouseResult && newCondition(tempSecondCondition, storeline, groupOptionPicked, numline, utility))
					{
						inHouseResult = true;
					}
				}
				else
				{
					secondCondition = tempSecondCondition;

					if (specialCondition(firstCondition, storeline, numline, utility))
					{
						inHouseResult = newCondition(secondCondition, storeline, groupOptionPicked, numline, utility);
					}
					else
					{
						inHouseResult = false;
					}
				}
				
				if (error)
				{
					return false;
				}
			}
			else
			{
				bool isNot = false;

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}

				vecstr optionInfo = GetOptionInfo(firstCondition, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

				if (error)
				{
					return false;
				}

				size_t x = tempSecondCondition.find("&");
				size_t y = tempSecondCondition.find("|");

				// bool1 & bool2 | bool3 ....
				if ((x == NOT_FOUND || x > y) && y != NOT_FOUND)
				{
					secondCondition = tempSecondCondition.substr(0, tempSecondCondition.find("|"));
					tempSecondCondition = tempSecondCondition.substr(tempSecondCondition.find("|") + 1);

					if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
					{
						inHouseResult = newCondition(secondCondition, storeline, groupOptionPicked, numline, utility);
					}
					else
					{
						inHouseResult = false;
					}

					if (!inHouseResult && newCondition(tempSecondCondition, storeline, groupOptionPicked, numline, utility))
					{
						inHouseResult = true;
					}
				}
				else
				{
					secondCondition = tempSecondCondition;

					if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
					{
						inHouseResult = newCondition(secondCondition, storeline, groupOptionPicked, numline, utility);
					}
					else
					{
						inHouseResult = false;
					}
				}
			}
		}
		else
		{
			ErrorMessage(1105, format, numline);
			return false;
		}

		string outHouse = condition.substr(backB + 1);

		x = outHouse.find("&");
		y = outHouse.find("|");

		if (x == NOT_FOUND && y == NOT_FOUND)
		{
			return inHouseResult;
		}
		else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
		{
			string secondCondition = outHouse.substr(outHouse.find("|") + 1);

			if (inHouseResult || newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
			{
				return true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string secondCondition = outHouse.substr(outHouse.find("&") + 1);

			if (inHouseResult && newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
			{
				return true;
			}
		}
		else
		{
			ErrorMessage(1106, format, numline, condition);
			return false;
		}
	}
	else
	{
		size_t x = condition.find("&");
		size_t y = condition.find("|");

		if (x == NOT_FOUND && y == NOT_FOUND)
		{
			string conditionOrder = condition;

			if (conditionOrder.find("<") == 0 && conditionOrder.find(">") == conditionOrder.length() - 1 && (conditionOrder.find("!=") != NOT_FOUND || conditionOrder.find("==") != NOT_FOUND))
			{
				return specialCondition(conditionOrder, storeline, numline, utility);
			}
			else
			{
				bool isNot = false;

				if (conditionOrder[0] == '!')
				{
					isNot = true;
					conditionOrder = conditionOrder.substr(1);
				}

				vecstr optionInfo = GetOptionInfo(conditionOrder, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

				if (error)
				{
					return false;
				}

				return GetFirstCondition(conditionOrder, optionInfo, numline, groupOptionPicked, isNot);
			}
		}
		else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
		{
			string firstCondition = condition.substr(0, condition.find("|"));
			string secondCondition = condition.substr(condition.find("|") + 1);

			if (firstCondition.find("<") == 0 && firstCondition.find(">") == firstCondition.length() - 1 && (firstCondition.find("!=") != NOT_FOUND || firstCondition.find("==") != NOT_FOUND))
			{
				if (specialCondition(firstCondition, storeline, numline, utility))
				{
					return true;
				}

				if (error)
				{
					return false;
				}
			}
			else
			{
				bool isNot = false;

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}

				vecstr optionInfo = GetOptionInfo(firstCondition, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

				if (error)
				{
					return false;
				}

				if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
				{
					return true;
				}
			}

			if (y != NOT_FOUND && newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
			{
				return true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string firstCondition = condition.substr(0, condition.find("&"));
			string secondCondition = condition.substr(condition.find("&") + 1);

			if (firstCondition.find("<") == 0 && firstCondition.find(">") == firstCondition.length() - 1 && (firstCondition.find("!=") != NOT_FOUND || firstCondition.find("==") != NOT_FOUND))
			{
				if (y != NOT_FOUND)
				{
					if (secondCondition[0] == '(')
					{
						int position;
						int openCounter = 0;

						for (unsigned int i = 0; i < secondCondition.size(); ++i)
						{
							if (secondCondition[i] == '(')
							{
								openCounter++;
							}
							else if (secondCondition[i] == ')')
							{
								openCounter--;

								if (openCounter == 0)
								{
									position = i + 1;
									break;
								}
							}
						}

						string thirdCondition = secondCondition.substr(position);
						secondCondition = secondCondition.substr(1, position - 2);

						if (thirdCondition.length() == 0)
						{
							if (specialCondition(firstCondition, storeline, numline, utility))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}
						}
						else
						{
							char logic = thirdCondition[0];
							thirdCondition = thirdCondition.substr(1);

							if (logic == '&')
							{
								if (specialCondition(firstCondition, storeline, numline, utility))
								{
									if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility) && newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
									{
										return true;
									}
								}
							}
							else if (logic == '|')
							{
								if (specialCondition(firstCondition, storeline, numline, utility))
								{
									if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
									{
										return true;
									}
								}

								if (newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}
							else
							{
								ErrorMessage(1101, format, numline);
								return false;
							}
						}
					}
					else
					{
						x = secondCondition.find("&");
						y = secondCondition.find("|");

						if (x == NOT_FOUND && y == NOT_FOUND)
						{
							if (specialCondition(firstCondition, storeline, numline, utility))
							{
								if (GetSecondCondition(secondCondition, numline, groupOptionPicked))
								{
									return true;
								}

								if (error)
								{
									return false;
								}
							}
						}
						else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
						{
							size_t position = secondCondition.find("|") + 1;
							string thirdCondition = secondCondition.substr(position);
							secondCondition = secondCondition.substr(0, position - 1);

							if (specialCondition(firstCondition, storeline, numline, utility))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}

							if (error)
							{
								return false;
							}

							if (newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
							{
								return true;
							}
						}
						else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
						{
							size_t position = secondCondition.find("&") + 1;
							string thirdCondition = secondCondition.substr(position);
							secondCondition = secondCondition.substr(0, position - 1);

							if (specialCondition(firstCondition, storeline, numline, utility))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility) && newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}

							if (error)
							{
								return false;
							}
						}
						else
						{
							ErrorMessage(1103, format, numline);
							return false;
						}
					}
				}
				else
				{
					if (specialCondition(firstCondition, storeline, numline, utility))
					{
						if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
						{
							return true;
						}
					}
				}
			}
			else
			{
				bool isNot = false;

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}

				vecstr optionInfo = GetOptionInfo(firstCondition, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

				if (error)
				{
					return false;
				}

				if (y != NOT_FOUND)
				{
					if (secondCondition[0] == '(')
					{
						int position;
						int openCounter = 0;

						for (unsigned int i = 0; i < secondCondition.size(); ++i)
						{
							if (secondCondition[i] == '(')
							{
								openCounter++;
							}
							else if (secondCondition[i] == ')')
							{
								openCounter--;

								if (openCounter == 0)
								{
									position = i + 1;
									break;
								}
							}
						}

						string thirdCondition = secondCondition.substr(position);
						secondCondition = secondCondition.substr(1, position - 2);

						if (thirdCondition.length() == 0)
						{
							if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}
						}
						else
						{
							char logic = thirdCondition[0];
							thirdCondition = thirdCondition.substr(1);

							if (logic == '&')
							{
								if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
								{
									if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility) && newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
									{
										return true;
									}
								}
							}
							else if (logic == '|')
							{
								if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
								{
									if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
									{
										return true;
									}
								}

								if (newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}
							else
							{
								ErrorMessage(1101, format, numline);
								return false;
							}
						}
					}
					else
					{
						x = secondCondition.find("&");
						y = secondCondition.find("|");

						if (x == NOT_FOUND && y == NOT_FOUND)
						{
							if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
							{
								if (GetSecondCondition(secondCondition, numline, groupOptionPicked))
								{
									return true;
								}
							}
						}
						else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
						{
							size_t position = secondCondition.find("|") + 1;
							string thirdCondition = secondCondition.substr(position);
							secondCondition = secondCondition.substr(0, position - 1);

							if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}

							if (newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
							{
								return true;
							}
						}
						else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
						{
							size_t position = secondCondition.find("&") + 1;
							string thirdCondition = secondCondition.substr(position);
							secondCondition = secondCondition.substr(0, position - 1);

							if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility) && newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}
						}
						else
						{
							ErrorMessage(1103, format, numline);
							return false;
						}
					}
				}
				else
				{
					if (GetFirstCondition(firstCondition, optionInfo, numline, groupOptionPicked, isNot))
					{
						if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
						{
							return true;
						}
					}
				}
			}
		}
		else
		{
			ErrorMessage(1102, format, numline);
			return false;
		}
	}

	return false;
}

void Furniture::processing(string& line, vecstr& storeline, string masterFormat, int linecount, id eventid, id variableid, vector<int> fixedStateID, int stateCountMultiplier, bool hasGroup, int optionMulti, int animMulti, string multiOption)
{
	__int64 counter = count(line.begin(), line.end(), '$') / 2;
	size_t curPos = -1;
	bool multiAnim;

	if (animMulti != -1)
	{
		multiAnim = true;
	}
	else
	{
		multiAnim = false;
	}

	for (int i = 0; i < counter; ++i)
	{
		bool isChange = false;
		curPos = line.find("$", curPos + 1);
		string change = line.substr(curPos, line.find("$", curPos + 1) - curPos + 1);
		string oldChange = change;
		change = change.substr(1, change.length() - 2);
		curPos = line.find("$", curPos + 1);

		while (true)
		{
			// order equation
			if (change.find("(") != NOT_FOUND && change.find("L", change.find("(")) != NOT_FOUND && change.find(")", change.find("(")) != NOT_FOUND)
			{
				__int64 maths = count(change.begin(), change.end(), '(');

				if (maths != 0 && maths == count(change.begin(), change.end(), ')'))
				{
					size_t nextpos = change.find("(");

					for (__int64 j = 0; j < maths; ++j)
					{
						string equation = change.substr(nextpos, change.find(")", nextpos) - 1);
						string number = "";
						string ID = "";

						if (equation.find("(S", 0) != NOT_FOUND)
						{
							ID = boost::regex_replace(string(equation), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (change.find("(S" + ID + "+") == NOT_FOUND)
							{
								ID = "";
							}

							number = boost::regex_replace(string(equation.substr(3 + ID.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
						}

						if (equation != "(S" + ID + "+" + number + ")" && isOnlyNumber(number))
						{
							size_t equationLength = equation.length();

							if (equation.find("L") != NOT_FOUND)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'L');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("L"), 1, to_string(lastOrder));
								}
							}

							if (equation.find("N") != NOT_FOUND)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'N');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("N"), 1, to_string(order + 1));
								}
							}

							calculate(equation, error);

							if (stoi(equation) > int(groupAnimInfo.size() - 1) || stoi(equation) < 0)
							{
								ErrorMessage(1148, format, linecount, change);
								return;
							}

							change.replace(nextpos, equationLength, equation);
							isChange = true;
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			// set animation end duration
			if (change.find("END", 0) != NOT_FOUND)
			{
				if (isEnd)
				{
					ErrorMessage(1107, format, linecount, change);
					return;
				}

				if (change.find(format + "[][END]", 0) != NOT_FOUND)
				{
					if (animMulti == -1)
					{
						ErrorMessage(1057, format, linecount, change);
						return;
					}

					if (groupAnimInfo[animMulti]->hasDuration)
					{
						change.replace(change.find(format + "[][END]"), 7 + format.length(), to_string(groupAnimInfo[animMulti]->duration));
					}
					else
					{
						change.replace(change.find(format + "[][END]"), 7 + format.length(), "0.000000");
						isEnd = true;
					}

					isChange = true;
				}

				if (change.find(format + "[F][END]", 0) != NOT_FOUND)
				{
					if (groupAnimInfo[0]->hasDuration)
					{
						change.replace(change.find(format + "[F][END]"), 8 + format.length(), to_string(groupAnimInfo[0]->duration));
					}
					else
					{
						change.replace(change.find(format + "[F][END]"), 8 + format.length(), "0.000000");
						isEnd = true;
					}

					isChange = true;
				}

				if (change.find(format + "[N][END]", 0) != NOT_FOUND)
				{
					int num = order;

					if (!isLastOrder)
					{
						num++;
					}

					if (groupAnimInfo[0]->hasDuration)
					{
						change.replace(change.find(format + "[N][END]"), 8 + format.length(), to_string(groupAnimInfo[num]->duration));
					}
					else
					{
						change.replace(change.find(format + "[N][END]"), 8 + format.length(), "0.000000");
						isEnd = true;
					}

					isChange = true;
				}

				if (change.find(format + "[B][END]", 0) != NOT_FOUND)
				{
					int num = order;

					if (order != 0)
					{
						num--;
					}

					if (groupAnimInfo[num]->hasDuration)
					{
						change.replace(change.find(format + "[B][END]"), 8 + format.length(), to_string(groupAnimInfo[num]->duration));
					}
					else
					{
						change.replace(change.find(format + "[B][END]"), 8 + format.length(), "0.000000");
						isEnd = true;
					}

					isChange = true;
				}

				if (change.find(format + "[L][END]", 0) != NOT_FOUND)
				{
					if (groupAnimInfo[0]->hasDuration)
					{
						change.replace(change.find(format + "[L][END]"), 8 + format.length(), to_string(groupAnimInfo[lastOrder]->duration));
					}
					else
					{
						change.replace(change.find(format + "[L][END]"), 8 + format.length(), "0.000000");
						isEnd = true;
					}

					isChange = true;
				}

				if (change.find(format + "[") != NOT_FOUND)
				{
					string number = boost::regex_replace(string(change.substr(change.find(format + "[") + 1 + format.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find(format + "[" + number + "][END]", 0) != NOT_FOUND && isOnlyNumber(number))
					{
						if (stoi(number) > int(groupAnimInfo.size() - 1) || stoi(number) < 0)
						{
							ErrorMessage(1155, lastOrder , format, linecount, change);
							return;
						}

						if (groupAnimInfo[stoi(number)]->hasDuration)
						{
							change.replace(change.find(format + "[" + number + "][END]"), 7 + number.length() + format.length(), to_string(groupAnimInfo[stoi(number)]->duration));
						}
						else
						{
							change.replace(change.find(format + "[" + number + "][END]"), 7, "0.000000");
							isEnd = true;
						}

						isChange = true;
					}
				}

				if (change.find("END", 0) != NOT_FOUND)
				{
					if (hasDuration)
					{
						change.replace(change.find("END"), 3, to_string(duration));
					}
					else
					{
						change.replace(change.find("END"), 3, "0.000000");
						isEnd = true;
					}

					isChange = true;
				}
			}
			
			// set state ID
			if (change.find("(S", 0) != NOT_FOUND)
			{
				string templine = change.substr(change.find("(S"));
				string ID = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
				int intID;

				if (change.find("(S" + ID + "+") == NOT_FOUND)
				{
					ID = "";
					intID = 0;
				}
				else
				{
					intID = stoi(ID) - 1;

					if (intID >= int(fixedStateID.size()))
					{
						ErrorMessage(1168, format, linecount, templine.substr(0, templine.find(")") + 1));
						return;
					}
				}

				if (change.find(format + "[][(S" + ID + "+", 0) != NOT_FOUND)
				{
					stateReplacer(change, ID, fixedStateID[intID] + ((animMulti - order) * stateCountMultiplier), linecount, hasGroup, "", true);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[F][(S" + ID + "+", 0) != NOT_FOUND)
				{
					stateReplacer(change, ID, fixedStateID[intID] - (order * stateCountMultiplier), linecount, hasGroup, "F", true);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[N][(S" + ID + "+", 0) != NOT_FOUND)
				{
					if (isLastOrder)
					{
						stateReplacer(change, ID, fixedStateID[intID], linecount, hasGroup, "N", true);
					}
					else
					{
						stateReplacer(change, ID, fixedStateID[intID] + stateCountMultiplier, linecount, hasGroup, "N", true);
					}

					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[B][(S" + ID + "+", 0) != NOT_FOUND)
				{
					if (order == 0)
					{
						stateReplacer(change, ID, fixedStateID[intID], linecount, hasGroup, "B", true);
					}
					else
					{
						stateReplacer(change, ID, fixedStateID[intID] - stateCountMultiplier, linecount, hasGroup, "B", true);
					}

					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[L][(S" + ID + "+", 0) != NOT_FOUND)
				{
					stateReplacer(change, ID, fixedStateID[intID] + ((lastOrder - order) * stateCountMultiplier), linecount, hasGroup, "L", true);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[") != NOT_FOUND)
				{
					string number = boost::regex_replace(string(change.substr(change.find(format + "[") + 1 + format.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find(format + "[" + number + "][(S" + ID + "+") != NOT_FOUND && isOnlyNumber(number))
					{
						string number2 = boost::regex_replace(string(change.substr(change.find(format + "[" + number + "][(S" + ID + "+") + format.length() + number.length() + ID.length() + 6)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
						{
							if (change.find(format + "[" + number + "][(S" + ID + "+" + number2 + ")]") != NOT_FOUND)
							{
								stateReplacer(change, ID, fixedStateID[intID] + ((stoi(number) - order) * stateCountMultiplier), linecount, hasGroup, number, true);
								isChange = true;
							}
						}
					}
				}

				if (change.find("(S" + ID + "+", 0) != NOT_FOUND)
				{
					string number = boost::regex_replace(string(change.substr(change.find("(S" + ID + "+") + 3 + ID.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find("(S" + ID + "+" + number + ")", 0) != NOT_FOUND && isOnlyNumber(number))
					{
						stateReplacer(change, ID, fixedStateID[intID], linecount, hasGroup);
						isChange = true;
					}

					if (error)
					{
						return;
					}
				}
			}

			// set file path
			if (change.find("FilePath", 0) != NOT_FOUND)
			{
				if (change.find(format + "[][FilePath]", 0) != NOT_FOUND)
				{
					if (animMulti != -1)
					{
						change.replace(change.find(format + "[][FilePath]"), 12 + format.length(), filepath + groupAnimInfo[animMulti]->filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[animMulti]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}
						
						isChange = true;
					}
					else
					{
						ErrorMessage(1146, format, linecount);
						return;
					}
				}

				if (change.find(format + "[F][FilePath]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[F][FilePath]"), 13 + format.length(), filepath + groupAnimInfo[0]->filename);
					int backtrack = storeline.size() - 1;

					while (backtrack != 0)
					{
						if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
						{
							string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
							groupAnimInfo[0]->clipname.push_back(clipname);
							break;
						}

						--backtrack;
					}

					isChange = true;
				}

				if (change.find(format + "[N][FilePath]", 0) != NOT_FOUND)
				{
					if (isLastOrder)
					{
						change.replace(change.find(format + "[N][FilePath]"), 13 + format.length(), filepath + filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[animMulti]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}
					}
					else
					{
						change.replace(change.find(format + "[N][FilePath]"), 13 + format.length(), filepath + groupAnimInfo[order + 1]->filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[order - 1]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}
					}

					isChange = true;
				}

				if (change.find(format + "[B][FilePath]", 0) != NOT_FOUND)
				{
					if (order == 0)
					{
						change.replace(change.find(format + "[B][FilePath]"), 13 + format.length(), filepath + filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[order]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}
					}
					else
					{
						change.replace(change.find(format + "[B][FilePath]"), 13 + format.length(), filepath + groupAnimInfo[order - 1]->filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[order - 1]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}
					}

					isChange = true;
				}

				if (change.find(format + "[L][FilePath]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[L][FilePath]"), 13 + format.length(), filepath + groupAnimInfo[lastOrder]->filename);
					int backtrack = storeline.size() - 1;

					while (backtrack != 0)
					{
						if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
						{
							string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
							groupAnimInfo[lastOrder]->clipname.push_back(clipname);
							break;
						}

						--backtrack;
					}

					isChange = true;
				}

				if (change.find(format + "[", 0) != NOT_FOUND)
				{
					string number = boost::regex_replace(string(change), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find(format + "[" + number + "][FilePath]", 0) != NOT_FOUND && isOnlyNumber(number))
					{
						change.replace(change.find(format + "[" + number + "][FilePath]"), 12 + format.length() + number.length(), filepath + groupAnimInfo[stoi(number)]->filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[stoi(number)]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}

						isChange = true;
					}
				}

				if (change.find("FilePath", 0) != NOT_FOUND)
				{
					if (filename != change)
					{
						change.replace(change.find("FilePath"), 8, filepath + filename);
						int backtrack = storeline.size() - 1;

						while (backtrack != 0)
						{
							if (storeline[backtrack].find("			<hkparam name=\"name\">") == 0 && storeline[backtrack].find("</hkparam>") != NOT_FOUND)
							{
								string clipname = storeline[backtrack].substr(24, storeline[backtrack].find("</hkparam>") - 24);
								groupAnimInfo[order]->clipname.push_back(clipname);
								break;
							}

							--backtrack;
						}

						isChange = true;
					}
					else
					{
						ErrorMessage(1134, format, linecount);
						return;
					}
				}
			}

			// set file name
			if (change.find("FileName", 0) != NOT_FOUND)
			{
				if (change.find(format + "[][FileName]", 0) != NOT_FOUND)
				{
					if (animMulti != -1)
					{
						change.replace(change.find(format + "[][FileName]"), 12 + format.length(), groupAnimInfo[animMulti]->filename.substr(0, groupAnimInfo[animMulti]->filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
						isChange = true;
					}
					else
					{
						ErrorMessage(1146, format, linecount);
						return;
					}
				}

				if (change.find(format + "[F][FileName]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[F][FileName]"), 13 + format.length(), groupAnimInfo[0]->filename.substr(0, groupAnimInfo[0]->filename.find_last_of(".")));
					int backtrack = storeline.size() - 1;
					isChange = true;
				}

				if (change.find(format + "[N][FileName]", 0) != NOT_FOUND)
				{
					if (isLastOrder)
					{
						change.replace(change.find(format + "[N][FileName]"), 13 + format.length(), filename.substr(0, filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
					}
					else
					{
						change.replace(change.find(format + "[N][FileName]"), 13 + format.length(), groupAnimInfo[order + 1]->filename.substr(0, groupAnimInfo[order + 1]->filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
					}

					isChange = true;
				}

				if (change.find(format + "[B][FileName]", 0) != NOT_FOUND)
				{
					if (order == 0)
					{
						change.replace(change.find(format + "[B][FileName]"), 13 + format.length(), filename.substr(0, filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
					}
					else
					{
						change.replace(change.find(format + "[B][FileName]"), 13 + format.length(), groupAnimInfo[order - 1]->filename.substr(0, groupAnimInfo[order - 1]->filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
					}

					isChange = true;
				}

				if (change.find(format + "[L][FileName]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[L][FileName]"), 13 + format.length(), groupAnimInfo[lastOrder]->filename.substr(0, groupAnimInfo[lastOrder]->filename.find_last_of(".")));
					int backtrack = storeline.size() - 1;
					isChange = true;
				}

				if (change.find(format + "[", 0) != NOT_FOUND)
				{
					string number = boost::regex_replace(string(change), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find(format + "[" + number + "][FileName]", 0) != NOT_FOUND && isOnlyNumber(number))
					{
						change.replace(change.find(format + "[" + number + "][FileName]"), 12 + format.length() + number.length(), groupAnimInfo[stoi(number)]->filename.substr(0, groupAnimInfo[stoi(number)]->filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
						isChange = true;
					}
				}

				if (change.find("FileName", 0) != NOT_FOUND)
				{
					if (filename != change)
					{
						change.replace(change.find("FileName"), 8, filename.substr(0, filename.find_last_of(".")));
						int backtrack = storeline.size() - 1;
						isChange = true;
					}
					else
					{
						ErrorMessage(1134, format, linecount);
						return;
					}
				}
			}

			// set path
			if (change.find("Path", 0) != NOT_FOUND)
			{
				if (change.find(format + "[][Path]", 0) != NOT_FOUND)
				{
					if (animMulti != -1)
					{
						change.replace(change.find(format + "[][Path]"), 8 + format.length(), filepath.substr(0, filepath.length() - 1));
						isChange = true;
					}
					else
					{
						ErrorMessage(1146, format, linecount);
						return;
					}
				}

				if (change.find(format + "[F][Path]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[F][Path]"), 9 + format.length(), filepath.substr(0, filepath.length() - 1));
					isChange = true;
				}

				if (change.find(format + "[N][Path]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[N][Path]"), 9 + format.length(), filepath.substr(0, filepath.length() - 1));
					isChange = true;
				}

				if (change.find(format + "[B][Path]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[B][Path]"), 9 + format.length(), filepath.substr(0, filepath.length() - 1));
					isChange = true;
				}

				if (change.find(format + "[L][Path]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[L][Path]"), 9 + format.length(), filepath.substr(0, filepath.length() - 1));
					isChange = true;
				}

				if (change.find(format + "[", 0) != NOT_FOUND)
				{
					string number = boost::regex_replace(string(change), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find(format + "[" + number + "][Path]", 0) != NOT_FOUND && isOnlyNumber(number))
					{
						change.replace(change.find(format + "[" + number + "][Path]"), 8 + format.length() + number.length(), filepath.substr(0, filepath.length() - 1));
						isChange = true;
					}
				}

				if (change.find("Path", 0) != NOT_FOUND)
				{
					if (filename != change)
					{
						change.replace(change.find("Path"), 4, filepath.substr(0, filepath.length() - 1));
						isChange = true;
					}
					else
					{
						ErrorMessage(1134, format, linecount);
						return;
					}
				}
			}

			// set AnimObject
			if (change.find("@AnimObject", 0) != NOT_FOUND)
			{
				if (change.find(format + "[", 0) != NOT_FOUND && change.find("][@AnimObject", 0) != NOT_FOUND)
				{
					int counter = sameWordCount(change, "][@AnimObject");

					for (int k = 0; k < counter; ++k)
					{
						size_t pos = change.find("][@AnimObject");
						int ref = sameWordCount(change, format + "[");
						size_t nextpos;

						for (int j = 0; j < ref; ++j)
						{
							size_t tempos = change.find(format + "[");

							if (tempos < pos)
							{
								nextpos = tempos;
							}
							else
							{
								break;
							}
						}

						pos = change.find("]", pos + 1) + 1;
						vecstr optionInfo = GetOptionInfo(change.substr(nextpos, pos - nextpos), format, linecount, lastOrder, groupAnimInfo, false, false, false, order, optionMulti);

						if (error)
						{
							return;
						}

						if (optionInfo[0] == format && optionInfo[2].find("@AnimObject") != NOT_FOUND)
						{
							int number;
							int groupNum;
							pos = change.find(format + "[") + 1 + format.length();
							string animNumStr = change.substr(pos, change.find("]", pos) - pos);

							if (optionInfo[1].length() == 0)
							{
								if (!multiAnim)
								{
									ErrorMessage(1126, format, linecount, change);
									return;
								}

								groupNum = animMulti;
							}
							else
							{
								groupNum = stoi(optionInfo[1]);
							}

							if (multiOption.length() != 0 && groupAnimInfo[groupNum]->optionPickedCount[multiOption] > 1)
							{
								number = optionMulti + 1;
							}
							else
							{
								if (!multiAnim && optionInfo[2] == "@AnimObject")
								{
									ErrorMessage(1126, format, linecount, change);
									return;
								}

								number = groupAnimInfo[groupNum]->AnimObject.begin()->first;
							}

							animObjectReplacer(change, groupAnimInfo[groupNum]->AnimObject, number, format, linecount, true, animNumStr);
							isChange = true;

							if (error)
							{
								return;
							}
						}
					}
				}

				if (change.find("@AnimObject", 0) != NOT_FOUND)
				{
					int counter = sameWordCount(change, "@AnimObject");

					for (int k = 0; k < counter; ++k)
					{
						int number;

						if (multiOption.length() != 0 && groupAnimInfo[order]->optionPickedCount[multiOption] > 1)
						{
							number = optionMulti + 1;
						}
						else
						{
							if (!multiAnim && change.find("@AnimObject/") == NOT_FOUND)
							{
								ErrorMessage(1126, format, linecount, change);
								return;
							}

							number = groupAnimInfo[order]->AnimObject.begin()->first;
						}

						animObjectReplacer(change, AnimObject, number, format, linecount);
						isChange = true;

						if (error)
						{
							return;
						}
					}
				}
			}

			// get Animation Event Name
			if (change.find("main_anim_event", 0) != NOT_FOUND)
			{
				if (change.find(format + "[][main_anim_event]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[][main_anim_event]"), 19 + format.length(), groupAnimInfo[animMulti]->mainAnimEvent);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[F][main_anim_event]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[F][main_anim_event]"), 20 + format.length(), groupAnimInfo[0]->mainAnimEvent);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[N][main_anim_event]", 0) != NOT_FOUND)
				{
					if (isLastOrder)
					{
						change.replace(change.find(format + "[N][main_anim_event]"), 20 + format.length(), mainAnimEvent);
					}
					else
					{
						change.replace(change.find(format + "[N][main_anim_event]"), 20 + format.length(), groupAnimInfo[order + 1]->mainAnimEvent);
					}

					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[L][main_anim_event]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[L][main_anim_event]"), 20 + format.length(), groupAnimInfo[lastOrder]->mainAnimEvent);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find("main_anim_event", 0) != NOT_FOUND)
				{
					change.replace(change.find("main_anim_event"), 15, mainAnimEvent);
					isChange = true;
				}
			}

			size_t position = change.find("[");

			if (position != NOT_FOUND && line.find("]", position) != NOT_FOUND)
			{
				addOnReplacer(change, addOn, addition, groupAddition, mixOptRegis, optionMulti, masterFormat, linecount);
				isChange = true;
			}

			// get last state
			if (change.find("LastState", 0) != NOT_FOUND)
			{
				int ID;
				string number = "";

				if (fixedStateID.size() > 1)
				{
					number = boost::regex_replace(string(change.substr(change.find("LastState"))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find("LastState" + number, 0) != NOT_FOUND && isOnlyNumber(number))
					{
						ID = stoi(number) - 1;

						if (ID >= int(fixedStateID.size()))
						{
							ErrorMessage(1168, format, linecount, "LastState" + number);
							return;
						}
					}
					else
					{
						number = "";
					}
				}
				else
				{
					ID = 0;
				}

				change.replace(change.find("LastState"), 9 + number.length(), to_string(lastOrder - order + fixedStateID[ID]));
				isChange = true;
			}

			position = change.find("eventID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				eventIDReplacer(change, format, eventid, ZeroEvent, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("variableID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				variableIDReplacer(change, format, variableid, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("crc32[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				CRC32Replacer(change, format, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("import[");

			if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
			{
				size_t nextpos = change.find("import[");
				string importer = change.substr(nextpos, change.find_last_of("]") - nextpos + 1);
				__int64 bracketCount = count(importer.begin(), importer.end(), '[');
				__int64 altBracketCount = count(importer.begin(), importer.end(), ']');

				if (IDExist[importer].length() == 0)
				{
					if (bracketCount != altBracketCount)
					{
						ErrorMessage(1139, format, linecount, importer);
						return;
					}

					size_t pos = importer.find("[") + 1;
					string file = importer.substr(pos, importer.find("]", pos) - pos);
					string keyword;
					string tempID;

					if (bracketCount > 1)
					{
						pos = importer.find("[", pos) + 1;
						keyword = importer.substr(pos, importer.find_last_of("]") - pos);
					}
					else
					{
						keyword = "";
					}

					if ((*newImport)[file][keyword].length() > 0)
					{
						tempID = (*newImport)[file][keyword];
					}
					else
					{
						tempID = strID;
						IDExist[importer] = tempID;
						(*newImport)[file][keyword] = tempID;
						newID();
					}

					change.replace(nextpos, importer.length(), tempID);
					isChange = true;
				}
				else
				{
					change.replace(nextpos, importer.length(), IDExist[importer]);
					isChange = true;
				}
			}

			if (error)
			{
				return;
			}

			if (change.find("MD") != NOT_FOUND)
			{
				if (fixedStateID.size() != 0 || eventid.size() != 0 || variableid.size() != 0 || stateCountMultiplier != 0)
				{
					ErrorMessage(1096, format, linecount);
					return;
				}

				motionDataReplacer(change, format, filename, linecount, groupAnimInfo, order, lastOrder, isLastOrder, animMulti, isChange);
			}

			if (error)
			{
				return;
			}

			if (change.find("RD") != NOT_FOUND)
			{
				if (fixedStateID.size() != 0 || eventid.size() != 0 || variableid.size() != 0 || stateCountMultiplier != 0)
				{
					ErrorMessage(1097, format, linecount);
					return;
				}

				rotationDataReplacer(change, format, filename, linecount, groupAnimInfo, order, lastOrder, isLastOrder, animMulti, isChange);
			}

			if (error)
			{
				return;
			}

			if (isChange)
			{
				line.replace(line.find(oldChange), oldChange.length(), change);
			}

			break;
		}
	}
}

void addOnReplacer(string& line, unordered_map<string, vecstr> addOn, ImportContainer addition, unordered_map<string, unordered_map<string, vecstr>> groupAddition, SSMap mixOpt, unsigned int count, string format, int numline)
{
	for (auto it = addOn.begin(); it != addOn.end(); ++it)
	{
		for (unsigned int j = 0; j < it->second.size(); ++j)
		{
			if (line.find(it->first + "[" + it->second[j] + "]", 0) != NOT_FOUND)
			{
				string add = it->first + "[" + it->second[j] + "]";
				int counter = sameWordCount(line, add);

				for (int i = 0; i < counter; ++i)
				{
					size_t pos = line.find(add, 0);

					if (groupAddition[it->first][it->second[j]].size() == 0 || count == -1)
					{
						if (addition[it->first][it->second[j]].length() == 0)
						{
							ErrorMessage(1117, format, numline, line);
							return;
						}

						line.replace(pos, add.length(), addition[it->first][it->second[j]]);
					}
					else
					{
						if (int(groupAddition[it->first][it->second[j]].size()) > count)
						{
							if (groupAddition[it->first][it->second[j]][count].length() == 0)
							{
								ErrorMessage(1117, format, numline, line);
								return;
							}

							line.replace(pos, add.length(), groupAddition[it->first][it->second[j]][count]);
						}
						else
						{
							ErrorMessage(1141);
							return;
						}
					}
				}
			}

			if (line.find(it->first + "*[" + it->second[j] + "]", 0) != NOT_FOUND)
			{
				string add = it->first + "*[" + it->second[j] + "]";
				int counter = sameWordCount(line, add);

				for (int i = 0; i < counter; ++i)
				{
					string option = mixOpt[it->first];
					size_t pos = line.find(add, 0);

					if (groupAddition[option][it->second[j]].size() == 0 || count == -1)
					{
						if (addition[option][it->second[j]].length() == 0)
						{
							ErrorMessage(1117, format, numline, line);
							return;
						}

						line.replace(pos, add.length(), addition[option][it->second[j]]);
					}
					else
					{
						if (int(groupAddition[option][it->second[j]].size()) > count)
						{
							if (groupAddition[option][it->second[j]][count].length() == 0)
							{
								ErrorMessage(1117, format, numline, line);
								return;
							}

							line.replace(pos, add.length(), groupAddition[option][it->second[j]][count]);
						}
						else
						{
							ErrorMessage(1141);
							return;
						}
					}
				}
			}

			if (line.find("[", 0) == NOT_FOUND && line.find("]", 0) == NOT_FOUND)
			{
				break;
			}
		}

		if (line.find("[", 0) == NOT_FOUND && line.find("]", 0) == NOT_FOUND)
		{
			break;
		}
	}
}

void animObjectReplacer(string& line, unordered_map<int, string> AnimObject, int id, string format, int linecount, bool otherAnim, string animNum)
{
	if (otherAnim)
	{
		int reference = sameWordCount(line, format + "[" + animNum + "][@AnimObject/");

		for (int k = 0; k < reference; ++k)
		{
			size_t nextpos = line.find(format + "[" + animNum + "][@AnimObject/");
			string object = line.substr(nextpos);
			string number = boost::regex_replace(string(object), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
			object = format + "[" + animNum + "][@AnimObject/" + number + "]";

			if (line.find(object) != NOT_FOUND && isOnlyNumber(number))
			{
				line.replace(nextpos, object.length(), AnimObject[stoi(number)]);
			}
			else
			{
				ErrorMessage(1108, format, linecount);
				return;
			}
		}

		string object = format + "[" + animNum + "]@AnimObject]";
		reference = sameWordCount(line, object);

		for (int k = 0; k < reference; ++k)
		{
			line.replace(line.find(object), object.length(), AnimObject[id]);
		}
	}
	else
	{
		int reference = sameWordCount(line, "@AnimObject/");

		for (int k = 0; k < reference; ++k)
		{
			size_t nextpos = line.find("@AnimObject/");
			string object = line.substr(nextpos);
			string number = boost::regex_replace(string(object), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
			object = "@AnimObject/" + number;

			if (line.find(object) != NOT_FOUND && isOnlyNumber(number))
			{
				line.replace(nextpos, object.length(), AnimObject[stoi(number)]);
			}
			else
			{
				ErrorMessage(1108, format, linecount);
				return;
			}
		}

		reference = sameWordCount(line, "@AnimObject");

		for (int k = 0; k < reference; ++k)
		{
			line.replace(line.find("@AnimObject"), 11, AnimObject[id]);
		}
	}
}

void Furniture::stateReplacer(string& line, std::string statenum, int stateID, int linecount, bool hasGroup, string otherAnimOrder, bool otherAnim)
{
	if (otherAnim)
	{
		string number = boost::regex_replace(string(line.substr(line.find("][(S" + statenum + "+") + 5 + statenum.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
		string state = format + "[" + otherAnimOrder + "][(S" + statenum + "+" + number + ")]";

		if (!isOnlyNumber(number))
		{
			ErrorMessage(1152, format, linecount, state);
			return;
		}

		if (line.find(state, 0) != NOT_FOUND)
		{
			size_t stateposition = line.find(state, 0);

			if (state == line.substr(stateposition, line.find(")]", stateposition) - stateposition + 2))
			{
				line.replace(stateposition, state.length(), to_string(stateID + stoi(number)));
			}
			else
			{
				ErrorMessage(1137, format, linecount, state);
				return;
			}
		}
	}
	else
	{
		string templine = line.substr(line.find("(S" + statenum + "+"));
		string number = boost::regex_replace(string(templine.substr(statenum.length() + 3)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
		string state = "(S" + statenum + "+" + number + ")";

		if (!isOnlyNumber(number))
		{
			ErrorMessage(1152, format, linecount, state);
			return;
		}

		if (line.find(state, 0) != NOT_FOUND)
		{
			size_t stateposition = line.find(state, 0);

			if (state == line.substr(stateposition, line.find(")", stateposition) - stateposition + 1))
			{
				size_t stateLength = state.length();
				state.replace(1, 1 + statenum.length(), to_string(stateID));
				calculate(state, error);

				if (error)
				{
					ErrorMessage(1151, format, linecount, state);
					return;
				}

				int ID;

				if (statenum.length() > 0)
				{
					ID = stoi(statenum) - 1;
				}
				else
				{
					ID = 0;
				}

				if (ID >= int((*lastState).size()))
				{
					ErrorMessage(1168, format, linecount, state);
					return;
				}

				if (stoi(state) >= (*lastState)[ID])
				{
					(*lastState)[ID] = stoi(state) + 1;
				}

				string tempStateNum = statenum;

				if (hasGroup)
				{
					subFunctionIDs[format + "[" + to_string(order) + "][(S" + statenum + "+" + number + ")]"] = state;
				}
				else
				{
					subFunctionIDs["(S" + statenum + "+" + number + ")"] = state;
				}

				line.replace(stateposition, stateLength, state);
			}
			else
			{
				ErrorMessage(1137, format, linecount, templine.substr(0, templine.find(")")));
				return;
			}
		}
	}
}

string Furniture::addOnConverter(string curline, vecstr elements)
{
	string line = curline;

	addition;

	return line;
}

bool Furniture::specialCondition(string condition, vecstr& storeline, int linecount, animationutility utility)
{
	bool isNot;
	size_t pos;

	if (condition.find("!=") != NOT_FOUND)
	{
		if (condition.find("==") != NOT_FOUND || sameWordCount(condition, "!=") > 1)
		{
			ErrorMessage(1124, format, linecount, condition);
			return false;
		}

		isNot = true;
		pos = condition.find("!=");
	}
	else if (condition.find("==") != NOT_FOUND)
	{
		if (condition.find("!=") != NOT_FOUND || sameWordCount(condition, "==") > 1)
		{
			ErrorMessage(1124, format, linecount, condition);
			return false;
		}

		isNot = false;
		pos = condition.find("==");
	}
	else
	{
		ErrorMessage(1125, format, linecount, condition);
		return false;
	}

	string condition1 = condition.substr(1, pos - 1);
	string condition2 = condition.substr(pos + 2);
	condition2.pop_back();

	if (condition1.length() != 0)
	{
		condition1 = "$" + condition1 + "$";
		processing(condition1, storeline, format, linecount, utility.eventid, utility.variableid, utility.fixedStateID, utility.stateCountMultiplier, utility.hasGroup, utility.optionMulti, utility.animMulti, utility.multiOption);

		if (error)
		{
			return false;
		}

		if (condition.length() == 0)
		{
			ErrorMessage(1172, format, linecount);
			return false;
		}
	}

	if (condition2.length() != 0)
	{
		condition2 = "$" + condition2 + "$";
		processing(condition2, storeline, format, linecount, utility.eventid, utility.variableid, utility.fixedStateID, utility.stateCountMultiplier, utility.hasGroup, utility.optionMulti, utility.animMulti, utility.multiOption);

		if (error)
		{
			return false;
		}

		if (condition.length() == 0)
		{
			ErrorMessage(1172, format, linecount);
			return false;
		}
	}

	if (condition1 != condition2)
	{
		return isNot;
	}
	else
	{
		return !isNot;
	}
}

void eventIDReplacer(string& line, string format, id eventid, string firstEvent, int linecount)
{
	int count = sameWordCount(line, "eventID[");

	for (int i = 0; i < count; ++i)
	{
		size_t nextpos = line.find("eventID[");
		string fullEventName = line.substr(nextpos, line.find("]", nextpos) - nextpos + 1);
		string eventName = fullEventName.substr(8, fullEventName.length() - 9);
		string newEventID = to_string(eventid[eventName]);

		if (newEventID == "0" && eventName != firstEvent)
		{
			if (format == "BASE")
			{
				ErrorMessage(1166);
				return;
			}
			else
			{
				ErrorMessage(1131, format, linecount, eventName);
				return;
			}
		}

		line.replace(line.find(fullEventName), fullEventName.length(), newEventID);
	}
}

void variableIDReplacer(string& line, string format, id variableid, int linecount)
{
	int count = sameWordCount(line, "variableID[");

	for (int i = 0; i < count; ++i)
	{
		size_t nextpos = line.find("variableID[");
		string varName = line.substr(nextpos, line.find("]", nextpos) - nextpos + 2);
		string newVarID = to_string(variableid[varName.substr(11, varName.length() - 12)]);

		if (newVarID == "0")
		{
			if (format == "BASE")
			{
				ErrorMessage(1166);
				return;
			}
			else
			{
				ErrorMessage(1132, format, linecount, varName);
				return;
			}
		}

		line.replace(line.find(varName), varName.length(), newVarID);
	}
}

vecstr GetOptionInfo(string line, string format, int numline, size_t lastOrder, vector<shared_ptr<animationInfo>> groupAnimInfo, bool allowNoFixAnim, bool isCondition, bool isGroup, int animMulti, int optionMulti)
{
	int limiter;
	vecstr optionInfo;

	if (isCondition)
	{
		limiter = 3;
	}
	else
	{
		limiter = 4;
	}

	optionInfo.reserve(limiter);

	if (line.find(format + "[") != NOT_FOUND && groupAnimInfo.size() != 0)
	{
		if (sameWordCount(line, format + "[") > 1)
		{
			ErrorMessage(1157, format, numline, line);
			return optionInfo;
		}

		string templine = line;
		size_t pos = templine.find(format + "[");
		optionInfo.push_back(templine.substr(pos, format.length()));
		templine = templine.substr(templine.find("[", pos) + 1);

		while (true)
		{
			pos = templine.find("]");
			optionInfo.push_back(templine.substr(0, pos));
			size_t optionLength = optionInfo.back().length() + 1;

			if (templine.length() > optionLength && templine[optionLength] == '[')
			{
				templine = templine.substr(templine.find("[") + 1);
			}
			else
			{
				break;
			}
		}

		if (optionInfo[1] == "F")
		{
			optionInfo[1] = "0";
		}
		else if (optionInfo[1] == "L")
		{
			optionInfo[1] = to_string(lastOrder);
		}
		else if (optionInfo[1] == "N")
		{
			if (!isGroup)
			{
				if (lastOrder == animMulti)
				{
					optionInfo[1] = to_string(animMulti);
				}
				else
				{
					optionInfo[1] = to_string(animMulti + 1);
				}
			}
			else
			{
				ErrorMessage(1052, format, numline, line);
				return optionInfo;
			}
		}
		else if (optionInfo[1] == "B")
		{
			if (!isGroup)
			{
				if (animMulti == 0)
				{
					optionInfo[1] = to_string(animMulti);
				}
				else
				{
					optionInfo[1] = to_string(animMulti - 1);
				}
			}
			else
			{
				ErrorMessage(1052, format, numline, line);
				return optionInfo;
			}
		}
		else if (optionInfo[1][0] == '(' && optionInfo[1][optionInfo[1].length() - 1] == ')')
		{
			int Fnum = sameWordCount(optionInfo[1], "F");
			int Nnum = sameWordCount(optionInfo[1], "N");
			int Bnum = sameWordCount(optionInfo[1], "B");
			int Lnum = sameWordCount(optionInfo[1], "L");

			if (isGroup && (Nnum != 0||Bnum))
			{
				ErrorMessage(1052, format, numline, line);
				return optionInfo;
			}

			for (int i = 0; i < Fnum; ++i)
			{
				optionInfo[1].replace(optionInfo[1].find("F"), 1, "0");
			}
			
			if (lastOrder == animMulti)
			{
				for (int i = 0; i < Nnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("N"), 1, to_string(animMulti));
				}
			}
			else
			{
				for (int i = 0; i < Nnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("N"), 1, to_string(animMulti + 1));
				}
			}

			if (animMulti == 0)
			{
				for (int i = 0; i < Bnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("B"), 1, to_string(animMulti));
				}
			}
			else
			{
				for (int i = 0; i < Bnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("B"), 1, to_string(animMulti - 1));
				}
			}

			for (int i = 0; i < Lnum; ++i)
			{
				optionInfo[1].replace(optionInfo[1].find("L"), 1, to_string(lastOrder));
			}

			calculate(optionInfo[1], error);

			if (error)
			{
				return optionInfo;
			}
		}
		else if (optionInfo[1].length() == 0)
		{
			if (!allowNoFixAnim)
			{
				if (!isGroup)
				{
					ErrorMessage(1057, format, numline, line);
				}
				else
				{
					ErrorMessage(1056, format, numline, line);
				}

				return optionInfo;
			}
			else
			{
				if (animMulti == -1)
				{
					if (!isCondition)
					{
						if (!isGroup)
						{
							ErrorMessage(1057, format, numline, line);
						}
						else
						{
							ErrorMessage(1056, format, numline, line);
						}
												
						return optionInfo;
					}
				}
				else
				{
					optionInfo[1] = to_string(animMulti);
				}
			}
		}
		else
		{
			templine = optionInfo[1];
			templine = templine + "a";
			string newtempline = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

			if (newtempline == templine)
			{
				ErrorMessage(1055, format, numline, line);
				return optionInfo;
			}

			if (stoi(optionInfo[1]) > int(lastOrder))
			{
				ErrorMessage(1148, format, numline, line);
				return optionInfo;
			}
		}			

		if (optionInfo[2][optionInfo[2].length() - 1] == '*')
		{
			string option = optionInfo[2].substr(0, optionInfo[2].length() - 1);

			if (groupAnimInfo[stoi(optionInfo[1])]->mixOptRegis[option].length() != 0)
			{
				optionInfo[2] = groupAnimInfo[stoi(optionInfo[1])]->mixOptRegis[option];
			}
			else
			{
				ErrorMessage(1109, format, numline, line);
				return optionInfo;
			}
		}

		if (!isCondition && optionInfo.size() > 3)
		{
			if (optionInfo.size() == 5)
			{
				bool isPassed = true;

				if (optionInfo[4].length() == 0)
				{
					if (optionMulti == -1)
					{
						ErrorMessage(1060, format, numline, line);
						return optionInfo;
					}
					else
					{
						optionInfo[4] = to_string(optionMulti);
					}
				}
				else
				{
					if (!isOnlyNumber(optionInfo[4]))
					{
						ErrorMessage(1055, format, numline, line);
						return optionInfo;
					}
				}
			}

			if (groupAnimInfo[stoi(optionInfo[1])]->groupAddition[optionInfo[2]][optionInfo[3]].size() != 0)
			{
				limiter++;
			}
		}
		
		if (optionInfo.size() > 2 && (optionInfo[2] == "main_anim_event" || optionInfo[2] == "FilePath" || optionInfo[2].find("@AnimObject") != NOT_FOUND || optionInfo[2].find("(S+") != NOT_FOUND))
		{
			limiter--;
		}

		if (int(optionInfo.size()) > limiter || int(optionInfo.size()) < limiter)
		{
			ErrorMessage(1054, limiter - 1, format, numline, line);
			return optionInfo;
		}

		if (optionInfo[2][0] == '^' && isGroup)
		{
			ErrorMessage(1149, format, numline, line);
			return optionInfo;
		}

		if (optionInfo[0] != format)
		{
			ErrorMessage(1051, format, numline, line);
		}
	}
	else
	{
		if (isGroup)
		{
			ErrorMessage(1153, format + "_group", numline, line);
		}
		else
		{
			optionInfo.push_back(format);
			optionInfo.push_back(to_string(animMulti));

			if (line.back() == '*')
			{
				string option = line.substr(0, line.length() - 1);

				if (groupAnimInfo[stoi(optionInfo[1])]->mixOptRegis[option].length() != 0)
				{
					optionInfo.push_back(groupAnimInfo[stoi(optionInfo[1])]->mixOptRegis[option]);
				}
				else
				{
					ErrorMessage(1109, format, numline, line);
					return optionInfo;
				}
			}
			else
			{
				optionInfo.push_back(line);
			}
		}
	}

	return optionInfo;
}

template <class T>
inline int numDigits(T number)
{
	int digits = 0;

	if (number < 0)
	{
		digits = 1; // remove this line if '-' counts as a digit
	}

	while (number)
	{
		number /= 10;
		digits++;
	}

	return digits;
}

string getOption(string curline, bool multi)
{
	if (!multi)
	{
		size_t nextpoint = curline.find("<!-- NEW ^") + 10;
		return curline.substr(nextpoint, curline.find("^ -->", nextpoint) - nextpoint);
	}
	else
	{
		size_t nextpoint = curline.find("<!-- NEW ^") + 10;
		return curline.substr(nextpoint, curline.find("^ +% -->", nextpoint) - nextpoint);
	}
}

inline bool isPassed(int condition, unordered_map<int, bool> IsConditionOpened)
{

	for (int k = condition - 1; k > 0; k--)
	{
		if (!IsConditionOpened[k])
		{
			return false;
		}
	}

	return true;
}

inline bool clearGroupNum(string option2, unordered_map<string, bool> optionPicked, bool isNot, unordered_map<string, bool> groupOption)
{
	string templine = boost::regex_replace(string(option2), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

	if (!groupOption[templine])
	{
		if (isNot)
		{
			return !optionPicked[templine];
		}
		else
		{
			return optionPicked[templine];
		}
	}
	else
	{
		return isNot;
	}
}

bool Furniture::GetSecondCondition(string secondCondition, int numline, vector<unordered_map<string, bool>> groupOptionPicked)
{
	bool isAnotherNot = false;

	if (secondCondition[0] == '!')
	{
		isAnotherNot = true;
		secondCondition = secondCondition.substr(1);
	}

	vecstr optionInfo2 = GetOptionInfo(secondCondition, format, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, false, order);

	if (error)
	{
		return false;
	}

	if (optionInfo2[2][0] == '^')
	{
		string conditionOrder;

		if (isalpha(optionInfo2[2][1]))
		{
			conditionOrder = boost::regex_replace(string(optionInfo2[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

			if (conditionOrder == "last")
			{
				if (isLastOrder)
				{
					if (isAnotherNot)
					{
						conditionOrder = "-1";
					}
					else
					{
						return true;
					}
				}
				else
				{
					if (isAnotherNot)
					{
						return true;
					}
					else
					{
						conditionOrder = "-1";
					}
				}
			}
			else if (conditionOrder == "first")
			{
				conditionOrder = "0";
			}
		}
		else if (isdigit(optionInfo2[2][1]))
		{
			conditionOrder = boost::regex_replace(string(optionInfo2[2]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
		}
		else
		{
			ErrorMessage(1138, format, numline, secondCondition);
			return false;
		}

		if (order == stoi(conditionOrder))
		{
			return !isAnotherNot;
		}
		else
		{
			return isAnotherNot;
		}
	}
	else
	{
		if (groupOptionPicked[stoi(optionInfo2[1])][optionInfo2[2]])
		{
			return !isAnotherNot;
		}
		else
		{
			return clearGroupNum(optionInfo2[2], groupOptionPicked[stoi(optionInfo2[1])], isAnotherNot, groupAnimInfo[stoi(optionInfo2[1])]->groupOption);
		}
	}
}

bool Furniture::GetFirstCondition(string firstCondition, vecstr optionInfo, int numline, vector<unordered_map<string, bool>> groupOptionPicked, bool isNot)
{
	if (optionInfo[2][0] == '^')
	{
		string conditionOrder;

		if (isalpha(optionInfo[2][1]))
		{
			conditionOrder = boost::regex_replace(string(optionInfo[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

			if (conditionOrder == "last")
			{
				if (isLastOrder)
				{
					if (isNot)
					{
						conditionOrder = "-1";
					}
					else
					{
						return true;
					}
				}
				else
				{
					if (isNot)
					{
						return true;
					}
					else
					{
						conditionOrder = "-1";
					}
				}
			}
			else if (conditionOrder == "first")
			{
				conditionOrder = "0";
			}
		}
		else if (isdigit(optionInfo[2][1]))
		{
			conditionOrder = boost::regex_replace(string(optionInfo[2]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
		}
		else
		{
			ErrorMessage(1138, format, numline, firstCondition);
			return false;
		}

		if (order == stoi(conditionOrder))
		{
			return !isNot;
		}
		else
		{
			return isNot;
		}
	}
	else
	{
		if (groupOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
		{
			return !isNot;
		}
		else
		{
			return clearGroupNum(optionInfo[2], groupOptionPicked[stoi(optionInfo[1])], isNot, groupAnimInfo[stoi(optionInfo[1])]->groupOption);
		}
	}
}

void Furniture::addAnimData(unordered_map<string, unordered_map<string, vecstr>> animdata)
{
	animdatalines = animdata;
}

void Furniture::addAnimSetData(unordered_map<string, map<string, vecstr, alphanum_less<string>>> animsetdata)
{
	asdlines = animsetdata;
}

void Furniture::GetAnimData(unordered_map<string, unordered_map<string, vecstr>>& newAnimDataLines)
{
	unordered_map<int, bool> IsConditionOpened;
	vector<unordered_map<string, bool>> groupOptionPicked;
	groupOptionPicked.reserve(groupAnimInfo.size());

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	for (auto it = animdatalines.begin(); it != animdatalines.end(); ++it)
	{
		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
		{
			try
			{
				AnimDataLineProcess(iter->second, newAnimDataLines[it->first][iter->first], format, groupOptionPicked);
			}
			catch (MDException&)
			{
				newAnimDataLines[it->first].erase(iter->first);
			}

			if (error)
			{
				return;
			}
		}

		if (newAnimDataLines[it->first].size() == 0)
		{
			newAnimDataLines.erase(it->first);
		}
	}

	return;
}

void Furniture::GetAnimSetData(unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newASDLines)
{
	unordered_map<int, bool> IsConditionOpened;
	vector<unordered_map<string, bool>> groupOptionPicked;
	groupOptionPicked.reserve(groupAnimInfo.size());

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}
	
	for (auto it = asdlines.begin(); it != asdlines.end(); ++it)
	{
		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
		{
			try
			{
				AnimDataLineProcess(iter->second, newASDLines[it->first][iter->first], format, groupOptionPicked, 1);
			}
			catch (MDException&)
			{
				ErrorMessage(1098);
				return;
			}

			if (error)
			{
				return;
			}
		}
	}

	return;
}

void Furniture::AnimDataLineProcess(vecstr originallines, vecstr& newlines, string format, vector<unordered_map<string, bool>> groupOptionPicked, int ASD)
{
	{
		vecstr emptyVS;
		newlines = emptyVS;
	}

	unordered_map<int, bool> IsConditionOpened;
	vecstr recorder;
	newlines.reserve(originallines.size() + 10 * memory);
	bool skip = false; // mainly used by NEW
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	bool open = false;
	bool multi = false;
	bool newOpen = false;
	int openOrder = -2;
	int condition = 0;
	__int64 openRange = 0;
	__int64 counter = 0;
	string multiOption;
	IsConditionOpened[0] = true;

	for (unsigned int i = 0; i < originallines.size(); ++i)
	{
		bool uniqueskip = false;
		string line = originallines[i];

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			condition++;

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
						string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
						animationutility utility;

						if (newCondition(conditionLine, newlines, groupOptionPicked, i + 1, utility))
						{
							skip = false;
							IsConditionOpened[condition] = true;
						}
						else
						{
							skip = true;
						}

						if (error)
						{
							return;
						}
					}
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				return;
			}

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
						string option = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
						animationutility utility;

						if (newCondition(option, newlines, groupOptionPicked, i + 1, utility))
						{
							skip = false;
							IsConditionOpened[condition] = true;
						}
						else
						{
							skip = true;
						}

						if (error)
						{
							return;
						}
					}
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				return;
			}

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						skip = false;
						IsConditionOpened[condition] = true;
						uniqueskip = true;
						size_t conditionPosition = line.find("<!-- CONDITION") + 14;
						string replacement1 = line.substr(0, conditionPosition + 1);
						string replacement2 = line.substr(conditionPosition);
						newlines.push_back(replacement1 + "START" + replacement2);
					}
					else
					{
						skip = true;
					}
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(originallines[i], false);
					bool isNot = false;

					if (curOption[0] == '!')
					{
						isNot = true;
						curOption = curOption.substr(1);
					}

					vecstr optionInfo = GetOptionInfo(curOption, format, i + 1, lastOrder, groupAnimInfo, false, true, false, order);

					if (error)
					{
						return;
					}

					if (optionInfo[2] != "AnimObject")
					{
						if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[optionInfo[2]])
						{
							if (isNot)
							{
								skip = true;

							}
							else
							{
								open = true;
							}
						}
						else
						{
							// clear group number
							string previous = optionInfo[2];
							string templine = boost::regex_replace(string(optionInfo[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

							if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[templine])
							{
								if (isNot)
								{
									skip = true;
								}
								else
								{
									open = true;
								}
							}
							else
							{
								string ID = boost::regex_replace(string(previous), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								// animobject bypass
								if (previous == "AnimObject/" + ID)
								{
									if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[previous])
									{
										if (isNot)
										{
											skip = true;
										}
										else
										{
											open = true;
										}
									}
									else
									{
										if (isNot)
										{
											open = true;
										}
										else
										{
											skip = true;
										}
									}
								}
								else
								{
									if (isNot)
									{
										open = true;
									}
									else
									{
										skip = true;
									}
								}
							}
						}
					}
					else
					{
						ErrorMessage(1150, format, i + 1);
						skip = true;
					}
				}
				else
				{
					ErrorMessage(1116, format, i + 1);
					return;
				}

				if (error)
				{
					return;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(line, true);
					bool isNot = false;

					if (curOption[0] == '!')
					{
						isNot = true;
						curOption = curOption.substr(1);
					}

					while (true)
					{
						vecstr optionInfo = GetOptionInfo(curOption, format, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true, false, order);
						string animID = boost::regex_replace(string(curOption), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (error)
						{
							return;
						}

						if (optionInfo[2] != "AnimObject/" + animID)
						{
							if (optionInfo[1].length() == 0)
							{
								recorder.reserve(originallines.size() / 5);
								open = true;
								multi = true;
								openOrder = -1;
								multiOption = optionInfo[2];
							}
							else
							{
								if (!groupOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
								{
									// animobject bypass
									if (optionInfo[2] == "AnimObject")
									{
										if (isNot)
										{
											skip = true;
										}
										else
										{
											recorder.reserve(originallines.size() / 5);
											open = true;
											multi = true;
											multiOption = optionInfo[2];
											openOrder = stoi(optionInfo[1]);
										}
									}
									else
									{
										// Check if current condition accepts other options that are linked
										if (isNot)
										{
											recorder.reserve(originallines.size() / 5);
											open = true;
											multi = true;
											multiOption = optionInfo[2];

											if (optionInfo[1].length() == 0)
											{
												openOrder = -1;
											}
											else
											{
												openOrder = stoi(optionInfo[1]);
											}
										}
										else
										{
											skip = true;
										}
									}
								}
								else
								{
									if (isNot)
									{
										skip = true;
									}
									else
									{
										recorder.reserve(originallines.size() / 5);
										open = true;
										multi = true;
										multiOption = optionInfo[2];
										openOrder = stoi(optionInfo[1]);
									}
								}
							}
						}
						else
						{
							WarningMessage(1005, format, i + 1);
							skip = true;
						}

						break;
					}
				}
				else
				{
					ErrorMessage(1115, format, i + 1);
					return;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					size_t orderPosition = line.find("<!-- NEW ORDER ") + 15;
					string curOrder = line.substr(orderPosition, line.find(" -->", orderPosition) - orderPosition);
					bool isNot = false;

					if (curOrder[0] == '!')
					{
						isNot = true;
						curOrder = curOrder.substr(1);
					}

					bool number = false;
					bool word = false;
					bool unknown = false;

					for (unsigned int j = 0; j < curOrder.size(); ++j)
					{
						if (isalpha(curOrder[j]))
						{
							word = true;
						}
						else if (isdigit(curOrder[j]))
						{
							number = true;
						}
						else
						{
							unknown = true;
						}
					}

					if (word && number)
					{
						ErrorMessage(1110, format, i + 1);
						return;
					}
					else if (unknown)
					{
						ErrorMessage(1111, format, i + 1);
						return;
					}
					else if (word)
					{
						if (boost::iequals(curOrder, "last"))
						{
							if (!isLastOrder)
							{
								if (!isNot)
								{
									skip = true;
								}
							}
							else if (isNot)
							{
								skip = true;
							}
						}
						else if (boost::iequals(curOrder, "first"))
						{
							if (order != 0)
							{
								if (!isNot)
								{
									skip = true;
								}
							}
							else if (isNot)
							{
								skip = true;
							}
						}
						else
						{
							ErrorMessage(1112, format, i + 1);
							return;
						}
					}
					else if (number)
					{
						if (order != stoi(curOrder))
						{
							if (!isNot)
							{
								skip = true;
							}
						}
						else if (isNot)
						{
							skip = true;
						}
					}
					else
					{
						ErrorMessage(1113, format, i + 1);
						return;
					}
				}
				else
				{
					ErrorMessage(1114, format, i + 1);
					return;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (!newOpen)
			{
				ErrorMessage(1171, format, i + 1);
				return;
			}

			newOpen = false;
			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			uniqueskip = true;
		}

		if (!skip && !uniqueskip && !freeze)
		{
			while (true)
			{
				if (multi)
				{
					recorder.push_back(line);
					break;
				}

				if (line.find("$") != NOT_FOUND)
				{
					// set animation ID
					if (line.find("$%$", 0) != NOT_FOUND)
					{
						line.replace(line.find("$%$"), 3, to_string(furnitureCount));
					}

					// multi choice selection
					if (line.find("$MC$", 0) != NOT_FOUND)
					{
						animationutility utility;
						multiChoice(line, newlines, groupOptionPicked, i + 1, utility);

						if (error)
						{
							return;
						}
					}

					id emptyID;
					vector<int> emptyVI;
					processing(line, newlines, format, i + 1, emptyID, emptyID, emptyVI, ASD, false);

					if (line.find("\n") != NOT_FOUND)
					{
						__int64 ref = count(line.begin(), line.end(), '\n');
						size_t pos = 0;

						for (__int64 j = 0; j < ref - 1; ++j)
						{
							string addline = line.substr(pos, line.find("\n") - pos);
							pos = line.find("\n", pos) + 1;
							newlines.push_back(addline);
						}

						line.replace(line.find("\n"), 1, "\0");
					}

					if (error)
					{
						return;
					}

					if (line.length() == 0)
					{
						ErrorMessage(1172, format, i + 1);
						return;
					}
				}

				newlines.push_back(line);
				break;
			}
		}

		if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND && IsConditionOpened[condition])
		{
			if (skip)
			{
				skip = false;
			}
			else
			{
				if (multi)
				{
					int size;

					if (openOrder == -2)
					{
						openOrder = 0;
						size = 1;
					}
					else if (openOrder == -1)
					{
						openOrder = 0;
						size = int(groupAnimInfo.size());
					}
					else
					{
						size = openOrder + 1;
					}

					for (int animMulti = openOrder; animMulti < size; ++animMulti)
					{
						for (int optionMulti = 0; optionMulti < groupAnimInfo[animMulti]->optionPickedCount[multiOption]; ++optionMulti)
						{
							for (unsigned int k = 0; k < recorder.size(); ++k)
							{
								string newline = recorder[k];

								if (newline.find("$") != NOT_FOUND)
								{
									// set animation ID
									if (newline.find("$%$", 0) != NOT_FOUND)
									{
										newline.replace(newline.find("$%$"), 3, to_string(furnitureCount));
									}

									// multi choice selection
									if (newline.find("$MC$", 0) != NOT_FOUND)
									{
										id emptyID;
										vector<int> emptyVI;
										animationutility utility(emptyID, emptyID, emptyVI, 0, false, optionMulti, animMulti, multiOption);
										multiChoice(newline, newlines, groupOptionPicked, i + 1, utility);

										if (error)
										{
											return;
										}
									}
								}
								if (newline.find("%", 0) != NOT_FOUND)
								{
									newline.replace(newline.find("%", 0), 1, to_string(optionMulti));
								}

								if (newline.find("$") != NOT_FOUND)
								{
									id emptyID;
									vector<int> emptyVI;
									processing(newline, newlines, format, i + 1 - int(recorder.size()) + k, emptyID, emptyID, emptyVI, ASD, false, optionMulti, animMulti, multiOption);

									if (line.find("\n") != NOT_FOUND)
									{
										__int64 ref = count(line.begin(), line.end(), '\n');
										size_t pos = 0;

										for (__int64 j = 0; j < ref - 1; ++j)
										{
											string addline = line.substr(pos, line.find("\n") - pos);
											pos = line.find("\n", pos) + 1;
											newlines.push_back(addline);
										}

										line.replace(line.find("\n"), 1, "\0");
									}

									if (error)
									{
										return;
									}

									if (line.length() == 0)
									{
										ErrorMessage(1172, format, i + 1);
										return;
									}
								}

								newlines.push_back(newline);
							}
						}
					}

					multiOption.clear();
					openOrder = -2;
				}

				recorder.clear();
			}

			multi = false;
			open = false;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1118, format, i + 1);
				return;
			}

			if (freeze && IsConditionOpened[condition])
			{
				// newlines.push_back(line);
				freeze = false;
			}

			if (isPassed(condition, IsConditionOpened))
			{
				skip = false;
			}
			else
			{
				skip = true;
			}

			IsConditionOpened[condition] = false;
			condition--;
		}

		if (error)
		{
			return;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, originallines.size());
		return;
	}

	for (auto itera = IsConditionOpened.begin(); itera != IsConditionOpened.end(); ++itera)
	{
		if (itera->second)
		{
			ErrorMessage(1145, format);
			return;
			break;
		}
	}

	if (condition != 0)
	{
		ErrorMessage(1122, format);
		return;
	}

	if (ASD == 0)
	{
		if (newlines.back().length() != 0)
		{
			newlines.push_back("");
		}
	}
	else
	{
		if (newlines.back().length() == 0)
		{
			newlines.pop_back();
		}
	}

	newlines.shrink_to_fit();
	return;
}

void Furniture::existingASDProcess(vecstr ASDLines, map<int, vecstr>& extract, int ASD)
{
	unordered_map<int, bool> IsConditionOpened;
	vector<unordered_map<string, bool>> groupOptionPicked;
	groupOptionPicked.reserve(groupAnimInfo.size());

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	vecstr store;
	vecstr recorder;
	bool skip = false; // mainly used by NEW
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	bool open = false;
	bool multi = false;
	bool newOpen = false;
	bool openStore = false;
	int openOrder = -2;
	int condition = 0;
	int curExtract = -1;
	__int64 openRange = 0;
	__int64 counter = 0;
	string multiOption;
	IsConditionOpened[0] = true;

	vecstr newlines;
	newlines.reserve(ASDLines.size() + 10 * memory);
	string line;

	for (unsigned int i = 0; i < ASDLines.size(); ++i)
	{
		line = ASDLines[i];
		bool uniqueskip = false;

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			condition++;

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
						string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
						animationutility utility;

						if (newCondition(conditionLine, newlines, groupOptionPicked, i + 1, utility))
						{
							skip = false;
							IsConditionOpened[condition] = true;
						}
						else
						{
							skip = true;
						}

						if (error)
						{
							newlines.shrink_to_fit();
							return;
						}
					}
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				return;
			}

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
						string option = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
						animationutility utility;

						if (newCondition(option, newlines, groupOptionPicked, i + 1, utility))
						{
							skip = false;
							IsConditionOpened[condition] = true;
						}
						else
						{
							skip = true;
						}

						if (error)
						{
							return;
						}
					}
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				return;
			}

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						skip = false;
						IsConditionOpened[condition] = true;
						uniqueskip = true;
						size_t conditionPosition = line.find("<!-- CONDITION") + 14;
						string replacement1 = line.substr(0, conditionPosition + 1);
						string replacement2 = line.substr(conditionPosition);
						newlines.push_back(replacement1 + "START" + replacement2);
					}
					else
					{
						skip = true;
					}
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(ASDLines[i], false);
					bool isNot = false;

					if (curOption[0] == '!')
					{
						isNot = true;
						curOption = curOption.substr(1);
					}

					vecstr optionInfo = GetOptionInfo(curOption, format, i + 1, lastOrder, groupAnimInfo, false, true, false, order);

					if (error)
					{
						return;
					}

					if (optionInfo[2] != "AnimObject")
					{
						if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[optionInfo[2]])
						{
							if (isNot)
							{
								skip = true;

							}
							else
							{
								open = true;
							}
						}
						else
						{
							// clear group number
							string previous = optionInfo[2];
							string templine = boost::regex_replace(string(optionInfo[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

							if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[templine])
							{
								if (isNot)
								{
									skip = true;
								}
								else
								{
									open = true;
								}
							}
							else
							{
								string ID = boost::regex_replace(string(previous), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								// animobject bypass
								if (previous == "AnimObject/" + ID)
								{
									if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[previous])
									{
										if (isNot)
										{
											skip = true;
										}
										else
										{
											open = true;
										}
									}
									else
									{
										if (isNot)
										{
											open = true;
										}
										else
										{
											skip = true;
										}
									}
								}
								else
								{
									if (isNot)
									{
										open = true;
									}
									else
									{
										skip = true;
									}
								}
							}
						}
					}
					else
					{
						ErrorMessage(1150, format, i + 1);
						skip = true;
					}
				}
				else
				{
					ErrorMessage(1116, format, i + 1);
					return;
				}

				if (error)
				{
					return;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(line, true);
					bool isNot = false;

					if (curOption[0] == '!')
					{
						isNot = true;
						curOption = curOption.substr(1);
					}

					while (true)
					{
						vecstr optionInfo = GetOptionInfo(curOption, format, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true, false, order);
						string animID = boost::regex_replace(string(curOption), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (error)
						{
							return;
						}

						if (optionInfo[2] != "AnimObject/" + animID)
						{
							if (optionInfo[1].length() == 0)
							{
								recorder.reserve(ASDLines.size() / 5);
								open = true;
								multi = true;
								openOrder = -1;
								multiOption = optionInfo[2];
							}
							else
							{
								if (!groupOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
								{
									// animobject bypass
									if (optionInfo[2] == "AnimObject")
									{
										if (isNot)
										{
											skip = true;
										}
										else
										{
											recorder.reserve(ASDLines.size() / 5);
											open = true;
											multi = true;
											multiOption = optionInfo[2];
											openOrder = stoi(optionInfo[1]);
										}
									}
									else
									{
										// Check if current condition accepts other options that are linked
										if (isNot)
										{
											recorder.reserve(ASDLines.size() / 5);
											open = true;
											multi = true;
											multiOption = optionInfo[2];

											if (optionInfo[1].length() == 0)
											{
												openOrder = -1;
											}
											else
											{
												openOrder = stoi(optionInfo[1]);
											}
										}
										else
										{
											skip = true;
										}
									}
								}
								else
								{
									if (isNot)
									{
										skip = true;
									}
									else
									{
										recorder.reserve(ASDLines.size() / 5);
										open = true;
										multi = true;
										multiOption = optionInfo[2];
										openOrder = stoi(optionInfo[1]);
									}
								}
							}
						}
						else
						{
							WarningMessage(1005, format, i + 1);
							skip = true;
						}

						break;
					}
				}
				else
				{
					ErrorMessage(1115, format, i + 1);
					return;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					size_t orderPosition = line.find("<!-- NEW ORDER ") + 15;
					string curOrder = line.substr(orderPosition, line.find(" -->", orderPosition) - orderPosition);
					bool isNot = false;

					if (curOrder[0] == '!')
					{
						isNot = true;
						curOrder = curOrder.substr(1);
					}

					bool number = false;
					bool word = false;
					bool unknown = false;

					for (unsigned int j = 0; j < curOrder.size(); ++j)
					{
						if (isalpha(curOrder[j]))
						{
							word = true;
						}
						else if (isdigit(curOrder[j]))
						{
							number = true;
						}
						else
						{
							unknown = true;
						}
					}

					if (word && number)
					{
						ErrorMessage(1110, format, i + 1);
						return;
					}
					else if (unknown)
					{
						ErrorMessage(1111, format, i + 1);
						return;
					}
					else if (word)
					{
						if (boost::iequals(curOrder, "last"))
						{
							if (!isLastOrder)
							{
								if (!isNot)
								{
									skip = true;
								}
							}
							else if (isNot)
							{
								skip = true;
							}
						}
						else if (boost::iequals(curOrder, "first"))
						{
							if (order != 0)
							{
								if (!isNot)
								{
									skip = true;
								}
							}
							else if (isNot)
							{
								skip = true;
							}
						}
						else
						{
							ErrorMessage(1112, format, i + 1);
							return;
						}
					}
					else if (number)
					{
						if (order != stoi(curOrder))
						{
							if (!isNot)
							{
								skip = true;
							}
						}
						else if (isNot)
						{
							skip = true;
						}
					}
					else
					{
						ErrorMessage(1113, format, i + 1);
						return;
					}
				}
				else
				{
					ErrorMessage(1114, format, i + 1);
					return;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (!newOpen)
			{
				ErrorMessage(1171, format, i + 1);
				return;
			}

			newOpen = false;
			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			uniqueskip = true;
		}

		if (!skip && !uniqueskip && !freeze)
		{
			while (true)
			{
				if (multi)
				{
					if (!openStore)
					{
						openStore = true;
						curExtract = i;
					}

					recorder.push_back(line);
					break;
				}

				if (line.find("$") != NOT_FOUND)
				{
					// set animation ID
					if (line.find("$%$", 0) != NOT_FOUND)
					{
						line.replace(line.find("$%$"), 3, to_string(furnitureCount));
					}

					// multi choice selection
					if (line.find("$MC$", 0) != NOT_FOUND)
					{
						animationutility utility;
						multiChoice(line, newlines, groupOptionPicked, i + 1, utility);

						if (error)
						{
							return;
						}
					}

					id emptyID;
					vector<int> emptyVI;
					processing(line, newlines, format, i + 1, emptyID, emptyID, emptyVI, ASD, false);

					if (line.find("\n") != NOT_FOUND)
					{
						__int64 ref = count(line.begin(), line.end(), '\n');
						size_t pos = 0;

						for (__int64 j = 0; j < ref - 1; ++j)
						{
							string addline = line.substr(pos, line.find("\n") - pos);
							pos = line.find("\n", pos) + 1;
							extract[curExtract].push_back(addline);
						}

						line.replace(line.find("\n"), 1, "\0");
					}

					if (error)
					{
						return;
					}

					if (line.length() == 0)
					{
						ErrorMessage(1172, format, i + 1);
						return;
					}
				}

				if (open || condition > 0)
				{
					if (!openStore)
					{
						openStore = true;
						curExtract = i;
					}

					extract[curExtract].push_back(line);
				}
				else if (openStore)
				{
					openStore = false;
				}

				newlines.push_back(line);
				break;
			}
		}

		if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND && IsConditionOpened[condition])
		{
			if (skip)
			{
				skip = false;
			}
			else
			{
				if (multi)
				{
					int size;

					if (openOrder == -2)
					{
						openOrder = 0;
						size = 1;
					}
					else if (openOrder == -1)
					{
						openOrder = 0;
						size = int(groupAnimInfo.size());
					}
					else
					{
						size = openOrder + 1;
					}

					for (int animMulti = openOrder; animMulti < size; ++animMulti)
					{
						for (int optionMulti = 0; optionMulti < groupAnimInfo[animMulti]->optionPickedCount[multiOption]; ++optionMulti)
						{
							for (unsigned int k = 0; k < recorder.size(); ++k)
							{
								string newline = recorder[k];

								if (newline.find("$") != NOT_FOUND)
								{
									// set animation ID
									if (newline.find("$%$", 0) != NOT_FOUND)
									{
										newline.replace(newline.find("$%$"), 3, to_string(furnitureCount));
									}

									// multi choice selection
									if (newline.find("$MC$", 0) != NOT_FOUND)
									{
										id emptyID;
										vector<int> emptyVI;
										animationutility utility(emptyID, emptyID, emptyVI, 0, false, optionMulti, animMulti, multiOption);
										multiChoice(newline, newlines, groupOptionPicked, i + 1, utility);

										if (error)
										{
											return;
										}
									}
								}
								if (newline.find("%", 0) != NOT_FOUND)
								{
									newline.replace(newline.find("%", 0), 1, to_string(optionMulti));
								}

								if (newline.find("$") != NOT_FOUND)
								{
									id emptyID;
									vector<int> emptyVI;
									processing(newline, newlines, format, i + 1 - int(recorder.size()) + k, emptyID, emptyID, emptyVI, ASD, false, optionMulti, animMulti, multiOption);

									if (line.find("\n") != NOT_FOUND)
									{
										__int64 ref = count(line.begin(), line.end(), '\n');
										size_t pos = 0;

										for (__int64 j = 0; j < ref - 1; ++j)
										{
											string addline = line.substr(pos, line.find("\n") - pos);
											pos = line.find("\n", pos) + 1;
											extract[curExtract].push_back(addline);
										}

										line.replace(line.find("\n"), 1, "\0");
									}

									if (error)
									{
										return;
									}

									if (line.length() == 0)
									{
										ErrorMessage(1172, format, i + 1);
										return;
									}
								}
								
								extract[curExtract].push_back(line);
								newlines.push_back(newline);
							}
						}
					}

					multiOption.clear();
					openOrder = -2;
				}

				recorder.clear();
			}

			multi = false;
			open = false;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1118, format, i + 1);
				return;
			}

			if (freeze && IsConditionOpened[condition])
			{
				// generatedlines.push_back(line);
				freeze = false;
			}

			if (isPassed(condition, IsConditionOpened))
			{
				skip = false;
			}
			else
			{
				skip = true;
			}

			IsConditionOpened[condition] = false;
			condition--;
		}

		if (error)
		{
			return;
		}
	}

	if (newOpen)
	{
		ErrorMessage(1116, format, ASDLines.size());
		return;
	}

	IsConditionOpened[0] = false;

	for (auto itera = IsConditionOpened.begin(); itera != IsConditionOpened.end(); ++itera)
	{
		if (itera->second)
		{
			ErrorMessage(1145, format);
			return;
			break;
		}
	}

	if (condition != 0)
	{
		ErrorMessage(1122, format);
	}
}

void motionDataReplacer(string& change, string format, string filename, int linecount, vector<shared_ptr<animationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool& isChange)
{
	if (change.find(format + "[][MD]", 0) != NOT_FOUND)
	{
		if (animMulti != -1)
		{
			if (groupAnimInfo[animMulti]->motionData.size() == 0)
			{
				MDException out;
				throw out;
			}

			string motionData = to_string(groupAnimInfo[animMulti]->motionData.size()) + "\n";

			for (unsigned int j = 0; j < groupAnimInfo[animMulti]->motionData.size(); ++j)
			{
				motionData.append(groupAnimInfo[animMulti]->motionData[j] + "\n");
			}

			if (motionData.length() == 0)
			{
				WarningMessage(1018, format, linecount);
			}

			change.replace(change.find(format + "[][MD]"), 6 + format.length(), motionData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1146, format, linecount);
			return;
		}
	}

	if (change.find(format + "[F][MD]", 0) != NOT_FOUND)
	{
		if (groupAnimInfo[0]->motionData.size() == 0)
		{
			MDException out;
			throw out;
		}

		string motionData = to_string(groupAnimInfo[0]->motionData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[0]->motionData.size(); ++j)
		{
			motionData.append(groupAnimInfo[0]->motionData[j] + "\n");
		}

		if (motionData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[F][MD]"), 7 + format.length(), motionData);
		isChange = true;
	}

	if (change.find(format + "[N][MD]", 0) != NOT_FOUND)
	{
		int nextorder;

		if (isLastOrder)
		{
			nextorder = order;
		}
		else
		{
			nextorder = order + 1;
		}

		if (groupAnimInfo[nextorder]->motionData.size() == 0)
		{
			MDException out;
			throw out;
		}

		string motionData = to_string(groupAnimInfo[nextorder]->motionData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[nextorder]->motionData.size(); ++j)
		{
			motionData.append(groupAnimInfo[nextorder]->motionData[j] + "\n");
		}

		if (motionData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[N][MD]"), 7 + format.length(), motionData);
		isChange = true;
	}

	if (change.find(format + "[B][MD]", 0) != NOT_FOUND)
	{
		int previousorder;

		if (order == 0)
		{
			previousorder = order;
		}
		else
		{
			previousorder = order - 1;
		}

		if (groupAnimInfo[previousorder]->motionData.size() == 0)
		{
			MDException out;
			throw out;
		}

		string motionData = to_string(groupAnimInfo[previousorder]->motionData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[previousorder]->motionData.size(); ++j)
		{
			motionData.append(groupAnimInfo[previousorder]->motionData[j] + "\n");
		}

		if (motionData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[B][MD]"), 7 + format.length(), motionData);
		isChange = true;
	}

	if (change.find(format + "[L][MD]", 0) != NOT_FOUND)
	{
		if (groupAnimInfo[lastOrder]->motionData.size() == 0)
		{
			MDException out;
			throw out;
		}

		string motionData = to_string(groupAnimInfo[lastOrder]->motionData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[lastOrder]->motionData.size(); ++j)
		{
			motionData.append(groupAnimInfo[lastOrder]->motionData[j] + "\n");
		}

		if (motionData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[L][MD]"), 7 + format.length(), motionData);
		isChange = true;
	}

	if (change.find(format + "[", 0) != NOT_FOUND)
	{
		string number = boost::regex_replace(string(change), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (change.find(format + "[" + number + "][MD]", 0) != NOT_FOUND && isOnlyNumber(number))
		{
			int curOrder = stoi(number);

			if (groupAnimInfo[curOrder]->motionData.size() == 0)
			{
				MDException out;
				throw out;
			}

			string motionData = to_string(groupAnimInfo[curOrder]->motionData.size()) + "\n";

			for (unsigned int j = 0; j < groupAnimInfo[curOrder]->motionData.size(); ++j)
			{
				motionData.append(groupAnimInfo[curOrder]->motionData[j] + "\n");
			}

			if (motionData.length() == 0)
			{
				WarningMessage(1018, format, linecount);
			}

			change.replace(change.find(format + "[" + number + "][MD]"), 6 + format.length() + number.length(), motionData);
			isChange = true;
		}
	}

	if (change.find("MD", 0) != NOT_FOUND)
	{
		if (filename != change)
		{
			if (groupAnimInfo[order]->motionData.size() == 0)
			{
				MDException out;
				throw out;
			}

			string motionData = to_string(groupAnimInfo[order]->motionData.size()) + "\n";

			for (unsigned int j = 0; j < groupAnimInfo[order]->motionData.size(); ++j)
			{
				if (groupAnimInfo[order]->motionData[j].length() == 0)
				{
					WarningMessage(1018, format, linecount);
				}

				motionData.append(groupAnimInfo[order]->motionData[j] + "\n");
			}

			change.replace(change.find("MD"), 2, motionData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1134, format, linecount);
			return;
		}
	}
}

void rotationDataReplacer(string& change, string format, string filename, int linecount, vector<shared_ptr<animationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool&isChange)
{
	if (change.find(format + "[][RD]", 0) != NOT_FOUND)
	{
		if (animMulti != -1)
		{
			string rotationData = to_string(groupAnimInfo[animMulti]->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < groupAnimInfo[animMulti]->rotationData.size(); ++j)
			{
				rotationData.append(groupAnimInfo[animMulti]->rotationData[j] + "\n");
			}

			if (rotationData.length() == 0)
			{
				WarningMessage(1018, format, linecount);
			}

			change.replace(change.find(format + "[][RD]"), 6 + format.length(), rotationData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1146, format, linecount);
			return;
		}
	}

	if (change.find(format + "[F][RD]", 0) != NOT_FOUND)
	{
		string rotationData = to_string(groupAnimInfo[0]->rotationData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[0]->rotationData.size(); ++j)
		{
			rotationData.append(groupAnimInfo[0]->rotationData[j] + "\n");
		}

		if (rotationData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[F][RD]"), 7 + format.length(), rotationData);
		isChange = true;
	}

	if (change.find(format + "[N][RD]", 0) != NOT_FOUND)
	{
		int nextorder;

		if (isLastOrder)
		{
			nextorder = order;
		}
		else
		{
			nextorder = order + 1;
		}

		string rotationData = to_string(groupAnimInfo[nextorder]->rotationData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[nextorder]->rotationData.size(); ++j)
		{
			rotationData.append(groupAnimInfo[nextorder]->rotationData[j] + "\n");
		}

		if (rotationData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[N][RD]"), 7 + format.length(), rotationData);
		isChange = true;
	}

	if (change.find(format + "[B][RD]", 0) != NOT_FOUND)
	{
		int previousorder;

		if (order == 0)
		{
			previousorder = order;
		}
		else
		{
			previousorder = order - 1;
		}

		string rotationData = to_string(groupAnimInfo[previousorder]->rotationData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[previousorder]->rotationData.size(); ++j)
		{
			rotationData.append(groupAnimInfo[previousorder]->rotationData[j] + "\n");
		}

		if (rotationData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[B][RD]"), 7 + format.length(), rotationData);
		isChange = true;
	}

	if (change.find(format + "[L][RD]", 0) != NOT_FOUND)
	{
		string rotationData = to_string(groupAnimInfo[order]->rotationData.size()) + "\n";

		for (unsigned int j = 0; j < groupAnimInfo[lastOrder]->rotationData.size(); ++j)
		{
			rotationData.append(groupAnimInfo[lastOrder]->rotationData[j] + "\n");
		}

		if (rotationData.length() == 0)
		{
			WarningMessage(1018, format, linecount);
		}

		change.replace(change.find(format + "[L][RD]"), 7 + format.length(), rotationData);
		isChange = true;
	}

	if (change.find(format + "[", 0) != NOT_FOUND)
	{
		string number = boost::regex_replace(string(change), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (change.find(format + "[" + number + "][RD]", 0) != NOT_FOUND && isOnlyNumber(number))
		{
			int curOrder = stoi(number);
			string rotationData = to_string(groupAnimInfo[curOrder]->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < groupAnimInfo[curOrder]->rotationData.size(); ++j)
			{
				rotationData.append(groupAnimInfo[curOrder]->rotationData[j] + "\n");
			}

			if (rotationData.length() == 0)
			{
				WarningMessage(1018, format, linecount);
			}

			change.replace(change.find(format + "[" + number + "][RD]"), 6 + format.length() + number.length(), rotationData);
			isChange = true;
		}
	}

	if (change.find("RD", 0) != NOT_FOUND)
	{
		if (filename != change)
		{
			string rotationData = to_string(groupAnimInfo[order]->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < groupAnimInfo[order]->rotationData.size(); ++j)
			{
				rotationData.append(groupAnimInfo[order]->rotationData[j] + "\n");
			}

			if (rotationData.length() == 0)
			{
				WarningMessage(1018, format, linecount);
			}

			change.replace(change.find("RD"), 2, rotationData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1134, format, linecount);
			return;
		}
	}
}

void CRC32Replacer(string& line, string format, int linecount)
{
	__int64 bracketCount = count(line.begin(), line.end(), '[');
	__int64 altBracketCount = count(line.begin(), line.end(), ']');

	if (bracketCount != altBracketCount)
	{
		ErrorMessage(1139, format, linecount, line);
		return;
	}

	int counter = 1;
	size_t pos = line.find("crc32[");
	size_t nextpos;

	for (unsigned int j = pos + 6; j < line.length(); ++j)
	{
		if (line[j] == '[')
		{
			++counter;
		}
		else if (line[j] == ']')
		{
			--counter;

			if (counter == 0)
			{
				nextpos = j + 1;
				break;
			}
		}
	}

	string fullline = line.substr(pos, nextpos - pos);
	pos += 6;
	--nextpos;
	string crc32line = boost::to_lower_copy(line.substr(pos, nextpos - pos));
	line.replace(line.find(fullline), fullline.length(), to_string(CRC32Convert(crc32line)));
}

inline unsigned int CRC32Convert(string line)
{
	boost::crc_optimal<32, 0x4C11DB7, 0, 0, true, true> result;
	result.process_bytes(line.data(), line.length());
	return result.checksum();
}
