#include "dataunification.h"

#pragma warning(disable:4503)

using namespace std;

void SeparateMod(string directory, string modecode, vecstr behaviorfilelist, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
	for (unsigned int j = 0; j < behaviorfilelist.size(); ++j)
	{
		vecstr nodelist;
		read_directory(directory + modecode + "\\" + behaviorfilelist[j] + "\\", nodelist);
		boost::filesystem::path curPath(directory + modecode + "\\" + behaviorfilelist[j]);

		if (boost::filesystem::is_directory(curPath))
		{
			if (boost::iequals(behaviorfilelist[j], "animationdatasinglefile"))
			{
				if (newAnimData.size() == 0)
				{
					cout << "ERROR(3017): Nemesis_animationdatasinglefile.txt not found. Please re-install Nemesis" << endl << endl;
					error = true;
					return;
				}

				for (unsigned int k = 0; k < nodelist.size(); ++k)
				{
					boost::filesystem::path curPath(directory + modecode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k]);

					if (boost::filesystem::is_directory(curPath))
					{
						bool newChar = false;

						if (newAnimData.find(nodelist[k]) == newAnimData.end())
						{
							animDataChar.push_back(nodelist[k]);
							newChar = true;
						}

						vecstr uniquecodelist;
						string filepath = directory + modecode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k];
						read_directory(filepath, uniquecodelist);

						for (unsigned int i = 0; i < uniquecodelist.size(); ++i)
						{
							if (!AnimDataUpdate(modecode, behaviorfilelist[j], nodelist[k], filepath + "\\" + uniquecodelist[k], newAnimData, animDataChar, animDataHeader, newChar))
							{
								error = true;
								return;
							}
						}
					}
				}
			}
			else
			{
				for (unsigned int k = 0; k < nodelist.size(); ++k)
				{
					if (!FunctionUpdate(modecode, behaviorfilelist[j], nodelist[k], newFile))
					{
						error = true;
						return;
					}
				}
			}
		}
	}
}

bool newAnimUpdate(string sourcefolder, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
	if (CreateDirectory((sourcefolder).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		vecstr codelist;
		read_directory(sourcefolder, codelist);

		for (unsigned int i = 0; i < codelist.size(); ++i)
		{
			string folderpath = sourcefolder + codelist[i];
			boost::filesystem::path codefile(folderpath);

			if (boost::filesystem::is_directory(codefile))
			{
				vecstr behaviorlist;
				read_directory(folderpath, behaviorlist);

				for (unsigned int j = 0; j < behaviorlist.size(); ++j)
				{
					boost::filesystem::path curfolder(folderpath + "\\" + behaviorlist[j]);

					if (boost::filesystem::is_directory(curfolder))
					{
						if (boost::iequals(behaviorlist[j], "animationdatasinglefile"))
						{
							vecstr characterlist;
							read_directory(folderpath + "\\" + behaviorlist[j], characterlist);

							for (unsigned int k = 0; k < characterlist.size(); ++k)
							{
								boost::filesystem::path characterfolder(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k]);

								if (boost::filesystem::is_directory(characterfolder))
								{
									if (!newAnimDataUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k], codelist[i], characterlist[k], newAnimData, animDataChar, animDataHeader))
									{
										return false;
									}
								}
							}
						}
						else
						{
							if (!newAnimUpdateExt(folderpath, behaviorlist[j], newFile))
							{
								return false;
							}
						}
					}
				}
			}
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

bool newAnimUpdateExt(string folderpath, string behaviorfile, unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr nodelist;
	read_directory(folderpath + "\\" + behaviorfile, nodelist);

	for (unsigned int k = 0; k < nodelist.size(); ++k)
	{
		boost::filesystem::path curfile(folderpath + "\\" + behaviorfile + "\\" + nodelist[k]);

		if (nodelist[k][0] == '#' && !boost::filesystem::is_directory(curfile))
		{
			string line;
			vecstr storeline = GetFunctionLines(folderpath + "\\" + behaviorfile + "\\" + nodelist[k]);

			if (error)
			{
				return false;
			}
			
			string nodeID = nodelist[k].substr(0, nodelist[k].find_last_of("."));
			vecstr originallines = newFile[behaviorfile][nodeID];

			if (error)
			{
				return false;
			}

			bool close = false;
			unordered_map<int, bool> conditionOpen;
			bool conditionOri = false;
			int linecount = 0;
			int conditionLvl = 0;
			vecstr newlines;
			vecstr combinelines;

			for (unsigned int k = 0; k < storeline.size(); ++k)
			{
				if (storeline[k].find("<!-- NEW", 0) == string::npos && !close)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != string::npos)
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						combinelines.push_back(originallines[linecount]);
						++linecount;

						while (true)
						{
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != string::npos)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								break;
							}
							else
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
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

					if (storeline[k].find("<!-- CONDITION END -->") != string::npos)
					{
						combinelines.push_back(storeline[k]);
						conditionOri = false;
						conditionOpen[conditionLvl] = false;
						--conditionLvl;
					}
					else if (conditionOri || !conditionOpen[conditionLvl])
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
					}
					else
					{
						combinelines.push_back(storeline[k]);
					}
				}
				else if (close && storeline[k].find("<!-- CLOSE -->", 0) != string::npos)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != string::npos)
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						combinelines.push_back(originallines[linecount]);
						++linecount;

						while (true)
						{
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != string::npos)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								break;
							}
							else
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
							}
						}
					}

					combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
					combinelines.push_back(storeline[k]);
					newlines.clear();
					close = false;
				}
				else
				{
					close = true;
				}

				if (storeline[k].find("<!-- CONDITION -->") != string::npos)
				{
					conditionOri = true;
				}

				if (close)
				{
					newlines.push_back(storeline[k]);
				}
			}

			if (combinelines.size() != 0 && combinelines.back().length() != 0)
			{
				combinelines.push_back("");
			}

			newFile[behaviorfile][nodeID] = combinelines;
		}
	}

	return true;
}

bool newAnimDataUpdateExt(string folderpath, string modcode, string characterfile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
	vecstr headerlist;
	read_directory(folderpath, headerlist);

	for (unsigned int k = 0; k < headerlist.size(); ++k)
	{
		string filepath = folderpath + "\\" + headerlist[k];
		boost::filesystem::path curfile(filepath);

		if (!boost::filesystem::is_directory(curfile))
		{
			string filename = curfile.stem().string();
			stringstream sstream(filename);
			istream_iterator<string> ssbegin(sstream);
			istream_iterator<string> ssend;
			vecstr fileparts(ssbegin, ssend);
			copy(fileparts.begin(), fileparts.end(), fileparts.begin());

			if (fileparts.size() < 3 && fileparts.size() != 0)
			{
				if (fileparts.size() == 1 && !isOnlyNumber(fileparts[0]) && !boost::iequals(fileparts[0], "$header$") && !boost::iequals(fileparts[0], "$info header$"))
				{
					cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << filepath << endl << endl;
					error = true;
					return false;
				}
				else if (fileparts.size() == 2 && (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
				{
					cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << filepath << endl << endl;
					error = true;
					return false;
				}

				string line;
				vecstr storeline = GetFunctionLines(filepath);

				if (error)
				{
					return false;
				}
				
				bool isNew = false;

				if (newAnimData.find(characterfile) == newAnimData.end())
				{
					animDataChar.push_back(characterfile);
					isNew = true;
				}

				if (newAnimData[characterfile].find(filename) == newAnimData[characterfile].end())
				{
					if (fileparts.size() > 1)		// type 1
					{
						if (!isNew && animDataHeader[characterfile].size() > 0)
						{
							vecstr tempheaders;
							size_t infoline = animDataHeader[characterfile].size();
							tempheaders.reserve(infoline);

							for (unsigned int i = 0; i < infoline; ++i)
							{
								if (boost::iequals(animDataHeader[characterfile][i], "$info header$") || animDataHeader[characterfile][i].find(" ") == string::npos)
								{
									infoline = i;
									tempheaders.push_back(filename);
									break;
								}

								tempheaders.push_back(animDataHeader[characterfile][i]);
							}
							
							for (unsigned int i = infoline; i < animDataHeader[characterfile].size(); ++i)
							{
								tempheaders.push_back(animDataHeader[characterfile][i]);
							}

							animDataHeader[characterfile] = tempheaders;
						}
						else
						{
							animDataHeader[characterfile].push_back(filename);
						}
					}
					else		// type 2
					{
						if (!boost::iequals(filename, "$info header$"))
						{
							if (newAnimData[characterfile].find("$info header$") == newAnimData[characterfile].end())
							{
								if (!isFileExist(folderpath + "\\$info header$.txt"))
								{
									cout << "ERROR(3012): \"$info header\" not found. Info header must exist in order to register any other info header. Please contact the template creator" << endl << "File: " << filepath << endl << endl;
									error = true;
									return false;
								}
							}

							animDataHeader[characterfile].push_back(filename);
						}
						else
						{
							vecstr tempheaders;
							size_t infoline = animDataHeader[characterfile].size();
							tempheaders.reserve(infoline);

							for (unsigned int i = 0; i < infoline; ++i)
							{
								if (animDataHeader[characterfile][i].find(" ") == string::npos)
								{
									infoline = i;
									tempheaders.push_back(filename);
									break;
								}

								tempheaders.push_back(animDataHeader[characterfile][i]);
							}

							for (unsigned int i = infoline; i < animDataHeader[characterfile].size(); ++i)
							{
								tempheaders.push_back(animDataHeader[characterfile][i]);
							}

							animDataHeader[characterfile] = tempheaders;
						}
					}

					isNew = true;
				}

				if (!isNew)
				{
					vecstr originallines = newAnimData[characterfile][filename];

					if (storeline.back().length() != 0 && originallines.back().length() == 0)
					{
						storeline.push_back("");
					}

					bool isHeader = false;
					bool close = false;
					bool conditionOri = false;

					unordered_map<int, bool> conditionOpen;
					unordered_map<int, vecstr> condition;

					int linecount = 0;
					int conditionLvl = 0;
					int type;

					vecstr newlines;
					vecstr combinelines;

					combinelines.reserve(storeline.size() + originallines.size());

					if (boost::iequals(characterfile, "$header$"))
					{
						isHeader = true;
					}

					if (filename == "$header$" || filename == "$info header$")
					{
						type = 0;
					}
					else if (hasAlpha(storeline[0]))
					{
						type = 1;
					}
					else if (isOnlyNumber(storeline[0]))
					{
						type = 2;
					}
					else
					{
						cout << "ERROR(3006): Invalid file name. Please enter a file name that is within the format. Please contact the mod author" << endl << "Character: " << characterfile << endl << "Header: " << fileparts[0] << endl << endl;
						error = true;
						return false;
					}

					for (unsigned int k = 0; k < storeline.size(); ++k)
					{
						if (storeline[k].find("<!-- CONDITION START ") != string::npos)
						{
							++conditionLvl;
							conditionOpen[conditionLvl] = true;
						}

						if (storeline[k].find("<!-- NEW", 0) == string::npos && !close)
						{
							if (originallines[linecount].find("<!-- NEW", 0) != string::npos)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								combinelines.push_back(originallines[linecount]);
								++linecount;

								while (true)
								{
									if (originallines[linecount].find("<!-- CLOSE -->", 0) != string::npos)
									{
										combinelines.push_back(originallines[linecount]);
										++linecount;
										break;
									}
									else
									{
										combinelines.push_back(originallines[linecount]);
										++linecount;
									}
								}
							}

							if (storeline[k].find("<!-- CONDITION END -->") != string::npos)
							{
								combinelines.push_back(storeline[k]);
								conditionOri = false;
								conditionOpen[conditionLvl] = false;
								--conditionLvl;
							}
							else if (conditionOri || !conditionOpen[conditionLvl])
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
							}
							else
							{
								condition[conditionLvl].push_back(storeline[k]);
							}
						}
						else if (close && storeline[k].find("<!-- CLOSE -->", 0) != string::npos)
						{
							if (originallines[linecount].find("<!-- NEW", 0) != string::npos)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								combinelines.push_back(originallines[linecount]);
								++linecount;

								while (true)
								{
									if (originallines[linecount].find("<!-- CLOSE -->", 0) != string::npos)
									{
										combinelines.push_back(originallines[linecount]);
										++linecount;
										break;
									}
									else
									{
										combinelines.push_back(originallines[linecount]);
										++linecount;
									}
								}
							}

							if (isHeader)
							{
								newlines.push_back(storeline[k]);
							}
							else
							{
								size_t startline = combinelines.size() + 1;
								combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
								combinelines.push_back(storeline[k]);
								newlines.clear();

								for (unsigned int j = startline; j < combinelines.size() - 1; ++j)
								{
									namespace AD = AnimDataFormat;
									AD::position position = AnimDataPosition(storeline, characterfile, filename, modcode, linecount, type);

									if (error)
									{
										return false;
									}

									if (position != AD::behaviorfilelist && position != AD::eventnamelist && position != AD::motiondatalist && position != AD::rotationdatalist)
									{
										cout << "ERROR(3018): Wrong format. Current line cannot be used to run NEW tab function. Only BehaviorFileList, EventNameList, MotionDataList, or RotationDataList can use NEW tab function. Please contact the mod author" << endl << "Mod: " << modcode << endl << "Character: " << characterfile << endl << "Header: " << filename << endl << endl;
										error = true;
										return false;
									}
								}
							}

							close = false;
						}
						else
						{
							close = true;
						}

						if (storeline[k].find("<!-- CONDITION -->") != string::npos)
						{
							conditionOri = true;
						}

						if (close)
						{
							newlines.push_back(storeline[k]);
						}
					}

					// for $header$
					if (isHeader && newlines.size() > 0)
					{
						combinelines.insert(combinelines.end(), newlines.begin(), newlines.end());
						newlines.clear();
					}

					combinelines.shrink_to_fit();
					newAnimData[characterfile][filename] = combinelines;
				}
				else
				{
					if (filename.find(modcode + "$") != string::npos)
					{
						string tempID;
						size_t partCount = fileparts.size();

						if (partCount < 3 && partCount != 0)
						{
							if (partCount == 1)
							{
								tempID = boost::regex_replace(string(fileparts[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								if (fileparts[0] != modcode + "$" + tempID || (!isOnlyNumber(fileparts[0]) && !boost::iequals(fileparts[0], "$header$") && !boost::iequals(fileparts[0], "$info header$")))
								{
									cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << filepath << endl << endl;
									error = true;
									return false;
								}
							}
							else if (partCount == 2)
							{
								tempID = boost::regex_replace(string(fileparts[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								if (fileparts[1] != modcode + "$" + tempID || (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
								{
									cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << filepath << endl << endl;
									error = true;
									return false;
								}
							}
							
							newAnimData[characterfile][filename] = storeline;
						}
						else
						{
							cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << filepath << endl << endl;
							error = true;
							return false;
						}
					}
					else
					{
						cout << "ERROR(2004): Invalid file name. File name must only contain  either <modcode>$<id>, <id>, \"$header$\", \"$info header$\" or <animation clip name> <id>. Please contact the mod author" << endl << "File: " << filepath << endl << endl;
						error = true;
						return false;
					}
				}
			}
		}
	}

	return true;
}

void JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{	
	if (CreateDirectory(directory.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		vecstr filelist;
		vector<thread> threads;
		read_directory(directory, filelist);

		for (unsigned int i = 0; i < filelist.size(); ++i)
		{
			boost::filesystem::path curPath(directory + filelist[i]);

			if (boost::filesystem::is_directory(curPath))
			{
				vecstr filelist2;
				read_directory(directory + filelist[i] + "\\", filelist2);
				threads.emplace_back(SeparateMod, directory, filelist[i], filelist2, ref(newFile), ref(newAnimData), ref(animDataChar), ref(animDataHeader));
			}
		}

		for (auto& th: threads)
		{
			th.join();
		}
	}
}

void CombiningFiles(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
	vecstr fileline;
	string compilingfolder = "temp_behaviors\\";
	
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

		if (CreateDirectory(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			string filename = compilingfolder + boost::algorithm::to_lower_copy(it->first) + ".txt";
			ofstream output(filename);

			if (output.is_open())
			{
				FunctionWriter writeoutput(&output);

				writeoutput << "<?xml version=\"1.0\" encoding=\"ascii\"?>" << "\n";
				writeoutput << "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" << rootID << "\">" << "\n";
				writeoutput << "\n";
				writeoutput << "	<hksection name=\"__data__\">" << "\n";
				writeoutput << "\n";

				for (unsigned int linecount = 0; linecount < fileline.size(); ++linecount)
				{
					writeoutput << fileline[linecount] << "\n";
				}

				writeoutput << "	</hksection>" << "\n";
				writeoutput << "\n";
				writeoutput << "</hkpackfile>" << "\n";
				fileline.clear();
				output.close();
			}
			else
			{
				cout << "ERROR(2600): Unable to create file" << endl << "File: " << filename << endl << endl;
				error = true;
				return;
			}
		}
	}


	if (CreateDirectory(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		string filename = compilingfolder + "animationdatasinglefile.txt";
		ofstream output(filename);
		ofstream outputlist("animationdata_list.txt");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				FunctionWriter writeoutput(&output);
				FunctionWriter fwriter(&outputlist);

				for (unsigned int i = 0; i < animDataChar.size(); ++i)
				{
					string character = animDataChar[i];
					fwriter << character << "\n";		// character

					for (unsigned int j = 0; j < animDataHeader[character].size(); ++j)
					{
						string header = animDataHeader[character][j];
						fwriter << header << "\n";

						for (unsigned int k = 0; k < newAnimData[character][header].size(); ++k)
						{
							writeoutput << newAnimData[character][header][k] << "\n";
						}

						if (j != 0 && header != "$info header$" && character != "$end$" && newAnimData[character][header].back().length() != 0)
						{
							writeoutput << "\n";
						}
					}

					fwriter << "\n";
				}

				output.close();
				outputlist.close();
			}
			else
			{
				cout << "ERROR(2600): Unable to create file" << endl << "File: animationdata_list.txt" << endl << endl;
				error = true;
				return;
			}
		}
		else
		{
			cout << "ERROR(2600): Unable to create file" << endl << "File: " << filename << endl << endl;
			error = true;
			return;
		}
	}
}
