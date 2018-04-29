#include "gettemplate.h"

using namespace std;

getTemplate::getTemplate()
{
	string templateDirectory = "behavior templates\\";
	string newpath;
	vecstr codelist;
	read_directory(templateDirectory, codelist);

	for (unsigned int k = 0; k < codelist.size(); ++k)
	{
		if (codelist[k].find(".", 0) == string::npos)
		{
			newpath = templateDirectory + codelist[k];
			boost::filesystem::path FOF(newpath);

			if (boost::iequals(codelist[k], "t") || boost::iequals(codelist[k], "aaprefix") || boost::iequals(codelist[k], "aaset") || boost::iequals(codelist[k], "md") || boost::iequals(codelist[k], "rd") || boost::iequals(codelist[k], "+"))
			{
				cout << "ERROR(3009): Unable to use " << codelist[k] << " as animation code. " << codelist[k] << " has been registered into the engine by default. Please contact the template creator" << endl << "Animation Code: " << codelist[k] << endl << endl;
				error = true;
				return;
			}

			if (boost::filesystem::is_directory(FOF))
			{
				vecstr folderlist;
				read_directory(newpath, folderlist);
				bool isOptionExist = false;
				bool registered = false;
				boost::filesystem::path animData;
				unordered_map<string, unordered_map<int, bool>> isStateJoint;		// behavior, node(function) ID, true/false; is this node(function) joining the animation template with the main branch?
				vector<boost::filesystem::path> pathVector;

				for (unsigned int l = 0; l < folderlist.size(); ++l)
				{
					newpath = templateDirectory + codelist[k] + "\\" + folderlist[l];
					boost::filesystem::path FOF2(newpath);

					if (boost::iequals(folderlist[l], "option_list.txt") && !boost::filesystem::is_directory(FOF2))
					{
						OptionList option(newpath, codelist[k]);

						if (error)
						{
							return;
						}

						option.setDebug(debug);
						optionlist[codelist[k]] = option;
						templatelist[codelist[k]] = true;
						isOptionExist = true;
					}
					else if (boost::iequals(folderlist[l], "animationdatasinglefile") && boost::filesystem::is_directory(FOF2))
					{
						animData = FOF2;
					}
					else
					{
						pathVector.push_back(FOF2);
					}
				}

				for (unsigned int l = 0; l < pathVector.size(); ++l)
				{
					if (boost::filesystem::is_directory(pathVector[l]))
					{
						vecstr filelist;
						newpath = pathVector[l].string();
						read_directory(newpath, filelist);
						string behaviorFolder = pathVector[l].stem().string();
						string lowerBehaviorFolder = boost::algorithm::to_lower_copy(behaviorFolder);
						bool noGroup = true;

						for (unsigned int i = 0; i < filelist.size(); i++)
						{
							newpath = templateDirectory + codelist[k] + "\\" + behaviorFolder + "\\" + filelist[i];
							boost::filesystem::path file(newpath);
							boost::algorithm::to_lower(filelist[i]);
							boost::algorithm::to_lower(codelist[k]);

							if (!boost::filesystem::is_directory(file))
							{
								if ((filelist[i] == codelist[k] + "_group.txt" || filelist[i] == codelist[k] + "_group.nmd"))
								{
									if (behaviortemplate[codelist[k] + "_group"][lowerBehaviorFolder].size() == 0)
									{
										behaviortemplate[codelist[k] + "_group"][lowerBehaviorFolder] = GetFunctionLines(newpath);
										noGroup = false;

										if (error)
										{
											return;
										}
									}
									else
									{
										cout << "ERROR(1019): Duplicated template file detected. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
										error = true;
										return;
									}
								}
								else if ((filelist[i] == codelist[k] + "_master.txt" || filelist[i] == codelist[k] + "_master.nmd"))
								{
									if (behaviortemplate[codelist[k] + "_master"][lowerBehaviorFolder].size() == 0)
									{
										behaviortemplate[codelist[k] + "_master"][lowerBehaviorFolder] = GetFunctionLines(newpath);
										noGroup = false;

										if (error)
										{
											return;
										}
									}
									else
									{
										cout << "ERROR(1019): Duplicated template file detected. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
										error = true;
										return;
									}
								}
								else if (filelist[i] == codelist[k] + ".txt")
								{
									registered = true;
									grouplist[lowerBehaviorFolder].push_back(codelist[k]);

									if (behaviortemplate[codelist[k]][lowerBehaviorFolder].size() == 0)
									{
										behaviortemplate[codelist[k]][lowerBehaviorFolder] = GetFunctionLines(newpath);

										if (error)
										{
											return;
										}
									}
									else
									{
										cout << "ERROR(1019): Failed to identify template file. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
										error = true;
										return;
									}
								}
								else if (filelist[i][0] == '#')
								{
									string number = boost::regex_replace(string(filelist[i]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (filelist[i] == "#" + number + ".txt")
									{
										existingFunctionID[codelist[k]][lowerBehaviorFolder].push_back(stoi(number));
									}

									vecstr storeline = GetFunctionLines(newpath);
									bool isJoint = false;
									bool isStateMachine = false;

									for (unsigned int j = 0; j < storeline.size(); j++)
									{
										if (storeline[j].find("class=\"hkbStateMachine\" signature=\"") != string::npos)
										{
											isStateMachine = true;
										}

										if (isStateMachine && storeline[j].find("<!-- NEW ^" + codelist[k]) != string::npos)
										{
											if (!isJoint)
											{
												isJoint = true;
											}
										}
										else if (isStateMachine && storeline[j].find("<!-- CLOSE -->") != string::npos)
										{
											if (isJoint)
											{
												isJoint = false;
											}
										}

										if (isStateMachine && storeline[j].find("#" + codelist[k]) != string::npos)
										{
											if (isJoint)
											{
												isStateJoint[lowerBehaviorFolder][stoi(number)] = true;
												break;
											}
										}
									}
								}
							}

							if (error)
							{
								return;
							}
						}

						if (optionlist[codelist[k]].multiState[lowerBehaviorFolder].size() > 1)
						{
							if (isStateJoint[lowerBehaviorFolder].size() == 0)
							{
								cout << "ERROR(1074): Joint not found. Animation will not work. Please contact the template creator" << endl << "File: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
								error = true;
								return;
							}
							else if (isStateJoint[lowerBehaviorFolder].size() != optionlist[codelist[k]].multiState[lowerBehaviorFolder].size())
							{
								cout << "ERROR(1073): Potential wrong or missing state reference. All joints must be registered in option_list.txt when using multi joints. Please contact the template creator" << endl << "File: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
								error = true;
								return;
							}

							for (auto it = optionlist[codelist[k]].multiState[lowerBehaviorFolder].begin(); it != optionlist[codelist[k]].multiState[lowerBehaviorFolder].end(); ++it)
							{
								if (isStateJoint[lowerBehaviorFolder][it->second])
								{
									mainBehaviorJoint[codelist[k]][lowerBehaviorFolder][it->first] = it->second;
								}
								else
								{
									cout << "ERROR(1075): Missing state reference detected. All joints must be registered in option_list.txt when using multi joints. Please contact the template creator" << endl << "File: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
									error = true;
									return;
								}
							}
						}
						else
						{
							if (optionlist[codelist[k]].multiState[lowerBehaviorFolder].size() == 1)
							{
								cout << "WARNING: Only 1 State Machine detected. Multi StateMachine ignored. Please contact the template creator" << endl << "File: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
							}

							if (isStateJoint[lowerBehaviorFolder].size() > 1)
							{
								cout << "ERROR(1072): Potential wrong state reference. Template containing multiple StateMachine joints requires registration in option_list.txt to work. Please contact the template creator" << endl << "File: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
								error = true;
								return;
							}
							else if (isStateJoint[lowerBehaviorFolder].size() == 0)
							{
								cout << "ERROR(1074): Joint not found. Animation will not work. Please contact the template creator" << endl << "File: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
								error = true;
								return;
							}

							mainBehaviorJoint[codelist[k]][lowerBehaviorFolder][0] = isStateJoint[lowerBehaviorFolder].begin()->first;
						}

						// Error checking
						if (noGroup)
						{
							if (!optionlist[codelist[k]].ignoreGroup)
							{
								if (optionlist[codelist[k]].groupMin != -1 || optionlist[codelist[k]].ruleOne.size() != 0 || optionlist[codelist[k]].ruleTwo.size() != 0)
								{
									cout << "ERROR(1061): " << codelist[k] << "_group.txt file cannot be found. Please contact the template creator" << endl << "File Path: " << templateDirectory << behaviorFolder << "\\" << codelist[k] << endl << endl;
									error = true;
									return;
								}

								if (behaviortemplate[codelist[k] + "_master"].size() != 0)
								{
									cout << "ERROR(1033): Missing group animation template. Master animation template is only available to group animation template. Please contact the template creator" << endl << "File Path: " << templateDirectory << behaviorFolder << "\\" << codelist[k] << endl << endl;
									error = true;
									return;
								}
							}
						}
						else
						{
							if (optionlist[codelist[k]].ignoreGroup)
							{
								cout << "ERROR(1079): " << codelist[k] << "_group.txt detected. Conflict with ignore_group condition in option_list.txt. Please contact the template creator" << endl << "File Path: " << templateDirectory << codelist[k] << "\\option_list.txt" << endl << endl;
								error = true;
								return;
							}
						}

						filelist.clear();
					}
				}

				if (error)
				{
					return;
				}

				if (animData.string().length() != 0)
				{
					string animDataPath = animData.string();
					vecstr characlist;
					read_directory(animDataPath, characlist);

					for (unsigned int l = 0; l < characlist.size(); ++l)
					{
						newpath = animDataPath + "\\" + characlist[l];
						boost::filesystem::path FOF3(newpath);

						if (boost::filesystem::is_directory(FOF3))
						{
							vecstr headerlist;
							read_directory(newpath, headerlist);

							for (unsigned int i = 0; i < headerlist.size(); ++i)
							{
								newpath = newpath + "\\" + headerlist[i];
								boost::filesystem::path FOF4(newpath);

								if (!boost::filesystem::is_directory(FOF4))
								{
									string lowerBehaviorFolder = boost::algorithm::to_lower_copy(animData.stem().string()) + " " + characlist[l];

									if (headerlist[i] == codelist[k] + ".txt")
									{
										registered = true;
										grouplist[lowerBehaviorFolder].push_back(codelist[k]);

										if (behaviortemplate[codelist[k]][lowerBehaviorFolder].size() == 0)
										{
											behaviortemplate[codelist[k]][lowerBehaviorFolder] = GetFunctionLines(newpath);

											if (error)
											{
												return;
											}
										}
										else
										{
											cout << "ERROR(1019): Failed to identify template file. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
											error = true;
											return;
										}
									}
									else
									{
										stringstream sstream(FOF4.stem().string());
										istream_iterator<string> ssbegin(sstream);
										istream_iterator<string> ssend;
										vecstr fileparts(ssbegin, ssend);
										copy(fileparts.begin(), fileparts.end(), fileparts.begin());

										if (fileparts.size() < 3 && fileparts.size() != 0)
										{
											if (fileparts.size() == 1 && !isOnlyNumber(fileparts[0]))
											{
												cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << newpath << endl << endl;
												error = true;
												return;
											}
											else if (fileparts.size() == 2 && (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
											{
												cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << newpath << endl << endl;
												error = true;
												return;
											}

											if (characterHeaders.find(characlist[l]) == characterHeaders.end())
											{
												cout << "ERROR(3011): Unregistered character detected. Please re-run Update Patcher to fix it" << endl << "Character: " << animDataPath << "\\" << characlist[l] << endl << endl;
												error = true;
												return;
											}

											if (behaviortemplate[codelist[k]][lowerBehaviorFolder].size() == 0)
											{
												behaviortemplate[codelist[k]][lowerBehaviorFolder] = GetFunctionLines(newpath);

												if (error)
												{
													return;
												}
											}
											else
											{
												cout << "ERROR(1019): Duplicated template file detected. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
												error = true;
												return;
											}
										}
									}
								}
							}
						}
					}
				}

				if (!isOptionExist && registered)
				{
					cout << "ERROR(1021): Missing option list file. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
					error = true;
					return;
				}
				
				folderlist.clear();
			}
		}

		if (error)
		{
			return;
		}
	}
}
