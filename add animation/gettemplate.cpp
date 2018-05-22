#include "gettemplate.h"

#pragma warning(disable:4503)

using namespace std;

getTemplate::getTemplate()
{
	string templateDirectory = "behavior templates\\";
	string newpath;
	vecstr codelist;
	read_directory(templateDirectory, codelist);

	for (unsigned int k = 0; k < codelist.size(); ++k)
	{
		if (codelist[k].find(".", 0) == NOT_FOUND)
		{
			newpath = templateDirectory + codelist[k];
			boost::filesystem::path FOF(newpath);

			if (boost::iequals(codelist[k], "t") || boost::iequals(codelist[k], "aaprefix") || boost::iequals(codelist[k], "aaset") || boost::iequals(codelist[k], "md") || boost::iequals(codelist[k], "rd") || boost::iequals(codelist[k], "+"))
			{
				ErrorMessage(3009, codelist[k]);
				return;
			}

			if (boost::filesystem::is_directory(FOF))
			{
				vecstr folderlist;
				read_directory(newpath, folderlist);
				bool isOptionExist = false;
				bool registered = false;
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

					pathVector.push_back(FOF2);
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

						for (unsigned int i = 0; i < filelist.size(); ++i)
						{
							newpath = templateDirectory + codelist[k] + "\\" + behaviorFolder + "\\" + filelist[i];
							boost::filesystem::path file(newpath);
							string lowerfilename = boost::to_lower_copy(filelist[i]);							
							boost::to_lower(codelist[k]);

							if (!boost::filesystem::is_directory(file))
							{
								if (boost::iequals(lowerfilename, codelist[k] + "_group.txt"))
								{
									if (behaviortemplate[codelist[k] + "_group"][lowerBehaviorFolder].size() == 0)
									{
										GetFunctionLines(newpath, behaviortemplate[codelist[k] + "_group"][lowerBehaviorFolder]);
										noGroup = false;

										if (error)
										{
											return;
										}
									}
									else
									{
										ErrorMessage(1019, newpath);
										return;
									}
								}
								else if (boost::iequals(lowerfilename, codelist[k] + "_master.txt"))
								{
									if (behaviortemplate[codelist[k] + "_master"][lowerBehaviorFolder].size() == 0)
									{
										GetFunctionLines(newpath, behaviortemplate[codelist[k] + "_master"][lowerBehaviorFolder]);
										noGroup = false;

										if (error)
										{
											return;
										}
									}
									else
									{
										ErrorMessage(1019, newpath);
										return;
									}
								}
								else if (boost::iequals(lowerfilename, codelist[k] + ".txt"))
								{
									registered = true;
									grouplist[lowerBehaviorFolder].push_back(codelist[k]);

									if (behaviortemplate[codelist[k]][lowerBehaviorFolder].size() == 0)
									{
										GetFunctionLines(newpath, behaviortemplate[codelist[k]][lowerBehaviorFolder]);

										if (error)
										{
											return;
										}
									}
									else
									{
										ErrorMessage(1019, newpath);
										return;
									}
								}
								else if (lowerfilename[0] == '#')
								{
									string number = boost::regex_replace(string(lowerfilename), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (lowerfilename == "#" + number + ".txt" && isOnlyNumber(number))
									{
										existingFunctionID[codelist[k]][lowerBehaviorFolder].push_back(stoi(number));
									}

									vecstr storeline;
									GetFunctionLines(newpath, storeline);
									bool isJoint = false;
									bool isStateMachine = false;

									for (unsigned int j = 0; j < storeline.size(); ++j)
									{
										if (storeline[j].find("class=\"hkbStateMachine\" signature=\"") != NOT_FOUND)
										{
											isStateMachine = true;
										}

										if (isStateMachine && storeline[j].find("<!-- NEW ^" + codelist[k]) != NOT_FOUND)
										{
											if (!isJoint)
											{
												isJoint = true;
											}
										}
										else if (isStateMachine && storeline[j].find("<!-- CLOSE -->") != NOT_FOUND)
										{
											if (isJoint)
											{
												isJoint = false;
											}
										}

										if (isStateMachine && storeline[j].find("#" + codelist[k]) != NOT_FOUND)
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
							else if (lowerBehaviorFolder == "animationdatasinglefile")
							{
								vecstr headerlist;
								read_directory(newpath, headerlist);
								grouplist[lowerBehaviorFolder].push_back(codelist[k]);
								string project = filelist[i] + ".txt";

								for (unsigned int j = 0; j < headerlist.size(); ++j)
								{
									string header = headerlist[j].substr(0, headerlist[j].find_last_of("."));

									if (header[0] == '$' && (header.back() == '$' || header.find_last_of("$UC") == header.length() - 1))
									{
										if (animdatatemplate[codelist[k]][project][header].size() == 0)
										{
											GetFunctionLines(newpath + "\\" + headerlist[j], animdatatemplate[codelist[k]][project][header], true);

											if (error)
											{
												return;
											}
										}
										else
										{
											ErrorMessage(1019, newpath);
											return;
										}
									}
									else
									{
										existingAnimDataHeader[codelist[k]][project].insert(header);
									}
								}
							}
							else if (lowerBehaviorFolder == "animationsetdatasinglefile")
							{
								vecstr headerlist;
								read_directory(newpath, headerlist);
								grouplist[lowerBehaviorFolder].push_back(codelist[k]);
								string project = filelist[i] + "\\" + filelist[i].substr(0, wordFind(filelist[i], "data", true)) + ".txt";

								for (unsigned int j = 0; j < headerlist.size(); ++j)
								{
									string header = headerlist[j].substr(0, headerlist[j].find_last_of("."));

									if (header[0] == '$' && header.back() == '$')
									{
										if (asdtemplate[codelist[k]][project][header].size() == 0)
										{
											GetFunctionLines(newpath + "\\" + headerlist[j], asdtemplate[codelist[k]][project][header], false);

											if (error)
											{
												return;
											}
										}
										else
										{
											ErrorMessage(1019, newpath);
											return;
										}
									}
									else
									{
										existingASDHeader[codelist[k]][project].insert(header);
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
								ErrorMessage(1074, templateDirectory + codelist[k]);
								return;
							}
							else if (isStateJoint[lowerBehaviorFolder].size() != optionlist[codelist[k]].multiState[lowerBehaviorFolder].size())
							{
								ErrorMessage(1073, templateDirectory + codelist[k]);
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
									ErrorMessage(1075, templateDirectory + codelist[k]);
									return;
								}
							}
						}
						else if (lowerBehaviorFolder != "animationdatasinglefile" && lowerBehaviorFolder != "animationsetdatasinglefile")
						{
							if (optionlist[codelist[k]].multiState[lowerBehaviorFolder].size() == 1)
							{
								WarningMessage(1008, templateDirectory + codelist[k] + "\\option_list.txt");
							}

							if (isStateJoint[lowerBehaviorFolder].size() > 1)
							{
								ErrorMessage(1072, templateDirectory + codelist[k]);
								return;
							}
							else if (isStateJoint[lowerBehaviorFolder].size() == 0)
							{
								ErrorMessage(1074, templateDirectory + codelist[k]);
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
									ErrorMessage(1061, codelist[k], templateDirectory + behaviorFolder + "\\" + codelist[k]);
									return;
								}

								if (behaviortemplate[codelist[k] + "_master"].size() != 0)
								{
									ErrorMessage(1085, templateDirectory + behaviorFolder + "\\" + codelist[k]);
									return;
								}
							}
						}
						else
						{
							if (optionlist[codelist[k]].ignoreGroup)
							{
								ErrorMessage(1079, codelist[k], templateDirectory + codelist[k] + "\\option_list.txt");
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

				if (!isOptionExist && registered)
				{
					ErrorMessage(1021, newpath);
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
