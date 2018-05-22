#include "functionupdate.h"

#pragma warning(disable:4503)

using namespace std;

typedef vector<string> vecstr;

unordered_map<string, mutex> locker;

string GetFunctionEdits(vecstr storeline, int numline)
{
	if (numline < int(storeline.size()))
	{
		return storeline[numline];
	}

	error = true;
	return "null";
}

vecstr GetFunctionEdits(string filename, vecstr storeline, int startline, int endline)
{	
	vecstr storage;
	storage.reserve(endline);

	if (int(storeline.size()) > endline + startline)
	{
		for (int i = startline; i < endline + startline; ++i)
		{
			storage.push_back(storeline[i]);
		}

		return storage;
	}
	else
	{
		ErrorMessage(2002, filename, startline, startline + endline);
		return storage;
	}
}

bool FunctionUpdate(string modcode, string behaviorfile, string nodefile, unordered_map<string, map<string, vecstr>>& newFile)
{
	lock_guard<mutex> filelocker(locker[behaviorfile + nodefile]);

	if (behaviorPath[behaviorfile].empty())
	{
		ErrorMessage(2006, behaviorfile);
		return false;
	}

	string filecheck = boost::regex_replace(string(nodefile), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")) + ".txt";
	string nodeID = nodefile.substr(0, nodefile.find_last_of("."));

	if ("#" + filecheck == nodefile)
	{
		vector<int> modEditCoordinate;
		unordered_map<string, string> modPath;
		unordered_map<string, vecstr> modEdits;
		unordered_map<string, int> modEditLine;
		unordered_map<int, int> NewCoordinate;
		unordered_map<int, int> Pair;

		int curline = 0;
		int editline = 0;
		int originalline = 0;
		int linecount = 0;
		int coordinate = 0;

		bool edited = false;
		bool originalopen = false;

		int startoriline = 0;
		int starteditline = 0;
		int variablecount;
		int eventcount;
		int attributecount;
		int characterpropertycount;

		vecstr storeline;
		string line;
		string filename = "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile;
		char charline[2000];
		FILE* BehaviorFormat;
		fopen_s(&BehaviorFormat, filename.c_str(), "r");

		if (BehaviorFormat)
		{
			bool IsEventVariable = false;

			while (fgets(charline, 2000, BehaviorFormat))
			{
				line = charline;

				if (line.back() == '\n')
				{
					line.pop_back();
				}
				
				if (line.find("hkbBehaviorGraphStringData", 0) != NOT_FOUND || line.find("hkbVariableValueSet", 0) != NOT_FOUND || line.find("hkbBehaviorGraphData", 0) != NOT_FOUND)
				{
					IsEventVariable = true;
				}

				if ((line.find("<!-- MOD_CODE", 0) != NOT_FOUND) && (line.find("OPEN -->", 0) != NOT_FOUND) && (!edited))
				{
					edited = true;
					editline = linecount;
					starteditline = linecount;
				}
				else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
				{
					edited = false;
					originalopen = true;
					originalline = linecount;
					startoriline = linecount;
				}
				else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					edited = false;

					if (originalopen)
					{
						int tempint = editline - originalline;
						NewCoordinate[linecount] = tempint;
						modEditLine[to_string(linecount) + "R"] = coordinate - tempint - 2;

						for (int i = startoriline; i < originalline; ++i)
						{
							modEditCoordinate.push_back(i);
							modEditLine[to_string(i)] = Pair[i];
						}

						originalopen = false;
					}
					else
					{
						int tempint = editline - starteditline;
						NewCoordinate[linecount] = tempint;
						modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
					}
				}
				else if (edited)
				{
					if (IsEventVariable && line.find("numelements=", 0) != NOT_FOUND)
					{
						if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != NOT_FOUND)
						{
							eventcount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
						else if (line.find("<hkparam name=\"attributeNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"attributeDefaults\" numelements=", 0) != NOT_FOUND)
						{
							attributecount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
						else if (line.find("<hkparam name=\"variableNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"wordVariableValues\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"variableInfos\" numelements=", 0) != NOT_FOUND)
						{
							variablecount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
						else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"characterPropertyInfos\" numelements=", 0) != NOT_FOUND)
						{
							characterpropertycount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
					}

					Pair[editline] = coordinate;
					++editline;
				}
				else if (!edited)
				{
					++linecount;
					++originalline;
				}

				storeline.push_back(line);
				coordinate++;
			}

			fclose(BehaviorFormat);
		}
		else
		{

			ErrorMessage(2000, filename);
			return false;
		}

		if (NewCoordinate.size() == 0)
		{
			WarningMessage(1017, filename);
			return false;
		}

		vecstr newline = newFile[behaviorfile][nodeID];
		vecstr functionline;
		functionline.reserve(newline.size());
		linecount = 0;
		int editcount = 0;
		bool skip = false;

		if (newline.size() != 0)
		{
			for (unsigned int i = 0; i < newline.size(); ++i)
			{
				line = newline[i];

				if (line.find("<!-- NEW", 0) != NOT_FOUND)
				{
					skip = true;
				}

				if (line.find("<!-- *", 0) == NOT_FOUND && !skip)
				{
					if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
					{
						if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != NOT_FOUND)
						{
							int tempint = eventcount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- EVENT numelement ", 0) != NOT_FOUND)
							{
								line.append(" <!-- EVENT numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("   				<!-- EVENT numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("<hkparam name=\"attributeNames\" numelements=", 0) != NOT_FOUND)
						{
							int tempint = attributecount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- ATTRIBUTE numelement ", 0) != NOT_FOUND)
							{
								line.append(" <!-- ATTRIBUTE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("   				<!-- ATTRIBUTE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("<hkparam name=\"variableNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"wordVariableValues\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"variableInfos\" numelements=", 0) != NOT_FOUND)
						{
							int tempint = variablecount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
							
							if (line.find("<!-- VARIABLE numelement ", 0) != NOT_FOUND)
							{
								line.append(" <!-- VARIABLE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("				<!-- VARIABLE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"characterPropertyInfos\" numelements=", 0) != NOT_FOUND)
						{
							int tempint = characterpropertycount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- CHARACTER numelement ", 0) != NOT_FOUND)
							{
								line.append(" <!-- CHARACTER numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("				<!-- CHARACTER numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("numelements=\"", 0) != NOT_FOUND)
						{
							string templine = GetFunctionEdits(storeline, modEditLine[to_string(linecount)]);

							if (error)
							{
								ErrorMessage(2005, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile, modEditLine[to_string(linecount)]);
								return false;
							}

							int difference = stoi(boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"))) - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- numelement *", 0) != NOT_FOUND)
							{
								line.append(" <!-- numelement *" + modcode + "* +" + to_string(difference) + "-->");
							}
							else
							{
								line.append("					<!-- numelement *" + modcode + "* +" + to_string(difference) + "-->");
							}
						}
						else
						{
							string templine = GetFunctionEdits(storeline, modEditLine[to_string(linecount)]);

							if (error)
							{
								ErrorMessage(2005, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile, modEditLine[to_string(linecount)]);
								return false;
							}

							if (templine.find("			<hkobject>") == NOT_FOUND && templine.find("			</hkobject>") == NOT_FOUND && templine.find("			<hkparam>") == NOT_FOUND && templine.find("			<hkparam>") == NOT_FOUND)
							{
								templine.append("					<!-- *" + modcode + "* -->");
								functionline.push_back(templine);

								if (line.find("<!-- original -->", 0) == NOT_FOUND)
								{
									line.append("				<!-- original -->");
								}
							}

						}

						if (editcount != modEditCoordinate.size() - 1)
						{
							editcount++;
						}
					}
					else if (NewCoordinate[linecount] > 0)
					{
						functionline.push_back("<!-- NEW *" + modcode + "* -->");

						vecstr storage = GetFunctionEdits("mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

						if (!error)
						{
							for (unsigned int j = 0; j < storage.size(); ++j)
							{
								functionline.push_back(storage[j]);
							}

							functionline.push_back("<!-- CLOSE -->");
						}
						else
						{
							return false;
						}
					}

					++linecount;
				}

				if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					skip = false;
				}

				functionline.push_back(line);
			}
		}
		else
		{
			ErrorMessage(2001, nodeID);
			return false;
		}

		functionline.shrink_to_fit();
		newFile[behaviorfile][nodeID] = functionline;
	}
	else if (nodefile.find("#") != NOT_FOUND && nodefile.find("$") != NOT_FOUND)
	{
		if (nodefile == "#" + modcode + "$" + filecheck)
		{
			GetFunctionLines("mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile, newFile[behaviorfile][nodeID]);
		}
		else
		{
			ErrorMessage(2003, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile);
			return false;
		}
	}
	else
	{
		ErrorMessage(2003, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile);
		return false;
	}

	return true;
}

bool AnimDataUpdate(string modcode, string animdatafile, string characterfile, string filepath, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, bool isNewCharacter)
{
	if (behaviorPath[animdatafile].empty())
	{
		ErrorMessage(2007, animdatafile);
		return false;
	}

	string filename = GetFileName(filepath);

	if (isNewCharacter)
	{
		GetFunctionLines(filepath, newAnimData[characterfile][filename]);
		animDataHeader[characterfile].push_back(filename);
	}
	else
	{
		vector<int> modEditCoordinate;
		unordered_map<string, int> modEditLine;
		unordered_map<int, int> NewCoordinate;
		unordered_map<int, int> Pair;

		bool edited = false;
		bool originalopen = false;

		int coordinate = 0;
		int startoriline = 0;
		int starteditline = 0;
		int editline = 0;
		int originalline = 0;
		int linecount = 0;
		int type;

		stringstream sstream(filename);
		istream_iterator<string> ssbegin(sstream);
		istream_iterator<string> ssend;
		vecstr fileparts(ssbegin, ssend);
		copy(fileparts.begin(), fileparts.end(), fileparts.begin());
		vecstr storeline;
		GetFunctionLines(filepath, storeline);
		string line;

		for (unsigned int i = 0; i < storeline.size(); ++i)
		{
			line = storeline[i];

			if ((line.find("<!-- MOD_CODE", 0) != NOT_FOUND) && (line.find("OPEN -->", 0) != NOT_FOUND) && (!edited))
			{
				edited = true;
				editline = linecount;
				starteditline = linecount;
			}
			else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
			{
				edited = false;
				originalopen = true;
				originalline = linecount;
				startoriline = linecount;
			}
			else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
			{
				edited = false;

				if (originalopen)
				{
					int tempint = editline - originalline;
					NewCoordinate[linecount] = tempint;
					modEditLine[to_string(linecount) + "R"] = coordinate - tempint - 2;

					for (int i = startoriline; i < originalline; ++i)
					{
						modEditCoordinate.push_back(i);
						modEditLine[to_string(i)] = Pair[i];
					}

					originalopen = false;
				}
				else
				{
					int tempint = editline - starteditline;
					NewCoordinate[linecount] = tempint;
					modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
				}
			}
			else if (edited)
			{
				Pair[editline] = coordinate;
				++editline;
			}
			else if (!edited)
			{
				++linecount;
				++originalline;
			}

			coordinate++;
		}

		if (NewCoordinate.size() == 0)
		{
			WarningMessage(1017, filepath);
			return false;
		}

		if (newAnimData[characterfile].find(filename) != newAnimData[characterfile].end())
		{
			vecstr newline = newAnimData[characterfile][filename];
			vecstr functionline;
			vecstr headerline;
			vector<vector<int>> newchecker;

			int editcount = 0;

			bool skip = false;
			bool isHeader = false;

			functionline.reserve(newline.size());
			linecount = 0;

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

			for (unsigned int i = 0; i < newline.size(); ++i)
			{
				line = newline[i];

				if (line.find("<!-- NEW", 0) != NOT_FOUND)
				{
					skip = true;
				}

				if (line.find("<!-- *", 0) == NOT_FOUND && !skip)
				{
					if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
					{
						string templine = GetFunctionEdits(storeline, modEditLine[to_string(linecount)]);

						if (error)
						{
							ErrorMessage(2005, filepath, modEditLine[to_string(linecount)]);
							return false;
						}

						int position = AnimDataPosition(storeline, characterfile, filename, modcode, linecount, type);

						if (error)
						{
							return false;
						}

						if (type == 0)
						{
							if (position == 3)		// behavior file count
							{
								position = -1;
							}
						}
						else if (type == 1)
						{
							if (position == 6)		// event name count
							{
								position = -1;
							}
						}
						else if (type == 2)
						{
							if (position == 3 || position == 5)		// motion data count or rotation data count
							{
								position = -1;
							}
						}

						if (position != -1)
						{
							templine.append("					<!-- *" + modcode + "* -->");
							functionline.push_back(templine);

							if (line.find("<!-- original -->", 0) == NOT_FOUND)
							{
								line.append("				<!-- original -->");
							}

							if (editcount != modEditCoordinate.size() - 1)
							{
								editcount++;
							}
						}
						else
						{
							line = "$elements$";
						}
					}
					else if (NewCoordinate[linecount] > 0)
					{
						if (isHeader)
						{
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								headerline.push_back("<!--NEW *" + modcode + "* -->");
								headerline.insert(headerline.end(), storage.begin(), storage.end());
								headerline.push_back("<!-- CLOSE -->");
							}
							else
							{
								return false;
							}
						}
						else
						{
							vector<int> tempVI;
							functionline.push_back("<!-- NEW *" + modcode + "* -->");
							tempVI.push_back(functionline.size());
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								functionline.insert(functionline.end(), storage.begin(), storage.end());
								tempVI.push_back(functionline.size());
								functionline.push_back("<!-- CLOSE -->");
							}
							else
							{
								return false;
							}

							newchecker.push_back(tempVI);
						}
					}

					++linecount;
				}

				if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					skip = false;
				}

				functionline.push_back(line);
			}

			if (isHeader && headerline.size() > 0)
			{
				functionline.insert(functionline.end(), headerline.begin(), headerline.end());
			}

			for (unsigned int i = 0; i < newchecker.size(); ++i)
			{
				bool islast = false;
				using namespace AnimDataFormat;

				if (newchecker[i].size() != 2)
				{
					ErrorMessage(3016, characterfile, filename);
					return false;
				}

				for (int j = newchecker[i][0]; j < newchecker[i][1]; ++j)
				{
					position curPosition = AnimDataPosition(functionline, characterfile, filename, modcode, j, type);
					
					if (error)
					{
						return false;
					}

					if (curPosition != behaviorfilelist && curPosition != eventnamelist && curPosition != motiondatalist && curPosition != rotationdatalist)
					{
						ErrorMessage(3018, modcode, characterfile, filename);
						return false;
					}
				}
			}

			functionline.shrink_to_fit();
			newAnimData[characterfile][filename] = functionline;
		}
		else if (filename.find(modcode + "$") != NOT_FOUND)
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

					if(fileparts[1] != modcode + "$" + tempID || (!hasAlpha(fileparts[0]) || !isOnlyNumber(fileparts[1])))
					{
						ErrorMessage(2004, filepath);
						return false;
					}
				}
								
				GetFunctionLines(filepath, newAnimData[characterfile][filename]);
				animDataHeader[characterfile].push_back(filename);
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

	return true;
}

bool AnimDataSetUpdate(string modcode, string animdatasetfile, string projectsource, string projectfile, string filepath, unordered_map<string, map<string, vecstr, alphanum_less<string>>>& newAnimDataSet, vecstr& projectList, bool isNewProject)
{
	if (behaviorPath[animdatasetfile].empty())
	{
		ErrorMessage(2007, animdatasetfile);
		return false;
	}

	string filename = GetFileName(filepath);

	if (isNewProject)
	{		
		GetFunctionLines(filepath, newAnimDataSet[projectfile][filename]);
	}
	else
	{
		vector<int> modEditCoordinate;
		unordered_map<string, int> modEditLine;
		unordered_map<int, int> NewCoordinate;
		unordered_map<int, int> Pair;

		bool edited = false;
		bool originalopen = false;

		int coordinate = 0;
		int startoriline = 0;
		int starteditline = 0;
		int editline = 0;
		int originalline = 0;
		int linecount = 0;
		int type;

		stringstream sstream(filename);
		istream_iterator<string> ssbegin(sstream);
		istream_iterator<string> ssend;
		vecstr fileparts(ssbegin, ssend);
		copy(fileparts.begin(), fileparts.end(), fileparts.begin());
		vecstr storeline;
		GetFunctionLines(filepath, storeline);
		string line;

		for (unsigned int i = 0; i < storeline.size(); ++i)
		{
			line = storeline[i];

			if ((line.find("<!-- MOD_CODE", 0) != NOT_FOUND) && (line.find("OPEN -->", 0) != NOT_FOUND) && (!edited))
			{
				edited = true;
				editline = linecount;
				starteditline = linecount;
			}
			else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
			{
				edited = false;
				originalopen = true;
				originalline = linecount;
				startoriline = linecount;
			}
			else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
			{
				edited = false;

				if (originalopen)
				{
					int tempint = editline - originalline;
					NewCoordinate[linecount] = tempint;
					modEditLine[to_string(linecount) + "R"] = coordinate - tempint - 2;

					for (int i = startoriline; i < originalline; ++i)
					{
						modEditCoordinate.push_back(i);
						modEditLine[to_string(i)] = Pair[i];
					}

					originalopen = false;
				}
				else
				{
					int tempint = editline - starteditline;
					NewCoordinate[linecount] = tempint;
					modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
				}
			}
			else if (edited)
			{
				Pair[editline] = coordinate;
				++editline;
			}
			else if (!edited)
			{
				++linecount;
				++originalline;
			}

			coordinate++;
		}

		if (NewCoordinate.size() == 0)
		{
			WarningMessage(1017, filepath);
			return false;
		}

		if (newAnimDataSet[projectfile].find(filename) != newAnimDataSet[projectfile].end())
		{
			vecstr newline = newAnimDataSet[projectfile][filename];
			vecstr functionline;
			vecstr headerline;
			vector<vector<int>> newchecker;

			int editcount = 0;

			bool skip = false;
			bool isHeader = false;

			functionline.reserve(newline.size());
			linecount = 0;

			if (boost::iequals(projectsource, "$header$"))
			{
				isHeader = true;
			}
			
			for (unsigned int i = 0; i < newline.size(); ++i)
			{
				line = newline[i];

				if (line.find("<!-- NEW", 0) != NOT_FOUND)
				{
					skip = true;
				}

				if (line.find("<!-- *", 0) == NOT_FOUND && !skip)
				{
					if (modEditCoordinate.size() > 0 && modEditCoordinate[editcount] == linecount)
					{
						string templine = GetFunctionEdits(storeline, modEditLine[to_string(linecount)]);

						if (error)
						{
							ErrorMessage(2005, filepath, modEditLine[to_string(linecount)]);
							return false;
						}

						using namespace ASDFormat;
						position curPosition = ASDPosition(storeline, projectsource, filename, modcode, linecount, false);

						if (error)
						{
							return false;
						}
						
						if (curPosition != equipcount && curPosition != typecount && curPosition != animpackcount && curPosition != attackcount && curPosition != crc32count)
						{
							templine.append("					<!-- *" + modcode + "* -->");
							functionline.push_back(templine);

							if (line.find("<!-- original -->", 0) == NOT_FOUND)
							{
								line.append("				<!-- original -->");
							}

							if (editcount != modEditCoordinate.size() - 1)
							{
								editcount++;
							}
						}
						else
						{
							line = "$elements$";
						}
					}
					else if (NewCoordinate[linecount] > 0)
					{
						if (isHeader)
						{
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								headerline.push_back("<!--NEW *" + modcode + "* -->");
								headerline.insert(headerline.end(), storage.begin(), storage.end());
								headerline.push_back("<!-- CLOSE -->");
							}
							else
							{
								return false;
							}
						}
						else
						{
							vector<int> tempVI;
							functionline.push_back("<!-- NEW *" + modcode + "* -->");
							tempVI.push_back(functionline.size());
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								functionline.insert(functionline.end(), storage.begin(), storage.end());
								tempVI.push_back(functionline.size());
								functionline.push_back("<!-- CLOSE -->");
							}
							else
							{
								return false;
							}

							newchecker.push_back(tempVI);
						}
					}

					++linecount;
				}

				if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
				{
					skip = false;
				}

				functionline.push_back(line);
			}

			if (isHeader && storeline.size() > 0)
			{
				functionline.insert(functionline.end(), storeline.begin(), storeline.end());
			}

			for (unsigned int i = 0; i < newchecker.size(); ++i)
			{
				bool islast = false;
				bool attacknew = false;
				using namespace ASDFormat;

				if (newchecker[i].size() != 2)
				{
					ErrorMessage(5016, projectsource, filename);
					return false;
				}

				if (animpackname == ASDPosition(functionline, projectsource, filename, modcode, newchecker[i][0], true))
				{
					attacknew = true;
				}

				for (int j = newchecker[i][0]; j < newchecker[i][1]; ++j)
				{
					position curPosition;

					if (!islast)
					{
						curPosition = ASDPosition(functionline, projectsource, filename, modcode, j, false);

						if (error)
						{
							return false;
						}

						if (curPosition == crc32list)
						{
							islast = true;
						}
					}
					else
					{
						curPosition = crc32list;
					}

					if (curPosition != equiplist && curPosition != typelist && curPosition != attacklist && curPosition != crc32list)
					{
						if (attacknew)
						{
							if ((curPosition != animpackname && curPosition != unknown3 && curPosition != attackcount))
							{
								ErrorMessage(3018, modcode, projectsource, filename);
								return false;
							}
						}
						else
						{
							ErrorMessage(3018, modcode, projectsource, filename);
							return false;
						}
					}
				}
			}

			functionline.shrink_to_fit();
			newAnimDataSet[projectfile][filename] = functionline;
		}
		else if (filename.find(modcode + "$") != NOT_FOUND)
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

				GetFunctionLines(filepath, newAnimDataSet[projectfile][filename]);
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

	return true;
}
