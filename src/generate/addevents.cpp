#include "header/debugmsg.h"

#include "header/generate/addevents.h"

using namespace std;

bool AddEvents(string curNum, vector<string>& storeline, string input, unordered_map<string, bool> orievent, unordered_map<string, bool>& isExist, int& counter,
	string& ZeroEvent, unordered_map<string, int>& eventid, unordered_map<int, string>& eventName)
{
	if (curNum == "eventNames")
	{
		if (isExist[input])
		{
			WarningMessage(1010, input);
		}
		else if (!orievent[input])
		{
			if (counter == 0)
			{
				ZeroEvent = input;
			}

			eventid[input] = counter;
			eventName[counter] = input;
			storeline.push_back("				<hkcstring>" + input + "</hkcstring>");
			isExist[input] = true;
			++counter;
		}
	}
	else if (curNum == "eventInfos")
	{
		if (!isExist[input] && !orievent[input])
		{
			storeline.push_back("				<hkobject>");
			storeline.push_back("					<hkparam name=\"flags\">0</hkparam>");
			storeline.push_back("				</hkobject>");
			isExist[input] = true;
			++counter;
		}
	}
	else
	{
		ErrorMessage(1031);
	}

	return true;
}
