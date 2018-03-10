#include "behaviorgenerator.h"

#pragma warning(disable:4503)

using namespace std;

typedef unordered_map<string, set<string>> mapSetString;

// DataPath skyrimDataPath;
SSMap behaviorpath;

IDCatcher::IDCatcher(int id, int curline)
{
	ID = id;
	line = curline;
}

int IDCatcher::getID()
{
	return ID;
}

int IDCatcher::getLine()
{
	return line;
}

void BehaviorCompilation(string directory, vecstr filelist, int curList, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior, getTemplate BehaviorTemplate, vector<unique_ptr<registerAnimation>>& animationList, unordered_map<string, vector<unique_ptr<Furniture>>>& newAnimation, mapSetString newAnimEvent, mapSetString newAnimVariable, unordered_map<string, var> AnimVar)
{
	ImportContainer exportID;
	string filepath = directory + filelist[curList];
	vecstr behaviorlines;
	map<int, vecstr> catalystMap;
	int curID = -500001;
	int lastID = 0;
	int nexteventid = 0;
	int nextvarid = 0;
	int eventelements = -1;
	int variableelements = -1;
	int attributeelements = -1;
	int characterelements = -1;
	int normalelements = -1;

	bool eventOpen = false;
	bool varOpen = false;
	bool attriOpen = false;
	bool charOpen = false;
	bool norElement = false;

	id eventid;
	id variableid;
	id attriid;
	id charid;
	unordered_map<int, string> eventName;
	unordered_map<int, string> varName;
	unordered_map<int, string> attriName;
	unordered_map<int, string> charName;
	unordered_map<string, vector<int>> newAnimCon;
	unordered_map<string, vecstr> parent;

	// read behavior file
	vecstr catalyst = GetFunctionLines(filepath);

	// check for error
	if (error)
	{
		return;
	}

	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time 2: " << duration / 1000 << " seconds" << endl;

	for (unsigned int l = 0; l < catalyst.size(); l++)
	{
		string line = catalyst[l];

		if (line.find("<hkobject name=\"", 0) != string::npos && line.find("signature=\"", 0) != string::npos)
		{
			int tempID = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

			if (tempID >= lastID)
			{
				lastID = tempID + 1;
			}
		}
	}

	if (error)
	{
		return;
	}


	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 3: " << duration / 1000 << " seconds" << endl;

	int newModID = -500000;
	int counter = 0;
	size_t elementLine = -1;
	size_t openRange = 0;
	bool isOpen = true;
	bool special = false;
	string curNum;
	string ZeroEvent;
	SSMap IDExist;
	vecstr recorder;
	unordered_map<string, vector<IDCatcher>> catcher;
	unordered_map<string, string> chosenLines;

	// add picked behavior and remove not picked behavior
	for (unsigned int l = 0; l < catalyst.size(); l++)
	{
		bool elementCatch = false;
		bool isNewID = false;
		string line = catalyst[l];
		bool skip = false;

		if (line.find("<!-- ", 0) != string::npos)
		{
			if (line.find("<!-- NEW *", 0) != string::npos)
			{
				size_t tempint = line.find("<!-- NEW *", 0) + 10;
				string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

				if (!chosenBehavior[modID])
				{
					isOpen = false;
				}

				skip = true;
			}
			else if (line.find("<!-- NEW ^", 0) != string::npos)
			{
				special = true;
			}
			else if (line.find("<!-- CLOSE -->", 0) != string::npos)
			{
				if (!isOpen)
				{
					isOpen = true;
				}

				if (!special)
				{
					skip = true;
				}
				else
				{
					special = false;
				}
			}
		}

		if (isOpen && !skip)
		{
			if (line == "	</hksection>")
			{
				break;
			}

			while(true)
			{
				if (line.find("<hkobject name=\"", 0) != string::npos && line.find("signature=\"", 0) != string::npos)
				{
					string tempID = boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

					if (line.find("\"#" + tempID + "\"") != string::npos)
					{
						catalystMap[curID].shrink_to_fit();
						curID = stoi(tempID);
						catalystMap[curID].reserve(100 * memory);
					}
					else
					{
						catalystMap[curID].shrink_to_fit();
						isNewID = true;
					}
				}

				if (line.find("<!-- *", 0) != string::npos)
				{
					size_t tempint = line.find("<!-- *") + 6;
					string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
					chosenLines[modID] = line;
					break;
				}
				else if (line.find("<!-- original -->", 0) != string::npos)
				{
					if (chosenLines.size() != 0)
					{
						line = behaviorLineChooser(line, chosenLines, behaviorPriority);
						chosenLines.clear();
					}
					else
					{
						cout << ">> ERROR(1165): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
						error = true;
						return;
					}
				}

				// get event id from newAnimation
				if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != string::npos || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != string::npos)
				{
					if (eventelements == -1)
					{
						if (line.find("</hkparam>") == string::npos)
						{
							eventOpen = true;
							elementCatch = true;
							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
						}
					}
					else
					{
						eventOpen = true;
						string templine = line.substr(0, line.find("<hkparam name=\"", 0));
						openRange = count(templine.begin(), templine.end(), '\t');
						size_t position = line.find("numelements=\"") + 13;
						string element = line.substr(position, line.find("\">") - position);
						line.replace(line.find(element), element.length(), to_string(eventelements));
					}

					size_t position = line.find("<hkparam name=\"") + 15;
					curNum = line.substr(position, line.find("\" numelements=\"") - position);
				}
				else if (line.find("<hkparam name=\"attributeNames\" numelements=") != string::npos || line.find("<hkparam name=\"attributeDefaults\" numelements=") != string::npos)
				{
					if (attributeelements == -1)
					{
						if (line.find("</hkparam>") == string::npos)
						{
							attriOpen = true;
							elementCatch = true;
							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
						}
					}
					else
					{
						size_t position = line.find("numelements=\"") + 13;
						string element = line.substr(position, line.find("\">") - position);
						line.replace(line.find(element), element.length(), to_string(attributeelements));
					}

					size_t position = line.find("<hkparam name=\"") + 15;
					curNum = line.substr(position, line.find("\" numelements=\"") - position);
				}
				else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=") != string::npos || line.find("<hkparam name=\"characterPropertyInfos\" numelements=") != string::npos)
				{
					if (characterelements == -1)
					{
						if (line.find("</hkparam>") == string::npos)
						{
							charOpen = true;
							elementCatch = true;
							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
						}
					}
					else
					{
						size_t position = line.find("numelements=\"") + 13;
						string element = line.substr(position, line.find("\">") - position);
						line.replace(line.find(element), element.length(), to_string(characterelements));
					}

					size_t position = line.find("<hkparam name=\"") + 15;
					curNum = line.substr(position, line.find("\" numelements=\"") - position);
				}
				else if (line.find("<hkparam name=\"variableNames\" numelements=") != string::npos || line.find("<hkparam name=\"wordVariableValues\" numelements=") != string::npos || line.find("<hkparam name=\"variableInfos\" numelements=") != string::npos)
				{
					if (variableelements == -1)
					{
						if (line.find("</hkparam>") == string::npos)
						{
							varOpen = true;
							elementCatch = true;
						}
					}
					else
					{
						varOpen = true;
						size_t position = line.find("numelements=\"") + 13;
						string element = line.substr(position, line.find("\">") - position);
						line.replace(line.find(element), element.length(), to_string(variableelements));
					}

					string templine = line.substr(0, line.find("<hkparam name=\"", 0));
					openRange = count(templine.begin(), templine.end(), '\t');
					size_t position = line.find("<hkparam name=\"") + 15;
					curNum = line.substr(position, line.find("\" numelements=\"") - position);
				}
				else if (line.find("<hkparam name=\"") != string::npos && line.find("numelements=\"") != string::npos && line.find("</hkparam>") == string::npos)
				{
					norElement = true;
					elementCatch = true;
					string templine = line.substr(0, line.find("<hkparam name=\"", 0));
					openRange = count(templine.begin(), templine.end(), '\t');
				}
				else if (line.find("</hkparam>") != string::npos && (norElement || eventOpen || varOpen || attriOpen || charOpen))
				{
					string templine = line.substr(0, line.find("</hkparam>"));
					size_t range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						// reset
						if (norElement)
						{
							norElement = false;
						}
						else if (eventOpen)
						{
							vecstr codelist = BehaviorTemplate.grouplist[filelist[curList].substr(0, filelist[curList].find_last_of("."))];

							for (unsigned int j = 0; j < codelist.size(); j++)
							{
								for (auto it = newAnimEvent[codelist[j]].begin(); it != newAnimEvent[codelist[j]].end(); ++it)
								{
									if (curNum == "eventNames")
									{
										string eventname = *it;
										string curline = "				<hkcstring>" + eventname + "</hkcstring>";

										if (eventid[eventname] != 0)
										{
											cout << "ERROR(1050): Duplicated event name found. Please change the name of the event name" << endl << "Event Name: " << eventname << endl << endl;
											error = true;
											return;
										}

										eventid[eventname] = counter;
										eventName[counter] = eventname;

										if (counter == 0)
										{
											ZeroEvent = eventname;
										}

										catalystMap[curID].push_back(curline);
									}
									else if (curNum == "eventInfos")
									{
										catalystMap[curID].push_back("				<hkobject>");
										catalystMap[curID].push_back("					<hkparam name=\"flags\">0</hkparam>");
										catalystMap[curID].push_back("				</hkobject>");
									}
									else
									{
										cout << ">> ERROR(1031): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
										error = true;
										return;
									}

									counter++;
								}
							}

							if (eventelements == -1)
							{
								eventelements = counter;
							}

							eventOpen = false;
						}
						else if (varOpen)
						{
							vecstr codelist = BehaviorTemplate.grouplist[filelist[curList].substr(0, filelist[curList].find_last_of("."))];

							for (unsigned int j = 0; j < codelist.size(); j++)
							{
								for (auto it = newAnimVariable[codelist[j]].begin(); it != newAnimVariable[codelist[j]].end(); ++it)
								{
									string variablename = *it;

									if (curNum == "variableNames")
									{
										string curline = "				<hkcstring>" + variablename + "</hkcstring>";
										varName[counter] = variablename;
										variableid[variablename] = counter;
										catalystMap[curID].push_back(curline);
									}
									else if (curNum == "wordVariableValues")
									{
										catalystMap[curID].push_back("				<hkobject>");
										catalystMap[curID].push_back("					<hkparam name=\"value\">" + AnimVar[variablename].init_value + "</hkparam>");
										catalystMap[curID].push_back("				</hkobject>");
									}
									else if (curNum == "variableInfos")
									{
										catalystMap[curID].push_back("				<hkobject>");
										catalystMap[curID].push_back("					<hkparam name=\"role\">");
										catalystMap[curID].push_back("						<hkobject>");
										catalystMap[curID].push_back("							<hkparam name=\"role\">ROLE_DEFAULT</hkparam>");
										catalystMap[curID].push_back("							<hkparam name=\"flags\">0</hkparam>");
										catalystMap[curID].push_back("						</hkobject>");
										catalystMap[curID].push_back("					</hkparam>");
										catalystMap[curID].push_back("					<hkparam name=\"type\">VARIABLE_TYPE_" + AnimVar[variablename].var_type + "</hkparam>");
										catalystMap[curID].push_back("				</hkobject>");
									}
									else
									{
										cout << ">> ERROR(1032): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
										error = true;
										return;
									}

									counter++;
								}
							}

							if (variableelements == -1)
							{
								variableelements = counter;
							}

							varOpen = false;
						}
						else if (attriOpen)
						{
							if (attributeelements == -1)
							{
								attributeelements = counter;
							}

							attriOpen = false;
						}
						else if (charOpen)
						{
							if (characterelements == -1)
							{
								characterelements = counter;
							}

							charOpen = false;
						}

						if (elementLine != -1)
						{
							size_t position = catalystMap[curID][elementLine].find("numelements=\"") + 13;
							string oldElement = catalystMap[curID][elementLine].substr(position, catalystMap[curID][elementLine].find("\">", position) - position);

							if (oldElement != to_string(counter))
							{
								catalystMap[curID][elementLine].replace(catalystMap[curID][elementLine].find(oldElement), oldElement.length(), to_string(counter));
							}

							elementLine = -1;
						}

						curNum = "";
						counter = 0;
						openRange = 0;
					}
				}

				if (line.find("<hkobject name=\"", 0) != string::npos && line.find("signature=\"", 0) != string::npos)
				{
					size_t nextpos = line.find("<hkobject name=\"", 0) + 17;
					string funcID = line.substr(nextpos, line.find("class=\"", nextpos) - nextpos - 2);

					if (funcID.find("$", 0) != string::npos)
					{
						string modID = funcID.substr(0, funcID.find("$"));
						if (chosenBehavior[modID])
						{
							string ID = to_string(lastID);
							size_t zero = 4 - ID.length();

							for (size_t j = 0; j < zero; j++)
							{
								ID = "0" + ID;
							}

							line.replace(line.find(funcID), funcID.length(), ID);
							IDExist[funcID] = ID;

							if (isNewID)
							{
								curID = lastID;
								catalystMap[curID].reserve(100 * memory);
								isNewID = false;
								lastID++;
							}
						}
						else if (modID == "MID")
						{
							cout << ">> ERROR(1020): BUG FOUND!! Function ID Leakage. Report to Nemesis' author! <<" << endl;
							error = true;
							produceBugReport(directory, chosenBehavior);
							return;
						}
					}
				}
				else if (line.find("$", line.find("#")) != string::npos)
				{
					if (line.find(">#") != string::npos)
					{
						size_t reference = count(line.begin(), line.end(), '#');
						size_t nextpos = 0;

						for (size_t k = 0; k < reference; k++)
						{
							nextpos = line.find("#", nextpos) + 1;
							string numID = boost::regex_replace(string(line.substr(nextpos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							string ID = line.substr(nextpos, line.find(numID) - nextpos + numID.length());

							if (line.find(ID, 0) != string::npos && ID.find("$") != string::npos)
							{
								if (IDExist[ID].length() != 0)
								{
									line.replace(nextpos, ID.length(), IDExist[ID]);
								}
								else if (!special)
								{
									IDCatcher catchingID(curID, int(catalystMap[curID].size()));
									catcher[ID].push_back(catchingID);
								}
							}
						}
					}
					else if (line.find("\t\t\t#") != string::npos)
					{
						stringstream sstream(line);
						istream_iterator<string> ssbegin(sstream);
						istream_iterator<string> ssend;
						vecstr generator(ssbegin, ssend);
						copy(generator.begin(), generator.end(), generator.begin());

						for (unsigned int p = 0; p < generator.size(); p++)
						{
							string ID = generator[p];
							string numID = boost::regex_replace(string(ID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (ID.find("$") != string::npos)
							{
								string masterFormat = ID.substr(1, ID.find("$") - 1);

								if (ID == "#" + masterFormat + "$" + numID && line.find(ID, 0) != string::npos)
								{
									ID = ID.substr(1, ID.length() - 1);

									if (IDExist[ID].length() != 0)
									{
										line.replace(line.find(ID), ID.length(), IDExist[ID]);
									}
									else if (!special)
									{
										IDCatcher catchingID(curID, int(catalystMap[curID].size()));
										catcher[ID].push_back(catchingID);
									}
								}
							}
						}
					}
				}

				if (eventOpen && line.find("<hkcstring>") != string::npos)
				{
					size_t nextpos = line.find("<hkcstring>") + 11;
					string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
					eventName[counter] = name;
					eventid[name] = counter;

					if (counter == 0)
					{
						ZeroEvent = name;
					}

					counter++;
				}
				else if (varOpen && line.find("<hkcstring>") != string::npos)
				{
					size_t nextpos = line.find("<hkcstring>") + 11;
					string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
					varName[counter] = name;
					variableid[name] = counter;
					counter++;
				}
				else if (attriOpen && line.find("<hkcstring>") != string::npos)
				{
					size_t nextpos = line.find("<hkcstring>") + 11;
					string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
					attriName[counter] = name;
					attriid[name] = counter;
					counter++;
				}
				else if (charOpen && line.find("<hkcstring>") != string::npos)
				{
					size_t nextpos = line.find("<hkcstring>") + 11;
					string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
					charName[counter] = name;
					charid[name] = counter;
					counter++;
				}
				else if (norElement)
				{
					string templine = catalyst[l];

					if (templine.find("<hkobject>") != string::npos)
					{
						templine = templine.substr(0, templine.find("<hkobject>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (range == openRange + 1)
						{
							counter++;
						}
					}
					else if (templine.find("\t\t\t#") != string::npos)
					{
						templine = templine.substr(0, templine.find("#", 0));
						size_t reference = count(templine.begin(), templine.end(), '\t');

						if (reference == openRange + 1)
						{
							int number = int(count(catalyst[l].begin(), catalyst[l].end(), '#'));
							counter += number;
						}
					}
				}

				if (isNewID)
				{
					curID = newModID;
					catalystMap[curID].reserve(100 * memory);
					newModID++;
				}

				if (line.find("$") != string::npos)
				{
					__int64 counter = count(line.begin(), line.end(), '$') / 2;
					size_t curPos = 0;

					for (int i = 0; i < counter; ++i)
					{
						bool isChange = false;
						curPos = line.find("$", curPos + 1);
						string change = line.substr(curPos, line.find("$", curPos + 1) - curPos + 1);
						string oldChange = change;
						change = change.substr(1, change.length() - 2);
						curPos = line.find("$", curPos + 1);

						if (change.find("eventID[", 0) != string::npos &&  change.find("]", 0) != string::npos)
						{
							eventIDReplacer(change, "BASE", eventid, ZeroEvent, l + 1);
							isChange = true;
						}

						if (error)
						{
							return;
						}

						if (change.find("variableID[", 0) != string::npos &&  change.find("]", 0) != string::npos)
						{
							variableIDReplacer(change, "BASE", variableid, l + 1);
							isChange = true;
						}

						if (error)
						{
							return;
						}

						if (isChange)
						{
							line.replace(line.find(oldChange), oldChange.length(), change);
						}
					}
				}

				catalystMap[curID].push_back(line);

				if (elementCatch)
				{
					elementLine = catalystMap[curID].size() - 1;
				}

				break;
			}
		}

		if (error)
		{
			return;
		}
	}

	catalystMap[curID].shrink_to_fit();

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 4: " << duration / 1000 << " seconds" << endl;

	// newAnimation ID in existing function
	if (catcher.size() != 0)
	{
		for (auto it = catcher.begin(); it != catcher.end(); ++it)
		{
			for (unsigned int k = 0; k < it->second.size(); k++)
			{
				int tempID = it->second[k].getID();
				int tempLine = it->second[k].getLine();
				string* line = &catalystMap[it->second[k].getID()][it->second[k].getLine()];

				if (IDExist[it->first].length() != 0)
				{
					line->replace(line->find(it->first), it->first.length(), IDExist[it->first]);
				}
				else
				{
					cout << "ERROR(1026): Missing referencing ID. Could be a bug. Report to Nemesis' author if you are sure" << endl << "File: " << filepath << endl << "Mod ID: " << it->first.substr(0, it->first.find("$")) << endl << endl;
					error = true;
					return;
				}
			}
		}
	}

	if (error)
	{
		return;
	}

	// add new animation
	vecstr newAnimationLines;
	vector<unique_ptr<vecstr>> allEditLines;

	for (auto it = BehaviorTemplate.grouplist.begin(); it != BehaviorTemplate.grouplist.end(); ++it)
	{
		// check for having newAnimation for the file
		if (it->second.size() > 0 && (filelist[curList] == it->first + ".nmd" || filelist[curList] == it->first + ".txt"))
		{
			vecstr templateGroup = it->second;

			for (unsigned int j = 0; j < templateGroup.size(); j++)
			{
				if (newAnimation[templateGroup[j]].size() != 0)
				{
					vector<SSMap> subFunctionIDs;
					subFunctionIDs.reserve(memory);
					vector<vector<SSMap>> groupFunctionIDs;
					groupFunctionIDs.reserve(memory);
					string templateCode = templateGroup[j];
					int stateID = 0;
					bool hasGroup = false;
					bool hasMaster = false;

					if (BehaviorTemplate.behaviortemplate[templateCode + "_group"].size() != 0)
					{
						hasGroup = true;
					}

					if (BehaviorTemplate.behaviortemplate[templateCode + "_master"].size() != 0)
					{
						hasMaster = true;
					}

					vector<vector<shared_ptr<animationInfo>>> groupAnimInfo;
					int stateMultiplier = GetStateCount(BehaviorTemplate.behaviortemplate[templateCode]);

					if (!hasGroup)
					{
						stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode], catalystMap);
						groupAnimInfo.push_back(newAnimation[templateCode][0]->GetGroupAnimInfo());
					}

					// individual animation
					for (unsigned int k = 0; k < newAnimation[templateCode].size(); k++)
					{
						// getlines from newAnination
						unique_ptr<Furniture> dummyAnimation = move(newAnimation[templateCode][k]);
						dummyAnimation->setZeroEvent(ZeroEvent);
						unique_ptr<vecstr> tempoLines = make_unique<vecstr>(dummyAnimation->GetFurnitureLine(lastID, exportID, eventid, variableid, stateID, stateMultiplier));

						if (error)
						{
							error = true;
							return;
						}

						subFunctionIDs.push_back(dummyAnimation->GetNewIDs());
						allEditLines.emplace_back(move(tempoLines));

						if (hasGroup && dummyAnimation->isLast())
						{
							subFunctionIDs.shrink_to_fit();
							groupFunctionIDs.push_back(subFunctionIDs);
							subFunctionIDs.clear();
							subFunctionIDs.reserve(memory);
							groupAnimInfo.push_back(dummyAnimation->GetGroupAnimInfo());
						}
					}

					subFunctionIDs.shrink_to_fit();

					time2 = boost::posix_time::microsec_clock::local_time();
					diff = time2 - time1;

					duration = double(diff.total_milliseconds());

					cout << "Processing time 4.1: " << duration / 1000 << " seconds" << endl;

					// group animation
					if (hasGroup)
					{
						if (hasMaster)
						{
							stateID = 0;
						}
						else
						{
							stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode], catalystMap);
						}

						for (unsigned int k = 0; k < groupFunctionIDs.size(); k++)
						{
							string filename = templateCode + "_group";
							groupTemplate group(BehaviorTemplate.behaviortemplate[filename]);
							group.setZeroEvent(ZeroEvent);
							unique_ptr<vecstr> groupBehaviorTemplate = make_unique<vecstr>(group.getFunctionLines(filename, stateID, groupFunctionIDs[k], groupAnimInfo[k], lastID, exportID, eventid, variableid, templateCode, k));

							if (error)
							{
								error = true;
								return;
							}

							allEditLines.emplace_back(move(groupBehaviorTemplate));
							subFunctionIDs.push_back(group.getFunctionIDs());
						}
					}

					subFunctionIDs.shrink_to_fit();

					time2 = boost::posix_time::microsec_clock::local_time();
					diff = time2 - time1;

					duration = double(diff.total_milliseconds());

					cout << "Processing time 4.2: " << duration / 1000 << " seconds" << endl;

					// master animation
					if (hasMaster)
					{
						string filename = templateCode + "_master";
						stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode], catalystMap);
						groupTemplate master(BehaviorTemplate.behaviortemplate[filename]);
						master.setZeroEvent(ZeroEvent);
						unique_ptr<vecstr> masterBehaviorTemplate = make_unique<vecstr>(master.getFunctionLines(filename, stateID, subFunctionIDs, groupAnimInfo[0], lastID, exportID, eventid, variableid, templateCode, -1));

						if (error)
						{
							error = true;
							return;
						}

						allEditLines.emplace_back(move(masterBehaviorTemplate));
						subFunctionIDs.clear();
						subFunctionIDs.push_back(master.getFunctionIDs());
					}

					// existing function
					for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode].size(); k++)
					{
						ExistingFunction exFunction;
						exFunction.setZeroEvent(ZeroEvent);
						catalystMap[BehaviorTemplate.existingFunctionID[templateCode][k]] = exFunction.groupExistingFunctionProcess(catalystMap[BehaviorTemplate.existingFunctionID[templateCode][k]], subFunctionIDs, groupAnimInfo[0], templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup);

						if (catalystMap[BehaviorTemplate.existingFunctionID[templateCode][k]].size() == 0 || error)
						{
							error = true;
							return;
						}
					}

					time2 = boost::posix_time::microsec_clock::local_time();
					diff = time2 - time1;

					duration = double(diff.total_milliseconds());

					cout << "Processing time 4.3: " << duration / 1000 << " seconds" << endl;

					if (error)
					{
						return;
					}
				}
			}
		}
	}

	// check for error
	if (error)
	{
		return;
	}


	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 5: " << duration / 1000 << " seconds" << endl;

	size_t reserveSize = 0;

	for (unsigned int j = 0; j < allEditLines.size(); j++)
	{
		reserveSize += allEditLines[j]->size();
	}

	for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
	{
		reserveSize += it->second.size();
	}

	behaviorlines.reserve(behaviorlines.size() + reserveSize + 4);

	for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
	{
		behaviorlines.insert(behaviorlines.end(), it->second.begin(), it->second.end());
	}

	for (unsigned int j = 0; j < allEditLines.size(); j++)
	{
		behaviorlines.insert(behaviorlines.end(), allEditLines[j]->begin(), allEditLines[j]->end());
	}

	// output import functions
	// Must be in vector
	vector<ImportContainer> groupExportID;
	groupExportID.push_back(exportID);
	vecstr additionallines = importOutput(groupExportID, int(groupExportID.size() - 1), lastID, filelist[curList]);
	behaviorlines.insert(behaviorlines.end(), additionallines.begin(), additionallines.end());
	behaviorlines.push_back("	</hksection>");
	behaviorlines.push_back("");
	behaviorlines.push_back("</hkpackfile>");

	if (error)
	{
		return;
	}


	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 6: " << duration / 1000 << " seconds" << endl;

	// final output
	
	string filename = "new_behaviors/" + filelist[curList];
	ofstream output(filename.substr(0, filename.find_last_of(".")) + ".xml");
	FunctionWriter fwriter(&output);

	if (output.is_open())
	{
		for (unsigned int i = 0; i < behaviorlines.size(); i++)
		{
			fwriter << behaviorlines[i] << "\n";
		}

		output.close();
	}
	else
	{
		cout << "ERROR(1025): Fail to output file" << endl << "File: " << filename << endl << endl;
		error = true;
		return;
	}
}

void GenerateBehavior(string directory, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior)
{
	// register animation & organize AE n Var
	unordered_map<string, int> animationCount;
	getTemplate BehaviorTemplate;
	vector<unique_ptr<registerAnimation>> animationList = move(openFile(BehaviorTemplate));
	unordered_map<string, vector<unique_ptr<Furniture>>> newAnimation;
	mapSetString newAnimEvent;
	mapSetString newAnimVariable;
	unordered_map<string, var> AnimVar;

	// check for error
	if (error)
	{
		return;
	}

	// read each animation list file
	for (unsigned int i = 0; i < animationList.size(); i++)
	{
		string modID = animationList[i]->modID;

		// read each animation in a group of the same type
		for (auto it = animationList[i]->templateType.begin(); it != animationList[i]->templateType.end(); ++it)
		{
			string firstAE;
			string templatecode = it->first;
			vector<shared_ptr<animationInfo>> animInfoGroup;
			vecstr* functionlines = &BehaviorTemplate.behaviortemplate[templatecode];
			newAnimation[templatecode].reserve(50 * memory);
			bool isGroup;
			
			if (BehaviorTemplate.behaviortemplate[templatecode + "_group"].size() != 0)
			{
				animInfoGroup.reserve(100);
				isGroup = true;
			}
			else
			{
				animInfoGroup.reserve(50 * memory);
				isGroup = false;
			}

			// read each line and generate lines categorized using template code (animation type)
			for (int j = 0; j < it->second; ++j)
			{
				int order = animationList[i]->isMulti[templatecode][j];
				int lastOrder = animationList[i]->last[templatecode][j];

				newAnimation[templatecode].emplace_back(make_unique<Furniture>(*functionlines, templatecode, animationCount[templatecode]++, modID + "\\", *animationList[i]->animInfo[templatecode][j]));
				vecstr tempEventID = newAnimation[templatecode].back()->GetEventID();
				vecstr tempVariableID = newAnimation[templatecode].back()->GetVariableID();
				newAnimEvent[templatecode].insert(tempEventID.begin(), tempEventID.end());
				newAnimVariable[templatecode].insert(tempVariableID.begin(), tempVariableID.end());

				if (order != 0)
				{
					newAnimation[templatecode].back()->setOrder(order);
				}
				else
				{
					firstAE = newAnimation[templatecode].back()->mainAnimEvent;

					if (BehaviorTemplate.optionlist[templatecode].eleEventGroupF.size() != 0)
					{
						vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleEventGroupF;
						vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleEventGroupFLine;
						ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
						unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

						for (unsigned int k = 0; k < elementList->size(); ++k)
						{
							vecstr* element = &(*elementList)[k];
							string elementLine = (*elementListLine)[k];
							vector<vecstr> groupAddOnElement;
							groupAddOnElement.reserve(memory / 10);

							for (unsigned int l = 0; l < element->size(); ++l)
							{
								string templine = (*element)[l];

								if (boost::iequals(templine, "main_anim_event"))
								{
									templine = newAnimation[templatecode].back()->mainAnimEvent;
									elementLine.replace(elementLine.find("$$"), 2, templine);
								}
								else
								{
									bool isDone = false;

									for (auto it = addOn.begin(); it != addOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, iter->second);
												isBreak = true;
												isDone = true;
												break;
											}
										}

										if (isBreak)
										{
											break;
										}
									}

									if (!isDone)
									{
										for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
										{
											bool isBreak = false;

											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, "##");
													isBreak = true;
													isDone = true;
													break;
												}

												groupAddOnElement.push_back(iter->second);
											}

											if (isBreak)
											{
												break;
											}
										}

									}
								}
							}

							if (groupAddOnElement.size() != 0)
							{
								vecstr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < newAnimEvent.size(); ++l)
								{
									newAnimEvent[templatecode].insert(animEvent[l]);
								}
							}
							else
							{
								newAnimEvent[templatecode].insert(elementLine);
							}
						}
					}

					if (BehaviorTemplate.optionlist[templatecode].eleVarGroupF.size() != 0)
					{
						vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleVarGroupF;
						vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleVarGroupFLine;
						ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
						unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

						for (unsigned int k = 0; k < elementList->size(); ++k)
						{
							vecstr* element = &(*elementList)[k];
							string elementLine = (*elementListLine)[k];
							vector<vecstr> groupAddOnElement;
							groupAddOnElement.reserve(memory / 10);

							for (unsigned int l = 0; l < element->size(); ++l)
							{
								string templine = (*element)[l];

								if (boost::iequals(templine, "main_anim_event"))
								{
									templine = newAnimation[templatecode].back()->mainAnimEvent;
									elementLine.replace(elementLine.find("$$"), 2, templine);
								}
								else
								{
									bool isBreak = false;

									for (auto it = addOn.begin(); it != addOn.end(); ++it)
									{
										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, iter->second);
												isBreak = true;
												break;
											}
										}

										if (isBreak)
										{
											break;
										}
									}

									if (!isBreak)
									{
										bool isDone = false;

										for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
										{
											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, "##");
													isDone = true;
													break;
												}

												groupAddOnElement.push_back(iter->second);
											}

											if (isDone)
											{
												break;
											}
										}
									}
								}
							}

							if (groupAddOnElement.size() != 0)
							{
								vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < animVar.size(); ++l)
								{
									newAnimEvent[templatecode].insert(animVar[l]);
								}
							}
							else
							{
								newAnimEvent[templatecode].insert(elementLine);
							}
						}
					}
				}

				newAnimation[templatecode].back()->setLastOrder(lastOrder);
				animInfoGroup.push_back(move(animationList[i]->animInfo[templatecode][j]));

				if (lastOrder == order && isGroup)
				{
					size_t animationSize = newAnimation[templatecode].size();
					animInfoGroup.shrink_to_fit();

					for (int k = 0; k < order + 1; ++k)
					{
						newAnimation[templatecode][animationSize - k - 1]->addGroupAnimInfo(animInfoGroup);

						if (error)
						{
							return;
						}
					}

					animInfoGroup.clear();
					animInfoGroup.reserve(100);

					if (BehaviorTemplate.optionlist[templatecode].eleEventGroupL.size() != 0)
					{
						vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleEventGroupL;
						vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleEventGroupLLine;
						ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
						unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

						for (unsigned int k = 0; k < elementList->size(); ++k)
						{
							vecstr* element = &(*elementList)[k];
							string elementLine = (*elementListLine)[k];
							vector<vecstr> groupAddOnElement;
							groupAddOnElement.reserve(memory / 10);

							for (unsigned int l = 0; l < element->size(); ++l)
							{
								string templine = (*element)[l];

								if (boost::iequals(templine, "main_anim_event"))
								{
									templine = newAnimation[templatecode].back()->mainAnimEvent;
									elementLine.replace(elementLine.find("$$"), 2, templine);
								}
								else
								{
									bool isDone = false;

									for (auto it = addOn.begin(); it != addOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, iter->second);
												isBreak = true;
												isDone = true;
												break;
											}
										}

										if (isBreak)
										{
											break;
										}
									}

									if (!isDone)
									{
										for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
										{
											bool isBreak = false;

											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, "##");
													isBreak = true;
													isDone = true;
													break;
												}

												groupAddOnElement.push_back(iter->second);
											}

											if (isBreak)
											{
												break;
											}
										}

									}
								}
							}

							if (groupAddOnElement.size() != 0)
							{
								vecstr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < animEvent.size(); ++l)
								{
									newAnimEvent[templatecode].insert(animEvent[l]);
								}
							}
							else
							{
								newAnimEvent[templatecode].insert(elementLine);
							}
						}
					}

					if (BehaviorTemplate.optionlist[templatecode].eleVarGroupL.size() != 0)
					{
						vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleVarGroupL;
						vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleVarGroupLLine;
						ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
						unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

						for (unsigned int k = 0; k < elementList->size(); ++k)
						{
							vecstr* element = &(*elementList)[k];
							string elementLine = (*elementListLine)[k];
							vector<vecstr> groupAddOnElement;
							groupAddOnElement.reserve(memory / 10);

							for (unsigned int l = 0; l < element->size(); ++l)
							{
								string templine = (*element)[l];

								if (boost::iequals(templine, "main_anim_event"))
								{
									templine = newAnimation[templatecode].back()->mainAnimEvent;
									elementLine.replace(elementLine.find("$$"), 2, templine);
								}
								else
								{
									bool isDone = false;

									for (auto it = addOn.begin(); it != addOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, iter->second);
												isBreak = true;
												isDone = true;
												break;
											}
										}

										if (isBreak)
										{
											break;
										}
									}

									if (!isDone)
									{
										for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
										{
											bool isBreak = false;

											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, "##");
													isBreak = true;
													isDone = true;
													break;
												}

												groupAddOnElement.push_back(iter->second);
											}

											if (isBreak)
											{
												break;
											}
										}

									}
								}
							}

							if (groupAddOnElement.size() != 0)
							{
								vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < animVar.size(); ++l)
								{
									newAnimEvent[templatecode].insert(animVar[l]);
								}
							}
							else
							{
								newAnimEvent[templatecode].insert(elementLine);
							}
						}
					}
				}

				if (BehaviorTemplate.optionlist[templatecode].eleEvent.size() != 0)
				{
					vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleEvent;
					vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleEventLine;
					ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
					unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

					for (unsigned int k = 0; k < elementList->size(); ++k)
					{
						vecstr* element = &(*elementList)[k];
						string elementLine = (*elementListLine)[k];
						vector<vecstr> groupAddOnElement;
						groupAddOnElement.reserve(memory / 10);

						for (unsigned int l = 0; l < element->size(); ++l)
						{
							string templine = (*element)[l];

							if (boost::iequals(templine, "main_anim_event"))
							{
								templine = newAnimation[templatecode].back()->mainAnimEvent;
								elementLine.replace(elementLine.find("$$"), 2, templine);
							}
							else
							{
								bool isDone = false;

								for (auto it = addOn.begin(); it != addOn.end(); ++it)
								{
									bool isBreak = false;

									for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
									{
										if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
										{
											elementLine.replace(elementLine.find("$$"), 2, iter->second);
											isBreak = true;
											isDone = true;
											break;
										}
									}

									if (isBreak)
									{
										break;
									}
								}

								if (!isDone)
								{
									for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, "##");
												isBreak = true;
												isDone = true;
												break;
											}

											groupAddOnElement.push_back(iter->second);
										}

										if (isBreak)
										{
											break;
										}
									}

								}
							}
						}

						if (groupAddOnElement.size() != 0)
						{
							vecstr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

							for (unsigned int l = 0; l < animEvent.size(); ++l)
							{
								newAnimEvent[templatecode].insert(animEvent[l]);
							}
						}
						else
						{
							newAnimEvent[templatecode].insert(elementLine);
						}
					}
				}

				if (BehaviorTemplate.optionlist[templatecode].eleVar.size() != 0)
				{
					vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleVar;
					vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleVarLine;
					ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
					unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

					for (unsigned int k = 0; k < elementList->size(); ++k)
					{
						vecstr* element = &(*elementList)[k];
						string elementLine = (*elementListLine)[k];
						vector<vecstr> groupAddOnElement;
						groupAddOnElement.reserve(memory / 10);

						for (unsigned int l = 0; l < element->size(); ++l)
						{
							string templine = (*element)[l];

							if (boost::iequals(templine, "main_anim_event"))
							{
								templine = newAnimation[templatecode].back()->mainAnimEvent;
								elementLine.replace(elementLine.find("$$"), 2, templine);
							}
							else
							{
								bool isDone = false;

								for (auto it = addOn.begin(); it != addOn.end(); ++it)
								{
									bool isBreak = false;

									for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
									{
										if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
										{
											elementLine.replace(elementLine.find("$$"), 2, iter->second);
											isBreak = true;
											isDone = true;
											break;
										}
									}

									if (isBreak)
									{
										break;
									}
								}

								if (!isDone)
								{
									for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, "##");
												isBreak = true;
												isDone = true;
												break;
											}

											groupAddOnElement.push_back(iter->second);
										}

										if (isBreak)
										{
											break;
										}
									}

								}
							}
						}

						if (groupAddOnElement.size() != 0)
						{
							vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

							for (unsigned int l = 0; l < animVar.size(); ++l)
							{
								newAnimEvent[templatecode].insert(animVar[l]);
							}
						}
						else
						{
							newAnimEvent[templatecode].insert(elementLine);
						}
					}
				}

				for (unsigned int k = 0; k < tempVariableID.size(); k++)
				{
					string name = tempVariableID[k];
					AnimVar[name] = animationList[i]->AnimVar[name];
				}

				if (error)
				{
					cout << "Fail to register new animation" << endl << endl;
					error = true;
					return;
				}
			}

			newAnimation[templatecode].shrink_to_fit();

			if (!isGroup)
			{
				animInfoGroup.shrink_to_fit();

				for (size_t j = 0; j < newAnimation[templatecode].size(); ++j)
				{
					newAnimation[templatecode][j]->addGroupAnimInfo(animInfoGroup);
				}
			}
		}
	}

	if (error)
	{
		return;
	}

	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time 1: " << duration / 1000 << " seconds" << endl;

	vecstr filelist;
	read_directory(directory, filelist);

	for (unsigned int i = 0; i < filelist.size(); i++)
	{
		BehaviorCompilation(directory, filelist, i, behaviorPriority, chosenBehavior, BehaviorTemplate, animationList, newAnimation, newAnimEvent, newAnimVariable, AnimVar);
	}
}
