#include "alternateanimation.h"
#include <boost\algorithm\string.hpp>

using namespace std;

void AAInitialize(string AAList)
{
	vecstr groupName;
	char charline[2000];
	unordered_map<string, string> existAAAnim;			// animation name, animation group name; has the animation been registered for AA?
	read_directory(AAList, groupName);

	for (unsigned int i = 0; i < groupName.size(); ++i)
	{
		if (!boost::iequals(groupName[i], "alternate animation.script"))
		{
			FILE* input;
			fopen_s(&input, (AAList + "\\" + groupName[i]).c_str(), "r");

			if (input)
			{
				string AAGroupName = groupName[i].substr(0, groupName[i].find_last_of("."));

				while (fgets(charline, 2000, input))
				{
					string animFile = charline;

					if (animFile.length() != 0)
					{
						if (animFile[animFile.size() - 1] == '\n')
						{
							animFile.pop_back();
						}

						string lowerAnimFile = boost::algorithm::to_lower_copy(animFile);

						if (existAAAnim[lowerAnimFile].length() == 0)
						{
							string lowerGroupName = boost::algorithm::to_lower_copy(AAGroupName);
							groupAA[lowerGroupName].push_back(lowerAnimFile);
							AAGroup[lowerAnimFile] = lowerGroupName;
							existAAAnim[lowerAnimFile] = lowerGroupName;
							groupNameList.push_back(lowerGroupName);
						}
						else
						{
							cout << "ERROR(4001): Duplication of animation detected in 2 or more groups. Please contact the template creator" << endl << "Group: " << AAGroupName << ", " << existAAAnim[lowerAnimFile] << endl << endl;
							error = true;
							fclose(input);
							return;
						}
					}
				}

				fclose(input);
			}
			else
			{
				cout << "ERROR(4000): Unable to open file. Please make sure " << AAList << " file is in the same directory as this program" << endl << endl;
				error = true;
				return;
			}
		}
	}
}

void AAInstallation()
{
	string filename = "alternate animation\\alternate animation.script";
	boost::filesystem::path source(filename);

#ifndef DEBUG
	filename = "data";
#else
	filename = skyrimdataPath.GetDataPath();
#endif

	if (CreateDirectory((filename).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		filename = filename + "\\scripts";

		if (CreateDirectory((filename).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			filename = filename + "\\Nemesis_AA_Core.pex";
		}
	}

	vecstr prefixList;
	unordered_map<string, bool> prefixDone;
	unordered_map<string, int> groupCounter;
	int prefixCounter = 0;

	fixedKeyInitialize();

	boost::filesystem::path target(filename);
	boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);
	FILE* f;
	fopen_s(&f, filename.c_str(), "r+b");

	if (f)
	{
		for (unsigned int i = 0; i < groupNameList.size(); ++i)							// animation group name; ie. _mtidle
		{
			for (unsigned int j = 0; j < groupAAPrefix[groupNameList[i]].size(); ++j)	// list of group aa prefix categorized by animation group name
			{
				unsigned char charcode;

				if (!prefixDone[groupAAPrefix[groupNameList[i]][j]])
				{
					prefixList.push_back(groupAAPrefix[groupNameList[i]][j]);
					prefixDone[groupAAPrefix[groupNameList[i]][j]] = true;
				}

				fseek(f, 1059 + (9 * (i + j)), SEEK_SET);
				charcode = 48 + (j / 10);
				fwrite(&charcode, sizeof(charcode), 1, f);

				fseek(f, 1060 + (9 * (i + j)), SEEK_SET);
				charcode = 48 + (j % 10);
				fwrite(&charcode, sizeof(charcode), 1, f);

				fseek(f, 1061 + (9 * (i + j)), SEEK_SET);
				charcode = 48 + (i / 10);
				fwrite(&charcode, sizeof(charcode), 1, f);

				fseek(f, 1062 + (9 * (i + j)), SEEK_SET);
				charcode = 48 + (i % 10);
				fwrite(&charcode, sizeof(charcode), 1, f);

				string counter = to_string(AAGroupCount[groupAAPrefix[groupNameList[i]][j]][groupNameList[i]]);

				if (counter == "0")
				{
					cout << "ERROR(3013): Un-registered animation group name detected. Please contact the mod author" << endl << "AAPrefix: " << groupAAPrefix[groupNameList[i]][j] << "Animation Group Name: " << groupNameList[i] << endl << endl;
					error = true;
					return;
				}

				groupCounter[groupNameList[i]] = groupCounter[groupNameList[i]] + stoi(counter);
				counter = to_string(groupCounter[groupNameList[i]]);

				while (counter.length() < 3)
				{
					counter = "0" + counter;
				}

				for (unsigned int k = 0; k < 3; ++k)
				{
					fseek(f, 1063 + k + (9 * (i + j)), SEEK_SET);
					charcode = unsigned char(counter[k]);
					fwrite(&charcode, sizeof(charcode), 1, f);
				}

				prefixCounter++;
			}
		}

		for (unsigned int i = 0; i < prefixList.size(); ++i)
		{
			int num = 0;

			while (num < 3)
			{
				fseek(f, 2431 + num + (5 * i), SEEK_SET);
				unsigned char charcode = unsigned char(prefixList[i][num]);
				fwrite(&charcode, sizeof(charcode), 1, f);
				++num;
			}
		}

		unsigned char filebyte[15000];
		fseek(f, 0, SEEK_END);
		int fileLength = ftell(f);

		if (fileLength == 0)
		{
			cout << "ERROR(4012): Empty \"alternate animation.script\" detected. Please re-install Nemesis" << endl;
			error = true;
			return;
		}

		rewind(f);
		fread(filebyte, fileLength, 1, f);

		int installationKey = ((getUniqueKey(filebyte, 1059, 1059 + prefixCounter * 9 - 3) % 500000) + (getUniqueKey(filebyte, 2431, 2431 + prefixList.size() * 5 - 3) % 500000));

		for (unsigned int i = 0; i < 6; ++i)
		{
			fseek(f, 2220 + i, SEEK_SET);
			unsigned char charcode = unsigned char(48 + (installationKey % 10));
			fwrite(&charcode, sizeof(charcode), 1, f);
			installationKey /= 10;
		}

		fclose(f);
	}
	else
	{
		cout << "ERROR(3002): Failed to open behavior template" << endl << "File: " << filename << endl << endl;
		error = true;
		return;
	}
}

void fixedKeyInitialize()
{
	int counter = 0;

	while (counter <= 256)
	{
		unsigned int key1 = unsigned int(counter);
		int key2 = 0;

		while (key2 <= 7)
		{
			if ((key1 & 1) > 0)		// is odd number
			{
				key1 = key1 >> 1 ^ 3988292384;
			}
			else
			{
				key1 >>= 1;
			}

			++key2;
		}

		fixedkey[counter] = key1;
		++counter;
	}
}

unsigned int getUniqueKey(unsigned char bytearray[], int byte1, int byte2)
{
	unsigned int uniqueKey = 0;
	int key1 = byte1;
	int key2 = byte2;
	int counter = byte1;

	while (counter <= key2)
	{
		unsigned char curByte = bytearray[counter];
		unsigned int key3 = uniqueKey ^ unsigned int(curByte);
		uniqueKey = uniqueKey >> 8 ^ fixedkey[key3 & 255];
		counter++;
	}

	return uniqueKey;
}
