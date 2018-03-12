#include "functionupdate.h"

using namespace std;

typedef vector<string> vecstr;

unordered_map<string, mutex> locker;

string GetFunctionEdits(vecstr storeline, int numline)
{
	if (int(storeline.size()) > numline)
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
		for (size_t i = startline; i < endline + startline; ++i)
		{
			storage.push_back(storeline[i]);
		}

		return storage;
	}
	else
	{
		cout << "ERROR(2003): Missing file" << endl << "File: " << filename << endl << "StartLine: " << startline << endl << "EndLine: " << startline + endline << endl << endl;
		error = true;
		return storage;
	}
}

bool FunctionUpdate(string modcode, string f2, string f3, int memoryStore, unordered_map<string, map<string, vecstr>>& newFile)
{
	lock_guard<mutex> filelocker(locker[f3]);
	string filecheck = boost::regex_replace(string(f3), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")) + ".txt";

	if ("#" + filecheck == f3)
	{
		unordered_map<string, string> modPath;
		unordered_map<string, vecstr> modEdits;
		unordered_map<string, vector<int>> modEditCoordinate;
		unordered_map<string, int> modEditLine;
		unordered_map<string, unordered_map<int, int>> NewCoordinate;
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
		string filename = "cache/" + modcode + "/" + f2 + "/" + f3;
		char charline[2000];
		FILE* BehaviorFormat;
		fopen_s(&BehaviorFormat, filename.c_str(), "r");

		if (BehaviorFormat)
		{
			bool IsEventVariable = false;

			while (fgets(charline, 2000, BehaviorFormat))
			{
				line = charline;
				line.pop_back();

				if (line.find("hkbBehaviorGraphStringData", 0) != string::npos || line.find("hkbVariableValueSet", 0) != string::npos || line.find("hkbBehaviorGraphData", 0) != string::npos)
				{
					IsEventVariable = true;
				}

				if ((line.find("<!-- MOD_CODE", 0) != string::npos) && (line.find("OPEN -->", 0) != string::npos) && (!edited))
				{
					edited = true;
					editline = linecount;
					starteditline = linecount;
				}
				else if (line.find("<!-- ORIGINAL -->", 0) != string::npos)
				{
					edited = false;
					originalopen = true;
					originalline = linecount;
					startoriline = linecount;
				}
				else if (line.find("<!-- CLOSE -->", 0) != string::npos)
				{
					edited = false;

					if (originalopen)
					{
						int tempint = editline - originalline;
						NewCoordinate[f3][linecount] = tempint;
						modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
						for (int i = startoriline; i < originalline; i++)
						{
							modEditCoordinate[f3].push_back(i);
							modEditLine[to_string(i)] = Pair[i];
						}
						originalopen = false;
					}
					else
					{
						int tempint = editline - starteditline;
						NewCoordinate[f3][linecount] = tempint;
						modEditLine[to_string(linecount) + "R"] = coordinate - tempint;
					}
				}
				else if (edited)
				{
					if (IsEventVariable && line.find("numelements=", 0) != string::npos)
					{
						if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != string::npos)
						{
							eventcount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
						else if (line.find("<hkparam name=\"attributeNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"attributeDefaults\" numelements=", 0) != string::npos)
						{
							attributecount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
						else if (line.find("<hkparam name=\"variableNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"wordVariableValues\" numelements=", 0) != string::npos || line.find("<hkparam name=\"variableInfos\" numelements=", 0) != string::npos)
						{
							variablecount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
						else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"characterPropertyInfos\" numelements=", 0) != string::npos)
						{
							characterpropertycount = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
						}
					}

					Pair[editline] = coordinate;
					editline++;
				}
				else if (!edited)
				{
					linecount++;
					originalline++;
				}

				storeline.push_back(line);
				coordinate++;
			}

			fclose(BehaviorFormat);
		}
		else
		{
			cout << "ERROR(2000): Failed to open file" << endl << "File: " << filename << endl << endl;
			error = true;
			return false;
		}

		vecstr newline = newFile[f2][f3.substr(0, f3.find_last_of("."))];
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

				if (line.find("<!-- NEW", 0) != string::npos)
				{
					skip = true;
				}

				if (line.find("<!-- *", 0) == string::npos && !skip)
				{
					if (modEditCoordinate[f3][editcount] == linecount)
					{
						if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != string::npos)
						{
							int tempint = eventcount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- EVENT numelement ", 0) != string::npos)
							{
								line.append(" <!-- EVENT numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("   				<!-- EVENT numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("<hkparam name=\"attributeNames\" numelements=", 0) != string::npos)
						{
							int tempint = attributecount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- ATTRIBUTE numelement ", 0) != string::npos)
							{
								line.append(" <!-- ATTRIBUTE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("   				<!-- ATTRIBUTE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("<hkparam name=\"variableNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"wordVariableValues\" numelements=", 0) != string::npos || line.find("<hkparam name=\"variableInfos\" numelements=", 0) != string::npos)
						{
							int tempint = variablecount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
							
							if (line.find("<!-- VARIABLE numelement ", 0) != string::npos)
							{
								line.append(" <!-- VARIABLE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("				<!-- VARIABLE numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"characterPropertyInfos\" numelements=", 0) != string::npos)
						{
							int tempint = characterpropertycount - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- CHARACTER numelement ", 0) != string::npos)
							{
								line.append(" <!-- CHARACTER numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
							else
							{
								line.append("				<!-- CHARACTER numelement " + modcode + " +" + to_string(tempint) + " $" + to_string(modEditLine[to_string(linecount)]) + " -->");
							}
						}
						else if (line.find("numelements=\"", 0) != string::npos)
						{
							string templine = GetFunctionEdits(storeline, modEditLine[to_string(linecount)]);

							if (error)
							{
								cout << "ERROR(2005): Missing edits" << endl << "File: " << "cache/" << modcode << "/" << f2 << "/" << f3 << endl << "Line: " << modEditLine[to_string(linecount)] << endl << endl;
								error = true;
								return false;
							}

							int difference = stoi(boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"))) - stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (line.find("<!-- numelement *", 0) != string::npos)
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
								cout << "ERROR(2005): Missing edits" << endl << "File: " << "cache/" << modcode << "/" << f2 << "/" << f3 << endl << "Line: " << modEditLine[to_string(linecount)] << endl << endl;
								error = true;
								return false;
							}

							templine.append("					<!-- *" + modcode + "* -->");
							functionline.push_back(templine);

							if (line.find("<!-- original -->", 0) == string::npos)
							{
								line.append("				<!-- original -->");
							}
						}

						if (editcount != modEditCoordinate[f3].size() - 1)
						{
							editcount++;
						}
					}
					else if (NewCoordinate[f3][linecount] > 0)
					{
						functionline.push_back("<!-- NEW *" + modcode + "* -->");

						vecstr storage = GetFunctionEdits("cache/" + modcode + "/" + f2 + "/" + f3, storeline, modEditLine[to_string(linecount) + "R"], NewCoordinate[f3][linecount]);

						if (!error)
						{
							for (unsigned int i = 0; i < storage.size(); i++)
							{
								functionline.push_back(storage[i]);
							}

							functionline.push_back("<!-- CLOSE -->");
						}
						else
						{
							return false;
						}
					}

					linecount++;
				}

				if (line.find("<!-- CLOSE -->", 0) != string::npos)
				{
					skip = false;
				}

				functionline.push_back(line);
			}
		}
		else
		{
			cout << "ERROR(2001): Failed to open file" << endl << "File: " << filename << endl << endl;
			error = true;
			return false;
		}

		functionline.shrink_to_fit();
		newFile[f2][f3.substr(0, f3.find_last_of("."))] = functionline;
	}
	else if (f3.find("#") != string::npos && f3.find("$") != string::npos)
	{
		if (f3 == "#" + modcode + "$" + filecheck)
		{
			newFile[f2][f3.substr(0, f3.find_last_of("."))] = GetFunctionLines("cache/" + modcode + "/" + f2 + "/" + f3);

			if (newFile[f2][f3.substr(0, f3.find_last_of("."))].back().length() != 0)
			{
				newFile[f2][f3.substr(0, f3.find_last_of("."))].push_back("");
			}
		}
		else
		{
			cout << "ERROR(2004): Invalid File" << endl << "File: cache/" << modcode << "/" + f2 << "/" << f3 << endl << endl;
			error = true;
			return false;
		}
	}
	else
	{
		cout << "ERROR(2004): Invalid File" << endl << "File: cache/" << modcode << "/" + f2 << "/" << f3 << endl << endl;
		error = true;
		return false;
	}

	return true;
}
