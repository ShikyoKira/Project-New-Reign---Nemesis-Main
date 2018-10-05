#include "furniture.h"
#include "alternateanimation.h"

#pragma warning(disable:4503)

using namespace std;

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

void Furniture::GetFurnitureLine(shared_ptr<vecstr> generatedlines, string curBehaviorFile, int nFunctionID, ImportContainer& import, id eventid, id variableid,
	vector<int>& stateID, vector<int> stateCountMultiplier, bool hasGroup, bool isCore, shared_ptr<group> groupFunction, shared_ptr<single> singleFunction, newAnimLock& animLock)
{
	vecstr recorder;

	behaviorFile = curBehaviorFile;
	newImport = const_cast<ImportContainer*>(&import);
	atomicLock = const_cast<newAnimLock*>(&animLock);
	nextFunctionID = nFunctionID;
	subFunctionIDs = singleFunction;

	bool negative = false;
	bool skip = false; // mainly used by NEW
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	bool open = false;
	bool norElement = false;
	bool multi = false;
	bool newOpen = false;
	int openOrder = -2;
	int condition = 0;
	vector<int> fixedStateID;
	__int64 openRange = 0;
	__int64 counter = 0;
	string multiOption;
	size_t elementLine = -1;
	unordered_map<int, bool> IsConditionOpened;
	vector<unordered_map<string, bool>> groupOptionPicked;

	strID = to_string(nextFunctionID);

	while (strID.length() < 4)
	{
		strID = "0" + strID;
	}

	for (auto ID : stateID)
	{
		fixedStateID.push_back(ID);
	}

	groupOptionPicked.reserve(groupAnimInfo.size());

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	IsConditionOpened[0] = true;
	generatedlines->reserve(furniturelines[behaviorFile].size() + 10 * memory);

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

				if (!freeze && !multi && ((newOpen && !skip) || !newOpen))
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
							string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
							animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);

							if (newCondition(conditionLine, *generatedlines, groupOptionPicked, i + 1, utility))
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
					ErrorMessage(1119, format, behaviorFile, i + 1);
					return;
				}

				if (!freeze && !multi && ((newOpen && !skip) || !newOpen))
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
							string option = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
							animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);

							if (newCondition(option, *generatedlines, groupOptionPicked, i + 1, utility))
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
					ErrorMessage(1119, format, behaviorFile, i + 1);
					return;
				}

				if (!freeze && !multi && ((newOpen && !skip) || !newOpen))
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
							generatedlines->push_back(replacement1 + "START" + replacement2);
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
					ErrorMessage(1116, format, behaviorFile, i + 1);
					return;
				}

				if (IsConditionOpened[condition])
				{
					if (!open)
					{
						string curOption = getOption(line, false);
						bool isNot = false;

						if (curOption[0] == '!')
						{
							isNot = true;
							curOption = curOption.substr(1);
						}

						vecstr optionInfo = GetOptionInfo(curOption, format, behaviorFile, i + 1, lastOrder, groupAnimInfo, false, true, order);

						if (error)
						{
							return;
						}

						if (optionInfo[2].find("AnimObject") != NOT_FOUND)
						{
							ErrorMessage(1150, format, behaviorFile, i + 1);
							skip = true;
						}

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
					else
					{
						ErrorMessage(1116, format, behaviorFile, i + 1);
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
					ErrorMessage(1116, format, behaviorFile, i + 1);
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

						vecstr optionInfo = GetOptionInfo(curOption, format, behaviorFile, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true, order);

						if (error)
						{
							return;
						}

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
						ErrorMessage(1115, format, behaviorFile, i + 1);
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
					ErrorMessage(1116, format, behaviorFile, i + 1);
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

						if (isOnlyNumber(curOrder))
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
							bool word = false;
							bool unknown = false;
							bool number = false;

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

							if (word & number)
							{
								ErrorMessage(1110, format, behaviorFile, i + 1);
								return;
							}
							else if (unknown)
							{
								ErrorMessage(1111, format, behaviorFile, i + 1);
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
									ErrorMessage(1112, format, behaviorFile, i + 1);
									return;
								}
							}
							else
							{
								ErrorMessage(1113, format, behaviorFile, i + 1);
								return;
							}
						}
					}
					else
					{
						ErrorMessage(1114, format, behaviorFile, i + 1);
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
					ErrorMessage(1171, format, behaviorFile, i + 1);
					return;
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
						ErrorMessage(1136, format, behaviorFile, i + 1);
						return;
					}
				}
				else if (line.find("</hkparam>") != NOT_FOUND && norElement)
				{
					string templine = line.substr(0, line.find("</hkparam>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						string oldElement;

						if ((*generatedlines)[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
						{
							size_t position = (*generatedlines)[elementLine].find("numelements=\"") + 13;
							oldElement = (*generatedlines)[elementLine].substr(position, (*generatedlines)[elementLine].find("\">", position) - position);
						}
						else
						{
							oldElement = "$elements$";
						}

						if (oldElement != to_string(counter))
						{
							(*generatedlines)[elementLine].replace((*generatedlines)[elementLine].find(oldElement), oldElement.length(), to_string(counter));
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
						multiChoice(line, *generatedlines, groupOptionPicked, i + 1, utility);

						if (error)
						{
							return;
						}
					}

					// set group function ID
					if (line.find(format + "_group$") != NOT_FOUND)
					{
						size_t pos = line.find(format + "_group$");
						string ID = boost::regex_replace(string(line.substr(pos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
						pos = line.find(format + "_group$" + ID, pos);
						string oldID = "MID$" + ID;

						if (pos != NOT_FOUND)
						{
							animLock.lockID();

							if (groupFunction->functionIDs.find(oldID) != groupFunction->functionIDs.end())
							{
								line.replace(pos, format.length() + 7 + ID.length(), groupFunction->functionIDs[oldID]);
							}
							else
							{
								groupFunction->functionIDs[oldID] = strID;
								line.replace(pos, format.length() + 7 + ID.length(), strID);
								newID();
							}

							animLock.releaseID();
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
								subFunctionIDs->format[templine] = ID;
								line.replace(line.find("MID$", MIDposition), oldID.length(), ID);
							}
							else
							{
								ErrorMessage(1130, format, behaviorFile, i + 1);
								return;
							}
						}
					}

					processing(line, *generatedlines, format, i + 1, eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup);

					if (error)
					{
						return;
					}

					if (line.length() == 0)
					{
						ErrorMessage(1172, format, behaviorFile, i + 1);
						return;
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
				else if (line.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
				{
					negative = true;
				}
				else if (negative && line.find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
				{
					if (line.find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
					{
						size_t pos = line.find("<hkparam name=\"relativeToEndOfClip\">") + 36;
						line.replace(pos, line.find("</hkparam>", pos) - pos, "true");
					}

					negative = false;
				}

				generatedlines->push_back(line);

				if (elementCatch)
				{
					elementLine = generatedlines->size() - 1;
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
						int openClose = condition;
						bool group = false;
						bool skip2 = false;
						bool freeze2 = false;

						if (openOrder == -2)
						{
							openOrder = 0;
							size = 1;
						}
						else if (openOrder == -1)
						{
							group = true;
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
									bool uniqueskip2 = false;
									string newline = recorder[k];

									if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
									{
										condition++;

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
													string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
													animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup, optionMulti, animMulti);

													if (newCondition(conditionLine, *generatedlines, groupOptionPicked, i + 1 - int(recorder.size()) + k, utility))
													{
														skip2 = false;
														IsConditionOpened[condition] = true;
													}
													else
													{
														skip2 = true;
													}

													if (error)
													{
														return;
													}
												}
											}
											else
											{
												skip2 = true;
												freeze2 = true;
											}
										}

										uniqueskip2 = true;
									}
									else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1119, format, behaviorFile, i + 1 - int(recorder.size()) + k);
											return;
										}

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
													string option = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
													animationutility utility(eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup, optionMulti, animMulti);

													if (newCondition(option, *generatedlines, groupOptionPicked, i + 1 - int(recorder.size()) + k, utility))
													{
														skip2 = false;
														IsConditionOpened[condition] = true;
													}
													else
													{
														skip2 = true;
													}

													if (error)
													{
														return;
													}
												}
											}
											else
											{
												skip2 = true;
												freeze2 = true;
											}
										}

										uniqueskip2 = true;
									}
									else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1119, format, behaviorFile, i + 1 - int(recorder.size()) + k);
											return;
										}

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													skip2 = false;
													IsConditionOpened[condition] = true;
													uniqueskip2 = true;
													size_t conditionPosition = line.find("<!-- CONDITION") + 14;
													string replacement1 = line.substr(0, conditionPosition + 1);
													string replacement2 = line.substr(conditionPosition);
													generatedlines->push_back(replacement1 + "START" + replacement2);
												}
												else
												{
													skip2 = true;
												}
											}
											else
											{
												skip2 = true;
												freeze2 = true;
											}
										}

										uniqueskip2 = true;
									}

									if (!skip2 && !uniqueskip2 && !freeze2)
									{
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
												multiChoice(newline, *generatedlines, groupOptionPicked, i + 1 - int(recorder.size()) + k, utility);

												if (error)
												{
													return;
												}
											}

											// set group function ID
											if (line.find(format + "_group$") != NOT_FOUND)
											{
												size_t pos = line.find(format + "_group$");
												string ID = boost::regex_replace(string(line.substr(pos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
												pos = line.find(format + "_group$" + ID, pos);
												string oldID = "MID$" + ID;

												if (pos != NOT_FOUND)
												{
													animLock.lockID();

													if (groupFunction->functionIDs.find(oldID) != groupFunction->functionIDs.end())
													{
														line.replace(pos, format.length() + 7 + ID.length(), groupFunction->functionIDs[oldID]);
													}
													else
													{
														groupFunction->functionIDs[oldID] = strID;
														line.replace(pos, format.length() + 7 + ID.length(), strID);
														newID();
													}

													animLock.releaseID();
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
														subFunctionIDs->format[templine] = ID;
														newline.replace(newline.find("MID$", MIDposition), oldID.length(), ID);
													}
													else
													{
														ErrorMessage(1130, format, behaviorFile, i + 1 - int(recorder.size()) + k);
														return;
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
											try
											{
												processing(newline, *generatedlines, format, i + 1 - int(recorder.size()) + k, eventid, variableid, fixedStateID, stateCountMultiplier, hasGroup, optionMulti, animMulti, multiOption);
											}
											catch (MDException&)
											{
												while (newline.find("MD") != NOT_FOUND)
												{
													newline.replace(newline.find("MD"), 2, "0");
												}
											}

											if (error)
											{
												return;
											}

											if (newline.length() == 0)
											{
												ErrorMessage(1172, format, behaviorFile, i + 1 - int(recorder.size()) + k);
												return;
											}
										}

										size_t pos = newline.find("(");

										if (pos != NOT_FOUND && newline.find(")", pos) != NOT_FOUND)
										{
											string templine = newline.substr(pos);
											int nextpos = openEndBracket(templine, '(', ')', format, behaviorFile, i + 1) + 1;

											if (error)
											{
												return;
											}

											templine = templine.substr(0, nextpos);
											string oldline = templine;
											calculate(templine, format, behaviorFile, i + 1);

											if (error)
											{
												return;
											}

											if (oldline != templine)
											{
												newline.replace(pos, oldline.length(), templine);
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
										else if (newline.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
										{
											negative = true;
										}
										else if (negative && newline.find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
										{
											if (newline.find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
											{
												size_t pos = newline.find("<hkparam name=\"relativeToEndOfClip\">") + 36;
												newline.replace(pos, newline.find("</hkparam>", pos) - pos, "true");
											}

											negative = false;
										}

										generatedlines->push_back(newline);
									}
								}
							}
						}

						if (openClose != condition)
						{
							ErrorMessage(1122, format, behaviorFile);
							return;
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
					ErrorMessage(1118, format, behaviorFile, i + 1);
					return;
				}

				if (!multi && ((newOpen && !skip) || !newOpen))
				{
					if (freeze && IsConditionOpened[condition])
					{
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
				}

				condition--;
			}
		}

		if (error)
		{
			return;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, behaviorFile, furniturelines[behaviorFile].size());
		return;
	}

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			ErrorMessage(1145, format, behaviorFile);
			return;
			break;
		}
	}

	if (condition != 0)
	{
		ErrorMessage(1122, format, behaviorFile);
		return;
	}

	for (auto it = addOn.begin(); it != addOn.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			string optionName;

			if (addition[it->first][it->second[i]].length() != 0)
			{
				optionName = it->first;
				subFunctionIDs->format[optionName + "[" + it->second[i] + "]"] = addition[optionName][it->second[i]];
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
							subFunctionIDs->format[optionName + "[" + it->second[i] + "][" + to_string(j) + "]"] = groupAddition[optionName][it->second[i]][j];
						}
					}
				}
				else
				{
					optionName = it->first;

					for (unsigned int j = 0; j < groupAddition[optionName][it->second[i]].size(); ++j)
					{
						subFunctionIDs->format[optionName + "[" + it->second[i] + "][" + to_string(j) + "]"] = groupAddition[optionName][it->second[i]][j];
					}
				}

			}
		}
	}

	if (AnimObject.size() != 0)
	{
		for (auto it = AnimObject.begin(); it != AnimObject.end(); ++it)
		{
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				subFunctionIDs->format["@AnimObject/" + to_string(it->first) + "[" + to_string(i) + "]"] = it->second[i];
			}
		}
	}

	if (fixedStateID.size() > 1)
	{
		for (unsigned int i = 0; i < fixedStateID.size(); ++i)
		{
			for (int j = 0; j < stateCountMultiplier[i]; ++j)
			{
				subFunctionIDs->format["(S" + to_string(i + 1) + "+" + to_string(j) + ")"] = to_string(fixedStateID[i] + j);
			}
		}
	}
	else
	{
		for (int j = 0; j < stateCountMultiplier[0]; ++j)
		{
			subFunctionIDs->format["(S+" + to_string(j) + ")"] = to_string(fixedStateID[0] + j);
		}
	}

	subFunctionIDs->format["main_anim_event"] = mainAnimEvent;
	subFunctionIDs->format["FilePath"] = filepath + filename;
	subFunctionIDs->format["FileName"] = filename.substr(0, filename.find_last_of("."));

	if (generatedlines->size() != 0)
	{
		if (!generatedlines->back().empty())
		{
			generatedlines->push_back("");
		}
		else if (generatedlines->size() > 1)
		{
			while (generatedlines->back().empty())
			{
				if ((*generatedlines)[generatedlines->size() - 2].empty())
				{
					generatedlines->pop_back();
				}
				else
				{
					break;
				}
			}
		}
	}

	generatedlines->shrink_to_fit();
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
	++(nextFunctionID);

	if (nextFunctionID == 9216)
	{
		++(nextFunctionID);
	}

	strID = to_string(nextFunctionID);

	while (strID.length() < 4)
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

		if (temp == 0 || temp > groupAnimInfo[order]->animObjectCount)
		{
			ErrorMessage(1144, listFilename, linecount);
			return;
		}

		optionPicked["AnimObject/" + to_string(temp)] = true;
		AnimObject[temp].push_back(ObjectName);
	}
}

void Furniture::setZeroEvent(string eventname)
{
	ZeroEvent = eventname;
}

void Furniture::setZeroVariable(string variablename)
{
	ZeroVariable = variablename;
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

int Furniture::getNextID(string behavior)
{
	unordered_map<int, bool> taken;
	int IDUsed = 0;

	for (auto& line : furniturelines[behavior])
	{
		if (line.find("<hkobject name=\"#MID$") != NOT_FOUND)
		{
			string number = boost::regex_replace(string(line), boost::regex(".*<hkobject name=\"#MID[$]([0-9]+)\" class=\".*"), string("\\1"));

			if (number != line && isOnlyNumber(number))
			{
				int num = stoi(number);

				if (!taken[num])
				{
					taken[stoi(number)] = true;
					++IDUsed;
				}
			}
		}
		else
		{
			size_t pos = line.find("import[");

			if (pos != NOT_FOUND && line.find("]", pos) != NOT_FOUND)
			{
				++IDUsed;
			}
		}
	}

	return IDUsed;
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
			ErrorMessage(1100, format, behaviorFile, numline);
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
				ErrorMessage(1104, format, behaviorFile, numline);
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

				inHouseResult = firstConditionProcess(inHouse, groupOptionPicked, isNot, numline, utility);

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
				vecstr optionInfo;

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}
				
				inHouseResult = firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility);

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
				vecstr optionInfo;

				size_t x = tempSecondCondition.find("&");
				size_t y = tempSecondCondition.find("|");

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}

				// bool1 & bool2 | bool3 ....
				if ((x == NOT_FOUND || x > y) && y != NOT_FOUND)
				{
					secondCondition = tempSecondCondition.substr(0, tempSecondCondition.find("|"));
					tempSecondCondition = tempSecondCondition.substr(tempSecondCondition.find("|") + 1);

					if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
					{
						inHouseResult = newCondition(secondCondition, storeline, groupOptionPicked, numline, utility);
					}
					else
					{
						inHouseResult = false;
					}

					if (error)
					{
						return false;
					}

					if (!inHouseResult && newCondition(tempSecondCondition, storeline, groupOptionPicked, numline, utility))
					{
						inHouseResult = true;
					}

				}
				else
				{
					secondCondition = tempSecondCondition;

					if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
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
		}
		else
		{
			ErrorMessage(1105, format, behaviorFile, numline);
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
				if (error)
				{
					return false;
				}

				return true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string secondCondition = outHouse.substr(outHouse.find("&") + 1);

			if (inHouseResult && newCondition(secondCondition, storeline, groupOptionPicked, numline, utility))
			{
				if (error)
				{
					return false;
				}

				return true;
			}
		}
		else
		{
			ErrorMessage(1106, format, behaviorFile, numline, condition);
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

				return firstConditionProcess(conditionOrder, groupOptionPicked, isNot, numline, utility);
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
				vecstr optionInfo;

				if (firstCondition[0] == '!')
				{
					isNot = true;
					firstCondition = firstCondition.substr(1);
				}

				if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
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
								ErrorMessage(1101, format, behaviorFile, numline);
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
								if (secondConditionProcess(secondCondition, groupOptionPicked, numline, utility))
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
							ErrorMessage(1103, format, behaviorFile, numline);
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
							if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
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
								if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
								{
									if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility) && newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
									{
										return true;
									}
								}
							}
							else if (logic == '|')
							{
								if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
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
								ErrorMessage(1101, format, behaviorFile, numline);
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
							if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
							{
								if (secondConditionProcess(secondCondition, groupOptionPicked, numline, utility))
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

							if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
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

							if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
							{
								if (newCondition(secondCondition, storeline, groupOptionPicked, numline, utility) && newCondition(thirdCondition, storeline, groupOptionPicked, numline, utility))
								{
									return true;
								}
							}
						}
						else
						{
							ErrorMessage(1103, format, behaviorFile, numline);
							return false;
						}
					}
				}
				else
				{
					if (firstConditionProcess(firstCondition, groupOptionPicked, isNot, numline, utility))
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
			ErrorMessage(1102, format, behaviorFile, numline);
			return false;
		}
	}

	return false;
}

void Furniture::processing(string& line, vecstr& storeline, string masterFormat, int linecount, id eventid, id variableid, vector<int> fixedStateID, vector<int> stateCountMultiplier, bool hasGroup, int optionMulti, int animMulti, string multiOption)
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

							calculate(equation, format, behaviorFile, linecount);

							if (error)
							{
								return;
							}

							if (stoi(equation) > int(groupAnimInfo.size() - 1) || stoi(equation) < 0)
							{
								ErrorMessage(1148, format, behaviorFile, linecount, change);
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
					ErrorMessage(1107, format, behaviorFile, linecount, change);
					return;
				}

				if (change.find(format + "[][END]", 0) != NOT_FOUND)
				{
					if (animMulti == -1)
					{
						ErrorMessage(1057, format, behaviorFile, linecount, change);
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
							ErrorMessage(1155, lastOrder , format, behaviorFile, linecount, change);
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

					if (intID >= int(fixedStateID.size()) || intID >= int(stateCountMultiplier.size()) || intID < 0)
					{
						ErrorMessage(1168, format, behaviorFile, linecount, templine.substr(0, templine.find(")") + 1));
						return;
					}
				}

				if (change.find(format + "[][(S" + ID + "+", 0) != NOT_FOUND)
				{
					stateReplacer(change, ID, fixedStateID[intID] + ((animMulti - order) * stateCountMultiplier[intID]), linecount, hasGroup, "", true);
					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[F][(S" + ID + "+", 0) != NOT_FOUND)
				{
					stateReplacer(change, ID, fixedStateID[intID] - (order * stateCountMultiplier[intID]), linecount, hasGroup, "F", true);
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
						stateReplacer(change, ID, fixedStateID[intID] + stateCountMultiplier[intID], linecount, hasGroup, "N", true);
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
						stateReplacer(change, ID, fixedStateID[intID] - stateCountMultiplier[intID], linecount, hasGroup, "B", true);
					}

					isChange = true;

					if (error)
					{
						return;
					}
				}

				if (change.find(format + "[L][(S" + ID + "+", 0) != NOT_FOUND)
				{
					stateReplacer(change, ID, fixedStateID[intID] + ((lastOrder - order) * stateCountMultiplier[intID]), linecount, hasGroup, "L", true);
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
								stateReplacer(change, ID, fixedStateID[intID] + ((stoi(number) - order) * stateCountMultiplier[intID]), linecount, hasGroup, number, true);
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
						ErrorMessage(1146, format, behaviorFile, linecount);
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
						ErrorMessage(1134, format, behaviorFile, linecount);
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
						ErrorMessage(1146, format, behaviorFile, linecount);
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
						ErrorMessage(1134, format, behaviorFile, linecount);
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
						ErrorMessage(1146, format, behaviorFile, linecount);
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
						ErrorMessage(1134, format, behaviorFile, linecount);
						return;
					}
				}
			}

			// set AnimObject
			if (change.find("@AnimObject/", 0) != NOT_FOUND)
			{
				if (!multiAnim)
				{
					ErrorMessage(1126, format, behaviorFile, linecount, change);
					return;
				}

				if (change.find(format + "[", 0) != NOT_FOUND && change.find("][@AnimObject/", 0) != NOT_FOUND)
				{
					int counter = sameWordCount(change, "][@AnimObject/");

					for (int k = 0; k < counter; ++k)
					{
						size_t pos = change.find("][@AnimObject/");
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
						vecstr optionInfo = GetOptionInfo(change.substr(nextpos, pos - nextpos), format, behaviorFile, linecount, lastOrder, groupAnimInfo, true, false, order, animMulti, optionMulti);

						if (error)
						{
							return;
						}

						if (optionInfo[0] == format && optionInfo[2].find("@AnimObject/") != NOT_FOUND)
						{
							int groupNum;
							pos = change.find(format + "[") + 1 + format.length();
							string animNumStr = change.substr(pos, change.find("]", pos) - pos);

							if (optionInfo[1].length() == 0)
							{
								if (!multiAnim)
								{
									ErrorMessage(1126, format, behaviorFile, linecount, change);
									return;
								}

								groupNum = animMulti;
							}
							else
							{
								groupNum = stoi(optionInfo[1]);
							}

							animObjectReplacer(change, behaviorFile, groupAnimInfo[groupNum]->AnimObject, format, linecount, optionMulti, true, animNumStr);
							isChange = true;

							if (error)
							{
								return;
							}
						}
					}
				}

				if (change.find("@AnimObject/", 0) != NOT_FOUND)
				{
					int counter = sameWordCount(change, "@AnimObject/");

					for (int k = 0; k < counter; ++k)
					{
						animObjectReplacer(change, behaviorFile, AnimObject, format, linecount, optionMulti);
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
				if (change.find("[" + format + "[][main_anim_event]]", 0) != NOT_FOUND && change.find("eventID[" + format + "[][main_anim_event]]") == NOT_FOUND)
				{
					change.replace(change.find("[" + format + "[][main_anim_event]]"), 21 + format.length(), groupAnimInfo[animMulti]->mainAnimEvent);
					isChange = true;
				}
				else if (change.find(format + "[][main_anim_event]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[][main_anim_event]"), 19 + format.length(), groupAnimInfo[animMulti]->mainAnimEvent);
					isChange = true;
				}
				
				if (change.find("[" + format + "[F][main_anim_event]]", 0) != NOT_FOUND && change.find("eventID[" + format + "[F][main_anim_event]]", 0) == NOT_FOUND)
				{
					change.replace(change.find("[" + format + "[F][main_anim_event]]"), 22 + format.length(), groupAnimInfo[0]->mainAnimEvent);
					isChange = true;
				}
				else if (change.find(format + "[F][main_anim_event]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[F][main_anim_event]"), 20 + format.length(), groupAnimInfo[0]->mainAnimEvent);
					isChange = true;
				}

				if (change.find("[" + format + "[N][main_anim_event]]", 0) != NOT_FOUND && change.find("eventID[" + format + "[N][main_anim_event]]", 0) == NOT_FOUND)
				{
					if (isLastOrder)
					{
						change.replace(change.find("[" + format + "[N][main_anim_event]]"), 22 + format.length(), mainAnimEvent);
					}
					else
					{
						change.replace(change.find("[" + format + "[N][main_anim_event]]"), 22 + format.length(), groupAnimInfo[order + 1]->mainAnimEvent);
					}

					isChange = true;
				}
				else if (change.find(format + "[N][main_anim_event]", 0) != NOT_FOUND)
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
				}
				
				if (change.find("[" + format + "[L][main_anim_event]]", 0) != NOT_FOUND && change.find("eventID[" + format + "[L][main_anim_event]]", 0) == NOT_FOUND)
				{
					change.replace(change.find("[" + format + "[L][main_anim_event]]"), 22 + format.length(), groupAnimInfo[lastOrder]->mainAnimEvent);
					isChange = true;
				}
				else if (change.find(format + "[L][main_anim_event]", 0) != NOT_FOUND)
				{
					change.replace(change.find(format + "[L][main_anim_event]"), 20 + format.length(), groupAnimInfo[lastOrder]->mainAnimEvent);
					isChange = true;
				}

				string test = boost::regex_replace(string(change), boost::regex(".*" + format + "\\[([0-9]+)\\]\\[main_anim_event\\].*"), string("\\1"));

				if (test != change)
				{
					if (change.find("[" + format + "[" + test + "][main_anim_event]]", 0) != NOT_FOUND && change.find("eventID[" + format + "[" + test + "][main_anim_event]]", 0) == NOT_FOUND)
					{
						if (isLastOrder)
						{
							change.replace(change.find("[" + format + "[" + test + "][main_anim_event]]"), 21 + format.length() + test.length(), mainAnimEvent);
						}
						else
						{
							change.replace(change.find("[" + format + "[" + test + "][main_anim_event]]"), 21 + format.length() + test.length(), groupAnimInfo[order + 1]->mainAnimEvent);
						}

						isChange = true;
					}
					else
					{
						if (isLastOrder)
						{
							change.replace(change.find(format + "[" + test + "][main_anim_event]"), 19 + format.length() + test.length(), mainAnimEvent);
						}
						else
						{
							change.replace(change.find(format + "[" + test + "][main_anim_event]"), 19 + format.length() + test.length(), groupAnimInfo[order + 1]->mainAnimEvent);
						}

						isChange = true;
					}
				}

				if (change.find("[main_anim_event]", 0) != NOT_FOUND && change.find("eventID[main_anim_event]") == NOT_FOUND)
				{
					change.replace(change.find("[main_anim_event]"), 17, mainAnimEvent);
					isChange = true;
				}
				else if (change.find("main_anim_event", 0) != NOT_FOUND)
				{
					change.replace(change.find("main_anim_event"), 15, mainAnimEvent);
					isChange = true;
				}

				if (error)
				{
					return;
				}
			}

			size_t position = change.find("[");

			if (position != NOT_FOUND && line.find("]", position) != NOT_FOUND)
			{
				addOnReplacer(change, behaviorFile, addOn, addition, groupAddition, mixOptRegis, optionMulti, masterFormat, linecount);
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
							ErrorMessage(1168, format, behaviorFile, linecount, "LastState" + number);
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
				eventIDReplacer(change, format, behaviorFile, eventid, ZeroEvent, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("variableID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				variableIDReplacer(change, format, behaviorFile, variableid, ZeroVariable, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("crc32[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				CRC32Replacer(change, format, behaviorFile, linecount);
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
						ErrorMessage(1139, format, behaviorFile, linecount, importer);
						return;
					}

					size_t pos = importer.find("[") + 1;
					string file = importer.substr(pos, importer.find("]", pos) - pos);
					string keyword = "";
					string tempID;

					if (bracketCount > 1)
					{
						pos = importer.find("[", pos);
						string tempKeyword = importer.substr(pos, importer.find_last_of("]") + 1 - pos);
						int openBrack = 0;

						for (unsigned int j = 0; j < tempKeyword.length(); ++j)
						{
							char curChar = tempKeyword[j];

							if (curChar == '[')
							{
								++openBrack;
							}
							else if (curChar == ']')
							{
								--openBrack;

								if (openBrack == 0)
								{
									keyword.append("!~^!");
								}
							}
							else
							{
								keyword = keyword + curChar;
							}
						}

						pos = keyword.rfind("!~^!");

						if (openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4)
						{
							ErrorMessage(1139, format, behaviorFile, linecount, importer);
							return;
						}
						else
						{
							keyword = keyword.substr(0, keyword.length() - 4);
						}
					}

					atomicLock->lockExport();

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

					atomicLock->releaseExport();
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
				if (fixedStateID.size() != 0 || eventid.size() != 0 || variableid.size() != 0)
				{
					ErrorMessage(1096, format, behaviorFile, linecount);
					return;
				}

				motionDataReplacer(change, format, filename, behaviorFile, linecount, groupAnimInfo, order, lastOrder, isLastOrder, animMulti, isChange);
			}

			if (error)
			{
				return;
			}

			if (change.find("RD") != NOT_FOUND)
			{
				if (fixedStateID.size() != 0 || eventid.size() != 0 || variableid.size() != 0)
				{
					ErrorMessage(1097, format, behaviorFile, linecount);
					return;
				}

				rotationDataReplacer(change, format, filename, behaviorFile, linecount, groupAnimInfo, order, lastOrder, isLastOrder, animMulti, isChange);
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

void addOnReplacer(string& line, string filename, unordered_map<string, vecstr> addOn, ImportContainer addition, unordered_map<string, unordered_map<string, vecstr>> groupAddition, SSMap mixOpt, unsigned int count, string format, int numline)
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
							ErrorMessage(1117, format, filename, numline, line);
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
								ErrorMessage(1117, format, filename, numline, line);
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
							ErrorMessage(1117, format, filename, numline, line);
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
								ErrorMessage(1117, format, filename, numline, line);
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

void animObjectReplacer(string& line, string filename, unordered_map<int, vecstr> AnimObject, string format, int linecount, int optionMulti, bool otherAnim, string animNum)
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
				line.replace(nextpos, object.length(), AnimObject[stoi(number)][optionMulti]);
			}
			else
			{
				ErrorMessage(1108, format, filename, linecount);
				return;
			}
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
				line.replace(nextpos, object.length(), AnimObject[stoi(number)][optionMulti]);
			}
			else
			{
				ErrorMessage(1108, format, filename, linecount);
				return;
			}
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
			ErrorMessage(1152, format, behaviorFile, linecount, state);
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
				ErrorMessage(1137, format, behaviorFile, linecount, state);
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
			ErrorMessage(1152, format, behaviorFile, linecount, state);
			return;
		}

		if (line.find(state, 0) != NOT_FOUND)
		{
			size_t stateposition = line.find(state, 0);

			if (state == line.substr(stateposition, line.find(")", stateposition) - stateposition + 1))
			{
				size_t stateLength = state.length();
				state.replace(1, 1 + statenum.length(), to_string(stateID));
				calculate(state, format, behaviorFile, linecount);

				if (error)
				{
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

				subFunctionIDs->format["(S" + statenum + "+" + number + ")"] = state;
				line.replace(stateposition, stateLength, state);
			}
			else
			{
				ErrorMessage(1137, format, behaviorFile, linecount, templine.substr(0, templine.find(")")));
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
			ErrorMessage(1124, format, behaviorFile, linecount, condition);
			return false;
		}

		isNot = true;
		pos = condition.find("!=");
	}
	else if (condition.find("==") != NOT_FOUND)
	{
		if (condition.find("!=") != NOT_FOUND || sameWordCount(condition, "==") > 1)
		{
			ErrorMessage(1124, format, behaviorFile, linecount, condition);
			return false;
		}

		isNot = false;
		pos = condition.find("==");
	}
	else
	{
		ErrorMessage(1125, format, behaviorFile, linecount, condition);
		return false;
	}

	string oriCondition1 = condition.substr(1, pos - 1);
	string oriCondition2 = condition.substr(pos + 2);
	oriCondition2.pop_back();
	vecstr optionInfo1 = GetOptionInfo(oriCondition1, format, behaviorFile, linecount, lastOrder, groupAnimInfo, true, false, order);
	vecstr optionInfo2 = GetOptionInfo(oriCondition2, format, behaviorFile, linecount, lastOrder, groupAnimInfo, true, false, order);

	if (error)
	{
		return false;
	}

	int animMulti1;
	int animMulti2;
	int endAnimMulti1;
	int endAnimMulti2;

	if (optionInfo1[1].length() == 0)
	{
		animMulti1 = 0;
		endAnimMulti1 = groupAnimInfo.size();
	}
	else
	{
		animMulti1 = stoi(optionInfo1[1]);
		endAnimMulti1 = animMulti1 + 1;
	}

	if (optionInfo2[1].length() == 0)
	{
		animMulti2 = 0;
		endAnimMulti2 = groupAnimInfo.size();
	}
	else
	{
		animMulti2 = stoi(optionInfo2[1]);
		endAnimMulti2 = animMulti2 + 1;
	}

	int multiCounter = 0;
	int maxMulti = max(endAnimMulti1 - animMulti1, endAnimMulti2 - animMulti2);
	unordered_map<int, unordered_map<int, string>> history1;
	unordered_map<int, unordered_map<int, string>> history2;

	while (multiCounter < maxMulti)
	{
		int optionMulti1 = -1;
		int optionMulti2 = -1;
		int endMulti1 = -1;
		int endMulti2 = -1;

		if (optionInfo1[2].find("@AnimObject/") != NOT_FOUND)
		{
			optionMulti1 = 0;
			endMulti1 = groupAnimInfo[animMulti1]->optionPickedCount[optionInfo1[2].substr(1)];
		}
		else if (optionInfo1.size() > 3)
		{
			unordered_map<string, vecstr>* addOnPtr = &groupAnimInfo[animMulti1]->addOn;

			if (addOnPtr->find(optionInfo1[2]) != addOnPtr->end() && (*addOnPtr)[optionInfo1[2]].size() > 0)
			{
				unordered_map<string, vecstr>* groupAdditionPtr = &groupAnimInfo[animMulti1]->groupAddition[optionInfo1[2]];

				if (groupAdditionPtr->find(optionInfo1[3]) != groupAdditionPtr->end() && (*groupAdditionPtr)[optionInfo1[3]].size() > 0)
				{
					if (optionInfo1.size() == 5 && optionInfo1[4].length() == 0)
					{
						optionMulti1 = 0;
						endMulti1 = (*groupAdditionPtr)[optionInfo1[3]].size();
					}
					else
					{
						optionMulti1 = stoi(optionInfo1[4]);
						endMulti1 = optionMulti1 + 1;
					}
				}
			}
		}
		else
		{
			optionMulti1 = 0;
			endMulti1 = 1;
		}


		if (optionInfo2[2].find("@AnimObject/") != NOT_FOUND)
		{
			optionMulti2 = 0;
			endMulti2 = groupAnimInfo[animMulti2]->optionPickedCount[optionInfo2[2].substr(1)];
		}
		else if (optionInfo2.size() == 4)
		{
			unordered_map<string, vecstr>* addOnPtr = &groupAnimInfo[animMulti2]->addOn;

			if (addOnPtr->find(optionInfo2[2]) != addOnPtr->end() && (*addOnPtr)[optionInfo2[2]].size() > 0)
			{
				unordered_map<string, vecstr>* groupAdditionPtr = &groupAnimInfo[animMulti2]->groupAddition[optionInfo2[2]];

				if (groupAdditionPtr->find(optionInfo2[3]) != groupAdditionPtr->end() && (*groupAdditionPtr)[optionInfo2[3]].size() > 0)
				{
					if (optionInfo2.size() == 5 && optionInfo2[4].length() == 0)
					{
						optionMulti2 = 0;
						endMulti2 = (*groupAdditionPtr)[optionInfo2[3]].size();
					}
					else
					{
						optionMulti2 = stoi(optionInfo2[4]);
						endMulti2 = optionMulti2 + 1;
					}

				}
			}
		}
		else
		{
			optionMulti2 = 0;
			endMulti2 = 1;
		}

		if (error)
		{
			return false;
		}

		int optionMulti = min(optionMulti1, optionMulti2);

		while (optionMulti < max(endMulti1, endMulti2))
		{
			string condition1 = "$" + oriCondition1 + "$";
			string condition2 = "$" + oriCondition2 + "$";

			if (condition1.length() != 0)
			{
				if (history1[animMulti1][optionMulti1].length() == 0)
				{
					processing(condition1, storeline, format, linecount, utility.eventid, utility.variableid, utility.fixedStateID, utility.stateCountMultiplier, utility.hasGroup, optionMulti1, animMulti1, utility.multiOption);
					history1[animMulti1][optionMulti1] = condition1;
				}
				else
				{
					condition1 = history1[animMulti1][optionMulti1];
				}

				if (error)
				{
					return false;
				}

				if (condition.length() == 0)
				{
					ErrorMessage(1172, format, behaviorFile, linecount);
					return false;
				}
			}

			if (condition2.length() != 0)
			{
				if (history2[animMulti2][optionMulti2].length() == 0)
				{
					processing(condition2, storeline, format, linecount, utility.eventid, utility.variableid, utility.fixedStateID, utility.stateCountMultiplier, utility.hasGroup, optionMulti2, animMulti2, utility.multiOption);
					history2[animMulti2][optionMulti2] = condition2;
				}
				else
				{
					condition2 = history2[animMulti2][optionMulti2];
				}

				if (error)
				{
					return false;
				}

				if (condition.length() == 0)
				{
					ErrorMessage(1172, format, behaviorFile, linecount);
					return false;
				}
			}

			if (condition1 != condition2)
			{
				return isNot;
			}

			if (optionMulti1 < endMulti1 && optionMulti1 != endMulti1 - 1)
			{
				++optionMulti1;
			}

			if (optionMulti2 < endMulti2 && optionMulti2 != endMulti2 - 1)
			{
				++optionMulti2;
			}

			if (error)
			{
				return false;
			}

			++optionMulti;
		}

		if (animMulti1 < endAnimMulti1 && animMulti1 != endAnimMulti1 - 1)
		{
			++animMulti1;
		}

		if (animMulti2 < endAnimMulti2 && animMulti2 != endAnimMulti2 - 1)
		{
			++animMulti2;
		}

		if (error)
		{
			return false;
		}

		++multiCounter;
	}
	
	return !isNot;
}

void eventIDReplacer(string& line, string format, string filename, id eventid, string firstEvent, int linecount)
{
	int count = sameWordCount(line, "eventID[");

	for (int i = 0; i < count; ++i)
	{
		size_t nextpos = line.find("eventID[");
		string fullEventName = line.substr(nextpos, line.find("]", nextpos) - nextpos + 1);
		string eventName = boost::regex_replace(string(fullEventName), boost::regex(".*eventID[[](.*)[]].*"), string("\\1"));
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
				ErrorMessage(1131, format, filename, linecount, eventName);
				return;
			}
		}

		line.replace(line.find(fullEventName), fullEventName.length(), newEventID);
	}
}

void variableIDReplacer(string& line, string format, string filename, id variableid, string ZeroVariable, int linecount)
{
	int count = sameWordCount(line, "variableID[");

	for (int i = 0; i < count; ++i)
	{
		size_t nextpos = line.find("variableID[");
		string fullVarName = line.substr(nextpos, line.find("]", nextpos) - nextpos + 1);
		string varName = boost::regex_replace(string(fullVarName), boost::regex(".*variableID[[](.*)[]].*"), string("\\1"));
		string newVarID = to_string(variableid[varName]);

		if (newVarID == "0" && ZeroVariable != varName)
		{
			if (format == "BASE")
			{
				ErrorMessage(1166);
				return;
			}
			else
			{
				ErrorMessage(1132, format, filename, linecount, varName);
				return;
			}
		}

		line.replace(line.find(fullVarName), fullVarName.length(), newVarID);
	}
}

vecstr GetOptionInfo(string line, string format, string filename, int numline, size_t lastOrder, vector<shared_ptr<animationInfo>> groupAnimInfo, bool allowNoFixAnim, bool isCondition, int order, int animMulti, int optionMulti)
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
			ErrorMessage(1157, format, filename, numline, line);
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
			if (lastOrder == animMulti || lastOrder == order)
			{
				optionInfo[1] = to_string(order);
			}
			else
			{
				optionInfo[1] = to_string(order + 1);
			}
		}
		else if (optionInfo[1] == "B")
		{
			if (animMulti == 0 || order == 0)
			{
				optionInfo[1] = to_string(order);
			}
			else
			{
				optionInfo[1] = to_string(order - 1);
			}
		}
		else if (optionInfo[1][0] == '(' && optionInfo[1][optionInfo[1].length() - 1] == ')')
		{
			int Fnum = sameWordCount(optionInfo[1], "F");
			int Nnum = sameWordCount(optionInfo[1], "N");
			int Bnum = sameWordCount(optionInfo[1], "B");
			int Lnum = sameWordCount(optionInfo[1], "L");

			for (int i = 0; i < Fnum; ++i)
			{
				optionInfo[1].replace(optionInfo[1].find("F"), 1, "0");
			}
			
			if (lastOrder == order)
			{
				for (int i = 0; i < Nnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("N"), 1, to_string(order));
				}
			}
			else
			{
				for (int i = 0; i < Nnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("N"), 1, to_string(order + 1));
				}
			}

			if (order == 0)
			{
				for (int i = 0; i < Bnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("B"), 1, to_string(order));
				}
			}
			else
			{
				for (int i = 0; i < Bnum; ++i)
				{
					optionInfo[1].replace(optionInfo[1].find("B"), 1, to_string(order - 1));
				}
			}

			for (int i = 0; i < Lnum; ++i)
			{
				optionInfo[1].replace(optionInfo[1].find("L"), 1, to_string(lastOrder));
			}

			calculate(optionInfo[1], format, filename, numline);

			if (error)
			{
				return optionInfo;
			}
		}
		else if (optionInfo[1].length() == 0)
		{
			if (animMulti == -1)
			{
				if (!allowNoFixAnim)
				{
					ErrorMessage(1057, format, filename, numline, line);
					return optionInfo;
				}
			}
			else
			{
				optionInfo[1] = to_string(animMulti);
			}
		}
		else
		{
			templine = optionInfo[1];
			templine = templine + "a";
			string newtempline = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

			if (newtempline == templine)
			{
				ErrorMessage(1179, format, filename, numline, line);
				return optionInfo;
			}

			if (stoi(optionInfo[1]) > int(lastOrder))
			{
				ErrorMessage(1148, format, filename, numline, line);
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
				ErrorMessage(1109, format, filename, numline, line);
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
						if (!allowNoFixAnim)
						{
							ErrorMessage(1060, format, filename, numline, line);
							return optionInfo;
						}
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
						ErrorMessage(1055, format, filename, numline, line);
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
			ErrorMessage(1054, limiter - 1, format, filename, numline, line);
			return optionInfo;
		}

		if (optionInfo[0] != format)
		{
			ErrorMessage(1051, format, filename, numline, line);
		}
	}
	else
	{
		optionInfo.push_back(format);

		if (order == -1)
		{
			if (!allowNoFixAnim)
			{
				ErrorMessage(1060, format, filename, numline, line);
				return optionInfo;
			}
		}
		else
		{
			optionInfo.push_back(to_string(order));
		}

		if (line.back() == '*')
		{
			string option = line.substr(0, line.length() - 1);

			if (groupAnimInfo[stoi(optionInfo[1])]->mixOptRegis[option].length() != 0)
			{
				optionInfo.push_back(groupAnimInfo[stoi(optionInfo[1])]->mixOptRegis[option]);
			}
			else
			{
				ErrorMessage(1109, format, filename, numline, line);
				return optionInfo;
			}
		}
		else
		{
			optionInfo.push_back(line);
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

bool Furniture::firstConditionProcess(string condition, vector<unordered_map<string, bool>> groupOptionPicked, bool isNot, int numline, animationutility utility)
{
	vecstr optionInfo;

	if (condition.find(format + "[][") != NOT_FOUND)
	{
		int formatGroup = 0;
		bool conditionResult;

		while (formatGroup < int(groupAnimInfo.size()))
		{
			if (utility.animMulti == -1)
			{
				optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, true, true, order, formatGroup);
			}
			else
			{
				optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, true, true, order, utility.animMulti);
			}

			if (error)
			{
				return false;
			}

			conditionResult = GetFirstCondition(condition, optionInfo, numline, groupOptionPicked, isNot);
			++formatGroup;

			if (error)
			{
				return false;
			}

			if (!conditionResult)
			{
				return false;
			}
		}
	}
	else
	{
		if (utility.animMulti == -1)
		{
			optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, order);
		}
		else
		{
			optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, true, true, order, utility.animMulti);
		}

		if (error)
		{
			return false;
		}

		return GetFirstCondition(condition, optionInfo, numline, groupOptionPicked, isNot);

		if (error)
		{
			return false;
		}
	}

	return true;
}

bool Furniture::secondConditionProcess(string condition, vector<unordered_map<string, bool>> groupOptionPicked, int numline, animationutility utility)
{
	bool conditionResult;
	bool isNot = false;
	vecstr optionInfo;

	if (condition[0] == '!')
	{
		isNot = true;
		condition = condition.substr(1);
	}

	if (condition.find(format + "[][") != NOT_FOUND)
	{
		int formatGroup = 0;

		while (formatGroup < int(groupAnimInfo.size()))
		{
			if (utility.animMulti == -1)
			{
				optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, order, formatGroup);
			}
			else
			{
				optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, true, true, order, utility.animMulti);
			}

			if (error)
			{
				return false;
			}

			conditionResult = GetFirstCondition(condition, optionInfo, numline, groupOptionPicked, isNot);
			++formatGroup;

			if (error)
			{
				return false;
			}

			if (!conditionResult)
			{
				return false;
			}
		}
	}
	else
	{
		if (utility.animMulti == -1)
		{
			optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, false, true, order);
		}
		else
		{
			optionInfo = GetOptionInfo(condition, format, behaviorFile, numline, groupOptionPicked.size() - 1, groupAnimInfo, true, true, order, utility.animMulti);
		}

		if (error)
		{
			return false;
		}

		return GetFirstCondition(condition, optionInfo, numline, groupOptionPicked, isNot);

		if (error)
		{
			return false;
		}
	}

	return true;
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
						return false;
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
						return false;
					}
				}
			}
			else if (conditionOrder == "first")
			{
				conditionOrder = "0";
			}
		}
		else if (isOnlyNumber(optionInfo[2]))
		{
			conditionOrder = optionInfo[2];
		}
		else
		{
			ErrorMessage(1138, format, behaviorFile, numline, firstCondition);
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

void Furniture::addAnimSetData(unordered_map<string, map<string, vecstr, alphanum_less>> animsetdata)
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

void Furniture::GetAnimSetData(unordered_map<string, map<string, vecstr, alphanum_less>>& newASDLines)
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
				vecstr dummy;
				AnimDataLineProcess(iter->second, dummy, format, groupOptionPicked, vector<int>(1));

				for (auto& line : dummy)
				{
					if (line.length() > 0)
					{
						newASDLines[it->first][iter->first].push_back(line);
					}
				}
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

void Furniture::AnimDataLineProcess(vecstr originallines, vecstr& newlines, string format, vector<unordered_map<string, bool>> groupOptionPicked, vector<int> ASD)
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

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND && !multi)
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

						if (conditionLine.find("[") == NOT_FOUND || conditionLine.find("]") == NOT_FOUND)
						{
							ErrorMessage(1153, format, behaviorFile, i + 1, conditionLine);
							return;
						}

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
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND && !multi)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, behaviorFile, i + 1);
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

						if (option.find("[") == NOT_FOUND || option.find("]") == NOT_FOUND)
						{
							ErrorMessage(1153, format, behaviorFile, i + 1, option);
							return;
						}

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
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND && !multi)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, behaviorFile, i + 1);
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
				ErrorMessage(1116, format, behaviorFile, i + 1);
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

					vecstr optionInfo = GetOptionInfo(curOption, format, behaviorFile, i + 1, lastOrder, groupAnimInfo, false, true, order);

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
						ErrorMessage(1150, format, behaviorFile, i + 1);
						skip = true;
					}
				}
				else
				{
					ErrorMessage(1116, format, behaviorFile, i + 1);
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
				ErrorMessage(1116, format, behaviorFile, i + 1);
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
						vecstr optionInfo = GetOptionInfo(curOption, format, behaviorFile, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true, order);

						if (error)
						{
							return;
						}

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

						break;
					}
				}
				else
				{
					ErrorMessage(1115, format, behaviorFile, i + 1);
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
				ErrorMessage(1116, format, behaviorFile, i + 1);
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
					
					if (isOnlyNumber(curOrder))
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
						bool word = false;
						bool unknown = false;
						bool number = false;

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

						if (word & number)
						{
							ErrorMessage(1110, format, behaviorFile, i + 1);
							return;
						}
						else if (unknown)
						{
							ErrorMessage(1111, format, behaviorFile, i + 1);
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
								ErrorMessage(1112, format, behaviorFile, i + 1);
								return;
							}
						}
						else
						{
							ErrorMessage(1113, format, behaviorFile, i + 1);
							return;
						}
					}
				}
				else
				{
					ErrorMessage(1114, format, behaviorFile, i + 1);
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
				ErrorMessage(1171, format, behaviorFile, i + 1);
				return;
			}

			newOpen = false;
			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi)
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
						ErrorMessage(1172, format, behaviorFile, i + 1);
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
					bool skip2 = false;
					bool freeze2 = false;

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
								bool uniqueskip2 = false;
								string newline = recorder[k];

								if (newline.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
								{
									condition++;

									if (!freeze2)
									{
										if (!IsConditionOpened[condition])
										{
											if (isPassed(condition, IsConditionOpened))
											{
												size_t optionPosition = newline.find("<!-- CONDITION START ^") + 22;
												string conditionLine = newline.substr(optionPosition, newline.find("^ -->", optionPosition) - optionPosition);
												animationutility utility;
												utility.animMulti = animMulti;

												if (conditionLine.find("[") == NOT_FOUND || conditionLine.find("]") == NOT_FOUND)
												{
													ErrorMessage(1153, format, behaviorFile, i + 1, conditionLine);
													return;
												}

												if (newCondition(conditionLine, newlines, groupOptionPicked, i + 1, utility))
												{
													skip2 = false;
													IsConditionOpened[condition] = true;
												}
												else
												{
													skip2 = true;
												}

												if (error)
												{
													return;
												}
											}
										}
										else
										{
											skip2 = true;
											freeze2 = true;
										}
									}

									uniqueskip2 = true;
								}
								else if (newline.find("<!-- CONDITION ^", 0) != NOT_FOUND)
								{
									if (condition == 0)
									{
										ErrorMessage(1119, format, behaviorFile, i + 1);
										return;
									}

									if (!freeze2)
									{
										if (!IsConditionOpened[condition])
										{
											if (isPassed(condition, IsConditionOpened))
											{
												size_t optionPosition = newline.find("<!-- CONDITION ^") + 16;
												string option = newline.substr(optionPosition, newline.find("^ -->", optionPosition) - optionPosition);
												animationutility utility;
												utility.animMulti = animMulti;

												if (option.find("[") == NOT_FOUND || option.find("]") == NOT_FOUND)
												{
													ErrorMessage(1153, format, behaviorFile, i + 1, option);
													return;
												}

												if (newCondition(option, newlines, groupOptionPicked, i + 1, utility))
												{
													skip2 = false;
													IsConditionOpened[condition] = true;
												}
												else
												{
													skip2 = true;
												}

												if (error)
												{
													return;
												}
											}
										}
										else
										{
											skip2 = true;
											freeze2 = true;
										}
									}

									uniqueskip2 = true;
								}
								else if (newline.find("<!-- CONDITION -->", 0) != NOT_FOUND)
								{
									if (condition == 0)
									{
										ErrorMessage(1119, format, behaviorFile, i + 1);
										return;
									}

									if (!freeze2)
									{
										if (!IsConditionOpened[condition])
										{
											if (isPassed(condition, IsConditionOpened))
											{
												skip2 = false;
												IsConditionOpened[condition] = true;
												uniqueskip2 = true;
												size_t conditionPosition = newline.find("<!-- CONDITION") + 14;
												string replacement1 = newline.substr(0, conditionPosition + 1);
												string replacement2 = newline.substr(conditionPosition);
												newlines.push_back(replacement1 + "START" + replacement2);
											}
											else
											{
												skip2 = true;
											}
										}
										else
										{
											skip2 = true;
											freeze2 = true;
										}
									}

									uniqueskip2 = true;
								}
								else if (newline.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
								{
									uniqueskip2 = true;
								}

								if (!skip2 && !uniqueskip2 && !freeze2)
								{
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
											animationutility utility(emptyID, emptyID, emptyVI, ASD, false, optionMulti, animMulti, multiOption);
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

										if (newline.find("\n") != NOT_FOUND)
										{
											__int64 ref = count(newline.begin(), newline.end(), '\n');
											size_t pos = 0;

											for (__int64 j = 0; j < ref - 1; ++j)
											{
												string addline = newline.substr(pos, newline.find("\n") - pos);
												pos = newline.find("\n", pos) + 1;
												newlines.push_back(addline);
											}

											newline.replace(newline.find("\n"), 1, "\0");
										}

										if (error)
										{
											return;
										}

										if (newline.length() == 0)
										{
											ErrorMessage(1172, format, behaviorFile, i + 1);
											return;
										}
									}

									size_t pos = newline.find("(");

									if (pos != NOT_FOUND && newline.find(")", pos) != NOT_FOUND)
									{
										string templine = newline.substr(pos);
										int nextpos = openEndBracket(templine, '(', ')', format, behaviorFile, i + 1) + 1;

										if (error)
										{
											return;
										}

										templine = templine.substr(0, nextpos);
										string oldline = templine;
										calculate(templine, format, behaviorFile, i + 1);

										if (error)
										{
											return;
										}

										if (oldline != templine)
										{
											newline.replace(pos, oldline.length(), templine);
										}
									}

									newlines.push_back(newline);
								}

								if (newline.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
								{
									if (condition == 0)
									{
										ErrorMessage(1118, format, behaviorFile, i + 1);
										return;
									}

									if (freeze2 && IsConditionOpened[condition])
									{
										freeze2 = false;
									}

									if (isPassed(condition, IsConditionOpened))
									{
										skip2 = false;
									}
									else
									{
										skip2 = true;
									}

									IsConditionOpened[condition] = false;
									condition--;
								}
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
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi)
		{
			if (condition == 0)
			{
				ErrorMessage(1118, format, behaviorFile, i + 1);
				return;
			}

			if (freeze && IsConditionOpened[condition])
			{
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
		ErrorMessage(1116, format, behaviorFile, originallines.size());
		return;
	}

	for (auto itera = IsConditionOpened.begin(); itera != IsConditionOpened.end(); ++itera)
	{
		if (itera->second)
		{
			ErrorMessage(1145, format, behaviorFile);
			return;
			break;
		}
	}

	if (condition != 0)
	{
		ErrorMessage(1122, format, behaviorFile);
		return;
	}

	if (ASD[0] == 0)
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

void Furniture::existingASDProcess(vecstr ASDLines, map<int, vecstr>& extract, vector<int> ASD)
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

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND && !multi)
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

						if (conditionLine.find("[") == NOT_FOUND)
						{
							if (format != conditionLine)
							{
								skip = true;
							}
						}
						else
						{
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
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND && !multi)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, behaviorFile, i + 1);
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

						if (option.find("[") == NOT_FOUND)
						{
							if (format != option)
							{
								skip = true;
							}
						}
						else
						{
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
				}
				else
				{
					skip = true;
					freeze = true;
				}
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND && !multi)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, behaviorFile, i + 1);
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
				ErrorMessage(1116, format, behaviorFile, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(ASDLines[i], false);

					if (curOption.find("[") == NOT_FOUND)
					{
						if (format != curOption)
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
						bool isNot = false;

						if (curOption[0] == '!')
						{
							isNot = true;
							curOption = curOption.substr(1);
						}

						vecstr optionInfo = GetOptionInfo(curOption, format, behaviorFile, i + 1, lastOrder, groupAnimInfo, false, true, order);

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
							ErrorMessage(1150, format, behaviorFile, i + 1);
							skip = true;
						}
					}
				}
				else
				{
					ErrorMessage(1116, format, behaviorFile, i + 1);
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
				ErrorMessage(1116, format, behaviorFile, i + 1);
				return;
			}

			if (IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(line, true);

					if (curOption.find("[") == NOT_FOUND)
					{
						if (format != curOption)
						{
							skip = true;
						}
						else
						{
							recorder.reserve(ASDLines.size() / 5);
							open = true;
							multi = true;
							multiOption = curOption;
							openOrder = -1;
						}
					}
					else
					{
						bool isNot = false;

						if (curOption[0] == '!')
						{
							isNot = true;
							curOption = curOption.substr(1);
						}

						while (true)
						{
							vecstr optionInfo = GetOptionInfo(curOption, format, behaviorFile, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true, order);

							if (error)
							{
								return;
							}

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
											openOrder = stoi(optionInfo[1]);
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

							break;
						}
					}
				}
				else
				{
					ErrorMessage(1115, format, behaviorFile, i + 1);
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
				ErrorMessage(1116, format, behaviorFile, i + 1);
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

					if (isOnlyNumber(curOrder))
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
						bool word = false;
						bool unknown = false;
						bool number = false;

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

						if (word & number)
						{
							ErrorMessage(1110, format, behaviorFile, i + 1);
							return;
						}
						else if (unknown)
						{
							ErrorMessage(1111, format, behaviorFile, i + 1);
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
								ErrorMessage(1112, format, behaviorFile, i + 1);
								return;
							}
						}
						else
						{
							ErrorMessage(1113, format, behaviorFile, i + 1);
							return;
						}
					}
				}
				else
				{
					ErrorMessage(1114, format, behaviorFile, i + 1);
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
				ErrorMessage(1171, format, behaviorFile, i + 1);
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
						ErrorMessage(1172, format, behaviorFile, i + 1);
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
					bool skip2 = false;
					bool freeze2 = false;

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
								bool uniqueskip2 = false;
								string newline = recorder[k];

								if (newline.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
								{
									condition++;

									if (!freeze2)
									{
										if (!IsConditionOpened[condition])
										{
											if (isPassed(condition, IsConditionOpened))
											{
												size_t optionPosition = newline.find("<!-- CONDITION START ^") + 22;
												string conditionLine = newline.substr(optionPosition, newline.find("^ -->", optionPosition) - optionPosition);
												animationutility utility;
												utility.animMulti = animMulti;

												if (newCondition(conditionLine, newlines, groupOptionPicked, i + 1, utility))
												{
													skip2 = false;
													IsConditionOpened[condition] = true;
												}
												else
												{
													skip2 = true;
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
											skip2 = true;
											freeze2 = true;
										}
									}

									uniqueskip2 = true;
								}
								else if (newline.find("<!-- CONDITION ^", 0) != NOT_FOUND)
								{
									if (condition == 0)
									{
										ErrorMessage(1119, format, behaviorFile, i + 1);
										return;
									}

									if (!freeze2)
									{
										if (!IsConditionOpened[condition])
										{
											if (isPassed(condition, IsConditionOpened))
											{
												size_t optionPosition = newline.find("<!-- CONDITION ^") + 16;
												string option = newline.substr(optionPosition, newline.find("^ -->", optionPosition) - optionPosition);
												animationutility utility;
												utility.animMulti = animMulti;

												if (newCondition(option, newlines, groupOptionPicked, i + 1, utility))
												{
													skip2 = false;
													IsConditionOpened[condition] = true;
												}
												else
												{
													skip2 = true;
												}

												if (error)
												{
													return;
												}
											}
										}
										else
										{
											skip2 = true;
											freeze2 = true;
										}
									}

									uniqueskip2 = true;
								}
								else if (newline.find("<!-- CONDITION -->", 0) != NOT_FOUND)
								{
									if (condition == 0)
									{
										ErrorMessage(1119, format, behaviorFile, i + 1);
										return;
									}

									if (!freeze2)
									{
										if (!IsConditionOpened[condition])
										{
											if (isPassed(condition, IsConditionOpened))
											{
												skip2 = false;
												IsConditionOpened[condition] = true;
												uniqueskip2 = true;
												size_t conditionPosition = newline.find("<!-- CONDITION") + 14;
												string replacement1 = newline.substr(0, conditionPosition + 1);
												string replacement2 = newline.substr(conditionPosition);
												newlines.push_back(replacement1 + "START" + replacement2);
											}
											else
											{
												skip2 = true;
											}
										}
										else
										{
											skip2 = true;
											freeze2 = true;
										}
									}

									uniqueskip2 = true;
								}

								if (!skip2 && !uniqueskip2 && !freeze2)
								{
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
											animationutility utility(emptyID, emptyID, emptyVI, ASD, false, optionMulti, animMulti, multiOption);
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

										if (newline.find("\n") != NOT_FOUND)
										{
											__int64 ref = count(newline.begin(), newline.end(), '\n');
											size_t pos = 0;

											for (__int64 j = 0; j < ref - 1; ++j)
											{
												string addline = newline.substr(pos, newline.find("\n") - pos);
												pos = newline.find("\n", pos) + 1;
												extract[curExtract].push_back(addline);
											}

											newline.replace(newline.find("\n"), 1, "\0");
										}

										if (error)
										{
											return;
										}

										if (newline.length() == 0)
										{
											ErrorMessage(1172, format, behaviorFile, i + 1);
											return;
										}
									}

									size_t pos = newline.find("(");

									if (pos != NOT_FOUND && newline.find(")", pos) != NOT_FOUND)
									{
										string templine = newline.substr(pos);
										int nextpos = openEndBracket(templine, '(', ')', format, behaviorFile, i + 1) + 1;

										if (error)
										{
											return;
										}

										templine = templine.substr(0, nextpos);
										string oldline = templine;
										calculate(templine, format, behaviorFile, i + 1);

										if (error)
										{
											return;
										}

										if (oldline != templine)
										{
											newline.replace(pos, oldline.length(), templine);
										}
									}

									extract[curExtract].push_back(newline);
									newlines.push_back(newline);
								}
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
				ErrorMessage(1118, format, behaviorFile, i + 1);
				return;
			}

			if (freeze && IsConditionOpened[condition])
			{
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
		ErrorMessage(1116, format, behaviorFile, ASDLines.size());
		return;
	}

	IsConditionOpened[0] = false;

	for (auto itera = IsConditionOpened.begin(); itera != IsConditionOpened.end(); ++itera)
	{
		if (itera->second)
		{
			ErrorMessage(1145, format, behaviorFile);
			return;
			break;
		}
	}

	if (condition != 0)
	{
		ErrorMessage(1122, format, behaviorFile);
	}
}

void motionDataReplacer(string& change, string format, string filename, string behaviorFile, int linecount, vector<shared_ptr<animationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool& isChange)
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
				WarningMessage(1018, format, behaviorFile, linecount);
			}

			change.replace(change.find(format + "[][MD]"), 6 + format.length(), motionData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1146, format, behaviorFile, linecount);
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
			WarningMessage(1018, format, behaviorFile, linecount);
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
			WarningMessage(1018, format, behaviorFile, linecount);
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
			WarningMessage(1018, format, behaviorFile, linecount);
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
			WarningMessage(1018, format, behaviorFile, linecount);
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
				WarningMessage(1018, format, behaviorFile, linecount);
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
					WarningMessage(1018, format, behaviorFile, linecount);
				}

				motionData.append(groupAnimInfo[order]->motionData[j] + "\n");
			}

			change.replace(change.find("MD"), 2, motionData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1134, format, behaviorFile, linecount);
			return;
		}
	}
}

void rotationDataReplacer(string& change, string format, string filename, string behaviorFile, int linecount, vector<shared_ptr<animationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool&isChange)
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
				WarningMessage(1019, format, behaviorFile, linecount);
			}

			change.replace(change.find(format + "[][RD]"), 6 + format.length(), rotationData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1146, format, behaviorFile, linecount);
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
			WarningMessage(1019, format, behaviorFile, linecount);
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
			WarningMessage(1019, format, behaviorFile, linecount);
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
			WarningMessage(1019, format, behaviorFile, linecount);
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
			WarningMessage(1019, format, behaviorFile, linecount);
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
				WarningMessage(1019, format, behaviorFile, linecount);
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
				WarningMessage(1019, format, behaviorFile, linecount);
			}

			change.replace(change.find("RD"), 2, rotationData);
			isChange = true;
		}
		else
		{
			ErrorMessage(1134, format, behaviorFile, linecount);
			return;
		}
	}
}

bool isEquation(string& line)
{
	for (char& curChar : line)
	{
		if (!isdigit(curChar) && curChar != '*' && curChar != '+' && curChar != '-' && curChar != '/' && curChar != '%' && curChar != '^' && curChar != '.' && curChar != '(' && curChar != ')')
		{
			return false;
		}
	}

	return true;
}

int openEndBracket(string& line, char openBrac, char closeBrac, string format, string filename, int linecount)
{
	int open = 0;

	for (unsigned int i = 0; i < line.length(); ++i)
	{
		if (line[i] == openBrac)
		{
			++open;
		}
		else if (line[i] == closeBrac)
		{
			--open;

			if (open == 0)
			{
				return i;
			}
		}
	}

	ErrorMessage(1180, format, filename, linecount);
	return -1;
}

void CRC32Replacer(string& line, string format, string behaviorFile, int linecount)
{
	__int64 bracketCount = count(line.begin(), line.end(), '[');
	__int64 altBracketCount = count(line.begin(), line.end(), ']');

	if (bracketCount != altBracketCount)
	{
		ErrorMessage(1181, format, behaviorFile, linecount, line);
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
