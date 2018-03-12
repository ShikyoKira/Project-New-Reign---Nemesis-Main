#include "dataunification.h"

using namespace std;

void SeparateMod(string dr, string f1, vecstr f2, unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr f3;

	for (unsigned int j = 0; j < f2.size(); j++)
	{
		read_directory(dr + f1 + "\\" + f2[j] + "\\", f3);

		for (unsigned int k = 0; k < f3.size(); k++)
		{
			if (!FunctionUpdate(f1, f2[j], f3[k], memory, newFile))
			{
				error = true;
				return;
			}
		}
	}
}

bool newAnimUpdate(string sourcefolder, string targetfolder, unordered_map<string, map<string, vecstr>>& newFile)
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
				boost::filesystem::path curfile(folderpath + "\\" + filelist2[j]);

				if (boost::filesystem::is_directory(curfile))
				{
					read_directory(folderpath + "\\" + filelist2[j], filelist3);

					for (unsigned int k = 0; k < filelist3.size(); k++)
					{
						if (filelist3[k][0] == '#')
						{
							string line;
							vecstr storeline = GetFunctionLines(folderpath + "\\" + filelist2[j] + "\\" + filelist3[k]);

							if (error)
							{
								return false;
							}

							vecstr originallines = newFile[filelist[i]][filelist3[k]];

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

							newFile[filelist[i]][filelist3[k]] = combinelines;
						}
					}

					filelist3.clear();
				}
				else
				{
					if (filelist2[j][0] == '#')
					{
						newFile[filelist[i]][filelist2[j]] = GetFunctionLines(folderpath + "/" + filelist2[j]);

						if (error)
						{
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
		cout << "ERROR(2604): Failed to create new folder" << endl << "Folder: " << sourcefolder << endl << endl;
		error = true;
		return false;
	}

	return true;
}

void JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr filelist;
	
	if (CreateDirectory(directory.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		vector<thread> threads;

		read_directory(directory, filelist);

		for (unsigned int i = 0; i < filelist.size(); i++)
		{
			vecstr filelist2;

			read_directory(directory + filelist[i] + "\\", filelist2);

			threads.emplace_back(SeparateMod, directory, filelist[i], filelist2, ref(newFile));
		}

		for (auto& th: threads)
		{
			th.join();
		}
	}
}

void CombiningFiles(unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr fileline;
	
	for (auto it = newFile.begin(); it != newFile.end(); ++it) // behavior file name
	{
		string rootID;
		bool isOpen = false;
		string OpeningMod;

		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter) // behavior node ID
		{
			if (iter->first.find("$") != string::npos)
			{
				string modID = iter->first.substr(1, iter->first.find("$") - 1);

				if (OpeningMod != modID && isOpen)
				{
					fileline.push_back("<!-- CLOSE -->");
					isOpen = false;
				}

				if (!isOpen)
				{
					fileline.push_back("<!-- NEW *" + modID + "* -->");
					OpeningMod = modID;
					isOpen = true;
				}
			}

			vecstr storeline = iter->second;
			string line;

			if (storeline.size() != 0)
			{
				for (unsigned int l = 0; l < storeline.size(); ++l)
				{
					line = storeline[l];

					if (line.find("class=\"hkRootLevelContainer\" signature=\"0x2772c11e\">", 0) != string::npos)
					{
						rootID = "#" + boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
					}

					fileline.push_back(line);
				}
			}
			else
			{
				cout << ">> ERROR(2605): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << "File: " << it->first << "(" << iter->first << ")" << endl << endl;
				error = true;
				return;
			}
		}

		if (isOpen)
		{
			fileline.push_back("<!-- CLOSE -->");
			isOpen = false;
		}

		string filename = it->first + ".txt";
		string compilingfolder = "temp_behaviors\\";

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
					writeoutput << fileline[linecount] << "\n";
				}

				writeoutput << "	</hksection>" << "\n";
				writeoutput << "\n";
				writeoutput << "</hkpackfile>" << "\n";
				writeoutput << "\n";

				output.close();
			}
			else
			{
				cout << "ERROR(2600): Unable to create file" << endl << "File: " << file << endl << endl;
				error = true;
				return;
			}
		}
	}
}
