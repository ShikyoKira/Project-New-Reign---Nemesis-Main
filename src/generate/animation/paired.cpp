#include "Global.h"

#include <boost/regex.hpp>

#include "generate/animation/paired.h"

using namespace std;

Paired::Paired(
    string line, vecstr pairedformat, int pairedcount, AOConvert import, string filepath, int stateID)
{
    bool option     = false;
    int position    = line.find("-");
    string templine = line;
    string animobjects;
    string filename;

    if (position != -1)
    {
        templine       = line.substr(position, line.find(" ", position) - position + 1);
        int positionAE = 3 + templine.length();
        mainAnimEvent  = line.substr(positionAE, line.find(" ", positionAE) - positionAE);

        int fileposition;
        int tempint = 0;
        for (int i = 0; i < 4; i++)
        {
            position = line.find(" ", tempint);
            tempint  = line.find(" ", position + 1);

            if (i == 2) { fileposition = position + 1; }
        }
        animobjects = line.substr(position + 1);

        int x = line.find(" ", fileposition);
        int y = line.length();
        int fileEndPoint;
        if (x == -1) { fileEndPoint = y; }
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
            tempint  = line.find(" ", position + 1);

            if (i == 1) { fileposition = position; }
        }
        animobjects = line.substr(position + 1);
        filename    = line.substr(fileposition + 1, line.find(" ", fileposition) - fileposition - 1);
    }

    // Part 1 - get animobjects
    if (animobjects.length() != 0 && animobjects != line)
    {
        size_t AOcount = count(animobjects.begin(), animobjects.end(), '/');

        if (AOcount > 2 && AOcount < 1) { error = true; }
        else
        {
            int tempint = 0;
            int nextobj = 0;
            for (unsigned int i = 0; i < AOcount; i++)
            {
                position = animobjects.find("/", tempint);
                if (AOcount == 2) { tempint = animobjects.find("/", position + 1); }
                string tempstr = animobjects.substr(nextobj, animobjects.find("/", position) - nextobj);
                int temp       = stoi(animobjects.substr(position + 1, 2));
                if (temp != 1 && temp != 2) { error = true; }
                nextobj       = animobjects.find(" ");
                AObject[temp] = tempstr;
            }
        }
    }

    pairedlines = pairedformat;

    {
        int numline = 7;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 16;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 59;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 68;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 111;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 141;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 156;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 220;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 229;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 272;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 281;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 324;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 354;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));
        numline = 369;
        pairedlines[numline].replace(pairedlines[numline].find("!%!"), 3, to_string(pairedcount));

        numline = 8;
        pairedlines[numline].replace(pairedlines[numline].find("!S$0!"), 5, to_string(stateID));
        numline = 221;
        pairedlines[numline].replace(pairedlines[numline].find("!S$1!"), 5, to_string(stateID + 1));
    }

    if (option)
    {
        while (true)
        {
            if (templine.find("Tn,", 0) != string::npos || templine.find("Tn ", 0) != string::npos)
            {
                nextTrans = true;
                templine.replace(templine.find("Tn"), 2, "Xx");
            }

            // part 2 - get triggers and timing
            if (templine.find("T2_", 0) != string::npos)
            {
                string templine2 = templine.substr(templine.find("T2_"));

                int triggercount = 0;
                int tempInt      = 0;
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

                pairedlines[384].replace(pairedlines[384].find("!nT2$1!"), 7, to_string(triggercount + 1));

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

                        if (y != -1) { tempInt = min(x, y); }
                        else
                        {
                            tempInt = x;
                        }

                        tempstr = templine2.substr(position, tempInt - position);
                    }
                    string eventname        = tempstr.substr(3, tempstr.find("/") - 3);
                    TriggerTime1[eventname] = tempstr.substr(tempstr.find("/") + 1);
                    eventID.push_back(eventname);
                    templine.replace(templine.find(tempstr), tempstr.length(), "xxx");
                }
            }
            else
            {
                pairedlines[384].replace(pairedlines[384].find("!nT2$1!"), 7, "1");
            }

            if (templine.find("T", 0) != string::npos)
            {
                string templine2    = templine.substr(templine.find("T"));
                size_t triggercount = count(templine2.begin(), templine2.end(), 'T');
                pairedlines[171].replace(pairedlines[171].find("!nT$2!"), 6, to_string(triggercount + 2));
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

                        if (y != -1) { tempInt = min(x, y); }
                        else
                        {
                            tempInt = x;
                        }
                        tempstr = templine2.substr(position, tempInt - position);
                    }
                    string eventname       = tempstr.substr(1, tempstr.find("/") - 1);
                    TriggerTime[eventname] = tempstr.substr(tempstr.find("/") + 1);
                    eventID.push_back(eventname);
                    templine.replace(templine.find(tempstr), tempstr.length(), "xxx");
                }
            }
            else
            {
                pairedlines[171].replace(pairedlines[171].find("!nT$2!"), 6, "2");
            }

            if (templine.find("ac0", 0) != string::npos)
            {
                AC0 = true;
                pairedlines[4].replace(pairedlines[4].find("!AC0!"), 5, import["endcamera"]);
                pairedlines[217].replace(pairedlines[217].find("!AC0!"), 5, import["endcamera"]);
            }

            if (templine.find("ac1", 0) != string::npos)
            {
                AC1 = true;
                pairedlines[3].replace(pairedlines[3].find("!AC1!"), 5, import["startcamera"]);
                pairedlines[216].replace(pairedlines[216].find("!AC1!"), 5, import["startcamera"]);
            }

            if (templine.find("ac ", 0) != string::npos || templine.find("ac,", 0) != string::npos)
            {
                AC  = true;
                AC0 = true;
                AC1 = true;
                pairedlines[4].replace(pairedlines[4].find("!AC0!"), 5, import["endcamera"]);
                pairedlines[217].replace(pairedlines[217].find("!AC0!"), 5, import["endcamera"]);
                pairedlines[3].replace(pairedlines[3].find("!AC1!"), 5, import["startcamera"]);
                pairedlines[216].replace(pairedlines[216].find("!AC1!"), 5, import["startcamera"]);
            }
            else
            {
                if (!AC0)
                {
                    pairedlines[4].replace(pairedlines[4].find("!AC0!"), 5, "null");
                    pairedlines[217].replace(pairedlines[217].find("!AC0!"), 5, "null");
                }

                if (!AC1)
                {
                    pairedlines[3].replace(pairedlines[3].find("!AC1!"), 5, "null");
                    pairedlines[216].replace(pairedlines[216].find("!AC1!"), 5, "null");
                }
            }

            if (templine.find("a ", 0) != string::npos || templine.find("a,", 0) != string::npos)
            { Loop = false; }

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
                int tempint = stoi(
                    boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
            }

            if (templine.find("bsa ", 0) != string::npos || templine.find("bsa,", 0) != string::npos) {}

            if (templine.find("h ", 0) != string::npos || templine.find("h,", 0) != string::npos)
            {
                HT = true;
                if (!AO)
                {
                    pairedlines[107].replace(pairedlines[107].find("!HO!"), 4, "null");
                    pairedlines[108].replace(pairedlines[108].find("!HO!"), 4, "null");

                    pairedlines[320].replace(pairedlines[320].find("!HO!"), 4, "null");
                    pairedlines[321].replace(pairedlines[321].find("!HO!"), 4, "null");
                }
                else
                {
                    if (AObject[1].length() > 0)
                    {
                        pairedlines[107].replace(pairedlines[107].find("!HO!"), 4, "#MID$7");
                        pairedlines[108].replace(pairedlines[108].find("!HO!"), 4, import["AOoff"]);
                        pairedlines[118].replace(pairedlines[118].find("!AO0*2$H!"), 9, "2");
                    }
                    else
                    {
                        pairedlines[107].replace(pairedlines[107].find("!HO!"), 4, "null");
                        pairedlines[108].replace(pairedlines[108].find("!HO!"), 4, "null");
                    }

                    if (AObject[2].length() > 0)
                    {
                        pairedlines[320].replace(pairedlines[320].find("!HO!"), 4, "#MID$18");
                        pairedlines[321].replace(pairedlines[321].find("!HO!"), 4, import["AOoff"]);
                        pairedlines[331].replace(pairedlines[331].find("!AO1*2$H!"), 9, "2");
                    }
                    else
                    {
                        pairedlines[320].replace(pairedlines[320].find("!HO!"), 4, "null");
                        pairedlines[321].replace(pairedlines[321].find("!HO!"), 4, "null");
                    }
                }
            }
            else
            {
                if (!AO)
                {
                    pairedlines[107].replace(pairedlines[107].find("!HO!"), 4, import["HToff"]);
                    pairedlines[108].replace(pairedlines[108].find("!HO!"), 4, import["HTon"]);

                    pairedlines[320].replace(pairedlines[320].find("!HO!"), 4, import["HToff"]);
                    pairedlines[321].replace(pairedlines[321].find("!HO!"), 4, import["HTon"]);
                }
                else
                {
                    if (AObject[1].length() > 0)
                    {
                        pairedlines[107].replace(pairedlines[107].find("!HO!"), 4, "#MID$7");
                        pairedlines[108].replace(pairedlines[108].find("!HO!"), 4, import["HTon,AOoff"]);
                        pairedlines[118].replace(pairedlines[118].find("!AO0*2$H!"), 9, "3");
                    }
                    else
                    {
                        pairedlines[107].replace(pairedlines[107].find("!HO!"), 4, import["HToff"]);
                        pairedlines[108].replace(pairedlines[108].find("!HO!"), 4, import["HTon"]);
                    }

                    if (AObject[2].length() > 0)
                    {
                        pairedlines[320].replace(pairedlines[320].find("!HO!"), 4, "#MID$18");
                        pairedlines[321].replace(pairedlines[321].find("!HO!"), 4, import["HTon,AOoff"]);
                        pairedlines[331].replace(pairedlines[331].find("!AO1*2$H!"), 9, "3");
                    }
                    else
                    {
                        pairedlines[320].replace(pairedlines[320].find("!HO!"), 4, import["HToff"]);
                        pairedlines[321].replace(pairedlines[321].find("!HO!"), 4, import["HTon"]);
                    }
                }
            }

            if (templine.find("k ", 0) != string::npos || templine.find("k,", 0) != string::npos)
            { Known = true; }

            if (templine.find("md ", 0) != string::npos || templine.find("md,", 0) != string::npos)
            { MD = true; }

            if (templine.find("st ", 0) != string::npos || templine.find("st,", 0) != string::npos)
            { PAO = true; }

            if (templine.find("D", 0) != string::npos)
            {
                int tempint;
                int x = templine.find(" ", templine.find("D"));
                int y = templine.find(",", templine.find("D"));

                if (y != -1 && x != -1) { tempint = min(x, y); }
                else
                {
                    if (x == -1 && y != -1) { tempint = y; }
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
                duration    = stod(time);
                pairedlines[187].replace(pairedlines[187].find("!END!"), 5, time);
                pairedlines[199].replace(pairedlines[199].find("!END!"), 5, time);
                pairedlines[400].replace(pairedlines[400].find("!END!"), 5, time);
            }
            else
            {
                pairedlines[187].replace(pairedlines[187].find("!END!"), 5, "0.000000");
                pairedlines[194].replace(pairedlines[194].find("false"), 5, "true");
                pairedlines[199].replace(pairedlines[199].find("!END!"), 5, "0.000000");
                pairedlines[206].replace(pairedlines[206].find("false"), 5, "true");
                pairedlines[400].replace(pairedlines[400].find("!END!"), 5, "0.000000");
                pairedlines[407].replace(pairedlines[407].find("false"), 5, "true");
            }

            break;
        }
    }

    if (filename != line)
    {
        pairedlines[157].replace(pairedlines[157].find("@M@"), 3, filepath + filename);
        pairedlines[370].replace(pairedlines[370].find("@M@"), 3, filepath + filename);
    }
    else
    {
        error = true;
    }
}

vecstr Paired::GetPairedLine(int nFunctionID, AOConvert AnimObjectNames, id eventid, id variableid)
{
    vecstr generatedlines;
    vecstr recordTrigger;
    AOConvert IDExist;

    NewAnimObject = AnimObjectNames;

    int nextFunctionID      = nFunctionID;
    bool nextID             = false;
    bool TriggerStateIgnore = false;
    bool TriggerState       = false;
    bool TriggerState1      = false;

    string strID = to_string(nextFunctionID);

    for (unsigned int i = 0; i < pairedlines.size(); i++)
    {
        string line = pairedlines[i];
        if (line.find("<!--", 0) == string::npos && !TriggerStateIgnore)
        {
            while (true)
            {
                while (true)
                {
                    if (strID.length() < 4) { strID = "0" + strID; }
                    else
                    {
                        break;
                    }
                }

                if (i > 116 && i < 138 && (!AO || AObject[1].length() == 0)) { break; }

                if (i > 329 && i < 351 && (!AO || AObject[2].length() == 0)) { break; }

                if (((i > 119 && i < 124) || (i > 332 && i < 337)) && HT) { break; }
                else if (i > 125 && i < 134)
                {
                    if (AObject[1].length() > 0 && AO)
                    {
                        if (line.find("!AO0N!", 0) != string::npos)
                        {
                            string tempID;
                            if (NewAnimObject[AObject[1]].length() != 0)
                            { tempID = NewAnimObject[AObject[1]]; }
                            else
                            {
                                tempID                    = "#" + strID;
                                NewAnimObject[AObject[1]] = tempID;
                                nextID                    = true;
                            }
                            line.replace(line.find("!AO0N!"), 6, tempID);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else if (i > 338 && i < 347)
                {
                    if (AObject[2].length() > 0 && AO)
                    {
                        if (line.find("!AO1N!", 0) != string::npos)
                        {
                            string tempID;
                            if (NewAnimObject[AObject[2]].length() != 0)
                            { tempID = NewAnimObject[AObject[2]]; }
                            else
                            {
                                tempID                    = "#" + strID;
                                NewAnimObject[AObject[2]] = tempID;
                                nextID                    = true;
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
                    int x           = line.find("<", MIDposition);
                    int y           = line.find("\"", MIDposition);
                    int position;
                    if (x == -1 && y != -1) { position = y; }
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
                    string ID    = line.substr(MIDposition, position - MIDposition);
                    int IDLength = ID.length();

                    if (IDExist[ID].length() > 0) { ID = IDExist[ID]; }
                    else
                    {
                        IDExist[ID] = strID;
                        ID          = strID;
                        int zero    = 4 - ID.length();

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
                    strID  = to_string(nextFunctionID);
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
            if (TriggerStateIgnore) { TriggerStateIgnore = false; }
            else if (TriggerState)
            {
                TriggerState = false;
                for (auto it = TriggerTime.begin(); it != TriggerTime.end(); ++it)
                {
                    for (unsigned int k = 0; k < recordTrigger.size(); k++)
                    {
                        string triggerline = recordTrigger[k];
                        if (triggerline.find("!tT$0!", 0) != string::npos)
                        { triggerline.replace(triggerline.find("!tT$0!"), 6, it->second); }
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
                for (auto it = TriggerTime1.begin(); it != TriggerTime1.end(); ++it)
                {
                    for (unsigned int k = 0; k < recordTrigger.size(); k++)
                    {
                        string triggerline = recordTrigger[k];
                        if (triggerline.find("!tT$1!", 0) != string::npos)
                        { triggerline.replace(triggerline.find("!tT$1!"), 6, it->second); }
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

AOConvert Paired::GetAnimObjectName()
{
    return NewAnimObject;
}

bool Paired::HasError()
{
    return error;
}

vecstr Paired::GetEventID()
{
    return eventID;
}

vecstr Paired::GetVariableID()
{
    return variableID;
}