#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <direct.h>
#include <cstring>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include "functions/renew.h"
#include "dataunification.h"
#include "behaviorgenerator.h"
#include "behaviorcheck.h"
#include "filechecker.h"
#include "skyrimdirectory.h"
#include "add animation\furniture.h"
#include <alphanum.hpp>

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, bool> behaviorlist;

void UpdateFiles(string directory, string newAnimDirectory)
{
	unordered_map<string, map<string, vecstr>> newFile;		// behavior file, function/node ID, vector<string>; memory to access each node

	unordered_map<string, unordered_map<string, vecstr>> newAnimData;		// character, unique code, vector<string>; memory to access each node
	vecstr animDataChar;													// order of the character
	unordered_map<string, vecstr> animDataHeader;							// order of the character's header

	unordered_map<string, map<string, vecstr, alphanum_less<string>>> newAnimDataSet;	// project, header, vector<string>; memory to access each node
	vecstr projectList;																	// order of the project

	// unordered_map<string, AnimationDataProject> newAnimDataSet;			// animdataset info

	ClearGlobal();

	try
	{
		// Check the existence of required files
		if (FileCheck())
		{
			// clear the temp_behaviors folder to prevent it from bloating
			ClearTempBehaviors();

			// copy latest vanilla into memory
			if (VanillaUpdate(newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList))
			{
				if (!error)
				{
					// check template for association with vanilla nodes from behavior template file
					if (newAnimUpdate(newAnimDirectory, newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList))
					{
						// comparing if different from mod file
						JoiningEdits(directory, newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList);

						if (!error)
						{
							// compiling all behaviors in "new" to "temp_behaviors" folder
							CombiningFiles(newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList);
						}
					}
				}
			}
		}
	}
	catch (exception& ex)
	{
		ErrorMessage(6001, ex.what());
	}

	ClearGlobal();
}

template<typename T>
void printVector(vector<T> list)
{
	for (unsigned int i = 0; i < list.size(); ++i)
	{
		cout << list[i] << endl;
	}
}

void speedTest()
{
	boost::posix_time::ptime ntime1 = boost::posix_time::microsec_clock::local_time();

	vecstr store;
	ifstream open("file3.txt");
	string line;

	store.reserve(fileLineCount("file3.txt"));

	while (getline(open, line))
	{
		store.push_back(line);
	}

	open.close();
	
	ofstream out("1.txt");
	
	for (unsigned int i = 0; i < store.size(); ++i)
	{
		out << store[i] << "\n";
	}

	out.close();

	boost::posix_time::ptime ntime2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = ntime2 - ntime1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time: " << duration / 1000 << " seconds" << endl;
}

void speedTest2()
{
	boost::posix_time::ptime ntime1 = boost::posix_time::microsec_clock::local_time();

	vecstr store;
	store.reserve(fileLineCount("file3.txt"));
	char line[1000];
	FILE* open;
	fopen_s(&open, "file3.txt", "r");

	while (fgets(line, 5000, open))
	{
		line[strlen(line) - 1] = '\0';
		store.push_back(line);
	}

	fclose(open);

	ofstream out("2.txt");
	
	for (unsigned int i = 0; i < store.size(); ++i)
	{
		out << store[i] << "\n";
	}

	out.close();

	boost::posix_time::ptime ntime2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = ntime2 - ntime1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time: " << duration / 1000 << " seconds" << endl;
}

#include <thread>

void funcSpeed()
{
	// for testing the speed of the function when multithreading

	vector<thread> cur;

	for (int i = 0; i < 1; ++i)
	{
		cur.emplace_back(speedTest);
	}

	for (auto& t : cur)
	{
		t.join();
	}

	speedTest();

	cout << "-----------------" << endl;
	cur.clear();

	for (int i = 0; i < 1; ++i)
	{
		cur.emplace_back(speedTest2);
	}

	for (auto& t : cur)
	{
		t.join();
	}

	speedTest();

	cout << "-----------------" << endl;
	cur.clear();
}

void test()
{
	// for testing function


	string line = "data\\meshes\\nemesis_animationdatasinglefile.txt";

	vecstr read;
	GetFunctionLines(line, read, false);

	cout << line << endl;
}

int main()
{
	if (!FileCheck())
	{
		return 0;
	}

	srand(unsigned int(time(NULL)));

	// english language pack
	NewDebugMessage("english");

	// For Debug purpose
	// =======================

	// Behavior Priority Order
	vecstr behaviorPriority;
	behaviorPriority.push_back("tudm");

	// Selected Behavior
	unordered_map<string, bool> chosenBehavior;
	chosenBehavior["tudm"] = true;

	// =======================

	string input;

	while (true)
	{
		cin >> input;
		time1 = boost::posix_time::microsec_clock::local_time();

		if ((input == "1" || input == "2" || input == "3" || input == "4"))
		{
			if (input == "1")
			{
				// Patcher Update
				UpdateFiles("mod\\", "behavior templates\\");
			}
			else if (input == "2")
			{
				try
				{
					GenerateBehavior("temp_behaviors\\", behaviorPriority, chosenBehavior);
				}
				catch (exception& ex)
				{
					ErrorMessage(6002, ex.what());
				}

				if (!error)
				{
					behaviorCheck();
				}
				else
				{
					error = false;
				}
			}
			else if (input == "3")
			{
				test();
			}
			else if (input == "4")
			{
				funcSpeed();
			}

			boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration diff = time2 - time1;

			double duration = double(diff.total_milliseconds());

			cout << "Total processing time: " << duration / 1000 << " seconds" << endl;

			system("pause");
			system("cls");
		}
		else
		{
			break;
		}
	}

	cin.sync();
	cin.get();

	return 0;
}