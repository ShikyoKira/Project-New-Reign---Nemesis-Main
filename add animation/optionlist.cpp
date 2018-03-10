#include "optionlist.h"

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
		char line[5000];
		int linecount = 0;
		bool minDone = false;
		bool ruleDone = false;
		bool stateDone = false;
		bool compulsoryDone = false;
		unordered_set<string> matcher;
		unordered_set<string> container;
		unordered_map<string, int> matchLine;
		unordered_map<string, bool> isElementExist;

		while (fgets(line, 5000, input))
		{
			linecount++;

			if (line[0] != '`' && line[0] != '\n')
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

						if (strline.find(AnimInfo[0] + " " + first + " " + second) != string::npos)
						{
							// first rule processing
							__int64 reference = count(first.begin(), first.end(), ',');

							if (reference != 0)
							{
								size_t nextpos = 0;
								size_t pos = 0;

								for (int i = 0; i < reference + 1; i++)
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

								for (int i = 0; i < reference + 1; i++)
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
							cout << "ERROR(1029): Invalid rule. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
							return;
						}

						ruleDone = true;
					}
					else
					{
						cout << "ERROR(1040): Rule has already been registered. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << "Line: " << linecount << endl << endl;
						error = true;
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
							cout << "ERROR(1013): Invalid state characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
							return;
						}

						stateDone = true;
					}
					else
					{
						cout << "ERROR(1041): State has already been registered. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
				}
				else if (lower == "minimum")
				{
					if (!minDone)
					{
						string number = boost::regex_replace(string(AnimInfo[1]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (strline.find(AnimInfo[0] + " " + number) != string::npos)
						{
							groupMin = stoi(number);
						}

						minDone = true;
					}
					else
					{
						cout << "ERROR(1042): Minimum condition has already been registered. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
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
									cout << "ERROR(1046): " << currentTab << " has already been linked with other option. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << "Option: " << currentTab << endl << endl;
									error = true;
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
							cout << "ERROR(1036): Invalid animation event characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
							return;
						}
					}

					string templine = AnimInfo[1];
					
					if (isElementExist[templine])
					{
						cout << "ERROR(1063): Duplicated " << templine << " registration detected. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << "Event: " << templine << endl << endl;
						error = true;
						return;
					}

					isElementExist[templine] = true;

					if (templine.find("<") != string::npos && templine.find(">", templine.find("<")) != string::npos)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(10);

							for (int i = 0; i < opening; i++)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == -1)
								{
									cout << "ERROR(1037): Invalid animation event characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									error = true;
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
							cout << "ERROR(1039): Missing closing bracket for event/variable. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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
							cout << "ERROR(1036): Invalid animation event characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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
						cout << "ERROR(1048): Invalid group order. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}

					string templine = AnimInfo[2];

					if (isElementExist[templine])
					{
						cout << "ERROR(1063): Duplicated " << templine << " registration detected. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << "Event: " << templine << endl << endl;
						error = true;
						return;
					}

					if (templine.find("<") != string::npos && templine.find(">", templine.find("<")) != string::npos)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(opening);

							for (int i = 0; i < opening; i++)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == -1)
								{
									cout << "ERROR(1037): Invalid animation event characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									error = true;
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
							cout << "ERROR(1039): Missing closing bracket for event/variable. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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
							cout << "ERROR(1043): Invalid variable characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
							return;
						}
					}

					string templine = AnimInfo[1];

					if (templine.find("<") != string::npos && templine.find(">", templine.find("<")) != string::npos)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(10);

							for (int i = 0; i < opening; i++)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == -1)
								{
									cout << "ERROR(1037): Invalid animation event characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									error = true;
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
							cout << "ERROR(1039): Missing closing bracket for event/variable. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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
							cout << "ERROR(1043): Invalid variable characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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
						cout << "ERROR(1048): Invalid group order. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}

					string templine = AnimInfo[2];

					if (templine.find("<") != string::npos && templine.find(">", templine.find("<")) != string::npos)
					{
						__int64 opening = count(strline.begin(), strline.end(), '<');
						__int64 closing = count(strline.begin(), strline.end(), '>');

						if (opening == closing)
						{
							size_t nextpos = 0;
							vecstr import;
							import.reserve(10);

							for (int i = 0; i < opening; i++)
							{
								nextpos = templine.find("<", nextpos) + 1;

								if (nextpos == -1)
								{
									cout << "ERROR(1037): Invalid animation event characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									error = true;
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
							cout << "ERROR(1039): Missing closing bracket for event/variable. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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
							cout << "ERROR(1044): Invalid option characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
							error = true;
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

						for (int i = 0; i < reference + 1; i++)
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
						cout << "ERROR(1045): Compulsory options have already been registered. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
				}
				else
				{
					size_t lineplus = boost::regex_replace(string(strline), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1")).length();

					if (lineplus < 5 && lineplus > 0)
					{
						string tempOption = strline.substr(0, lineplus);

						if (strline.find("<", lineplus) != string::npos && strline.find(">", lineplus) != string::npos)
						{
							if (boost::iequals(tempOption, "animobject") || boost::iequals(tempOption, "end"))
							{
								cout << "ERROR(1049): " << tempOption << " cannot be used as it has already been registered by default. Use other name for your option. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line : " << linecount << endl << endl;
								error = true;
								return;
							}

							if (tempOption != "o")
							{
								if (boost::iequals(tempOption, format))
								{
									cout << "ERROR(1064): Invalid option name. The option name must not share the same name as the format of the animation. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									error = true;
									return;
								}

								storelist[tempOption] = true;
								optionOrder.push_back(tempOption);
							}
							else
							{
								cout << "ERROR(1049): " << tempOption << " cannot be used as it has already been registered by default. Use other name for your option. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line : " << linecount << endl << endl;
								error = true;
								return;
							}

							__int64 opening = count(strline.begin(), strline.end(), '<');
							__int64 closing = count(strline.begin(), strline.end(), '>');

							if (opening == closing)
							{
								addOn[tempOption].reserve(opening);
								joint[tempOption].reserve(opening - 1);
								size_t nextpos = 0;

								for (int i = 0; i < opening; i++)
								{
									nextpos = strline.find("<", nextpos) + 1;
									string addition = strline.substr(nextpos, strline.find(">", nextpos) - nextpos);
									string tempAddOn = boost::regex_replace(string(addition), boost::regex("[^A-Za-z\\s]*([A-Za-z\\s]+).*"), string("\\1"));

									if (tempAddOn != addition)
									{
										cout << "ERROR(1015): Add-on contain invalid characters, only alphabet is accepted. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
										error = true;
										return;
									}
									else
									{
										if (Debug && isElementExist[addition] && addition != "variable" && addition != "event")
										{
											cout << "WARNING: Duplicate of element detected. This is not an error message. Program will still function as normal" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
										}

										isElementExist[addition] = true;
										addOn[tempOption].push_back(addition);
										container.insert(addition);
										isAddOn[tempOption] = true;
									}

									if (i != opening - 1)
									{
										size_t pos = strline.find(">", nextpos) + 1;
										string jointStr = strline.substr(pos, strline.find("<", pos) - pos);
										joint[tempOption].push_back(jointStr);
									}
								}

								if (isAddOn[tempOption] && line[tempOption.length()] == '[' && line[tempOption.length() + 1] == ']')
								{
									groupOption[tempOption] = true;

									// cout << "ERROR(1022): Option array and add-on cannot be used together. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									// error = true;
									// return;
								}

							}
							else
							{
								cout << "ERROR(1014): Missing closing bracket for add-on. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
								error = true;
								return;
							}
						}
						else
						{
							bool number = false;

							for (unsigned int i = 0; i < strlen(line); i++)
							{
								if (!isalpha(line[i]) && !isalnum(line[i]) && line[i] != '[' && line[i] != ']')
								{
									cout << "ERROR(1012): Option contain invalid characters, only alphabet is accepted. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
									error = true;
									return;
								}
							}
							
							if (boost::iequals(tempOption, "animobject") || tempOption == "D")
							{
								cout << "ERROR(1049): " << tempOption << " cannot be used as it has already been registered by default. Use other name for your option. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line : " << linecount << endl << endl;
								error = true;
								return;
							}

							if (boost::iequals(tempOption, format))
							{
								cout << "ERROR(1064): Invalid option name. The option name must not share the same name as the format of the animation. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
								error = true;
								return;
							}

							storelist[tempOption] = true;
							optionOrder.push_back(tempOption);

							if (line[tempOption.length()] == '[' && line[tempOption.length() + 1] == ']')
							{
								cout << "ERROR(1022): Option array can only be used with add-on. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
								error = true;
								return;
							}
						}
					}
					else
					{
						cout << "ERROR(1011): Option contain too many characters. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
				}
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
				cout << "ERROR(1038): Reference not found. Element must be created/existed before being referred. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Line: " << matchLine[*it] << endl << "Element: " << *it << endl << endl;
				error = true;
				return;
			}
		}
		
		fclose(input);
	}
	else
	{
		cout << "ERROR(1010): Unable to open format file. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << endl;
		error = true;
		return;
	}

	if (eleEvent.size() != eleEventLine.size() || eleEventGroupF.size() != eleEventGroupFLine.size() || eleEventGroupL.size() != eleEventGroupLLine.size() || eleVar.size() != eleVarLine.size() || eleVarGroupF.size() != eleVarGroupFLine.size() || eleVarGroupL.size() != eleVarGroupLLine.size())
	{
		cout << ">> ERROR(1047): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
		error = true;
		return;
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

			bool isOptionGroup = groupOption[it->first];
			mixOptRegis[it->first] = mixedOption;
			mixOptRever[mixedOption].push_back(it->first);

			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (isOptionGroup != groupOption[it->second[i]])
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

				cout << "ERROR(1062): Different element detected across linked options. All linked options must have the same elements including grouping/non-grouping. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << "Elements: " << errorElements << endl << endl;
				return;
			}

		}
	}

	// linked option existence and add on presence
	// for (auto it = linked.begin(); it != linked.end(); ++it)
	{
	//	bool isMatched = false;

	//	for (auto iter = storelist.begin(); iter != storelist.end(); ++iter)
		{
	//		if (it->first == iter->first)
			{
	//			for (auto iterator = addOn.begin(); iterator != addOn.end(); ++iterator)
				{
	//				if (iter->first == iterator->first)
					{
	//					isMatched = true;
	//					break;
					}
				}

	//			if (isMatched)
				{
	//				break;
				}
			}
		}

	//	if (isMatched)
		{
	//		break;
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
							cout << "ERROR(1053): Unable link options containing different number of elements or name of the element. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << endl;
							error = true;
							return;
						}
					}

					isDone[it->second[i]] = true;
					isDone[it->first] = true;
				}
				else
				{
					cout << "ERROR(1053): Unable link options containing different number of elements or name of the element. Please contact the template creator" << endl << "Template: " << format << endl << "File: " << filepath << endl << endl;
					error = true;
					return;
				}
			}
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
					cout << "WARNING: Similar option's name is not encouraged unless you know what you are doing. Priority matching will be ensued. Program will function as normal" << endl << "Option 1: " << it->first << endl << "Option 2: " << iter->first << endl << "File: " << filepath << endl << endl;
					cout << "This is not an error message!! This warning message is mainly for template creator. Users do not need to report it." << endl << endl;
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

	for (unsigned int i = 0; i < shorter.size(); i++)
	{
		if (shorter[i] != longer[i])
		{
			return false;
		}
	}

	return true;
}
