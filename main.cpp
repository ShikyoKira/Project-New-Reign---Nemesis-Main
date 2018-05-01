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

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, bool> behaviorlist;

void UpdateFiles(string directory, string newAnimDirectory)
{
	unordered_map<string, map<string, vecstr>> newFile;						// behavior file, function/node ID, vector<string>; memory to access each node
	unordered_map<string, unordered_map<std::string, vecstr>> newAnimData;	// character, unique code, vector<string>; memory to access each node
	vecstr animDataChar;													// order of the character
	unordered_map<string, vecstr> animDataHeader;							// order of the character's header

	// Check the existence of required files
	if (FileCheck())
	{
		// clear the temp_behaviors folder to prevent it from bloating
		ClearTempBehaviors();

		// copy latest vanilla into memory
		if (VanillaUpdate(newFile, newAnimData, animDataChar, animDataHeader))
		{
			if (!error)
			{
				// check template for association with vanilla nodes
				if (newAnimUpdate(newAnimDirectory, newFile, newAnimData, animDataChar, animDataHeader))
				{
					// comparing if different
					JoiningEdits(directory, newFile, newAnimData, animDataChar, animDataHeader);

					if (!error)
					{
						// compiling all behaviors in "new" to "temp_behaviors" folder
						CombiningFiles(newFile, newAnimData, animDataChar, animDataHeader);
					}
				}
			}
		}
	}
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
	string filename = "Nemesis_magic_readied_direction_behavior.txt";
	vecstr storeline = GetFunctionLines(filename);

	ofstream out(filename);

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		if (storeline[i].find("SERIALIZE_IGNORED") == string::npos)
		{
			out << storeline[i] << "\n";
		}
	}
}

int main()
{
	if (!FileCheck())
	{
		return 0;
	}

	srand(unsigned int(time(NULL)));
	time1 = boost::posix_time::microsec_clock::local_time();

	// funcSpeed();
	// test();

	// REMEMBER ADD FILE CHECKER
	

	// Patcher Update
	UpdateFiles("mod\\", "behavior templates\\");


	// For Debug purpose
	// =======================

	// Behavior Priority Order
	vecstr behaviorPriority;
	behaviorPriority.push_back("tudm");

	// Selected Behavior
	unordered_map<string, bool> chosenBehavior;
	chosenBehavior["tudm"] = true;

	// =======================


	GenerateBehavior("temp_behaviors\\", behaviorPriority, chosenBehavior);
	
	if (!error)
	{
		behaviorCheck();
	}

	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Total processing time: " << duration / 1000 << " seconds" << endl;


	unordered_map<string, string> emptyBehaviorPath;
	behaviorPath = emptyBehaviorPath;

	unordered_map<string, unordered_map<string, bool>> emptyRegisteredAnim;
	registeredAnim = emptyRegisteredAnim;

	unordered_map<string, set<string>> emptyUsedAnim;
	usedAnim = emptyUsedAnim;

	unordered_map<string, unordered_map<string, vector<set<string>>>> emptyAnimModMatch;
	animModMatch = emptyAnimModMatch;

	unordered_map<string, vector<string>> emptyBehaviorJoints;
	behaviorJoints = emptyBehaviorJoints;


	cin.sync();
	cin.get();

	return 0;
}