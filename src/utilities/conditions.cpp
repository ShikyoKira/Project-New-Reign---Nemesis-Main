#include "utilities/conditions.h"

#include "generate/animation/newanimation.h"
#include "generate/animation/singletemplate.h"
#include "generate/animation/templateprocessing.h"

using namespace std;

vecstr GetOptionInfo(string line, string format, string filename, int numline);
vecstr GetOptionInfo(string line, string format, string masterformat, string filename, string multiOption, int numline);
void ProcessFunction(string change, string line, string format, string behaviorFile, string multiOption, bool& isEnd, int numline, size_t curPos,
	OptionList& optionlist, map<int, vector<shared_ptr<nemesis::scope>>>& lineblocks, vector<AddOnInfo>& addInfo, bool& isTrueMulti, bool isGroup = false, bool isMaster = false,
	bool isMC = true, proc& process = proc());

nemesis::Condt::Condt(string condition, string format, string behaviorFile, string originalCondition, string multiOption, int numline, bool isGroup, bool isMaster,
	OptionList& optionlist)
{
	original = originalCondition;

	if (condition[0] == '(')
	{
		if (condition.find(")") == NOT_FOUND || count(condition.begin(), condition.end(), '(') != count(condition.begin(), condition.end(), ')'))
		{
			isGroup ? ErrorMessage(1105, format + "_group", behaviorFile, numline) : ErrorMessage(1105, format, behaviorFile, numline);
		}

		size_t c_and = 0;
		size_t c_or = 0;
		size_t backB = 0;

		for (unsigned int i = 0; i < condition.size(); ++i)
		{
			if (condition[i] == '(')
			{
				++c_or;
			}
			else if (condition[i] == ')')
			{
				--c_or;

				if (c_or == 0)
				{
					backB = i - 1;
					break;
				}
			}
		}

		// (bool1 & bool2) | bool3 ...
		// --------------^
		string inHouse = condition.substr(1, backB);

		if (error) throw nemesis::exception();

		nestedcond = make_shared<nemesis::Condt>(inHouse, format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);

		if (backB + 2 < condition.length())
		{
			string outHouse = condition.substr(backB + 2);

			if (outHouse.length() == 0 || (outHouse[0] != '|' && outHouse[0] != '&'))
			{
				isGroup ? ErrorMessage(1106, format + "_group", behaviorFile, numline, original) : ErrorMessage(1106, format, behaviorFile, numline, original);
			}

			// ... | (bool1 & bool2)...
			// ----^
			if (outHouse[0] == '|')
			{
				OR = true;
			}

			// ... & (bool1 | bool2)...
			// ----^
			else
			{
				// ntg
			}

			next = make_shared<nemesis::Condt>(outHouse.substr(1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
		}
	}
	else
	{
		size_t c_and = condition.find("&");
		size_t c_or = condition.find("|");

		if (c_and == NOT_FOUND)
		{
			// AND & OR not exist
			if (c_or == NOT_FOUND)
			{
				singleCondition(condition, format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
			}

			// OR exist but not AND
			else
			{
				OR = true;
				singleCondition(condition.substr(0, c_or), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
				next = make_shared<nemesis::Condt>(condition.substr(c_or + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
			}
		}

		// AND exist but not OR
		else if (c_or == NOT_FOUND)
		{
			singleCondition(condition.substr(0, c_and), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
			next = make_shared<nemesis::Condt>(condition.substr(c_and + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
		}

		// AND & OR exist
		else
		{
			// OR before AND
			if (c_or < c_and)
			{
				OR = true;
				singleCondition(condition.substr(0, c_or), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
				next = make_shared<nemesis::Condt>(condition.substr(c_or + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
			}

			// AND before OR
			else
			{
				singleCondition(condition.substr(0, c_and), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
				next = make_shared<nemesis::Condt>(condition.substr(c_and + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
			}
		}
	}
}

bool nemesis::Condt::isTrue(Condt& condt, shared_ptr<Condt> curptr)
{
	return isTrue(condt.prcs, condt.fmt, condt.bhvfile, condt.nm, condt.ig, condt.im, curptr);
}

bool nemesis::Condt::isTrue(proc* process, string format, string behaviorFile, int numline, bool isGroup, bool isMaster, shared_ptr<nemesis::Condt> curptr)
{
	prcs = process;
	fmt = format;
	bhvfile = behaviorFile;
	nm = numline;
	ig = isGroup;
	im = isMaster;

	while (true)
	{
		bool result = false;

		if (curptr->nestedcond)
		{
			result = curptr->nestedcond->isTrue(*this, curptr->nestedcond);
		}
		else if (curptr->optioncondt && curptr->optioncondt->size() > 2)
		{
			if (isGroup)
			{
				size_t groupMulti;
				size_t groupSize;
				bool g_multi = false;

				if ((*curptr->optioncondt)[3] == format + "_group" || (*curptr->optioncondt)[3] == format)
				{
					result = true;
				}
				else
				{
					if ((*curptr->optioncondt)[1].length() == 0)
					{
						if (prcs->groupMulti == -1 || prcs->multiOption != format || prcs->multiOption == format + "_group")
						{
							g_multi = true;
							groupMulti = 0;
							groupSize = prcs->masterOptionPicked->size();
						}
						else
						{
							groupMulti = prcs->groupMulti;
						}
					}
					else
					{
						groupMulti = stoi((*curptr->optioncondt)[1]);
					}

					if (!g_multi)
					{
						groupSize = groupMulti + 1;
					}

					while (groupMulti < groupSize)
					{
						size_t animMulti;
						bool a_multi = false;

						if ((*curptr->optioncondt)[2].length() == 0)
						{
							if (prcs->animMulti == -1 || prcs->multiOption != format)
							{
								a_multi = true;
							}
							else
							{
								animMulti = prcs->animMulti;
							}
						}
						else if ((*curptr->optioncondt)[2] == "F")
						{
							animMulti = 0;
						}
						else if ((*curptr->optioncondt)[2] == "L")
						{
							animMulti = prcs->masterOptionPicked[groupMulti].size() - 1;
						}
						else
						{
							animMulti = stoi((*curptr->optioncondt)[2]);
						}

						if (a_multi)
						{
							animMulti = 0;
							size_t a_size = (*prcs->masterOptionPicked)[groupMulti].size();

							while (animMulti < a_size)
							{
								result = curptr->isNot ? !(*prcs->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]] :
									(*prcs->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]];

								if (!result) break;

								++animMulti;
							}
						}
						else
						{
							result = curptr->isNot ? !(*prcs->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]] :
								(*prcs->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]];
						}

						++groupMulti;
					}
				}
			}
			else if ((*curptr->optioncondt)[2] == format)
			{
				result = true;
			}
			else
			{
				size_t animMulti;
				bool multi = false;

				if ((*curptr->optioncondt)[1].length() == 0)
				{
					prcs->animMulti == -1 || prcs->multiOption != format ? multi = true : animMulti = prcs->animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "order")
				{
					prcs->animMulti == -1 || prcs->multiOption != format ? animMulti = prcs->order : animMulti = prcs->animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "F")
				{
					animMulti = 0;
				}
				else if ((*curptr->optioncondt)[1] == "N")
				{
					animMulti = prcs->order;

					if (!prcs->curAnim->isLast()) ++animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "B")
				{
					animMulti = prcs->order;

					if (animMulti > 0) --animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "L")
				{
					animMulti = prcs->lastorder;
				}
				else
				{
					animMulti = stoi((*curptr->optioncondt)[1]);
				}

				if (multi)
				{
					animMulti = (*prcs->groupOptionPicked).size();

					for (size_t i = 0; i < animMulti; ++i)
					{
						result = curptr->isNot ? !(*prcs->groupOptionPicked)[i][(*curptr->optioncondt)[2]] :
							(*prcs->groupOptionPicked)[i][(*curptr->optioncondt)[2]];

						if (!result) break;
					}
				}
				else
				{
					result = curptr->isNot ? !(*prcs->groupOptionPicked)[animMulti][(*curptr->optioncondt)[2]] :
						(*prcs->groupOptionPicked)[animMulti][(*curptr->optioncondt)[2]];
				}
			}
		}
		else if (curptr->isOrder)
		{
			if (prcs->animMulti == -1 || prcs->multiOption != format)
			{
				if (curptr->isNot)
				{
					result = curptr->last ? !prcs->curAnim->isLast() : curptr->hiddenOrder != prcs->order;
				}
				else
				{
					result = curptr->last ? prcs->curAnim->isLast() : curptr->hiddenOrder == prcs->order;
				}
			}
			else
			{
				if (curptr->isNot)
				{
					result = curptr->last ? !prcs->curAnim->isLast() : curptr->hiddenOrder != prcs->animMulti;
				}
				else
				{
					result = curptr->last ? prcs->curAnim->isLast() : curptr->hiddenOrder == prcs->animMulti;
				}
			}
		}
		else
		{
			result = curptr->specialIsTrueA();
		}

		if (!curptr->next) return result;

		if (curptr->OR)
		{
			if (result) return result;

			curptr = curptr->next;
		}
		else
		{
			if (!result) return result;

			curptr = curptr->next;
		}
	}
}

bool nemesis::Condt::isMultiTrue(proc* process, string format, string behaviorFile, int numline, int& animMulti, bool isGroup, bool isMaster, int& groupMulti)
{
	if (isGroup)
	{
		if (optioncondt && optioncondt->size() > 3)
		{
			if (original == format + "_group")
			{
				groupMulti = -1;
				return true;
			}

			groupMulti = process->groupMulti;

			if (groupMulti == -1) ErrorMessage(1202, format + "_master", behaviorFile, numline, original);

			if ((*optioncondt)[2].length() == 0)
			{
				animMulti = -1;
				return true;
			}
			else if ((*optioncondt)[2] == "F")
			{
				animMulti = 0;
			}
			else if ((*optioncondt)[2] == "L")
			{
				animMulti = (*process->masterOptionPicked)[groupMulti].size() - 1;
			}
			else
			{
				animMulti = stoi((*optioncondt)[2]);

				if (animMulti >= int((*process->masterOptionPicked)[groupMulti].size())) ErrorMessage(1148, format + "_group", behaviorFile, numline, original);
			}

			return (*process->masterOptionPicked)[groupMulti][animMulti][(*optioncondt)[2]];
		}
		else
		{
			ErrorMessage(1138, format + "_group", behaviorFile, numline, original);
		}
	}
	else
	{
		if (optioncondt && optioncondt->size() > 2)
		{
			if ((*optioncondt)[1].length() == 0)
			{
				ErrorMessage(1057, format, behaviorFile, numline, original);
			}
			else if ((*optioncondt)[1] == "order")
			{
				animMulti = process->order;
			}
			else if ((*optioncondt)[1] == "F")
			{
				animMulti = 0;
			}
			else if ((*optioncondt)[1] == "N")
			{
				animMulti = process->order;

				if (!process->curAnim->isLast()) ++animMulti;
			}
			else if ((*optioncondt)[1] == "B")
			{
				animMulti = process->order;

				if (animMulti > 0) --animMulti;
			}
			else if ((*optioncondt)[1] == "L")
			{
				animMulti = process->lastorder;
			}
			else
			{
				animMulti = stoi((*optioncondt)[1]);
			}

			if (original == format)
			{
				animMulti = -1;
				return true;
			}

			return (*process->groupOptionPicked)[animMulti][(*optioncondt)[2]];
		}
		else
		{
			ErrorMessage(1138, format, behaviorFile, numline, original);
		}
	}

	return false;
}

bool nemesis::Condt::specialIsTrueA()
{
	int animMulti = prcs->animMulti;
	int optionMulti = prcs->optionMulti;
	bool result = specialIsTrueB();
	prcs->animMulti = animMulti;
	prcs->optionMulti = optionMulti;
	return result;
}

bool nemesis::Condt::specialIsTrueB()
{
	if (cmp1.size() > 0 || cmp2.size() > 0)
	{
		set<int> animMulti1_list;
		set<int> animMulti2_list;

		if (cmpbool1)
		{
			for (unsigned int i = 0; i < prcs->curAnim->GetGroupAnimInfo().size(); ++i)
			{
				animMulti1_list.insert(i);
			}
		}
		else
		{
			animMulti1_list.insert(0);
		}

		if (cmpbool2)
		{
			for (unsigned int i = 0; i < prcs->curAnim->GetGroupAnimInfo().size(); ++i)
			{
				animMulti2_list.insert(i);
			}
		}
		else
		{
			animMulti2_list.insert(0);
		}

		unordered_map<int, unordered_map<int, string>> history1;
		unordered_map<int, unordered_map<int, string>> history2;

		for (auto& animMulti1 : animMulti1_list)
		{
			for (auto& animMulti2 : animMulti2_list)
			{
				int optionMulti1 = -1;
				int optionMulti2 = -1;
				int endMulti1 = -1;
				int endMulti2 = -1;
				bool out = false;

				if (cmp1.size() > 0 && cmpinfo1.size() > 0)
				{
					for (auto& addinfo : cmpinfo1)
					{
						if (addinfo.optionMulti == -1)
						{
							try
							{
								GetMultiFromAddOn(*this, addinfo, original, animMulti1, optionMulti1, endMulti1);
							}
							catch (bool)
							{
								break;
							}
						}
						else
						{
							if (optionMulti1 > addinfo.optionMulti || optionMulti1 == -1)
							{
								optionMulti1 = addinfo.optionMulti;
							}

							if (endMulti1 < addinfo.optionMulti + 1)
							{
								endMulti1 = addinfo.optionMulti + 1;
							}
						}
					}
				}
				else
				{
					out = true;
					optionMulti1 = 0;
					endMulti1 = 1;
				}

				if (cmp2.size() > 0 && cmpinfo2.size() > 0)
				{
					for (auto& addinfo : cmpinfo2)
					{
						if (addinfo.optionMulti == -1)
						{
							try
							{
								GetMultiFromAddOn(*this,  addinfo, original, animMulti2, optionMulti2, endMulti2);
							}
							catch (bool)
							{
								break;
							}
						}
						else
						{
							if (optionMulti2 > addinfo.optionMulti || optionMulti2 == -1)
							{
								optionMulti2 = addinfo.optionMulti;
							}

							if (endMulti2 < addinfo.optionMulti + 1)
							{
								endMulti2 = addinfo.optionMulti + 1;
							}
						}
					}
				}
				else
				{
					out = true;
					optionMulti2 = 0;
					endMulti2 = 1;
				}

				if (error) throw nemesis::exception();

				if (!out && (optionMulti1 != optionMulti2 || endMulti1 != endMulti2))
				{
					if (!isNot)
					{
						if (OR && next)
						{
							return next->isTrue(*this, next);
						}

						return false;
					}
					else if (oneTime)
					{
						if (!OR && next)
						{
							return next->isTrue(*this, next);
						}

						return true;
					}
				}
				else
				{
					unordered_set<string> condlist1;
					unordered_set<string> condlist2;
					endMulti1 = max(endMulti1, endMulti2);

					while (optionMulti1 < endMulti1)
					{
						string condition1;
						string condition2;
						prcs->optionMulti = optionMulti1;

						if (cmp1.size() != 0)
						{
							if (history1[animMulti1][optionMulti1].length() == 0)
							{
								vecstr lines = cmp1;
								prcs->animMulti = animMulti1;

								for (auto& blocklist : cmp1_block)
								{
									for (auto& blok : blocklist.second)
									{
										(prcs->*blok->func)(*blok, lines);
									}
								}

								for (auto& each : lines)
								{
									condition1.append(each);
								}

								history1[animMulti1][optionMulti1] = condition1;
							}
							else
							{
								condition1 = history1[animMulti1][optionMulti1];
							}

							if (error) throw nemesis::exception();

							if (cmp1.size() == 0)
							{
								ErrorMessage(1172, fmt, bhvfile, nm);
							}
						}

						if (cmp2.size() != 0)
						{
							if (history2[animMulti2][optionMulti1].length() == 0)
							{
								vecstr lines = cmp2;
								prcs->animMulti = animMulti2;

								for (auto& blocklist : cmp2_block)
								{
									for (auto& blok : blocklist.second)
									{
										(prcs->*blok->func)(*blok, lines);
									}
								}

								for (auto& each : lines)
								{
									condition2.append(each);
								}

								history2[animMulti2][optionMulti1] = condition2;
							}
							else
							{
								condition2 = history2[animMulti2][optionMulti1];
							}

							if (cmp2.size() == 0)
							{
								ErrorMessage(1172, fmt, bhvfile, nm);
							}
						}

						condlist1.insert(condition1);
						condlist2.insert(condition2);

						if (error) throw nemesis::exception();

						++optionMulti1;
					}

					// NOT EUQAL
					if (condlist1 != condlist2)
					{
						// BUT looking for EQUAL
						if (!isNot)
						{
							// proceed to next OR condition
							if (OR && next)
							{
								return next->isTrue(*this, next);
							}

							// FALSE because AND
							return false;
						}

						// IS NOT EQUAL 1 time only?
						else if (oneTime)
						{
							// proceed to next AND condition
							if (!OR && next)
							{
								return next->isTrue(*this, next);
							}

							// TRUE because OR
							return true;
						}
					}

					// EQUAL
					else
					{
						// BUT looking for NOT EQUAL
						if (isNot)
						{
							// proceed to next OR condition
							if (OR && next)
							{
								return next->isTrue(*this, next);
							}

							// FALSE because AND
							return false;
						}

						// IS EQUAL 1 time only?
						else if (oneTime)
						{
							// proceed to next AND condition
							if (!OR && next)
							{
								return next->isTrue(*this, next);
							}

							// TRUE because OR
							return true;
						}
					}
				}

				if (error) throw nemesis::exception();
			}
		}
	}

	return true;
}

void nemesis::Condt::specialCondition(string condition, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster, OptionList& optionlist)
{
	size_t pos;

	if (condition.find("!=") != NOT_FOUND)
	{
		if (condition.find("==") != NOT_FOUND || sameWordCount(condition, "!=") > 1)
		{
			ErrorMessage(1124, format, behaviorFile, numline, original);
		}

		isNot = true;
		pos = condition.find("!=");
	}
	else if (condition.find("==") != NOT_FOUND)
	{
		if (condition.find("!=") != NOT_FOUND || sameWordCount(condition, "==") > 1)
		{
			ErrorMessage(1124, format, behaviorFile, numline, original);
		}

		pos = condition.find("==");
	}
	else
	{
		ErrorMessage(1125, format, behaviorFile, numline, original);
	}

	string oriCondition1 = condition.substr(1, pos - 1);
	string oriCondition2 = condition.substr(pos + 2);

	// <optionA != optionB>*
	// only need to fulfill the condition once
	if (oriCondition2.length() > 0)
	{
		oriCondition2.pop_back();

		if (condition.back() == '*' && oriCondition2.length() > 0)
		{
			oneTime = true;
			oriCondition2.pop_back();
		}
	}

	if (error) throw nemesis::exception();

	if (oriCondition1.length() > 0)
	{
		for (auto& ch : oriCondition1)
		{
			cmp1.push_back(string(1, ch));
		}

		bool isEnd = false;
		ProcessFunction(oriCondition1, oriCondition1, format, behaviorFile, multiOption, isEnd, numline, 0, optionlist, cmp1_block, cmpinfo1, cmpbool1);
	}

	if (oriCondition2.length() > 0)
	{
		for (auto& ch : oriCondition2)
		{
			cmp2.push_back(string(1, ch));
		}

		bool isEnd = false;
		ProcessFunction(oriCondition2, oriCondition2, format, behaviorFile, multiOption, isEnd, numline, 0, optionlist, cmp2_block, cmpinfo2, cmpbool2);
	}

	if (error) throw nemesis::exception();
}

void nemesis::Condt::conditionProcess(string condition, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster)
{
	if (condition[0] == '^' && condition.back() == '^')
	{
		string conditionOrder;

		if (isalpha(condition[1]))
		{
			conditionOrder = boost::regex_replace(string(condition), boost::regex("\\^([A-Za-z]+)\\^"), string("\\1"));

			if (nemesis::iequals(conditionOrder, "last"))
			{
				last = true;
			}
			else if (nemesis::iequals(conditionOrder, "first"))
			{
				hiddenOrder = 0;
			}
			else
			{
				isGroup ? ErrorMessage(1138, format + "_group", behaviorFile, numline, condition) : ErrorMessage(1138, format, behaviorFile, numline, condition);
			}
		}
		else
		{
			conditionOrder = condition.substr(1, condition.length() - 2);

			if (!isOnlyNumber(conditionOrder))
			{
				isGroup ? ErrorMessage(1138, format + "_group", behaviorFile, numline, condition) : ErrorMessage(1138, format, behaviorFile, numline, condition);
			}

			hiddenOrder = stoi(conditionOrder);
		}

		isOrder = true;
	}
	else
	{
		optioncondt = isGroup ? (isMaster ? make_shared<vecstr>(GetOptionInfo(condition, format + "_master", format, behaviorFile, multiOption, numline)) :
			make_shared<vecstr>(GetOptionInfo(condition, format + "_group", format, behaviorFile, multiOption, numline))) :
			make_shared<vecstr>(GetOptionInfo(condition, format, behaviorFile, numline));
	}

	if (error) throw nemesis::exception();
}

nemesis::MultiChoice::MultiChoice(string cond, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster, OptionList& optionlist,
	size_t posA, size_t posB)
{
	if (cond.length() > 0)
	{
		condition = make_shared<nemesis::Condt>(cond, format, behaviorFile, cond, multiOption, numline, isGroup, isMaster, optionlist);
	}
	else
	{
		condition = make_shared<nemesis::Condt>(format, format, behaviorFile, cond, multiOption, numline, isGroup, isMaster, optionlist);
	}

	locateA = posA;
	locateB = posB - 1;
}

void nemesis::Condt::singleCondition(string condition, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster, OptionList& optionlist)
{
	if (condition.find("<") == 0 && (condition.find(">") == condition.length() - 1 || condition.find(">*") == condition.length() - 2) &&
		(condition.find("!=") != NOT_FOUND || condition.find("==") != NOT_FOUND))
	{
		specialCondition(condition, format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
	}
	else
	{
		string conditionOrder = condition;

		if (conditionOrder[0] == '!')
		{
			isNot = true;
			conditionOrder = conditionOrder.substr(1);
		}

		conditionProcess(conditionOrder, format, behaviorFile, multiOption, numline, isGroup, isMaster);
	}
}

vecstr GetOptionInfo(string line, string format, string filename, int numline)
{
	vecstr optionInfo;
	optionInfo.reserve(3);
	optionInfo.push_back(format);

	if (line.find(format + "[") != NOT_FOUND)
	{
		if (sameWordCount(line, format + "[") > 1)
		{
			ErrorMessage(1157, format, filename, numline, line);
		}

		string templine = line;
		size_t pos = templine.find(format + "[");
		templine = templine.substr(templine.find("[", pos) + 1);

		while (true)
		{
			pos = templine.find("]");
			optionInfo.push_back(templine.substr(0, pos));
			size_t optionLength = optionInfo.back().length() + 1;

			if (templine.length() > optionLength && templine[optionLength] == '[') templine = templine.substr(templine.find("[") + 1);
			else break;
		}
	}
	else
	{
		optionInfo.push_back("order");
		optionInfo.push_back(line);
	}

	if (error) throw nemesis::exception();

	return optionInfo;
}

vecstr GetOptionInfo(string line, string format, string masterformat, string filename, string multiOption, int numline)
{
	vecstr optionInfo;
	optionInfo.reserve(5);
	optionInfo.push_back(masterformat + "_group");

	if (line.find(masterformat + "_group[") != NOT_FOUND)
	{
		if (sameWordCount(line, masterformat + "_group[") > 1) ErrorMessage(1157, format, filename, numline, line);

		string templine = line;
		size_t pos = templine.find(masterformat + "_group[");
		templine = templine.substr(templine.find("[", pos) + 1);

		while (true)
		{
			pos = templine.find("]");
			optionInfo.push_back(templine.substr(0, pos));
			size_t optionLength = optionInfo.back().length() + 1;

			if (templine.length() > optionLength && templine[optionLength] == '[')
			{
				templine = templine.substr(templine.find("[") + 1);
			}
			else
			{
				break;
			}
		}

		if (optionInfo.size() < 2) ErrorMessage(1192, format, filename, numline, line);
		else if (optionInfo[1].length() != 0) ErrorMessage(1201, format, filename, numline, line);
	}
	else if (line.find(masterformat + "[") != NOT_FOUND)
	{
		if (sameWordCount(line, masterformat + "[") > 1) ErrorMessage(1157, format, filename, numline, line);

		if (format != masterformat + "_group" && multiOption != masterformat + "_group") ErrorMessage(1202, format, filename, numline, line);

		optionInfo.push_back("");

		string templine = line;
		size_t pos = templine.find(masterformat + "[");
		templine = templine.substr(templine.find("[", pos) + 1);

		while (true)
		{
			pos = templine.find("]");
			optionInfo.push_back(templine.substr(0, pos));
			size_t optionLength = optionInfo.back().length() + 1;

			if (templine.length() > optionLength && templine[optionLength] == '[') templine = templine.substr(templine.find("[") + 1);
			else break;
		}
	}
	else if (line == masterformat || line == masterformat + "_group" || multiOption == masterformat)
	{
		optionInfo.push_back("");
		optionInfo.push_back("");
		optionInfo.push_back(line);
	}
	else
	{
		ErrorMessage(1192, format, filename, numline, line);
	}

	if (error) throw nemesis::exception();

	return optionInfo;
}

void GetMultiFromAddOn(const nemesis::Condt& curcond, const AddOnInfo& addinfo, const string& original, int animMulti, int& optionMulti, int& endMulti)
{
	auto& groupAnimInfo = curcond.prcs->curAnim->GetGroupAnimInfo();

	if (addinfo.header.find("@AnimObject/") != NOT_FOUND)
	{
		optionMulti = 0;
		endMulti = groupAnimInfo[animMulti]->optionPickedCount[addinfo.header.substr(1)];
	}
	else
	{
		unordered_map<string, vecstr>* addOnPtr = &groupAnimInfo[animMulti]->addOn;

		if (addOnPtr->find(addinfo.header) != addOnPtr->end() && (*addOnPtr)[addinfo.header].size() > 0)
		{
			unordered_map<string, vecstr>* groupAdditionPtr = &groupAnimInfo[animMulti]->groupAddition[addinfo.header];

			if (groupAdditionPtr->find(addinfo.addition) != groupAdditionPtr->end() && (*groupAdditionPtr)[addinfo.addition].size() > 0)
			{
				if (addinfo.optionMulti == -1)
				{
					optionMulti = 0;
					endMulti = (*groupAdditionPtr)[addinfo.addition].size();
					throw false;
				}
				else if (addinfo.optionMulti < int((*groupAdditionPtr)[addinfo.addition].size()))
				{
					optionMulti = addinfo.optionMulti;
					endMulti = optionMulti + 1;
				}
				else
				{
					curcond.ig ? ErrorMessage(1148, curcond.fmt + "_group", curcond.bhvfile, curcond.nm, original)
						: ErrorMessage(1148, curcond.fmt, curcond.bhvfile, curcond.nm, original);
				}
			}
		}
	}
}
