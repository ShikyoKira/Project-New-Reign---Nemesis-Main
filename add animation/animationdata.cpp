#include "animationdata.h"

#pragma warning(disable:4503)

using namespace std;

AnimDataProject::AnimDataProject(vecstr animdatafile, string project, string filepath, string modcode)
{
	int startline = 0;
	BehaviorListProcess(*this, startline, animdatafile, project, modcode);

	if (startline >= int(animdatafile.size()))
	{
		return;
	}

	AnimDataProcess(animdatalist, startline, animdatafile, project, modcode);

	if (startline >= int(animdatafile.size()))
	{
		return;
	}

	InfoDataProcess(infodatalist, startline, animdatafile, project, modcode);
}

void BehaviorListProcess(AnimDataProject& storeline, int& startline, vecstr& animdatafile, string project, string modcode)
{
	if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(3005, project, "Header");
		throw 1;
	}

	int i = startline + 1;

	if (i + 4 >= int(animdatafile.size()))
	{
		ErrorMessage(3021, project);
		throw 1;
	}

	storeline.unknown1 = animdatafile[i++];
	++i;

	while (!isOnlyNumber(animdatafile[i]))
	{
		if (hasAlpha(animdatafile[i]))
		{
			storeline.behaviorlist.push_back(animdatafile[i++]);
		}
		else
		{
			ErrorMessage(3005, project, "Header");
			throw 1;
		}
	}

	storeline.unknown2 = animdatafile[i++];
	startline = i;

	if (i < int(animdatafile.size()) && !hasAlpha(animdatafile[i]))
	{
		ErrorMessage(3005, project, "Header");
		throw 1;
	}
}

void AnimDataProcess(vector<AnimDataPack>& storeline, int& startline, vecstr& animdatafile, string project, string modcode)
{
	for (int i = startline; i < int(animdatafile.size()); ++i)
	{
		if (!hasAlpha(animdatafile[i]))
		{
			ErrorMessage(3014, project, animdatafile[i]);
			throw 1;
		}

		string name = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		string uniquecode = animdatafile[i++];
		AnimDataPack curAP;
		curAP.name = name;
		curAP.uniquecode = uniquecode;

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		curAP.unknown1 = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		curAP.unknown2 = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		curAP.unknown3 = animdatafile[i++];

		if (!isOnlyNumber(animdatafile[i++]))
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		if (animdatafile[i].length() != 0 && hasAlpha(animdatafile[i]))
		{
			while (animdatafile[i].length() != 0)
			{
				curAP.eventname.push_back(animdatafile[i++]);
			}
		}

		if (animdatafile[i++].length() != 0)
		{
			ErrorMessage(3005, project, name);
			throw 1;
		}

		storeline.push_back(curAP);

		if (isOnlyNumber(animdatafile[i]))
		{
			startline = i;
			break;
		}
		else
		{
			--i;
		}		
	}
}

void InfoDataProcess(vector<InfoDataPack>& storeline, int& startline, vecstr& animdatafile, string project, string modcode)
{
	unordered_map<string, bool> isExist;

	for (int i = startline; i < int(animdatafile.size()); ++i)
	{
		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3020, project, animdatafile[i]);
			throw 1;
		}

		InfoDataPack curIP;
		string uniquecode = animdatafile[i++];
		curIP.uniquecode = uniquecode;

		if (isExist[uniquecode])
		{
			ErrorMessage(3022, project, uniquecode);
			throw 1;
		}

		isExist[uniquecode] = true;

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		curIP.duration = animdatafile[i];

		if (!isOnlyNumber(animdatafile[++i]))
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		++i;

		while (count(animdatafile[i].begin(), animdatafile[i].end(), ' ') != 0)
		{
			curIP.motiondata.push_back(animdatafile[i++]);
		}

		if (!isOnlyNumber(animdatafile[i++]))
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		while (count(animdatafile[i].begin(), animdatafile[i].end(), ' ') != 0)
		{
			curIP.rotationdata.push_back(animdatafile[i++]);
		}

		if (animdatafile[i].length() != 0)
		{
			ErrorMessage(3020, project, uniquecode);
			throw 1;
		}

		storeline.push_back(curIP);
	}
}

int AnimDataProject::GetAnimTotalLine()
{
	int counter = 3 + int(behaviorlist.size());

	for (auto& it : animdatalist)
	{
		counter += 7 + it.eventname.size();
	}

	return counter;
}

int AnimDataProject::GetInfoTotalLine()
{
	int counter = 0;

	for (auto& it : infodatalist)
	{
		counter += 5 + int(it.motiondata.size()) + int(it.rotationdata.size());
	}

	return counter;
}

AnimDataFormat::position AnimDataPosition(vecstr animData, string character, string header, string modcode, int linecount, int type, bool muteError)
{
	// has function
	bool isOpen = false;
	int functionstart = -1;
	int conditionOpen = 0;
	unordered_map<int, bool> isCondition;
	unordered_map<int, bool> isConditionOri;
	unordered_map<int, AnimDataFunct> marker;

	for (unsigned int i = 0; i < animData.size(); ++i)
	{
		if (animData[i].find("<!-- ") != NOT_FOUND)
		{
			if (functionstart == -1)
			{
				functionstart = i;
			}

			marker[i].skip = true;

			if (animData[i].find("<!-- CLOSE -->") != NOT_FOUND)
			{
				isOpen = false;
			}
			else if (animData[i].find("<!-- CONDITION END -->") != NOT_FOUND)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = false;
				--conditionOpen;
			}
		}

		if (isOpen)
		{
			marker[i].isNew = true;
		}

		if (isCondition[conditionOpen])
		{
			marker[i].isCondition = true;
			marker[i].conditionOpen = conditionOpen;

		}
		else if (isConditionOri[conditionOpen])
		{
			marker[i].isConditionOri = true;
			marker[i].conditionOpen = conditionOpen;
		}

		if (animData[i].find("<!-- ") != NOT_FOUND)
		{
			if (animData[i].find("<!-- NEW") != NOT_FOUND)
			{
				isOpen = true;
			}
			else if (animData[i].find("<!-- CONDITION START ^") != NOT_FOUND)
			{
				++conditionOpen;
				isCondition[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION -->") != NOT_FOUND)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION ^") != NOT_FOUND)
			{
				marker[i].nextCondition = true;
			}
		}
	}

	using namespace AnimDataFormat;

	if (linecount < int(animData.size()))
	{
		if (type == 0)
		{
			if (linecount < 3 && functionstart == 0)
			{
				return AnimDataConvert(type, linecount + 1, muteError);
				// 1. total line count
				// 2. unknown number
				// 3. behavior file count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != NOT_FOUND)
				{
					if (!muteError)
					{
						ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
						return xerror;
					}
				}

				int id = 0;
				unordered_map<int, int> subid;

				for (int i = 0; i < linecount + 1; ++i)
				{
					if (!marker[i].skip)
					{
						if (marker[i].isCondition)
						{
							++subid[marker[i].conditionOpen];
						}
						else
						{
							if (marker[i].conditionOpen > 1)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
							}
							else
							{
								subid[1] = 0;
								++id;

								if (id > 3)
								{
									break;
								}
							}
						}

						if (marker[i].isNew)
						{
							int tempid = id;

							for (auto it = subid.begin(); it != subid.end(); ++it)
							{
								tempid = tempid + it->second;
							}

							if (tempid != 4)
							{
								if (!muteError)
								{
									ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
									return xerror;
								}
							}
						}
					}
					else if (marker[i].nextCondition)
					{
						subid[marker[i].conditionOpen] = 0;
					}
				}

				for (auto it = subid.begin(); it != subid.end(); ++it)
				{
					id = id + it->second;
				}

				if (id < 4)
				{
					return AnimDataConvert(type, id, muteError);
				}
			}

			if (isOnlyNumber(animData[linecount]))
			{
				return unknown5;
				// 5. unknown number
			}
			else
			{
				return behaviorfilelist;
				// 4. behavior file list
			}
		}
		else if (type == 1)
		{
			if (linecount < 6 && functionstart > 5)
			{
				return AnimDataConvert(type, linecount + 1, muteError);
				// 1. animation name (clip)
				// 2. unique code
				// 3. unknown number (1)
				// 4. unknown number (0)
				// 5. unknown number (0)
				// 6. event name count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != NOT_FOUND)
				{
					if (!muteError)
					{
						ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
						return xerror;
					}
				}

				int id = 0;
				unordered_map<int, int> subid;

				for (int i = 0; i < linecount + 1; ++i)
				{
					if (!marker[i].skip)
					{
						if (marker[i].isCondition)
						{
							++subid[marker[i].conditionOpen];
						}
						else
						{
							if (marker[i].conditionOpen > 1)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
							}
							else
							{
								subid[1] = 0;
								++id;

								if (id > 6)
								{
									break;
								}
							}
						}

						if (marker[i].isNew)
						{
							int tempid = id;

							for (auto it = subid.begin(); it != subid.end(); ++it)
							{
								tempid = tempid + it->second;
							}

							if (tempid != 7)
							{
								ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
								return xerror;
							}
						}
					}
					else if (marker[i].nextCondition)
					{
						subid[marker[i].conditionOpen] = 0;
					}
				}

				for (auto it = subid.begin(); it != subid.end(); ++it)
				{
					id = id + it->second;
				}

				if (id < 7)
				{
					return AnimDataConvert(type, id, muteError);
				}
			}

			if (linecount == int(animData.size()) - 1)
			{
				return space;
				// 8. <space>
			}
			else if (hasAlpha(animData[linecount]))
			{
				return eventnamelist;
				// 7. event name list
			}
		}
		else if (type == 2)
		{
			int nextline = 3;

			if (linecount < 3 && functionstart > 2)
			{
				return AnimDataConvert(type, linecount + 1, muteError);
				// 1. unique code
				// 2. total animation (clip) length
				// 3. motion data count
			}
			else
			{
				if (animData[linecount].find("<!-- ") != NOT_FOUND)
				{
					if (!muteError)
					{
						ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
						return xerror;
					}
				}

				int id = 0;
				unordered_map<int, int> subid;

				for (int i = 0; i < linecount + 1; ++i)
				{
					if (!marker[i].skip)
					{
						if (marker[i].isCondition)
						{
							++subid[marker[i].conditionOpen];
						}
						else
						{
							if (marker[i].conditionOpen > 1)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
							}
							else
							{
								subid[1] = 0;
								++id;

								if (id > 3)
								{
									nextline = i;
									break;
								}
							}
						}

						if (marker[i].isNew)
						{
							int tempid = id;

							for (auto it = subid.begin(); it != subid.end(); ++it)
							{
								tempid = tempid + it->second;
							}

							if (tempid != 4 && tempid != 6)
							{
								if (!muteError)
								{
									ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
									return xerror;
								}
							}
						}
					}
					else if (marker[i].nextCondition)
					{
						subid[marker[i].conditionOpen] = 0;
					}
				}

				for (auto it = subid.begin(); it != subid.end(); ++it)
				{
					id = id + it->second;
				}

				if (id < 4)
				{
					return AnimDataConvert(type, id, muteError);
				}
			}

			if (linecount == int(animData.size()) - 1)
			{
				return space;
				// 7. <space>
			}

			int id = 4;
			bool nextplus = false;
			unordered_map<int, int> subid;
			
			for (int i = nextline; i < linecount + 1; ++i)
			{
				if (!marker[i].skip)
				{
					if (marker[i].isCondition)
					{
						if (isOnlyNumber(animData[i]) && count(animData[i].begin(), animData[i].end(), ' ') == 0)
						{
							++subid[marker[i].conditionOpen];

							if (marker.find(i + 1) != marker.end())
							{
								if (marker[i + 1].conditionOpen == marker[i].conditionOpen)
								{
									nextplus = true;
								}
							}
						}
						else if (nextplus)
						{
							++subid[marker[i].conditionOpen];
							nextplus = false;
						}
					}
					else
					{
						if (marker[i].conditionOpen > 1)
						{
							if (isOnlyNumber(animData[i]) && count(animData[i].begin(), animData[i].end(), ' ') == 0)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];

								if (marker.find(i + 1) != marker.end())
								{
									if (marker[i + 1].conditionOpen == marker[i].conditionOpen)
									{
										nextplus = true;
									}
								}
							}
							else if (nextplus)
							{
								subid[marker[i].conditionOpen] = 0;
								++subid[marker[i].conditionOpen - 1];
								nextplus = false;
							}
						}
						else
						{
							if (isOnlyNumber(animData[i]) && count(animData[i].begin(), animData[i].end(), ' ') == 0)
							{
								subid[1] = 0;
								++id;

								if (marker.find(i + 1) == marker.end())
								{
									nextplus = true;
								}
							}
							else if (nextplus)
							{
								subid[1] = 0;
								++id;
								nextplus = false;
							}
						}
					}

					if (marker[i].isNew)
					{
						int tempid = id;

						for (auto it = subid.begin(); it != subid.end(); ++it)
						{
							tempid = tempid + it->second;
						}

						if (tempid != 4 && tempid != 6)
						{
							ErrorMessage(3007, modcode, "animationdatasinglefile.txt", linecount, header);
							return xerror;
						}
					}
				}
				else if (marker[i].nextCondition)
				{
					subid[marker[i].conditionOpen] = 0;
				}
			}

			for (auto it = subid.begin(); it != subid.end(); ++it)
			{
				id = id + it->second;
			}

			if (id < 7 && id > 3)
			{
				return AnimDataConvert(type, id, muteError);
				// 4. motion data list
				// 5. rotation data count
				// 6. rotation data list
			}
		}
	}

	if (!muteError)
	{
		ErrorMessage(3005, character, header);
	}

	return xerror;
}

AnimDataFormat::position AnimDataConvert(int type, int position, bool muteError)
{
	using namespace AnimDataFormat;

	if (type == 1)
	{
		if (position == 1)
		{
			return totallinecount;
		}
		else if (position == 2)
		{
			return unknown2;
		}
		else if (position == 3)
		{
			return behaviorfilecount;
		}
		else if (position == 4)
		{
			return behaviorfilelist;
		}
		else if (position == 5)
		{
			return unknown5;
		}
	}
	else if (type == 2)
	{
		if (position == 1)
		{
			return animationname;
		}
		else if (position == 2)
		{
			return uniquecode;
		}
		else if (position == 3)
		{
			return unknown3;
		}
		else if (position == 4)
		{
			return unknown4;
		}
		else if (position == 5)
		{
			return unknown5;
		}
		else if (position == 6)
		{
			return eventnamecount;
		}
		else if (position == 7)
		{
			return eventnamelist;
		}
		else if (position == 8)
		{
			return space;
		}
	}
	else
	{
		if (position == 1)
		{
			return uniquecode;
		}
		else if (position == 2)
		{
			return totalcliplength;
		}
		else if (position == 3)
		{
			return motiondatacount;
		}
		else if (position == 4)
		{
			return motiondatalist;
		}
		else if (position == 5)
		{
			return rotationdatacount;
		}
		else if (position == 6)
		{
			return rotationdatalist;
		}
		else if (position == 7)
		{
			return space;
		}
	}

	return xerror;
}
