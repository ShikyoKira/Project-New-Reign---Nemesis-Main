#include "import.h"

using namespace std;

vecstr importOutput(vector<ImportContainer> ExportID, int counter, int nextID, string file)
{
	vecstr behaviorlines;
	ImportContainer newExportID;
	vector<ImportContainer> exportID = ExportID;
	int lastID = nextID;

	for (auto it = exportID[counter].begin(); it != exportID[counter].end(); ++it)
	{
		string filename = file.substr(0, file.find_last_of("."));

		if (isFileExist("behavior templates/" + filename + "/" + it->first + ".nmd"))
		{
			filename = "behavior templates/" + filename + "/" + it->first + ".nmd";
		}
		else if (isFileExist("behavior templates/" + filename + "/" + it->first + ".txt"))
		{
			filename = "behavior templates/" + filename + "/" + it->first + ".txt";
		}
		else
		{
			cout << "ERROR(1027): Missing import file. Please contact the template creator" << endl << "File : " << filename << endl << endl;
			error = true;
			return behaviorlines;
		}

		vecstr exportFormat = GetFunctionLines(filename);

		if (error)
		{
			return behaviorlines;
		}

		behaviorlines.reserve(exportFormat.size());

		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
		{
			if (iter->second.length() > 0)
			{
				SSMap IDExist;
				behaviorlines.reserve(behaviorlines.size() + exportFormat.size() + 1);

				for (unsigned int j = 0; j < exportFormat.size(); j++)
				{
					string line = exportFormat[j];

					if (line.find("$import[1][2]$", 0) != string::npos)
					{
						int reference = sameWordCount(line, "$import[1][2]$");

						for (int k = 0; k < reference; k++)
						{
							line.replace(line.find("$import[1][2]$"), 14, iter->second);
						}
					}

					if (line.find("$import[1]$", 0) != string::npos)
					{
						int reference = sameWordCount(line, "$import[1]$");

						for (int k = 0; k < reference; k++)
						{
							line.replace(line.find("$import[1]$"), 11, it->first);
						}
					}

					if (line.find("$import[", 0) != string::npos && line.find("]$", line.find("$import[" + 1)) != string::npos)
					{
						int reference = sameWordCount(line, "$import[");

						for (int k = 0; k < reference; k++)
						{
							string number = boost::regex_replace(string(line.substr(line.find("$import["))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (line.find("$import[" + number + "]$", 0) != string::npos)
							{
								// check if it is number
								for (size_t p = 0; p < number.size(); ++p)
								{
									if (!isalnum(number[p]) || isalpha(number[p]))
									{
										cout << "ERROR(1154): Invalid import input. Only number is acceptable. Please contact the template creator" << endl << "Template: " << it->first << endl << "Line: " << j + 1 << endl << endl;
										error = true;
										behaviorlines.shrink_to_fit();
										return behaviorlines;
									}
								}

								stringstream sstream(iter->first);
								istream_iterator<string> ssbegin(sstream);
								istream_iterator<string> ssend;
								vecstr keywords(ssbegin, ssend);
								copy(keywords.begin(), keywords.end(), keywords.begin());

								line.replace(line.find("$import[" + number + "]$"), 10 + number.length(), keywords[stoi(number) - 2]);
							}
						}
					}

					if (line.find("$import[", 0) != string::npos && line.find("]$", line.find("$import[" + 1)) != string::npos)
					{
						size_t nextpos = line.find("$import[");
						string importer = line.substr(nextpos + 1, line.find("]$") - nextpos);
						size_t bracketCount = count(importer.begin(), importer.end(), '[');
						size_t altBracketCount = count(importer.begin(), importer.end(), ']');

						if (IDExist[importer].length() == 0)
						{
							if (bracketCount == 2)
							{
								if (bracketCount == altBracketCount)
								{
									size_t pos = importer.find("[") + 1;
									string file = importer.substr(pos, importer.find("]", pos) - pos);
									pos = importer.find("[", pos) + 1;
									string keyword = importer.substr(pos, importer.find("]", pos) - pos);
									string tempID;

									for (unsigned int i = 0; i < exportID.size(); ++i)
									{
										if (exportID[i][file][keyword].length() > 0)
										{
											tempID = exportID[i][file][keyword];
											break;
										}
									}

									if (tempID.length() == 0)
									{
										tempID = to_string(lastID);

										for (unsigned int i = 0; i < 4 - tempID.length(); i++)
										{
											tempID = "0" + tempID;
										}

										IDExist[importer] = tempID;
										newExportID[file][keyword] = tempID;
										exportID.push_back(newExportID);
										lastID++;
									}

									line.replace(nextpos, importer.length() + 2, tempID);
								}
							}
							else
							{
								cout << "ERROR(1139): Invalid import element count. Only 2 elements are acceptable. Please contact the template creator" << endl << "Template: " << it->first << endl << "Line: " << j + 1 << endl << "Import: " << importer << endl << endl;
								error = true;
								behaviorlines.shrink_to_fit();
								return behaviorlines;
							}
						}
						else
						{
							line.replace(nextpos, importer.length() + 2, IDExist[importer]);
						}
					}

					if (line.find("MID$", 0) != string::npos)
					{
						int reference = sameWordCount(line, "MID$");

						for (int k = 0; k < reference; k++)
						{
							string tempID = line.substr(line.find("MID$"));
							string number = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
							string oldID = "MID$" + number;

							if (line.find(oldID) != string::npos)
							{
								size_t nextpos = line.find(oldID);

								if (IDExist[oldID].length() > 0)
								{
									tempID = IDExist[oldID];
								}
								else
								{
									tempID = to_string(lastID);

									for (unsigned int i = 0; i < 4 - tempID.length(); i++)
									{
										tempID = "0" + tempID;
									}

									IDExist[oldID] = tempID;
									lastID++;
								}

								line.replace(nextpos, oldID.length(), tempID);
							}
							else
							{
								cout << "ERROR(1028): Invalid ID. Please contact the template creator" << endl << "File : " << iter->first << "Line: " << j + 1 << endl << endl;
								error = true;
								behaviorlines.shrink_to_fit();
								return behaviorlines;
							}
						}
					}

					behaviorlines.push_back(line);
				}

				if (behaviorlines.back().length() != 0)
				{
					behaviorlines.push_back("");
				}
			}
		}
	}

	if (newExportID.size() != 0)
	{
		vecstr additionlines = importOutput(exportID, int(exportID.size() - 1), lastID, file);
		behaviorlines.reserve(behaviorlines.size() + additionlines.size());
		behaviorlines.insert(behaviorlines.end(), additionlines.begin(), additionlines.end());
	}
	else
	{
		behaviorlines.shrink_to_fit();
	}

	return behaviorlines;
}
