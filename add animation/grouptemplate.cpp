#include "grouptemplate.h"

#pragma warning(disable:4503)

using namespace std;

string zeroEvent;

groupTemplate::groupTemplate(vecstr grouptemplateformat)
{
	templatelines = grouptemplateformat;
}

vecstr groupTemplate::getFunctionLines(string behaviorFile, string formatname, vector<int>& stateID, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, int& nFunctionID, ImportContainer& import, id eventid, id variableid, string masterFormat, int groupCount)
{
	format = formatname;
	unordered_map<string, string> IDExist;
	vecstr tempstore;
	vecstr functionline;
	vecstr templateID;
	string multiOption;
	bool newOpen = false;
	bool multi = false;
	bool norElement = false;
	bool open = false;
	bool skip = false;
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	__int64 openRange = 0;
	vector<int> fixedStateID = stateID;
	int condition = 0;
	int counter = 0;
	int order = -2;
	strID = to_string(nFunctionID);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	newImport = const_cast<ImportContainer*>(&import);
	nextStateID = const_cast<vector<int>*>(&stateID);
	unordered_map<int, bool> IsConditionOpened;
	size_t elementLine = -1;
	vector<unordered_map<string, bool>> groupOptionPicked;

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	for (unsigned int i = 0; i < 4 - strID.length(); ++i)
	{
		strID = "0" + strID;
	}

	// check error before initialization
	if (subFunctionIDs.size() != groupAnimInfo.size() && groupCount != -1)
	{
		ErrorMessage(1156, format);
		return functionline;
	}

	IsConditionOpened[0] = true;
	functionline.reserve(templatelines.size() + 20 * memory);

	for (unsigned int i = 0; i < templatelines.size(); ++i)
	{
		bool uniqueskip = false;
		bool elementCatch = false;

		if (templatelines[i].find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			condition++;

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = templatelines[i].find("<!-- CONDITION START ^") + 22;
						string conditionLine = templatelines[i].substr(optionPosition, templatelines[i].find("^ -->", optionPosition) - optionPosition);

						if (newCondition(conditionLine, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat))
						{
							skip = false;
							IsConditionOpened[condition] = true;
						}
						else
						{
							skip = true;
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
		else if (templatelines[i].find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = templatelines[i].find("<!-- CONDITION ^") + 16;
						string option = templatelines[i].substr(optionPosition, templatelines[i].find("^", optionPosition) - optionPosition);

						if (newCondition(option, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat))
						{
							skip = false;
							IsConditionOpened[condition] = true;
							uniqueskip = true;
						}
						else
						{
							skip = true;
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
		else if (templatelines[i].find("<!-- CONDITION -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
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
						size_t conditionPosition = templatelines[i].find("<!-- CONDITION") + 14;
						string replacement1 = templatelines[i].substr(0, conditionPosition + 1);
						string replacement2 = templatelines[i].substr(conditionPosition);
						functionline.push_back(replacement1 + "START" + replacement2);
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
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "^ -->", 0) != NOT_FOUND || templatelines[i].find("<!-- NEW ^" + masterFormat + "_group^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			ErrorMessage(1164, format, i + 1);
			functionline.shrink_to_fit();
			return functionline;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if(IsConditionOpened[condition])
			{
				if (groupCount != -1)
				{
					if (!open)
					{
						open = true;
						multi = true;
						order = -1;
						multiOption = masterFormat;
					}
					else
					{
						ErrorMessage(1115, format, i + 1);
						functionline.shrink_to_fit();
						return functionline;
					}
				}
				else
				{
					ErrorMessage(1159, format, i + 1);
					functionline.shrink_to_fit();
					return functionline;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "_group^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if(IsConditionOpened[condition])
			{
				if (groupCount == -1)
				{
					if (!open)
					{
						open = true;
						multi = true;
						order = -1;
						multiOption = masterFormat + "_group";
					}
					else
					{
						ErrorMessage(1115, format, i + 1);
						functionline.shrink_to_fit();
						return functionline;
					}
				}
				else
				{
					ErrorMessage(1160, format, i + 1);
					functionline.shrink_to_fit();
					return functionline;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "_master^ -->", 0) != NOT_FOUND || templatelines[i].find("<!-- NEW ^" + masterFormat + "_master^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			ErrorMessage(1163, format, i + 1);
			functionline.shrink_to_fit();
			return functionline;
		}
		else if (templatelines[i].find("<!-- NEW ^", 0) != NOT_FOUND && templatelines[i].find("^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if(IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(templatelines[i], false);
					bool isNot = false;

					if (curOption[0] == '!')
					{
						isNot = true;
						curOption = curOption.substr(1);
					}

					vecstr optionInfo = GetOptionInfo(curOption, masterFormat, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, false, true);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}

					if (optionInfo[2] != "AnimObject")
					{
						if (!groupAnimInfo[stoi(optionInfo[1])]->optionPicked[curOption])
						{
							// clear group number
							string previous = optionInfo[2];
							curOption = boost::regex_replace(string(optionInfo[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

							if (groupAnimInfo[stoi(optionInfo[1])]->optionPicked[curOption])
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
									// Check if current condition accepts other options that are linked
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
							if (isNot)
							{
								skip = true;
							}
							else
							{
								open = true;
							}
						}
					}
					else
					{
						WarningMessage(1004, format, i + 1);
						skip = true;
					}
				}
				else
				{
					ErrorMessage(1116, format, i + 1);
					functionline.shrink_to_fit();
					return functionline;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- NEW ^", 0) != NOT_FOUND && templatelines[i].find("^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if(IsConditionOpened[condition])
			{
				if (!open)
				{
					string curOption = getOption(templatelines[i], true);
					bool isNot = false;

					if (curOption[0] == '!')
					{
						isNot = true;
						curOption = curOption.substr(1);
					}

					vecstr optionInfo = GetOptionInfo(curOption, masterFormat, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}

					string animID = boost::regex_replace(string(optionInfo[2]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (optionInfo[2] != "AnimObject/" + animID)
					{
						if (!groupAnimInfo[stoi(optionInfo[1])]->optionPicked[optionInfo[2]])
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
									tempstore.reserve(templatelines.size() / 5);
									open = true;
									multi = true;
									multiOption = optionInfo[2];

									if (optionInfo[1].length() == 0)
									{
										order = -1;
									}
									else
									{
										order = stoi(optionInfo[1]);
									}
								}
							}
							else
							{
								// Check if current condition accepts other options that are linked
								if (isNot)
								{
									tempstore.reserve(templatelines.size() / 5);
									open = true;
									multi = true;
									multiOption = optionInfo[2];

									if (optionInfo[1].length() == 0)
									{
										order = -1;
									}
									else
									{
										order = stoi(optionInfo[1]);
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
								tempstore.reserve(templatelines.size() / 5);
								open = true;
								multi = true;
								multiOption = optionInfo[2];

								if (optionInfo[1].length() == 0)
								{
									order = -1;
								}
								else
								{
									order = stoi(optionInfo[1]);
								}
							}
						}
					}
					else
					{
						WarningMessage(1005, format, i + 1);
						skip = true;
					}
				}
				else
				{
					ErrorMessage(1115, format, i + 1);
					functionline.shrink_to_fit();
					return functionline;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (!newOpen)
			{
				ErrorMessage(1171, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			newOpen = false;
			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			uniqueskip = true;
		}

		if (!skip && !freeze && !uniqueskip)
		{
			while (true)
			{
				if (multi)
				{
					tempstore.push_back(templatelines[i]);
					break;
				}

				if (templatelines[i].find("MID$", 0) != NOT_FOUND)
				{
					int counter = sameWordCount(templatelines[i], "MID$");

					for (int k = 0; k < counter; ++k)
					{
						size_t MIDposition = templatelines[i].find("MID$");
						string ID = boost::regex_replace(string(templatelines[i].substr(MIDposition)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
						string oldID = "MID$" + ID;

						if (templatelines[i].find(oldID, MIDposition) != NOT_FOUND)
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

							string templine = templatelines[i];
							templine.replace(templine.find("MID$", MIDposition), 3, format);
							templine = templine.substr(templine.find(format + "$", MIDposition), format.length() + oldID.length() - 3);
							functionIDs[templine] = ID;
							templatelines[i].replace(templatelines[i].find("MID$"), oldID.length(), ID);
						}
					}
				}

				// set state ID
				if (templatelines[i].find("$(S", 0) != NOT_FOUND)
				{
					string templine = templatelines[i].substr(templatelines[i].find("$(S"));
					string ID = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
					int intID;

					if (templatelines[i].find("$(S" + ID + "+") == NOT_FOUND)
					{
						ID = "";
						intID = 0;
					}
					else
					{
						intID = stoi(ID) - 1;

						if (intID >= int(fixedStateID.size()))
						{
							ErrorMessage(1168, format, i + 1, templine.substr(0, templine.find(")") + 1));
							functionline.shrink_to_fit();
							return functionline;
						}
					}

					if (templatelines[i].find("$(S" + ID + "+") != NOT_FOUND)
					{
						stateReplacer(templatelines[i], ID, fixedStateID[intID], i + 1, groupCount);
					}
				}

				if (groupCount != -1)
				{
					// set animation ID
					if (templatelines[i].find("$%$", 0) != NOT_FOUND)
					{
						templatelines[i].replace(templatelines[i].find("$%$"), 3, to_string(groupCount));
					}

					// multi choice selection
					if (templatelines[i].find("$MC$", 0) != NOT_FOUND)
					{
						multiChoice(templatelines[i], groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat);

						if (error)
						{
							functionline.shrink_to_fit();
							return functionline;
						}
					}

					if (templatelines[i].find("%") != NOT_FOUND && templatelines[i].find("+%") == NOT_FOUND)
					{
						__int64 counter = count(templatelines[i].begin(), templatelines[i].end(), '%');

						for (int p = 0; p < counter; ++p)
						{
							templatelines[i].replace(templatelines[i].find("%"), 1, to_string(0));
						}
					}
				}

				// compute numelements
				if (templatelines[i].find("<hkparam name=\"") != NOT_FOUND && templatelines[i].find("numelements=\"") != NOT_FOUND && templatelines[i].find("</hkparam>") == NOT_FOUND && templatelines[i].find("<!-- COMPUTE -->", templatelines[i].find("numelements=\"")) != NOT_FOUND)
				{
					if (!norElement)
					{
						norElement = true;
						elementCatch = true;
						string templine = templatelines[i].substr(0, templatelines[i].find("<hkparam name=\"", 0));
						openRange = count(templine.begin(), templine.end(), '\t');
					}
					else
					{
						ErrorMessage(1136, format, i + 1);
						functionline.shrink_to_fit();
						return functionline;
					}
				}
				else if (templatelines[i].find("</hkparam>") != NOT_FOUND && norElement)
				{
					string templine = templatelines[i].substr(0, templatelines[i].find("</hkparam>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						string oldElement;

						if (functionline[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
						{
							size_t position = functionline[elementLine].find("numelements=\"") + 13;
							oldElement = functionline[elementLine].substr(position, functionline[elementLine].find("\">", position) - position);
						}
						else
						{
							oldElement = "$elements$";
						}

						if (oldElement != to_string(counter))
						{
							functionline[elementLine].replace(functionline[elementLine].find(oldElement), oldElement.length(), to_string(counter));
						}

						norElement = false;
						counter = 0;
						elementLine = -1;
					}
				}

				if (norElement)
				{
					string templine = templatelines[i];

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
							__int64 number = count(templatelines[i].begin(), templatelines[i].end(), '#');
							counter += int(number);
						}
					}
				}

				if ((templatelines[i].find("#" + masterFormat + "$") != NOT_FOUND || templatelines[i].find("#" + masterFormat + "_group$") != NOT_FOUND))
				{
					vecstr generator;
					size_t nextpos = -1;

					if(templatelines[i].find("\t\t\t#") != NOT_FOUND)
					{
						stringstream sstream(templatelines[i]);
						istream_iterator<string> ssbegin(sstream);
						istream_iterator<string> ssend;
						vecstr tempGenerator(ssbegin, ssend);
						copy(generator.begin(), generator.end(), generator.begin());
						generator = tempGenerator;
					}
					else
					{
						int ref = sameWordCount(templatelines[i], "#" + masterFormat);

						for (int j = 0; j < ref; ++j)
						{
							nextpos = templatelines[i].find("#" + masterFormat, nextpos + 1);
							string templine = templatelines[i].substr(nextpos);
							string ID = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							templine = templatelines[i].substr(nextpos, templatelines[i].find(ID, nextpos) - nextpos);
							generator.push_back(templine);
						}
					}

					nextpos = 0;

					for (unsigned int p = 0; p < generator.size(); p++)
					{
						string ID = generator[p];

						if (ID.find("#" + masterFormat + "$") != NOT_FOUND && multiOption == masterFormat)
						{
							nextpos = templatelines[i].find("#" + masterFormat + "$", nextpos) + 1;
							string tempID = templatelines[i].substr(nextpos);
							string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							curID = masterFormat + "$" + curID;

							if (tempID.find(curID, 0) != NOT_FOUND && nextpos == templatelines[i].find(curID))
							{
								if (subFunctionIDs[0][curID].length() == 0)
								{
									ErrorMessage(2017, format, i + 1);
									functionline.shrink_to_fit();
									return functionline;
								}

								templatelines[i].replace(nextpos, curID.length(), subFunctionIDs[0][curID]);
							}
							else
							{
								ErrorMessage(2018, format, i + 1, curID);
								functionline.shrink_to_fit();
								return functionline;
							}
						}
						else if (ID.find("#" + masterFormat + "_group") != NOT_FOUND && multiOption == masterFormat + "_group")
						{
							nextpos = templatelines[i].find("#" + masterFormat + "_group$", nextpos) + 1;
							string tempID = templatelines[i].substr(nextpos);
							string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							curID = multiOption + "$" + curID;

							if (tempID.find(curID, 0) != NOT_FOUND && nextpos == templatelines[i].find(curID))
							{
								for (unsigned int k = 0; k < subFunctionIDs.size(); ++k) // number of variation
								{
									if (subFunctionIDs[k][curID].length() == 0)
									{
										ErrorMessage(2017, format, i + 1);
										functionline.shrink_to_fit();
										return functionline;
									}

									templatelines[i].replace(nextpos, curID.length(), subFunctionIDs[k][curID]);
								}
							}
							else
							{
								ErrorMessage(2018, format, i + 1, curID);
								functionline.shrink_to_fit();
								return functionline;
							}
						}
					}
				}

				if (templatelines[i].find("$") != NOT_FOUND && groupCount != -1)
				{
					processing(templatelines[i], masterFormat, i + 1, subFunctionIDs, groupAnimInfo, eventid, variableid);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}
				}

				if (templatelines[i].find("<hkparam name=\"animationName\">") != NOT_FOUND)
				{
					size_t pos = templatelines[i].find("animationName\">") + 15;
					string animPath = templatelines[i].substr(pos, templatelines[i].find("</hkparam>", pos) - pos);
					boost::algorithm::to_lower(animPath);
					addUsedAnim(behaviorFile, animPath);
				}

				functionline.push_back(templatelines[i]);

				if (elementCatch)
				{
					elementLine = functionline.size() - 1;
				}

				break;
			}
		}

		if (templatelines[i].find("<!-- CLOSE -->", 0) != NOT_FOUND && IsConditionOpened[condition])
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

					if (order == -2)
					{
						order = 0;
						size = 1;
					}
					else if (order == -1)
					{
						order = 0;
						size = int(subFunctionIDs.size());
					}
					else
					{
						size = order + 1;
					}

					for (int animMulti = order; animMulti < size; ++animMulti) // variation for each animation >> multi animation
					{
						for (int optionMulti = 0; optionMulti < groupAnimInfo[animMulti]->optionPickedCount[multiOption]; ++optionMulti)
						{
							for (unsigned int l = 0; l < tempstore.size(); ++l) // part lines need to add
							{
								string curLine = tempstore[l];
								bool storeSkip = false;

								if (curLine.find("<!-- ") != NOT_FOUND && curLine.find(" -->", curLine.find("<!-- ")) != NOT_FOUND && curLine.find("$MC$") == NOT_FOUND)
								{
									bool isNot = false;
									size_t pos = curLine.find("<!-- ") + 5;
									string option = curLine.substr(pos, curLine.find(" -->") - pos);
									
									if (option[0] == '!')
									{
										isNot = true;
										option = option.substr(1);
									}

									if (option.find("L") != NOT_FOUND)
									{
										option.replace(option.find("L"), 1, to_string(groupAnimInfo.size() - 1));
									}

									if (option.find("F") != NOT_FOUND)
									{
										option.replace(option.find("F"), 1, "0");
									}

									if (sameWordCount(option, "$") == 2 && option.find("$(") != NOT_FOUND && option.find(")$", option.find("$(")) != NOT_FOUND)
									{
										pos = option.find("$(") + 1;
										option = option.substr(pos, option.find(")$") - pos + 1);
										calculate(option, error);

										if (error)
										{
											ErrorMessage(2020, format, i + 1, "$" + option + "$");
											functionline.shrink_to_fit();
											return functionline;
										}
									}

									if (to_string(animMulti) == option && isNot)
									{
										break;
									}
									else if (to_string(animMulti) != option && !isNot)
									{
										break;
									}

									storeSkip = true;
								}

								if (curLine.find("$") != NOT_FOUND)
								{
									// set animation ID
									if (curLine.find("$%$", 0) != NOT_FOUND)
									{
										curLine.replace(curLine.find("$%$"), 3, to_string(groupCount));
									}

									if (curLine.find("MID$", 0) != NOT_FOUND)
									{
										int counter = sameWordCount(curLine, "MID$");

										for (int k = 0; k < counter; ++k)
										{
											size_t MIDposition = curLine.find("MID$");
											string ID = boost::regex_replace(string(curLine.substr(MIDposition)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											string oldID = "MID$" + ID;

											if (curLine.find(oldID, MIDposition) != NOT_FOUND)
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

												string templine = curLine;
												templine.replace(templine.find("MID$", MIDposition), 3, format);
												templine = templine.substr(templine.find(format + "$", MIDposition), format.length() + oldID.length() - 3);
												functionIDs[templine] = ID;
												curLine.replace(curLine.find("MID$"), oldID.length(), ID);
											}
										}
									}

									// set state ID
									if (curLine.find("$(S", 0) != NOT_FOUND)
									{
										string templine = curLine.substr(curLine.find("$(S"));
										string ID = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
										int intID;

										if (curLine.find("$(S" + ID + "+") == NOT_FOUND)
										{
											ID = "";
											intID = 0;
										}
										else
										{
											intID = stoi(ID) - 1;

											if (intID >= int(fixedStateID.size()))
											{
												ErrorMessage(1127, format, i + 1);
												functionline.shrink_to_fit();
												return functionline;
											}
										}

										if (curLine.find("$(S" + ID + "+") != NOT_FOUND)
										{
											stateReplacer(curLine, ID, fixedStateID[intID], i + 1, groupCount);
										}
									}

									// multi choice selection
									if (curLine.find("$MC$", 0) != NOT_FOUND)
									{
										multiChoice(curLine, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat);

										if (error)
										{
											functionline.shrink_to_fit();
											return functionline;
										}
									}
								}

								if (curLine.find("%") != NOT_FOUND && curLine.find("+%") == NOT_FOUND)
								{
									__int64 counter = count(curLine.begin(), curLine.end(), '%');

									for (int p = 0; p < counter; ++p)
									{
										curLine.replace(curLine.find("%"), 1, to_string(optionMulti));
									}
								}

								if (norElement)
								{
									string templine = curLine;

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
											__int64 number = count(curLine.begin(), curLine.end(), '#');
											counter += int(number);
										}
									}
								}

								if (curLine.find("\t\t\t#") != NOT_FOUND && (curLine.find("#" + masterFormat + "$") != NOT_FOUND || curLine.find("#" + masterFormat + "_group$") != NOT_FOUND))
								{
									stringstream sstream(curLine);
									istream_iterator<string> ssbegin(sstream);
									istream_iterator<string> ssend;
									vecstr generator(ssbegin, ssend);
									copy(generator.begin(), generator.end(), generator.begin());
									size_t nextpos = 0;

									for (unsigned int p = 0; p < generator.size(); p++)
									{
										string ID = generator[p];

										if (ID.find("#" + masterFormat + "$") != NOT_FOUND && multiOption == masterFormat)
										{
											nextpos = curLine.find("#" + masterFormat + "$", nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = masterFormat + "$" + curID;

											if (tempID.find(curID, 0) != NOT_FOUND && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													ErrorMessage(2017, format, i + 1);
													functionline.shrink_to_fit();
													return functionline;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												ErrorMessage(2018, format, i + 1, curID);
												functionline.shrink_to_fit();
												return functionline;
											}
										}
										else if (ID.find("#" + masterFormat + "_group") != NOT_FOUND && multiOption == masterFormat + "_group")
										{
											nextpos = curLine.find("#" + multiOption, nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = multiOption + "$" + curID;

											if (tempID.find(curID, 0) != NOT_FOUND && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													ErrorMessage(2017, format, i + 1);
													functionline.shrink_to_fit();
													return functionline;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												ErrorMessage(2018, format, i + 1, curID);
												functionline.shrink_to_fit();
												return functionline;
											}
										}
									}
								}

								if (curLine.find("$") != NOT_FOUND)
								{
									processing(curLine, masterFormat, i + 1 + l - int(tempstore.size()), subFunctionIDs, groupAnimInfo, eventid, variableid, optionMulti, animMulti, multiOption);

									if (error)
									{
										functionline.shrink_to_fit();
										return functionline;
									}
								}

								if (!storeSkip)
								{
									if (curLine.find("<hkparam name=\"animationName\">") != NOT_FOUND)
									{
										size_t pos = curLine.find("animationName\">") + 15;
										string animPath = curLine.substr(pos, curLine.find("</hkparam>", pos) - pos);
										boost::algorithm::to_lower(animPath);
										addUsedAnim(behaviorFile, animPath);
									}

									functionline.push_back(curLine);
								}
							}
						}
					}

					order = -2;
				}

				tempstore.clear();
			}

			multi = false;
			open = false;
		}
		else if (templatelines[i].find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1118, format, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if (freeze && IsConditionOpened[condition])
			{
				if (templatelines[i].find("<hkparam name=\"animationName\">") != NOT_FOUND)
				{
					size_t pos = templatelines[i].find("animationName\">") + 15;
					string animPath = templatelines[i].substr(pos, templatelines[i].find("</hkparam>", pos) - pos);
					addUsedAnim(behaviorFile, animPath);
				}

				functionline.push_back(templatelines[i]);
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
			functionline.shrink_to_fit();
			return functionline;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, templatelines.size());
		functionline.shrink_to_fit();
		return functionline;
	}

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			ErrorMessage(1120, format, behaviorFile);
			functionline.shrink_to_fit();
			return functionline;
			break;
		}
	}

	if (fixedStateID.size() > 1)
	{
		for (unsigned int i = 0; i < fixedStateID.size(); ++i)
		{
			functionIDs[format + "[" + to_string(groupCount) + "][(S+" + to_string(i) + ")]"] = to_string(fixedStateID[i]);
		}
	}
	else
	{
		functionIDs[format + "[" + to_string(groupCount) + "][(S+0)]"] = to_string(fixedStateID[0]);
	}


	if (functionline.size() != 0 && functionline.back().length() != 0)
	{
		functionline.push_back("");
	}

	functionline.shrink_to_fit();
	return functionline;
}

SSMap groupTemplate::getFunctionIDs()
{
	return functionIDs;
}

vecstr ExistingFunction::groupExistingFunctionProcess(int curFunctionID, vecstr existingFunctionLines, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, string curformat, ImportContainer import, id eventid, id variableid, int& nFunctionID, bool hasMaster, bool hasGroup, vecstr templateGroup)
{
	vecstr newFunctionLines;
	vecstr tempstore;
	vecstr empty;

	newImport = const_cast<ImportContainer*>(&import);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	format = curformat;
	strID = to_string(nFunctionID);

	string multiOption;
	bool skipTemplate = false;
	bool newOpen = false;
	bool functionNot = false;
	bool isElement = false;
	bool multi = false;
	bool open = false;
	bool skip = false;
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	int condition = 0;
	__int64 openRange = 0;
	__int64 elementCount = 0;
	int counter = 0;
	int order = -2;
	int elementLine = 0;
	unordered_map<int, bool> IsConditionOpened;
	unordered_map<string, bool> otherAnimType;
	vector<unordered_map<string, bool>> groupOptionPicked;

	// check error before initialization
	if (subFunctionIDs.size() != groupAnimInfo.size() && !hasGroup && !groupAnimInfo[0]->ignoreGroup)
	{
		ErrorMessage(1123, format + "(#" + to_string(curFunctionID) + ")");
		return newFunctionLines;
	}

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}


	for (unsigned int i = 0; i < 4 - strID.length(); ++i)
	{
		strID = "0" + strID;
	}

	for (unsigned int i = 0; i < templateGroup.size(); ++i)
	{
		if (templateGroup[i] != format)
		{
			otherAnimType[templateGroup[i]] = true;
			otherAnimType[templateGroup[i] + "_group"] = true;
			otherAnimType[templateGroup[i] + "_master"] = true;
		}
	}

	IsConditionOpened[0] = true;
	newFunctionLines.reserve(existingFunctionLines.size() + 20 * memory);

	for (unsigned int i = 0; i < existingFunctionLines.size(); ++i)
	{
		bool uniqueskip = false;
		bool elementCatch = false;

		string line = existingFunctionLines[i];

		if (line.find("<!-- CONDITION START", 0) != NOT_FOUND)
		{
			condition++;

			if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
			{
				if (!freeze)
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
							string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);

							if (newCondition(conditionLine, groupOptionPicked, groupAnimInfo, i + 1, format, format))
							{
								skip = false;
								IsConditionOpened[condition] = true;
							}
							else
							{
								skip = true;
							}

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
				ErrorMessage(1121, format, curFunctionID, i + 1);
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, i + 1);
				return empty;
			}

			if (!freeze)
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
						string option = line.substr(optionPosition, line.find("^", optionPosition) - optionPosition);

						if (newCondition(option, groupOptionPicked, groupAnimInfo, i + 1, format, format))
						{
							skip = false;
							IsConditionOpened[condition] = true;
							uniqueskip = true;
							string replacement1 = line.substr(0, optionPosition - 1);
							string replacement2 = line.substr(optionPosition - 2);
							newFunctionLines.push_back(replacement1 + "START" + replacement2);
						}
						else
						{
							skip = true;
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
				return empty;
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
						newFunctionLines.push_back(replacement1 + "START" + replacement2);
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
		else if (line.find("<!-- NEW ^" + format + "^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, existingFunctionLines.size());
				return empty;
			}

			if(IsConditionOpened[condition])
			{
				if (!hasGroup)
				{
					if (!open)
					{
						open = true;
						multi = true;
						order = -1;
						multiOption = format;
					}
					else
					{
						ErrorMessage(1115, format, i + 1);
						return empty;
					}
				}
				else
				{
					ErrorMessage(1161, format, curFunctionID, i + 1);
					return empty;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^" + format + "_group^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, existingFunctionLines.size());
				return empty;
			}

			if (IsConditionOpened[condition])
			{
				if (hasGroup && !hasMaster)
				{
					if (!open)
					{
						open = true;
						multi = true;
						order = -1;
						multiOption = format + "_group";
					}
					else
					{
						ErrorMessage(1115, format, i + 1);
						return empty;
					}
				}
				else
				{
					ErrorMessage(1167, format + "(#" + to_string(curFunctionID) + ")", i + 1);
					return empty;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^" + format + "_master^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, existingFunctionLines.size());
				return empty;
			}

			if (IsConditionOpened[condition])
			{
				if (hasMaster)
				{
					if (!open)
					{
						open = true;
						multi = true;
						order = -1;
						multiOption = format + "_master";
					}
					else
					{
						ErrorMessage(1115, format, i + 1);
						return empty;
					}
				}
				else
				{
					ErrorMessage(1162, format + "(#" + to_string(curFunctionID) + ")", i + 1);
					return empty;
				}
			}

			newOpen = true;
			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^" + format + "^ -->", 0) != NOT_FOUND || line.find("<!-- NEW ^" + format + "_group^ -->", 0) != NOT_FOUND || line.find("<!-- NEW ^" + format + "_master^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, existingFunctionLines.size());
				return empty;
			}

			ErrorMessage(1164, format + "(#" + to_string(curFunctionID) + ")", i + 1);
			return empty;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, existingFunctionLines.size());
				return empty;
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

					if (curOption.find(format + "[") != NOT_FOUND && curOption.find("]") != NOT_FOUND)
					{
						int pos = 0;
						vecstr formatInfo = GetOptionInfo(curOption, format, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, false, true);

						if (error)
						{
							return empty;
						}

						if (formatInfo[2] == "AnimObject")
						{
							ErrorMessage(1129, format, curFunctionID, i + 1, curOption);
							return empty;
						}

						if (groupOptionPicked[stoi(formatInfo[1])][formatInfo[2]])
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
					ErrorMessage(1116, format, i + 1);
					return empty;
				}
			}

			newOpen = true;
			size_t pos = line.find("<!-- NEW ^") + 10;
			string checker = line.substr(pos, line.find("^", pos) - pos);

			if (!otherAnimType[checker])
			{
				uniqueskip = true;
			}
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, existingFunctionLines.size());
				return empty;
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

					if (curOption.find(format + "[") != NOT_FOUND && curOption.find("]") != NOT_FOUND)
					{
						int pos = 0;
						vecstr formatInfo = GetOptionInfo(curOption, format, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, true, true);

						if (error)
						{
							return empty;
						}

						if (isNot)
						{
							skip = true;
						}
						else
						{
							open = true;
							multi = true;

							if (formatInfo[1].length() == 0)
							{
								order = -1;
							}
							else
							{
								order = stoi(formatInfo[1]);
							}
						}
					}
				}
				else
				{
					ErrorMessage(1115, format, i + 1);
					return empty;
				}
			}

			newOpen = true;
			size_t pos = line.find("<!-- NEW ^") + 10;
			string checker = line.substr(pos, line.find("^", pos) - pos);

			if (!otherAnimType[checker])
			{
				uniqueskip = true;
			}
			else
			{
				skipTemplate = true;
			}
		}
		else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (!newOpen)
			{
				ErrorMessage(1171, format, i + 1);
				return empty;
			}

			newOpen = false;

			if (skipTemplate)
			{
				skipTemplate = false;
			}
			else
			{
				uniqueskip = true;
			}
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			uniqueskip = true;
		}

		if (!uniqueskip && !skip && !freeze)
		{
			while (true)
			{
				if (multi)
				{
					tempstore.push_back(line);
					break;
				}

				// set animation ID
				if (line.find("$%$", 0) != NOT_FOUND)
				{
					line.replace(line.find("$%$"), 3, "0");
				}

				// multi choice selection
				if (line.find("$MC$", 0) != NOT_FOUND)
				{
					multiChoice(line, groupOptionPicked, groupAnimInfo, i + 1, format, format);

					if (error)
					{
						return empty;
					}
				}

				// compute numelements
				if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND)
				{
					if (!isElement)
					{
						isElement = true;
						elementCatch = true;
						string templine = line.substr(0, line.find("<hkparam name=\"", 0));
						openRange = count(templine.begin(), templine.end(), '\t');
					}
					else
					{
						ErrorMessage(2019, format + "(#" + to_string(curFunctionID) + ")", i + 1);
						return empty;
					}
				}
				else if (line.find("</hkparam>") != NOT_FOUND && isElement)
				{
					string templine = line.substr(0, line.find("</hkparam>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						string oldElement;

						if (newFunctionLines[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
						{
							size_t position = newFunctionLines[elementLine].find("numelements=\"") + 13;
							oldElement = newFunctionLines[elementLine].substr(position, newFunctionLines[elementLine].find("\">", position) - position);
						}
						else
						{
							oldElement = "$elements$";
						}

						if (oldElement != to_string(elementCount))
						{
							newFunctionLines[elementLine].replace(newFunctionLines[elementLine].find(oldElement), oldElement.length(), to_string(elementCount));
						}

						isElement = false;
						elementCount = 0;
						elementLine = NULL;
					}
				}

				if (isElement)
				{
					string templine = line;

					if (templine.find("<hkobject>") != NOT_FOUND)
					{
						templine = templine.substr(0, templine.find("<hkobject>"));
						__int64 range = count(templine.begin(), templine.end(), '\t');

						if (range == openRange + 1)
						{
							elementCount++;
						}
					}
					else if (templine.find("\t\t\t#") != NOT_FOUND)
					{
						templine = templine.substr(0, templine.find("#", 0));
						__int64 reference = count(templine.begin(), templine.end(), '\t');

						if (reference == openRange + 1)
						{
							__int64 number = count(line.begin(), line.end(), '#');
							elementCount += number;
						}
					}
				}
				
				if (line.find("$") != NOT_FOUND && !hasGroup)
				{
					processing(line, curFunctionID, i + 1, subFunctionIDs, groupAnimInfo, eventid, variableid, hasGroup);
					
					if (error)
					{
						return empty;
					}
				}

				newFunctionLines.push_back(line);

				if (elementCatch)
				{
					elementLine = newFunctionLines.size() - 1;
				}

				break;
			}
		}

		if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
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

					if (order == -2)
					{
						order = 0;
						size = 1;
					}
					else if (order == -1)
					{
						order = 0;
						size = int(subFunctionIDs.size());
					}
					else
					{
						size = order + 1;
					}

					for (int animMulti = order; animMulti < size; ++animMulti) // each animation in a group
					{
						for (int optionMulti = 0; optionMulti < groupAnimInfo[animMulti]->optionPickedCount[multiOption]; ++optionMulti)		// option with different add-ons
						{
							for (unsigned int l = 0; l < tempstore.size(); ++l) // part lines need to add
							{
								string curLine = tempstore[l];
								size_t linecount = i + 1 - tempstore.size() + l + (optionMulti * tempstore.size()) + (groupAnimInfo[animMulti]->optionPickedCount[multiOption] * animMulti * tempstore.size());

								if (curLine.find("<hkparam name=\"") != NOT_FOUND && curLine.find("numelements=\"") != NOT_FOUND && curLine.find("</hkparam>") == NOT_FOUND && curLine.find("<!-- COMPUTE -->", 0) != NOT_FOUND)
								{
									ErrorMessage(1140, format, curFunctionID, linecount);
									return empty;
								}

								if (isElement)
								{
									string templine = curLine;

									if (templine.find("<hkobject>") != NOT_FOUND)
									{
										templine = templine.substr(0, templine.find("<hkobject>"));
										__int64 range = count(templine.begin(), templine.end(), '\t');

										if (range == openRange + 1)
										{
											elementCount++;
										}
									}
									else if (templine.find("\t\t\t#") != NOT_FOUND)
									{
										templine = templine.substr(0, templine.find("#", 0));
										__int64 reference = count(templine.begin(), templine.end(), '\t');

										if (reference == openRange + 1)
										{
											__int64 number = count(curLine.begin(), curLine.end(), '#');
											elementCount += number;
										}
									}
								}

								if (curLine.find("\t\t\t#") != NOT_FOUND && (curLine.find("#" + format + "$") != NOT_FOUND || curLine.find("#" + format + "_group$") != NOT_FOUND || curLine.find("#" + format + "_master") != NOT_FOUND))
								{
									stringstream sstream(curLine);
									istream_iterator<string> ssbegin(sstream);
									istream_iterator<string> ssend;
									vecstr generator(ssbegin, ssend);
									copy(generator.begin(), generator.end(), generator.begin());
									size_t nextpos = 0;

									for (unsigned int p = 0; p < generator.size(); p++)
									{
										string ID = generator[p];

										if (ID.find("#" + format + "$") != NOT_FOUND && multiOption == format)
										{
											nextpos = curLine.find("#" + format + "$", nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = format + "$" + curID;

											if (tempID.find(curID, 0) != NOT_FOUND && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													ErrorMessage(2011, format + "(#" + to_string(curFunctionID) + ")", linecount, curID);
													return empty;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												ErrorMessage(2012, format + "(#" + to_string(curFunctionID) + ")", linecount, curID);
												return empty;
											}
										}
										else if (ID.find("#" + format + "_group") != NOT_FOUND && multiOption == format + "_group")
										{
											nextpos = curLine.find("#" + multiOption, nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = multiOption + "$" + curID;

											if (tempID.find(curID, 0) != NOT_FOUND && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													ErrorMessage(2011, format + "(#" + to_string(curFunctionID) + ")", linecount, curID);
													return empty;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												ErrorMessage(2012, format + "(#" + to_string(curFunctionID) + ")", linecount, curID);
												return empty;
											}

										}
										else if (ID.find("#" + format + "_master") != NOT_FOUND && multiOption == format + "_master")
										{
											nextpos = curLine.find("#" + multiOption, nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = multiOption + "$" + curID;

											if (tempID.find(curID, 0) != NOT_FOUND && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													ErrorMessage(2011, format + "(#" + to_string(curFunctionID) + ")", linecount, curID);
													return empty;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												ErrorMessage(2012, format + "(#" + to_string(curFunctionID) + ")", linecount, curID);
												return empty;
											}

										}
									}
								}

								if (curLine.find("%") != NOT_FOUND)
								{
									__int64 counter = count(curLine.begin(), curLine.end(), '%');

									for (int p = 0; p < counter; ++p)
									{
										curLine.replace(curLine.find("%"), 1, to_string(l));
									}
								}

								// multi choice selection
								if (curLine.find("$MC$", 0) != NOT_FOUND)
								{
									multiChoice(curLine, groupOptionPicked, groupAnimInfo, int(linecount), format, format);

									if (error)
									{
										return empty;
									}
								}

								// set animation ID
								if (curLine.find("$%$", 0) != NOT_FOUND)
								{
									curLine.replace(curLine.find("$%$"), 3, "0");
								}

								if (curLine.find("$") != NOT_FOUND && !hasGroup)
								{
									processing(curLine, curFunctionID, int(i + 1 - tempstore.size() + l + (optionMulti * animMulti * tempstore.size())), subFunctionIDs, groupAnimInfo, eventid, variableid, hasGroup, optionMulti, animMulti, multiOption);

									if (error)
									{
										return empty;
									}
								}

								if (tempstore[l].find("<!-- NEW ^" + format, 0) == NOT_FOUND && tempstore[l].find("<!-- CLOSE -->", 0) == NOT_FOUND)
								{
									newFunctionLines.push_back(curLine);
								}
							}
						}
					}
				}

				tempstore.clear();
				tempstore.reserve(existingFunctionLines.size() / 10);
			}

			open = false;
			multi = false;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1118, format, i + 1);
				return empty;
			}

			if (freeze && IsConditionOpened[condition])
			{
				newFunctionLines.push_back(line);
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
			return empty;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, existingFunctionLines.size());
		return empty;
	}

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			ErrorMessage(1120, format, "#" + to_string(curFunctionID));
			return empty;
		}
	}

	if (newFunctionLines.size() != 0 && newFunctionLines.back().length() != 0)
	{
		newFunctionLines.push_back("");
	}

	newFunctionLines.shrink_to_fit();	
	return newFunctionLines;
}

void groupTemplate::stateReplacer(string& line, string statenum, int stateID, int linecount, int groupCount)
{
	int count = sameWordCount(line, "$(S" + statenum + "+");

	for (int i = 0; i < count; ++i)
	{
		string number = boost::regex_replace(string(line.substr(line.find("$(S" + statenum + "+") + statenum.length() + 4)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
		string state = "$(S" + statenum + "+" + number + ")$";

		if (line.find(state, 0) != NOT_FOUND)
		{
			size_t stateposition = line.find(state, 0);

			if (state == line.substr(stateposition, line.find(")$", stateposition) - stateposition + 2))
			{
				size_t stateLength = state.length();
				state = state.substr(1, state.length() - 2);
				state.replace(1, 1 + statenum.length(), to_string(stateID));
				calculate(state, error);
				functionIDs[format + "[" + to_string(groupCount) + "][(S" + statenum + "+" + number + ")]"] = state;
				line.replace(stateposition, stateLength, state);

				int ID;

				if (statenum.length() > 0)
				{
					ID = stoi(statenum) - 1;
				}
				else
				{
					ID = 0;
				}

				if (ID >= int((*nextStateID).size()))
				{
					ErrorMessage(1168, format, linecount, state);
					return;
				}

				if (stoi(state) >= (*nextStateID)[ID])
				{
					(*nextStateID)[ID] = stoi(state) + 1;
				}
			}
			else
			{
				ErrorMessage(1137, format, linecount, state);
				return;
			}
		}
	}
}

void groupTemplate::processing(string& line, string masterFormat, int linecount, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, id eventid, id variableid, int optionMulti, int animMulti, string multiOption)
{
	__int64 counter = count(line.begin(), line.end(), '$') / 2;
	size_t curPos = 0;

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

						if (equation != "(S" + ID + "+" + number + ")")
						{
							size_t equationLength = equation.length();

							if (equation.find("L") != NOT_FOUND)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'L');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("L"), 1, to_string(groupAnimInfo.size() - 1));
								}
							}

							calculate(equation, error);

							if (stoi(equation) > int(groupAnimInfo.size() - 1) || stoi(equation) < 0)
							{
								ErrorMessage(1155, groupAnimInfo.size() - 1, format, linecount, change);
								return;
							}

							change.replace(nextpos, equationLength, equation);
							isChange = true;
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			if (change.find(masterFormat + "[", 0) != NOT_FOUND && change.find("][", change.find(masterFormat + "[")) != NOT_FOUND)
			{
				formatReplace(change, masterFormat, subFunctionIDs[animMulti], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			size_t position = change.find("eventID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				eventIDReplacer(change, format, eventid, zeroEvent, linecount);
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
						ErrorMessage(2013, format, linecount);
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

			if (isChange)
			{
				line.replace(line.find(oldChange), oldChange.length(), change);
			}

			break;
		}
	}
}

void ExistingFunction::processing(string& line, int curFunctionID, int linecount, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, id eventid, id variableid, bool hasGroup, int optionMulti, int animMulti, string multiOption)
{
	__int64 counter = count(line.begin(), line.end(), '$') / 2;
	size_t curPos = 0;

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
						
						if (equation != "(S" + ID + "+" + number + ")")
						{
							size_t equationLength = equation.length();

							if (equation.find("L") != NOT_FOUND)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'L');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("L"), 1, to_string(groupAnimInfo.size() - 1));
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

			if (hasGroup)
			{
				if (change.find("[FilePath]") != NOT_FOUND)
				{
					if (change.find(format + "[][FilePath]", 0) != NOT_FOUND)
					{
						ErrorMessage(1052, format, linecount, line);
						return;
					}

					if (change.find(format + "[F][FilePath]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[F][FilePath]"), 13 + format.length(), subFunctionIDs[animMulti][format + "[0][FilePath]"]);
						isChange = true;
					}

					if (change.find(format + "[N][FilePath]", 0) != NOT_FOUND)
					{
						ErrorMessage(1056, format, linecount, line);
						return;
					}

					if (change.find(format + "[L][FilePath]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[L][FilePath]"), 13 + format.length(), subFunctionIDs[animMulti][format + "[" + to_string(groupAnimInfo.size() - 1) + "][FilePath]"]);
						isChange = true;
					}

					if (change.find(format + "[", 0) != NOT_FOUND)
					{
						string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (change.find(format + "[" + number + "][FilePath]", 0) != NOT_FOUND)
						{
							if (unsigned int(stoi(number)) >= groupAnimInfo.size())
							{
								change.replace(change.find(format + "[" + number + "][FilePath]"), 8 + format.length() + number.length(), subFunctionIDs[animMulti][format + "[" + number + "][FilePath]"]);
								isChange = true;
							}
							else
							{
								ErrorMessage(1148, format, linecount, change);
								return;
							}
						}
					}
				}

				if (change.find("[FileName]") != NOT_FOUND)
				{
					if (change.find(format + "[][FileName]", 0) != NOT_FOUND)
					{
						ErrorMessage(1052, format, linecount, line);
						return;
					}

					if (change.find(format + "[F][FileName]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[F][FileName]"), 13 + format.length(), subFunctionIDs[animMulti][format + "[0][FileName]"]);
						isChange = true;
					}

					if (change.find(format + "[N][FileName]", 0) != NOT_FOUND)
					{
						ErrorMessage(1056, format, linecount, line);
						return;
					}

					if (change.find(format + "[L][FileName]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[L][FileName]"), 13 + format.length(), subFunctionIDs[animMulti][format + "[" + to_string(groupAnimInfo.size() - 1) + "][FileName]"]);
						isChange = true;
					}

					if (change.find(format + "[", 0) != NOT_FOUND)
					{
						string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (change.find(format + "[" + number + "][FileName]", 0) != NOT_FOUND)
						{
							if (unsigned int(stoi(number)) >= groupAnimInfo.size())
							{
								change.replace(change.find(format + "[" + number + "][FileName]"), 8 + format.length() + number.length(), subFunctionIDs[animMulti][format + "[" + number + "][FileName]"]);
								isChange = true;
							}
							else
							{
								ErrorMessage(1148, format, linecount, change);
								return;
							}
						}
					}
				}

				if (change.find(format + "[][", 0) != NOT_FOUND && change.find("]", change.find(format + "[][")) != NOT_FOUND)
				{
					formatReplace(change, format, subFunctionIDs[animMulti], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
					isChange = true;
				}

				if (error)
				{
					ErrorMessage(2014, format + "(#" + to_string(curFunctionID) + ")", linecount);
					return;
				}

				if (change.find(format + "[F][", 0) != NOT_FOUND && change.find("]", change.find(format + "[F][")) != NOT_FOUND)
				{
					formatReplace(change, format, subFunctionIDs[0], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
					isChange = true;
				}

				if (error)
				{
					ErrorMessage(2014, format + "(#" + to_string(curFunctionID) + ")", linecount);
					return;
				}

				if (change.find(format + "[L][", 0) != NOT_FOUND && change.find("]", change.find(format + "[L][")) != NOT_FOUND)
				{
					formatReplace(change, format, subFunctionIDs[groupAnimInfo.size() - 1], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
					isChange = true;
				}

				if (error)
				{
					ErrorMessage(2014, format + "(#" + to_string(curFunctionID) + ")", linecount);
					return;
				}

				if (change.find(format + "_group[][", 0) != NOT_FOUND && change.find("]", change.find(format + "_group[][")) != NOT_FOUND)
				{
					formatReplace(change, format + "_group", subFunctionIDs[animMulti], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
					isChange = true;
				}

				if (error)
				{
					ErrorMessage(2014, format + "(#" + to_string(curFunctionID) + ")", linecount);
					return;
				}
			}
			else
			{
				for (auto it = subFunctionIDs[animMulti].begin(); it != subFunctionIDs[animMulti].end(); ++it)
				{
					if (change.find(it->first) != NOT_FOUND)
					{
						int counter = sameWordCount(change, it->first);

						for (int j = 0; j < counter; ++j)
						{
							change.replace(change.find(it->first), it->first.length(), it->second);
							isChange = true;
						}

						if (change.length() == it->second.length())
						{
							break;
						}
					}
				}
			}

			size_t position = change.find("eventID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				eventIDReplacer(change, format, eventid, zeroEvent, linecount);
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
						ErrorMessage(2013, format + "(#" + to_string(curFunctionID) + ")", linecount);
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

			if (change.find("MD") != NOT_FOUND)
			{
				ErrorMessage(1096, format, linecount);
				return;
			}

			if (change.find("RD") != NOT_FOUND)
			{
				ErrorMessage(1097, format, linecount);
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

void multiChoice(string& line, vector<unordered_map<string, bool>> groupOptionPicked, vector<shared_ptr<animationInfo>> groupAnimInfo, int numline, string templatename, string masterformat)
{
	if (line.find("<!-- ", 0) != NOT_FOUND)
	{
		size_t nextposition = 0;
		int choicecount = 0;

		while (true)
		{
			if (line.find("<!--", nextposition) != NOT_FOUND)
			{
				choicecount++;
				nextposition = line.find("<!-- ", nextposition) + 1;
			}
			else
			{
				break;
			}
		}

		if (line.find("*", 0) != NOT_FOUND || line.find("@", 0) != NOT_FOUND || line.find("%", 0) != NOT_FOUND)
		{
			ErrorMessage(2015, templatename, numline);
			return;
		}

		bool none = true;
		nextposition = 0;

		for (int i = 0; i < choicecount; ++i)
		{
			vecstr opt;
			vector<char> storechar;
			nextposition = line.find("<!-- ", nextposition) + 5;
			string tempstr = line.substr(nextposition, line.find(" ", nextposition) - nextposition);

			if (line.find("<!-- " + tempstr + " -->", 0) == NOT_FOUND)
			{
				if (newCondition(tempstr, groupOptionPicked, groupAnimInfo, numline, templatename, masterformat))
				{
					nextposition = line.find(" ", nextposition) + 1;
					string output = line.substr(nextposition, line.find(" ", nextposition) - nextposition);
					line.replace(line.find("$MC$", 0), 4, output);
					none = false;
					break;
				}
			}
			else if (i == choicecount - 1)
			{
				line.replace(line.find("$MC$", 0), 4, tempstr);
				none = false;
				break;
			}
			else
			{
				ErrorMessage(2016, templatename, numline);
				return;
			}
		}

		if (none)
		{
			line.replace(line.find("$MC$", 0), 4, "null");
		}
	}
	else
	{
		line.replace(line.find("$MC$"), 4, "null");
	}

	line = line.substr(0, line.find("</hkparam>") + 10);
}

bool newCondition(string condition, vector<unordered_map<string, bool>> curOptionPicked, vector<shared_ptr<animationInfo>> groupAnimInfo, int numline, string templatename, string masterformat)
{
	if (condition[0] == '(')
	{
		size_t backB = condition.find(")", 0);
		string inHouse = condition.substr(1, backB - 1);
		size_t x = inHouse.find("&");
		size_t y = inHouse.find("|");

		bool inHouseResult;

		if (x == NOT_FOUND && y == NOT_FOUND)
		{
			bool isNot = false;

			if (inHouse[0] == '!')
			{
				isNot = true;
				inHouse = inHouse.substr(1);
			}
			
			vecstr optionInfo = GetOptionInfo(inHouse, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

			if (error)
			{
				return false;
			}

			if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
			{
				inHouseResult = !isNot;
			}
			else
			{
				// Check if current condition accepts other options that are linked
				inHouseResult = isNot;
			}
		}
		else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
		{
			string firstCondition = inHouse.substr(0, inHouse.find("|"));
			string secondCondition = inHouse.substr(inHouse.find("|") + 1);
			bool isNot = false;

			if (firstCondition[0] == '!')
			{
				isNot = true;
				firstCondition = firstCondition.substr(1);
			}

			vecstr optionInfo = GetOptionInfo(firstCondition, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

			if (error)
			{
				return false;
			}

			if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
			{
				inHouseResult = !isNot;
			}
			else
			{
				inHouseResult = isNot;
			}

			if (!inHouseResult && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
			{
				inHouseResult = true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string firstCondition = inHouse.substr(0, inHouse.find("&"));
			string tempSecondCondition = inHouse.substr(inHouse.find("&") + 1);
			string secondCondition;
			bool isNot = false;

			if (firstCondition[0] == '!')
			{
				isNot = true;
				firstCondition = firstCondition.substr(1);
			}

			vecstr optionInfo = GetOptionInfo(firstCondition, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

			if (error)
			{
				return false;
			}

			size_t x = tempSecondCondition.find("&");
			size_t y = tempSecondCondition.find("|");

			if ((x == NOT_FOUND || x > y) && y != NOT_FOUND)
			{
				secondCondition = tempSecondCondition.substr(0, tempSecondCondition.find("|"));
				tempSecondCondition = tempSecondCondition.substr(tempSecondCondition.find("|") + 1);

				if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
				{
					if (isNot)
					{
						inHouseResult = false;
					}
					else
					{
						inHouseResult = newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat);
					}
				}
				else
				{
					if (isNot)
					{
						inHouseResult = newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat);
					}
					else
					{
						inHouseResult = false;
					}
				}

				if (!inHouseResult && newCondition(tempSecondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
				{
					inHouseResult = true;
				}
			}
			else
			{
				secondCondition = tempSecondCondition;

				if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
				{
					if (isNot)
					{
						inHouseResult = false;
					}
					else
					{
						inHouseResult = newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat);
					}
				}
				else
				{
					if (isNot)
					{
						inHouseResult = newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat);
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
			ErrorMessage(1105, templatename, numline);
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

			if (inHouseResult || newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
			{
				return true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string secondCondition = inHouse.substr(inHouse.find("&") + 1);

			if (inHouseResult && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
			{
				return true;
			}
		}
		else
		{
			ErrorMessage(1106, templatename, numline, condition);
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
			bool isNot = false;

			if (conditionOrder[0] == '!')
			{
				isNot = true;
				conditionOrder = conditionOrder.substr(1);
			}

			vecstr optionInfo = GetOptionInfo(conditionOrder, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

			if (error)
			{
				return false;
			}

			if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
			{
				return !isNot;
			}
			else
			{
				return isNot;
			}
		}
		else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
		{
			string firstCondition = condition.substr(0, condition.find("|"));
			string secondCondition = condition.substr(condition.find("|") + 1);
			bool isNot = false;

			if (firstCondition[0] == '!')
			{
				isNot = true;
				firstCondition = firstCondition.substr(1);
			}

			vecstr optionInfo = GetOptionInfo(firstCondition, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

			if (error)
			{
				return false;
			}

			if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
			{
				if (!isNot)
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
			}

			if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
			{
				return true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string firstCondition = condition.substr(0, condition.find("&"));
			string secondCondition = condition.substr(condition.find("&") + 1);
			bool isNot = false;

			if (firstCondition[0] == '!')
			{
				isNot = true;
				firstCondition = firstCondition.substr(1);
			}

			vecstr optionInfo = GetOptionInfo(firstCondition, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

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
						if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
						{
							if (!isNot && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
							{
								return true;
							}
						}
						else
						{
							if (isNot && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
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
							if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
							{
								if (!isNot && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat) && newCondition(thirdCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
								{
									return true;
								}
							}
							else
							{
								if (isNot)
								{
									if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat) && newCondition(thirdCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
									{
										return true;
									}
								}
							}
						}
						else if (logic == '|')
						{
							if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
							{
								if (!isNot)
								{
									if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
									{
										return true;
									}
								}
							}
							else
							{
								if (isNot)
								{
									if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
									{
										return true;
									}
								}
							}

							if (newCondition(thirdCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
							{
								return true;
							}
						}
						else
						{
							ErrorMessage(1101, templatename, numline);
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
						if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
						{
							if (!isNot)
							{
								bool isAnotherNot = false;

								if (secondCondition[0] == '!')
								{
									isAnotherNot = true;
									secondCondition = secondCondition.substr(1);
								}

								vecstr optionInfo2 = GetOptionInfo(secondCondition, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

								if (error)
								{
									return false;
								}

								if (curOptionPicked[stoi(optionInfo2[1])][optionInfo2[2]])
								{
									if (!isAnotherNot)
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
								}
							}
						}
						else
						{
							if (isNot)
							{
								bool isAnotherNot = false;

								if (secondCondition[0] == '!')
								{
									isAnotherNot = true;
									secondCondition = secondCondition.substr(1);
								}

								vecstr optionInfo2 = GetOptionInfo(secondCondition, masterformat, numline, curOptionPicked.size() - 1, groupAnimInfo, false, true);

								if (error)
								{
									return false;
								}

								if (curOptionPicked[stoi(optionInfo2[1])][optionInfo2[2]])
								{
									if (!isAnotherNot)
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
								}
							}
						}
					}
					else if (x == NOT_FOUND || (x > y && y != NOT_FOUND))
					{
						size_t position = secondCondition.find("|") + 1;
						string thirdCondition = secondCondition.substr(position);
						secondCondition = secondCondition.substr(0, position - 1);

						if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
						{
							if (!isNot)
							{
								if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
								{
									return true;
								}
							}
						}
						else
						{
							if (isNot)
							{
								if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
								{
									return true;
								}
							}
						}

						if (newCondition(thirdCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
						{
							return true;
						}
					}
					else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
					{
						size_t position = secondCondition.find("&") + 1;
						string thirdCondition = secondCondition.substr(position);
						secondCondition = secondCondition.substr(0, position - 1);

						if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
						{
							if (!isNot)
							{
								if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat) && newCondition(thirdCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
								{
									return true;
								}
							}
						}
						else
						{
							if (isNot)
							{
								if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat) && newCondition(thirdCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
								{
									return true;
								}
							}
						}
					}
					else
					{
						ErrorMessage(1103, templatename, numline);
						return false;
					}
				}
			}
			else
			{
				if (curOptionPicked[stoi(optionInfo[1])][optionInfo[2]])
				{
					if (!isNot && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
					{
						return true;
					}
				}
				else
				{
					if (isNot)
					{
						if (newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
						{
							return true;
						}
					}
				}
			}
		}
		else
		{
			ErrorMessage(1102, templatename, numline);
			return false;
		}
	}

	return false;
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

inline void groupTemplate::newID()
{
	(*nextFunctionID)++;
	strID = to_string(*nextFunctionID);
	size_t zero = 4 - strID.length();

	for (size_t j = 0; j < zero; j++)
	{
		strID = "0" + strID;
	}
}

void groupTemplate::setZeroEvent(string eventname)
{
	zeroEvent = eventname;
}

inline void ExistingFunction::newID()
{
	(*nextFunctionID)++;
	strID = to_string(*nextFunctionID);
	size_t zero = 4 - strID.length();

	for (size_t j = 0; j < zero; j++)
	{
		strID = "0" + strID;
	}
}

void ExistingFunction::setZeroEvent(string eventname)
{
	zeroEvent = eventname;
}

void formatReplace(string& curline, string format, SSMap subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, int linecount, size_t lastOrder, int optionMulti, int animMulti)
{
	bool isAnimMulti;

	if (animMulti == -1)
	{
		isAnimMulti = false;
	}
	else
	{
		isAnimMulti = true;
	}
	
	vecstr groupline = GetOptionInfo(curline, format, linecount, lastOrder, groupAnimInfo, isAnimMulti, false, true, animMulti, optionMulti);

	if (error)
	{
		return;
	}

	size_t pos = curline.find(format + "[");
	string originalLine = curline.substr(pos, curline.find("]", curline.find_last_of("[")) - pos + 1);
	string subFunction = format;

	if (groupline[1].length() == 0 && isAnimMulti)
	{
		groupline[1] = to_string(animMulti);
	}

	if (groupline.back() == "@AnimObject")
	{
		if (optionMulti == -1)
		{
			ErrorMessage(1128, format, linecount);
			return;
		}

		groupline.back() = "@AnimObject/" + to_string(optionMulti + 1);
	}

	for (unsigned int i = 1; i < groupline.size(); ++i)
	{
		subFunction = subFunction + "[" + groupline[i] + "]";
	}

	if (subFunctionIDs[subFunction].length() == 0 || groupline.size() < 3)
	{
		ErrorMessage(1158, format, linecount, curline);
		return;
	}

	curline.replace(curline.find(originalLine), originalLine.length(), subFunctionIDs[subFunction]);
}
