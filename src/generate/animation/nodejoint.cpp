#include <boost/regex.hpp>

#include "utilities/compute.h"
#include "utilities/algorithm.h"
#include "utilities/stringsplit.h"

#include "generate/AnimationUtility.h"

#include "generate/animation/nodejoint.h"
#include "generate/animation/templatetree.h"
#include "generate/animation/animationinfo.h"

using namespace std;

extern bool newCondition(string condition, string filename, vector<vector<unordered_map<string, bool>>> optionPicked, vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
	int numline, string format, string masterformat, AnimationUtility utility);
extern vecstr GetOptionInfo(string line, string format, string filename, int numline, vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo, bool allowNoFixAnim,
	bool isCondition, int groupMulti = -1, int animMulti = -1, int optionMulti = -1, string multiOption = "");
extern int formatGroupReplace(string& curline, string oriline, int point, string filename, string format, shared_ptr<master> subFunctionIDs,
	vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo, int linecount, int groupMulti, int optionMulti, int animMulti, string multiOption, bool& innerError);
extern void eventIDReplacer(string& line, string format, string filename, id eventid, string firstEvent, int linecount);
extern void variableIDReplacer(string& line, string format, string filename, id variableid, string ZeroVariable, int linecount);
extern void CRC32Replacer(string& line, string format, string behaviorFile, int linecount);
extern void multiChoice(string& line, string filename, vector<vector<unordered_map<string, bool>>> masterOptionPicked, vector<vector<shared_ptr<AnimationInfo>>> groupAnimInfo,
	int numline, string format, string masterformat, AnimationUtility utility);
extern int openEndBracket(string& line, char openBrac, char closeBrac, string format, string filename, int linecount);

void processing(string& line, shared_ptr<NodePackedParameters> parameters);
void processing(string& line, string format, string filename, string behaviorFile, int& nextFunctionID, string& strID, vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo,
	SSMap& IDExist, shared_ptr<master> subFunctionIDs, ImportContainer& import, int linecount, id eventid, id variableid, string zeroEvent, string zeroVariable,
	int groupMulti, int animMulti, int optionMulti, bool hasGroup, string multiOption, vector<vector<unordered_map<string, bool>>>& optionPicked,
	unsigned __int64& openRange, unsigned int& elementCount, string& templateLine, LineCheck* elementCatch, bool& negative);
void processing2(string& line, string format, string filename, int& nextFunctionID, string& strID, vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo, SSMap& IDExist,
	shared_ptr<master> subFunctionIDs, ImportContainer& import, int linecount, id eventid, id variableid, string zeroEvent, string zeroVariable,
	int groupMulti, int animMulti, int optionMulti, bool hasGroup, string multiOption);
void newID(int& nextFunctionID, string& strID);

LineCheck::LineCheck(string _line, unsigned int _row)
{
	line = _line;
	row = _row;
}

LineCheck::LineCheck(shared_ptr<NodeJoint> _nested, unsigned int _row)
{
	nested = _nested;
	row = _row;
}

LineCheck& LineCheck::operator=(string _line)
{
	line = _line;
	nested = nullptr;
	return *this;
}

LineCheck& LineCheck::operator=(shared_ptr<NodeJoint> _nested)
{
	line.clear();
	nested = _nested;
	return *this;
}

NodePackedParameters::NodePackedParameters(string _format, string _filename, std::string _behaviorFile, int& _nextFunctionID, string& _strID,
	vector<vector<shared_ptr<AnimationInfo>>>& _groupAnimInfo, SSMap& _IDExist, shared_ptr<master> _subFunctionIDs, ImportContainer& _import, int _linecount, id _eventid,
	id _variableid, string _zeroEvent, string _zeroVariable, int _groupMulti, int _animMulti, int _optionMulti, bool _hasGroup, string _multiOption,
	vector<vector<unordered_map<string, bool>>>& _optionPicked, unsigned __int64& _openRange, unsigned int& _elementCount, string& _line,
	LineCheck* _elementCatch, bool& _negative) : nextFunctionID(_nextFunctionID), strID(_strID), groupAnimInfo(_groupAnimInfo), IDExist(_IDExist), import(_import),
	optionPicked(_optionPicked), openRange(_openRange), elementCount(_elementCount), line(_line), elementCatch(_elementCatch), negative(_negative)
{
	format = _format;
	filename = _filename;
	behaviorFile = _behaviorFile;
	subFunctionIDs = _subFunctionIDs;
	linecount = _linecount;
	eventid = _eventid;
	variableid = _variableid;
	zeroEvent = _zeroEvent;
	zeroVariable = _zeroVariable;
	groupMulti = _groupMulti;
	animMulti = _animMulti;
	optionMulti = _optionMulti;
	hasGroup = _hasGroup;
	multiOption = _multiOption;
}

NodeJoint::NodeJoint(vecstr& node, string format, string filename, string _behaviorFile, unordered_map<string, string> otherAnimType, unsigned int startline, unsigned int size)
{
	behaviorFile = _behaviorFile;

	for (auto& templatecode : otherAnimType)
	{
		templateGroup.insert(templatecode.second);
	}

	templateGroup.insert(format);
	dataBake(node, format, filename, otherAnimType, startline, size);
}

NodeJoint::NodeJoint(vecstr& node, string format, string filename, string _behaviorFile, setstr _templateGroup, unsigned int startline, unsigned int size)
{
	behaviorFile = _behaviorFile;
	templateGroup = _templateGroup;
	unordered_map<string, string> otherAnimType;

	for (auto& templatecode : templateGroup)
	{
		if (templatecode != format)
		{
			string lowercode = nemesis::to_lower_copy(templatecode);
			otherAnimType[lowercode] = lowercode;
			otherAnimType[lowercode + "_group"] = lowercode;
			otherAnimType[lowercode + "_master"] = lowercode;
		}
	}

	dataBake(node, format, filename, otherAnimType, startline, size);
}

void NodeJoint::insertData(string format, string filename, vector<vector<unordered_map<string, bool>>>& optionPicked, vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo,
	int groupMulti, int animMulti, int optionMulti, bool hasMaster, bool hasGroup, bool ignoreGroup, string multiOption, int& nextFunctionID, string& strID, SSMap& IDExist,
	ImportContainer& import, id eventid, id variableid, string zeroEvent, string zeroVariable, unsigned __int64& openRange, unsigned int elementCount,
	LineCheck* elementCatch, shared_ptr<master> subFunctionIDs, bool& negative)
{
	vector<unsigned int>* rows_ptr;
	vector<unsigned int> rows;

	if (templateSection.size() > 0)
	{
		rows_ptr = &templateSection[format];
	}
	else
	{
		rows_ptr = &rows;

		for (unsigned int sect = 0; sect < storeTemplate.size(); ++sect)
		{
			rows_ptr->push_back(sect);
		}
	}

	for (unsigned int sect : *rows_ptr)
	{
		if (storeTemplate[sect].nested)
		{
			if (!subFunctionIDs) continue;

			AnimationUtility utility;
			string condition = output[sect].back().nested->condt;

			switch (storeTemplate[sect].nested->type)
			{
				case NEW:
					if (condt.find("[") == NOT_FOUND)
					{
						if (hasMaster)
						{
							if (format + "_master" == condition)
							{
								output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster, hasGroup,
									ignoreGroup, condition, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount,
									elementCatch, subFunctionIDs, negative);
								output[sect].back().deleted = false;
							}
						}
						else if (hasGroup)
						{
							if (format + "_group" == condition)
							{
								output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster, hasGroup,
									ignoreGroup, condition, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount,
									elementCatch, subFunctionIDs, negative);
								output[sect].back().deleted = false;
							}
						}
						else if (format == condition)
						{
							output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster, hasGroup,
								ignoreGroup, condition, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount,
								elementCatch, subFunctionIDs, negative);
							output[sect].back().deleted = false;
						}
					}
					else
					{
						bool isNot = false;

						if (condition[0] == '!')
						{
							isNot = true;
							condition = condition.substr(1);
						}

						if (condition.find(format + "[") != NOT_FOUND && condition.find("]") != NOT_FOUND)
						{
							int pos = 0;
							vecstr formatInfo = GetOptionInfo(condition, format, filename, storeTemplate[sect].nested->row, groupAnimInfo, false, true, groupMulti, animMulti,
								optionMulti, multiOption);

							if (formatInfo[2].find("AnimObject") != NOT_FOUND) ErrorMessage(1129, format, filename, storeTemplate[sect].nested->row);

							if (optionPicked[stoi(formatInfo[1])][stoi(formatInfo[2])][formatInfo[3]])
							{
								if (!isNot)
								{
									output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster,
										hasGroup, ignoreGroup, formatInfo[3], nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange,
										elementCount, elementCatch, subFunctionIDs, negative);
									output[sect].back().deleted = false;
								}
							}
							else
							{
								if (isNot)
								{
									output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster,
										hasGroup, ignoreGroup, output[sect].back().nested->condt, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent,
										zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative);
									output[sect].back().deleted = false;
								}
							}
						}
					}
					break;
				case FOREACH:
					forEachProcess(output, storeTemplate[sect], condition, sect, format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti,
						optionMulti, hasMaster, hasGroup, ignoreGroup, multiOption, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable,
						openRange, elementCount, elementCatch, subFunctionIDs, negative);
					break;
				case CONDITION_START:
					utility.originalCondition = condition;
					utility.nodeProcess = make_shared<NodePackedParameters>(format, filename, behaviorFile, nextFunctionID, strID, groupAnimInfo, IDExist, subFunctionIDs, import,
						output[sect].back().row, eventid, variableid, zeroEvent, zeroVariable, groupMulti, animMulti, optionMulti, hasGroup, multiOption, optionPicked,
						openRange, elementCount, storeTemplate[sect].nested->condt, elementCatch, negative);
					utility.isExisting = true;
					utility.hasGroup = groupMulti == -1;
					utility.groupMulti = groupMulti;
					utility.animMulti = animMulti;

					if (newCondition(output[sect].back().nested->condt, filename, optionPicked, groupAnimInfo, output[sect].back().row, format, format, utility))
					{
						output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster, hasGroup,
							ignoreGroup, multiOption, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount,
							elementCatch, subFunctionIDs, negative);
						output[sect].back().deleted = false;
						++sect;

						while (storeTemplate[sect].nested && (storeTemplate[sect].nested->type == CONDITION_ELSE || storeTemplate[sect].nested->type == CONDITION))
						{
							output[sect].back().nested->conditionSkip = true;
							sect++;
						}
					}
					break;
				case CONDITION_ELSE:
					if (!output[sect].back().nested->conditionSkip)
					{
						utility.originalCondition = condition;
						utility.nodeProcess = make_shared<NodePackedParameters>(format, filename, behaviorFile, nextFunctionID, strID, groupAnimInfo, IDExist, subFunctionIDs,
							import, output[sect].back().row, eventid, variableid, zeroEvent, zeroVariable, groupMulti, animMulti, optionMulti, hasGroup, multiOption, optionPicked,
							openRange, elementCount, storeTemplate[sect].nested->condt, elementCatch, negative);
						utility.isExisting = true;
						utility.hasGroup = groupMulti == -1;
						utility.groupMulti = groupMulti;
						utility.animMulti = animMulti;

						if (newCondition(output[sect].back().nested->condt, filename, optionPicked, groupAnimInfo, output[sect].back().row, format, format, utility))
						{
							output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster, hasGroup,
								ignoreGroup, multiOption, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount,
								elementCatch, subFunctionIDs, negative);
							output[sect].back().deleted = false;
							++sect;

							while (storeTemplate[sect].nested && (storeTemplate[sect].nested->type == CONDITION_ELSE || storeTemplate[sect].nested->type == CONDITION))
							{
								output[sect].back().nested->conditionSkip = true;
								sect++;
							}
						}
					}
					break;
				case CONDITION:
					if (!output[sect].back().nested->conditionSkip)
					{
						output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, hasMaster, hasGroup,
							ignoreGroup, multiOption, nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount,
							elementCatch, subFunctionIDs, negative);
						output[sect].back().deleted = false;
					}
					break;
				default:
					break;
			}
		}
		else
		{
			if (storeTemplate[sect].process)
			{
				processing(output[sect].back().line, format, filename, behaviorFile, nextFunctionID, strID, groupAnimInfo, IDExist, subFunctionIDs, import, output[sect].back().row,
					eventid, variableid, zeroEvent, zeroVariable, groupMulti, animMulti, optionMulti, hasGroup, multiOption, optionPicked, openRange, elementCount,
					storeTemplate[sect].line, elementCatch, negative);
			}

			string& line = output[sect].back().line;

			if (line.find("%") != NOT_FOUND)
			{
				__int64 counter = count(line.begin(), line.end(), '%');
				string num;

				if (optionMulti != -1) num = to_string(optionMulti);
				else if (animMulti != -1) num = to_string(animMulti);
				else if (groupMulti != -1) num = to_string(groupMulti);
				else num = "0";

				for (int p = 0; p < counter; ++p)
				{
					line.replace(line.find("%"), 1, num);
				}
			}

			size_t pos = line.find("(");

			if (pos != NOT_FOUND && line.find(")", pos) != NOT_FOUND)
			{
				string templine = line.substr(pos);
				int nextpos = openEndBracket(templine, '(', ')', format, filename, output[sect].back().row) + 1;
				templine = templine.substr(0, nextpos);
				string oldline = templine;
				nemesis::calculate(templine, format, filename, output[sect].back().row);

				if (oldline != templine) line.replace(pos, oldline.length(), templine);
			}

			if (line.find("<hkparam name=\"animationName\">") != NOT_FOUND)
			{
				pos = line.find("animationName\">") + 15;
				string animPath = line.substr(pos, line.find("</hkparam>", pos) - pos);
				nemesis::to_lower(animPath);
				addUsedAnim(behaviorFile, animPath);
			}
			else if (line.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
			{
				negative = true;
			}
			else if (negative && line.find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
			{
				if (line.find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
				{
					size_t pos = line.find("<hkparam name=\"relativeToEndOfClip\">") + 36;
					line.replace(pos, line.find("</hkparam>", pos) - pos, "true");
				}

				negative = false;
			}
		}
	}
}

void NodeJoint::forEachProcess(vector<vector<LineCheck>>& output, LineCheck& storeTemplate, string condition, unsigned int sect, string format, string filename,
	vector<vector<unordered_map<string, bool>>>& optionPicked, vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo, int groupMulti, int animMulti, int optionMulti,
	bool hasMaster, bool hasGroup, bool ignoreGroup, string multiOption, int& nextFunctionID, string& strID, SSMap& IDExist, ImportContainer& import,
	id eventid, id variableid, string zeroEvent, string zeroVariable, unsigned __int64 openRange, unsigned int elementCount, LineCheck* elementCatch,
	shared_ptr<master> subFunctionIDs, bool& negative)
{
	if (hasMaster)
	{
		if (condition == format + "_master")
		{
			if (groupAnimInfo.size() > 0)
			{
				output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, -1, -1, -1, false, hasGroup, ignoreGroup, format + "_master", nextFunctionID, strID,
					IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative);
				output[sect].back().deleted = false;
			}
		}
	}
	else if (hasGroup)
	{
		unsigned int groupOrder;
		unsigned int groupSize;

		// <!-- FOREACH ^format_group^ -->
		if (condition == format + "_group")
		{
			groupOrder = 0;
			groupSize = groupAnimInfo.size();

			for (unsigned int multi = groupOrder; multi < groupSize; ++multi)
			{
				output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, multi, -1, -1, false, false, ignoreGroup, format + "_group", nextFunctionID,
					strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative);
				output[sect].back().deleted = false;

				if (multi + 1 != groupSize)
				{
					output[sect].push_back(storeTemplate);
					output[sect].back().nested = make_shared<NodeJoint>(*storeTemplate.nested);
				}
			}
		}
		else
		{
			if (condition.find(format + "[") == NOT_FOUND)
			{
				for (auto& each : templateGroup)
				{
					if (condition.find(each + "[") == NOT_FOUND || condition.find(each + "_group[") != NOT_FOUND || condition.find(each + "_master[") != NOT_FOUND
						|| condition == each || condition == each + "_group" || condition == each + "_master")
					{
						return;
					}
				}

				ErrorMessage(1161, format, filename, storeTemplate.nested->row);
			}

			vecstr formatInfo;

			try
			{
				formatInfo = GetOptionInfo(condition, format, filename, storeTemplate.nested->row, groupAnimInfo, true, true, groupMulti, animMulti,
					optionMulti, multiOption);
			}
			catch (...)
			{
				return;
			}

			if (formatInfo[1].length() == 0)
			{
				groupOrder = 0;
				groupSize = groupAnimInfo.size();
			}
			else
			{
				groupOrder = stoi(formatInfo[1]);
				groupSize = groupOrder + 1;
			}

			if (groupSize <= groupAnimInfo.size())
			{
				for (unsigned int multi = groupOrder; multi < groupSize; ++multi)
				{
					unsigned int order;
					unsigned int size;

					if (formatInfo[2].length() == 0)
					{
						order = 0;
						size = subFunctionIDs->grouplist[multi]->singlelist.size();
					}
					else
					{
						order = stoi(formatInfo[2]);
						size = order + 1;
					}

					if (size <= subFunctionIDs->grouplist[multi]->singlelist.size())
					{
						for (unsigned int animMulti = order; animMulti < size; ++animMulti)
						{
							optionMultiLoop(output[sect], format, filename, optionPicked, groupAnimInfo, multi, animMulti, optionMulti, ignoreGroup, formatInfo[3], nextFunctionID, strID,
								IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative, storeTemplate);
						}
					}
				}
			}
		}
	}
	// <!-- FOREACH ^format^ -->
	else if (condition == format)
	{
		if (ignoreGroup)
		{
			bool changed = false;

			for (unsigned int multi = 0; multi < groupAnimInfo.size(); ++multi)
			{
				output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, multi, 0, -1, false, false, ignoreGroup, format, nextFunctionID, strID, IDExist,
					import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative);
				output[sect].back().deleted = false;
				
				if (multi + 1 != groupAnimInfo.size())
				{
					output[sect].push_back(storeTemplate);
					output[sect].back().nested = make_shared<NodeJoint>(*storeTemplate.nested);
				}
			}
		}
		else
		{
			unsigned int multi = groupMulti == -1 ? 0 : groupMulti;

			if (multi < subFunctionIDs->grouplist.size())
			{
				unsigned int order = 0;
				unsigned int size = subFunctionIDs->grouplist[multi]->singlelist.size();

				for (unsigned int animMulti = order; animMulti < size; ++animMulti)
				{
					output[sect].back().nested->insertData(format, filename, optionPicked, groupAnimInfo, multi, animMulti, -1, false, false, ignoreGroup, format, nextFunctionID, strID,
						IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative);
					output[sect].back().deleted = false;

					if (animMulti + 1 != size)
					{
						output[sect].push_back(storeTemplate);
						output[sect].back().nested = make_shared<NodeJoint>(*storeTemplate.nested);
					}
				}
			}
		}
	}
	else
	{
		// <!-- FOREACH ^format[][T]^ -->
		if (condition.find(format + "[") != NOT_FOUND && condition.find("]") != NOT_FOUND)
		{
			vecstr formatInfo;

			try
			{
				formatInfo = GetOptionInfo(condition, format, filename, storeTemplate.nested->row, groupAnimInfo, true, true, groupMulti, animMulti,
					optionMulti, multiOption);
			}
			catch (...)
			{
				return;
			}

			optionMultiLoop(output[sect], format, filename, optionPicked, groupAnimInfo, stoi(formatInfo[1]), stoi(formatInfo[2]), optionMulti, ignoreGroup,
				formatInfo[3], nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative,
				storeTemplate);
		}
		// <!-- FOREACH ^format_group[][][T]^ -->
		else if (condition.find(format + "_group[") != NOT_FOUND && condition.find("]") != NOT_FOUND)
		{
			vecstr formatInfo;

			try
			{
				formatInfo = GetOptionInfo(condition, format, filename, storeTemplate.nested->row, groupAnimInfo, true, true, groupMulti, animMulti,
					optionMulti, multiOption);
			}
			catch (...)
			{
				return;
			}

			unsigned int groupOrder = formatInfo[1].length() == 0 ? groupMulti : stoi(formatInfo[1]);

			if (animMulti != -1)
			{
				optionMultiLoop(output[sect], format, filename, optionPicked, groupAnimInfo, groupOrder, formatInfo[2].length() == 0 ? animMulti : stoi(formatInfo[2]), optionMulti,
					ignoreGroup, formatInfo[3], nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch,
					subFunctionIDs, negative, storeTemplate);
			}
			else if (groupOrder < subFunctionIDs->grouplist.size())
			{
				unsigned int order;
				unsigned int size;

				if (formatInfo[2].length() == 0)
				{
					order = 0;
					size = subFunctionIDs->grouplist[groupOrder]->singlelist.size();
				}
				else
				{
					order = stoi(formatInfo[2]);
					size = order + 1;
				}

				if (size < subFunctionIDs->grouplist[groupOrder]->singlelist.size())
				{
					for (order = 0; order < size; ++order)
					{
						optionMultiLoop(output[sect], format, filename, optionPicked, groupAnimInfo, groupOrder, order, optionMulti, ignoreGroup, formatInfo[3], nextFunctionID, strID,
							IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative, storeTemplate);
					}
				}
			}
		}
		// <!-- FOREACH ^T^ -->
		else if (multiOption == format)
		{
			vecstr formatInfo;

			try
			{
				formatInfo = GetOptionInfo(condition, format, filename, storeTemplate.nested->row, groupAnimInfo, true, true, groupMulti, animMulti,
					optionMulti, multiOption);
			}
			catch (...)
			{
				return;
			}

			optionMultiLoop(output[sect], format, filename, optionPicked, groupAnimInfo, stoi(formatInfo[1]), stoi(formatInfo[2]), optionMulti, ignoreGroup,
				formatInfo[3], nextFunctionID, strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative,
				storeTemplate);
		}
	}
}

void NodeJoint::optionMultiLoop(vector<LineCheck>& output, string format, string filename, vector<vector<unordered_map<string, bool>>>& optionPicked,
	vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo, int groupMulti, int animMulti, int optionMulti, bool ignoreGroup, string multiOption, int& nextFunctionID,
	string& strID, SSMap& IDExist, ImportContainer& import, id eventid, id variableid, string zeroEvent, string zeroVariable, unsigned __int64 openRange, unsigned int elementCount,
	LineCheck* elementCatch, shared_ptr<master> subFunctionIDs, bool& negative, LineCheck temp)
{
	if (static_cast<unsigned int>(groupMulti) < groupAnimInfo.size() && static_cast<unsigned int>(animMulti) < groupAnimInfo[groupMulti].size())
	{
		unsigned int order = optionMulti == -1 ? 0 : optionMulti;
		unsigned int repeatCount = groupAnimInfo[groupMulti][animMulti]->optionPickedCount[multiOption];

		if (order < repeatCount)
		{
			for (unsigned int optionMulti = order; optionMulti < repeatCount; ++optionMulti)
			{
				output.back().nested->insertData(format, filename, optionPicked, groupAnimInfo, groupMulti, animMulti, optionMulti, false, false, ignoreGroup, multiOption, nextFunctionID,
					strID, IDExist, import, eventid, variableid, zeroEvent, zeroVariable, openRange, elementCount, elementCatch, subFunctionIDs, negative);
				output.back().deleted = false;

				if (optionMulti + 1 != repeatCount)
				{
					output.push_back(temp);
					output.back().nested = make_shared<NodeJoint>(*temp.nested);
				}
			}
		}
	}
}

vecstr NodeJoint::unpack()
{
	vecstr storeline;

	for (auto& group : output)
	{
		for (auto& each : group)
		{
			if (!each.deleted)
			{
				if (each.nested)
				{
					vecstr sub = each.nested->unpack();
					storeline.insert(storeline.end(), sub.begin(), sub.end());
				}
				else
				{
					storeline.push_back(each.line);
				}
			}
		}
	}

	bool elementCatch = false;
	int element;
	int counter = 0;
	int openRange = 0;

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		string& line = storeline[i];

		// compute numelements
		if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND)
		{
			if (!elementCatch)
			{
				elementCatch = true;
				element = i;
				string templine = line.substr(0, line.find("<hkparam name=\"", 0));
				openRange = count(templine.begin(), templine.end(), '\t');
			}
			else
			{
				ErrorMessage(2019, "General", behaviorFile, i + 1);
			}
		}
		else if (elementCatch)
		{
			if (line.find("</hkparam>") != NOT_FOUND)
			{
				string templine = line.substr(0, line.find("</hkparam>"));
				__int64 range = count(templine.begin(), templine.end(), '\t');

				if (openRange == range)
				{
					string oldElement;
					string& ref_line = storeline[element];

					if (ref_line.find("numelements=\"$elements$\">", 0) == NOT_FOUND)
					{
						size_t position = ref_line.find("numelements=\"") + 13;
						oldElement = ref_line.substr(position, ref_line.find("\">", position) - position);
					}
					else
					{
						oldElement = "$elements$";
					}

					if (oldElement != to_string(counter))
					{
						ref_line.replace(ref_line.find(oldElement), oldElement.length(), to_string(counter));
					}

					counter = 0;
					elementCatch = false;
				}
			}
			else
			{
				string templine = line;

				if (templine.find("<hkobject>") != NOT_FOUND)
				{
					templine = templine.substr(0, templine.find("<hkobject>"));
					__int64 range = count(templine.begin(), templine.end(), '\t');

					if (range == openRange + 1) counter++;
				}
				else if (templine.find("\t\t\t#") != NOT_FOUND)
				{
					templine = templine.substr(0, templine.find("#", 0));
					__int64 reference = count(templine.begin(), templine.end(), '\t');

					if (reference == openRange + 1)
					{
						__int64 number = count(line.begin(), line.end(), '#');
						counter += static_cast<int>(number);
					}
				}
			}
		}
	}

	return storeline;
}
void NodeJoint::dataBake(vecstr& node, string format, string filename, unordered_map<string, string> otherAnimType, unsigned int startline, unsigned int size)
{
	string multiOption;
	vecstr tempstore;
	FuncType curType = NONE;
	int scope = 0;
	unsigned int start;
	bool conditionOpen = false;
	bool root;

	if (size == -1)
	{
		size = node.size();
		root = true;
	}
	else
	{
		row = startline - 1;
		root = false;
	}

	for (unsigned int i = startline; i < size; ++i)
	{
		string line = node[i];

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = CONDITION_START;
				start = i + 1;
				unsigned int pos = line.find("<!-- CONDITION START ^") + 22;
				multiOption = line.substr(pos, line.find("^ -->", pos) - pos);
				conditionOpen = true;
			}

			++scope;
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			--scope;

			if (scope == 0)
			{
				if (!conditionOpen) ErrorMessage(1119, format, filename, i + 1);

				storeTemplate.push_back(LineCheck(make_shared<NodeJoint>(node, format, filename, behaviorFile, otherAnimType, start, i), i + 1));
				storeTemplate.back().nested->condt = multiOption;
				storeTemplate.back().nested->type = curType;
				output.push_back(vector<LineCheck>());
				output.back().push_back(storeTemplate.back());

				curType = CONDITION_ELSE;
				start = i + 1;
				unsigned int pos = line.find("<!-- CONDITION ^") + 16;
				multiOption = line.substr(pos, line.find("^", pos) - pos);
			}

			++scope;

		}
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
		{
			--scope;

			if (scope == 0)
			{
				if (!conditionOpen) ErrorMessage(1119, format, filename, i + 1);

				storeTemplate.push_back(LineCheck(make_shared<NodeJoint>(node, format, filename, behaviorFile, otherAnimType, start, i), i + 1));
				storeTemplate.back().nested->condt = multiOption;
				storeTemplate.back().nested->type = curType;
				output.push_back(vector<LineCheck>());
				output.back().push_back(storeTemplate.back());

				curType = CONDITION;
				start = i + 1;
				multiOption = "";
			}

			++scope;
		}
		else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = FOREACH;
				start = i + 1;
				unsigned int pos = line.find("<!-- FOREACH ^") + 14;
				multiOption = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));
			}

			++scope;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", line.find("<!-- NEW ^", 0)) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = NEW;
				start = i + 1;
				unsigned int pos = line.find("<!-- NEW ^") + 10;
				multiOption = nemesis::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));
			}

			++scope;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			--scope;

			if (scope == 0)
			{
				if (!conditionOpen) ErrorMessage(1118, format, filename, i + 1);

				storeTemplate.push_back(LineCheck(make_shared<NodeJoint>(node, format, filename, behaviorFile, otherAnimType, start, i), i + 1));
				storeTemplate.back().nested->condt = multiOption;
				storeTemplate.back().nested->type = curType;
				output.push_back(vector<LineCheck>());
				output.back().push_back(storeTemplate.back());
				curType = NONE;
				conditionOpen = false;
			}
		}
		else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (scope == 0) ErrorMessage(1171, format, filename, i + 1);

			--scope;

			if (scope == 0)
			{
				if (startline == 0)
				{
					string tempCode;

					if (multiOption == format || multiOption == format + "_group" || multiOption == format + "_master" || multiOption.find(format + "[") == 0 ||
						multiOption.find(format + "_group[") == 0 || multiOption.find(format + "_master[") == 0)
					{
						tempCode = format;
					}
					else
					{
						for (auto& animType : otherAnimType)
						{
							if (multiOption == animType.first || multiOption.find(animType.first + "[") == 0)
							{
								tempCode = animType.second;
								break;
							}
						}
					}

					templateSection[tempCode].push_back(storeTemplate.size());
				}

				storeTemplate.push_back(LineCheck(make_shared<NodeJoint>(node, format, filename, behaviorFile, otherAnimType, start, i), start));
				storeTemplate.back().nested->condt = multiOption;
				storeTemplate.back().nested->type = curType;
				output.push_back(vector<LineCheck>());
				output.back().push_back(storeTemplate.back());
				curType = NONE;
			}
		}
		else if (scope == 0)
		{
			storeTemplate.push_back(LineCheck(line, i + 1));
			output.push_back(vector<LineCheck>());
			output.back().push_back(LineCheck(line, i + 1));
			output.back().back().deleted = false;

			if (count(line.begin(), line.end(), '$') > 0)
			{
				storeTemplate.back().process = true;
				output.back().back().process = true;
			}
		}
	}
}

void processing(string& line, shared_ptr<NodePackedParameters> parameters)
{
	processing(line, parameters->format, parameters->filename, parameters->behaviorFile, parameters->nextFunctionID, parameters->strID, parameters->groupAnimInfo, parameters->IDExist,
		parameters->subFunctionIDs, parameters->import, parameters->linecount, parameters->eventid, parameters->variableid, parameters->zeroEvent, parameters->zeroVariable,
		parameters->groupMulti, parameters->animMulti, parameters->optionMulti, parameters->hasGroup, parameters->multiOption, parameters->optionPicked, parameters->openRange,
		parameters->elementCount, parameters->line, parameters->elementCatch, parameters->negative);
}

void processing(string& lineRef, string format, string filename, string behaviorFile, int& nextFunctionID, string& strID, vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo,
	SSMap& IDExist, shared_ptr<master> subFunctionIDs, ImportContainer& import, int linecount, id eventid, id variableid, string zeroEvent, string zeroVariable,
	int groupMulti, int animMulti, int optionMulti, bool hasGroup, string multiOption, vector<vector<unordered_map<string, bool>>>& optionPicked,
	unsigned __int64& openRange, unsigned int& elementCount, string& templateLine, LineCheck* elementCatch, bool& negative)
{
	string line = templateLine;

	// set animation ID
	while (line.find("$%$", 0) != NOT_FOUND)
	{
		string num;

		if (optionMulti != -1) num = to_string(optionMulti);
		else if (animMulti != -1) num = to_string(animMulti);
		else if (groupMulti != -1) num = to_string(groupMulti);
		else num = "0";

		line.replace(line.find("$%$"), 3, num);
	}

	// multi choice selection
	if (line.find("$MC$", 0) != NOT_FOUND)
	{
		AnimationUtility utility;
		utility.originalCondition = line;
		utility.nodeProcess = make_shared<NodePackedParameters>(format, filename, behaviorFile, nextFunctionID, strID, groupAnimInfo, IDExist, subFunctionIDs, import, linecount,
			eventid, variableid, zeroEvent, zeroVariable, groupMulti, animMulti, optionMulti, hasGroup, multiOption, optionPicked, openRange, elementCount, templateLine, elementCatch,
			negative);
		utility.hasGroup = groupMulti == -1;
		utility.groupMulti = groupMulti;
		utility.animMulti = animMulti;
		utility.optionMulti = optionMulti;
		utility.multiOption = multiOption;

		multiChoice(line, filename, optionPicked, groupAnimInfo, linecount, format, format, utility);
	}

	if (line.find("\t\t\t#") != NOT_FOUND && (line.find("#" + format + "$") != NOT_FOUND
		|| line.find("#" + format + "_group$") != NOT_FOUND || line.find("#" + format + "_master") != NOT_FOUND))
	{
		vecstr generator;
		StringSplit(line, generator);
		size_t nextpos = 0;

		for (auto& ID : generator)
		{
			if (ID.find("#" + format + "$") != NOT_FOUND && multiOption == format)
			{
				nextpos = line.find("#" + format + "$", nextpos) + 1;
				string tempID = line.substr(nextpos);
				string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
				curID = format + "$" + curID;

				if (tempID.find(curID, 0) != NOT_FOUND && nextpos == line.find(curID))
				{
					if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[curID].length() == 0)
					{
						ErrorMessage(2011, format, filename, linecount, curID);
					}

					line.replace(nextpos, curID.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[curID]);
				}
				else
				{
					ErrorMessage(2012, format, filename, linecount, curID);
				}
			}
			else if (ID.find("#" + format + "_group") != NOT_FOUND && multiOption == format + "_group")
			{
				nextpos = line.find("#" + multiOption, nextpos) + 1;
				string tempID = line.substr(nextpos);
				string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
				curID = multiOption + "$" + curID;

				if (tempID.find(curID, 0) != NOT_FOUND && nextpos == line.find(curID))
				{
					if (subFunctionIDs->grouplist[groupMulti]->functionIDs[curID].length() == 0)
					{
						ErrorMessage(2011, format, filename, linecount, curID);
					}

					line.replace(nextpos, curID.length(), subFunctionIDs->grouplist[groupMulti]->functionIDs[curID]);
				}
				else
				{
					ErrorMessage(2012, format, filename, linecount, curID);
				}
			}
			else if (ID.find("#" + format + "_master") != NOT_FOUND && multiOption == format + "_master")
			{
				nextpos = line.find("#" + multiOption, nextpos) + 1;
				string tempID = line.substr(nextpos);
				string curID = boost::regex_replace(string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
				curID = multiOption + "$" + curID;

				if (tempID.find(curID, 0) != NOT_FOUND && nextpos == line.find(curID))
				{
					if (subFunctionIDs->functionIDs[curID].length() == 0)
					{
						ErrorMessage(2011, format, filename, linecount, curID);
					}

					line.replace(nextpos, curID.length(), subFunctionIDs->functionIDs[curID]);
				}
				else
				{
					ErrorMessage(2012, format, filename, linecount, curID);
				}
			}
		}
	}

	if (error) throw nemesis::exception();

	if (line.find("$") != NOT_FOUND)
	{
		processing2(line, format, filename, nextFunctionID, strID, groupAnimInfo, IDExist, subFunctionIDs, import, linecount, eventid, variableid, zeroEvent,
			zeroVariable, groupMulti, animMulti, optionMulti, hasGroup, multiOption);
	}

	lineRef = line;
}

void processing2(string& line, string format, string filename, int& nextFunctionID, string& strID, vector<vector<shared_ptr<AnimationInfo>>>& groupAnimInfo, SSMap& IDExist,
	shared_ptr<master> subFunctionIDs, ImportContainer& import, int linecount, id eventid, id variableid, string zeroEvent, string zeroVariable,
	int groupMulti, int animMulti, int optionMulti, bool hasGroup, string multiOption)
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
		size_t pos = change.find(format + "[", 0);
		bool innerError = false;

		while (true)
		{
			// order equation
			if (change.find("(") != NOT_FOUND && change.find("L", change.find("(")) != NOT_FOUND && change.find(")", change.find("(")) != NOT_FOUND)
			{
				__int64 maths = count(change.begin(), change.end(), '(');

				if (maths != 0 && maths == count(change.begin(), change.end(), ')'))
				{
					size_t nextpos = change.find("(");

					for (__int64 j = 0; j < maths; ++j)
					{
						string equation = change.substr(nextpos, change.find(")", nextpos) - 1);
						string number = "";
						string ID = "";

						if (equation.find("(S", 0) != NOT_FOUND)
						{
							ID = boost::regex_replace(string(equation), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (change.find("(S" + ID + "+") == NOT_FOUND) ID = "";

							number = boost::regex_replace(string(equation.substr(3 + ID.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
						}

						if (equation != "(S" + ID + "+" + number + ")")
						{
							size_t equationLength = equation.length();

							if (equation.find("L") != NOT_FOUND)
							{
								__int64 maths2 = count(change.begin(), change.end(), 'L');

								for (__int64 k = 0; k < maths2; ++k)
								{
									equation.replace(equation.find("L"), 1, to_string(groupAnimInfo[groupMulti].size() - 1));
								}
							}

							nemesis::calculate(equation, format, filename, linecount);

							if (stoi(equation) > int(groupAnimInfo[groupMulti].size() - 1) || stoi(equation) < 0) ErrorMessage(1148, format, filename, linecount, change);

							change.replace(nextpos, equationLength, equation);
							isChange = true;
						}

						nextpos = change.find("(", nextpos + 1);
					}
				}
			}

			if (groupMulti > -1)
			{
				size_t pos = change.find(format + "[");

				if (pos != string::npos)
				{
					if (multiOption != format || animMulti == -1)
					{
						ErrorMessage(1052, format, filename, linecount, line);
					}

					if (change.find("[FilePath]") != NOT_FOUND)
					{
						if (change.find(format + "[][FilePath]", 0) != NOT_FOUND)
						{
							change.replace(change.find(format + "[][FilePath]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FilePath"]);
							isChange = true;
						}

						if (change.find(format + "[F][FilePath]", 0) != NOT_FOUND)
						{
							change.replace(change.find(format + "[F][FilePath]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FilePath"]);
							isChange = true;
						}

						if (change.find(format + "[N][FilePath]", 0) != NOT_FOUND) ErrorMessage(1056, format, filename, linecount, line);
						if (change.find(format + "[B][FilePath]", 0) != NOT_FOUND) ErrorMessage(1056, format, filename, linecount, line);

						if (change.find(format + "[L][FilePath]", 0) != NOT_FOUND)
						{
							change.replace(change.find(format + "[L][FilePath]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FilePath"]);
							isChange = true;
						}

						if (change.find(format + "[", 0) != NOT_FOUND)
						{
							string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (change.find(format + "[" + number + "][FilePath]", 0) != NOT_FOUND)
							{
								if (unsigned int(stoi(number)) >= groupAnimInfo[groupMulti].size())
								{
									change.replace(change.find(format + "[" + number + "][FilePath]"), 8 + format.length() + number.length(),
										subFunctionIDs->grouplist[groupMulti]->singlelist[stoi(number)]->format["FilePath"]);
									isChange = true;
								}
								else
								{
									ErrorMessage(1148, format, filename, linecount, change);
								}
							}
						}
					}

					if (change.find("[FileName]") != NOT_FOUND)
					{
						if (change.find(format + "[][FileName]", 0) != NOT_FOUND)
						{
							change.replace(change.find(format + "[][FileName]"), 12 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["FileName"]);
						}

						if (change.find(format + "[F][FileName]", 0) != NOT_FOUND)
						{
							change.replace(change.find(format + "[F][FileName]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["FileName"]);
							isChange = true;
						}

						if (change.find(format + "[N][FileName]", 0) != NOT_FOUND) ErrorMessage(1056, format, filename, linecount, line);
						if (change.find(format + "[B][FileName]", 0) != NOT_FOUND) ErrorMessage(1056, format, filename, linecount, line);

						if (change.find(format + "[L][FileName]", 0) != NOT_FOUND)
						{
							change.replace(change.find(format + "[L][FileName]"), 13 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["FileName"]);
							isChange = true;
						}

						if (change.find(format + "[", 0) != NOT_FOUND)
						{
							string number = boost::regex_replace(string(change.substr(change.find(format + "[", 0))), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (change.find(format + "[" + number + "][FileName]", 0) != NOT_FOUND)
							{
								if (unsigned int(stoi(number)) >= groupAnimInfo[groupMulti].size())
								{
									change.replace(change.find(format + "[" + number + "][FileName]"), 8 + format.length() + number.length(),
										subFunctionIDs->grouplist[groupMulti]->singlelist[stoi(number)]->format["FileName"]);
									isChange = true;
								}
								else
								{
									ErrorMessage(1148, format, filename, linecount, change);
								}
							}
						}

						if (error) throw nemesis::exception();
					}

					if (change.find("main_anim_event") != NOT_FOUND)
					{
						size_t pos = change.find("[" + format + "[][main_anim_event]]", 0);

						if (pos != NOT_FOUND && (change.find("eventID[" + format + "[][main_anim_event]]") == NOT_FOUND
							|| change.find("eventID[" + format + "[][main_anim_event]]") != pos - 7))
						{
							change.replace(pos, 21 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["main_anim_event"]);
							isChange = true;
						}
						else
						{
							pos = change.find(format + "[][main_anim_event]", 0);

							if (pos != NOT_FOUND)
							{
								change.replace(pos, 19 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["main_anim_event"]);
								isChange = true;
							}
						}

						pos = change.find("[" + format + "[F][main_anim_event]]", 0);

						if (pos != NOT_FOUND && (change.find("eventID[" + format + "[F][main_anim_event]]") == NOT_FOUND
							|| change.find("eventID[" + format + "[F][main_anim_event]]") != pos - 7))
						{
							change.replace(pos, 22 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["main_anim_event"]);
							isChange = true;
						}
						else
						{
							pos = change.find(format + "[F][main_anim_event]", 0);

							if (pos != NOT_FOUND)
							{
								change.replace(pos, 20 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[0]->format["main_anim_event"]);
								isChange = true;
							}
						}

						if (change.find(format + "[N][main_anim_event]", 0) != NOT_FOUND) ErrorMessage(1056, format, filename, linecount, line);
						if (change.find(format + "[B][main_anim_event]", 0) != NOT_FOUND) ErrorMessage(1056, format, filename, linecount, line);

						pos = change.find("[" + format + "[L][main_anim_event]]", 0);

						if (pos != NOT_FOUND && (change.find("eventID[" + format + "[L][main_anim_event]]") == NOT_FOUND
							|| change.find("eventID[" + format + "[L][main_anim_event]]") != pos - 7))
						{
							change.replace(pos, 22 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["main_anim_event"]);
							isChange = true;
						}
						else
						{
							pos = change.find(format + "[L][main_anim_event]", 0);

							if (pos != NOT_FOUND)
							{
								change.replace(pos, 20 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist.back()->format["main_anim_event"]);
								isChange = true;
							}
						}

						if (change.find(format + "[", 0) != NOT_FOUND)
						{
							string number = boost::regex_replace(string(change), boost::regex(format + "\\[([0-9]+)\\]\\[main_anim_event\\].*"), string("\\1"));

							if (number != change)
							{
								if (unsigned int(stoi(number)) >= groupAnimInfo[groupMulti].size())
								{
									pos = change.find("[" + format + "[" + number + "][main_anim_event]]", 0);

									if (pos != NOT_FOUND && (change.find("eventID[" + format + "[" + number + "][main_anim_event]]") == NOT_FOUND
										|| change.find("eventID[" + format + "[" + number + "][main_anim_event]]") != pos - 7))
									{
										change.replace(pos, 21 + format.length(), subFunctionIDs->grouplist[groupMulti]->singlelist[stoi(number)]->format["main_anim_event"]);
										isChange = true;
									}
									else
									{
										pos = change.find(format + "[" + number + "][main_anim_event]", 0);

										if (pos != NOT_FOUND)
										{
											change.replace(pos, 19 + format.length() + number.length(),
												subFunctionIDs->grouplist[groupMulti]->singlelist[stoi(number)]->format["main_anim_event"]);
											isChange = true;
										}
									}
								}
								else
								{
									ErrorMessage(1148, format, filename, linecount, change);
								}
							}
						}

						if (error) throw nemesis::exception();
					}

					if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
					{
						formatGroupReplace(change, change, 0, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);
						isChange = true;

						if (innerError) ErrorMessage(2014, format, filename, linecount);

						if (error) throw nemesis::exception();
					}
				}

				pos = change.find(format + "_group[][");

				if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
				{
					formatGroupReplace(change, change, 0, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);
					isChange = true;

					if (innerError) ErrorMessage(2014, format, filename, linecount);

					if (error) throw nemesis::exception();
				}

				pos = change.find(format + "_master[");

				if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
				{
					if (subFunctionIDs->functionIDs.size() == 0) ErrorMessage(1208, format, filename, linecount);

					for (auto& each : subFunctionIDs->functionIDs)
					{
						string match = format + "_master[" + each.first + "]";

						if (change.find(match) != NOT_FOUND)
						{
							int counter = sameWordCount(change, match);

							for (int j = 0; j < counter; ++j)
							{
								change.replace(change.find(match), match.length(), each.second);
								isChange = true;
							}

							if (change.length() == each.second.length()) break;
						}
					}
				}

				if (multiOption == format)
				{
					string tempchange = change;
					pos = change.find("[");

					pos = change.find("{main_anim_event}", 0);

					if (pos != NOT_FOUND)
					{
						change.replace(pos, 17, subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["main_anim_event"]);
						isChange = true;
					}
					else
					{
						pos = change.find("main_anim_event", 0);

						if (pos != NOT_FOUND)
						{
							change.replace(pos, 15, subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format["main_anim_event"]);
							isChange = true;
						}
					}

					if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
					{
						formatGroupReplace(change, change, 0, filename, format, subFunctionIDs, groupAnimInfo, linecount, groupMulti, optionMulti, animMulti, multiOption, innerError);

						if (innerError) ErrorMessage(2014, format, filename, linecount);

						if (error) throw nemesis::exception();
					}
					else if (subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change].length() > 0)
					{
						change = subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format[change];
					}

					if (change != tempchange) isChange = true;
				}

				if (animMulti > -1)
				{
					for (auto& each : subFunctionIDs->grouplist[groupMulti]->singlelist[animMulti]->format)
					{
						if (change.find(each.first) != NOT_FOUND)
						{
							int counter = sameWordCount(change, each.first);

							for (int j = 0; j < counter; ++j)
							{
								change.replace(change.find(each.first), each.first.length(), each.second);
								isChange = true;
							}

							if (change.length() == each.second.length()) break;
						}
					}
				}
				else
				{
					for (auto& each : subFunctionIDs->grouplist[groupMulti]->functionIDs)
					{
						if (change.find(each.first) != NOT_FOUND)
						{
							int counter = sameWordCount(change, each.first);

							for (int j = 0; j < counter; ++j)
							{
								change.replace(change.find(each.first), each.first.length(), each.second);
								isChange = true;
							}

							if (change.length() == each.second.length()) break;
						}
					}
				}

				if (error) throw nemesis::exception();
			}
			else
			{
				for (auto& each : subFunctionIDs->functionIDs)
				{
					if (change.find(each.first) != NOT_FOUND)
					{
						int counter = sameWordCount(change, each.first);

						for (int j = 0; j < counter; ++j)
						{
							change.replace(change.find(each.first), each.first.length(), each.second);
							isChange = true;
						}

						if (change.length() == each.second.length()) break;
					}
				}
			}

			size_t position = change.find("eventID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				eventIDReplacer(change, format, filename, eventid, zeroEvent, linecount);
				isChange = true;
			}

			position = change.find("variableID[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				variableIDReplacer(change, format, filename, variableid, zeroVariable, linecount);
				isChange = true;
			}

			position = change.find("crc32[");

			if (position != NOT_FOUND &&  change.find("]", position) != NOT_FOUND)
			{
				CRC32Replacer(change, format, filename, linecount);
				isChange = true;
			}

			if (error) throw nemesis::exception();

			position = change.find("import[");

			if (position != NOT_FOUND && change.find("]", position) != NOT_FOUND)
			{
				size_t nextpos = change.find("import[");
				string importer = change.substr(nextpos, change.find_last_of("]") - nextpos + 1);
				__int64 bracketCount = count(importer.begin(), importer.end(), '[');
				__int64 altBracketCount = count(importer.begin(), importer.end(), ']');

				if (IDExist[importer].length() == 0)
				{
					if (bracketCount != altBracketCount) ErrorMessage(2013, format, filename, linecount, importer);

					size_t pos = importer.find("[") + 1;
					string file = importer.substr(pos, importer.find("]", pos) - pos);
					string keyword;
					string tempID;

					if (bracketCount > 1)
					{
						pos = importer.find("[", pos);
						string tempKeyword = importer.substr(pos, importer.find_last_of("]") + 1 - pos);
						int openBrack = 0;

						for (auto& curChar : tempKeyword)
						{
							if (curChar == '[')
							{
								++openBrack;
							}
							else if (curChar == ']')
							{
								--openBrack;

								if (openBrack == 0) keyword.append("!~^!");
							}
							else
							{
								keyword = keyword + curChar;
							}
						}

						pos = keyword.rfind("!~^!");

						if (openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4) ErrorMessage(2013, format, filename, linecount, importer);
						else keyword = keyword.substr(0, keyword.length() - 4);
					}
					else
					{
						keyword = "";
					}

					if (import[file][keyword].length() > 0)
					{
						tempID = import[file][keyword];
					}
					else
					{
						tempID = strID;
						IDExist[importer] = tempID;
						import[file][keyword] = tempID;
						newID(nextFunctionID, strID);
					}

					change.replace(nextpos, importer.length(), tempID);
					isChange = true;
				}
				else
				{
					change.replace(nextpos, importer.length(), IDExist[importer]);
					isChange = true;
				}
			}

			if (change.find("MD") != NOT_FOUND) ErrorMessage(1096, format, filename, linecount);
			if (change.find("RD") != NOT_FOUND) ErrorMessage(1097, format, filename, linecount);
			if (isChange) line.replace(line.find(oldChange), oldChange.length(), change);

			break;
		}
	}
}

void newID(int& nextFunctionID, string& strID)
{
	++nextFunctionID;

	if (nextFunctionID == 9216) ++nextFunctionID;

	strID = to_string(nextFunctionID);

	while (strID.length() < 4)
	{
		strID = "0" + strID;
	}
}
