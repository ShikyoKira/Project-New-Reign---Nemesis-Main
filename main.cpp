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
#include "add animation\furniture.h"
#include "skyrimdirectory.h"

using namespace std;

unordered_map<string, bool> behaviorlist;

void UpdateFiles(string directory, string newAnimDirectory)
{
	if (!error)
	{
		// update "vanilla" folder
		VanillaUpdate();

		// remove outdated behavior files + copy "vanilla" to "new"
		Clearing(directory);
	}
	
	if (!error)
	{
		// comparing
		if (newAnimUpdate(newAnimDirectory, directory))
		{
			// modify "new"
			JoiningEdits(directory);
		}
	}

	if (!error)
	{
		// compiling all behaviors in "new" to "temp_behaviors" folder
		CombiningFiles(directory);
	}
}

template<typename T>
void printVector(vector<T> list)
{
	for (unsigned int i = 0; i < list.size(); i++)
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



}

int main()
{
	srand(unsigned int(time(NULL)));
	time1 = boost::posix_time::microsec_clock::local_time();

	// funcSpeed();
	// test();

	// REMEMBER ADD FILE CHECKER
	

	// Patcher Update
	UpdateFiles("cache/", "behavior templates/");


	// For Debug purpose
	// =====================

	// Behavior Priority Order
	vecstr behaviorPriority;
	behaviorPriority.push_back("tudm");

	// Selected Behavior
	unordered_map<string, bool> chosenBehavior;
	chosenBehavior["tudm"] = true;

	// =======================


	// GenerateBehavior("temp_behaviors/", behaviorPriority, chosenBehavior);
	
	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Total processing time: " << duration / 1000 << " seconds" << endl;


	cin.sync();
	cin.get();

	return 0;
}