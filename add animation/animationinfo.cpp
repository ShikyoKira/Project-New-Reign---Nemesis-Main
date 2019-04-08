#include "animationinfo.h"

#pragma warning(disable:4503)

using namespace std;

animationInfo::animationInfo(vecstr newAnimInfo, string curFilename, OptionList behaviorOption, int linecount, bool& isOExist, bool noOption)
{
	unsigned int k = 0;
	string line = newAnimInfo[1];
	animInfo = newAnimInfo;
	ignoreGroup = behaviorOption.ignoreGroup;
	groupOption = behaviorOption.groupOption;

	if (animInfo.size() < 3)
	{
		ErrorMessage(1142, curFilename, linecount);
	}

	optionPicked[behaviorOption.templatecode] = true;
	optionPicked[behaviorOption.templatecode + "_group"] = true;
	optionPicked[behaviorOption.templatecode + "_master"] = true;
	++optionPickedCount[behaviorOption.templatecode];
	++optionPickedCount[behaviorOption.templatecode + "_group"];
	++optionPickedCount[behaviorOption.templatecode + "_master"];

	addOn = behaviorOption.addOn;
	mixOptRegis = behaviorOption.mixOptRegis;
	mixOptRever = behaviorOption.mixOptRever;

	if (!noOption)
	{
		++k;
		string anim = line.substr(1, line.length() - 1);
		__int64 counter = count(anim.begin(), anim.end(), ',') + 1;
		unordered_map<string, bool> optionList = behaviorOption.storelist;
		unordered_map<string, vecstr> joint = behaviorOption.joint;
		vecstr optionOrder = behaviorOption.optionOrder;

		for (int i = 0; i < counter; ++i)
		{
			size_t pos = anim.find(",");
			string option;

			if (i != counter - 1)
			{
				option = anim.substr(0, pos);
				anim = anim.substr(pos + 1);
			}
			else
			{
				option = anim;
			}

			if (option == "o")
			{
				isOExist = true;
			}

			while (true)
			{
				if (option == "k" || option == "bsa")
				{
					if (!known)
					{
						known = true;
					}
					else
					{
						WarningMessage(1012);
					}

					break;
				}

				if (option[0] == 'D' && isOnlyNumber(option.substr(1)))
				{
					string time = boost::regex_replace(string(option), boost::regex("[^0-9]*([0-9]+(\\.([0-9]+)?)?).*"), string("\\1"));

					if ("D" + time == option)
					{
						duration = stod(time);
						hasDuration = true;
						optionPicked["D"] = true;
						optionPickedCount["D"]++;
						break;
					}
				}

				if (optionList[option] && !groupOption[option])
				{
					if (optionPicked[option])
					{
						string totalline = "";

						for (auto& curline : newAnimInfo)
						{
							totalline = totalline + curline + " ";
						}

						totalline.pop_back();
						ErrorMessage(1178, behaviorOption.templatecode, curFilename, linecount, totalline);
					}

					optionPicked[option] = true;
					optionPickedCount[option]++;
				}
				else
				{
					bool isSameOption = true;
					string header = "";

					for (unsigned int m = 0; m < optionOrder.size(); ++m)
					{
						bool loose = false;

						// check on group / addon option
						for (unsigned int j = 0; j < optionOrder[m].size(); ++j)
						{
							if (optionOrder[m][j] != option[j])
							{
								break;
							}
							else if (j == optionOrder[m].size() - 1)
							{
								if (header.length() < optionOrder[m].length())
								{
									header = optionOrder[m];
									string nonHeader = option.substr(header.length());
									string group = boost::regex_replace(string(nonHeader), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (group.length() != 0 && option == header + group && (group != nonHeader))
									{
										optionPicked[option] = true;
										optionPickedCount[option]++;
										loose = true;
									}
									else if (addOn[header].size() != 0)
									{
										if (option.length() > header.length())
										{
											// Eligibility test
											bool isPassed = true;
											string section = nonHeader;

											for (unsigned int k = 0; k < joint[header].size(); ++k)
											{
												string newSection = section.substr(section.find(joint[header][k]) + 1);

												if (newSection.length() < section.length())
												{
													section = newSection;
												}
												else
												{
													isPassed = false;
													break;
												}
											}

											if (!isPassed)
											{
												break;
											}

											isPassed = false;

											for (unsigned int k = 0; k < addOn[header].size(); ++k)
											{
												if (nonHeader.length() != 0)
												{
													string addOnName = addOn[header][k];

													if (k != addOn[header].size() - 1)
													{
														size_t pos = nonHeader.find(joint[header][k], 0) + joint[header][k].length();
														string name = nonHeader.substr(0, pos - joint[header][k].length());
														groupAdditionProcess(header, addOnName, name, groupOption, behaviorOption.modAddOn);
														isPassed = true;
														nonHeader = nonHeader.substr(pos);
													}
													else
													{
														groupAdditionProcess(header, addOnName, nonHeader, groupOption, behaviorOption.modAddOn);
														isPassed = true;
														break;
													}
												}
												else
												{
													break;
												}
											}

											if (isPassed)
											{
												optionPicked[header] = true;
												optionPickedCount[header]++;
												loose = true;
												break;
											}
										}
									}
								}
							}
						}

						if (loose)
						{
							break;
						}
						else
						{
							header = "";
						}
					}

					if (header.length() == 0)
					{
						WarningMessage(1026, curFilename, linecount, option);
					}
				}

				break;
			}
		}
	}

	mainAnimEvent = animInfo[k + 1];
	filename = animInfo[k + 2];
	eventID.push_back(mainAnimEvent);

	if (animInfo.size() > k + 3)
	{
		vecstr animobjects;
		unsigned int add = 3;

		while (k + add < animInfo.size())
		{
			animobjects.push_back(animInfo[k + add]);
			++add;

			if (add > 6)
			{
				// ErrorMessage(1143, curFilename, linecount);
			}
		}

		// Get animobjects
		animObjectCount = behaviorOption.animObjectCount;
		storeAnimObject(animobjects, curFilename, linecount);
	}

	optionPickedCount[behaviorOption.templatecode] = 1;
}

void animationInfo::addFilename(string curFilename)
{
	filename = curFilename;
}

void animationInfo::storeAnimObject(vecstr animobjects, string listFilename, int linecount)
{
	size_t position;

	for (unsigned int i = 0; i < animobjects.size(); ++i)
	{
		if (animobjects[i].find("/") == NOT_FOUND)
		{
			optionPicked["AnimObject/1"] = true;
			AnimObject[1].push_back(animobjects[i]);
			++optionPickedCount["AnimObject/1"];
		}
		else
		{
			position = animobjects[i].find("/");

			string ObjectName = animobjects[i].substr(0, animobjects[i].find("/", position));
			int temp = stoi(animobjects[i].substr(position + 1, 2));

			if (temp == 0 || temp > animObjectCount)
			{
				ErrorMessage(1144, listFilename, linecount);
			}

			string AO = "AnimObject/" + to_string(temp);
			optionPicked[AO] = true;
			AnimObject[temp].push_back(ObjectName);
			++optionPickedCount[AO];
		}
	}
}

void animationInfo::groupAdditionProcess(string header, string addOnName, string name, unordered_map<string, bool> groupOption, unordered_map<string, unordered_map<string, string>> modAddOn)
{
	string newName = name;

	if (modAddOn[header][addOnName].length() != 0)
	{
		string newAddOn = modAddOn[header][addOnName];
		newName = newAddOn.replace(newAddOn.find("$$$"), 3, newName);
	}

	if (groupOption[header])
	{
		string reheader = header;

		if (mixOptRegis[header].length() != 0)
		{
			reheader = mixOptRegis[header];
			optionPicked[header + to_string(optionPickedCount[reheader])] = true;
			optionPicked[reheader] = true;
			optionPickedCount[reheader]++;
			groupAddition[reheader][addOnName].push_back(newName);
		}

		groupAddition[header][addOnName].push_back(newName);
	}
	else
	{
		addition[header][addOnName] = newName;
	}

	if (boost::iequals(addOnName, "event"))
	{
		eventID.push_back(newName);
	}
	else if (boost::iequals(addOnName, "variable"))
	{
		variableID.push_back(newName);
	}
};