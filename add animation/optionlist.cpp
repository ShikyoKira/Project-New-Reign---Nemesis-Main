#include "optionlist.h"

#pragma warning(disable:4503)

using namespace std;

bool Debug = false;

OptionList::OptionList()
{

}

OptionList::OptionList(string filepath, string format)
{
	templatecode = format;
	unordered_map<string, bool> isAddOn;
	unordered_map<string, vecstr> linked;
	FILE* input;
	fopen_s(&input, filepath.c_str(), "r");

	if (input)
	{
		char line[2000];
		int linecount = 0;
		bool minDone = false;
		bool ruleDone = false;
		bool stateDone = false;
		bool compulsoryDone = false;
		unordered_set<string> matcher;
		unordered_set<string> container;
		unordered_map<string, int> matchLine;
		unordered_map<string, bool> isElementExist;
		unordered_map<int, bool> isNumExist;

		while (fgets(line, 2000, input))
		{
			++linecount;

			if (line[0] != '\'' && line[0] != '\n')
			{
				if (line[strlen(line) - 1] == '\n')
				{
					line[strlen(line) - 1] = '\0';
				}

				string strline(line);
				stringstream sstream(line);
				istream_iterator<string> ssbegin(sstream);
				istream_iterator<string> ssend;
				vecstr AnimInfo(ssbegin, ssend);
				copy(AnimInfo.begin(), AnimInfo.end(), AnimInfo.begin());

				string lower = boost::algorithm::to_lower_copy(AnimInfo[0]);

				if (lower == "rules")
				{
					if (!ruleDone)
					{
						string first = AnimInfo[1];
						string second = AnimInfo[2];

						if (strline.find(AnimInfo[0] + " " + first + " " + second) != NOT_FOUND)
						{
							// first rule processing
							__int64 reference = count(first.begin(), first.end(), ',');

							if (reference != 0)
							{
								size_t nextpos = 0;
								size_t pos = 0;

								for (int i = 0; i < reference + 1; ++i)
								{
									if (i != reference)
									{
										nextpos = first.find(",", nextpos + 1);
										ruleOne.push_back(first.substr(pos, nextpos - pos));
										pos = nextpos + 1;
									}
									else
									{
										ruleOne.push_back(first.substr(pos));
									}
								}
							}
							else if (first != "/")
							{
								ruleOne.push_back(first);
							}

							// second rule processing
							reference = count(second.begin(), second.end(), ',');

							if (reference != 0)
							{
								size_t nextpos = 0;
								size_t pos = 0;

								for (int i = 0; i < reference + 1; ++i)
								{
									if (i != reference)
									{
										nextpos = second.find(",", nextpos + 1);
										ruleTwo.push_back(second.substr(pos, nextpos - pos));
										pos = nextpos + 1;
									}
									else
									{
										ruleTwo.push_back(second.substr(pos));
									}
								}
							}
							else if (second != "/")
							{
								ruleTwo.push_back(second);
							}
						}
						else
						{
							ErrorMessage(1029, format, filepath, linecount);
							fclose(input);
							return;
						}

						ruleDone = true;
					}
					else
					{
						ErrorMessage(1040, format, filepath, linecount);
						fclose(input);
						return;
					}
				}
				else if (lower == "state")
				{
					if (!stateDone)
					{
						string startState = AnimInfo[1];
						string filteredStartState = boost::regex_replace(string(startState), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (startState == filteredStartState)
						{
							startStateID = filteredStartState;
						}
						else
						{
							ErrorMessage(1013, format, filepath, linecount);
							fclose(input);
							return;
						}

						stateDone = true;
					}
					else
					{
						ErrorMessage(1041, format, filepath, linecount);
						fclose(input);
						return;
					}
				}
				else if (lower == "minimum")
				{
					if (!minDone)
					{
						string number = boost::regex_replace(string(AnimInfo[1]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (strline.find(AnimInfo[0] + " " + number) != NOT_FOUND)
						{
							groupMin = stoi(number);
						}

						minDone = true;
					}
					else
					{
						ErrorMessage(1042, format, filepath, linecount);
						fclose(input);
						return;
					}
				}
				else if (lower == "ignore_group")
				{
					ignoreGroup = true;
				}
				else if (lower == "link")
				{
					for (unsigned int i = 1; i < AnimInfo.size(); ++i)
					{
						string currentTab;
						vecstr linker;

						for (unsigned int j = 1; j < AnimInfo.size(); ++j)
						{
							if (i == j)
							{
								currentTab = AnimInfo[j];

								if (linked[currentTab].size() != 0)
								{
									ErrorMessage(1046, currentTab, format, filepath, linecount);
									fclose(input);
									return;
								}
							}
							else
							{
								linker.push_back(AnimInfo[j]);
							}
						}

						linked[currentTab] = linker;
					}
				}
				else if (lower == "event")
				{
					for (unsigned int i = 0; i < strlen(line); ++i)
					{
						if (!isalnum(line[i]) && line[i] != '_' && line[i] != '<' && line[i] != '>' && line[i] != ' ')
						{
							ErrorMessage(1036, format, filepath, linecount);
							fclose(input);
							return;
						}
					}

					string templine = AnimInfo[1];
					
					if (isElementExist[templine])
					{
						ErrorMessage(1063, templine, format, filepath, linecount, templine);
						fclose(input);
						return;
					}

					isElementExist[templine] = true;

					if (templine.find("<") != NOT_FOUND && templine.find(">", templine.find("<")) != NOT_FOUND)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(10);

							for (int i = 0; i < opening; ++i)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == NOT_FOUND)
								{
									ErrorMessage(1037, format, filepath, linecount);
									fclose(input);
									return;
								}

								string element = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);

								if (!boost::iequals(element, "main_anim_event"))
								{
									matcher.insert(element);
									matchLine[element] = linecount;
								}

								import.push_back(element);
								templine.replace(templine.find("<" + element + ">"), element.length() + 2, "$$");
							}

							import.shrink_to_fit();
							eleEvent.push_back(import);
							eleEventLine.push_back(templine);
						}
						else
						{
							ErrorMessage(1039, format, filepath, linecount);
							fclose(input);
							return;
						}
					}
					else
					{
						vecstr import;
						eleEvent.push_back(import);
						eleEventLine.push_back(templine);
					}
				}
				else if (lower == "event_group")
				{
					for (unsigned int i = 0; i < strlen(line); ++i)
					{
						if (!isalnum(line[i]) && line[i] != '_' && line[i] != '<' && line[i] != '>' && line[i] != ' ')
						{
							ErrorMessage(1036, format, filepath, linecount);
							fclose(input);
							return;
						}
					}

					bool isLast;

					if (AnimInfo[1] == "F")
					{
						isLast = false;
					}
					else if (AnimInfo[1] == "L")
					{
						isLast = true;
					}
					else
					{
						ErrorMessage(1048, format, filepath, linecount);
						fclose(input);
						return;
					}

					string templine = AnimInfo[2];

					if (isElementExist[templine])
					{
						ErrorMessage(1063, templine, format, filepath, linecount);
						fclose(input);
						return;
					}

					if (templine.find("<") != NOT_FOUND && templine.find(">", templine.find("<")) != NOT_FOUND)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(int(opening));

							for (int i = 0; i < opening; ++i)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == NOT_FOUND)
								{
									ErrorMessage(1037, format, filepath, linecount);
									fclose(input);
									return;
								}

								string element = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);

								if (!boost::iequals(element, "main_anim_event"))
								{
									matcher.insert(element);
									matchLine[element] = linecount;
								}

								import.push_back(element);
								templine.replace(templine.find("<" + element + ">"), element.length() + 2, "$$");
							}
							
							if (isLast)
							{
								eleEventGroupL.push_back(import);
								eleEventGroupLLine.push_back(templine);
							}
							else
							{
								eleEventGroupF.push_back(import);
								eleEventGroupFLine.push_back(templine);
							}
						}
						else
						{
							ErrorMessage(1039, format, filepath, linecount);
							fclose(input);
							return;
						}
					}
					else
					{
						vecstr import;

						if (isLast)
						{
							eleEventGroupL.push_back(import);
							eleEventGroupLLine.push_back(templine);
						}
						else
						{
							eleEventGroupF.push_back(import);
							eleEventGroupFLine.push_back(templine);
						}
					}
				}
				else if (lower == "variable")
				{
					for (unsigned int i = 0; i < strlen(line); ++i)
					{
						if (!isalnum(line[i]) && line[i] != '_' && line[i] != '<' && line[i] != '>' && line[i] != ' ')
						{
							ErrorMessage(1043, format, filepath, linecount);
							fclose(input);
							return;
						}
					}

					string templine = AnimInfo[1];

					if (templine.find("<") != NOT_FOUND && templine.find(">", templine.find("<")) != NOT_FOUND)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(10);

							for (int i = 0; i < opening; ++i)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == NOT_FOUND)
								{
									ErrorMessage(1037, format, filepath, linecount);
									fclose(input);
									return;
								}

								string element = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);
								matcher.insert(element);
								matchLine[element] = linecount;
								import.push_back(element);
								templine.replace(templine.find("<" + element + ">"), element.length() + 2, "$$");
							}

							import.shrink_to_fit();
							eleVar.push_back(import);
							eleVarLine.push_back(templine);
						}
						else
						{
							ErrorMessage(1039, format, filepath, linecount);
							fclose(input);
							return;
						}
					}
					else
					{
						vecstr import;
						eleVar.push_back(import);
						eleVarLine.push_back(templine);
					}
				}
				else if (lower == "variable_group")
				{
					for (unsigned int i = 0; i < strlen(line); ++i)
					{
						if (!isalnum(line[i]) && line[i] != '_' && line[i] != '<' && line[i] != '>' && line[i] != ' ')
						{
							ErrorMessage(1043, format, filepath, linecount);
							fclose(input);
							return;
						}
					}

					bool isLast;

					if (AnimInfo[1] == "F")
					{
						isLast = false;
					}
					else if (AnimInfo[1] == "L")
					{
						isLast = true;
					}
					else
					{
						ErrorMessage(1048, format, filepath, linecount);
						fclose(input);
						return;
					}

					string templine = AnimInfo[2];

					if (templine.find("<") != NOT_FOUND && templine.find(">", templine.find("<")) != NOT_FOUND)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(10);

							for (int i = 0; i < opening; ++i)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == NOT_FOUND)
								{
									ErrorMessage(1037, format, filepath, linecount);
									fclose(input);
									return;
								}

								string element = templine.substr(nextpos, templine.find(">", nextpos) - nextpos);
								matcher.insert(element);
								matchLine[element] = linecount;
								import.push_back(element);
								templine.replace(templine.find("<" + element + ">"), element.length() + 2, "$$");
							}

							import.shrink_to_fit();

							if (isLast)
							{
								eleVarGroupL.push_back(import);
								eleVarGroupLLine.push_back(templine);
							}
							else
							{
								eleVarGroupF.push_back(import);
								eleVarGroupFLine.push_back(templine);
							}
						}
						else
						{
							ErrorMessage(1039, format, filepath, linecount);
							fclose(input);
							return;
						}
					}
					else
					{
						vecstr import;

						if (isLast)
						{
							eleVarGroupL.push_back(import);
							eleVarGroupLLine.push_back(templine);
						}
						else
						{
							eleVarGroupF.push_back(import);
							eleVarGroupFLine.push_back(templine);
						}
					}
				}
				else if (lower == "compulsory")
				{
					for (unsigned int i = 0; i < strlen(line); ++i)
					{
						if (!isalpha(line[i]))
						{
							ErrorMessage(1044, format, filepath, linecount);
							fclose(input);
							return;
						}
					}

					if (!compulsoryDone)
					{
						string rule = AnimInfo[1];

						// compulsory processing
						__int64 reference = count(rule.begin(), rule.end(), ',');

						size_t nextpos = 0;
						size_t pos = 0;

						for (int i = 0; i < reference + 1; ++i)
						{
							if (i != reference)
							{
								nextpos = rule.find(",", nextpos + 1);
								compulsory.push_back(rule.substr(pos, nextpos - pos));
								pos = nextpos + 1;
							}
							else
							{
								compulsory.push_back(rule.substr(pos));
							}
						}

						compulsoryDone = true;
					}
					else
					{
						ErrorMessage(1045, format, filepath, linecount);
						fclose(input);
						return;
					}
				}
				else if (lower == "core")
				{
					core = true;
				}
				else if (lower == "add")				// Add <option> <addon> <modifier>
				{
					if (AnimInfo[3].find("$$$") == NOT_FOUND)
					{
						ErrorMessage(1006, format, filepath, linecount, strline);
						fclose(input);
						return;
					}

					if (AnimInfo[3] == "$$$")
					{
						ErrorMessage(1005, format, filepath, linecount, strline);
						fclose(input);
						return;
					}

					modAddOn[AnimInfo[1]][AnimInfo[2]] = AnimInfo[3];
				}
				else
				{
					string ID = boost::regex_replace(string(lower), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (AnimInfo[0] == "S" + ID)
					{
						if (ID == "0")
						{
							ErrorMessage(1076, format, filepath, linecount);
							fclose(input);
							return;
						}

						if (AnimInfo.size() < 3)
						{
							ErrorMessage(1069, format, filepath, linecount);
							fclose(input);
							return;
						}
						else if (AnimInfo.size() > 3)
						{
							ErrorMessage(1070, format, filepath, linecount);
							fclose(input);
							return;
						}

						string functionID = boost::regex_replace(string(AnimInfo[2]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (AnimInfo[2] != "#" + functionID)
						{
							ErrorMessage(1071, format, filepath, linecount);
							fclose(input);
							return;
						}

						if (behaviorPath[boost::algorithm::to_lower_copy(AnimInfo[1])].length() == 0)
						{
							ErrorMessage(1083, AnimInfo[1], format, filepath, linecount);
							fclose(input);
							return;
						}

						multiState[boost::algorithm::to_lower_copy(AnimInfo[1])][stoi(ID)] = stoi(functionID);
						isNumExist[stoi(ID)] = true;
					}
					else
					{
						size_t lineplus = boost::regex_replace(string(strline), boost::regex("[<\\s]*([^<\\s]+).*"), string("\\1")).length();

						if (lineplus > 4 || lineplus < 1)
						{
							ErrorMessage(1011, format, filepath, linecount, AnimInfo[0]);
							fclose(input);
							return;
						}

						string tempOption = strline.substr(0, lineplus);
						string recontext = tempOption;

						if (boost::iequals(tempOption, "k") || boost::iequals(tempOption, "bsa"))
						{
							ErrorMessage(1049, tempOption, format, filepath, linecount);
							fclose(input);
							return;
						}

						if (strline.find("<", lineplus) != NOT_FOUND && strline.find(">", lineplus) != NOT_FOUND)
						{
							if (boost::iequals(tempOption, "animobject") || boost::iequals(tempOption, "end"))
							{
								ErrorMessage(1049, tempOption, format, filepath, linecount);
								fclose(input);
								return;
							}

							if (boost::iequals(tempOption, format))
							{
								ErrorMessage(1064, format, filepath, linecount);
								fclose(input);
								return;
							}

							storelist[tempOption] = true;
							optionOrder.push_back(tempOption);
							__int64 opening = count(strline.begin(), strline.end(), '<');
							__int64 closing = count(strline.begin(), strline.end(), '>');

							if (opening == closing)
							{
								addOn[tempOption].reserve(int(opening));
								joint[tempOption].reserve(int(opening) - 1);
								size_t nextpos = 0;

								for (int i = 0; i < opening; ++i)
								{
									nextpos = strline.find("<", nextpos) + 1;
									string addition = strline.substr(nextpos, strline.find(">", nextpos) - nextpos);
									string tempAddOn = boost::regex_replace(string(addition), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

									if (tempAddOn != addition)
									{
										ErrorMessage(1015, format, filepath, linecount);
										fclose(input);
										return;
									}
									else
									{
										if (Debug && isElementExist[addition] && addition != "variable" && addition != "event")
										{
											WarningMessage(1001, format, filepath, linecount);
										}

										isElementExist[addition] = true;
										addOn[tempOption].push_back(addition);
										container.insert(addition);
										isAddOn[tempOption] = true;
										recontext = recontext + "<" + tempAddOn + ">";
									}

									if (i != opening - 1)
									{
										size_t pos = strline.find(">", nextpos) + 1;
										string jointStr = strline.substr(pos, strline.find("<", pos) - pos);
										joint[tempOption].push_back(jointStr);
										recontext = recontext + jointStr;
									}
								}

								if (strline.length() > 2 && strline.substr(strline.length() - 2) == "[]")
								{
									groupOption[tempOption] = true;
									recontext = recontext + "[]";
								}
							}
							else
							{
								ErrorMessage(1014, format, filepath, linecount);
								fclose(input);
								return;
							}

							boost::algorithm::erase_all(strline, " ");

							if (recontext != strline)
							{
								ErrorMessage(1033, format, filepath, linecount);
								fclose(input);
								return;
							}
						}
						else
						{
							bool number = false;

							for (unsigned int i = 0; i < strlen(line); ++i)
							{
								if (!isalnum(line[i]) && line[i] != '[' && line[i] != ']')
								{
									ErrorMessage(1012, format, filepath, linecount);
									fclose(input);
									return;
								}
							}

							if (boost::iequals(tempOption, "animobject") || tempOption == "D")
							{
								ErrorMessage(1049, tempOption, format, filepath, linecount);
								fclose(input);
								return;
							}

							if (boost::iequals(tempOption, format))
							{
								ErrorMessage(1064, format, filepath, linecount);
								fclose(input);
								return;
							}

							storelist[tempOption] = true;
							optionOrder.push_back(tempOption);

							if (strline.length() > 2 && strline.substr(strline.length() - 2) == "[]")
							{
								ErrorMessage(1022, format, filepath, linecount);
								fclose(input);
								return;
							}

							boost::algorithm::erase_all(strline, " ");

							if (recontext != strline)
							{
								ErrorMessage(1033, format, filepath, linecount);
								fclose(input);
								return;
							}
						}
					}
				}
			}
		}

		fclose(input);

		for (unsigned int i = 1; i < isNumExist.size(); ++i)
		{
			if (!isNumExist[i])
			{
				ErrorMessage(1024, isNumExist.size(), format, filepath, linecount);
				return;
			}
		}

		// check if variable exist or not
		for (auto it = matcher.begin(); it != matcher.end(); ++it)
		{
			bool isMatched = false;

			for (auto iter = container.begin(); iter != container.end(); ++iter)
			{
				if (*it == *iter)
				{
					isMatched = true;
					break;
				}
			}

			if (!isMatched)
			{
				ErrorMessage(1038, format, filepath, matchLine[*it], *it);
				return;
			}
		}

		// check for validity of modAddOn
		for (auto it = modAddOn.begin(); it != modAddOn.end(); ++it)
		{
			// check if option exist
			if (isAddOn[it->first])
			{
				for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
				{
					size_t addOnSize = addOn[it->first].size();
					bool pass = false;

					for (unsigned int i = 0; i < addOnSize; ++i)
					{
						if (iter->first == addOn[it->first][i])
						{
							pass = true;
							break;
						}
					}

					if (!pass)
					{
						ErrorMessage(1007, it->first, iter->first, format, filepath, linecount);
						return;
					}
				}
			}
			else
			{
				if (storelist[it->first])
				{
					ErrorMessage(1017, it->first, format, filepath, linecount);
					return;
				}
				else
				{
					ErrorMessage(1010, format, filepath, linecount, it->first);
					return;
				}
			}
		}
	}
	else
	{
		ErrorMessage(1009, format, filepath);
		return;
	}

	if (eleEvent.size() != eleEventLine.size() || eleEventGroupF.size() != eleEventGroupFLine.size() || eleEventGroupL.size() != eleEventGroupLLine.size() || eleVar.size() != eleVarLine.size() || eleVarGroupF.size() != eleVarGroupFLine.size() || eleVarGroupL.size() != eleVarGroupLLine.size())
	{
		ErrorMessage(1047);
		return;
	}

	if (ignoreGroup)
	{
		if (eleEventGroupF.size() != 0 || eleEventGroupL.size() != 0)
		{
			ErrorMessage(1080, format, filepath);
			return;
		}

		if (eleVarGroupF.size() != 0 || eleVarGroupL.size() != 0)
		{
			ErrorMessage(1080, format, filepath);
			return;
		}
	}

	for (auto it = linked.begin(); it != linked.end(); ++it)
	{
		if (mixOptRegis[it->first].length() == 0)
		{
			string mixedOption = it->first;

			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				mixedOption = mixedOption + "&" + it->second[i];
			}

			mixOptRegis[it->first] = mixedOption;
			mixOptRever[mixedOption].push_back(it->first);

			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (groupOption[it->first] != groupOption[it->second[i]])
				{
					error = true;
				}

				if (addOn[it->first] != addOn[it->second[i]])
				{
					error = true;
				}

				mixOptRegis[it->second[i]] = mixedOption;
				mixOptRever[mixedOption].push_back(it->second[i]);
			}

			if (error)
			{
				string errorElements;
				errorElements.append(it->first);

				for (unsigned int i = 0; i < it->second.size(); ++i)
				{
					errorElements.append("," + it->second[i]);
				}

				ErrorMessage(1062, format, filepath, errorElements);
				return;
			}

		}
	}
	
	unordered_map<string, bool> isDone;

	// linked option existence element matching
	for (auto it = linked.begin(); it != linked.end(); ++it)
	{
		if (!isDone[it->first])
		{
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (addOn[it->first].size() == addOn[it->second[i]].size())
				{
					for (unsigned j = 0; j < addOn[it->first].size(); ++j)
					{
						if (addOn[it->first][j] != addOn[it->second[i]][j])
						{
							ErrorMessage(1053, format, filepath);
							return;
						}
					}

					isDone[it->second[i]] = true;
					isDone[it->first] = true;
				}
				else
				{
					ErrorMessage(1053, format, filepath);
					return;
				}
			}
		}
	}

	// throw error
	if (core)
	{
		if (ignoreGroup)
		{
			throwError(format, filepath);
			return;
		}
		else if (groupMin != -1)
		{
			throwError(format, filepath);
			return;
		}
		else if (startStateID.length() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (ruleOne.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (ruleTwo.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (compulsory.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (optionOrder.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (multiState.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (storelist.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (groupOption.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (mixOptRegis.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (mixOptRever.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (addOn.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (joint.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (eleEvent.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (eleEventGroupF.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (eleEventGroupL.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (eleVar.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (eleVarGroupF.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
		else if (eleVarGroupL.size() != 0)
		{
			throwError(format, filepath);
			return;
		}
	}

	// Warning message checking
	// Priority matching necessary checking
	if (Debug)
	{
		for (auto it = storelist.begin(); it != storelist.end(); ++it)
		{
			for (auto iter = storelist.begin(); iter != storelist.end(); ++iter)
			{
				if (it->first != iter->first && optionMatching(it->first, iter->first) && (isAddOn[it->first] || isAddOn[iter->first]))
				{
					WarningMessage(1002, it->first, iter->first, filepath);
				}
			}
		}
	}
}

void OptionList::setDebug(bool isDebug)
{
	Debug = isDebug;
}

bool optionMatching(string option1, string option2)
{
	string shorter;
	string longer;

	if (option1.length() < option2.length())
	{
		shorter = option1;
		longer = option2;
	}
	else
	{
		shorter = option2;
		longer = option1;
	}

	for (unsigned int i = 0; i < shorter.size(); ++i)
	{
		if (shorter[i] != longer[i])
		{
			return false;
		}
	}

	return true;
}

void throwError(string format, string filepath)
{
	ErrorMessage(1084, format, filepath);
}