#include "behaviorcheck.h"

#pragma warning(disable:4503)

using namespace std;

void behaviorCheck()
{
	// test duplicate anim file
	for (auto it = usedAnim.begin(); it != usedAnim.end(); ++it)
	{
		vecstr forwardPort = behaviorJoints[it->first];

		for (unsigned int j = 0; j < forwardPort.size(); ++j)
		{
			for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
			{
				string filename = GetFileName(*iter);

				if (!registeredAnim[forwardPort[j]][filename])
				{
					cout << "WARNING: Animation has not been registered in " << forwardPort[j] << "file. The animation will not work. Please contact the mod author" << endl << "Animation: " << *iter << endl << endl;
				}
			}
		}
	}

	// test if anim file used has been registered
	for (auto it = animModMatch.begin(); it != animModMatch.end(); ++it)		// first string
	{
		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)		// second string
		{
			if (iter->second.size() == 2)
			{
				if (iter->second[0].size() > 1)
				{
					cout << "WARNING: Duplicated animation file detected" << endl << "File: " << it->first << endl << "Animation: " << iter->first << endl << "Mod: ";
					string warning;

					for (auto iteration = iter->second[1].begin(); iteration != iter->second[1].end(); ++iteration)
					{
						warning = warning + *iteration + ",";
					}

					warning.pop_back();
					cout << warning << endl << endl;
				}
			}
			else
			{
				cout << ">> ERROR(1066): BUG FOUND!! Report to Nemesis' author immediately << " << endl << endl;
				error = true;
				return;
			}
		}
	}
}
