#include "dataunification.h"

#pragma warning(disable:4503)

using namespace std;

void SeparateMod(string directory, string modecode, vecstr behaviorfilelist, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newAnimDataSet, vecstr& projectList)
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
					ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");
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
								if (error)
								{
									return;
								}
							}
						}
					}
				}
			}
			else if (boost::iequals(behaviorfilelist[j], "animationsetdatasinglefile"))
			{
				if (newAnimDataSet.size() == 0)
				{
					ErrorMessage(3017, "nemesis_animationsetdatasinglefile.txt");
					return;
				}

				for (unsigned int k = 0; k < nodelist.size(); ++k)
				{
					boost::filesystem::path curPath(directory + modecode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k]);

					if (boost::filesystem::is_directory(curPath) && wordFind(nodelist[k], "data") != NOT_FOUND)
					{
						bool newChar = false;
						string project = nodelist[k] + "\\" + nodelist[k].substr(0, wordFind(nodelist[k], "data", true)) + ".txt";

						if (newAnimDataSet.find(project) == newAnimDataSet.end())
						{
							projectList.push_back(project);
							newChar = true;
						}

						vecstr uniquecodelist;
						string filepath = directory + modecode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k];
						read_directory(filepath, uniquecodelist);

						for (unsigned int i = 0; i < uniquecodelist.size(); ++i)
						{
							if (!AnimDataSetUpdate(modecode, behaviorfilelist[j], nodelist[k], project, filepath + "\\" + uniquecodelist[k], newAnimDataSet, projectList, newChar))
							{
								if (error)
								{
									return;
								}
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
						if (error)
						{
							return;
						}
					}
				}
			}
		}
	}
}

bool newAnimUpdate(string sourcefolder, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newAnimDataSet, vecstr& projectList)
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
						else if (boost::iequals(behaviorlist[j], "animationsetdatasinglefile"))
						{
							vecstr projectfile;
							read_directory(folderpath + "\\" + behaviorlist[j], projectfile);

							for (unsigned int k = 0; k < projectfile.size(); ++k)
							{
								boost::filesystem::path characterfolder(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k]);

								if (boost::filesystem::is_directory(characterfolder) && wordFind(projectfile[k], "data") == projectfile[k].length() - 4)
								{
									if (!newAnimDataSetUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k], codelist[i], projectfile[k], newAnimDataSet, projectList))
									{
										return false;
									}
								}
							}
						}
						else
						{
							if (!newAnimUpdateExt(folderpath, codelist[i], behaviorlist[j], newFile))
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
		ErrorMessage(2010, sourcefolder);
		return false;
	}

	return true;
}

bool newAnimUpdateExt(string folderpath, string modcode, string behaviorfile, unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr nodelist;
	read_directory(folderpath + "\\" + behaviorfile, nodelist);

	for (unsigned int k = 0; k < nodelist.size(); ++k)
	{
		boost::filesystem::path curfile(folderpath + "\\" + behaviorfile + "\\" + nodelist[k]);

		if (nodelist[k][0] == '#' && !boost::filesystem::is_directory(curfile))
		{
			string line;
			vecstr storeline;
			GetFunctionLines(folderpath + "\\" + behaviorfile + "\\" + nodelist[k], storeline);

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

			if (originallines.size() == 0)
			{
				ErrorMessage(1170, modcode, curfile.string());
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
				if (storeline[k].find("<!-- CONDITION START ") != NOT_FOUND)
				{
					++conditionLvl;
					conditionOpen[conditionLvl] = true;
				}

				if (storeline[k].find("<!-- NEW", 0) == NOT_FOUND && !close)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						combinelines.push_back(originallines[linecount]);
						++linecount;

						while (true)
						{
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
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
					else if (storeline[k].find("<!-- ", 0) != NOT_FOUND && storeline[k].find("numelements +", storeline[k].find("<!-- ")) != NOT_FOUND)
					{
						size_t position = storeline[k].find("<!-- ");
						string templine = storeline[k].substr(position, storeline[k].find("-->", position) - position + 3);

						if (originallines[linecount].find("<!-- ", 0) != NOT_FOUND && originallines[linecount].find("-->", 0) != NOT_FOUND)
						{
							originallines[linecount].append(" " + templine);
						}
						else
						{
							originallines[linecount].append("			" + templine);
						}
					}

					if (storeline[k].find("<!-- CONDITION END -->") != NOT_FOUND)
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
				else if (close && storeline[k].find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
					{
						combinelines.push_back(originallines[linecount]);
						++linecount;
						combinelines.push_back(originallines[linecount]);
						++linecount;

						while (true)
						{
							if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
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

				if (storeline[k].find("<!-- CONDITION -->") != NOT_FOUND)
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
					ErrorMessage(2004, filepath);
					return false;
				}
				else if (fileparts.size() == 2 && (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
				{
					ErrorMessage(2004, filepath);
					return false;
				}

				string line;
				vecstr storeline;
				GetFunctionLines(filepath, storeline);

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
								if (boost::iequals(animDataHeader[characterfile][i], "$info header$") || animDataHeader[characterfile][i].find(" ") == NOT_FOUND)
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
									ErrorMessage(3012, filepath);
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
								if (animDataHeader[characterfile][i].find(" ") == NOT_FOUND)
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
						ErrorMessage(3006, characterfile, fileparts[0]);
						return false;
					}

					for (unsigned int k = 0; k < storeline.size(); ++k)
					{
						if (storeline[k].find("<!-- CONDITION START ") != NOT_FOUND)
						{
							++conditionLvl;
							conditionOpen[conditionLvl] = true;
						}

						if (storeline[k].find("<!-- NEW", 0) == NOT_FOUND && !close)
						{
							if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								combinelines.push_back(originallines[linecount]);
								++linecount;

								while (true)
								{
									if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
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

							if (storeline[k].find("<!-- CONDITION END -->") != NOT_FOUND)
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
						else if (close && storeline[k].find("<!-- CLOSE -->", 0) != NOT_FOUND)
						{
							if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
							{
								combinelines.push_back(originallines[linecount]);
								++linecount;
								combinelines.push_back(originallines[linecount]);
								++linecount;

								while (true)
								{
									if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
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
										ErrorMessage(3018, modcode, characterfile, filename);
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

						if (storeline[k].find("<!-- CONDITION -->") != NOT_FOUND)
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
					if (filename.find(modcode + "$") != NOT_FOUND)
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
									ErrorMessage(2004, filepath);
									return false;
								}
							}
							else if (partCount == 2)
							{
								tempID = boost::regex_replace(string(fileparts[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

								if (fileparts[1] != modcode + "$" + tempID || (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
								{
									ErrorMessage(2004, filepath);
									return false;
								}
							}
							
							// newAnimData[characterfile][filename] = storeline;
						}
						else
						{
							ErrorMessage(2004, filepath);
							return false;
						}
					}
					else
					{
						ErrorMessage(2004, filepath);
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool newAnimDataSetUpdateExt(string folderpath, string modcode, string projectfile, unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newAnimDataSet, vecstr& projectList)
{
	vecstr datalist;
	vecstr headerfile;
	read_directory(folderpath, headerfile);
	string project = projectfile + "\\" + projectfile.substr(0, wordFind(projectfile, "data", true)) + ".txt";
	bool isNew = false;

	if (newAnimDataSet.find(project) == newAnimDataSet.end())
	{
		isNew = true;
		// project = "$" + project + "=" + modcode + "$";
		projectList.push_back(project);
	}

	for (unsigned int k = 0; k < headerfile.size(); ++k)
	{
		string filename = folderpath + "\\" + headerfile[k];
		string lowerheader = boost::to_lower_copy(headerfile[k]);
		boost::filesystem::path curfile(filename);

		if (!boost::filesystem::is_directory(curfile) && curfile.stem().string() == ".txt")
		{
			vecstr storeline;
			GetFunctionLines(filename, storeline);

			if (error)
			{
				return false;
			}

			if (isNew || newAnimDataSet[project].find(lowerheader) == newAnimDataSet[project].end())
			{
				isNew = true;
			}

			if (!isNew)
			{
				vecstr originallines = newAnimDataSet[project][lowerheader];
				bool close = false;
				unordered_map<int, bool> conditionOpen;
				bool conditionOri = false;
				int linecount = 0;
				int conditionLvl = 0;
				vecstr newlines;
				vecstr combinelines;

				for (unsigned int k = 0; k < storeline.size(); ++k)
				{
					if (storeline[k].find("<!-- CONDITION START ") != NOT_FOUND)
					{
						++conditionLvl;
						conditionOpen[conditionLvl] = true;
					}

					if (storeline[k].find("<!-- NEW", 0) == NOT_FOUND && !close)
					{
						if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
						{
							combinelines.push_back(originallines[linecount]);
							++linecount;
							combinelines.push_back(originallines[linecount]);
							++linecount;

							while (true)
							{
								if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
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

						if (storeline[k].find("<!-- CONDITION END -->") != NOT_FOUND)
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
					else if (close && storeline[k].find("<!-- CLOSE -->", 0) != NOT_FOUND)
					{
						if (originallines[linecount].find("<!-- NEW", 0) != NOT_FOUND)
						{
							combinelines.push_back(originallines[linecount]);
							++linecount;
							combinelines.push_back(originallines[linecount]);
							++linecount;

							while (true)
							{
								if (originallines[linecount].find("<!-- CLOSE -->", 0) != NOT_FOUND)
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

					if (storeline[k].find("<!-- CONDITION -->") != NOT_FOUND)
					{
						conditionOri = true;
					}

					if (close)
					{
						newlines.push_back(storeline[k]);
					}
				}

				newAnimDataSet[project][lowerheader] = combinelines;
			}
			else
			{
				newAnimDataSet[project][lowerheader] = storeline;
			}
		}
	}

	return true;
}

void JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newAnimDataSet, vecstr& projectList)
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
				threads.emplace_back(SeparateMod, directory, filelist[i], filelist2, ref(newFile), ref(newAnimData), ref(animDataChar), ref(animDataHeader), ref(newAnimDataSet), ref(projectList));
			}
		}

		for (auto& th: threads)
		{
			th.join();
		}
	}
}

void CombiningFiles(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newAnimDataSet, vecstr& projectList)
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
			if (iter->first.find("$") != NOT_FOUND)
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

					if (line.find("class=\"hkRootLevelContainer\" signature=\"0x2772c11e\">", 0) != NOT_FOUND)
					{
						rootID = "#" + boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
					}

					fileline.push_back(line);
				}
			}
			else
			{
				ErrorMessage(2008, it->first + "(" + iter->first + ")");
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
			string lowerBehaviorFile = boost::algorithm::to_lower_copy(it->first);
			string filename = compilingfolder + lowerBehaviorFile + ".txt";
			ofstream output(filename);

			if (output.is_open())
			{
				FunctionWriter writeoutput(&output);
				bool behaviorRef = false;

				writeoutput << "<?xml version=\"1.0\" encoding=\"ascii\"?>" << "\n";
				writeoutput << "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" << rootID << "\">" << "\n";
				writeoutput << "\n";
				writeoutput << "	<hksection name=\"__data__\">" << "\n";
				writeoutput << "\n";

				for (unsigned int linecount = 0; linecount < fileline.size(); ++linecount)
				{
					writeoutput << fileline[linecount] << "\n";
					size_t pos = fileline[linecount].find("<hkobject name=\"");

					if (pos != NOT_FOUND && fileline[linecount].find("signature=\"", pos) != NOT_FOUND)
					{
						if (fileline[linecount].find("class=\"hkbBehaviorReferenceGenerator\" signature=\"", pos) != NOT_FOUND)
						{
							behaviorRef = true;
						}
						else
						{
							behaviorRef = false;
						}
					}
					
					if (behaviorRef && fileline[linecount].find("<hkparam name=\"behaviorName\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorName\">") + 14;
						string behaviorName = GetFileName(fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos));
						boost::algorithm::to_lower(behaviorName);
						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
						behaviorRef = false;
					}
					else if (fileline[linecount].find("<hkparam name=\"behaviorFilename\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorFilename\">") + 18;
						string behaviorName = fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos);
						behaviorName = GetFileName(behaviorName);
						boost::algorithm::to_lower(behaviorName);
						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
					}
				}

				writeoutput << "	</hksection>" << "\n";
				writeoutput << "\n";
				writeoutput << "</hkpackfile>" << "\n";
				fileline.clear();
				output.close();
			}
			else
			{
				ErrorMessage(2009, filename);
				return;
			}
		}
	}

	behaviorJointsOutput();

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
				ErrorMessage(2009, "animationdata_list.txt");
				return;
			}
		}
		else
		{
			ErrorMessage(2009, filename);
			return;
		}
	}
	
	if (CreateDirectory(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		string filename = compilingfolder + "animationsetdatasinglefile.txt";
		ofstream output(filename);
		ofstream outputlist("animationsetdata_list.txt");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				FunctionWriter writeoutput(&output);
				FunctionWriter fwriter(&outputlist);

				for (unsigned int i = 0; i < projectList.size(); ++i)
				{
					string project = projectList[i];
					fwriter << project << "\n";		// character

					for (auto it = newAnimDataSet[project].begin(); it != newAnimDataSet[project].end(); ++it)
					{
						string header = it->first;
						fwriter << header << "\n";

						for (unsigned int k = 0; k < it->second.size(); ++k)
						{
							writeoutput << it->second[k] << "\n";
						}
					}

					fwriter << "\n";
				}

				output.close();
				outputlist.close();
			}
			else
			{
				ErrorMessage(2009, "animationsetdata_list.txt");
				return;
			}
		}
		else
		{
			ErrorMessage(2009, filename);
			return;
		}
	}
}

void behaviorJointsOutput()
{
	unordered_map<string, vector<string>> combinedBehaviorJoints;

	for (auto it = behaviorJoints.begin(); it != behaviorJoints.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			vecstr temp = it->second;

			while (!behaviorJoints[temp[i]].empty())
			{
				temp = behaviorJoints[temp[i]];
			}

			combinedBehaviorJoints[it->first] = temp;
			break;
		}
	}

	ofstream output("behavior_joints.txt");

	if (output.is_open())
	{
		FunctionWriter fwriter(&output);

		for (auto it : combinedBehaviorJoints)
		{
			fwriter << it.first << "\n";

			for (unsigned int i = 0; i < it.second.size(); ++i)
			{
				fwriter << it.second[i] << "\n";
			}

			fwriter << "\n";
		}

		output.close();
	}
	else
	{
		ErrorMessage(2009, "behavior_joint.txt");
		return;
	}
}
