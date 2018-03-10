#include "grouptemplate.h"

using namespace std;

string zeroEvent;

inline int sameWordCount(string line, string word)
{
	size_t nextWord = 0;
	int wordCount = 0;

	while (true)
	{
		nextWord = line.find(word, nextWord + 1);
		if (nextWord != -1)
		{
			wordCount++;
		}
		else
		{
			break;
		}
	}
	return wordCount;
}

groupTemplate::groupTemplate(vecstr grouptemplateformat)
{
	templatelines = grouptemplateformat;
}

vecstr groupTemplate::getFunctionLines(string formatname, int& stateID, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, int& nFunctionID, ImportContainer& import, id eventid, id variableid, string masterFormat, int groupCount)
{
	format = formatname;
	unordered_map<string, string> IDExist;
	vecstr tempstore;
	vecstr functionline;
	vecstr templateID;
	string multiOption;
	bool multi = false;
	bool norElement = false;
	bool open = false;
	bool skip = false;
	bool freeze = false; // mainly used by CONDITION to freeze following CONDITION
	__int64 openRange = 0;
	int fixedStateID = stateID;
	int condition = 0;
	int counter = 0;
	int order = -2;
	strID = to_string(nFunctionID);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	newImport = const_cast<ImportContainer*>(&import);
	nextStateID = const_cast<int*>(&stateID);
	unordered_map<int, bool> IsConditionOpened;
	size_t elementLine = -1;
	vector<unordered_map<string, bool>> groupOptionPicked;

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	for (unsigned int i = 0; i < 4 - strID.length(); i++)
	{
		strID = "0" + strID;
	}

	// check error before initialization
	if (subFunctionIDs.size() != groupAnimInfo.size() && groupCount != -1)
	{
		cout << ">> ERROR(1156): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << "Template: " << format << endl << endl;
		error = true;
		return functionline;
	}

	IsConditionOpened[0] = true;
	functionline.reserve(templatelines.size() + 20 * memory);

	for (unsigned int i = 0; i < templatelines.size(); ++i)
	{
		bool uniqueskip = false;
		bool elementCatch = false;

		if (templatelines[i].find("<!-- CONDITION START ^", 0) != string::npos)
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
		else if (templatelines[i].find("<!-- CONDITION ^", 0) != string::npos)
		{
			if (condition == 0)
			{
				cout << "ERROR(1118): Opening of condition is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
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
		else if (templatelines[i].find("<!-- CONDITION -->", 0) != string::npos)
		{
			if (condition == 0)
			{
				cout << "ERROR(1120): Opening of condition is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
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
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "^ -->", 0) != string::npos || templatelines[i].find("<!-- NEW ^" + masterFormat + "_group^ -->", 0) != string::npos)
		{
			cout << "ERROR(1162): Basic new tab is not supported to access other template. Use multi new tab instead. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
			error = true;
			functionline.shrink_to_fit();
			return functionline;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					error = true;
					functionline.shrink_to_fit();
					return functionline;
				}
			}
			else
			{
				cout << "ERROR(1159): Failure to access individual animation template. Master animation template has no access to individual animation template but group animation template. Please contact the template creator" << endl << "Template: " << format << "Line: " << i + 1 << endl << endl;
				error = true;
				functionline.shrink_to_fit();
				return functionline;
			}

			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "_group^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					error = true;
					functionline.shrink_to_fit();
					return functionline;
				}
			}
			else
			{
				cout << "ERROR(1160): Failure to access group animation template. Group animation template has no access to other group animation template but individual animation template. Use import function instead. Please contact the template creator" << endl << "Template: " << format << "Line: " << i + 1 << endl << endl;
				error = true;
				functionline.shrink_to_fit();
				return functionline;
			}

			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- NEW ^" + masterFormat + "_master^ -->", 0) != string::npos || templatelines[i].find("<!-- NEW ^" + masterFormat + "_master^ +% -->", 0) != string::npos)
		{
			string templatename;

			if (groupCount == -1)
			{
				templatename = masterFormat + "_master";
			}
			else
			{
				templatename = masterFormat + "_group";
			}

			cout << "ERROR(1164): Fail to access master animation template. Only existing node has access to master animation template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
			error = true;
			functionline.shrink_to_fit();
			return functionline;
		}
		else if (templatelines[i].find("<!-- NEW ^", 0) != string::npos && templatelines[i].find("^ -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "WARNING: General AnimObject cannot be used in non-multi new. Use Specific AnimObject instead. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					skip = true;
				}
			}
			else
			{
				cout << "ERROR(1117): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
				functionline.shrink_to_fit();
				return functionline;
			}

			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- NEW ^", 0) != string::npos && templatelines[i].find("^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "WARNING: specific AnimObject cannot be used in multi new. Use \"AnimObject\" instead. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					skip = true;
				}
			}
			else
			{
				cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
				functionline.shrink_to_fit();
				return functionline;
			}

			uniqueskip = true;
		}
		else if (templatelines[i].find("<!-- CLOSE -->", 0) != string::npos || templatelines[i].find("<!-- CONDITION END -->", 0) != string::npos)
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

				if (templatelines[i].find("MID$", 0) != string::npos)
				{
					int counter = sameWordCount(templatelines[i], "MID$");

					for (int k = 0; k < counter; k++)
					{
						size_t MIDposition = templatelines[i].find("MID$");
						string ID = boost::regex_replace(string(templatelines[i].substr(MIDposition)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
						string oldID = "MID$" + ID;

						if (templatelines[i].find(oldID, MIDposition) != string::npos)
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
				if (templatelines[i].find("$(S+", 0) != string::npos)
				{
					templatelines[i] = stateReplacer(templatelines[i], fixedStateID, i + 1, groupCount);
				}

				if (groupCount != -1)
				{
					// set animation ID
					if (templatelines[i].find("$%$", 0) != string::npos)
					{
						templatelines[i].replace(templatelines[i].find("$%$"), 3, to_string(groupCount));
					}

					// multi choice selection
					if (templatelines[i].find("$MC$", 0) != string::npos)
					{
						multiChoice(templatelines[i], groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat);

						if (error)
						{
							functionline.shrink_to_fit();
							return functionline;
						}
					}

					if (templatelines[i].find("%") != string::npos && templatelines[i].find("+%") == string::npos)
					{
						__int64 counter = count(templatelines[i].begin(), templatelines[i].end(), '%');

						for (int p = 0; p < counter; ++p)
						{
							templatelines[i].replace(templatelines[i].find("%"), 1, to_string(0));
						}
					}
				}

				// compute numelements
				if (templatelines[i].find("<hkparam name=\"") != string::npos && templatelines[i].find("numelements=\"") != string::npos && templatelines[i].find("</hkparam>") == string::npos && templatelines[i].find("<!-- COMPUTE -->", templatelines[i].find("numelements=\"")) != string::npos)
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
						cout << "ERROR(1136): Multiple computation function running concurrently detected. Only 1 computation function can be done at a time" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
						error = true;
						functionline.shrink_to_fit();
						return functionline;
					}
				}
				else if (templatelines[i].find("</hkparam>") != string::npos && norElement)
				{
					string templine = templatelines[i].substr(0, templatelines[i].find("</hkparam>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						string oldElement;

						if (functionline[elementLine].find("numelements=\"$elements$\">", 0) == string::npos)
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

					if (templine.find("<hkobject>") != string::npos)
					{
						templine = templine.substr(0, templine.find("<hkobject>"));
						__int64 range = count(templine.begin(), templine.end(), '\t');

						if (range == openRange + 1)
						{
							counter++;
						}
					}
					else if (templine.find("\t\t\t#") != string::npos)
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

				if (templatelines[i].find("\t\t\t#") != string::npos && templatelines[i].find("$") != string::npos && templatelines[i].find("#" + masterFormat + "$") != string::npos && templatelines[i].find("#" + masterFormat + "_group$") != string::npos)
				{
					stringstream sstream(templatelines[i]);
					istream_iterator<string> ssbegin(sstream);
					istream_iterator<string> ssend;
					vecstr generator(ssbegin, ssend);
					copy(generator.begin(), generator.end(), generator.begin());
					size_t nextpos = 0;

					for (unsigned int p = 0; p < generator.size(); p++)
					{
						string ID = generator[p];

						if (ID.find("#" + masterFormat + "$") != string::npos && multiOption == masterFormat)
						{
							nextpos = templatelines[i].find("#" + masterFormat + "$", nextpos) + 1;
							string tempID = templatelines[i].substr(nextpos);
							string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							curID = masterFormat + "$" + curID;

							if (tempID.find(curID, 0) != string::npos && nextpos == templatelines[i].find(curID))
							{
								if (subFunctionIDs[0][curID].length() == 0)
								{
									cout << "ERROR(2105): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
									error = true;
									functionline.shrink_to_fit();
									return functionline;
								}

								templatelines[i].replace(nextpos, curID.length(), subFunctionIDs[0][curID]);
							}
							else
							{
								cout << "ERROR(2106): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << "ID: " << curID << endl << endl;
								error = true;
								functionline.shrink_to_fit();
								return functionline;
							}
						}
						else if (ID.find("#" + masterFormat + "_group") != string::npos && multiOption == masterFormat + "_group")
						{
							nextpos = templatelines[i].find("#" + masterFormat + "_group$", nextpos) + 1;
							string tempID = templatelines[i].substr(nextpos);
							string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							curID = multiOption + "$" + curID;

							if (tempID.find(curID, 0) != string::npos && nextpos == templatelines[i].find(curID))
							{
								for (unsigned int k = 0; k < subFunctionIDs.size(); k++) // number of variation
								{
									if (subFunctionIDs[k][curID].length() == 0)
									{
										cout << "ERROR(2105): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
										error = true;
										functionline.shrink_to_fit();
										return functionline;
									}

									templatelines[i].replace(nextpos, curID.length(), subFunctionIDs[k][curID]);
								}
							}
							else
							{
								cout << "ERROR(2106): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << "ID: " << curID << endl << endl;
								error = true;
								functionline.shrink_to_fit();
								return functionline;
							}
						}
					}
				}

				if (templatelines[i].find("$") != string::npos && groupCount != -1)
				{
					processing(templatelines[i], masterFormat, i + 1, subFunctionIDs, groupAnimInfo, eventid, variableid);

					if (error)
					{
						functionline.shrink_to_fit();
						return functionline;
					}
				}

				functionline.push_back(templatelines[i]);

				if (elementCatch)
				{
					elementLine = functionline.size() - 1;
				}

				break;
			}
		}

		if (templatelines[i].find("<!-- CLOSE -->", 0) != string::npos && IsConditionOpened[condition])
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

								if (curLine.find("<!-- ") != string::npos && curLine.find(" -->", curLine.find("<!-- ")) != string::npos && curLine.find("$MC$") == string::npos)
								{
									bool isNot = false;
									size_t pos = curLine.find("<!-- ") + 5;
									string option = curLine.substr(pos, curLine.find(" -->") - pos);
									
									if (option[0] == '!')
									{
										isNot = true;
										option = option.substr(1);
									}

									if (option.find("L") != string::npos)
									{
										option.replace(option.find("L"), 1, to_string(groupAnimInfo.size() - 1));
									}

									if (option.find("F") != string::npos)
									{
										option.replace(option.find("F"), 1, "0");
									}

									if (sameWordCount(option, "$") == 2 && option.find("$(") != string::npos && option.find(")$", option.find("$(")) != string::npos)
									{
										pos = option.find("$(") + 1;
										option = option.substr(pos, option.find(")$") - pos + 1);
										calculate(option, error);

										if (error)
										{
											cout << "ERROR(2113): Invalid equation. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << "Equation: $" << option << "$" << endl << endl;
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

								if (curLine.find("$") != string::npos)
								{
									// set animation ID
									if (curLine.find("$%$", 0) != string::npos)
									{
										curLine.replace(curLine.find("$%$"), 3, to_string(groupCount));
									}

									if (curLine.find("MID$", 0) != string::npos)
									{
										int counter = sameWordCount(curLine, "MID$");

										for (int k = 0; k < counter; k++)
										{
											size_t MIDposition = curLine.find("MID$");
											string ID = boost::regex_replace(string(curLine.substr(MIDposition)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											string oldID = "MID$" + ID;

											if (curLine.find(oldID, MIDposition) != string::npos)
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
									if (curLine.find("$(S+", 0) != string::npos)
									{
										curLine = stateReplacer(curLine, fixedStateID, i + 1, groupCount);
									}

									// multi choice selection
									if (curLine.find("$MC$", 0) != string::npos)
									{
										multiChoice(curLine, groupOptionPicked, groupAnimInfo, i + 1, format, masterFormat);

										if (error)
										{
											functionline.shrink_to_fit();
											return functionline;
										}
									}
								}

								if (curLine.find("%") != string::npos && curLine.find("+%") == string::npos)
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

									if (templine.find("<hkobject>") != string::npos)
									{
										templine = templine.substr(0, templine.find("<hkobject>"));
										__int64 range = count(templine.begin(), templine.end(), '\t');

										if (range == openRange + 1)
										{
											counter++;
										}
									}
									else if (templine.find("\t\t\t#") != string::npos)
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

								if (curLine.find("\t\t\t#") != string::npos && (curLine.find("#" + masterFormat + "$") != string::npos || curLine.find("#" + masterFormat + "_group$") != string::npos))
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

										if (ID.find("#" + masterFormat + "$") != string::npos && multiOption == masterFormat)
										{
											nextpos = curLine.find("#" + masterFormat + "$", nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = masterFormat + "$" + curID;

											if (tempID.find(curID, 0) != string::npos && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													cout << "ERROR(2105): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
													error = true;
													functionline.shrink_to_fit();
													return functionline;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												cout << "ERROR(2106): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << "ID: " << curID << endl << endl;
												error = true;
												functionline.shrink_to_fit();
												return functionline;
											}
										}
										else if (ID.find("#" + masterFormat + "_group") != string::npos && multiOption == masterFormat + "_group")
										{
											nextpos = curLine.find("#" + multiOption, nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = multiOption + "$" + curID;

											if (tempID.find(curID, 0) != string::npos && nextpos == curLine.find(curID))
											{
												for (unsigned int k = 0; k < subFunctionIDs.size(); k++) // number of variation
												{
													if (subFunctionIDs[k][curID].length() == 0)
													{
														cout << "ERROR(2105): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
														error = true;
														functionline.shrink_to_fit();
														return functionline;
													}

													curLine.replace(nextpos, curID.length(), subFunctionIDs[k][curID]);
												}
											}
											else
											{
												cout << "ERROR(2106): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << "ID: " << curID << endl << endl;
												error = true;
												functionline.shrink_to_fit();
												return functionline;
											}
										}
									}
								}

								if (curLine.find("$") != string::npos)
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
		else if (templatelines[i].find("<!-- CONDITION END -->", 0) != string::npos)
		{
			if (condition == 0)
			{
				cout << "ERROR(1119): Unable to close condition. No opened condition is found. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
				functionline.shrink_to_fit();
				return functionline;
			}

			if (freeze && IsConditionOpened[condition])
			{
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

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			cout << "ERROR(1108): Unresolved option section. Closing of option is required. Please contact the template creator" << endl << "Template: " << format << endl << endl;
			error = true;
			functionline.shrink_to_fit();
			return functionline;
			break;
		}
	}

	functionIDs[format + "[" + to_string(groupCount) + "][StateID]"] = to_string(fixedStateID);

	if (functionline.back().length() != 0)
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

vecstr ExistingFunction::groupExistingFunctionProcess(vecstr existingFunctionLines, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, string curformat, ImportContainer import, id eventid, id variableid, int& nFunctionID, bool hasMaster, bool hasGroup)
{
	vecstr newFunctionLines;
	vecstr tempstore;
	vecstr empty;

	string multiOption;
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
	unordered_map<int, bool> IsConditionOpened;
	shared_ptr<string> elementLine = make_shared<string>();
	vector<unordered_map<string, bool>> groupOptionPicked;

	// check error before initialization
	if (subFunctionIDs.size() != groupAnimInfo.size() && !hasGroup)
	{
		cout << ">> ERROR(1155): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << "Template: " << format << endl << endl;
		error = true;
		return newFunctionLines;
	}

	for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
	{
		groupOptionPicked.push_back(groupAnimInfo[i]->optionPicked);
	}

	newImport = const_cast<ImportContainer*>(&import);
	nextFunctionID = const_cast<int*>(&nFunctionID);
	format = curformat;
	strID = to_string(nFunctionID);


	for (unsigned int i = 0; i < 4 - strID.length(); i++)
	{
		strID = "0" + strID;
	}

	IsConditionOpened[0] = true;
	newFunctionLines.reserve(existingFunctionLines.size() + 20 * memory);

	for (unsigned int i = 0; i < existingFunctionLines.size(); ++i)
	{
		bool uniqueskip = false;
		bool elementCatch = false;

		string line = existingFunctionLines[i];

		if (line.find("<!-- CONDITION START", 0) != string::npos)
		{
			condition++;

			if (line.find("<!-- CONDITION START ^", 0) != string::npos)
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
				cout << "ERROR(1121): Invalid condition. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CONDITION ^", 0) != string::npos)
		{
			if (condition == 0)
			{
				cout << "ERROR(1118): Opening of condition is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
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
		else if (line.find("<!-- CONDITION -->", 0) != string::npos)
		{
			if (condition == 0)
			{
				cout << "ERROR(1120): Opening of condition is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
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
		else if (line.find("<!-- NEW ^" + format + "^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					error = true;
					return empty;
				}
			}
			else
			{
				cout << "ERROR(1161): Failure to access individual animation template. Existing node can only access to the highest level of animation template. Please contact the template creator" << endl << "Template: " << format << "Line: " << i + 1 << endl << endl;
				error = true;
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^" + format + "_group^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					error = true;
					return empty;
				}
			}
			else
			{
				cout << "ERROR(1162): Failure to access group animation template. Existing node can only access to the highest level of animation template. Please contact the template creator" << endl << "Template: " << format << "Line: " << i + 1 << endl << endl;
				error = true;
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^" + format + "_master^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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
					cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
					error = true;
					return empty;
				}
			}
			else
			{
				cout << "ERROR(1163): Failure to access master animation template. Master animation template not found. Please contact the template creator" << endl << "Template: " << format << "Line: " << i + 1 << endl << endl;
				error = true;
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^" + format + "^ -->", 0) != string::npos || line.find("<!-- NEW ^" + format + "_group^ -->", 0) != string::npos || line.find("<!-- NEW ^" + format + "_master^ -->", 0) != string::npos)
		{
			cout << "ERROR(1162): Basic new tab is not supported to access other template. Use multi new tab instead. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
			error = true;
			return empty;
		}
		else if (line.find("<!-- NEW ^", 0) != string::npos && line.find("^ -->", 0) != string::npos && IsConditionOpened[condition])
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

				if (curOption.find(format + "[") != string::npos && curOption.find("]") != string::npos)
				{
					int pos = 0;
					vecstr formatInfo = GetOptionInfo(curOption, format, i + 1, groupAnimInfo.size() - 1, groupAnimInfo, false, true);

					if (error)
					{
						return empty;
					}

					if (formatInfo[2] == "AnimObject")
					{
						cout << "ERROR(1153): Invalid element. Specifying the AnimObject is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << "Option: " << curOption << endl << endl;
						skip = true;
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
				cout << "ERROR(1117): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- NEW ^", 0) != string::npos && line.find("^ +% -->", 0) != string::npos && IsConditionOpened[condition])
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

				if (curOption.find(format + "[") != string::npos && curOption.find("]") != string::npos)
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
				cout << "ERROR(1115): Unresolved order section. Closing of order is required. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
				return empty;
			}

			uniqueskip = true;
		}
		else if (line.find("<!-- CLOSE -->", 0) != string::npos || line.find("<!-- CONDITION END -->", 0) != string::npos)
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
					
					if (elementCatch)
					{
						elementLine = make_shared<string>(tempstore.back());
					}

					break;
				}

				// set animation ID
				if (line.find("$%$", 0) != string::npos)
				{
					line.replace(line.find("$%$"), 3, "0");
				}

				// multi choice selection
				if (line.find("$MC$", 0) != string::npos)
				{
					multiChoice(line, groupOptionPicked, groupAnimInfo, i + 1, format, format);

					if (error)
					{
						return empty;
					}
				}

				// compute numelements
				if (line.find("<hkparam name=\"") != string::npos && line.find("numelements=\"") != string::npos && line.find("</hkparam>") == string::npos && line.find("<!-- COMPUTE -->", 0) != string::npos)
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
						cout << ">> ERROR(2103): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
						error = true;
						return empty;
					}
				}
				else if (line.find("</hkparam>") != string::npos && isElement)
				{
					string templine = line.substr(0, line.find("</hkparam>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						string oldElement;

						if (elementLine->find("numelements=\"$elements$\">", 0) == string::npos)
						{
							size_t position = elementLine->find("numelements=\"") + 13;
							oldElement = elementLine->substr(position, elementLine->find("\">", position) - position);
						}
						else
						{
							oldElement = "$elements$";
						}

						if (oldElement != to_string(elementCount))
						{
							elementLine->replace(elementLine->find(oldElement), oldElement.length(), to_string(elementCount));
						}

						isElement = false;
						elementCount = 0;
						elementLine = NULL;
					}
				}

				if (isElement)
				{
					string templine = line;

					if (templine.find("<hkobject>") != string::npos)
					{
						templine = templine.substr(0, templine.find("<hkobject>"));
						__int64 range = count(templine.begin(), templine.end(), '\t');

						if (range == openRange + 1)
						{
							elementCount++;
						}
					}
					else if (templine.find("\t\t\t#") != string::npos)
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
				
				if (line.find("$") != string::npos && !hasGroup)
				{
					processing(line, i + 1, subFunctionIDs, groupAnimInfo, eventid, variableid);
					
					if (error)
					{
						return empty;
					}
				}

				newFunctionLines.push_back(line);

				if (elementCatch)
				{
					elementLine = make_shared<string>(newFunctionLines.back());
				}

				break;
			}
		}

		if (line.find("<!-- CLOSE -->", 0) != string::npos)
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
								size_t linecount = i + 1 - tempstore.size() + l + (optionMulti * tempstore.size()) + (groupAnimInfo[animMulti]->optionPickedCount[multiOption] * animMulti * tempstore.size());

								if (curLine.find("<hkparam name=\"") != string::npos && curLine.find("numelements=\"") != string::npos && curLine.find("</hkparam>") == string::npos && curLine.find("<!-- COMPUTE -->", 0) != string::npos)
								{
									cout << "ERROR(1140): Unable to call computation function within multi new tab. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
									error = true;
									return empty;
								}

								if (isElement)
								{
									string templine = curLine;

									if (templine.find("<hkobject>") != string::npos)
									{
										templine = templine.substr(0, templine.find("<hkobject>"));
										__int64 range = count(templine.begin(), templine.end(), '\t');

										if (range == openRange + 1)
										{
											elementCount++;
										}
									}
									else if (templine.find("\t\t\t#") != string::npos)
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

								if (curLine.find("\t\t\t#") != string::npos && (curLine.find("#" + format + "$") != string::npos || curLine.find("#" + format + "_group$") != string::npos || curLine.find("#" + format + "_master") != string::npos))
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

										if (ID.find("#" + format + "$") != string::npos && multiOption == format)
										{
											nextpos = curLine.find("#" + format + "$", nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = format + "$" + curID;

											if (tempID.find(curID, 0) != string::npos && nextpos == curLine.find(curID))
											{
												if (subFunctionIDs[animMulti][curID].length() == 0)
												{
													cout << "ERROR(2102): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "ID: " << curID << endl << endl;
													return empty;
												}

												curLine.replace(nextpos, curID.length(), subFunctionIDs[animMulti][curID]);
											}
											else
											{
												cout << "ERROR(2104): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << linecount << endl << "ID: " << curID << endl << endl;
												return empty;
											}
										}
										else if (ID.find("#" + format + "_group") != string::npos && multiOption == format + "_group")
										{
											nextpos = curLine.find("#" + multiOption, nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = multiOption + "$" + curID;

											if (tempID.find(curID, 0) != string::npos && nextpos == curLine.find(curID))
											{
												for (unsigned int k = 0; k < subFunctionIDs.size(); k++) // number of variation
												{
													if (subFunctionIDs[k][curID].length() == 0)
													{
														cout << "ERROR(2102): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "ID: " << curID << endl << endl;
														return empty;
													}

													curLine.replace(nextpos, curID.length(), subFunctionIDs[k][curID]);
												}
											}
											else
											{
												cout << "ERROR(2104): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << linecount << endl << "ID: " << curID << endl << endl;
												return empty;
											}

										}
										else if (ID.find("#" + format + "_master") != string::npos && multiOption == format + "_master")
										{
											nextpos = curLine.find("#" + multiOption, nextpos) + 1;
											string tempID = curLine.substr(nextpos);
											string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
											curID = multiOption + "$" + curID;

											if (tempID.find(curID, 0) != string::npos && nextpos == curLine.find(curID))
											{
												for (unsigned int k = 0; k < subFunctionIDs.size(); k++) // number of variation
												{
													if (subFunctionIDs[k][curID].length() == 0)
													{
														cout << "ERROR(2102): Unknown ID found in template. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "ID: " << curID << endl << endl;
														return empty;
													}

													curLine.replace(nextpos, curID.length(), subFunctionIDs[k][curID]);
												}
											}
											else
											{
												cout << "ERROR(2104): Incomplete ID found in template. Please contact the template creator " << endl << "Template: " << format << endl << "Line: " << linecount << endl << "ID: " << curID << endl << endl;
												return empty;
											}

										}
									}
								}

								if (curLine.find("%") != string::npos)
								{
									__int64 counter = count(curLine.begin(), curLine.end(), '%');

									for (int p = 0; p < counter; ++p)
									{
										curLine.replace(curLine.find("%"), 1, to_string(l));
									}
								}

								// multi choice selection
								if (curLine.find("$MC$", 0) != string::npos)
								{
									multiChoice(curLine, groupOptionPicked, groupAnimInfo, int(linecount), format, format);

									if (error)
									{
										return empty;
									}
								}

								// set animation ID
								if (curLine.find("$%$", 0) != string::npos)
								{
									curLine.replace(curLine.find("$%$"), 3, "0");
								}

								if (curLine.find("$") != string::npos && !hasGroup)
								{
									processing(curLine, int(i + 1 - tempstore.size() + l + (optionMulti * animMulti * tempstore.size())), subFunctionIDs, groupAnimInfo, eventid, variableid, optionMulti, animMulti, multiOption);

									if (error)
									{
										return empty;
									}
								}

								if (tempstore[l].find("<!-- NEW ^" + format, 0) == string::npos && tempstore[l].find("<!-- CLOSE -->", 0) == string::npos)
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
		else if (line.find("<!-- CONDITION END -->", 0) != string::npos)
		{
			if (condition == 0)
			{
				cout << "ERROR(1119): Unable to close condition. No opened condition is found. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << i + 1 << endl << endl;
				error = true;
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

	for (auto it = IsConditionOpened.begin(); it != IsConditionOpened.end(); ++it)
	{
		if (it->second)
		{
			cout << "ERROR(1108): Unresolved option section. Closing of option is required. Please contact the template creator" << endl << "Template: " << format << endl << endl;
			error = true;
			return empty;
		}
	}

	if (newFunctionLines.back().length() != 0)
	{
		newFunctionLines.push_back("");
	}

	newFunctionLines.shrink_to_fit();	
	return newFunctionLines;
}

string groupTemplate::stateReplacer(string curline, int stateID, int linecount, int groupCount)
{
	string line = curline;
	int count = sameWordCount(line, "$(S+");

	for (int i = 0; i < count; ++i)
	{
		string number = boost::regex_replace(string(line.substr(line.find("$(S+"))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
		string state = "$(S+" + number + ")$";

		if (line.find(state, 0) != string::npos)
		{
			size_t stateposition = line.find(state, 0);

			if (state == line.substr(stateposition, line.find(")$", stateposition) - stateposition + 2))
			{
				size_t stateLength = state.length();
				state = state.substr(1, state.length() - 2);
				state.replace(1, 1, to_string(stateID));
				calculate(state, error);
				functionIDs[format + "[" + to_string(groupCount) + "][(S+" + number + ")]"] = state;
				line.replace(stateposition, stateLength, state);

				if (stoi(state) >= (*nextStateID))
				{
					(*nextStateID) = stoi(state) + 1;
				}
			}
			else
			{
				cout << "ERROR(2108): Invalid state. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "State: " << state << endl << endl;
				return "";
			}
		}
	}

	return line;
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
			if (change.find("(") != string::npos && change.find("L", change.find("(")) != string::npos && change.find(")", change.find("(")) != string::npos)
			{
				__int64 maths = count(change.begin(), change.end(), '(');

				if (maths != 0 && maths == count(change.begin(), change.end(), ')'))
				{
					size_t nextpos = change.find("(");

					for (__int64 j = 0; j < maths; ++j)
					{
						string equation = change.substr(nextpos, change.find(")", nextpos) - 1);
						string number = boost::regex_replace(string(equation), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (equation != "(S+" + number + ")")
						{
							size_t equationLength = equation.length();

							if (equation.find("L") != string::npos)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'L');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("L"), 1, to_string(groupAnimInfo.size() - 1));
								}
							}

							calculate(equation, error);

							if (stoi(equation) > groupAnimInfo.size() - 1 || stoi(equation) < 0)
							{
								cout << "ERROR(1155): Invalid order number. Enter number from 0 to " << groupAnimInfo.size() - 1 << ". Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "Option: " << change << endl << endl;
								error = true;
								return;
							}

							change.replace(nextpos, equationLength, equation);
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			if (change.find(masterFormat + "[][", 0) != string::npos && change.find("][", change.find(masterFormat + "[][")) != string::npos)
			{
				formatReplace(change, masterFormat, subFunctionIDs[animMulti], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find(masterFormat + "[F][", 0) != string::npos && change.find("][", change.find(masterFormat + "[F][")) != string::npos)
			{
				formatReplace(change, masterFormat, subFunctionIDs[0], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find(masterFormat + "[L][", 0) != string::npos && change.find("][", change.find(masterFormat + "[L][")) != string::npos)
			{
				formatReplace(change, masterFormat, subFunctionIDs[groupAnimInfo.size() - 1], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find(masterFormat + "_group[", 0) != string::npos && change.find("]", change.find(masterFormat + "_group[")) != string::npos)
			{
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find("eventID[", 0) != string::npos &&  change.find("]", 0) != string::npos)
			{
				eventIDReplacer(change, format, eventid, zeroEvent, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find("variableID[", 0) != string::npos &&  change.find("]", 0) != string::npos)
			{
				variableIDReplacer(change, format, variableid, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find("import[", 0) != string::npos && change.find("]", 0) != string::npos)
			{
				size_t nextpos = change.find("import[");
				string importer = change.substr(nextpos, change.find("]", change.find("]") + 1) - nextpos + 1);
				__int64 bracketCount = count(importer.begin(), importer.end(), '[');
				__int64 altBracketCount = count(importer.begin(), importer.end(), ']');

				if (IDExist[importer].length() == 0)
				{
					if (bracketCount == 2)
					{
						if (bracketCount == altBracketCount)
						{
							size_t pos = importer.find("[") + 1;
							string file = importer.substr(pos, importer.find("]", pos) - pos);
							pos = importer.find("[", pos) + 1;
							string keyword = importer.substr(pos, importer.find("]", pos) - pos);

							string tempID;

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
					}
					else
					{
						cout << "ERROR(2109): Invalid import input. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
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

void ExistingFunction::processing(string& line, int linecount, vector<SSMap> subFunctionIDs, vector<shared_ptr<animationInfo>> groupAnimInfo, id eventid, id variableid, int optionMulti, int animMulti, string multiOption)
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
			if (change.find("(") != string::npos && change.find("L", change.find("(")) != string::npos && change.find(")", change.find("(")) != string::npos)
			{
				__int64 maths = count(change.begin(), change.end(), '(');

				if (maths != 0 && maths == count(change.begin(), change.end(), ')'))
				{
					size_t nextpos = change.find("(");

					for (__int64 j = 0; j < maths; ++j)
					{
						string equation = change.substr(nextpos, change.find(")", nextpos) - 1);
						string number = boost::regex_replace(string(equation), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (equation != "(S+" + number + ")")
						{
							size_t equationLength = equation.length();

							if (equation.find("L") != string::npos)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'L');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("L"), 1, to_string(groupAnimInfo.size() - 1));
								}
							}

							calculate(equation, error);

							if (stoi(equation) > groupAnimInfo.size() - 1 || stoi(equation) < 0)
							{
								cout << "ERROR(1148): \"Minimum\" in option_list.txt must be used and contain larger value than the 1st element being used. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "Option: " << change << endl << endl;
								error = true;
								return;
							}

							change.replace(nextpos, equationLength, equation);
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			if (change.find("[File]") != string::npos)
			{
				if (change.find(format + "[][File]", 0) != string::npos)
				{
					cout << "ERROR(1052): Invalid element. Only \"F\" or \"L\" is acceptable for the 1st element. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
					error = true;
					return;
				}

				if (change.find(format + "[F][File]", 0) != string::npos)
				{
					change.replace(change.find(format + "[F][File]"), 9 + format.length(), subFunctionIDs[0][format + "File"]);
				}

				if (change.find(format + "[N][File]", 0) != string::npos)
				{
					cout << "ERROR(1056): Invalid elmenent. Only \"F\" \"L\", or number is acceptable for the 1st element. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
					error = true;
					return;
				}

				if (change.find(format + "[L][File]", 0) != string::npos)
				{
					change.replace(change.find(format + "[L][File]"), 9 + format.length(), subFunctionIDs[groupAnimInfo.size() - 1][format + "File"]);
				}

				if (change.find(format + "[", 0) != string::npos)
				{
					string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (change.find(format + "[" + number + "][File]", 0) != string::npos)
					{
						if (unsigned int(stoi(number)) >= groupAnimInfo.size())
						{
							change.replace(change.find(format + "[" + number + "][File]"), 8 + format.length() + number.length(), subFunctionIDs[stoi(number)][format + "File"]);
						}
						else
						{
							cout << "ERROR(1148): \"Minimum\" in option_list.txt must be used and contain larger value than the 1st element being used. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "Option: " << change << endl << endl;
							error = true;
							return;
						}
					}
				}
			}

			if (change.find(format + "[][", 0) != string::npos && change.find("]", change.find(format + "[][")) != string::npos)
			{
				formatReplace(change, format, subFunctionIDs[animMulti], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				cout << "ERROR(2111): Unknown reference. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
				return;
			}

			if (change.find(format + "[F][", 0) != string::npos && change.find("]", change.find(format + "[F][")) != string::npos)
			{
				formatReplace(change, format, subFunctionIDs[0], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				cout << "ERROR(2111): Unknown reference. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
				return;
			}

			if (change.find(format + "[L][", 0) != string::npos && change.find("]", change.find(format + "[L][")) != string::npos)
			{
				formatReplace(change, format, subFunctionIDs[groupAnimInfo.size() - 1], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				cout << "ERROR(2111): Unknown reference. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
				return;
			}

			if (change.find(format + "_group[][", 0) != string::npos && change.find("]", change.find(format + "_group[][")) != string::npos)
			{
				formatReplace(change, format + "_group", subFunctionIDs[animMulti], groupAnimInfo, linecount, groupAnimInfo.size() - 1, optionMulti, animMulti);
				isChange = true;
			}

			if (error)
			{
				cout << "ERROR(2111): Unknown reference. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
				return;
			}

			if (change.find("eventID[", 0) != string::npos && change.find("]", 0) != string::npos)
			{
				eventIDReplacer(change, format, eventid, zeroEvent, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find("variableID[", 0) != string::npos && change.find("]", 0) != string::npos)
			{
				variableIDReplacer(change, format, variableid, linecount);
				isChange = true;
			}

			if (error)
			{
				return;
			}

			if (change.find("import[", 0) != string::npos && change.find("]", 0) != string::npos)
			{
				size_t nextpos = change.find("import[");
				string importer = change.substr(nextpos, change.find("]", change.find("]") + 1) - nextpos + 1);
				__int64 bracketCount = count(importer.begin(), importer.end(), '[');
				__int64 altBracketCount = count(importer.begin(), importer.end(), ']');

				if (IDExist[importer].length() == 0)
				{
					if (bracketCount == 2)
					{
						if (bracketCount == altBracketCount)
						{
							size_t pos = importer.find("[") + 1;
							string file = importer.substr(pos, importer.find("]", pos) - pos);
							pos = importer.find("[", pos) + 1;
							string keyword = importer.substr(pos, importer.find("]", pos) - pos);

							string tempID;

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
					}
					else
					{
						cout << "ERROR(2109): Invalid import input. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
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

void multiChoice(string& line, vector<unordered_map<string, bool>> groupOptionPicked, vector<shared_ptr<animationInfo>> groupAnimInfo, int numline, string templatename, string masterformat)
{
	if (line.find("<!-- ", 0) != string::npos)
	{
		size_t nextposition = 0;
		int choicecount = 0;

		while (true)
		{
			if (line.find("<!--", nextposition) != string::npos)
			{
				choicecount++;
				nextposition = line.find("<!-- ", nextposition) + 1;
			}
			else
			{
				break;
			}
		}

		if (line.find("*", 0) != string::npos || line.find("@", 0) != string::npos || line.find("%", 0) != string::npos)
		{
			cout << "ERROR(2110): Invalid character input detected" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << endl;
			error = true;
			return;
		}

		bool none = true;
		nextposition = 0;

		for (int i = 0; i < choicecount; i++)
		{
			vecstr opt;
			vector<char> storechar;
			nextposition = line.find("<!-- ", nextposition) + 5;
			string tempstr = line.substr(nextposition, line.find(" ", nextposition) - nextposition);

			if (line.find("<!-- " + tempstr + " -->", 0) == string::npos)
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
				cout << "ERROR(2112): Invalid template condition" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << endl;
				error = true;
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

		if (x == -1 && y == -1)
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
		else if (x == -1 || (x > y && y != -1))
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
		else if (y == -1 || (x < y && x != -1))
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

			if ((x == -1 || x > y) && y != -1)
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
			cout << "ERROR(1105): Invalid template condition" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << endl;
			error = true;
			return false;
		}

		string outHouse = condition.substr(backB + 1);

		x = outHouse.find("&");
		y = outHouse.find("|");

		if (x == -1 && y == -1)
		{
			return inHouseResult;
		}
		else if (x == -1 || (x > y && y != -1))
		{
			string secondCondition = outHouse.substr(outHouse.find("|") + 1);

			if (inHouseResult || newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
			{
				return true;
			}
		}
		else if (y == -1 || (x < y && x != -1))
		{
			string secondCondition = inHouse.substr(inHouse.find("&") + 1);

			if (inHouseResult && newCondition(secondCondition, curOptionPicked, groupAnimInfo, numline, templatename, masterformat))
			{
				return true;
			}
		}
		else
		{
			cout << "ERROR(1106): Invalid template condition" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << "Condition: " << condition << endl << endl;
			error = true;
			return false;
		}
	}
	else
	{
		size_t x = condition.find("&");
		size_t y = condition.find("|");

		if (x == -1 && y == -1)
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
		else if (x == -1 || (x > y && y != -1))
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
		else if (y == -1 || (x < y && x != -1))
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

			if (y != -1)
			{
				if (secondCondition[0] == '(')
				{
					int position;
					int openCounter = 0;

					for (unsigned int i = 0; i < secondCondition.size(); i++)
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
							cout << "ERROR(1101): Invalid template condition" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << endl;
							error = true;
							return false;
						}
					}
				}
				else
				{
					x = secondCondition.find("&");
					y = secondCondition.find("|");

					if (x == -1 && y == -1)
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
					else if (x == -1 || (x > y && y != -1))
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
					else if (y == -1 || (x < y && x != -1))
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
						cout << "ERROR(1103): Invalid template condition" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << endl;
						error = true;
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
			cout << "ERROR(1102): Invalid template condition" << endl << "Template: " << templatename << endl << "Line: " << numline << endl << endl;
			error = true;
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

	for (unsigned int i = 1; i < groupline.size(); ++i)
	{
		subFunction = subFunction + "[" + groupline[i] + "]";
	}

	if (subFunctionIDs[subFunction].length() == 0 || groupline.size() < 3)
	{
		cout << "ERROR(1158): Invalid function call. Please contact the template creator" << endl << "Template: " << format << endl << "Line: " << linecount << endl << "Function call: " << curline << endl << endl;
		error = true;
		return;
	}

	curline.replace(curline.find(originalLine), originalLine.length(), subFunctionIDs[subFunction]);
}
