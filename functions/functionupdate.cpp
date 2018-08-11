#include "functionupdate.h"
#include "lastupdate.h"
#include <boost\thread.hpp>
#include <atomic>

#pragma warning(disable:4503)

using namespace std;

atomic_flag filelock = ATOMIC_FLAG_INIT;

typedef vector<string> vecstr;

// mutex locker;		issue #61

bool CombineAnimData(string filepath, string filename, string characterfile, string modcode, vecstr& newline, vecstr& storeline, vector<int> modEditCoordinate,
	unordered_map<string, int> modEditLine, unordered_map<int, int> NewCoordinate, unordered_map<int, int> Pair, unordered_map<string, string>& lastUpdate);
bool ClassCheck(vector<int>& modEditCoordinate, unordered_map<string, int>& modEditLine, unordered_map<int, int>& NewCoordinate, int linecount, vecstr& storeline, string filepath,
	string projectfile, string filename, string modcode);

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
	int combine = endline + startline;

	if (int(storeline.size()) > combine)
	{
		for (int i = startline; i < combine; ++i)
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

bool FunctionUpdate(string modcode, string behaviorfile, string nodefile, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, string>& lastUpdate)
{
	// lock_guard <mutex> filelocker(locker);		issue #61

	while (filelock.test_and_set(memory_order_acquire));

	if (behaviorPath[behaviorfile].empty())
	{
		ErrorMessage(2006, behaviorfile);
		filelock.clear(memory_order_release);
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
		bool bigger = false;

		int startoriline = 0;
		int starteditline = 0;
		int variablecount;
		int eventcount;
		int attributecount;
		int characterpropertycount;

		vecstr storeline;
		string line;
		string filename = "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile;

		if (!saveLastUpdate(filename, lastUpdate))
		{
			return false;
		}

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
						int addLineCount = editline - originalline;
						NewCoordinate[linecount] = addLineCount;
						modEditLine[to_string(linecount) + "R"] = starteditline - startoriline + originalline + 1;

						for (int i = startoriline; i < originalline; ++i)
						{
							modEditCoordinate.push_back(i);
							modEditLine[to_string(i)] = Pair[i];
						}

						originalopen = false;
					}
					else
					{
						int addLineCount = editline - starteditline;
						NewCoordinate[linecount] = addLineCount;
						modEditLine[to_string(linecount) + "R"] = coordinate - addLineCount;
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
			filelock.clear(memory_order_release);
			return false;
		}

		if (NewCoordinate.size() == 0)
		{
			WarningMessage(1017, filename);
			filelock.clear(memory_order_release);
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
			for (auto& counter : NewCoordinate)
			{
				if (counter.first >= int(newline.size()))
				{
					bigger = true;
					break;
				}
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
								filelock.clear(memory_order_release);
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
								filelock.clear(memory_order_release);
								return false;
							}

							if (templine.find("			<hkobject>") != NOT_FOUND || templine.find("			</hkobject>") != NOT_FOUND ||
								templine.find("			<hkparam>") != NOT_FOUND || templine.find("			<hkparam>") != NOT_FOUND)
							{
								WarningMessage(1022, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile, modEditLine[to_string(linecount)]);
							}

							templine.append("					<!-- *" + modcode + "* -->");
							functionline.push_back(templine);

							if (line.find("<!-- original -->", 0) == NOT_FOUND)
							{
								line.append("				<!-- original -->");
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
							filelock.clear(memory_order_release);
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
			filelock.clear(memory_order_release);
			return false;
		}

		if (bigger)
		{
			for (unsigned int i = newline.size(); i < storeline.size(); ++i)
			{
				if (NewCoordinate[linecount] > 0)
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
						filelock.clear(memory_order_release);
						return false;
					}
				}

				++linecount;
			}
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
			filelock.clear(memory_order_release);
			return false;
		}
	}
	else
	{
		ErrorMessage(2003, "mod\\" + modcode + "\\" + behaviorfile + "\\" + nodefile);
		filelock.clear(memory_order_release);
		return false;
	}

	filelock.clear(memory_order_release);
	return true;
}

bool AnimDataUpdate(string modcode, string animdatafile, string characterfile, string filepath, MasterAnimData& animData,
	bool isNewCharacter, unordered_map<string, string>& lastUpdate)
{
	if (behaviorPath[animdatafile].empty())
	{
		ErrorMessage(2007, animdatafile);
		return false;
	}

	string filename = GetFileName(filepath);

	if (isNewCharacter)
	{
		if (filename.find(modcode + "$") == NOT_FOUND)
		{
			if (!saveLastUpdate(filepath, lastUpdate))
			{
				return false;
			}
		}

		GetFunctionLines(filepath, animData.newAnimData[characterfile][filename]);

		if (isOnlyNumber(filename))		// info data
		{
			animData.animDataInfo[characterfile].push_back(filename);
		}
		else if (filename.find("~") != NOT_FOUND)	// anim data
		{
			string tempname = boost::regex_replace(string(filename), boost::regex("[^~]*~([0-9]+)"), string("\\1"));

			if (tempname == filename)
			{
				ErrorMessage(2004, filepath);
				return false;
			}

			animData.animDataHeader[characterfile].push_back(filename);
		}
		else
		{
			ErrorMessage(2004, filepath);
			return false;
		}
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

		vecstr storeline;
		string line;

		GetFunctionLines(filepath, storeline);

		if (error)
		{
			return false;
		}

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

		if (animData.newAnimData[characterfile].find(filename) != animData.newAnimData[characterfile].end())
		{
			if (!saveLastUpdate(filepath, lastUpdate))
			{
				return false;
			}

			CombineAnimData(filepath, filename, characterfile, modcode, animData.newAnimData[characterfile][filename], storeline, modEditCoordinate, modEditLine, NewCoordinate, Pair, lastUpdate);
		}
		else if (filename.find(modcode + "$") != NOT_FOUND)
		{
			string tempID = boost::regex_replace(string(filename), boost::regex(modcode + "$([0-9]+)"), string("\\1"));

			if (filename != tempID && isOnlyNumber(tempID))
			{
				tempID = boost::regex_replace(string(filename), boost::regex(modcode + "$([0-9]+)$UC"), string("\\1"));

				if (filename == tempID)		// anim data
				{
					GetFunctionLines(filepath, animData.newAnimData[characterfile][filename]);
					animData.animDataHeader[characterfile].push_back(filename);
				}
				else		// info data
				{
					GetFunctionLines(filepath, animData.newAnimData[characterfile][filename]);
					animData.animDataInfo[characterfile].push_back(filename);
				}						
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

bool CombineAnimData(string filepath, string filename, string characterfile, string modcode, vecstr& newline, vecstr& storeline, vector<int> modEditCoordinate,
	unordered_map<string, int> modEditLine, unordered_map<int, int> NewCoordinate, unordered_map<int, int> Pair, unordered_map<string, string>& lastUpdate)
{
	string line;

	vecstr functionline;
	vecstr headerline;
	vector<vector<int>> newchecker;

	int linecount = 0;
	int editcount = 0;
	int type;

	bool skip = false;
	bool isHeader = false;
	bool bigger = false;

	functionline.reserve(newline.size());

	if (characterfile == "$header$")
	{
		isHeader = true;

		if (!saveLastUpdate(filepath, lastUpdate))
		{
			return false;
		}
	}
	else
	{
		if (filename == "$header$")
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
			ErrorMessage(3006, characterfile, filename);
			return false;
		}
	}

	for (auto& counter : NewCoordinate)
	{
		if (counter.first >= int(newline.size()))
		{
			bigger = true;
			break;
		}
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

				if (!isHeader)
				{
					int position = AnimDataPosition(storeline, characterfile, filename, modcode, modEditLine[to_string(linecount)], type);

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
				}
				else
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
			}
			else if (NewCoordinate[linecount] > 0)
			{
				if (isHeader)
				{
					vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

					if (!error)
					{
						headerline.push_back("<!-- NEW *" + modcode + "* -->");
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

	if (bigger)
	{
		for (unsigned int i = newline.size(); i < storeline.size(); ++i)
		{
			if (NewCoordinate[linecount] > 0)
			{
				if (isHeader)
				{
					vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

					if (!error)
					{
						headerline.push_back("<!-- NEW *" + modcode + "* -->");
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
	newline = functionline;
	return true;
}

bool AnimSetDataUpdate(string modcode, string animdatasetfile, string projectsource, string projectfile, string filepath, MasterAnimSetData& animSetData,
	bool isNewProject, unordered_map<string, string>& lastUpdate)
{
	if (behaviorPath[animdatasetfile].empty())
	{
		ErrorMessage(2007, animdatasetfile);
		return false;
	}

	string filename = GetFileName(filepath);
	string lowerfile = boost::to_lower_copy(filename);

	if (isNewProject)
	{		
		GetFunctionLines(filepath, animSetData.newAnimSetData[projectfile][lowerfile]);
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

		vecstr storeline;
		GetFunctionLines(filepath, storeline);
		string line;

		if (!saveLastUpdate(filepath, lastUpdate))
		{
			return false;
		}

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

		if (animSetData.newAnimSetData[projectfile].find(lowerfile) != animSetData.newAnimSetData[projectfile].end())
		{
			vecstr newline = animSetData.newAnimSetData[projectfile][lowerfile];
			vecstr functionline;
			vecstr headerline;

			int editcount = 0;

			bool skip = false;
			bool isHeader = false;
			bool bigger = false;

			functionline.reserve(newline.size());
			linecount = 0;

			if (boost::iequals(projectfile, "$header$"))
			{
				isHeader = true;
			}

			for (auto& counter : NewCoordinate)
			{
				if (counter.first >= int(newline.size()))
				{
					bigger = true;
					break;
				}
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
						position curPosition = ASDPosition(storeline, projectfile, filename, modcode, modEditLine[to_string(linecount)], false);

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
					}
					else if (NewCoordinate[linecount] > 0)
					{
						if (isHeader)
						{
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								headerline.push_back("<!-- NEW *" + modcode + "* -->");
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
							if (!ClassCheck(modEditCoordinate, modEditLine, NewCoordinate, linecount, storeline, filepath, projectfile, filename, modcode))
							{
								return false;
							}

							functionline.push_back("<!-- NEW *" + modcode + "* -->");
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								functionline.insert(functionline.end(), storage.begin(), storage.end());
								functionline.push_back("<!-- CLOSE -->");
							}
							else
							{
								return false;
							}
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

			if (bigger)
			{
				for (unsigned int i = newline.size(); i < storeline.size(); ++i)
				{
					if (NewCoordinate[linecount] > 0)
					{
						if (isHeader)
						{
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								headerline.push_back("<!-- NEW *" + modcode + "* -->");
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
							if (!ClassCheck(modEditCoordinate, modEditLine, NewCoordinate, linecount, storeline, filepath, projectfile, filename, modcode))
							{
								return false;
							}

							functionline.push_back("<!-- NEW *" + modcode + "* -->");
							vecstr storage = GetFunctionEdits(filepath, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[linecount]);

							if (!error)
							{
								functionline.insert(functionline.end(), storage.begin(), storage.end());
								functionline.push_back("<!-- CLOSE -->");
							}
							else
							{
								return false;
							}
						}
					}

					++linecount;
				}
			}

			if (isHeader && headerline.size() > 0)
			{
				functionline.insert(functionline.end(), headerline.begin(), headerline.end());
			}

			functionline.shrink_to_fit();
			animSetData.newAnimSetData[projectfile][lowerfile] = functionline;
		}
		else
		{
			ErrorMessage(2004, filepath);
			return false;
		}
	}

	return true;
}

bool ClassCheck(vector<int>& modEditCoordinate, unordered_map<string, int>& modEditLine, unordered_map<int, int>& NewCoordinate, int linecount, vecstr& storeline, string filepath,
	string projectfile,	string filename, string modcode)
{
	unsigned int endline = modEditLine[to_string(linecount) + "R"] + NewCoordinate[linecount];
	bool attacknew = false;
	bool islast = false;

	if (ASDFormat::animpackname == ASDPosition(storeline, projectfile, filename, modcode, modEditLine[to_string(linecount) + "R"], true))
	{
		attacknew = true;
	}

	for (unsigned int k = modEditLine[to_string(linecount) + "R"]; k < endline; ++k)
	{
		using namespace ASDFormat;

		position curPosition;

		if (!islast)
		{
			curPosition = ASDPosition(storeline, projectfile, filename, modcode, k, false);

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
					ErrorMessage(5021, modcode, filepath, k + 1);
					return false;
				}
			}
			else
			{
				ErrorMessage(5021, modcode, filepath, k + 1);
				return false;
			}
		}
	}

	return true;
}
