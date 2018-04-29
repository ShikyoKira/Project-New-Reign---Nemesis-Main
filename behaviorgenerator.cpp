#include "behaviorgenerator.h"

#pragma warning(disable:4503)

using namespace std;

vector<thread> hkxThread;

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

void BehaviorCompilation(string directory, vecstr filelist, int curList, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior, getTemplate BehaviorTemplate, vector<unique_ptr<registerAnimation>>& animationList, unordered_map<string, vector<shared_ptr<Furniture>>>& newAnimation, mapSetString newAnimEvent, mapSetString newAnimVariable, unordered_map<string, var> AnimVar, unordered_map<string, unordered_map<int, bool>> ignoreFunction, unordered_map<string, vector<string>> modAnimBehavior)
{
	ImportContainer exportID;

	string filepath = directory + filelist[curList];
	string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
	string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

	int lastID = 0;
	int nexteventid = 0;
	int nextvarid = 0;
	int eventelements = -1;
	int variableelements = -1;
	int attributeelements = -1;
	int characterelements = -1;
	int normalelements = -1;
	int curID = 0;

	bool eventOpen = false;
	bool varOpen = false;
	bool attriOpen = false;
	bool charOpen = false;
	bool animOpen = false;
	bool otherAnimOpen = false;
	bool norElement = false;
	bool isClip = false;
	bool behaviorRef = false;
	bool hasAA = false;

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
	unordered_map<int, unordered_map<string, vecstr>> clipAA;

	map<int, vecstr> catalystMap;

	vector<int> behaviorDataNode;

	set<string> AAGroupList;
	set<string> AAEventName;

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

	if (alternateAnim.size() != 0)
	{
		hasAA = true;
	}

	for (unsigned int l = 0; l < catalyst.size(); l++)
	{
		string line = catalyst[l];

		if (line.find("<hkobject name=\"", 0) != string::npos && line.find("signature=\"", 0) != string::npos)
		{
			string templine = line.substr(0, line.find("class"));
			string tempID = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
			curID = stoi(tempID);

			if (tempID != templine && curID >= lastID)
			{
				lastID = curID + 1;
			}

			if (line.find("class=\"hkbClipGenerator\" signature=\"", 0) != string::npos)
			{
				isClip = true;
			}
			else
			{
				isClip = false;
			}

			if (line.find("class=\"hkbBehaviorReferenceGenerator\" signature=\"", 0) != string::npos)
			{
				behaviorRef = true;
			}
			else
			{
				behaviorRef = false;
			}
		}
		else if (isClip && line.find("<hkparam name=\"animationName\">") != string::npos)
		{
			size_t pos = line.find("animationName\">") + 15;
			string animPath = line.substr(pos, line.find("</hkparam>", pos) - pos);
			boost::algorithm::to_lower(animPath);
			usedAnim[lowerBehaviorFile].insert(animPath);
			string animFile = GetFileName(animPath) + ".hkx";
			isClip = false;

			if (error)
			{
				return;
			}

			// Dynamic AA function without a template
			if (hasAA && alternateAnim.find(animFile) != alternateAnim.end())
			{
				clipAA[curID][animFile] = alternateAnim[animFile];
				AAGroupList.insert(AAGroup[animFile]);

				if (AAHasEvent.find(animFile) != AAHasEvent.end())
				{
					for (unsigned int j = 0; j < AAHasEvent[animFile].size(); ++j)
					{
						for (unsigned int k = 0; k < AAEvent[AAHasEvent[animFile][j]].size(); ++k)
						{
							AAEventName.insert(AAEvent[AAHasEvent[animFile][j]][k]);
						}
					}
				}
			}
		}
		else if (behaviorRef && line.find("<hkparam name=\"behaviorName\">") != string::npos)
		{
			size_t nextpos = line.find("behaviorName\">") + 14;
			string behaviorName = GetFileName(line.substr(nextpos, line.find("</hkparam>", nextpos) - nextpos));
			boost::algorithm::to_lower(behaviorName);
			behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
			behaviorRef = false;
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
	int firstID;

	if (!boost::iequals(behaviorFile, "animationdatasinglefile"))
	{
		firstID = stoi(boost::regex_replace(string(catalyst[1].substr(catalyst[1].find("toplevelobject="))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
	}

	curID = firstID;
	size_t elementLine = -1;
	size_t openRange = 0;
	bool isOpen = true;
	bool special = false;
	string newMod;
	string curNum;
	string ZeroEvent;
	SSMap IDExist;
	unordered_map<string, string> chosenLines;
	unordered_map<string, vector<IDCatcher>> catcher;
	
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
				else
				{
					newMod = modID;
				}

				skip = true;
			}
			else if (line.find("<!-- NEW ^", 0) != string::npos)
			{
				special = true;
			}
			else if (line.find("<!-- CLOSE -->", 0) != string::npos)
			{
				isOpen = true;
				newMod.clear();

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
						if (catalystMap[curID].size() != 0)
						{
							if (catalystMap[curID].back().empty())
							{
								if (catalystMap[curID].size() > 1 && catalystMap[curID][catalystMap[curID].size() - 2].empty())
								{
									catalystMap[curID].pop_back();
								}
							}
							else
							{
								catalystMap[curID].push_back("");
							}
						}

						catalystMap[curID].shrink_to_fit();
						curID = stoi(tempID);
						catalystMap[curID].reserve(100 * memory);
					}
					else
					{
						catalystMap[curID].shrink_to_fit();
						isNewID = true;
					}

					if (line.find("class=\"hkbBehaviorGraphStringData\" signature=\"", 0) != string::npos || line.find("class=\"hkbVariableValueSet\" signature=\"", 0) != string::npos || line.find("class=\"hkbBehaviorGraphData\" signature=\"", 0) != string::npos)
					{
						behaviorDataNode.push_back(curID);
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
				else if (line.find("<hkparam name=\"animationNames\" numelements=\"") != string::npos)
				{
					animOpen = true;
					elementCatch = true;
					string templine = line.substr(0, line.find("<hkparam name=\"", 0));
					openRange = count(templine.begin(), templine.end(), '\t');
				}
				else if (line.find("<hkparam name=\"deformableSkinNames\" numelements=\"") != string::npos || line.find("<hkparam name=\"rigidSkinNames\" numelements=\"") != string::npos || line.find("<hkparam name=\"animationFilenames\" numelements=\"") != string::npos)
				{
					otherAnimOpen = true;
					elementCatch = true;
					string templine = line.substr(0, line.find("<hkparam name=\"", 0));
					openRange = count(templine.begin(), templine.end(), '\t');
				}
				else if (line.find("<hkparam name=\"") != string::npos && line.find("numelements=\"") != string::npos && line.find("</hkparam>") == string::npos)
				{
					norElement = true;
					elementCatch = true;
					string templine = line.substr(0, line.find("<hkparam name=\"", 0));
					openRange = count(templine.begin(), templine.end(), '\t');
				}
				else if (line.find("</hkparam>") != string::npos && (norElement || eventOpen || varOpen || attriOpen || charOpen || animOpen || otherAnimOpen))
				{
					string templine = line.substr(0, line.find("</hkparam>"));
					size_t range = count(templine.begin(), templine.end(), '\t');

					if (openRange == range)
					{
						unordered_map<string, bool> isExist;

						// reset
						if (norElement)
						{
							norElement = false;
						}
						else if (eventOpen)
						{
							for (auto it = AAEventName.begin(); it != AAEventName.end(); ++it)
							{
								if (curNum == "eventNames")
								{
									if (isExist[*it])
									{
										cout << "WARNING: Duplicated event name found" << endl << "Event Name: " << *it << endl << endl;
									}
									else
									{
										if (counter == 0)
										{
											ZeroEvent = *it;
										}

										eventid[*it] = counter;
										eventName[counter] = *it;
										catalystMap[curID].push_back("				<hkcstring>" + *it + "</hkcstring>");
										isExist[*it] = true;
										counter++;
									}
								}
								else if (curNum == "eventInfos")
								{
									if (!isExist[*it])
									{
										catalystMap[curID].push_back("				<hkobject>");
										catalystMap[curID].push_back("					<hkparam name=\"flags\">0</hkparam>");
										catalystMap[curID].push_back("				</hkobject>");
										isExist[*it] = true;
										counter++;
									}
								}
								else
								{
									cout << ">> ERROR(1031): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
									error = true;
									return;
								}
							}

							vecstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

							for (unsigned int j = 0; j < codelist.size(); j++)
							{
								for (auto it = newAnimEvent[codelist[j]].begin(); it != newAnimEvent[codelist[j]].end(); ++it)
								{
									if (curNum == "eventNames")
									{
										if (isExist[*it])
										{
											cout << "WARNING: Duplicated event name found" << endl << "Event Name: " << *it << endl << endl;
										}
										else
										{
											if (counter == 0)
											{
												ZeroEvent = *it;
											}

											eventid[*it] = counter;
											eventName[counter] = *it;
											catalystMap[curID].push_back("				<hkcstring>" + *it + "</hkcstring>");
											isExist[*it] = true;
											counter++;
										}
									}
									else if (curNum == "eventInfos")
									{
										if (!isExist[*it])
										{
											catalystMap[curID].push_back("				<hkobject>");
											catalystMap[curID].push_back("					<hkparam name=\"flags\">0</hkparam>");
											catalystMap[curID].push_back("				</hkobject>");
											isExist[*it] = true;
											counter++;
										}
									}
									else
									{
										cout << ">> ERROR(1031): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
										error = true;
										return;
									}
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
							for (auto it = AAGroupList.begin(); it != AAGroupList.end(); ++it)
							{
								string variablename = "Nemesis_AA_" + *it;

								if (curNum == "variableNames")
								{
									if (isExist[variablename])
									{
										cout << "WARNING: Duplicated variable name found" << endl << "Event Name: " << variablename << endl << endl;
									}
									else
									{
										string curline = "				<hkcstring>" + variablename + "</hkcstring>";
										varName[counter] = variablename;
										variableid[variablename] = counter;
										catalystMap[curID].push_back(curline);
										isExist[variablename] = true;
										counter++;
									}
								}
								else if (curNum == "wordVariableValues")
								{
									if (!isExist[variablename])
									{
										catalystMap[curID].push_back("				<hkobject>");
										catalystMap[curID].push_back("					<hkparam name=\"value\">0</hkparam>");
										catalystMap[curID].push_back("				</hkobject>");
										isExist[variablename] = true;
										counter++;
									}
								}
								else if (curNum == "variableInfos")
								{
									if (!isExist[variablename])
									{
										catalystMap[curID].push_back("				<hkobject>");
										catalystMap[curID].push_back("					<hkparam name=\"role\">");
										catalystMap[curID].push_back("						<hkobject>");
										catalystMap[curID].push_back("							<hkparam name=\"role\">ROLE_DEFAULT</hkparam>");
										catalystMap[curID].push_back("							<hkparam name=\"flags\">0</hkparam>");
										catalystMap[curID].push_back("						</hkobject>");
										catalystMap[curID].push_back("					</hkparam>");
										catalystMap[curID].push_back("					<hkparam name=\"type\">VARIABLE_TYPE_INT32</hkparam>");
										catalystMap[curID].push_back("				</hkobject>");
										isExist[variablename] = true;
										counter++;
									}
								}
								else
								{
									cout << ">> ERROR(1032): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
									error = true;
									return;
								}
							}

							vecstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

							for (unsigned int j = 0; j < codelist.size(); j++)
							{
								for (auto it = newAnimVariable[codelist[j]].begin(); it != newAnimVariable[codelist[j]].end(); ++it)
								{
									string variablename = *it;

									if (curNum == "variableNames")
									{
										if (isExist[variablename])
										{
											cout << "WARNING: Duplicated variable name found" << endl << "Event Name: " << variablename << endl << endl;
										}
										else
										{
											string curline = "				<hkcstring>" + variablename + "</hkcstring>";
											varName[counter] = variablename;
											variableid[variablename] = counter;
											catalystMap[curID].push_back(curline);
											isExist[variablename] = true;
											counter++;
										}
									}
									else if (curNum == "wordVariableValues")
									{
										if (!isExist[variablename])
										{
											catalystMap[curID].push_back("				<hkobject>");
											catalystMap[curID].push_back("					<hkparam name=\"value\">" + AnimVar[variablename].init_value + "</hkparam>");
											catalystMap[curID].push_back("				</hkobject>");
											isExist[variablename] = true;
											counter++;
										}
									}
									else if (curNum == "variableInfos")
									{
										if (!isExist[variablename])
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
											isExist[variablename] = true;
											counter++;
										}
									}
									else
									{
										cout << ">> ERROR(1032): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
										error = true;
										return;
									}
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
						else if (animOpen)		// adding new animation to character file
						{
							for (auto it = BehaviorTemplate.grouplist.begin(); it != BehaviorTemplate.grouplist.end(); ++it)
							{
								vecstr behaviorNames = behaviorJoints[it->first];

								for (unsigned int k = 0; k < behaviorNames.size(); ++k)
								{
									if (it->second.size() > 0 && lowerBehaviorFile == behaviorNames[k])
									{
										vecstr templateGroup = it->second;

										for (unsigned int j = 0; j < templateGroup.size(); j++)
										{
											string templatecode = templateGroup[j];

											if (newAnimation[templatecode].size() != 0 && !BehaviorTemplate.optionlist[templatecode].core)
											{
												for (unsigned int k = 0; k < newAnimation[templatecode].size(); k++)
												{
													if (!newAnimation[templatecode][k]->isKnown())
													{
														string animPath = "Animation\\" + newAnimation[templatecode][k]->GetFilePath();
														catalystMap[curID].push_back("				<hkcstring>" + animPath + "</hkcstring>");
														newMod = animPath.substr(10, animPath.find("\\", 10) - 10);
														boost::algorithm::to_lower(animPath);
														string animFile = GetFileName(animPath);
														size_t matchSize = animModMatch[behaviorFile][animFile].size();

														if (matchSize == 0)
														{
															set<string> tempSetString;
															tempSetString.insert(animPath);
															animModMatch[behaviorFile][animFile].push_back(tempSetString);
															tempSetString.clear();
															tempSetString.insert(newMod);
															animModMatch[behaviorFile][animFile].push_back(tempSetString);
														}
														else if (matchSize == 2)
														{
															animModMatch[behaviorFile][animFile][0].insert(animPath);
															animModMatch[behaviorFile][animFile][1].insert(newMod);
														}
														else
														{
															cout << ">> ERROR(1067): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
															error = true;
															return;
														}

														counter++;
													}
												}
											}
										}
									}
								}
							}

							animOpen = false;
						}
						else if (otherAnimOpen)
						{
							otherAnimOpen = false;
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

				// counting for numelement
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
				else if (varOpen)
				{
					if (curNum == "variableNames" && line.find("<hkcstring>") != string::npos)
					{
						size_t nextpos = line.find("<hkcstring>") + 11;
						string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
						varName[counter] = name;
						variableid[name] = counter;
						counter++;
					}
					else if (curNum == "wordVariableValues" && line.find("<hkparam name=\"value\">") != string::npos)
					{
						counter++;
					}
					else if (curNum == "variableInfos" && line.find("<hkparam name=\"type\">") != string::npos)
					{
						counter++;
					}
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
				else if (animOpen && line.find("<hkcstring>") != string::npos)
				{
					size_t nextpos = line.find("<hkcstring>") + 11;
					string animPath = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
					boost::algorithm::to_lower(animPath);
					string animFile = GetFileName(animPath);
					registeredAnim[lowerBehaviorFile][animFile] = true;
					counter++;

					if (newMod.length() == 0)
					{
						newMod = "Skyrim";
					}

					size_t matchSize = animModMatch[behaviorFile][animFile].size();

					if (matchSize == 0)
					{
						set<string> tempSetString;
						tempSetString.insert(animPath);
						animModMatch[behaviorFile][animFile].push_back(tempSetString);
						tempSetString.clear();
						tempSetString.insert(newMod);
						animModMatch[behaviorFile][animFile].push_back(tempSetString);
					}
					else if (matchSize == 2)
					{
						animModMatch[behaviorFile][animFile][0].insert(animPath);
						animModMatch[behaviorFile][animFile][1].insert(newMod);
					}
					else
					{
						cout << ">> ERROR(1067): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
						error = true;
						return;
					}
				}
				else if (otherAnimOpen && line.find("<hkcstring>") != string::npos)
				{
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
					if (!ignoreFunction[filelist[curList]][curID])
					{
						__int64 funCounter = count(line.begin(), line.end(), '$') / 2;
						size_t curPos = 0;

						for (int i = 0; i < funCounter; ++i)
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

	if (catalystMap[curID].size() != 0)
	{
		if (catalystMap[curID].back().empty())
		{
			if (catalystMap[curID].size() > 1 && catalystMap[curID][catalystMap[curID].size() - 2].empty())
			{
				catalystMap[curID].pop_back();
			}
		}
		else
		{
			catalystMap[curID].push_back("");
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
	vector<unique_ptr<vecstr>> allEditLines;

	for (auto it = BehaviorTemplate.grouplist.begin(); it != BehaviorTemplate.grouplist.end(); ++it)
	{
		// check for having newAnimation for the file
		if (it->second.size() > 0 && boost::algorithm::to_lower_copy(filelist[curList]) == it->first + ".txt")
		{
			vecstr templateGroup = it->second;

			for (unsigned int j = 0; j < templateGroup.size(); j++)
			{
				string templateCode = templateGroup[j];
				vecstr opening;
				opening.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE START ======================== -->");
				opening.push_back("");
				allEditLines.push_back(make_unique<vecstr>(opening));
				bool hasGroup = false;
				bool hasMaster = false;
				bool ignoreGroup = false;

				if (BehaviorTemplate.behaviortemplate.count(templateCode + "_group") > 0 && BehaviorTemplate.behaviortemplate[templateCode + "_group"].count(it->first) > 0 && BehaviorTemplate.behaviortemplate[templateCode + "_group"][it->first].size() > 0)
				{
					hasGroup = true;
				}

				if (BehaviorTemplate.behaviortemplate.count(templateCode + "_master") > 0 && BehaviorTemplate.behaviortemplate[templateCode + "_master"].count(it->first) > 0 && BehaviorTemplate.behaviortemplate[templateCode + "_master"][it->first].size() > 0)
				{
					hasMaster = true;
				}
				
				if (newAnimation.count(templateCode) != 0 && newAnimation[templateCode].size() != 0)
				{
					vector<SSMap> subFunctionIDs;
					subFunctionIDs.reserve(memory);
					vector<vector<SSMap>> groupFunctionIDs;
					vector<SSMap> newSubFunctionIDs;
					vector<int> stateID = { 0 };
					vector<vector<shared_ptr<animationInfo>>> groupAnimInfo;
					int stateMultiplier = GetStateCount(BehaviorTemplate.behaviortemplate[templateCode][it->first]);

					if (!hasGroup)
					{
						stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][it->first], catalystMap);

						if (newAnimation[templateCode][0]->GetGroupAnimInfo()[0]->ignoreGroup)
						{
							ignoreGroup = true;
							newSubFunctionIDs.reserve(memory);
						}
					}
					else
					{
						groupFunctionIDs.reserve(memory);
					}

					if (error)
					{
						return;
					}

					// individual animation
					for (unsigned int k = 0; k < newAnimation[templateCode].size(); k++)
					{
						// getlines from newAnination
						shared_ptr<Furniture> dummyAnimation = newAnimation[templateCode][k];
						dummyAnimation->setZeroEvent(ZeroEvent);
						unique_ptr<vecstr> tempoLines = make_unique<vecstr>(dummyAnimation->GetFurnitureLine(lowerBehaviorFile, lastID, exportID, eventid, variableid, stateID, stateMultiplier, hasGroup, BehaviorTemplate.optionlist[templateCode].core));

						if (error)
						{
							return;
						}

						if (tempoLines->size() != 0)
						{
							if (tempoLines->back().empty())
							{
								if (tempoLines->size() > 1 && (*tempoLines)[tempoLines->size() - 2].empty())
								{
									tempoLines->pop_back();
								}
							}
							else
							{
								tempoLines->push_back("");
							}
						}

						subFunctionIDs.push_back(dummyAnimation->GetNewIDs());
						allEditLines.emplace_back(move(tempoLines));

						if (dummyAnimation->isLast())
						{
							if (hasGroup)
							{
								subFunctionIDs.shrink_to_fit();
								groupFunctionIDs.push_back(subFunctionIDs);
								subFunctionIDs.clear();
								subFunctionIDs.reserve(memory);
								stateID = { 0 };
							}
							else if (ignoreGroup)
							{
								newSubFunctionIDs.push_back(subFunctionIDs[0]);
								subFunctionIDs.clear();
							}

							groupAnimInfo.push_back(dummyAnimation->GetGroupAnimInfo());
						}
					}

					if (ignoreGroup)
					{
						subFunctionIDs = newSubFunctionIDs;
					}
					else if (!hasGroup)
					{
						vector<shared_ptr<animationInfo>> subGroupAnimInfo;
						vector<vector<shared_ptr<animationInfo>>> newGroupAnimInfo;

						for (unsigned int l = 0; l < groupAnimInfo.size(); ++l)
						{
							subGroupAnimInfo.push_back(groupAnimInfo[l][0]);
						}

						newGroupAnimInfo.push_back(subGroupAnimInfo);
						groupAnimInfo = newGroupAnimInfo;
					}

					subFunctionIDs.shrink_to_fit();

					time2 = boost::posix_time::microsec_clock::local_time();
					diff = time2 - time1;

					duration = double(diff.total_milliseconds());

					cout << "Processing time 4.1: " << duration / 1000 << " seconds" << endl;

					// group animation
					if (hasGroup)
					{
						if (!hasMaster)
						{
							stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][it->first], catalystMap);

							if (error)
							{
								return;
							}
						}
						else
						{
							stateID = { 0 };
						}

						for (unsigned int k = 0; k < groupFunctionIDs.size(); k++)
						{
							string filename = templateCode + "_group";
							groupTemplate group(BehaviorTemplate.behaviortemplate[filename][it->first]);
							group.setZeroEvent(ZeroEvent);
							unique_ptr<vecstr> groupBehaviorTemplate = make_unique<vecstr>(group.getFunctionLines(lowerBehaviorFile, filename, stateID, groupFunctionIDs[k], groupAnimInfo[k], lastID, exportID, eventid, variableid, templateCode, k + 1));

							if (error)
							{
								return;
							}

							if (groupBehaviorTemplate->size() != 0)
							{
								if (groupBehaviorTemplate->back().empty())
								{
									if (groupBehaviorTemplate->size() > 1 && (*groupBehaviorTemplate)[groupBehaviorTemplate->size() - 2].empty())
									{
										groupBehaviorTemplate->pop_back();
									}
								}
								else
								{
									groupBehaviorTemplate->push_back("");
								}
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
						stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][it->first], catalystMap);

						if (error)
						{
							return;
						}

						groupTemplate master(BehaviorTemplate.behaviortemplate[filename][it->first]);
						master.setZeroEvent(ZeroEvent);
						unique_ptr<vecstr> masterBehaviorTemplate = make_unique<vecstr>(master.getFunctionLines(lowerBehaviorFile, filename, stateID, subFunctionIDs, groupAnimInfo[0], lastID, exportID, eventid, variableid, templateCode, -1));

						if (error)
						{
							return;
						}

						if(masterBehaviorTemplate->size() != 0)
						{
							if (masterBehaviorTemplate->back().empty())
							{
								if (masterBehaviorTemplate->size() > 1 && (*masterBehaviorTemplate)[masterBehaviorTemplate->size() - 2].empty())
								{
									masterBehaviorTemplate->pop_back();
								}
							}
							else
							{
								masterBehaviorTemplate->push_back("");
							}
						}

						allEditLines.emplace_back(move(masterBehaviorTemplate));
						subFunctionIDs.clear();
						subFunctionIDs.push_back(master.getFunctionIDs());
					}

					// existing function
					for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode][it->first].size(); k++)
					{
						ExistingFunction exFunction;
						exFunction.setZeroEvent(ZeroEvent);
						int functionID = BehaviorTemplate.existingFunctionID[templateCode][it->first][k];
						catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], subFunctionIDs, groupAnimInfo[0], templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup, templateGroup);

						if (catalystMap[functionID].size() == 0 || error)
						{
							error = true;
							return;
						}

						if (catalystMap[functionID].back().empty())
						{
							if (catalystMap[functionID].size() > 1 && catalystMap[functionID][catalystMap[functionID].size() - 2].empty())
							{
								catalystMap[functionID].pop_back();
							}
						}
						else
						{
							catalystMap[functionID].push_back("");
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
				else
				{
					// existing function
					for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode][it->first].size(); k++)
					{
						vector<shared_ptr<animationInfo>> animInfo;
						animInfo.push_back(make_shared<animationInfo>());
						SSMap dummy;
						vector<SSMap> subFunctionIDs;
						subFunctionIDs.push_back(dummy);
						ExistingFunction exFunction;
						exFunction.setZeroEvent(ZeroEvent);
						int functionID = BehaviorTemplate.existingFunctionID[templateCode][it->first][k];
						catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], subFunctionIDs, animInfo, templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup, templateGroup);

						if (catalystMap[functionID].size() == 0 || error)
						{
							error = true;
							return;
						}

						if (catalystMap[functionID].back().empty())
						{
							if (catalystMap[functionID].size() > 1 && catalystMap[functionID][catalystMap[functionID].size() - 2].empty())
							{
								catalystMap[functionID].pop_back();
							}
						}
						else
						{
							catalystMap[functionID].push_back("");
						}
					}
				}

				vecstr closing;
				closing.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE END ======================== -->");
				allEditLines.push_back(make_unique<vecstr>(closing));
			}
		}
	}

	// check for error
	if (error)
	{
		return;
	}

	// AA animation installation
	vecstr AAlines;

	if (clipAA.size() != 0)
	{
		unordered_map<string, int> replacerCount;

		for (auto iter = clipAA.begin(); iter != clipAA.end(); ++iter)
		{
			vecstr msglines;

			for (auto it = iter->second.begin(); it != iter->second.end(); ++it)
			{
				vecstr children;
				string groupName = AAGroup[it->first];
				msglines.push_back("		<hkobject name=\"#" + to_string(iter->first) + "\" class=\"hkbManualSelectorGenerator\" signature=\"0xd932fab8\">");

				string importline = "variableID[Nemesis_AA_" + groupName + "]";
				variableIDReplacer(importline, "AA", variableid, 0);

				if (error)
				{
					return;
				}

				if (exportID["variable_binding"]["selectedGeneratorIndex " + importline].length() == 0)
				{
					string tempID = to_string(lastID);
					exportID["variable_binding"]["selectedGeneratorIndex " + importline] = tempID;
					importline = "			<hkparam name=\"variableBindingSet\">#" + tempID + "</hkparam>";
					lastID++;
				}
				else
				{
					importline = "			<hkparam name=\"variableBindingSet\">#" + exportID["variable_binding"]["selectedGeneratorIndex " + importline] + "</hkparam>";
				}

				msglines.push_back(importline);
				msglines.push_back("			<hkparam name=\"userData\">0</hkparam>");
				replacerCount[it->first]++;
				msglines.push_back("			<hkparam name=\"name\">Nemesis_" + to_string(replacerCount[it->first]) + "_" + groupName + "_MSG</hkparam>");
				msglines.push_back("			<hkparam name=\"generators\" numelements=\"" + to_string(it->second.size() + 1) + "\">");

				string baseID = to_string(lastID);
				msglines.push_back("				#" + baseID);
				lastID++;

				for (unsigned int i = 0; i < it->second.size(); ++i)
				{
					if (it->second[i] != "x")
					{
						string tempID = to_string(lastID);
						msglines.push_back("				#" + tempID);
						children.push_back(tempID);
						lastID++;
					}
					else
					{
						msglines.push_back("				#" + baseID);
					}
				}

				msglines.push_back("			</hkparam>");
				msglines.push_back("			<hkparam name=\"selectedGeneratorIndex\">0</hkparam>");
				msglines.push_back("			<hkparam name=\"currentGeneratorIndex\">0</hkparam>");
				msglines.push_back("		</hkobject>");
				msglines.push_back("");
				
				AAlines.push_back("		<hkobject name=\"#" + baseID + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");

				unordered_map<string, vecstr> triggerID;
				int originTrigger = -1;

				for (unsigned int i = 1; i < catalystMap[iter->first].size(); ++i)
				{
					AAlines.push_back(catalystMap[iter->first][i]);

					if (catalystMap[iter->first][i].find("<hkparam name=\"triggers\">") != string::npos)
					{
						if (originTrigger == -1)
						{
							size_t pos = catalystMap[iter->first][i].find("<hkparam name=\"triggers\">") + 25;
							string templine = catalystMap[iter->first][i].substr(pos, catalystMap[iter->first][i].find("</hkparam>", pos) - pos);

							if (templine != "null")
							{
								originTrigger = stoi(boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
							}
						}
						else
						{
							cout << ">> ERROR(3003): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
							error = true;
							return;
						}
					}
				}

				if (AAlines.back().length() != 0)
				{
					AAlines.push_back("");
				}

				int num = 0;

				for (unsigned int i = 0; i < children.size(); ++i)
				{
					AAlines.push_back("		<hkobject name=\"#" + children[i] + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");
					AAlines.push_back(catalystMap[iter->first][1]);
					AAlines.push_back(catalystMap[iter->first][2]);
					AAlines.push_back(catalystMap[iter->first][3]);

					while (it->second[num] == "x")
					{
						++num;
					}

					AAlines.push_back("			<hkparam name=\"animationName\">Animation\\" + it->second[num] + "</hkparam>");

					if (AAEvent.count(it->second[num]) > 0)
					{
						string tempID = to_string(lastID);
						AAlines.push_back("			<hkparam name=\"triggers\">#" + tempID + "</hkparam>");
						triggerID[tempID] = AAEvent[it->second[i - 2]];
						lastID++;
					}
					else
					{
						AAlines.push_back(catalystMap[iter->first][5]);
					}

					for (unsigned int j = 6; j < catalystMap[iter->first].size(); ++j)
					{
						AAlines.push_back(catalystMap[iter->first][j]);
					}

					if (AAlines.back().length() != 0)
					{
						AAlines.push_back("");
					}

					++num;
				}

				for (auto itera = triggerID.begin(); itera != triggerID.end(); ++itera)
				{
					AAlines.push_back("		<hkobject name=\"#" + itera->first + "\" class=\"hkbClipTriggerArray\" signature=\"0x59c23a0f\">");

					if (originTrigger != -1)
					{
						string templine = catalystMap[originTrigger][1];
						string elements = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

						if (elements != templine)
						{
							int curElements = stoi(elements);
							AAlines.push_back("			<hkparam name=\"triggers\" numelements=\"" + to_string((itera->second.size() / 2) + curElements) + "\">");
						}
					}
					else
					{
						AAlines.push_back("			<hkparam name=\"triggers\" numelements=\"" + to_string(itera->second.size() / 2) + "\">");
					}

					for (unsigned int i = 0; i < itera->second.size(); ++i)
					{
						AAlines.push_back("				<hkobject>");
						AAlines.push_back("					<hkparam name=\"localTime\">" + itera->second[i + 1] +"</hkparam>");
						AAlines.push_back("					<hkparam name=\"event\">");
						AAlines.push_back("						<hkobject>");

						string eventID = "eventID[" + itera->second[i] + "]";
						eventIDReplacer(eventID, "AA", eventid, ZeroEvent, 0);

						if (error)
						{
							return;
						}

						AAlines.push_back("							<hkparam name=\"id\">" + eventID + "</hkparam>");
						AAlines.push_back("							<hkparam name=\"payload\">null</hkparam>");
						AAlines.push_back("						</hkobject>");
						AAlines.push_back("					</hkparam>");
						AAlines.push_back("					<hkparam name=\"relativeToEndOfClip\">false</hkparam>");
						AAlines.push_back("					<hkparam name=\"acyclic\">false</hkparam>");
						AAlines.push_back("					<hkparam name=\"isAnnotation\">false</hkparam>");
						AAlines.push_back("				</hkobject>");
						AAlines.push_back("			</hkparam>");
						AAlines.push_back("		</hkobject>");
						AAlines.push_back("");

						++i;
					}
				}
			}

			catalystMap[iter->first] = msglines;
		}
	}

	time2 = boost::posix_time::microsec_clock::local_time();
	diff = time2 - time1;

	duration = double(diff.total_milliseconds());

	cout << "Processing time 5: " << duration / 1000 << " seconds" << endl;

	size_t reserveSize = 0;
	vecstr behaviorlines;

	// output import functions
	// Must be in vector
	vector<ImportContainer> groupExportID;
	groupExportID.push_back(exportID);
	vecstr additionallines = importOutput(groupExportID, int(groupExportID.size() - 1), lastID, filelist[curList]);

	for (unsigned int j = 0; j < allEditLines.size(); j++)
	{
		reserveSize += allEditLines[j]->size();
	}

	for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
	{
		reserveSize += it->second.size();
	}

	reserveSize = reserveSize + additionallines.size() + AAlines.size();
	behaviorlines.reserve(behaviorlines.size() + reserveSize + 7);

	for (int i = firstID; i < firstID + 4; ++i)
	{
		behaviorlines.insert(behaviorlines.end(), catalystMap[i].begin(), catalystMap[i].end());
	}

	behaviorlines.push_back("<!-- ======================== NEMESIS import TEMPLATE START ======================== -->");
	behaviorlines.push_back("");
	behaviorlines.insert(behaviorlines.end(), additionallines.begin(), additionallines.end());
	behaviorlines.push_back("<!-- ======================== NEMESIS import TEMPLATE END ======================== -->");

	behaviorlines.push_back("<!-- ======================== NEMESIS alternate animation TEMPLATE START ======================== -->");
	behaviorlines.push_back("");
	behaviorlines.insert(behaviorlines.end(), AAlines.begin(), AAlines.end());
	behaviorlines.push_back("<!-- ======================== NEMESIS alternate animation TEMPLATE END ======================== -->");

	for (unsigned int j = 0; j < allEditLines.size(); j++)
	{
		behaviorlines.insert(behaviorlines.end(), allEditLines[j]->begin(), allEditLines[j]->end());
	}

	firstID = firstID + 3;

	for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
	{
		if (it->first > firstID)
		{
			behaviorlines.insert(behaviorlines.end(), it->second.begin(), it->second.end());
		}
	}

	if (behaviorlines.back().length() != 0)
	{
		behaviorlines.push_back("");
	}

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



	if (behaviorPath[lowerBehaviorFile].size() == 0)
	{
		cout << "ERROR(1068): Missing \"" << behaviorFile << "\" file. Perform \"Update Patcher\" operation to fix this. If it doesn't fix it, contact mod author not Nemesis' author" << endl << "File :" << behaviorFile << endl << endl;
		error = true;
		return;
	}

	// final output
#ifndef DEBUG
	string outputdir = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
	string outputdir = behaviorPath[lowerBehaviorFile];
#endif

	string filename = "temp_behaviors\\xml\\" + lowerBehaviorFile;
	FolderCreate(filename.substr(0, filename.find_last_of(filename)));
	FolderCreate(outputdir.substr(0, outputdir.find_last_of(outputdir)));
	ofstream output(filename + ".xml");

	if (output.is_open())
	{
		FunctionWriter fwriter(&output);

		for (unsigned int i = 0; i < behaviorlines.size(); i++)
		{
			fwriter << behaviorlines[i] << "\n";
		}

		output.close();
	}
	else
	{
		cout << "ERROR(1025): Fail to output xml file" << endl << "File: " << filename << endl << endl;
		error = true;
		return;
	}

	hkxThread.emplace_back(hkxcmdOutput, filename, outputdir);
}

void GenerateBehavior(string directory, vecstr behaviorPriority, unordered_map<string, bool> chosenBehavior)
{
	{	// clear all data for consistency check
		unordered_map<string, unordered_map<string, bool>> emptyRegisteredAnim;
		unordered_map<string, unordered_map<string, vector<set<string>>>> emptyAnimModMatch;
		unordered_map<string, set<string>> emptyUsedAnim;
		unordered_map<string, vector<string>> emptyBehaviorJoints;
		unordered_map<string, string> emptyPath;

		registeredAnim = emptyRegisteredAnim;
		usedAnim = emptyUsedAnim;
		animModMatch = emptyAnimModMatch;
		behaviorJoints = emptyBehaviorJoints;
		behaviorPath = emptyPath;
	}

	GetBehaviorPath();
	GetAnimData();

	if (error)
	{
		return;
	}
	
	// register animation & organize AE n Var
	unordered_map<string, int> animationCount;												// animation type counter; use to determine how many of the that type of animation have been installed
	getTemplate BehaviorTemplate;															// get animation type
	vector<unique_ptr<registerAnimation>> animationList = move(openFile(BehaviorTemplate));	// get anim list installed by mods
	unordered_map<string, vector<shared_ptr<Furniture>>> newAnimation;						// record each of the animation created from registerAnimation
	mapSetString newAnimEvent;																// template code, 
	mapSetString newAnimVariable;
	unordered_map<string, var> AnimVar;
	unordered_map<string, vector<string>> modAnimBehavior;									// behavior directory, list of behavior files; use to get behavior reference
	unordered_map<string, unordered_map<int, bool>> ignoreFunction;							// behavior file, function ID, true/false; is the function part of animation template?

	// check for error
	if (error)
	{
		return;
	}

	// read each animation list file'
	for (unsigned int i = 0; i < animationList.size(); i++)
	{
		string modID = animationList[i]->modID;

		{
			string templatecode = "core";
			unordered_map<string, vecstr>* functionlines = &BehaviorTemplate.behaviortemplate[templatecode];
			shared_ptr<animationInfo> dummy = make_shared<animationInfo>();
			dummy->addFilename(animationList[i]->behaviorFile);
			dummy->ignoreGroup = false;
			dummy->optionPickedCount[templatecode] = 1;
			animationCount[templatecode]++;
			newAnimation[templatecode].emplace_back(make_unique<Furniture>(*functionlines, templatecode, animationCount[templatecode], "", *dummy));
			vector<shared_ptr<animationInfo>> animInfoGroup = { dummy };
			newAnimation[templatecode].back()->addGroupAnimInfo(animInfoGroup);
			newAnimation[templatecode].back()->setLastOrder(0);
		}

		// read each animation in a group of the same type
		for (auto it = animationList[i]->templateType.begin(); it != animationList[i]->templateType.end(); ++it)
		{
			string firstAE;
			string templatecode = it->first;
			vector<shared_ptr<animationInfo>> animInfoGroup;
			unordered_map<string, vecstr>* functionlines = &BehaviorTemplate.behaviortemplate[templatecode];
			newAnimation[templatecode].reserve(50 * memory);
			animationCount[templatecode]++;
			bool isGroup;
			bool ignoreGroup = BehaviorTemplate.optionlist[templatecode].ignoreGroup;

			if (BehaviorTemplate.behaviortemplate.count(templatecode + "_group") != 0 && BehaviorTemplate.behaviortemplate[templatecode + "_group"].size() != 0)
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
				newAnimation[templatecode].emplace_back(make_unique<Furniture>(*functionlines, templatecode, animationCount[templatecode], modID + "\\", *animationList[i]->animInfo[templatecode][j]));
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

							if (groupAddOnElement.size() != 0)
							{
								vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < animVar.size(); ++l)
								{
									newAnimVariable[templatecode].insert(animVar[l]);
								}
							}
							else
							{
								newAnimVariable[templatecode].insert(elementLine);
							}
						}
					}
				}

				newAnimation[templatecode].back()->setLastOrder(lastOrder);
				animInfoGroup.push_back(move(animationList[i]->animInfo[templatecode][j]));

				if (!ignoreGroup)
				{
					animationCount[templatecode]++;
				}

				if (lastOrder == order)
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

					if (ignoreGroup)
					{
						animationCount[templatecode]++;
					}
					else
					{
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

								if (groupAddOnElement.size() != 0)
								{
									vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

									for (unsigned int l = 0; l < animVar.size(); ++l)
									{
										newAnimVariable[templatecode].insert(animVar[l]);
									}
								}
								else
								{
									newAnimVariable[templatecode].insert(elementLine);
								}
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
								newAnimVariable[templatecode].insert(animVar[l]);
							}
						}
						else
						{
							newAnimVariable[templatecode].insert(elementLine);
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
		}
	}

	if (error)
	{
		return;
	}

	// Ignore function in master branch
	for (auto it = BehaviorTemplate.grouplist.begin(); it != BehaviorTemplate.grouplist.end(); ++it)
	{
		vecstr templateGroup = it->second;

		for (unsigned int j = 0; j < templateGroup.size(); j++)
		{
			// existing function
			for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateGroup[j]][it->first].size(); k++)
			{
				ignoreFunction[it->first + ".txt"][BehaviorTemplate.existingFunctionID[templateGroup[j]][it->first][k]] = true;
			}
		}
	}

	boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = time2 - time1;

	double duration = double(diff.total_milliseconds());

	cout << "Processing time 1: " << duration / 1000 << " seconds" << endl;

	vector<thread> behaviorGenerate;
	set<int> characterFiles;
	vecstr filelist;
	int animData = -1;
	read_directory(directory, filelist);

	for (unsigned int i = 0; i < filelist.size(); i++)
	{
		if (!boost::filesystem::is_directory(directory + filelist[i]))
		{
			bool skip = false;
			string tempfilename = filelist[i].substr(0, filelist[i].find_last_of("."));
			boost::algorithm::to_lower(tempfilename);
			string temppath = behaviorPath[tempfilename];

			if (temppath.length() != 0)
			{
				size_t nextpos = 0;
				size_t lastpos = temppath.find_last_of("\\");

				while (temppath.find("\\", nextpos) != lastpos)
				{
					nextpos = temppath.find("\\", nextpos) + 1;
				}

				temppath = temppath.substr(nextpos, lastpos - nextpos);
			}

			if (boost::iequals(filelist[i], "animationdatasinglefile.txt"))
			{
				animData = i;
			}
			else if (temppath.find("characters") != 0)
			{
				behaviorGenerate.emplace_back(BehaviorCompilation, directory, filelist, i, behaviorPriority, chosenBehavior, BehaviorTemplate, ref(animationList), ref(newAnimation), newAnimEvent, newAnimVariable, AnimVar, ignoreFunction, modAnimBehavior);
			}
			else
			{
				characterFiles.insert(i);
				characterHKX(directory, filelist[i]);
			}
		}
	}

	for (auto& endt : behaviorGenerate)
	{
		endt.join();
	}

	if (error)
	{
		return;
	}

	if (animData == -1)
	{
		cout << "ERROR(3004): Missing \"animationdatasinglefile.txt\" file in \"temp_behaviors\" folder. Please reinstall Nemesis and/or run update patcher" << endl << endl;
		error = true;
		return;
	}

	thread animDataThread (AnimDataCompilation, directory, filelist, animData, behaviorPriority, chosenBehavior, BehaviorTemplate, ref(animationList), ref(newAnimation), AnimVar, modAnimBehavior);

	unordered_map<string, vector<string>> tempBehaviorJoints;

	for (auto it = behaviorJoints.begin(); it != behaviorJoints.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			vecstr temp = it->second;

			while (!behaviorJoints[temp[i]].empty())
			{
				temp = behaviorJoints[temp[i]];
			}

			tempBehaviorJoints[it->first] = temp;
			break;
		}
	}

	behaviorJoints = tempBehaviorJoints;
	vector<thread> characterGenerate;

	if (characterFiles.size() != 0)
	{
		for (auto it = characterFiles.begin(); it != characterFiles.end(); ++it)
		{
			characterGenerate.emplace_back(BehaviorCompilation, directory, filelist, *it, behaviorPriority, chosenBehavior, BehaviorTemplate, ref(animationList), ref(newAnimation), newAnimEvent, newAnimVariable, AnimVar, ignoreFunction, modAnimBehavior);			
		}

	}

	if (AAGroup.size() > 0)
	{
		AAInstallation();		// install AA script
	}
	
	for (auto& endt : characterGenerate)
	{
		endt.join();
	}

	for (auto& endt : hkxThread)
	{
		endt.join();
	}

	animDataThread.join();

	if (error)
	{
		return;
	}
}

void hkxcmdOutput(string xmlfile, string hkxfile)
{
	string args = "convert -v:WIN32 \"" + xmlfile + ".xml\" \"" + hkxfile + ".hkx\"";

	if (boost::process::system("hkxcmd " + args) != 0)
	{
		cout << "ERROR(1095): Fail to output hkx file" << endl << "File: " << xmlfile << ".xml" << endl << endl;
		error = true;
		return;
	}
	else
	{
		if (!isFileExist(hkxfile + ".hkx"))
		{
			cout << "ERROR(1095): Fail to output hkx file" << endl << "File: " << xmlfile << ".xml" << endl << endl;
			error = true;
			return;
		}
		else if (remove((xmlfile + ".xml").c_str()) != 0)
		{
			cout << "WARNING: Fail to remove xml file" << endl << "File: " << xmlfile << ".xml" << endl << endl;
		}
	}
}
