#include "killmove.h"

using namespace std;

KillMove::KillMove(string line, vecstr killmoveformat, int killmovecount, AOConvert import, string filepath, int stateID)
{
	bool option = false;
	int position = line.find("-");
	string templine = line;
	string animobjects;
	string filename;

	if (position != -1)
	{
		templine = line.substr(position, line.find(" ", position) - position + 1);
		int positionAE = 3 + templine.length();
		mainAnimEvent = line.substr(positionAE, line.find(" ", positionAE) - positionAE);

		int fileposition;
		int tempint = 0;
		for (int i = 0; i < 4; i++)
		{
			position = line.find(" ", tempint);
			tempint = line.find(" ", position + 1);

			if (i == 2)
			{
				fileposition = position + 1;
			}
		}
		animobjects = line.substr(position + 1);

		int x = line.find(" ", fileposition);
		int y = line.length();
		int fileEndPoint;
		if (x == -1)
		{
			fileEndPoint = y;
		}
		else
		{
			fileEndPoint = min(x, y);
		}

		filename = line.substr(fileposition, fileEndPoint - fileposition);

		option = true;
	}
	else
	{
		mainAnimEvent = line.substr(3, line.find(" ", 3) - 3);

		int fileposition;
		int tempint = 0;
		for (int i = 0; i < 3; i++)
		{
			position = line.find(" ", tempint);
			tempint = line.find(" ", position + 1);

			if (i == 1)
			{
				fileposition = position;
			}
		}
		animobjects = line.substr(position + 1);
		filename = line.substr(fileposition + 1, line.find(" ", fileposition) - fileposition - 1);
	}

	// Part 1 - get animobjects
	if (animobjects.length() != 0 && animobjects != line)
	{
		size_t AOcount = count(animobjects.begin(), animobjects.end(), '/');

		if (AOcount > 2 && AOcount < 1)
		{
			error = true;
		}
		else
		{
			int tempint = 0;
			int nextobj = 0;
			for (unsigned int i = 0; i < AOcount; i++)
			{
				position = animobjects.find("/", tempint);
				if (AOcount == 2)
				{
					tempint = animobjects.find("/", position + 1);
				}
				string tempstr = animobjects.substr(nextobj, animobjects.find("/", position) - nextobj);
				int temp = stoi(animobjects.substr(position + 1, 2));
				if (temp != 1 && temp != 2)
				{
					error = true;
				}
				nextobj = animobjects.find(" ");
				AObject[temp] = tempstr;
			}
		}
	}
	
	killmovelines = killmoveformat;

	{
		int numline = 7;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 16;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 24;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 67;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 76;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 119;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 149;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 164;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 252;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 261;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 269;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 298;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 341;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 350;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 393;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 423;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));
		numline = 438;
		killmovelines[numline].replace(killmovelines[numline].find("$%$"), 3, to_string(killmovecount));


		numline = 8;
		killmovelines[numline].replace(killmovelines[numline].find("(S$0)"), 5, to_string(stateID));
		numline = 253;
		killmovelines[numline].replace(killmovelines[numline].find("(S$1)"), 5, to_string(stateID + 1));
	}

	if (option)
	{
		while (true)
		{
			if (templine.find("Tn,", 0) != string::npos || templine.find("Tn ",0) != string::npos)
			{
				nextTrans = true;
				templine.replace(templine.find("Tn"), 2, "Xx");
			}

			// part 2 - get triggers and timing
			if (templine.find("T2_", 0) != string::npos)
			{
				string templine2 = templine.substr(templine.find("T2_"));

				int triggercount = 0;
				int tempInt = 0;
				while (true)
				{
					if (templine2.find("T2_", tempInt) != string::npos)
					{
						triggercount++;
						tempInt = templine2.find("T2_", tempInt) + 1;
					}
					else
					{
						break;
					}
				}

				killmovelines[453].replace(killmovelines[453].find("!nT2$4!"), 7, to_string(triggercount + 4));

				tempInt = 0;
				for (int i = 0; i < triggercount; i++)
				{
					int position = templine2.find("T2_", tempInt);
					string tempstr;
					if (i != triggercount - 1)
					{
						tempInt = templine2.find("T2_", position + 1);
						tempstr = templine2.substr(position, tempInt - position - 1);
					}
					else
					{
						int x = templine2.find(" ");
						int y = templine2.find(",", position + 1);

						if (y != -1)
						{
							tempInt = min(x, y);
						}
						else
						{
							tempInt = x;
						}

						tempstr = templine2.substr(position, tempInt - position);
					}
					string eventname = tempstr.substr(3, tempstr.find("/") - 3);
					TriggerTime1[eventname] = tempstr.substr(tempstr.find("/") + 1);
					eventID.push_back(eventname);
					templine.replace(templine.find(tempstr), tempstr.length(), "xxx");
				}
			}
			else
			{
				killmovelines[453].replace(killmovelines[453].find("!nT2$4!"), 7, "4");
			}
			
			if (templine.find("T", 0) != string::npos)
			{
				string templine2 = templine.substr(templine.find("T"));
				size_t triggercount = count(templine2.begin(), templine2.end(), 'T');
				killmovelines[179].replace(killmovelines[179].find("!nT$4!"), 6, to_string(triggercount + 4));
				int tempInt = 0;
				for (unsigned int i = 0; i < triggercount; i++)
				{
					string tempstr;
					int position = templine2.find("T", tempInt);
					if (i != triggercount - 1)
					{
						tempInt = templine2.find(",", position + 1);
						tempstr = templine2.substr(position, tempInt - position - 1);
					}
					else
					{
						int x = templine2.find(" ");
						int y = templine2.find(",", position + 1);

						if (y != -1)
						{
							tempInt = min(x, y);
						}
						else
						{
							tempInt = x;
						}
						tempstr = templine2.substr(position, tempInt - position);
					}
					string eventname = tempstr.substr(1, tempstr.find("/") - 1);
					TriggerTime[eventname] = tempstr.substr(tempstr.find("/") + 1);
					eventID.push_back(eventname);
					templine.replace(templine.find(tempstr), tempstr.length(), "xxx");
				}
			}
			else
			{
				killmovelines[179].replace(killmovelines[179].find("!nT$4!"), 6, "4");
			}

			if (templine.find("ac0", 0) != string::npos)
			{
				AC0 = true;
				killmovelines[4].replace(killmovelines[4].find("!AC0!"), 5, import["0_master-endcamera"]);
				killmovelines[249].replace(killmovelines[249].find("!AC0!"), 5, import["0_master-endcamera"]);
			}

			if (templine.find("ac1", 0) != string::npos)
			{
				AC1 = true;
				killmovelines[3].replace(killmovelines[3].find("!AC1!"), 5, import["0_master-startcamera"]);
				killmovelines[248].replace(killmovelines[248].find("!AC1!"), 5, import["0_master-startkillcamera"]);
			}

			if (templine.find("ac ", 0) != string::npos || templine.find("ac,", 0) != string::npos)
			{
				AC = true;
				AC0 = true;
				AC1 = true;
				killmovelines[4].replace(killmovelines[4].find("!AC0!"), 5, import["0_master-endcamera"]);
				killmovelines[249].replace(killmovelines[249].find("!AC0!"), 5, import["0_master-endcamera"]);
				killmovelines[3].replace(killmovelines[3].find("!AC1!"), 5, import["0_master-startcamera"]);
				killmovelines[248].replace(killmovelines[248].find("!AC1!"), 5, import["0_master-startkillcamera"]);
			}
			else
			{
				if (!AC0)
				{
					killmovelines[4].replace(killmovelines[4].find("!AC0!"), 5, "null");
					killmovelines[249].replace(killmovelines[249].find("!AC0!"), 5, "null");
				}

				if (!AC1)
				{
					killmovelines[3].replace(killmovelines[3].find("!AC1!"), 5, "null");
					killmovelines[248].replace(killmovelines[248].find("!AC1!"), 5, import["0_master-pairkill"]);
				}
			}

			if (templine.find("a ", 0) != string::npos || templine.find("a,", 0) != string::npos)
			{
				Loop = false;
			}

			if (templine.find("o ", 0) != string::npos || templine.find("o,", 0) != string::npos)
			{
				if (animobjects == line)
				{
					error = true;
					break;
				}

				AO = true;
			}
			else
			{
				if (animobjects != line)
				{
					error = true;
					break;
				}
			}

			if (templine.find("B", 0) != string::npos)
			{
				string temp = templine.substr(templine.find("B"));
				int tempint = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
			}

			if (templine.find("bsa ", 0) != string::npos || templine.find("bsa,", 0) != string::npos)
			{

			}

			if (templine.find("h ", 0) != string::npos || templine.find("h,", 0) != string::npos)
			{
				HT = true;
				if (!AO)
				{
					killmovelines[115].replace(killmovelines[115].find("!HO!"), 4, "null");
					killmovelines[116].replace(killmovelines[116].find("!HO!"), 4, "null");

					killmovelines[389].replace(killmovelines[389].find("!HO!"), 4, "null");
					killmovelines[390].replace(killmovelines[390].find("!HO!"), 4, "null");
				}
				else
				{
					if (AObject[1].length() > 0)
					{
						killmovelines[115].replace(killmovelines[115].find("!HO!"), 4, "#MID$8");
						killmovelines[116].replace(killmovelines[116].find("!HO!"), 4, import["0_master-AOoff"]);
						killmovelines[126].replace(killmovelines[126].find("!AO0*2$H!"), 9, "2");
					}
					else
					{
						killmovelines[115].replace(killmovelines[115].find("!HO!"), 4, "null");
						killmovelines[116].replace(killmovelines[116].find("!HO!"), 4, "null");
					}

					if (AObject[2].length() > 0)
					{
						killmovelines[389].replace(killmovelines[389].find("!HO!"), 4, "#MID$22");
						killmovelines[390].replace(killmovelines[390].find("!HO!"), 4, import["0_master-AOoff"]);
						killmovelines[400].replace(killmovelines[400].find("!AO1*2$H!"), 9, "2");
					}
					else
					{
						killmovelines[389].replace(killmovelines[389].find("!HO!"), 4, "null");
						killmovelines[390].replace(killmovelines[390].find("!HO!"), 4, "null");
					}
				}
			}
			else
			{
				if (!AO)
				{
					killmovelines[115].replace(killmovelines[115].find("!HO!"), 4, import["0_master-HToff"]);
					killmovelines[116].replace(killmovelines[116].find("!HO!"), 4, import["0_master-HTon"]);

					killmovelines[389].replace(killmovelines[389].find("!HO!"), 4, import["0_master-HToff"]);
					killmovelines[390].replace(killmovelines[390].find("!HO!"), 4, import["0_master-HTon"]);
				}
				else
				{
					if (AObject[1].length() > 0)
					{
						killmovelines[115].replace(killmovelines[115].find("!HO!"), 4, "#MID$8");
						killmovelines[116].replace(killmovelines[116].find("!HO!"), 4, import["0_master-HTon,AOoff"]);
						killmovelines[126].replace(killmovelines[126].find("!AO0*2$H!"), 9, "3");
					}
					else
					{
						killmovelines[115].replace(killmovelines[115].find("!HO!"), 4, import["0_master-HToff"]);
						killmovelines[116].replace(killmovelines[116].find("!HO!"), 4, import["0_master-HTon"]);
					}

					if (AObject[2].length() > 0)
					{
						killmovelines[389].replace(killmovelines[389].find("!HO!"), 4, "#MID$22");
						killmovelines[390].replace(killmovelines[390].find("!HO!"), 4, import["0_master-HTon,AOoff"]);
						killmovelines[400].replace(killmovelines[400].find("!AO1*2$H!"), 9, "3");
					}
					else
					{
						killmovelines[389].replace(killmovelines[389].find("!HO!"), 4, import["0_master-HToff"]);
						killmovelines[390].replace(killmovelines[390].find("!HO!"), 4, import["0_master-HTon"]);
					}
				}
			}

			if (templine.find("k ", 0) != string::npos || templine.find("k,", 0) != string::npos)
			{
				Known = true;
			}

			if (templine.find("md ", 0) != string::npos || templine.find("md,", 0) != string::npos)
			{
				MD = true;
			}

			if (templine.find("st ", 0) != string::npos || templine.find("st,", 0) != string::npos)
			{
				PAO = true;
			}

			if (templine.find("D", 0) != string::npos)
			{
				int tempint;
				int x = templine.find(" ", templine.find("D"));
				int y = templine.find(",", templine.find("D"));

				if (y != -1 && x != -1)
				{
					tempint = min(x, y);
				}
				else
				{
					if (x == -1 && y != -1)
					{
						tempint = y;
					}
					else if (y == -1 && x != -1)
					{
						tempint = x;
					}
					else
					{
						tempint = templine.length();
					}
				}
				string time = templine.substr(templine.find("D") + 1, tempint - templine.find("D") - 1);
				duration = stod(time);
				killmovelines[207].replace(killmovelines[207].find("!END!"), 5, time);
				killmovelines[219].replace(killmovelines[219].find("!END!"), 5, time);
				killmovelines[231].replace(killmovelines[231].find("!END!"), 5, time);
				killmovelines[481].replace(killmovelines[481].find("!END!"), 5, time);
				killmovelines[493].replace(killmovelines[493].find("!END!"), 5, time);
				killmovelines[505].replace(killmovelines[505].find("!END!"), 5, time);
			}
			else
			{
				killmovelines[207].replace(killmovelines[207].find("!END!"), 5, "0.000000");
				killmovelines[214].replace(killmovelines[214].find("false"), 5, "true");
				killmovelines[219].replace(killmovelines[219].find("!END!"), 5, "0.000000");
				killmovelines[226].replace(killmovelines[226].find("false"), 5, "true");
				killmovelines[231].replace(killmovelines[231].find("!END!"), 5, "0.000000");
				killmovelines[238].replace(killmovelines[238].find("false"), 5, "true");
				killmovelines[481].replace(killmovelines[481].find("!END!"), 5, "0.000000");
				killmovelines[488].replace(killmovelines[488].find("false"), 5, "true");
				killmovelines[493].replace(killmovelines[493].find("!END!"), 5, "0.000000");
				killmovelines[500].replace(killmovelines[500].find("false"), 5, "true");
				killmovelines[505].replace(killmovelines[505].find("!END!"), 5, "0.000000");
				killmovelines[512].replace(killmovelines[512].find("false"), 5, "true");
			}

			break;
		}				
	}

	if (filename != line)
	{
		killmovelines[165].replace(killmovelines[165].find("@M@"), 3, filepath + filename);
		killmovelines[439].replace(killmovelines[439].find("@M@"), 3, filepath + filename);
	}
	else
	{
		error = true;
	}
}

vecstr KillMove::GetKillMoveLine(int nFunctionID, AOConvert AnimObjectNames, id eventid, id variableid)
{
	vecstr generatedlines;
	vecstr recordTrigger;
	AOConvert IDExist;

	NewAnimObject = AnimObjectNames;

	int nextFunctionID = nFunctionID;
	bool nextID = false;
	bool TriggerStateIgnore = false;
	bool TriggerState = false;
	bool TriggerState1 = false;

	string strID = to_string(nextFunctionID);

	for (unsigned int i = 0; i < killmovelines.size(); i++)
	{
		string line = killmovelines[i];
		if (line.find("<!--", 0) == string::npos && !TriggerStateIgnore)
		{
			while (true)
			{
				while (true)
				{
					if (strID.length() < 4)
					{
						strID = "0" + strID;
					}
					else
					{
						break;
					}
				}
				
				if (i > 124 && i < 146 && (!AO || (AObject[1].length() == 0 && HT)))
				{
					break;
				}

				if (i > 398 && i < 420 && (!AO || (AObject[2].length() == 0 && HT)))
				{
					break;
				}

				if ((i > 127 && i < 132) || (i > 401 && i < 406))
				{
					if (HT)
					{
						break;
					}
				}
				else if (i > 133 && i < 142)
				{
					if (AObject[1].length() > 0 && AO)
					{
						if (line.find("!AO0N!", 0) != string::npos)
						{
							string tempID;
							if (NewAnimObject[AObject[1]].length() != 0)
							{
								tempID = NewAnimObject[AObject[1]];
							}
							else
							{
								tempID = "#" + strID;
								NewAnimObject[AObject[1]] = tempID;
								nextID = true;
							}
							line.replace(line.find("!AO0N!"), 6, tempID);
						}
					}
					else
					{
						break;
					}
				}
				else if (i > 407 && i < 416)
				{
					if (AObject[2].length() > 0 && AO)
					{
						if (line.find("!AO1N!", 0) != string::npos)
						{
							string tempID;
							if (NewAnimObject[AObject[2]].length() != 0)
							{
								tempID = NewAnimObject[AObject[2]];
							}
							else
							{
								tempID = "#" + strID;
								NewAnimObject[AObject[2]] = tempID;
								nextID = true;
							}
							line.replace(line.find("!AO1N!"), 6, tempID);
						}
					}
					else
					{
						break;
					}
				}

				if (TriggerState || TriggerState1)
				{
					recordTrigger.push_back(line);
					break;
				}

				if (line.find("MID$", 0) != string::npos)
				{
					int MIDposition = line.find("MID$");
					int x = line.find("<", MIDposition);
					int y = line.find("\"", MIDposition);
					int position;
					if (x == -1 && y != -1)
					{
						position = y;
					}
					else if (x != -1 && y == -1)
					{
						position = x;
					}
					else if (x != -1 && y != -1)
					{
						position = min(x, y);
					}
					else
					{
						position = line.length();
					}
					string ID = line.substr(MIDposition, position - MIDposition);
					int IDLength = ID.length();

					if (IDExist[ID].length() > 0)
					{
						ID = IDExist[ID];
					}
					else
					{
						IDExist[ID] = strID;
						ID = strID;
						int zero = 4 - ID.length();

						for (int j = 0; j < zero; j++)
						{
							ID = "0" + ID;
						}

						nextID = true;
					}

					line.replace(line.find("MID$"), IDLength, ID);
				}

				if (nextID)
				{
					nextFunctionID++;
					strID = to_string(nextFunctionID);
					nextID = false;
				}

				generatedlines.push_back(line);
				break;
			}
		}
		else if (line.find("<!-- NEW *Trigger0* +% -->", 0) != string::npos)
		{
			if (TriggerTime.size() != 0)
			{
				recordTrigger.clear();
				TriggerState = true;
			}
			else
			{
				TriggerStateIgnore = true;
			}
		}
		else if (line.find("<!-- NEW *Trigger1* +% -->", 0) != string::npos)
		{
			if (TriggerTime1.size() != 0)
			{
				recordTrigger.clear();
				TriggerState1 = true;
			}
			else
			{
				TriggerStateIgnore = true;
			}
		}
		else if (line.find("<!-- CLOSE -->", 0) != string::npos)
		{
			if (TriggerStateIgnore)
			{
				TriggerStateIgnore = false;
			}
			else if (TriggerState)
			{
				TriggerState = false;
				for (auto it = TriggerTime.begin(); it != TriggerTime.end(); ++it)
				{
					for (unsigned int k = 0; k < recordTrigger.size(); k++)
					{
						string triggerline = recordTrigger[k];
						if (triggerline.find("!tT$0!", 0) != string::npos)
						{
							triggerline.replace(triggerline.find("!tT$0!"), 6, it->second);
						}
						else if (triggerline.find("!T$0!", 0) != string::npos)
						{
							triggerline.replace(triggerline.find("!T$0!"), 5, to_string(eventid[it->first]));
						}
						generatedlines.push_back(triggerline);
					}
				}
			}
			else if (TriggerState1)
			{
				TriggerState1 = false;
				for (auto it = TriggerTime1.begin(); it != TriggerTime1.end(); it++)
				{
					for (unsigned int k = 0; k < recordTrigger.size(); k++)
					{
						string triggerline = recordTrigger[k];
						if (triggerline.find("!tT$1!", 0) != string::npos)
						{
							triggerline.replace(triggerline.find("!tT$1!"), 6, it->second);
						}
						else if (triggerline.find("!T$1!", 0) != string::npos)
						{
							triggerline.replace(triggerline.find("!T$1!"), 5, to_string(eventid[it->first]));
						}
						generatedlines.push_back(triggerline);
					}
				}
			}
		}
	}

	return generatedlines;
}

AOConvert KillMove::GetAnimObjectName()
{
	return NewAnimObject;
}

bool KillMove::HasError()
{
	return error;
}

vecstr KillMove::GetEventID()
{
	return eventID;
}

vecstr KillMove::GetVariableID()
{
	return variableID;
}