#include "animationinfo.h"

#pragma warning(disable:4503)

using namespace std;

animationInfo::animationInfo(vecstr newAnimInfo, string curFilename, OptionList behaviorOption, int linecount, bool& isOExist, bool noOption)
{
	unsigned int k = 0;
	bool known = false;
	string line = newAnimInfo[1];
	animInfo = newAnimInfo;

	if (animInfo.size() < 3)
	{
		cout << "ERROR(1142): Missing information" << endl << "File: " << curFilename << endl << "Line: " << linecount << endl << endl;
		error = true;
		return;
	}

	if (!noOption)
	{
		k++;
		string anim = line.substr(1, line.length() - 1);
		__int64 counter = count(anim.begin(), anim.end(), ',') + 1;
		unordered_map<string, bool> optionList = behaviorOption.storelist;
		unordered_map<string, bool> optionGroup = behaviorOption.groupOption;
		unordered_map<string, vecstr> joint = behaviorOption.joint;
		vecstr optionOrder = behaviorOption.optionOrder;

		addOn = behaviorOption.addOn;
		mixOptRegis = behaviorOption.mixOptRegis;
		mixOptRever = behaviorOption.mixOptRever;
		
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
					known = true;
					break;
				}

				if (option[0] == 'D' && isalnum(option[1]) && !isalpha(option[1]))
				{
					string time = boost::regex_replace(string(option), boost::regex("[^0-9]*([0-9]+(\\.([0-9]+)?)?).*"), string("\\1"));
					time = time.substr(1);

					if ("D" + time == option)
					{
						duration = stod(time);
						hasDuration = true;
						optionPicked["D"] = true;
						optionPickedCount["D"]++;
						break;
					}
				}

				if (optionList[option] && !optionGroup[option])
				{
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
						for (unsigned int j = 0; j < optionOrder[m].size(); j++)
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

											for (unsigned int k = 0; k < joint[header].size(); k++)
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

											for (unsigned int k = 0; k < addOn[header].size(); k++)
											{
												if (nonHeader.length() != 0)
												{
													string addOnName = addOn[header][k];

													if (k != addOn[header].size() - 1)
													{
														size_t pos = nonHeader.find(joint[header][k], 0) + joint[header][k].length();
														string name = nonHeader.substr(0, pos - joint[header][k].length());
														groupAdditionProcess(header, addOnName, name, optionGroup);
														isPassed = true;
														nonHeader = nonHeader.substr(pos);
													}
													else
													{
														groupAdditionProcess(header, addOnName, nonHeader, optionGroup);
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
						cout << "ERROR(1133): Invalid option" << endl << "File: " << curFilename << endl << "Line: " << linecount << endl << "Option: " << option << endl << endl;
						error = true;
						return;
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
		animobjects.push_back(animInfo[k + 3]);

		if (animInfo.size() > k + 4)
		{
			animobjects.push_back(animInfo[k + 4]);

			if (animInfo.size() > k + 5)
			{
				cout << "ERROR(1143): Input overload" << endl << "File: " << curFilename << endl << "Line: " << linecount << endl;
				error = true;
				return;
			}
		}

		// Get animobjects
		storeAnimObject(animobjects, curFilename, linecount);
		optionPickedCount["AnimObject"] = int(animobjects.size());
	}

	optionPickedCount[behaviorOption.templatecode] = 1;
}

void animationInfo::addFilename(std::string curFilename)
{
	filename = curFilename;
}

void animationInfo::storeAnimObject(vecstr animobjects, string listFilename, int lineCount)
{
	size_t position;

	for (unsigned int i = 0; i < animobjects.size(); i++)
	{
		position = animobjects[i].find("/");

		string ObjectName = animobjects[i].substr(0, animobjects[i].find("/", position));
		int temp = stoi(animobjects[i].substr(position + 1, 2));

		if (temp != 1 && temp != 2)
		{
			cout << "ERROR(1144): Invalid AnimObject" << endl << "File: " << listFilename << endl << "Line: " << lineCount << endl;
			error = true;
			return;
		}

		optionPicked["AnimObject/" + to_string(temp)] = true;
		AnimObject[temp] = ObjectName;
	}
}

void animationInfo::groupAdditionProcess(string header, string addOnName, string name, unordered_map<string, bool> optionGroup)
{
	if (optionGroup[header])
	{
		string reheader = header;

		if (mixOptRegis[header].length() != 0)
		{
			reheader = mixOptRegis[header];
			optionPicked[header + to_string(optionPickedCount[reheader])] = true;
			optionPicked[reheader] = true;
			optionPickedCount[reheader]++;
			groupAddition[reheader][addOnName].push_back(name);
		}

		groupAddition[header][addOnName].push_back(name);
	}
	else
	{
		addition[header][addOnName] = name;
	}

	if (boost::iequals(addOnName, "event"))
	{
		eventID.push_back(name);
	}
	else if (boost::iequals(addOnName, "variable"))
	{
		variableID.push_back(name);
	}
};