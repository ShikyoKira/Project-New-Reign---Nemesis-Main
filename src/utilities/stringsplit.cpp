#include "header/utilities/stringsplit.h"

using namespace std;

void StringSplit(string line, vecstr& container)
{
	container = vecstr((istream_iterator<string>(istringstream(line))), istream_iterator<string>());
}

void StringSplit(string line, vecstr& container, string anyChar)
{
	container.clear();
	container.push_back(string());

	for (auto& each : line)
	{
		if (anyChar.find(each) != NOT_FOUND)
		{
			container.push_back(string());
		}
		else
		{
			container.back().push_back(each);
		}
	}
}
