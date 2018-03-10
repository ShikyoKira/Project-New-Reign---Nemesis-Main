#include "dataunification.h"

using namespace std;

void SeparateMod(string dr, string f1, vecstr f2)
{
	vecstr f3;

	for (unsigned int j = 0; j < f2.size(); j++)
	{
		read_directory(dr + f1 + "/" + f2[j] + "/", f3);

		for (unsigned int k = 0; k < f3.size(); k++)
		{
			if (!FunctionUpdate(f1, f2[j], f3[k], memory))
			{
				error = true;
				return;
			}
		}
	}
}

bool newAnimUpdate(string sourcefolder, string targetfolder)
{
	vecstr filelist;
	vecstr filelist2;
	vecstr filelist3;

	if (CreateDirectory((sourcefolder).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		read_directory(sourcefolder, filelist);

		for (unsigned int i = 0; i < filelist.size(); i++)
		{
			string folderpath = sourcefolder + filelist[i];
			read_directory(folderpath, filelist2);

			for (unsigned int j = 0; j < filelist2.size(); j++)
			{
				boost::filesystem::path curfile(folderpath + "/" + filelist2[j]);

				if (boost::filesystem::is_directory(curfile))
				{
					read_directory(folderpath + "/" + filelist2[j], filelist3);

					for (unsigned int k = 0; k < filelist3.size(); k++)
					{
						if (filelist3[k][0] == '#')
						{
							string line;
							string readfile = folderpath + "/" + filelist2[j] + "/" + filelist3[k];
							vecstr storeline = GetFunctionLines(readfile);

							if (error)
							{
								return false;
							}

							string targetfile = targetfolder + "new/" + filelist[i] + "/" + filelist3[k];
							vecstr originallines = GetFunctionLines(targetfile);

							if (error)
							{
								return false;
							}

							bool close = false;
							int linecount = 0;
							vecstr newlines;
							vecstr combinelines;

							for (unsigned int k = 0; k < storeline.size(); k++)
							{
								if (storeline[k].find("<!-- NEW", 0) == string::npos && !close)
								{
									if (originallines[linecount].find("<!-- NEW", 0) != string::npos)
									{
										combinelines.push_back(originallines[linecount]);
										linecount++;
										combinelines.push_back(originallines[linecount]);
										linecount++;

										while (true)
										{
											if (originallines[linecount].find("<!-- CLOSE -->", 0) != string::npos)
											{
												combinelines.push_back(originallines[linecount]);
												linecount++;
												break;
											}
										}
									}
									else if (storeline[k].find("<!-- ", 0) != string::npos && storeline[k].find("numelements +", 0) != string::npos)
									{
										size_t position = storeline[k].find("<!-- ");
										string templine = storeline[k].substr(position, storeline[k].find("-->", position) - position + 3);

										if (originallines[linecount].find("<!-- ", 0) != string::npos && originallines[linecount].find("-->", 0) != string::npos)
										{
											originallines[linecount].append(" " + templine);
										}
										else
										{
											originallines[linecount].append("			" + templine);
										}
									}

									combinelines.push_back(originallines[linecount]);
									linecount++;
								}
								else if (close && storeline[k].find("<!-- CLOSE -->", 0) != string::npos)
								{
									if (originallines[linecount].find("<!-- NEW", 0) != string::npos)
									{
										combinelines.push_back(originallines[linecount]);
										linecount++;
										combinelines.push_back(originallines[linecount]);
										linecount++;

										while (true)
										{
											if (originallines[linecount].find("<!-- CLOSE -->", 0) != string::npos)
											{
												combinelines.push_back(originallines[linecount]);
												linecount++;
												break;
											}
										}
									}

									for (unsigned int l = 0; l < newlines.size(); l++)
									{
										combinelines.push_back(newlines[l]);
									}

									combinelines.push_back(storeline[k]);
									close = false;
								}
								else
								{
									close = true;
								}

								if (close)
								{
									newlines.push_back(storeline[k]);
								}
							}

							ofstream pasteTarget(targetfile);
							FunctionWriter fwriter(&pasteTarget);

							if (pasteTarget.is_open())
							{
								for (unsigned int k = 0; k < combinelines.size(); k++)
								{
									fwriter << combinelines[k] << "\n";
								}

								pasteTarget.close();
							}
							else
							{
								cout << "ERROR(2602): Unable to open file " << endl << "File: " << targetfile << endl << endl;
								error = true;
								return false;
							}
						}
					}

					filelist3.clear();
				}
				else
				{
					if (filelist2[j][0] == '#')
					{
						vecstr storeline = GetFunctionLines(folderpath + "/" + filelist2[j]);

						if (error)
						{
							return false;
						}

						string targetfile = targetfolder + "new/" + filelist[i] + "/" + filelist2[j];
						ofstream pasteTarget(targetfile);
						FunctionWriter fwriter(&pasteTarget);

						if (pasteTarget.is_open())
						{
							for (unsigned int k = 0; k < storeline.size(); k++)
							{
								fwriter << storeline[k] << "\n";
							}
							pasteTarget.close();
						}
						else
						{
							cout << "ERROR(2603): Unable to write file " << endl << "File: " << targetfile << endl << endl;
							error = true;
							return false;
						}
					}
				}
			}

			filelist2.clear();
		}
	}
	else
	{
		cout << "ERROR(2604): Failed to create new folder (Folder: " << sourcefolder << ")" << endl;
		error = true;
		return false;
	}

	return true;
}

void JoiningEdits(string directory)
{
	vecstr filelist;
	
	if (CreateDirectory(directory.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		vector<thread> threads;

		read_directory(directory, filelist);

		for (unsigned int i = 0; i < filelist.size(); i++)
		{
			vecstr filelist2;

			read_directory(directory + filelist[i] + "/", filelist2);

			if ((filelist[i] != "new") && (filelist[i] != "vanilla"))
			{
				threads.emplace_back(SeparateMod, directory, filelist[i], filelist2);
			}
		}

		for (auto& th: threads)
		{
			th.join();
		}
	}
}

void CombiningFiles(string directory)
{
	vecstr filelist;
	vecstr filelist2;
	vecstr filelist3;
	vecstr fileline;

	read_directory(directory, filelist);

	for (unsigned int i = 0; i < filelist.size(); i++)
	{
		if (filelist[i] == "new")
		{
			read_directory(directory + filelist[i] + "/", filelist2);

			for (unsigned int j = 0; j < filelist2.size(); j++)
			{
				read_directory(directory + filelist[i] + "/" + filelist2[j] + "/", filelist3);

				string rootID;
				bool isOpen = false;
				string OpeningMod;

				for (unsigned int k = 0; k < filelist3.size(); k++)
				{
					if (filelist3[k].find("$") != string::npos)
					{
						string modID = filelist3[k].substr(1, filelist3[k].find("$") - 1);

						if (OpeningMod != modID && isOpen)
						{
							fileline.push_back("<!-- CLOSE -->\n");
							isOpen = false;
						}

						if (!isOpen)
						{
							fileline.push_back("<!-- NEW *" + modID + "* -->\n");
							OpeningMod = modID;
							isOpen = true;
						}
					}

					string line;
					string file = directory + filelist[i] + "/" + filelist2[j] + "/" + filelist3[k];
					char charline[5000];
					FILE* BehaviorFormat;
					fopen_s(&BehaviorFormat, file.c_str(), "r");

					if (BehaviorFormat)
					{
						while (fgets(charline, 5000, BehaviorFormat))
						{
							if (string(charline).find("class=\"hkRootLevelContainer\" signature=\"0x2772c11e\">", 0) != string::npos)
							{
								rootID = "#" + boost::regex_replace(string(charline), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							}

							fileline.push_back(charline);
						}

						fileline.push_back("\n");
						fclose(BehaviorFormat);
					}
					else
					{
						cout << "ERROR(2601): Unable to open file (File: " << file << ")" << endl;
						error = true;
						return;
					}
				}

				if (isOpen)
				{
					fileline.push_back("<!-- CLOSE -->\n");
					isOpen = false;
				}

				string filename = filelist2[j] + ".txt";
				string compilingfolder = "temp_behaviors/";

				if (CreateDirectory(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
				{
					string file = compilingfolder + filename;
					ofstream output(file);

					if (output.is_open())
					{
						FunctionWriter writeoutput(&output);

						writeoutput << "<?xml version=\"1.0\" encoding=\"ascii\"?>" << "\n";
						writeoutput << "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" << rootID << "\">" << "\n";
						writeoutput << "\n";
						writeoutput << "	<hksection name=\"__data__\">" << "\n";
						writeoutput << "\n";

						for (unsigned int linecount = 0; linecount < fileline.size(); linecount++)
						{
							writeoutput << fileline[linecount];
						}
						
						writeoutput << "	</hksection>" << "\n";
						writeoutput << "\n";
						writeoutput << "</hkpackfile>" << "\n";
						writeoutput << "\n";

						output.close();
					}
					else
					{
						cout << "ERROR(2600): Unable to create file (File: " << file << ")" << endl;
						error = true;
						return;
					}
				}
			}
		}
	}
}
