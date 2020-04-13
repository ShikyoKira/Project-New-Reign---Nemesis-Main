#include "utilities/compute.h"
#include "utilities/conditions.h"

#include "generate/alternateanimation.h"
#include "generate/animationdatatracker.h"

#include "generate/animation/templatetree.h"
#include "generate/animation/grouptemplate.h"
#include "generate/animation/singletemplate.h"
#include "generate/animation/templateprocessing.h"

using namespace std;


string strDoubleDecimal(double num, int decimal)
{
	if (num != static_cast<int>(num))
	{
		stringstream ss;
		ss << fixed << setprecision(decimal);
		ss << num;
		string strNum = ss.str();
		strNum.erase(strNum.find_last_not_of('0') + 1, NOT_FOUND);
		return strNum;
	}
	else
	{
		stringstream ss;
		ss << fixed << setprecision(decimal);
		ss << static_cast<int>(num);
		return ss.str();
	}
}

void stateInput(string& state, string stateID, string format, string behaviorFile, string original, bool isMaster, int groupMulti, int animMulti, int numline,
	size_t& stateCount, shared_ptr<master>& masterFunction)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, original);

	if (isMaster)
	{
		if (masterFunction->grouplist[groupMulti]->functionIDs.find(stateID) != masterFunction->grouplist[groupMulti]->functionIDs.end())
		{
			state = masterFunction->grouplist[groupMulti]->functionIDs[stateID];
		}
		else
		{
			ErrorMessage(1127, format, behaviorFile, numline);
		}
	}
	else
	{
		if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, original);

		if (masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format.find(stateID) != masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format.end())
		{
			state = masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format[stateID];
		}
		else
		{
			ErrorMessage(1127, format, behaviorFile, numline);
		}
	}
}

string nemesis::smatch::operator[](int number)
{
	return match[number];
}

size_t nemesis::smatch::position(int number)
{
	return positionlist[number];
}

size_t nemesis::smatch::size()
{
	return match.size();
}

namespace nemesis
{
	bool regex_search(string line, nemesis::smatch& n_match, boost::regex rgx)
	{
		n_match = nemesis::smatch();
		boost::smatch match;

		if (!boost::regex_search(string(line), match, rgx)) return false;

		if (match.size() > 1)
		{
			n_match.match.reserve(match.size());
			n_match.positionlist.reserve(match.size());
			n_match.positionlist.push_back(match.position());
			n_match.match.push_back(line.substr(match.position(), match[0].length()));

			for (unsigned int i = 1; i < match.size(); ++i)
			{
				n_match.positionlist.push_back(match.position(i));
				n_match.match.push_back(line.substr(match.position(i), match[i].length()));
			}
		}

		return true;
	}
}

void proc::Register(string n_format, string n_masterformat, string n_behaviorFile, string n_filepath, string n_filename, string n_mainAnimEvent, string& n_strID,
	string n_zeroEvent, string n_zeroVariable, bool n_hasGroup, bool& n_negative, bool& n_isEnd, bool& n_norElement, bool& n_elementCatch, bool n_hasDuration,
	double n_duration, int& n_openRange, int& n_counter, size_t& n_elementLine, int n_furnitureCount, id& n_eventid, id& n_variableid, vector<int> n_fixedStateID,
	vector<int> n_stateCountMultiplier, NewAnimLock* n_animLock, int n_order, int n_lastorder, SSMap& n_IDExist, unordered_map<int, vecstr>& n_AnimObject,
	ImportContainer& n_addition, ImportContainer* n_newImport, unordered_map<string, unordered_map<string, vecstr>>& n_groupAddition,
	vector<unordered_map<string, bool>>& n_groupOptionPicked, shared_ptr<group> n_groupFunction, shared_ptr<vecstr> n_generatedlines, NewAnimation* n_curAnim)
{
	format = n_format;
	masterformat = n_masterformat;
	behaviorFile = n_behaviorFile;
	filepath = n_filepath;
	filename = n_filename;
	mainAnimEvent = n_mainAnimEvent;
	strID = &n_strID;
	zeroEvent = n_zeroEvent;
	zeroVariable = n_zeroVariable;
	hasGroup = n_hasGroup;
	negative = &n_negative;
	isEnd = &n_isEnd;
	norElement = &n_norElement;
	elementCatch = &n_elementCatch;
	hasDuration = n_hasDuration;
	duration = n_duration;
	openRange = &n_openRange;
	counter = &n_counter;
	elementLine = &n_elementLine;
	furnitureCount = n_furnitureCount;
	eventid = &n_eventid;
	variableid = &n_variableid;
	fixedStateID = n_fixedStateID;
	stateCountMultiplier = n_stateCountMultiplier;
	animLock = n_animLock;
	order = n_order;
	lastorder = n_lastorder;
	IDExist = &n_IDExist;
	AnimObject = &n_AnimObject;
	addition = &n_addition;
	newImport = n_newImport;
	groupAddition = &n_groupAddition;
	groupOptionPicked = &n_groupOptionPicked;
	groupFunction = n_groupFunction;
	generatedlines = n_generatedlines;
	curAnim = n_curAnim;
}

void proc::installBlock(nemesis::scope blok, int curline)
{
	blockCheck(blok.front, blok.back);
	lineblocks[curline].blocksize[blok.size].push_back(blok);
}

void proc::installBlock(nemesis::scope blok, int curline, vector<nemesis::MultiChoice> m_condiiton)
{
	blockCheck(blok.front, blok.back);
	hasMC[curline] = true;
	multiChoice[curline] = m_condiiton;
	choiceblocks[curline].push_back(blok);
}

void proc::relativeNegative(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (*isEnd || *negative)
		{
			blocks[blok.front] = "true";
		}
		else
		{
			blocks[blok.front] = "false";
		}

		*negative = false;
		*isEnd = false;
	}
}

void proc::compute(nemesis::scope blok, vecstr& blocks)
{
	*elementCatch = true;
	*norElement = true;
}

void proc::rangeCompute(nemesis::scope blok, vecstr& blocks)
{
	(*generatedlines)[*elementLine] = boost::regex_replace(string((*generatedlines)[*elementLine]),
		boost::regex("(.*<hkparam name\\=\".+\" numelements\\=\").+(\">.*)"), string("\\1" + to_string(*counter) + "\\2"));
	*norElement = false;
	*counter = 0;
	*elementLine = -1;
}

void proc::upCounter(nemesis::scope blok, vecstr& blocks)
{
	++(*counter);
}

void proc::upCounterPlus(nemesis::scope blok, vecstr& blocks)
{
	string full = combineBlocks(0, blocks.size() - 1, blocks);
	(*counter) += count(full.begin(), full.end(), '#');
}

void proc::animCount(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = to_string(furnitureCount);
}

void proc::multiChoiceRegis(nemesis::scope blok, vecstr& blocks)
{
	if (!captured)
	{
		blocks[blok.front] = "null";
	}
	else
	{
		string result = combineBlocks(captured->front, captured->back, blocks);
		clearBlocks(blok, blocks);
		blocks[blok.front] = result;
	}
}

void proc::groupIDRegis(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string oldID = masterformat + "_group$" + blok.olddata[0];
		animLock->lockID();

		if (groupFunction->functionIDs.find(oldID) != groupFunction->functionIDs.end())
		{
			blocks[blok.front] = groupFunction->functionIDs[oldID];
		}
		else
		{
			groupFunction->functionIDs[oldID] = *strID;
			blocks[blok.front] = *strID;
			curAnim->newID();
		}

		animLock->releaseID();
	}
}

void proc::IDRegis(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string ID;
		string oldID = masterformat + "$" + blok.olddata[0];

		if ((*IDExist)[oldID].length() > 0)
		{
			ID = (*IDExist)[oldID];
		}
		else
		{
			(*IDExist)[oldID] = *strID;
			ID = *strID;
			curAnim->newID();
		}

		curAnim->subFunctionIDs->format[oldID] = ID;
		blocks[blok.front] = ID;
	}
}

void proc::IDRegisAnim(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string ID;
		string oldID = masterformat + "$" + blok.olddata[0];
		int n_groupMulti;

		if (isMaster && groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[1]);

		n_groupMulti = groupMulti;

		if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, blok.olddata[0]);

		if (masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format.find(oldID) !=
			masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format.end())
		{
			ID = masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format[oldID];
			(*IDExist)[oldID] = ID;
		}
		else
		{
			if ((*IDExist)[oldID].length() > 0)
			{
				ID = (*IDExist)[oldID];
			}
			else
			{
				(*IDExist)[oldID] = *strID;
				ID = *strID;
				curGroup->newID();
			}

			masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format[oldID] = ID;
		}

		blocks[blok.front] = ID;
	}
}

void proc::IDRegisGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string ID;
		string oldID = masterformat + "_group$" + blok.olddata[0];
		int n_groupMulti;
		
		if (isMaster && groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[1]);

		n_groupMulti = groupMulti;

		if (masterFunction->grouplist[n_groupMulti]->functionIDs.find(oldID) != masterFunction->grouplist[n_groupMulti]->functionIDs.end())
		{
			ID = masterFunction->grouplist[n_groupMulti]->functionIDs[oldID];
			(*IDExist)[oldID] = ID;
		}
		else
		{
			if ((*IDExist)[oldID].length() > 0)
			{
				ID = (*IDExist)[oldID];
			}
			else
			{
				(*IDExist)[oldID] = *strID;
				ID = *strID;
				curGroup->newID();
			}

			masterFunction->grouplist[n_groupMulti]->functionIDs[oldID] = ID;
		}

		blocks[blok.front] = ID;
	}
}

void proc::IDRegisMaster(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string ID;
		string oldID = masterformat + "_master$" + blok.olddata[0];

		if (masterFunction->functionIDs.find(oldID) != masterFunction->functionIDs.end())
		{
			ID = masterFunction->functionIDs[oldID];
			(*IDExist)[oldID] = ID;
		}
		else
		{
			if ((*IDExist)[oldID].length() > 0)
			{
				ID = (*IDExist)[oldID];
			}
			else
			{
				(*IDExist)[oldID] = *strID;
				ID = *strID;
				curGroup->newID();
			}

			masterFunction->functionIDs[oldID] = ID;
		}

		blocks[blok.front] = ID;
	}
}

void proc::computation(nemesis::scope blok, vecstr& blocks)
{
	string equation = combineBlocks(blok, blocks);
	size_t equationLength = equation.length();

	if (clearBlocks(blok, blocks))
	{
		if (equation.find("L") != NOT_FOUND)
		{
			if (isMaster) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);
			if (isGroup) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);

			int maths2 = count(equation.begin(), equation.end(), 'L');

			for (__int64 k = 0; k < maths2; ++k)
			{
				equation.replace(equation.find("L"), 1, to_string(lastorder));
			}
		}

		if (equation.find("N") != NOT_FOUND)
		{
			if (isMaster) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);
			if (isGroup) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);

			int maths2 = count(equation.begin(), equation.end(), 'N');
			string nextorder;
			nextorder = curAnim->isLast() ? to_string(order) : to_string(order + 1);

			for (__int64 k = 0; k < maths2; ++k)
			{
				equation.replace(equation.find("N"), 1, nextorder);
			}
		}

		if (equation.find("B") != NOT_FOUND)
		{
			if (isMaster) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);
			if (isGroup) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);

			int maths2 = count(equation.begin(), equation.end(), 'B');
			string preorder;
			preorder = order == 0 ? to_string(order) : to_string(order - 1);

			for (__int64 k = 0; k < maths2; ++k)
			{
				equation.replace(equation.find("B"), 1, preorder);
			}
		}

		if (equation.find("F") != NOT_FOUND)
		{
			int maths2 = count(equation.begin(), equation.end(), 'F');

			for (__int64 k = 0; k < maths2; ++k)
			{
				equation.replace(equation.find("F"), 1, "0");
			}
		}

		nemesis::calculate(equation, format, behaviorFile, numline);
		blocks[blok.front] = equation;
	}
}

void proc::endMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, blok.olddata[0]);

		if (curAnim->GetGroupAnimInfo()[animMulti]->hasDuration)
		{
			blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[animMulti]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (curAnim->GetGroupAnimInfo()[0]->hasDuration)
		{
			blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[0]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}	
}

void proc::endNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		int num = order;

		if (!curAnim->isLast()) num++;

		if (curAnim->GetGroupAnimInfo()[num]->hasDuration)
		{
			blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[num]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		int num = order;

		if (order != 0) num--;

		if (curAnim->GetGroupAnimInfo()[num]->hasDuration)
		{
			blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[num]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (curAnim->GetGroupAnimInfo()[curAnim->GetGroupAnimInfo().size()]->hasDuration)
		{
			blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[lastorder]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}	
}

void proc::endNumGroup(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		if (curAnim->GetGroupAnimInfo()[num]->hasDuration)
		{
			blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[num]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, blok.olddata[0]);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		if (curGroup->groupAnimInfo[groupMulti][animMulti]->hasDuration)
		{
			blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][animMulti]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		if (curGroup->groupAnimInfo[groupMulti][0]->hasDuration)
		{
			blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][0]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		if (curGroup->groupAnimInfo[groupMulti][curGroup->groupAnimInfo[groupMulti].size()]->hasDuration)
		{
			blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][lastorder]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endNumMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		if (curGroup->groupAnimInfo[groupMulti][num]->hasDuration)
		{
			blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][num]->duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::endSingle(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (hasDuration)
		{
			blocks[blok.front] = to_string(duration);
		}
		else
		{
			blocks[blok.front] = "0.000000";
			*isEnd = true;
		}
	}
}

void proc::stateMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + ((animMulti - order) *
		stateCountMultiplier[blok.olddataint[0]]) + blok.olddataint[1]);
}

void proc::stateFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] - (order * stateCountMultiplier[blok.olddataint[0]]) + blok.olddataint[1]);
}

void proc::stateNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		curAnim->isLast() ? blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + blok.olddataint[1]) :
			blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + stateCountMultiplier[blok.olddataint[0]] + blok.olddataint[1]);
	}
}

void proc::stateBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		order == 0 ? blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + blok.olddataint[1]) :
			blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] - stateCountMultiplier[blok.olddataint[0]] + blok.olddataint[1]);
	}
}

void proc::stateLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + ((lastorder - order) * stateCountMultiplier[blok.olddataint[0]]) + blok.olddataint[1]);
	}
}

void proc::stateNumGroup(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (blok.olddataint[2] >= int(curAnim->GetGroupAnimInfo().size())) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		blocks[blok.front] = to_string(fixedStateID[num] + ((blok.olddataint[2] - order) * stateCountMultiplier[num]) + blok.olddataint[1]);
	}
}

void proc::stateMultiMasterToGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string state;
		stateInput(state, (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+") + to_string(blok.olddataint[1]) + ")", format, behaviorFile,
			blok.olddata[0], true, groupMulti, animMulti, numline, stateCount, masterFunction);
		blocks[blok.front] = state;
	}
}

void proc::stateMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string state;
		stateInput(state, (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+") + to_string(blok.olddataint[1]) + ")", format, behaviorFile,
			blok.olddata[0], false, groupMulti, animMulti, numline, stateCount, masterFunction);
		blocks[blok.front] = state;
	}
}

void proc::stateFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string state;
		stateInput(state, (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+") + to_string(blok.olddataint[1]) + ")", format, behaviorFile,
			blok.olddata[0], false, groupMulti, 0, numline, stateCount, masterFunction);
		blocks[blok.front] = state;
	}
}

void proc::stateLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string state;
		stateInput(state, (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+") + to_string(blok.olddataint[1]) + ")", format, behaviorFile,
			blok.olddata[0], false, groupMulti, masterFunction->grouplist[groupMulti]->singlelist.size() - 1, numline, stateCount, masterFunction);
		blocks[blok.front] = state;
	}
}

void proc::stateNumMaster(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[2];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		string state;
		stateInput(state, (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+") + to_string(blok.olddataint[1]) + ")", format, behaviorFile,
			blok.olddata[0], false, groupMulti, num, numline, stateCount, masterFunction);
		blocks[blok.front] = state;
	}
}

void proc::stateSingle(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string state = to_string(fixedStateID[blok.olddataint[0]] + blok.olddataint[1]);
		string original = "(S+" + to_string(blok.olddataint[1]) + ")";

		if (isMaster) masterFunction->functionIDs[original] = state;
		else if (isGroup) masterFunction->grouplist[groupMulti]->functionIDs[original] = state;

		blocks[blok.front] = state;
	}
}

void proc::filepathMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[animMulti]->filename;
}

void proc::filepathFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[0]->filename;
}

void proc::filepathNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		curAnim->isLast() ? blocks[blok.front] = filepath + filename : blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[order + 1]->filename;
	}
}

void proc::filepathBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		order == 0 ? blocks[blok.front] = filepath + filename : blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[order - 1]->filename;
	}
}

void proc::filepathLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[lastorder]->filename;
}

void proc::filepathNumGroup(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[num]->filename;
}

void proc::filepathMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format["FilePath"];
}

void proc::filepathFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist[0]->format["FilePath"];
}

void proc::filepathLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist.back()->format["FilePath"];
}

void proc::filepathNumMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist[blok.olddataint[0]]->format["FilePath"];
}

void proc::filepathSingle(nemesis::scope blok, vecstr& blocks)
{
	if (filename == combineBlocks(blok, blocks)) ErrorMessage(1134, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + filename;
}

void proc::filenameMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &curAnim->GetGroupAnimInfo()[animMulti]->filename;
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &curAnim->GetGroupAnimInfo()[0]->filename;
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (curAnim->isLast())
		{
			blocks[blok.front] = filename.substr(0, filename.find_last_of("."));
		}
		else
		{
			string* tempfile = &curAnim->GetGroupAnimInfo()[order + 1]->filename;
			blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
		}
	}
}

void proc::filenameBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (order == 0)
		{
			blocks[blok.front] = filename.substr(0, filename.find_last_of("."));
		}
		else
		{
			string* tempfile = &curAnim->GetGroupAnimInfo()[order - 1]->filename;
			blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
		}
	}
}

void proc::filenameLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &curAnim->GetGroupAnimInfo()[lastorder]->filename;
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameNumGroup(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &curAnim->GetGroupAnimInfo()[num]->filename;
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format["FileName"];
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &masterFunction->grouplist[groupMulti]->singlelist[0]->format["FileName"];
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &masterFunction->grouplist[groupMulti]->singlelist.back()->format["FileName"];
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameNumMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		string* tempfile = &masterFunction->grouplist[groupMulti]->singlelist[blok.olddataint[0]]->format["FileName"];
		blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
	}
}

void proc::filenameSingle(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = filename.substr(0, filename.find_last_of("."));
}

void proc::pathSingle(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if (isGroup) blocks[blok.front] = masterFunction->grouplist[0]->singlelist[0]->format["Path"];
		else if (isMaster) blocks[blok.front] = masterFunction->grouplist[0]->singlelist[0]->format["Path"];
		else blocks[blok.front] = filepath.substr(0, filepath.length() - 1);
	}
}

void proc::AOMultiGroupA(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1 || optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[animMulti]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOMultiGroupB(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[animMulti]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AOFirstGroupA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[0]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOFirstGroupB(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[0]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AONextGroupA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1)
	{
		ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));
	}

	if (clearBlocks(blok, blocks))
	{
		vecstr list;

		curAnim->isLast() ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]] :
			list = curAnim->GetGroupAnimInfo()[order + 1]->AnimObject[blok.olddataint[0]];

		if (int(list.size()) > optionMulti) blocks[blok.front] = list[optionMulti];
	}
}

void proc::AONextGroupB(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		vecstr list;
		curAnim->isLast() ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]] :
			list = curAnim->GetGroupAnimInfo()[order + 1]->AnimObject[blok.olddataint[0]];

		if (int(list.size()) > blok.olddataint[1])
		{
			blocks[blok.front] = list[blok.olddataint[1]];
		}
	}
}

void proc::AOBackGroupA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks))
	{
		vecstr list;
		order == 0 ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]] :
			list = curAnim->GetGroupAnimInfo()[order - 1]->AnimObject[blok.olddataint[0]];

		if (int(list.size()) > optionMulti) blocks[blok.front] = list[optionMulti];
	}
}

void proc::AOBackGroupB(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		vecstr list;
		order == 0 ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]] :
			list = curAnim->GetGroupAnimInfo()[order - 1]->AnimObject[blok.olddataint[0]];

		if (int(list.size()) > blok.olddataint[1]) blocks[blok.front] = list[blok.olddataint[1]];
	}
}

void proc::AOLastGroupA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[lastorder]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOLastGroupB(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[lastorder]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AONumGroupA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[num]->AnimObject[blok.olddataint[1]][optionMulti];
}

void proc::AONumGroupB(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[num]->AnimObject[blok.olddataint[1]][blok.olddataint[2]];
}

void proc::AOMultiMasterA(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1 || optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][animMulti]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOMultiMasterB(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][animMulti]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AOFirstMasterA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][0]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOFirstMasterB(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][0]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AOLastMasterA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][lastorder]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOLastMasterB(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][lastorder]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AONumMasterA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][num]->AnimObject[blok.olddataint[1]][optionMulti];
}

void proc::AONumMasterB(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][num]->AnimObject[blok.olddataint[1]][blok.olddataint[2]];
}

void proc::AOSingleA(nemesis::scope blok, vecstr& blocks)
{
	if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks)) blocks[blok.front] = (*AnimObject)[blok.olddataint[0]][optionMulti];
}

void proc::AOSingleB(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = (*AnimObject)[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::MAEMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[animMulti]->mainAnimEvent;
}

void proc::MAEFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[0]->mainAnimEvent;
}

void proc::MAENextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		curAnim->isLast() ? blocks[blok.front] = curAnim->GetGroupAnimInfo()[order]->mainAnimEvent :
			blocks[blok.front] = curAnim->GetGroupAnimInfo()[order + 1]->mainAnimEvent;
	}
}

void proc::MAEBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		order == 0 ? blocks[blok.front] = curAnim->GetGroupAnimInfo()[order]->mainAnimEvent :
			blocks[blok.front] = curAnim->GetGroupAnimInfo()[order - 1]->mainAnimEvent;
	}
}

void proc::MAELastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[lastorder]->mainAnimEvent;
}

void proc::MAENumGroup(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);
	
	if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[num]->mainAnimEvent;
}

void proc::MAEMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][animMulti]->mainAnimEvent;
}

void proc::MAEFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][0]->mainAnimEvent;
}

void proc::MAELastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][lastorder]->mainAnimEvent;
}

void proc::MAENumMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][num]->mainAnimEvent;
}

void proc::MAESingle(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks)) blocks[blok.front] = mainAnimEvent;
}

void proc::addOnMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curAnim->GetGroupAnimInfo()[animMulti]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curAnim->GetGroupAnimInfo()[animMulti]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curAnim->GetGroupAnimInfo()[0]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curAnim->GetGroupAnimInfo()[0]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		int curorder = order;

		if (!curAnim->isLast()) ++curorder;

		vecstr* list = &curAnim->GetGroupAnimInfo()[curorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curAnim->GetGroupAnimInfo()[curorder]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		int curorder = order;

		if (order > 0) --curorder;

		vecstr* list = &curAnim->GetGroupAnimInfo()[curorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curAnim->GetGroupAnimInfo()[curorder]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curAnim->GetGroupAnimInfo()[lastorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			if (curAnim->GetGroupAnimInfo()[lastorder]->addition[blok.olddata[0]][blok.olddata[1]].length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = curAnim->GetGroupAnimInfo()[lastorder]->addition[blok.olddata[0]][blok.olddata[1]];
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}	
}

void proc::addOnNumGroup(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curAnim->GetGroupAnimInfo()[num]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curAnim->GetGroupAnimInfo()[num]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[2]);

	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curGroup->groupAnimInfo[groupMulti][animMulti]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curGroup->groupAnimInfo[groupMulti][animMulti]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[2]);

	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curGroup->groupAnimInfo[groupMulti][0]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curGroup->groupAnimInfo[groupMulti][0]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[2]);

	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curGroup->groupAnimInfo[groupMulti][lastorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curGroup->groupAnimInfo[groupMulti][lastorder]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnNumMaster(nemesis::scope blok, vecstr& blocks)
{
	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		vecstr* list = &curGroup->groupAnimInfo[groupMulti][num]->groupAddition[blok.olddata[0]][blok.olddata[1]];

		if (list->size() == 0 || optionMulti == -1)
		{
			string output = curGroup->groupAnimInfo[groupMulti][num]->addition[blok.olddata[0]][blok.olddata[1]];

			if (output.length() == 0)
				ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = output;
		}
		else if (int(list->size()) > optionMulti)
		{
			if ((*list)[optionMulti].length() == 0)
				ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*list)[optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}
}

void proc::addOnSingle(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		if ((*groupAddition)[blok.olddata[0]][blok.olddata[1]].size() == 0 || optionMulti == -1)
		{
			if ((*addition)[blok.olddata[0]][blok.olddata[1]].length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

			blocks[blok.front] = (*addition)[blok.olddata[0]][blok.olddata[1]];
		}
		else if (int((*groupAddition)[blok.olddata[0]][blok.olddata[1]].size()) > optionMulti)
		{
			if ((*groupAddition)[blok.olddata[0]][blok.olddata[1]][optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

			blocks[blok.front] = (*groupAddition)[blok.olddata[0]][blok.olddata[1]][optionMulti];
		}
		else
		{
			ErrorMessage(1141);
		}
	}	
}

void proc::lastState(nemesis::scope blok, vecstr& blocks)
{
	if (clearBlocks(blok, blocks))
	{
		size_t ID = 0;

		if (fixedStateID.size() > 1)
		{
			if (blok.olddata[0].length() > 0)
			{
				ID = stoi(blok.olddata[0]) - 1;

				if (ID >= fixedStateID.size()) ErrorMessage(1168, format, behaviorFile, numline, "LastState" + blok.olddata[0]);
			}
		}

		blocks[blok.front] = to_string(lastorder - order + fixedStateID[ID]);
	}
}

void proc::eventID(nemesis::scope blok, vecstr& blocks)
{
	string eventname = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

	if (clearBlocks(blok, blocks))
	{
		string newEventID = to_string((*eventid)[eventname]);

		if (newEventID == "0" && eventname != zeroEvent)
		{
			if (format == "BASE")
			{
				ErrorMessage(1166);
			}
			else
			{
				ErrorMessage(1131, format, filename, numline, eventname);
			}
		}

		blocks[blok.front] = newEventID;
	}
}

void proc::variableID(nemesis::scope blok, vecstr& blocks)
{
	string variablename = combineBlocks(blok.olddataint[0], blok.olddataint[1] , blocks);

	if (clearBlocks(blok, blocks))
	{
		string newVarID = to_string((*variableid)[variablename]);

		if (newVarID == "0" && variablename != zeroVariable)
		{
			if (format == "BASE")
			{
				ErrorMessage(1166);
			}
			else
			{
				ErrorMessage(1132, format, filename, numline, variablename);
			}
		}

		blocks[blok.front] = newVarID;
	}
}

void proc::crc32(nemesis::scope blok, vecstr& blocks)
{
	string crc32line = nemesis::to_lower_copy(combineBlocks(blok, blocks));

	if (clearBlocks(blok, blocks)) blocks[blok.front] = to_string(CRC32Convert(crc32line));
}

void proc::import(nemesis::scope blok, vecstr& blocks)
{
	string import = combineBlocks(blok, blocks);

	if (clearBlocks(blok, blocks))
	{
		if ((*IDExist)[import].length() == 0)
		{
			size_t pos = import.find("[") + 1;
			string file = import.substr(pos, import.find("]", pos) - pos);
			string keyword = "";
			string tempID;

			if (import.find("[", pos) != NOT_FOUND)
			{
				pos = import.find("[", pos);
				string tempKeyword = import.substr(pos, import.find_last_of("]") + 1 - pos);
				int openBrack = 0;

				for (unsigned int j = 0; j < tempKeyword.length(); ++j)
				{
					char curChar = tempKeyword[j];

					if (curChar == '[') ++openBrack;
					else if (curChar == ']')
					{
						--openBrack;

						if (openBrack == 0)
						{
							keyword.append("!~^!");
						}
					}
					else keyword.push_back(curChar);
				}

				pos = keyword.rfind("!~^!");

				openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4 ? ErrorMessage(1139, format, behaviorFile, numline, import) :
					keyword = keyword.substr(0, keyword.length() - 4);
			}

			animLock->lockExport();

			if ((*newImport)[file][keyword].length() > 0)
			{
				tempID = (*newImport)[file][keyword];
			}
			else
			{
				tempID = *strID;
				(*IDExist)[import] = tempID;
				(*newImport)[file][keyword] = tempID;
				isMaster || isGroup ? curGroup->newID() : curAnim->newID();
			}

			animLock->releaseExport();
			blocks[blok.front] = tempID;
		}
		else
		{
			blocks[blok.front] = (*IDExist)[import];
		}
	}	
}

void proc::motionDataMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[animMulti];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}	
}

void proc::motionDataFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[0];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		int nextorder;
		curAnim->isLast() ? nextorder = order : nextorder = order + 1;
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[nextorder];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}	
}

void proc::motionDataBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		int previousorder;
		order == 0 ? previousorder = order : previousorder = order - 1;
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[previousorder];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}	
}

void proc::motionDataLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[lastorder];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataNumGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[num];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][animMulti];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][0];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][lastorder];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataNumMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][num];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::motionDataSingle(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1096, format, behaviorFile, numline);

	if (filename == blok.olddata[0]) ErrorMessage(1134, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[order];
		string motionData;

		if (curInfo->motionData.size() == 0)
		{
			WarningMessage(1018, format, behaviorFile, numline);
			motionData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			motionData = to_string(curInfo->motionData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
			{
				motionData.append(curInfo->motionData[j] + "\n");
			}
		}

		blocks[blok.front] = motionData;
	}
}

void proc::rotationDataMultiGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[animMulti];
		string rotationData;

		if (curInfo->rotationData.size())
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataFirstGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[0];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataNextGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		int nextorder;
		curAnim->isLast() ? nextorder = order: nextorder = order + 1;
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[nextorder];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}	
}

void proc::rotationDataBackGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		int previousorder;
		order == 0 ? previousorder = order : previousorder = order - 1;
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[previousorder];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataLastGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)ErrorMessage(1097, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[lastorder];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataNumGroup(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	size_t num = blok.olddataint[0];

	if (num >= curAnim->GetGroupAnimInfo().size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[num];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataMultiMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][animMulti];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataFirstMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][0];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataLastMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)ErrorMessage(1097, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][lastorder];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataNumMaster(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

	size_t num = blok.olddataint[0];

	if (num >= curGroup->groupAnimInfo[groupMulti].size()) ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][num];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::rotationDataSingle(nemesis::scope blok, vecstr& blocks)
{
	if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0) ErrorMessage(1097, format, behaviorFile, numline);

	if (filename == blok.olddata[0]) ErrorMessage(1134, format, behaviorFile, numline);

	if (clearBlocks(blok, blocks))
	{
		shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[order];
		string rotationData;

		if (curInfo->rotationData.size() == 0)
		{
			WarningMessage(1019, format, behaviorFile, numline);
			rotationData = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
		}
		else
		{
			rotationData = to_string(curInfo->rotationData.size()) + "\n";

			for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
			{
				rotationData.append(curInfo->rotationData[j] + "\n");
			}
		}

		blocks[blok.front] = rotationData;
	}
}

void proc::animOrder(nemesis::scope blok, vecstr& blocks)
{
	string animPath = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

	if (clearBlocks(blok, blocks))
	{
		auto& ptr = charAnimDataInfo.find(nemesis::to_lower_copy(project.substr(0, project.rfind(".txt"))));

		if (ptr != charAnimDataInfo.end())
		{
			auto& ptr2 = ptr->second.find(nemesis::to_lower_copy(std::filesystem::path(animPath).filename().string()));

			if (ptr2 != ptr->second.end())
			{
				blocks[blok.front] = to_string(ptr2->second->GetOrder());
			}
		}
	}
}

void proc::regisAnim(nemesis::scope blok, vecstr& blocks)
{
	string animPath = combineBlocks(blok, blocks);
	addUsedAnim(behaviorFile, animPath);
}

void proc::regisBehavior(nemesis::scope blok, vecstr& blocks)
{
	string behaviorName = combineBlocks(blok, blocks);
	nemesis::to_lower(behaviorName);
	behaviorJoints[behaviorName].push_back(nemesis::to_lower_copy(behaviorFile));
}

void proc::localNegative(nemesis::scope blok, vecstr& blocks)
{
	if (combineBlocks(blok, blocks)[0] == '-') *negative = true;
}

void proc::blocksCompile(vecstr blocks)
{
	for (auto& bracket : brackets[numline])
	{
		blocks[bracket] = "";
	}

	if (hasMC[numline])
	{
		bool done = false;

		for (auto& choice : multiChoice[numline])
		{
			if (done)
			{
				failed.push_back(choice_c(choice.locateA, choice.locateB));
			}
			else
			{
				if (choice.condition->isTrue(this, format, behaviorFile, numline, isGroup, isMaster, choice.condition))
				{
					done = true;
					captured = make_shared<choice_c>(choice.locateA, choice.locateB);
				}
				else
				{
					failed.push_back(choice_c(choice.locateA, choice.locateB));
				}
			}
		}
	}

	for (auto& bloklist : lineblocks[numline].blocksize)
	{
		for (auto& blok : bloklist.second)
		{
			(this->*blok.func)(blok, blocks);
		}
	}

	if (hasMC[numline])
	{		
		for (auto& blok : choiceblocks[numline])
		{
			(this->*blok.func)(blok, blocks);
		}

		failed.clear();
		captured.reset();
	}

	line->clear();

	for (auto& each : blocks)
	{
		line->append(each);
	}
}

bool proc::isThisMaster()
{
	return groupMulti > -1;
}

bool proc::clearBlocks(nemesis::scope& blok, vecstr& blocks)
{
	if (failed.size() > 0)
	{
		for (auto& fail : failed)
		{
			// fail_front < front < back < fail_back
			if (fail.front <= blok.front && blok.back <= fail.back) return false;
		}
	}

	for (size_t i = blok.front; i <= blok.back; ++i)
	{
		blocks[i].clear();
	}

	return true;
}

void proc::blockCheck(size_t front, size_t back)
{
	for (auto& blocklist : lineblocks[numline].blocksize)
	{
		for (auto& blok : blocklist.second)
		{
			if (blok.front < front)
			{
				// b_front < front < b_back < back
				if (front < blok.back) ErrorMessage(1193, format, behaviorFile, numline);
			}
			// front < b_front < back < b_back
			else if (blok.front < back) ErrorMessage(1193, format, behaviorFile, numline);
		}
	}
}

string proc::combineBlocks(nemesis::scope& blok, vecstr& blocks)
{
	string option;

	for (unsigned int i = blok.front; i <= blok.back; ++i)
	{
		option.append(blocks[i]);
	}

	return option;
}

string proc::combineBlocks(size_t front, size_t back, vecstr& blocks)
{
	string option;

	for (unsigned int i = front; i <= back; ++i)
	{
		option.append(blocks[i]);
	}

	return option;
}

choice_c::choice_c(size_t posA, size_t posB)
{
	front = posA;
	back = posB;
}
