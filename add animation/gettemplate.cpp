#include "gettemplate.h"

using namespace std;

getTemplate::getTemplate()
{
	string templateDirectory = "behavior templates/";
	string newpath;

	vecstr folderlist;
	vecstr codelist;
	vecstr filelist;

	read_directory(templateDirectory, folderlist);

	for (unsigned int l = 0; l < folderlist.size(); l++)
	{
		if (folderlist[l].find(".", 0) == string::npos)
		{
			newpath = templateDirectory + folderlist[l];
			boost::filesystem::path FOF(newpath);

			if (boost::filesystem::is_directory(FOF))
			{
				read_directory(newpath, codelist);

				for (unsigned int k = 0; k < codelist.size(); k++)
				{
					newpath = templateDirectory + folderlist[l] + "/" + codelist[k];
					boost::filesystem::path FOF2(newpath);

					if (boost::filesystem::is_directory(FOF2))
					{
						read_directory(newpath, filelist);
						bool isOptionExist = false;
						bool noGroup = true;

						for (unsigned int i = 0; i < filelist.size(); i++)
						{
							newpath = templateDirectory + folderlist[l] + "/" + codelist[k] + "/" + filelist[i];
							boost::filesystem::path file(newpath);
							boost::algorithm::to_lower(filelist[i]);
							boost::algorithm::to_lower(codelist[k]);

							if (filelist[i] == "option_list.txt" && !boost::filesystem::is_directory(file))
							{
								OptionList option(newpath, codelist[k]);

								if (error)
								{
									return;
								}

								option.setDebug(debug);
								optionlist[codelist[k]] = option;
								grouplist[folderlist[l]].push_back(codelist[k]);
								templatelist[codelist[k]] = true;
								isOptionExist = true;
							}
							else if ((filelist[i] == codelist[k] + "_group.txt" || filelist[i] == codelist[k] + "_group.nmd") && !boost::filesystem::is_directory(file))
							{
								if (behaviortemplate[codelist[k] + "_group"].size() == 0)
								{
									behaviortemplate[codelist[k] + "_group"] = GetFunctionLines(newpath);
									noGroup = false;
								}
								else
								{
									cout << "ERROR(1019): Duplicated template file detected. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
									error = true;
									return;
								}
							}
							else if ((filelist[i] == codelist[k] + "_master.txt" || filelist[i] == codelist[k] + "_master.nmd") && !boost::filesystem::is_directory(file))
							{
								if (behaviortemplate[codelist[k] + "_master"].size() == 0)
								{
									behaviortemplate[codelist[k] + "_master"] = GetFunctionLines(newpath);
								}
								else
								{
									cout << "ERROR(1019): Duplicated template file detected. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
									error = true;
									return;
								}
							}
							else if ((filelist[i] == codelist[k] + ".txt" || filelist[i] == codelist[k] + ".nmd") && !boost::filesystem::is_directory(file))
							{
								if (behaviortemplate[codelist[k]].size() == 0)
								{
									behaviortemplate[codelist[k]] = GetFunctionLines(newpath);
								}
								else
								{
									cout << "ERROR(1019): Failed to identify template file. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
									error = true;
									return;
								}
							}
							else if (filelist[i][0] == '#' && !boost::filesystem::is_directory(file))
							{
								string number = boost::regex_replace(string(filelist[i]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								if (filelist[i] == "#" + number + ".txt")
								{
									existingFunctionID[codelist[k]].push_back(stoi(number));
								}

								vecstr storeline = GetFunctionLines(newpath);
								bool isJoint = false;

								for (unsigned int j = 0; j < storeline.size(); j++)
								{
									if (storeline[j].find("<!-- NEW ^" + codelist[k]) != string::npos)
									{
										if (!isJoint)
										{
											isJoint = true;
										}
										else
										{
											mainBehaviorJoint[codelist[k]] = stoi(number);
											break;
										}
									}

									if (storeline[j].find("#" + codelist[k]) != string::npos)
									{
										if (!isJoint)
										{
											isJoint = true;
										}
										else
										{
											mainBehaviorJoint[codelist[k]] = stoi(number);
											break;
										}
									}
								}
							}

							if (error)
							{
								return;
							}
						}

						// Error checking
						if (noGroup)
						{
							if (optionlist[codelist[k]].groupMin != -1)
							{
								cout << "ERROR(1061): " << codelist[k] << "_group.txt file cannot be found. Unable to proceed when \"minimum\" is used without " << codelist[k] << "_group.txt. Please contact the template creator" << endl << "File Path: " << templateDirectory << folderlist[l] << "/" << codelist[k] << endl << endl;
								error = true;
								return;
							}

							if (behaviortemplate[codelist[k] + "_master"].size() != 0)
							{
								cout << "ERROR(1033): Missing group animation template. Master animation template is only available to group animation template. Please contact the template creator" << endl << "File Path: " << templateDirectory << folderlist[l] << "/" << codelist[k] << endl << endl;
								error = true;
								return;
							}
						}

						if (!isOptionExist)
						{
							cout << "ERROR(1021): Missing option list file. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
							error = true;
							return;
						}

						if ((optionlist[codelist[k]].groupMin != 0 || optionlist[codelist[k]].ruleOne.size() != 0 || optionlist[codelist[k]].ruleTwo.size() != 0) && noGroup)
						{
							cout << "ERROR(1024): Missing group file. Please contact the template creator" << endl << "File: " << newpath << endl << endl;
							error = true;
							return;
						}

						filelist.clear();
					}
				}

				if (error)
				{
					return;
				}

				codelist.clear();
			}
		}

		if (error)
		{
			return;
		}
	}
}
