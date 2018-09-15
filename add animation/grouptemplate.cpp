#include "grouptemplate.h"

#pragma warning(disable:4503)

using namespace std;

string zeroEvent;
string zeroVariable;

int formatGroupReplace(std::string& curline, std::string oriline, int point, std::string filename, std::string format, shared_ptr<master> subFunctionIDs, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, int linecount, int groupMulti, int optionMulti, int animMulti, std::string multiOption, bool innerError);

groupTemplate::groupTemplate(vecstr grouptemplateformat)
{
	templatelines = grouptemplateformat;
}

vecstr groupTemplate::getFunctionLines(string behaviorFile, string formatname, vector<int>& stateID, shared_ptr<master> newSubFunctionIDs,
	vector<vector<shared_ptr<animationInfo>>> newGroupAnimInfo, int& nFunctionID, ImportContainer& import, id newEventID, id newVariableID,
	string masterFormat, int groupCount)
{
	if (groupCount == -1)
	{
		filename = masterFormat + "_master";
	}
	else
	{
		filename = masterFormat + "_group";
	}

	format = formatname;
	unordered_map<string, string> IDExist;
	vecstr tempstore;
	vecstr functionline;
	vecstr templateID;
	string multiOption;
	bool negative = false;
	bool newOpen = false;
	bool multi = false;
	bool norElement = false;
	bool open = false;
	bool skip = false;
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	__int64 openRange = 0;
	vector<int> fixedStateID;
	int curGroup = groupCount - 1;
	int condition = 0;
	int counter = 0;
	int order = -2;
	int groupOrder = -2;
	strID = to_string(nFunctionID);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	newImport = const_cast<ImportContainer*>(&import);
	nextStateID = stateID;
	subFunctionIDs = newSubFunctionIDs;
	groupAnimInfo = newGroupAnimInfo;
	eventid = newEventID;
	variableid = newVariableID;
	unordered_map<int, bool> IsConditionOpened;
	size_t elementLine = -1;
	vector<vector<unordered_map<string, bool>>> groupOptionPicked;

	for (auto& groupInfo : groupAnimInfo)
	{
		vector<unordered_map<string, bool>> curGroupInfo;

		for (auto& animInfo : groupInfo)
		{
			curGroupInfo.push_back(animInfo->optionPicked);
		}

		if (curGroupInfo.size() > 0)
		{
			groupOptionPicked.push_back(curGroupInfo);
		}
	}

	while (strID.length() < 4)
	{
		strID = "0" + strID;
	}

	for (auto ID : stateID)
	{
		fixedStateID.push_back(ID);
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

			if (!freeze && !multi && ((newOpen && !skip) || !newOpen))
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = templatelines[i].find("<!-- CONDITION START ^") + 22;
						string conditionLine = templatelines[i].substr(optionPosition, templatelines[i].find("^ -->", optionPosition) - optionPosition);
						animationutility utility;
						utility.hasGroup = true;
						utility.groupMulti = curGroup;

						if (newCondition(conditionLine, filename, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat, utility))
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
							functionline.shrink_to_fit();
							return functionline;
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
				ErrorMessage(1119, format, filename, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if (!freeze && !multi && ((newOpen && !skip) || !newOpen))
			{
				if (!IsConditionOpened[condition])
				{
					if (isPassed(condition, IsConditionOpened))
					{
						size_t optionPosition = templatelines[i].find("<!-- CONDITION ^") + 16;
						string option = templatelines[i].substr(optionPosition, templatelines[i].find("^", optionPosition) - optionPosition);
						animationutility utility;
						utility.hasGroup = true;
						utility.groupMulti = curGroup;

						if (newCondition(option, filename, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat, utility))
						{
							skip = false;
							IsConditionOpened[condition] = true;
							uniqueskip = true;
						}
						else
						{
							skip = true;
						}

						if (error)
						{
							functionline.shrink_to_fit();
							return functionline;
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
				ErrorMessage(1119, format, filename, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if (!freeze && !multi && ((newOpen && !skip) || !newOpen))
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
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "^ -->", 0) != NOT_FOUND || templatelines[i].find("<!-- NEW ^" + masterFormat + "_group^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, filename, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			ErrorMessage(1164, format, filename, i + 1);
			functionline.shrink_to_fit();
			return functionline;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, filename, i + 1);
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
						ErrorMessage(1115, format, filename, i + 1);
						functionline.shrink_to_fit();
						return functionline;
					}
				}
				else
				{
					ErrorMessage(1159, format, filename, i + 1);
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
				ErrorMessage(1116, format, filename, i + 1);
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
						groupOrder = -1;
						multiOption = masterFormat + "_group";
					}
					else
					{
						ErrorMessage(1115, format, filename, i + 1);
						functionline.shrink_to_fit();
						return functionline;
					}
				}
				else
				{
					ErrorMessage(1160, format, filename, i + 1);
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
				ErrorMessage(1116, format, filename, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			ErrorMessage(1163, format, filename, i + 1);
			functionline.shrink_to_fit();
			return functionline;
		}
		else if (templatelines[i].find("<!-- NEW ^", 0) != NOT_FOUND && templatelines[i].find("^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, filename, i + 1);
				functionline.shrink_to_fit();
				return functionline;
			}

			if (IsConditionOpened[condition])
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

					vecstr optionInfo = GetOptionInfo(curOption, masterFormat, filename, i + 1, groupAnimInfo, false, true, groupCount);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}

					if (optionInfo[3].find("AnimObject") != NOT_FOUND)
					{
						ErrorMessage(1129, format, filename, i + 1);
						skip = true;
					}

					if (!groupAnimInfo[stoi(optionInfo[1])][stoi(optionInfo[2])]->optionPicked[curOption])
					{
						// clear group number
						curOption = boost::regex_replace(string(optionInfo[2]), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

						if (groupAnimInfo[stoi(optionInfo[1])][stoi(optionInfo[2])]->optionPicked[curOption])
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
					ErrorMessage(1116, format, filename, i + 1);
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
				ErrorMessage(1116, format, filename, i + 1);
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

					vecstr optionInfo = GetOptionInfo(curOption, masterFormat, filename, i + 1, groupAnimInfo, true, true, curGroup);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}

					if (!groupAnimInfo[stoi(optionInfo[1])][stoi(optionInfo[2])]->optionPicked[optionInfo[3]])
					{
						// Check if current condition accepts other options that are linked
						if (isNot)
						{
							tempstore.reserve(templatelines.size() / 5);
							open = true;
							multi = true;
							multiOption = optionInfo[3];

							if (optionInfo[2].length() == 0)
							{
								order = -1;
							}
							else
							{
								order = stoi(optionInfo[2]);
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
							tempstore.reserve(templatelines.size() / 5);
							open = true;
							multi = true;
							multiOption = optionInfo[3];

							if (optionInfo[2].length() == 0)
							{
								order = -1;
							}
							else
							{
								order = stoi(optionInfo[2]);
							}
						}
					}
				}
				else
				{
					ErrorMessage(1115, format, filename, i + 1);
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
				ErrorMessage(1171, format, filename, i + 1);
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
							if (groupCount != -1 && subFunctionIDs->grouplist[curGroup]->functionIDs.find(oldID) != subFunctionIDs->grouplist[curGroup]->functionIDs.end())
							{
								ID = subFunctionIDs->grouplist[curGroup]->functionIDs[oldID];
								subFunctionIDs->grouplist[curGroup]->functionIDs.erase(oldID);
								IDExist[oldID] = ID;
							}
							else
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
							}

							string templine = templatelines[i];
							templine.replace(templine.find("MID$", MIDposition), 3, format);
							templine = templine.substr(templine.find(format + "$", MIDposition), format.length() + oldID.length() - 3);
							templatelines[i].replace(templatelines[i].find("MID$"), oldID.length(), ID);

							if (groupCount == -1)
							{
								// master
								subFunctionIDs->functionIDs[templine] = ID;
							}
							else
							{
								// group
								subFunctionIDs->grouplist[curGroup]->functionIDs[templine] = ID;
							}
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

						if (intID >= int(fixedStateID.size()) || intID < 0)
						{
							ErrorMessage(1168, format, filename, i + 1, templine.substr(0, templine.find(")") + 1));
							functionline.shrink_to_fit();
							return functionline;
						}
					}

					if (templatelines[i].find("$(S" + ID + "+") != NOT_FOUND)
					{
						stateReplacer(templatelines[i], filename, ID, fixedStateID[intID], i + 1, curGroup);
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
						animationutility utility;
						utility.hasGroup = true;
						utility.groupMulti = curGroup;
						multiChoice(templatelines[i], filename, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat, utility);

						if (error)
						{
							functionline.shrink_to_fit();
							return functionline;
						}
					}

					if (templatelines[i].find("%") != NOT_FOUND)
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
						ErrorMessage(1136, format, filename, i + 1);
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

					if (templatelines[i].find("\t\t\t#") != NOT_FOUND)
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
								if (subFunctionIDs->grouplist[curGroup]->singlelist[0]->format[curID].length() == 0)
								{
									ErrorMessage(2017, format, filename, i + 1);
									functionline.shrink_to_fit();
									return functionline;
								}

								templatelines[i].replace(nextpos, curID.length(), subFunctionIDs->grouplist[curGroup]->singlelist[0]->format[curID]);
							}
							else
							{
								ErrorMessage(2018, format, filename, i + 1, curID);
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
								for (unsigned int k = 0; k < subFunctionIDs->grouplist.size(); ++k) // number of variation
								{
									if (subFunctionIDs->grouplist[k]->functionIDs[curID].length() == 0)
									{
										ErrorMessage(2017, format, filename, i + 1);
										functionline.shrink_to_fit();
										return functionline;
									}

									templatelines[i].replace(nextpos, curID.length(), subFunctionIDs->grouplist[k]->functionIDs[curID]);
								}
							}
							else
							{
								ErrorMessage(2018, format, filename, i + 1, curID);
								functionline.shrink_to_fit();
								return functionline;
							}
						}
					}
				}

				if (templatelines[i].find("$") != NOT_FOUND && groupCount != -1)
				{
					processing(templatelines[i], filename, masterFormat, i + 1, eventid, variableid, curGroup);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}
				}

				size_t pos = templatelines[i].find("(");

				if (pos != NOT_FOUND && templatelines[i].find(")", pos) != NOT_FOUND)
				{
					string templine = templatelines[i].substr(pos);
					int nextpos = openEndBracket(templine, '(', ')', format, filename, i + 1) + 1;

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}

					templine = templine.substr(0, nextpos);
					string oldline = templine;
					calculate(templine, format, filename, i + 1);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}

					if (oldline != templine)
					{
						templatelines[i].replace(pos, oldline.length(), templine);
					}
				}

				if (templatelines[i].find("<hkparam name=\"animationName\">") != NOT_FOUND)
				{
					pos = templatelines[i].find("animationName\">") + 15;
					string animPath = templatelines[i].substr(pos, templatelines[i].find("</hkparam>", pos) - pos);
					boost::algorithm::to_lower(animPath);
					addUsedAnim(behaviorFile, animPath);
				}
				else if (templatelines[i].find("<hkparam name=\"localTime\">-") != NOT_FOUND)
				{
					negative = true;
				}
				else if (negative && templatelines[i].find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
				{
					if (templatelines[i].find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
					{
						size_t pos = templatelines[i].find("<hkparam name=\"relativeToEndOfClip\">") + 36;
						templatelines[i].replace(pos, templatelines[i].find("</hkparam>", pos) - pos, "true");
					}

					negative = false;
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
					int tempOrder;
					int groupSize;

					if (groupOrder == -2)
					{
						groupOrder = curGroup;
						groupSize = groupCount;
					}
					else if (groupOrder == -1)
					{
						groupOrder = 0;
						groupSize = int(subFunctionIDs->grouplist.size());
					}
					else
					{
						groupSize = groupOrder + 1;
					}

					for (int groupMulti = groupOrder; groupMulti < groupSize; ++groupMulti)
					{
						if (order == -2)
						{
							tempOrder = 0;
							size = 1;
						}
						else if (order == -1)
						{
							tempOrder = 0;
							size = int(subFunctionIDs->grouplist[groupMulti]->singlelist.size());
						}
						else
						{
							tempOrder = order;
							size = order + 1;
						}

						for (int animMulti = tempOrder; animMulti < size; ++animMulti) // each animation in a group
						{
							int optionMulti = 0;

							while (true)
							{
								if (groupCount == -1 && multiOption == masterFormat + "_group")
								{

								}
								else if (optionMulti >= groupAnimInfo[curGroup][animMulti]->optionPickedCount[multiOption])
								{
									break;
								}

								bool skip2 = false;
								bool freeze2 = false;

								for (unsigned int l = 0; l < tempstore.size(); ++l) // part lines need to add
								{
									string curLine = tempstore[l];
									bool uniqueskip2 = false;
									size_t linecount = i + 1 + l - int(tempstore.size());

									if (curLine.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
									{
										condition++;

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													size_t optionPosition = curLine.find("<!-- CONDITION START ^") + 22;
													string conditionLine = curLine.substr(optionPosition, curLine.find("^ -->", optionPosition) - optionPosition);
													animationutility utility;
													utility.hasGroup = true;
													utility.groupMulti = groupMulti;
													utility.animMulti = animMulti;
													utility.optionMulti = optionMulti;
													utility.multiOption = multiOption;

													if (newCondition(conditionLine, filename, groupOptionPicked, groupAnimInfo, linecount, format, masterFormat, utility))
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
														functionline.shrink_to_fit();
														return functionline;
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
									else if (curLine.find("<!-- CONDITION ^", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1119, format, filename, linecount);
											functionline.shrink_to_fit();
											return functionline;
										}

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													size_t optionPosition = curLine.find("<!-- CONDITION ^") + 16;
													string option = curLine.substr(optionPosition, curLine.find("^", optionPosition) - optionPosition);
													animationutility utility;
													utility.hasGroup = true;
													utility.groupMulti = groupMulti;
													utility.animMulti = animMulti;
													utility.optionMulti = optionMulti;
													utility.multiOption = multiOption;

													if (newCondition(option, filename, groupOptionPicked, groupAnimInfo, linecount, format, masterFormat, utility))
													{
														skip2 = false;
														IsConditionOpened[condition] = true;
														uniqueskip2 = true;
													}
													else
													{
														skip2 = true;
													}

													if (error)
													{
														functionline.shrink_to_fit();
														return functionline;
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
									else if (curLine.find("<!-- CONDITION -->", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1119, format, filename, linecount);
											functionline.shrink_to_fit();
											return functionline;
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
													size_t conditionPosition = curLine.find("<!-- CONDITION") + 14;
													string replacement1 = curLine.substr(0, conditionPosition + 1);
													string replacement2 = curLine.substr(conditionPosition);
													functionline.push_back(replacement1 + "START" + replacement2);
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
									else if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
									{
										uniqueskip2 = true;
									}

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

										if (to_string(animMulti) == option && isNot)
										{
											break;
										}
										else if (to_string(animMulti) != option && !isNot)
										{
											break;
										}

										uniqueskip2 = true;
									}

									if (!skip2 && !freeze2 && !uniqueskip2)
									{
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
														if (groupCount != -1 && subFunctionIDs->grouplist[curGroup]->functionIDs.find(oldID) != subFunctionIDs->grouplist[curGroup]->functionIDs.end())
														{
															ID = subFunctionIDs->grouplist[curGroup]->functionIDs[oldID];
															subFunctionIDs->grouplist[curGroup]->functionIDs.erase(oldID);
															IDExist[oldID] = ID;
														}
														else
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
														}

														string templine = curLine;
														templine.replace(templine.find("MID$", MIDposition), 3, format);
														templine = templine.substr(templine.find(format + "$", MIDposition), format.length() + oldID.length() - 3);
														curLine.replace(curLine.find("MID$"), oldID.length(), ID);

														if (groupCount == -1)
														{
															// master
															subFunctionIDs->functionIDs[templine] = ID;
														}
														else
														{
															// group
															subFunctionIDs->grouplist[curGroup]->functionIDs[templine] = ID;
														}
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

													if (intID >= int(fixedStateID.size()) || intID < 0)
													{
														ErrorMessage(1127, format, filename, linecount);
														functionline.shrink_to_fit();
														return functionline;
													}
												}

												if (curLine.find("$(S" + ID + "+") != NOT_FOUND)
												{
													stateReplacer(curLine, filename, ID, fixedStateID[intID], linecount, curGroup);
												}
											}

											// multi choice selection
											if (curLine.find("$MC$", 0) != NOT_FOUND)
											{
												animationutility utility;
												utility.hasGroup = true;
												utility.groupMulti = groupMulti;
												utility.animMulti = animMulti;
												utility.optionMulti = optionMulti;
												utility.multiOption = multiOption;

												multiChoice(curLine, filename, groupOptionPicked, groupAnimInfo, linecount, format, masterFormat, utility);

												if (error)
												{
													functionline.shrink_to_fit();
													return functionline;
												}
											}
										}

										if (curLine.find("%") != NOT_FOUND)
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
														if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[curID].length() == 0)
														{
															ErrorMessage(2017, format, filename, linecount);
															functionline.shrink_to_fit();
															return functionline;
														}

														curLine.replace(nextpos, curID.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[curID]);
													}
													else
													{
														ErrorMessage(2018, format, filename, linecount, curID);
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
														if (subFunctionIDs->grouplist[groupMulti]->functionIDs[curID].length() == 0)
														{
															ErrorMessage(2017, format, filename, linecount);
															functionline.shrink_to_fit();
															return functionline;
														}

														curLine.replace(nextpos, curID.length(), subFunctionIDs->grouplist[groupMulti]->functionIDs[curID]);
													}
													else
													{
														ErrorMessage(2018, format, filename, linecount, curID);
														functionline.shrink_to_fit();
														return functionline;
													}
												}
											}
										}

										if (curLine.find("$") != NOT_FOUND)
										{
											processing(curLine, filename, masterFormat, linecount, eventid, variableid, curGroup, optionMulti, animMulti, multiOption);

											if (error)
											{
												functionline.shrink_to_fit();
												return functionline;
											}
										}

										size_t pos = curLine.find("(");

										if (pos != NOT_FOUND && curLine.find(")", pos) != NOT_FOUND)
										{
											string templine = curLine.substr(pos);
											int nextpos = openEndBracket(templine, '(', ')', format, filename, linecount) + 1;

											if (error)
											{
												functionline.shrink_to_fit();
												return functionline;
											}

											templine = templine.substr(0, nextpos);
											string oldline = templine;
											calculate(templine, format, filename, linecount);

											if (error)
											{
												functionline.shrink_to_fit();
												return functionline;
											}

											if (oldline != templine)
											{
												curLine.replace(pos, oldline.length(), templine);
											}
										}

										if (curLine.find("<hkparam name=\"animationName\">") != NOT_FOUND)
										{
											pos = curLine.find("animationName\">") + 15;
											string animPath = curLine.substr(pos, curLine.find("</hkparam>", pos) - pos);
											boost::algorithm::to_lower(animPath);
											addUsedAnim(behaviorFile, animPath);
										}
										else if (curLine.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
										{
											negative = true;
										}
										else if (negative && curLine.find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
										{
											if (curLine.find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
											{
												size_t pos = curLine.find("<hkparam name=\"relativeToEndOfClip\">") + 36;
												curLine.replace(pos, curLine.find("</hkparam>", pos) - pos, "true");
											}

											negative = false;
										}

										functionline.push_back(curLine);
									}

									if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1118, format, filename, linecount);
											functionline.shrink_to_fit();
											return functionline;
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

								if (groupCount == -1 && multiOption == masterFormat + "_group")
								{
									break;
								}

								++optionMulti;
							}
						}

						order = -2;
					}
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
				ErrorMessage(1118, format, filename, i + 1);
				functionline.shrink_to_fit();
				return functionline;
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

		if (error)
		{
			functionline.shrink_to_fit();
			return functionline;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, filename, templatelines.size());
		functionline.shrink_to_fit();
		return functionline;
	}

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			ErrorMessage(1120, format, filename);
			functionline.shrink_to_fit();
			return functionline;
			break;
		}
	}

	if (functionline.size() != 0)
	{
		if (!functionline.back().empty())
		{
			functionline.push_back("");
		}
		else if (functionline.size() > 1)
		{
			while (functionline.back().empty())
			{
				if (functionline[functionline.size() - 2].empty())
				{
					functionline.pop_back();
				}
				else
				{
					break;
				}
			}
		}
	}

	functionline.shrink_to_fit();
	return functionline;
}

vecstr ExistingFunction::groupExistingFunctionProcess(int curFunctionID, vecstr existingFunctionLines, shared_ptr<master> newSubFunctionIDs,
	vector<vector<shared_ptr<animationInfo>>> newGroupAnimInfo, string curformat, ImportContainer& import, id newEventID, id newVariableID, int& nFunctionID,
	bool hasMaster, bool hasGroup, vecstr templateGroup, bool ignoreGroup)
{
	vecstr newFunctionLines;
	vecstr tempstore;
	vecstr empty;

	newImport = const_cast<ImportContainer*>(&import);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	format = curformat;
	strID = to_string(nFunctionID);
	subFunctionIDs = newSubFunctionIDs;
	groupAnimInfo = newGroupAnimInfo;
	eventid = newEventID;
	variableid = newVariableID;

	string multiOption;
	bool formatOpen = false;
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
	int groupOrder = -2;
	int elementLine = 0;
	unordered_map<int, bool> IsConditionOpened;
	unordered_map<string, bool> otherAnimType;
	vector<vector<unordered_map<string, bool>>> groupOptionPicked;

	string IDFileName = to_string(curFunctionID);

	while (IDFileName.length() < 4)
	{
		IDFileName = "0" + IDFileName;
	}

	IDFileName = "#" + IDFileName;

	for (auto& groupInfo : groupAnimInfo)
	{
		vector<unordered_map<string, bool>> curGroupInfo;

		for (auto& animInfo : groupInfo)
		{
			curGroupInfo.push_back(animInfo->optionPicked);
		}

		if (curGroupInfo.size() > 0)
		{
			groupOptionPicked.push_back(curGroupInfo);
		}
	}

	while (strID.length() < 4)
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

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			condition++;

			if (!multi && !skipTemplate && ((newOpen && !skip) || !newOpen))
			{
				if (!freeze)
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION START ^") + 22;
							string conditionLine = line.substr(optionPosition, line.find("^ -->", optionPosition) - optionPosition);
							animationutility utility;
							utility.hasGroup = hasGroup;

							if (newCondition(conditionLine, IDFileName, groupOptionPicked, groupAnimInfo, i + 1, format, format, utility))
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
								return empty;
							}
						}
					}
					else
					{
						skip = true;
					}
				}

				uniqueskip = true;
			}
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, IDFileName, i + 1);
				return empty;
			}

			if (!multi && !skipTemplate && ((newOpen && !skip) || !newOpen))
			{
				if (!freeze)
				{
					if (!IsConditionOpened[condition])
					{
						if (isPassed(condition, IsConditionOpened))
						{
							size_t optionPosition = line.find("<!-- CONDITION ^") + 16;
							string option = line.substr(optionPosition, line.find("^", optionPosition) - optionPosition);
							animationutility utility;
							utility.hasGroup = hasGroup;

							if (newCondition(option, "#" + IDFileName, groupOptionPicked, groupAnimInfo, i + 1, format, format, utility))
							{
								skip = false;
								IsConditionOpened[condition] = true;
								uniqueskip = true;
							}
							else
							{
								skip = true;
							}

							if (error)
							{
								return empty;
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
		}
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1119, format, IDFileName, i + 1);
				return empty;
			}

			if (!multi && !skipTemplate && ((newOpen && !skip) || !newOpen))
			{
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
		}
		else if (line.find("<!-- NEW ^" + format + "^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
				return empty;
			}

			if (IsConditionOpened[condition])
			{
				if (!hasGroup)
				{
					if (!open)
					{
						if (groupAnimInfo.size() > 0)
						{
							if (ignoreGroup)
							{
								groupOrder = -1;
							}
							else
							{
								order = -1;
							}

							open = true;
							multi = true;
							multiOption = format;
							formatOpen = true;
						}
						else
						{
							skip = true;
						}
					}
					else
					{
						ErrorMessage(1115, format, IDFileName, i + 1);
						return empty;
					}
				}
				else
				{
					ErrorMessage(1161, format, IDFileName, i + 1);
					return empty;
				}
			}

			newOpen = true;
			uniqueskip = true;
			formatOpen = true;
		}
		else if (line.find("<!-- NEW ^" + format + "_group^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
				return empty;
			}

			if (IsConditionOpened[condition])
			{
				if (hasGroup && !hasMaster)
				{
					if (!open)
					{
						if (groupAnimInfo.size() > 0)
						{
							open = true;
							multi = true;
							groupOrder = -1;
							multiOption = format + "_group";
							formatOpen = true;
						}
						else
						{
							skip = true;
						}
					}
					else
					{
						ErrorMessage(1115, format, IDFileName, i + 1);
						return empty;
					}
				}
				else
				{
					ErrorMessage(1167, format, IDFileName, i + 1);
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
				ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
				return empty;
			}

			if (IsConditionOpened[condition])
			{
				if (hasMaster)
				{
					if (!open)
					{
						if (groupAnimInfo.size() > 0)
						{
							open = true;
							multi = true;
							groupOrder = -1;
							multiOption = format + "_master";
							formatOpen = true;
						}
						else
						{
							skip = true;
						}
					}
					else
					{
						ErrorMessage(1115, format, IDFileName, i + 1);
						return empty;
					}
				}
				else
				{
					ErrorMessage(1162, format, IDFileName, i + 1);
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
				ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
				return empty;
			}

			ErrorMessage(1164, format, IDFileName, i + 1);
			return empty;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
				return empty;
			}

			newOpen = true;
			size_t pos = line.find("<!-- NEW ^") + 10;
			string checker = line.substr(pos, line.find("^", pos) - pos);

			if (!otherAnimType[checker])
			{
				uniqueskip = true;

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
							vecstr formatInfo = GetOptionInfo(curOption, format, IDFileName, i + 1, groupAnimInfo, false, true);

							if (error)
							{
								return empty;
							}

							if (formatInfo[2].find("AnimObject") != NOT_FOUND)
							{
								ErrorMessage(1129, format, IDFileName, i + 1);
								return empty;
							}

							if (groupOptionPicked[stoi(formatInfo[1])][stoi(formatInfo[2])][formatInfo[3]])
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
						ErrorMessage(1116, format, IDFileName, i + 1);
						return empty;
					}
				}

			}
			else
			{
				skipTemplate = true;
			}
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
		{
			if (newOpen)
			{
				ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
				return empty;
			}

			newOpen = true;
			size_t pos = line.find("<!-- NEW ^") + 10;
			string checker = line.substr(pos, line.find("^", pos) - pos);

			if (!otherAnimType[checker])
			{
				uniqueskip = true;

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
							vecstr formatInfo = GetOptionInfo(curOption, format, IDFileName, i + 1, groupAnimInfo, true, true);

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
									groupOrder = -1;
								}
								else
								{
									groupOrder = stoi(formatInfo[1]);
								}

								if (formatInfo[2].length() == 0)
								{
									order = -1;
								}
								else
								{
									order = stoi(formatInfo[2]);
								}
							}
						}
					}
					else
					{
						ErrorMessage(1115, format, IDFileName, i + 1);
						return empty;
					}
				}
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
				ErrorMessage(1171, format, IDFileName, i + 1);
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
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND && !multi && !skipTemplate && ((newOpen && !skip) || !newOpen))
		{
			uniqueskip = true;
		}

		if (!uniqueskip && !skip && !freeze)
		{
			while (true)
			{
				if (skipTemplate)
				{
					newFunctionLines.push_back(line);
					break;
				}

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
					animationutility utility;
					utility.hasGroup = hasGroup;

					multiChoice(line, IDFileName, groupOptionPicked, groupAnimInfo, i + 1, format, format, utility);

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
						ErrorMessage(2019, format, IDFileName, i + 1);
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
					processing(line, IDFileName, curFunctionID, i + 1, eventid, variableid, 0);
					
					if (error)
					{
						return empty;
					}
				}

				size_t pos = line.find("(");

				if (pos != NOT_FOUND && line.find(")", pos) != NOT_FOUND)
				{
					string templine = line.substr(pos);
					int nextpos = openEndBracket(templine, '(', ')', format, IDFileName, i + 1) + 1;

					if (error)
					{
						return empty;
					}

					templine = templine.substr(0, nextpos);
					string oldline = templine;
					calculate(templine, format, IDFileName, i + 1);

					if (error)
					{
						return empty;
					}

					if (oldline != templine)
					{
						line.replace(pos, oldline.length(), templine);
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
					int tempOrder;
					int groupSize;

					if (groupOrder == -2)
					{
						groupOrder = 0;
						groupSize = 1;
					}
					else if (groupOrder == -1)
					{
						groupOrder = 0;
						groupSize = int(groupAnimInfo.size());
					}
					else
					{
						groupSize = groupOrder + 1;
					}

					for (int groupMulti = groupOrder; groupMulti < groupSize; ++groupMulti)
					{
						if (order == -2)
						{
							tempOrder = 0;
							size = 1;
						}
						else if (order == -1)
						{
							tempOrder = 0;
							size = int(subFunctionIDs->grouplist[groupMulti]->singlelist.size());
						}
						else
						{
							tempOrder = order;
							size = order + 1;
						}

						for (int animMulti = tempOrder; animMulti < size; ++animMulti) // each animation in a group
						{
							int repeatCount;

							if (formatOpen)
							{
								repeatCount = 1;
							}
							else
							{
								repeatCount = groupAnimInfo[groupMulti][animMulti]->optionPickedCount[multiOption];
							}

							for (int optionMulti = 0; optionMulti < repeatCount; ++optionMulti)		// option with different add-ons
							{
								bool skip2 = false;
								bool freeze2 = false;

								for (unsigned int l = 0; l < tempstore.size(); ++l) // part lines need to add
								{
									string curLine = tempstore[l];
									size_t linecount = i + 1 + l - int(tempstore.size());
									bool uniqueskip2 = false;

									if (curLine.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
									{
										condition++;

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													size_t optionPosition = curLine.find("<!-- CONDITION START ^") + 22;
													string conditionLine = curLine.substr(optionPosition, curLine.find("^ -->", optionPosition) - optionPosition);
													animationutility utility;
													utility.hasGroup = hasGroup;
													utility.groupMulti = groupMulti;
													utility.animMulti = animMulti;
													utility.optionMulti = optionMulti;
													utility.multiOption = multiOption;

													if (newCondition(conditionLine, IDFileName, groupOptionPicked, groupAnimInfo, linecount, format, format, utility))
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
														return empty;
													}
												}
											}
											else
											{
												skip2 = true;
											}
										}

										uniqueskip2 = true;
									}
									else if (curLine.find("<!-- CONDITION ^", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1119, format, IDFileName, linecount);
											return empty;
										}

										if (!freeze2)
										{
											if (!IsConditionOpened[condition])
											{
												if (isPassed(condition, IsConditionOpened))
												{
													size_t optionPosition = curLine.find("<!-- CONDITION ^") + 16;
													string option = curLine.substr(optionPosition, curLine.find("^", optionPosition) - optionPosition);
													animationutility utility;
													utility.hasGroup = hasGroup;
													utility.groupMulti = groupMulti;
													utility.animMulti = animMulti;
													utility.optionMulti = optionMulti;
													utility.multiOption = multiOption;

													if (newCondition(option, "#" + IDFileName, groupOptionPicked, groupAnimInfo, linecount, format, format, utility))
													{
														skip2 = false;
														IsConditionOpened[condition] = true;
														uniqueskip2 = true;
														string replacement1 = curLine.substr(0, optionPosition - 1);
														string replacement2 = curLine.substr(optionPosition - 2);
														newFunctionLines.push_back(replacement1 + "START" + replacement2);
													}
													else
													{
														skip2 = true;
													}

													if (error)
													{
														return empty;
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
									else if (curLine.find("<!-- CONDITION -->", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1119, format, IDFileName, linecount);
											return empty;
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
									else if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
									{
										uniqueskip2 = true;
									}

									if (!skip2 && !freeze2 && !uniqueskip2)
									{
										if (curLine.find("<hkparam name=\"") != NOT_FOUND && curLine.find("numelements=\"") != NOT_FOUND && curLine.find("</hkparam>") == NOT_FOUND && curLine.find("<!-- COMPUTE -->", 0) != NOT_FOUND)
										{
											ErrorMessage(1140, format, IDFileName, linecount);
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
														if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[curID].length() == 0)
														{
															ErrorMessage(2011, format, IDFileName, linecount, curID);
															return empty;
														}

														curLine.replace(nextpos, curID.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[curID]);
													}
													else
													{
														ErrorMessage(2012, format, IDFileName, linecount, curID);
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
														if (subFunctionIDs->grouplist[groupMulti]->functionIDs[curID].length() == 0)
														{
															ErrorMessage(2011, format, IDFileName, linecount, curID);
															return empty;
														}

														curLine.replace(nextpos, curID.length(), subFunctionIDs->grouplist[groupMulti]->functionIDs[curID]);
													}
													else
													{
														ErrorMessage(2012, format, IDFileName, linecount, curID);
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
														if (subFunctionIDs->functionIDs[curID].length() == 0)
														{
															ErrorMessage(2011, format, IDFileName, linecount, curID);
															return empty;
														}

														curLine.replace(nextpos, curID.length(), subFunctionIDs->functionIDs[curID]);
													}
													else
													{
														ErrorMessage(2012, format, IDFileName, linecount, curID);
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
											animationutility utility;
											utility.hasGroup = hasGroup;
											utility.groupMulti = groupMulti;
											utility.animMulti = animMulti;
											utility.optionMulti = optionMulti;
											utility.multiOption = multiOption;

											multiChoice(curLine, IDFileName, groupOptionPicked, groupAnimInfo, linecount, format, format, utility);

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

										if (curLine.find("$") != NOT_FOUND)
										{
											processing(curLine, IDFileName, curFunctionID, linecount, eventid, variableid, groupMulti, optionMulti, animMulti, multiOption);

											if (error)
											{
												return empty;
											}
										}

										size_t pos = curLine.find("(");

										if (pos != NOT_FOUND && curLine.find(")", pos) != NOT_FOUND)
										{
											string templine = curLine.substr(pos);
											int nextpos = openEndBracket(templine, '(', ')', format, IDFileName, linecount) + 1;

											if (error)
											{
												return empty;
											}

											templine = templine.substr(0, nextpos);
											string oldline = templine;
											calculate(templine, format, IDFileName, linecount);

											if (error)
											{
												return empty;
											}

											if (oldline != templine)
											{
												curLine.replace(pos, oldline.length(), templine);
											}
										}

										if (curLine.find("<!-- NEW ^" + format, 0) == NOT_FOUND && curLine.find("<!-- CLOSE -->", 0) == NOT_FOUND)
										{
											newFunctionLines.push_back(curLine);
										}
									}

									if (curLine.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
									{
										if (condition == 0)
										{
											ErrorMessage(1118, format, IDFileName, linecount);
											return empty;
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
					}
				}

				tempstore.clear();
				tempstore.reserve(existingFunctionLines.size() / 10);
			}

			open = false;
			multi = false;
			formatOpen = false;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			if (condition == 0)
			{
				ErrorMessage(1118, format, IDFileName, i + 1);
				return empty;
			}

			if (!multi && !skipTemplate && ((newOpen && !skip) || !newOpen))
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

		if (error)
		{
			return empty;
		}
	}

	IsConditionOpened[0] = false;

	if (newOpen)
	{
		ErrorMessage(1116, format, IDFileName, existingFunctionLines.size());
		return empty;
	}

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			ErrorMessage(1120, format, IDFileName);
			return empty;
		}
	}

	if (newFunctionLines.size() != 0)
	{
		if (!newFunctionLines.back().empty())
		{
			newFunctionLines.push_back("");
		}
		else if (newFunctionLines.size() > 1)
		{
			while (newFunctionLines.back().empty())
			{
				if (newFunctionLines[newFunctionLines.size() - 2].empty())
				{
					newFunctionLines.pop_back();
				}
				else
				{
					break;
				}
			}
		}
	}

	newFunctionLines.shrink_to_fit();	
	return newFunctionLines;
}

void groupTemplate::stateReplacer(string& line, string filename, string statenum, int stateID, int linecount, int groupMulti)
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
				calculate(state, format, filename, linecount);

				if (error)
				{
					return;
				}

				if (groupMulti > -1)
				{
					// group
					subFunctionIDs->grouplist[groupMulti]->functionIDs["(S" + statenum + "+" + number + ")"] = state;
				}
				else
				{
					// master
					subFunctionIDs->functionIDs["(S" + statenum + "+" + number + ")"] = state;
				}

				line.replace(stateposition, stateLength, state);
			}
			else
			{
				ErrorMessage(1137, format, filename, linecount, state);
				return;
			}
		}
	}
}

void groupTemplate::processing(string& line, string filename, string masterFormat, int linecount, id eventid, id variableid, int groupMulti, int optionMulti,
	int animMulti, string multiOption)
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

							calculate(equation, format, filename, linecount);

							if (error)
							{
								return;
							}

							if (stoi(equation) > int(groupAnimInfo.size() - 1) || stoi(equation) < 0)
							{
								ErrorMessage(1155, groupAnimInfo.size() - 1, format, filename, linecount, change);
								return;
							}

							change.replace(nextpos, equationLength, equation);
							isChange = true;
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			size_t pos = change.find(masterFormat + "[", 0);
			bool innerError = false;

			if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
			{
				formatGroupReplace(change, change, 0, filename, masterFormat, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);
				isChange = true;

				if (innerError)
				{
					ErrorMessage(2014, format, filename, linecount);
					return;
				}
				else if (error)
				{
					return;
				}
			}

			pos = change.find(masterFormat + "_group[", 0);

			if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
			{
				formatGroupReplace(change, change, 0, filename, masterFormat + "_group", subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);
				isChange = true;

				if (innerError)
				{
					ErrorMessage(2014, format, filename, linecount);
					return;
				}
				else if (error)
				{
					return;
				}
			}

			if (multiOption == masterFormat)
			{
				string tempchange = change;
				pos = change.find("[");

				if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
				{
					formatGroupReplace(change, change, 0, filename, masterFormat, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);

					if (innerError)
					{
						ErrorMessage(2014, format, filename, linecount);
						return;
					}
					else if (error)
					{
						return;
					}
				}
				else
				{
					if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change].length() > 0)
					{
						change = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change];
					}
				}

				if (change != tempchange)
				{
					isChange = true;
				}
			}

			size_t position = change.find("eventID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				eventIDReplacer(change, format, filename, eventid, zeroEvent, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("variableID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				variableIDReplacer(change, format, filename, variableid, zeroVariable, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("crc32[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				CRC32Replacer(change, format, filename, linecount);
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
						ErrorMessage(2013, format, filename, linecount, importer);
						return;
					}

					size_t pos = importer.find("[") + 1;
					string file = importer.substr(pos, importer.find("]", pos) - pos);
					string keyword;
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
							ErrorMessage(2013, format, filename, linecount, importer);
							return;
						}
						else
						{
							keyword = keyword.substr(0, keyword.length() - 4);
						}
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

void ExistingFunction::processing(string& line, string filename, int curFunctionID, int linecount, id eventid, id variableid, int groupMulti, int optionMulti,
	int animMulti, string multiOption)
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
									equation.replace(equation.find("L"), 1, to_string(groupAnimInfo[groupMulti].size() - 1));
								}
							}

							calculate(equation, format, filename, linecount);

							if (error)
							{
								return;
							}

							if (stoi(equation) > int(groupAnimInfo[groupMulti].size() - 1) || stoi(equation) < 0)
							{
								ErrorMessage(1148, format, filename, linecount, change);
								return;
							}

							change.replace(nextpos, equationLength, equation);
							isChange = true;
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			if (groupMulti > -1)
			{
				if (change.find("[FilePath]") != NOT_FOUND)
				{
					if (change.find(format + "[][FilePath]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[][FilePath]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FilePath"]);
						isChange = true;
					}

					if (change.find(format + "[F][FilePath]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[F][FilePath]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FilePath"]);
						isChange = true;
					}

					if (change.find(format + "[N][FilePath]", 0) != NOT_FOUND)
					{
						ErrorMessage(1056, format, filename, linecount, line);
						return;
					}

					if (change.find(format + "[L][FilePath]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[L][FilePath]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FilePath"]);
						isChange = true;
					}

					if (change.find(format + "[", 0) != NOT_FOUND)
					{
						string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (change.find(format + "[" + number + "][FilePath]", 0) != NOT_FOUND)
						{
							if (unsigned int(stoi(number)) >= groupAnimInfo[groupMulti].size())
							{
								change.replace(change.find(format + "[" + number + "][FilePath]"), 8 + format.length() + number.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[stoi(number)]->format["FilePath"]);
								isChange = true;
							}
							else
							{
								ErrorMessage(1148, format, filename, linecount, change);
								return;
							}
						}
					}
				}

				if (change.find("[FileName]") != NOT_FOUND)
				{
					if (change.find(format + "[][FileName]", 0) != NOT_FOUND)
					{
						if (multiOption != format || animMulti == -1)
						{
							ErrorMessage(1052, format, filename, linecount, line);
							return;
						}

						change.replace(change.find(format + "[][FileName]"), 12 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FileName"]);
					}

					if (change.find(format + "[F][FileName]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[F][FileName]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FileName"]);
						isChange = true;
					}

					if (change.find(format + "[N][FileName]", 0) != NOT_FOUND)
					{
						ErrorMessage(1056, format, filename, linecount, line);
						return;
					}

					if (change.find(format + "[L][FileName]", 0) != NOT_FOUND)
					{
						change.replace(change.find(format + "[L][FileName]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FileName"]);
						isChange = true;
					}

					if (change.find(format + "[", 0) != NOT_FOUND)
					{
						string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (change.find(format + "[" + number + "][FileName]", 0) != NOT_FOUND)
						{
							if (unsigned int(stoi(number)) >= groupAnimInfo[groupMulti].size())
							{
								change.replace(change.find(format + "[" + number + "][FileName]"), 8 + format.length() + number.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[stoi(number)]->format["FileName"]);
								isChange = true;
							}
							else
							{
								ErrorMessage(1148, format, filename, linecount, change);
								return;
							}
						}
					}
				}

				size_t pos = change.find(format + "[", 0);
				bool innerError = false;

				if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
				{
					formatGroupReplace(change, change, 0, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);
					isChange = true;

					if (innerError)
					{
						ErrorMessage(2014, format, filename, linecount);
						return;
					}
					else if (error)
					{
						return;
					}
				}

				if (change.find(format + "_group[][", 0) != NOT_FOUND && change.find("]", change.find(format + "_group[][")) != NOT_FOUND)
				{
					formatGroupReplace(change, change, 0, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);
					isChange = true;

					if (innerError)
					{
						ErrorMessage(2014, format, filename, linecount);
						return;
					}
					else if (error)
					{
						return;
					}
				}

				if (multiOption == format)
				{
					string tempchange = change;
					pos = change.find("[");

					if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
					{
						formatGroupReplace(change, change, 0, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);

						if (innerError)
						{
							ErrorMessage(2014, format, filename, linecount);
							return;
						}
						else if (error)
						{
							return;
						}
					}
					else
					{
						if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change].length() > 0)
						{
							change = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change];
						}
					}

					if (change != tempchange)
					{
						isChange = true;
					}
				}

				if (error)
				{
					return;
				}
			}
			else
			{
				for (auto it = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format.begin(); it != subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format.end(); ++it)
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
				eventIDReplacer(change, format, filename, eventid, zeroEvent, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}
			
			position = change.find("variableID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				variableIDReplacer(change, format, filename, variableid, zeroVariable, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			position = change.find("crc32[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				CRC32Replacer(change, format, filename, linecount);
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
						ErrorMessage(2013, format, filename, linecount, importer);
						return;
					}

					size_t pos = importer.find("[") + 1;
					string file = importer.substr(pos, importer.find("]", pos) - pos);
					string keyword;
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
							ErrorMessage(2013, format, filename, linecount, importer);
							return;
						}
						else
						{
							keyword = keyword.substr(0, keyword.length() - 4);
						}
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
				ErrorMessage(1096, format, filename, linecount);
				return;
			}

			if (change.find("RD") != NOT_FOUND)
			{
				ErrorMessage(1097, format, filename, linecount);
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

void multiChoice(string& line, string filename, vector<vector<unordered_map<string, bool>>> groupOptionPicked,
	vector<vector<shared_ptr<animationInfo>>> groupAnimInfo, int numline, string templatename, string masterformat, animationutility utility)
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
			ErrorMessage(2015, templatename, filename, numline);
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
				if (newCondition(tempstr, filename, groupOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
				{
					nextposition = line.find(" ", nextposition) + 1;
					string output = line.substr(nextposition, line.find(" ", nextposition) - nextposition);
					line.replace(line.find("$MC$", 0), 4, output);
					none = false;
					break;
				}

				if (error)
				{
					return;
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
				ErrorMessage(2016, templatename, filename, numline);
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

bool newCondition(string condition, string filename, vector<vector<unordered_map<string, bool>>> curOptionPicked,
	vector<vector<shared_ptr<animationInfo>>> groupAnimInfo, int numline, string templatename, string masterformat, animationutility utility)
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
			
			inHouseResult = conditionProcess(inHouse, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility);
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

			inHouseResult = conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility);

			if (!inHouseResult && newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
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

			size_t x = tempSecondCondition.find("&");
			size_t y = tempSecondCondition.find("|");

			if ((x == NOT_FOUND || x > y) && y != NOT_FOUND)
			{
				secondCondition = tempSecondCondition.substr(0, tempSecondCondition.find("|"));
				tempSecondCondition = tempSecondCondition.substr(tempSecondCondition.find("|") + 1);

				if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
				{
					if (error)
					{
						return false;
					}

					inHouseResult = newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility);
				}
				else
				{
					inHouseResult = false;
				}

				if (error)
				{
					return false;
				}

				if (!inHouseResult && newCondition(tempSecondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
				{
					inHouseResult = true;
				}
			}
			else
			{
				secondCondition = tempSecondCondition;

				if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
				{
					if (error)
					{
						return false;
					}

					inHouseResult = newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility);
				}
				else
				{
					inHouseResult = false;
				}

				if (error)
				{
					return false;
				}
			}
		}
		else
		{
			ErrorMessage(1105, templatename, filename, numline);
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

			if (inHouseResult || newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
			{
				return true;
			}
		}
		else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
		{
			string secondCondition = inHouse.substr(inHouse.find("&") + 1);

			if (inHouseResult && newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
			{
				return true;
			}
		}
		else
		{
			ErrorMessage(1106, templatename, filename, numline, condition);
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

			return conditionProcess(conditionOrder, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility);
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

			if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
			{
				return true;
			}

			if (error)
			{
				return false;
			}

			if (newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
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
						if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
						{
							if (error)
							{
								return false;
							}

							if (newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
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
						char logic = thirdCondition[0];
						thirdCondition = thirdCondition.substr(1);

						if (logic == '&')
						{
							if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
							{
								if (error)
								{
									return false;
								}

								if (newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility) && newCondition(thirdCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
								{
									return true;
								}
							}
						}
						else if (logic == '|')
						{
							if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
							{
								if (newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
								{
									return true;
								}
							}

							if (newCondition(thirdCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
							{
								return true;
							}
						}
						else
						{
							ErrorMessage(1101, templatename, filename, numline);
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
						if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
						{
							if (error)
							{
								return false;
							}

							bool isAnotherNot = false;

							if (secondCondition[0] == '!')
							{
								isAnotherNot = true;
								secondCondition = secondCondition.substr(1);
							}

							if (conditionProcess(secondCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isAnotherNot, utility))
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
						
						if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
						{
							if (newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
							{
								return true;
							}
						}

						if (newCondition(thirdCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
						{
							return true;
						}
					}
					else if (y == NOT_FOUND || (x < y && x != NOT_FOUND))
					{
						size_t position = secondCondition.find("&") + 1;
						string thirdCondition = secondCondition.substr(position);
						secondCondition = secondCondition.substr(0, position - 1);

						if (conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility))
						{
							if (newCondition(secondCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility) && newCondition(thirdCondition, filename, curOptionPicked, groupAnimInfo, numline, templatename, masterformat, utility))
							{
								return true;
							}
						}
					}
					else
					{
						ErrorMessage(1103, templatename, filename, numline);
						return false;
					}
				}
			}
			else
			{
				return conditionProcess(firstCondition, masterformat, filename, numline, curOptionPicked, groupAnimInfo, isNot, utility);
			}
		}
		else
		{
			ErrorMessage(1102, templatename, filename, numline);
			return false;
		}
	}

	return false;
}

vecstr GetOptionInfo(string line, string format, string filename, int numline, vector<vector<shared_ptr<animationInfo>>> groupAnimInfo, bool allowNoFixAnim,
	bool isCondition, int groupMulti, int animMulti, int optionMulti, string multiOption)
{
	int lastGroup = groupAnimInfo.size() - 1;
	int limiter;
	vecstr optionInfo;

	if (isCondition)
	{
		limiter = 4;
	}
	else
	{
		limiter = 5;
	}

	if (line.find(format + "_group[") != NOT_FOUND && groupAnimInfo.size() != 0 && groupMulti < int(groupAnimInfo.size()))
	{
		if (sameWordCount(line, format + "_group[") > 1)
		{
			ErrorMessage(1157, format, filename, numline, line);
			return optionInfo;
		}

		string templine = line;
		size_t pos = templine.find(format + "_group[");
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

		optionInfo[1] = optionOrderProcess(optionInfo[1], format, filename, numline, groupMulti, lastGroup, isCondition, allowNoFixAnim);

		if (error)
		{
			return optionInfo;
		}

		if (optionInfo[1] != "" && !isOnlyNumber(optionInfo[1]))
		{
			ErrorMessage(1121, format, filename, numline, line);
			return optionInfo;
		}

		int curGroup = stoi(optionInfo[1]);

		if (optionInfo.size() > 3)
		{
			optionInfo[2] = optionOrderProcess(optionInfo[2], format, filename, numline, animMulti, groupAnimInfo[groupMulti].size() - 1, isCondition, allowNoFixAnim);

			if (error)
			{
				return optionInfo;
			}

			if (optionInfo[2] != "" && !isOnlyNumber(optionInfo[2]))
			{
				ErrorMessage(1121, format, filename, numline, line);
				return optionInfo;
			}

			int curOrder = stoi(optionInfo[2]);

			if (optionInfo[3].back() == '*')
			{
				string option = optionInfo[3].substr(0, optionInfo[3].length() - 1);
				SSMap* mapPtr = &groupAnimInfo[curGroup][curOrder]->mixOptRegis;

				if (mapPtr->find(option) != mapPtr->end() && (*mapPtr)[option].length() != 0)
				{
					optionInfo[3] = (*mapPtr)[option];
				}
				else
				{
					ErrorMessage(1109, format, filename, numline, line);
					return optionInfo;
				}
			}

			optionLimiter(optionInfo, line, format, filename, numline, groupAnimInfo, 3, isCondition, curGroup, curOrder, limiter, optionMulti);
		}
		else
		{
			if (isOnlyNumber(optionInfo[2]))
			{
				ErrorMessage(1183, format, filename, numline, line);
				return optionInfo;
			}
		}
	}
	else if (line.find(format + "[") != NOT_FOUND && groupAnimInfo.size() != 0 && groupMulti < int(groupAnimInfo.size()))
	{
		nonGroupOptionInfo(optionInfo, line, format, filename, numline, groupAnimInfo, allowNoFixAnim, isCondition, groupMulti, animMulti, optionMulti, multiOption);
		optionLimiter(optionInfo, line, format, filename, numline, groupAnimInfo, 3, isCondition, stoi(optionInfo[1]), stoi(optionInfo[2]), limiter, optionMulti);
	}
	else if (multiOption == format && format.find("_group") == NOT_FOUND)
	{
		nonGroupOptionInfo(optionInfo, line, format, filename, numline, groupAnimInfo, allowNoFixAnim, isCondition, groupMulti, animMulti, optionMulti, multiOption);
		optionLimiter(optionInfo, line, format, filename, numline, groupAnimInfo, 3, isCondition, stoi(optionInfo[1]), stoi(optionInfo[2]), limiter, optionMulti);
	}
	else
	{
		ErrorMessage(1183, format, filename, numline, line);
	}

	return optionInfo;
}

void nonGroupOptionInfo(vecstr& optionInfo, string line, string format, string filename, int numline, vector<vector<shared_ptr<animationInfo>>> groupAnimInfo,
	bool allowNoFixAnim, bool isCondition, int groupMulti, int animMulti, int optionMulti, string multiOption)
{
	string templine = line;
	size_t pos = templine.find(format + "[");
	optionInfo.push_back(format + "_group");
	templine = templine.substr(templine.find("[", pos) + 1);

	if (groupMulti == -1)
	{
		ErrorMessage(1056, format, filename, numline, line);
		return;
	}

	optionInfo.push_back(to_string(groupMulti));

	if (line.find(format + "[") == NOT_FOUND && line.find(format + "_group[") == NOT_FOUND && multiOption == format)
	{
		if (animMulti == -1)
		{
			ErrorMessage(1056, format, filename, numline, line);
			return;
		}

		optionInfo.push_back(to_string(animMulti));
		optionInfo.push_back(line);
		return;
	}

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

	optionInfo[2] = optionOrderProcess(optionInfo[2], format, filename, numline, animMulti, groupAnimInfo[groupMulti].size() - 1, isCondition, allowNoFixAnim);

	if (error)
	{
		return;
	}

	if (!isOnlyNumber(optionInfo[2]))
	{
		ErrorMessage(1183, format, filename, numline, line);
		return;
	}

	int curOrder = stoi(optionInfo[2]);

	if (optionInfo[3].back() == '*')
	{
		string option = optionInfo[3].substr(0, optionInfo[3].length() - 1);
		SSMap* mapPtr = &groupAnimInfo[groupMulti][curOrder]->mixOptRegis;

		if (mapPtr->find(option) != mapPtr->end() && (*mapPtr)[option].length() != 0)
		{
			optionInfo.back() = (*mapPtr)[option];
		}
		else
		{
			ErrorMessage(1109, format, filename, numline, line);
			return;
		}
	}
}

void optionLimiter(vecstr optionInfo, string line, string format, string filename, int numline, vector<vector<shared_ptr<animationInfo>>> groupAnimInfo,
	int open, bool isCondition, int curGroup, int curOrder, int limiter, int optionMulti)
{
	if (!isCondition && int(optionInfo.size()) > open + 1)
	{
		if (optionInfo.size() == open + 3)
		{
			bool isPassed = true;

			if (optionInfo[open + 2].length() == 0)
			{
				if (optionMulti == -1)
				{
					ErrorMessage(1060, format, filename, numline, line);
					return;
				}
				else
				{
					optionInfo[open + 2] = to_string(optionMulti);
				}
			}
			else
			{
				if (!isOnlyNumber(optionInfo[open + 2]))
				{
					ErrorMessage(1055, format, filename, numline, line);
					return;
				}
			}
		}

		if (groupAnimInfo[curGroup][curOrder]->groupAddition[optionInfo[open]][optionInfo[open + 1]].size() != 0)
		{
			limiter++;
		}
	}

	if (int(optionInfo.size()) > limiter || int(optionInfo.size()) < limiter)
	{
		ErrorMessage(1054, limiter - 1, format, filename, numline, line);
		return;
	}

	if (optionInfo[open][0] == '^')
	{
		ErrorMessage(1149, format, filename, numline, line);
		return;
	}
}

string optionOrderProcess(string line, string format, string filename, int numline, int groupMulti, int lastOrder, bool isCondition, bool allowNoFixAnim)
{
	string newline = line;

	if (newline == "F")
	{
		newline = "0";
	}
	else if (newline == "L")
	{
		newline = to_string(lastOrder);
	}
	else if (newline == "N")
	{
		if (lastOrder - 1 == groupMulti)
		{
			newline = to_string(groupMulti);
		}
		else
		{
			newline = to_string(groupMulti + 1);
		}
	}
	else if (newline == "B")
	{
		if (groupMulti == 0)
		{
			newline = to_string(groupMulti);
		}
		else
		{
			newline = to_string(groupMulti - 1);
		}
	}
	else if (newline[0] == '(' && newline[newline.length() - 1] == ')')
	{
		int Fnum = sameWordCount(newline, "F");
		int Nnum = sameWordCount(newline, "N");
		int Bnum = sameWordCount(newline, "B");
		int Lnum = sameWordCount(newline, "L");

		for (int i = 0; i < Fnum; ++i)
		{
			newline.replace(newline.find("F"), 1, "0");
		}

		if (lastOrder == groupMulti)
		{
			for (int i = 0; i < Nnum; ++i)
			{
				newline.replace(newline.find("N"), 1, to_string(groupMulti));
			}
		}
		else
		{
			for (int i = 0; i < Nnum; ++i)
			{
				newline.replace(newline.find("N"), 1, to_string(groupMulti + 1));
			}
		}

		if (groupMulti == 0)
		{
			for (int i = 0; i < Bnum; ++i)
			{
				newline.replace(newline.find("B"), 1, to_string(groupMulti));
			}
		}
		else
		{
			for (int i = 0; i < Bnum; ++i)
			{
				newline.replace(newline.find("B"), 1, to_string(groupMulti - 1));
			}
		}

		for (int i = 0; i < Lnum; ++i)
		{
			newline.replace(newline.find("L"), 1, to_string(lastOrder));
		}

		calculate(newline, format, filename, numline);

		if (error)
		{
			return newline;
		}
	}
	else if (newline.length() == 0)
	{
		if (!allowNoFixAnim)
		{
			ErrorMessage(1056, format, filename, numline, line);
			return newline;
		}
		else
		{
			if (groupMulti == -1)
			{
				if (!isCondition)
				{
					ErrorMessage(1056, format, filename, numline, line);
					return newline;
				}
			}
			else
			{
				newline = to_string(groupMulti);
			}
		}
	}
	else
	{
		string templine = newline;
		templine = templine + "a";
		string newtempline = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (newtempline == templine)
		{
			ErrorMessage(1055, format, filename, numline, line);
			return newline;
		}

		if (stoi(newline) > int(lastOrder))
		{
			ErrorMessage(1148, format, filename, numline, line);
			return newline;
		}
	}
	
	return newline;
}

bool conditionProcess(string condition, string masterformat, string filename, int numline, vector<vector<unordered_map<string, bool>>> curOptionPicked,
	vector<vector<shared_ptr<animationInfo>>> groupAnimInfo, bool isNot, animationutility utility)
{
	if (condition.find(masterformat + "_group[][") != NOT_FOUND)
	{
		int GOG = 0;

		while (GOG < int(groupAnimInfo.size()))
		{
			int formatGroup = 0;
			int groupEnd = 1;
			bool conditionResult;

			if (condition.find(masterformat + "_group[][]") != NOT_FOUND)
			{
				groupEnd = int(groupAnimInfo[GOG].size());
			}
			else
			{
				groupEnd = int(condition.find(masterformat + "_group[][")) + masterformat.length() + 9;
				string number = condition.substr(groupEnd, condition.find("]", groupEnd) - groupEnd);
				optionOrderProcess(number, masterformat, filename, numline, groupAnimInfo[GOG].size() - 1, groupAnimInfo[GOG].size() - 1, true, true);

				if (number.length() > 0)
				{
					formatGroup = stoi(number);
					groupEnd = formatGroup + 1;
				}
				else
				{
					formatGroup = 0;
					groupEnd = int(groupAnimInfo[GOG].size());
				}
			}

			while (formatGroup < groupEnd)
			{
				vecstr optionInfo = GetOptionInfo(condition, masterformat, filename, numline, groupAnimInfo, true, true, GOG, formatGroup,
					utility.optionMulti);

				if (error)
				{
					return false;
				}

				if (curOptionPicked[stoi(optionInfo[1])][stoi(optionInfo[2])][optionInfo[3]])
				{
					conditionResult = !isNot;
				}
				else
				{
					conditionResult = isNot;
				}

				if (!conditionResult)
				{
					return false;
				}

				++formatGroup;
			}

			++GOG;
		}
	}
	if (condition.find(masterformat + "[][") != NOT_FOUND)
	{
		if (utility.groupMulti == -1)
		{
			ErrorMessage(1056, masterformat, filename, numline, condition);
			return false;
		}

		int formatGroup = 0;
		bool conditionResult;

		while (formatGroup < int(groupAnimInfo[utility.groupMulti].size()))
		{
			vecstr optionInfo = GetOptionInfo(condition, masterformat, filename, numline, groupAnimInfo, true, true, utility.groupMulti,
				formatGroup, utility.optionMulti);

			if (error)
			{
				return false;
			}

			if (curOptionPicked[stoi(optionInfo[1])][stoi(optionInfo[2])][optionInfo[3]])
			{
				conditionResult = !isNot;
			}
			else
			{
				conditionResult = isNot;
			}

			if (!conditionResult)
			{
				return false;
			}

			++formatGroup;
		}
	}
	else
	{
		vecstr optionInfo = GetOptionInfo(condition, masterformat, filename, numline, groupAnimInfo, false, true, utility.groupMulti,
			utility.animMulti, utility.optionMulti, utility.multiOption);

		if (error)
		{
			return false;
		}

		if (curOptionPicked[stoi(optionInfo[1])][stoi(optionInfo[2])][optionInfo[3]])
		{
			return !isNot;
		}
		else
		{
			return isNot;
		}
	}

	return true;
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
	++(*nextFunctionID);

	if (*nextFunctionID == 9216)
	{
		++(*nextFunctionID);
	}

	strID = to_string(*nextFunctionID);

	while (strID.length() < 4)
	{
		strID = "0" + strID;
	}
}

void groupTemplate::setZeroEvent(string eventname)
{
	zeroEvent = eventname;
}

void groupTemplate::setZeroVariable(string variablename)
{
	zeroVariable = variablename;
}

inline void ExistingFunction::newID()
{
	++(*nextFunctionID);

	if (*nextFunctionID == 9216)
	{
		++(*nextFunctionID);
	}

	strID = to_string(*nextFunctionID);

	while (strID.length() < 4)
	{
		strID = "0" + strID;
	}
}

void ExistingFunction::setZeroEvent(string eventname)
{
	zeroEvent = eventname;
}

void ExistingFunction::setZeroVariable(string variablename)
{
	zeroVariable = variablename;
}

int formatGroupReplace(string& curline, string oriline, int point, string filename, string format, shared_ptr<master> subFunctionIDs,
	vector<vector<shared_ptr<animationInfo>>> groupAnimInfo, int linecount, int groupMulti, int optionMulti, int animMulti, string multiOption, bool innerError)
{
	int open = 0;
	int curGroup;
	int curAnim;
	int curPoint = curline.length();

	if (animMulti > -1 && groupMulti > -1)
	{
		for (unsigned int i = point; i < curline.length(); ++i)
		{
			if (curline[i] == '[')
			{
				++open;

				if (open > 1)
				{
					i = formatGroupReplace(curline, oriline, curGroup, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti,
						optionMulti, animMulti, multiOption, innerError);
					--open;

					if (innerError)
					{
						return 0;
					}
				}

				curGroup = i + 1;
			}
			else if (curline[i] == ']')
			{
				--open;

				if (open < 0)
				{
					break;
				}

				curPoint = i + 1;
			}
		}
	}
	else
	{
		for (unsigned int i = point; i < curline.length(); ++i)
		{
			if (curline[i] == '[')
			{
				++open;

				if (open > 1)
				{
					i = formatGroupReplace(curline, oriline, curGroup, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti,
						optionMulti, animMulti, multiOption, innerError);
					--open;

					if (innerError)
					{
						return 0;
					}
				}

				curGroup = i + 1;
			}
			else if (curline[i] == ']')
			{
				--open;

				if (open < 0)
				{
					break;
				}

				curPoint = i + 1;
			}
		}
	}

	if (open > 0)
	{
		ErrorMessage(1183, format, filename, linecount, oriline);
		return 0;
	}

	if (curline != oriline)
	{
		if (curline.find("[") == NOT_FOUND)
		{
			return 0;
		}
		else if (curline.find(format + "[") == NOT_FOUND && curline.find(format + "_group[") == NOT_FOUND)
		{
			return 0;
		}
	}

	string originalLine = curline.substr(point, curPoint - point);
	vecstr groupline = GetOptionInfo(originalLine, format, filename, linecount, groupAnimInfo, true, true, groupMulti, animMulti, optionMulti, multiOption);

	if (error)
	{
		return 0;
	}

	string input;
	string output;
	curGroup = stoi(groupline[1]);
	curAnim = animMulti;

	if (isOnlyNumber(groupline[2]))
	{
		open = 4;
		curAnim = stoi(groupline[2]);
		input = groupline[3];
	}
	else
	{
		open = 3;
		input = groupline[2];
	}

	if (groupline.back().find("@AnimObject/") != NOT_FOUND)
	{
		if (optionMulti != -1)
		{
			ErrorMessage(1128, format, filename, linecount);
			return 0;
		}

		groupline.back().append(to_string(optionMulti + 1));

		for (unsigned int d = open; d < groupline.size(); ++d)
		{
			input = input + "[" + groupline[d] + "]";
		}
	}
	else if (groupline.back().length() == 0)
	{
		if (optionMulti != -1)
		{
			ErrorMessage(1128, format, filename, linecount);
			return 0;
		}

		groupline.back().append(to_string(optionMulti));

		for (unsigned int d = open; d < groupline.size() - 1; ++d)
		{
			input = input + "[" + groupline[d] + "]";
		}
	}
	else
	{
		for (unsigned int d = open; d < groupline.size(); ++d)
		{
			input = input + "[" + groupline[d] + "]";
		}
	}
	
	if (open == 4)
	{
		unordered_map<string, string>* mapPtr = &subFunctionIDs->grouplist[stoi(groupline[1])]->singlelist[stoi(groupline[2])]->format;
		shared_ptr<animationInfo> animInfo = groupAnimInfo[stoi(groupline[1])][stoi(groupline[2])];

		if (mapPtr->find(input) != mapPtr->end() && (*mapPtr)[input].length() > 0)
		{
			output = (*mapPtr)[input];
		}
		else if (animInfo->addOn.find(groupline[3]) != animInfo->addOn.end() && animInfo->addOn[groupline[3]].size() > 0 && optionMulti > -1)
		{
			addOnReplacer(input, filename, animInfo->addOn, animInfo->addition, animInfo->groupAddition, animInfo->mixOptRegis, optionMulti, format, linecount);
		}
		else if (multiOption == format && groupMulti != -1 && animMulti != -1)
		{
			for (unsigned int i = point; i < curline.length(); ++i)
			{
				if (curline[i] == '[')
				{
					curGroup = i + 1;
				}
				else if (curline[i] == ']')
				{
					string templine = curline.substr(curGroup, i - curGroup);

					if (templine.find("[") == NOT_FOUND && templine.find("]") == NOT_FOUND)
					{
						unordered_map<std::string, std::string>* formatOut = &subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format;

						if (formatOut->find(templine) != formatOut->end() && (*formatOut)[templine].length() > 0)
						{
							curline.replace(curline.find(templine), templine.length(), (*formatOut)[templine]);
						}
						else
						{
							for (auto& match : *formatOut)
							{
								if (templine.find(match.first) != NOT_FOUND)
								{
									curline.replace(curline.find(templine), templine.length(), match.second);
								}
							}
						}
					}
				}
			}

			return point;
		}
	}
	else
	{
		output = subFunctionIDs->grouplist[stoi(groupline[1])]->functionIDs[input];
	}

	if (output.length() == 0 || groupline.size() < 3)
	{
		if (curline.find(format + "[") != NOT_FOUND || curline.find(format + "_group[") != NOT_FOUND)
		{
			ErrorMessage(1158, format, filename, linecount, curline);
			return 0;
		}
	}
	else
	{
		curline.replace(point, originalLine.length(), output);
	}

	return point;
}
