#include <boost/regex.hpp>

#include "utilities/stringsplit.h"

#include "generate/animation/newanimation.h"
#include "generate/animation/singletemplate.h"

using namespace std;

void stateInstall(string line, string change, string format, string behaviorFile, int numline, size_t curPos, string animOrder, bool isMC,
	map<int, vector<range>>& lineblocks, proc& process, void (proc::*func)(range, vecstr&));
void mainAnimEventInstall(string format, string behaviorFile, string change, int numline, size_t curPos, boost::regex expr, bool isGroup, bool isMaster, proc& process);
void ProcessFunction(string change, string line, string format, string behaviorFile, string multiOption, bool& isEnd, int numline, size_t curPos,
	OptionList& optionlist, map<int, vector<shared_ptr<range>>>& lineblocks, vector<addOnInfo>& addInfo, bool& isTrueMulti, bool isGroup = false, bool isMaster = false,
	bool isMC = true, proc& process = proc());
void GetMultiFromAddOn(addOnInfo& addinfo, string format, string behaviorFile, string original, int numline, int animMulti, bool isGroup,
	vector<shared_ptr<AnimationInfo>>& groupAnimInfo, int& optionMulti, int& endMulti);

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

void AnimTemplate::ExamineTemplate(string n_format, string n_file, vecstr templatelines, bool isGroup, bool isMaster, OptionList optionlist)
{
	bool isCore = optionlist.core;
	bool isEnd = false;
	bool open = false;
	bool multi = false;
	bool newOpen = false;
	bool norElement = false;
	bool negative = false;
	int openOrder = -2;
	int condition = 0;
	int openRange = 0;
	format = n_format;
	behaviorFile = n_file;
	string elementline;
	unordered_map<int, vecstr> conditionStore;
	unordered_map<int, string> openConditions;
	vector<condset*> generatedlines;
	generatedlines.push_back(&lines);

	if (isCore)
	{
		for (unsigned int i = 0; i < templatelines.size(); ++i)
		{
			bool uniqueskip = false;
			string line = templatelines[i];
			Process(line, "", norElement, isEnd, isGroup, isMaster, openRange, i + 1, optionlist, generatedlines.back());

			if (error) throw nemesis::exception();
		}
	}
	else
	{
		for (unsigned int i = 0; i < templatelines.size(); ++i)
		{
			bool uniqueskip = false;
			string line = templatelines[i];
			nemesis::smatch match;

			if (i == 216)
				line = line;

			if (nemesis::regex_search(line, match, boost::regex(".*<!-- CONDITION START \\^(.+?)\\^ -->.*")))
			{
				condition++;
				string multiOption;

				for (auto& it = generatedlines.rbegin(); it != generatedlines.rend(); ++it)
					if ((*it)->isMulti) multiOption = (*it)->conditions;

				generatedlines.back()->lines.push_back(stackline());
				generatedlines.back()->lines.back().nestedcond.push_back(condset());
				generatedlines.push_back(&generatedlines.back()->lines.back().nestedcond.back());
				generatedlines.back()->conditions = match[1];
				generatedlines.back()->n_conditions = make_shared<condt>(match[1], format, behaviorFile, match[1], multiOption, i + 1, isGroup, isMaster, optionlist);
				generatedlines.back()->linenum = i + 1;
				uniqueskip = true;
			}
			else if (nemesis::regex_search(line, match, boost::regex(".*<!-- CONDITION \\^(.+?)\\^ -->.*")))
			{
				if (condition == 0)
				{
					ErrorMessage(1119, format, behaviorFile, i + 1);
				}

				string multiOption;

				for (auto& it = generatedlines.rbegin(); it != generatedlines.rend(); ++it)
					if ((*it)->isMulti) multiOption = (*it)->conditions;

				generatedlines.pop_back();
				generatedlines.back()->lines.back().nestedcond.push_back(condset());
				generatedlines.push_back(&generatedlines.back()->lines.back().nestedcond.back());
				generatedlines.back()->conditions = match[1];
				generatedlines.back()->n_conditions = make_shared<condt>(match[1], format, behaviorFile, match[1], multiOption, i + 1, isGroup, isMaster, optionlist);
				generatedlines.back()->linenum = i + 1;
				uniqueskip = true;
			}
			else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
			{
				if (condition == 0)
				{
					ErrorMessage(1119, format, behaviorFile, i + 1);
				}

				generatedlines.pop_back();
				generatedlines.back()->lines.back().nestedcond.push_back(condset());
				generatedlines.push_back(&generatedlines.back()->lines.back().nestedcond.back());
				uniqueskip = true;
			}
			else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
			{
				if (newOpen)
				{
					ErrorMessage(1116, format, behaviorFile, i + 1);
				}

				string multiOption;

				for (auto& it = generatedlines.rbegin(); it != generatedlines.rend(); ++it)
					if ((*it)->isMulti) multiOption = (*it)->conditions;

				generatedlines.back()->lines.push_back(stackline());
				generatedlines.back()->lines.back().nestedcond.push_back(condset());
				generatedlines.push_back(&generatedlines.back()->lines.back().nestedcond.back());
				generatedlines.back()->conditions = getOption(line);
				generatedlines.back()->n_conditions = make_shared<condt>(generatedlines.back()->conditions, format, behaviorFile, generatedlines.back()->conditions,
					multiOption, i + 1, isGroup, isMaster, optionlist);
				generatedlines.back()->linenum = i + 1;
				newOpen = true;
				uniqueskip = true;
			}
			else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
			{
				if (newOpen) ErrorMessage(1116, format, behaviorFile, i + 1);

				string multiOption;

				for (auto& it = generatedlines.rbegin(); it != generatedlines.rend(); ++it)
					if ((*it)->isMulti) multiOption = (*it)->conditions;

				generatedlines.back()->lines.push_back(stackline());
				generatedlines.back()->lines.back().nestedcond.push_back(condset());
				generatedlines.push_back(&generatedlines.back()->lines.back().nestedcond.back());
				generatedlines.back()->conditions = getOption(line);

				if (isMaster && generatedlines.back()->conditions == format && multiOption != format + "_group") ErrorMessage(1202, format, behaviorFile, i + 1);

				generatedlines.back()->n_conditions = make_shared<condt>(generatedlines.back()->conditions, format, behaviorFile, generatedlines.back()->conditions,
					multiOption, i + 1, isGroup, isMaster, optionlist);
				generatedlines.back()->linenum = i + 1;
				generatedlines.back()->isMulti = true;
				newOpen = true;
				uniqueskip = true;
			}
			else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
			{
				if (isMaster) ErrorMessage(1202, format, behaviorFile, i + 1);

				if (newOpen) ErrorMessage(1116, format, behaviorFile, i + 1);

				string multiOption;

				for (auto& it = generatedlines.rbegin(); it != generatedlines.rend(); ++it)
					if ((*it)->isMulti) multiOption = (*it)->conditions;

				generatedlines.back()->lines.push_back(stackline());
				generatedlines.back()->lines.back().nestedcond.push_back(condset());
				generatedlines.push_back(&generatedlines.back()->lines.back().nestedcond.back());
				generatedlines.back()->conditions = boost::regex_replace(string(line), boost::regex(".*<!-- NEW ORDER (.+?) -->.*"), string("\\1"));
				generatedlines.back()->n_conditions = make_shared<condt>(generatedlines.back()->conditions, format, behaviorFile, generatedlines.back()->conditions,
					multiOption, i + 1, isGroup, isMaster, optionlist);
				generatedlines.back()->linenum = i + 1;
				generatedlines.back()->isOrder = true;
				newOpen = true;
				uniqueskip = true;
			}
			else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
			{
				if (!newOpen) ErrorMessage(1171, format, behaviorFile, i + 1);

				generatedlines.pop_back();
				newOpen = false;
				uniqueskip = true;
			}
			else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
			{
				--condition;
				generatedlines.pop_back();
				uniqueskip = true;
			}

			if (!uniqueskip)
			{
				string multiOption;

				for (auto& it = generatedlines.rbegin(); it != generatedlines.rend(); ++it)
					if ((*it)->isMulti) multiOption = (*it)->conditions;

				Process(line, multiOption, norElement, isEnd, isGroup, isMaster, openRange, i + 1, optionlist, generatedlines.back());
			}

			if (error) throw nemesis::exception();
		}
	}

	if (generatedlines.size() != 1)
	{
		ErrorMessage(1122, format, behaviorFile);
	}

	if (newOpen)
	{
		ErrorMessage(1116, format, behaviorFile, templatelines.size() + 1);
	}

	if (condition > 0)
	{
		ErrorMessage(1145, format, behaviorFile);
	}

	size = templatelines.size();
}

void AnimTemplate::Process(string& line, string multiOption, bool& norElement, bool& isEnd, bool isGroup, bool isMaster, int& openRange, int numline,
	OptionList& optionlist, condset* generatedlines)
{
	bool hasProcess = false;

	// compute numelements
	if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND &&
		line.find("<!-- COMPUTE -->", line.find("numelements=\"")) != NOT_FOUND)
	{
		if (!norElement)
		{
			norElement = true;
			hasProcess = true;
			string templine = line.substr(0, line.find("<hkparam name=\"", 0));
			openRange = count(templine.begin(), templine.end(), '\t');
			process.installBlock(range(0, 0, vector<int>{ openRange }, &proc::compute), numline);
		}
		else
		{
			ErrorMessage(1136, format, behaviorFile, numline);
		}
	}
	else if (norElement && line.find("</hkparam>") != NOT_FOUND)
	{
		string templine = line.substr(0, line.find("</hkparam>"));

		if (openRange == count(templine.begin(), templine.end(), '\t'))
		{
			hasProcess = true;
			norElement = false;
			process.installBlock(range(0, 0, &proc::rangeCompute), numline);
		}
	}

	if (norElement)
	{
		string templine = line;

		if (templine.find("<hkobject>") != NOT_FOUND)
		{
			templine = templine.substr(0, templine.find("<hkobject>"));

			if (openRange + 1 == count(templine.begin(), templine.end(), '\t'))
			{
				hasProcess = true;
				process.installBlock(range(0, 99999, &proc::upCounter), numline);
			}
		}
		else if (templine.find("\t\t\t#") != NOT_FOUND)
		{
			templine = templine.substr(0, templine.find("#", 0));

			if (openRange + 1 == count(templine.begin(), templine.end(), '\t'))
			{
				hasProcess = true;
				process.installBlock(range(0, 99999, &proc::upCounterPlus), numline);
			}
		}
	}

	if (line.find("$") != NOT_FOUND)
	{
		boost::regex exp("(?<!MID)(?<!\\$MC)(?<!" + format + "_master)(?<!" + format +
			"_group)(?<!\\$%)\\$(?!%\\$)(?!MC\\$)(?!elements\\$)(.+?)(?<!MID)(?<!\\$MC)(?<!" + format + "_master)(?<!" + format +
			"_group)(?<!\\$%)\\$(?!%\\$)(?!MC\\$)(?!elements\\$)");

		for (boost::sregex_iterator itr(line.begin(), line.end(), exp); itr != boost::sregex_iterator(); ++itr)
		{
			bool isChange = false;
			string change = itr->str(1);
			size_t curPos = itr->position();
			process.brackets[numline].push_back(itr->position());
			process.brackets[numline].push_back(itr->position() + itr->str().length() - 1);
			map<int, vector<shared_ptr<range>>> dummy1;
			vector<addOnInfo> dummy2;
			bool dummy3;
			ProcessFunction(change, line, format, behaviorFile, multiOption, isEnd, numline, curPos + 1, optionlist, dummy1, dummy2, dummy3, isGroup, isMaster, false,
				process);
			hasProcess = true;
		}

		size_t pos = line.find("$%$");

		if (pos != NOT_FOUND)
		{
			while (pos != NOT_FOUND)
			{
				process.installBlock(range(pos, pos + 3, &proc::animCount), numline);
				pos = line.find("$%$", pos + 1);
			}

			hasProcess = true;
		}

		pos = line.find("$MC$");

		// multi choice selection
		if (pos != NOT_FOUND)
		{
			vector<multichoice> m_conditions;
			process.hasMC[numline] = true;

			for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("[\\s]+<!-- (.+?) -->[\\s]*?"));
				itr != boost::sregex_iterator(); ++itr)
			{
				string output = itr->str(1);
				pos = itr->position(1);
				vecstr curElements;
				StringSplit(output, curElements);

				if (curElements.size() == 1)
				{
					m_conditions.push_back(multichoice("", format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist, pos, pos + output.length()));
				}
				else if (curElements.size() > 1)
				{
					pos = pos + output.length();
					m_conditions.push_back(multichoice(curElements[0], format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist,
						pos - curElements.back().length(), pos));
				}
			}

			for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("\\$MC\\$")); itr != boost::sregex_iterator(); ++itr)
			{
				pos = itr->position();
				process.installBlock(range(pos, pos + itr->str().length(), &proc::multiChoiceRegis), numline, m_conditions);
				hasProcess = true;
			}
		}

		// get group node ID
		if (isGroup)
		{
			for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex(format + "\\$([0-9]+)"));
				itr != boost::sregex_iterator(); ++itr)
			{
				string ID = itr->str(1);
				pos = itr->position();
				process.installBlock(range(pos, pos + itr->str().length(), vecstr{ ID, line }, &proc::IDRegisAnim), numline);
				hasProcess = true;
			}

			for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex(format + "_group\\$([0-9]+)"));
				itr != boost::sregex_iterator(); ++itr)
			{
				string ID = itr->str(1);
				pos = itr->position();
				process.installBlock(range(pos, pos + itr->str().length(), vecstr{ ID, line }, &proc::IDRegisGroup), numline);
				hasProcess = true;
			}
		}
		else
		{
			for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex(format + "_group\\$([0-9]+)"));
				itr != boost::sregex_iterator(); ++itr)
			{
				string ID = itr->str(1);
				pos = itr->position();
				process.installBlock(range(pos, pos + itr->str().length(), vecstr{ ID }, &proc::groupIDRegis), numline);
				hasProcess = true;
			}
		}

		pos = line.find("MID$");

		// set function ID
		if (pos != NOT_FOUND)
		{
			void (proc::*func)(range, vecstr&);

			if (isMaster) func = &proc::IDRegisMaster;
			else if (isGroup) func = &proc::IDRegisGroup;
			else func = &proc::IDRegis;

			for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("MID\\$([0-9]+)")); itr != boost::sregex_iterator(); ++itr)
			{
				pos = itr->position();
				process.installBlock(range(pos, pos + itr->str().length(), vecstr{ itr->str(1), line }, func), numline);
				hasProcess = true;
			}
		}
	}

	if (isEnd)
	{
		for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("<hkparam name\\=\"relativeToEndOfClip\">(.+?)<\\/hkparam>"));
			itr != boost::sregex_iterator(); ++itr)
		{
			isEnd = false;
			hasProcess = true;
			size_t pos = itr->position(1);
			process.installBlock(range(pos, pos + itr->str(1).length(), &proc::relativeNegative), numline);
		}

		for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("<hkparam name\\=\"localTime\">(.+?)<\\/hkparam>"));
			itr != boost::sregex_iterator(); ++itr)
		{
			hasProcess = true;
			size_t pos = itr->position(1);
			process.installBlock(range(pos, pos + itr->str(1).length(), &proc::localNegative), numline);
		}
	}

	for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("<hkparam name\\=\"animationName\">(.+?)<\\/hkparam>"));
		itr != boost::sregex_iterator(); ++itr)
	{
		hasProcess = true;
		size_t pos = itr->position(1);
		process.installBlock(range(pos, pos + itr->str(1).length(), &proc::regisAnim), numline);
	}

	for (auto& itr = boost::sregex_iterator(line.begin(), line.end(), boost::regex("<hkparam name\\=\"behaviorName\">(.+?)<\\/hkparam>"));
		itr != boost::sregex_iterator(); ++itr)
	{
		hasProcess = true;
		size_t pos = itr->position(1);
		process.installBlock(range(pos, pos + itr->str(1).length(), &proc::regisBehavior), numline);
	}

	generatedlines->lines.push_back(stackline(line, hasProcess, numline));
}

stackline::stackline(string curline, bool process, size_t num)
{
	lineblocks.reserve(curline.length());

	if (process)
	{
		for (auto& ch : curline)
		{
			lineblocks.push_back(string(1, ch));
		}
	}
	else
	{
		line = curline;
	}

	hasProcess = process;
	linecount = num;
}

addOnInfo::addOnInfo(string n_h, string n_a)
{
	header = n_h;
	addition = n_a;
}

addOnInfo::addOnInfo(string n_h, string n_a, int n_om)
{
	header = n_h;
	addition = n_a;
	optionMulti = n_om;
}

string getOption(string curline)
{
	return boost::regex_replace(string(curline), boost::regex(".*<!-- (?:FOREACH|NEW) \\^(.+?)\\^ -->.*"), string("\\1"));
}

void stateInstall(string line, string change, string format, string behaviorFile, int numline, size_t curPos, string animOrder, bool isMC,
	map<int, vector<shared_ptr<range>>>& lineblocks, proc& process, void (proc::*func)(range, vecstr&))
{
	int intID;
	boost::regex expr(format + "\\[" + animOrder + "\\]\\[\\(S([0-9]*)\\+([0-9]+)\\)\\]");

	for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), expr); itr != boost::sregex_iterator(); ++itr)
	{
		string ID = itr->str(1);
		string number = itr->str(2);
		size_t post = curPos + itr->position();

		if (!isOnlyNumber(number))
		{
			ErrorMessage(1152, format, behaviorFile, numline, itr->str());
		}

		if (ID.length() == 0)
		{
			intID = 0;
		}
		else
		{
			intID = stoi(ID) - 1;
		}

		if (format + "[" + animOrder + "][(S" + ID + "+" + number + ")]" == line.substr(post, itr->str().length()))
		{
			if (animOrder.length() > 0 && isOnlyNumber(animOrder))
			{
				string full = itr->str();
				range blok(post, post + full.length(), vector<int>{ intID, stoi(number), stoi(animOrder) }, vecstr{ full }, func);

				if (isMC)
				{
					lineblocks[blok.size].push_back(make_shared<range>(blok));
				}
				else
				{
					process.installBlock(blok, numline);
				}

			}
			else
			{
				range blok(post, post + itr->str().length(), vector<int>{ intID, stoi(number) }, func);

				if (isMC)
				{
					lineblocks[blok.size].push_back(make_shared<range>(blok));
				}
				else
				{
					process.installBlock(blok, numline);
				}
			}
		}
		else
		{
			ErrorMessage(1137, format, behaviorFile, numline, itr->str());
		}

		if (error) throw nemesis::exception();
	}
}

void mainAnimEventInstall(string format, string behaviorFile, string change, int numline, size_t curPos, boost::regex expr, bool isGroup, bool isMaster, proc& process)
{
	for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), expr); itr != boost::sregex_iterator(); ++itr)
	{
		bool num = false;
		string first = itr->str(1);
		size_t post = curPos + itr->position();
		void (proc::*func)(range, vecstr&);

		if (first.length() == 0)
		{
			func = isGroup ? &proc::MAEMultiMaster : &proc::MAEMultiGroup;
		}
		else if (first == "F")
		{
			func = isGroup ? &proc::MAEFirstMaster : &proc::MAEFirstGroup;
		}
		else if (first == "N")
		{
			if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, change);
			else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, change);
			func = &proc::MAENextGroup;
		}
		else if (first == "B")
		{
			if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, change);
			else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, change);
			func = &proc::MAEBackGroup;
		}
		else if (first == "L")
		{
			func = isGroup ? &proc::MAELastMaster : &proc::MAELastGroup;
		}
		else
		{ 
			num = true;
		}
		
		if (num)
		{
			isGroup ? process.installBlock(range(post, post + itr->str().length(), vector<int> { stoi(first) }, vecstr{ itr->str() }, &proc::MAENumMaster), numline) :
				process.installBlock(range(post, post + itr->str().length(), vector<int> { stoi(first) }, vecstr{ itr->str() }, &proc::MAENumGroup), numline);
		}
		else
		{
			isGroup ? process.installBlock(range(post, post + itr->str().length(), vecstr{ itr->str() }, func), numline) :
				process.installBlock(range(post, post + itr->str().length(), func), numline);
		}
	}
}

bool condt::isTrue(proc* process, string format, string behaviorFile, int numline, bool isGroup, bool isMaster, shared_ptr<condt> curptr)
{
	while (true)
	{
		bool result = false;

		if (curptr->nestedcond)
		{
			result = curptr->nestedcond->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, curptr->nestedcond);
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
						if (process->groupMulti == -1 || process->multiOption != format || process->multiOption == format + "_group")
						{
							g_multi = true;
							groupMulti = 0;
							groupSize = process->masterOptionPicked->size();
						}
						else
						{
							groupMulti = process->groupMulti;
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
							if (process->animMulti == -1 || process->multiOption != format)
							{
								a_multi = true;
							}
							else
							{
								animMulti = process->animMulti;
							}
						}
						else if ((*curptr->optioncondt)[2] == "F")
						{
							animMulti = 0;
						}
						else if ((*curptr->optioncondt)[2] == "L")
						{
							animMulti = process->masterOptionPicked[groupMulti].size() - 1;
						}
						else
						{
							animMulti = stoi((*curptr->optioncondt)[2]);
						}

						if (a_multi)
						{
							animMulti = 0;
							size_t a_size = (*process->masterOptionPicked)[groupMulti].size();

							while (animMulti < a_size)
							{
								result = curptr->isNot ? !(*process->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]] :
									(*process->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]];

								if (!result) break;

								++animMulti;
							}
						}
						else
						{
							result = curptr->isNot ? !(*process->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]] :
								(*process->masterOptionPicked)[groupMulti][animMulti][(*curptr->optioncondt)[3]];
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
					process->animMulti == -1 || process->multiOption != format ? multi = true : animMulti = process->animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "order")
				{
					process->animMulti == -1 || process->multiOption != format ? animMulti = process->order : animMulti = process->animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "F")
				{
					animMulti = 0;
				}
				else if ((*curptr->optioncondt)[1] == "N")
				{
					animMulti = process->order;

					if (!process->curAnim->isLast()) ++animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "B")
				{
					animMulti = process->order;

					if (animMulti > 0) --animMulti;
				}
				else if ((*curptr->optioncondt)[1] == "L")
				{
					animMulti = process->lastorder;
				}
				else
				{
					animMulti = stoi((*curptr->optioncondt)[1]);
				}

				if (multi)
				{
					animMulti = (*process->groupOptionPicked).size();

					for (size_t i = 0; i < animMulti; ++i)
					{
						result = curptr->isNot ? !(*process->groupOptionPicked)[i][(*curptr->optioncondt)[2]] :
							(*process->groupOptionPicked)[i][(*curptr->optioncondt)[2]];

						if (!result) break;
					}
				}
				else
				{
					result = curptr->isNot ? !(*process->groupOptionPicked)[animMulti][(*curptr->optioncondt)[2]] :
						(*process->groupOptionPicked)[animMulti][(*curptr->optioncondt)[2]];
				}
			}
		}
		else if (curptr->isOrder)
		{
			if (process->animMulti == -1 || process->multiOption != format)
			{
				if (curptr->isNot)
				{
					result = curptr->last ? !process->curAnim->isLast() : curptr->hiddenOrder != process->order;
				}
				else
				{
					result = curptr->last ? process->curAnim->isLast() : curptr->hiddenOrder == process->order;
				}
			}
			else
			{
				if (curptr->isNot)
				{
					result = curptr->last ? !process->curAnim->isLast() : curptr->hiddenOrder != process->animMulti;
				}
				else
				{
					result = curptr->last ? process->curAnim->isLast() : curptr->hiddenOrder == process->animMulti;
				}
			}
		}
		else
		{
			result = curptr->specialIsTrueA(process, format, behaviorFile, numline, isGroup, isMaster, process->curAnim->GetGroupAnimInfo());
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

bool condt::isMultiTrue(proc* process, string format, string behaviorFile, int numline, int& animMulti, bool isGroup, bool isMaster, int& groupMulti)
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

bool condt::specialIsTrueA(proc* process, string format, string behaviorFile, int numline, bool isGroup, bool isMaster, vector<shared_ptr<AnimationInfo>>& groupAnimInfo)
{
	int animMulti = process->animMulti;
	int optionMulti = process->optionMulti;
	bool result = specialIsTrueB(process, format, behaviorFile, numline, isGroup, isMaster, groupAnimInfo);
	process->animMulti = animMulti;
	process->optionMulti = optionMulti;
	return result;
}

bool condt::specialIsTrueB(proc* process, string format, string behaviorFile, int numline, bool isGroup, bool isMaster, vector<shared_ptr<AnimationInfo>>& groupAnimInfo)
{
	if (cmp1.size() > 0 || cmp2.size() > 0)
	{
		set<int> animMulti1_list;
		set<int> animMulti2_list;

		if (cmpbool1)
		{
			for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
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
			for (unsigned int i = 0; i < groupAnimInfo.size(); ++i)
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
								GetMultiFromAddOn(addinfo, format, behaviorFile, original, numline, animMulti1, isGroup, groupAnimInfo, optionMulti1, endMulti1);
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
								GetMultiFromAddOn(addinfo, format, behaviorFile, original, numline, animMulti2, isGroup, groupAnimInfo, optionMulti2, endMulti2);
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
							return next->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, next);
						}

						return false;
					}
					else if (oneTime)
					{
						if (!OR && next)
						{
							return next->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, next);
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
						process->optionMulti = optionMulti1;

						if (cmp1.size() != 0)
						{
							if (history1[animMulti1][optionMulti1].length() == 0)
							{
								vecstr lines = cmp1;
								process->animMulti = animMulti1;

								for (auto& blocklist : cmp1_block)
								{
									for (auto& blok : blocklist.second)
									{
										(process->*blok->func)(*blok, lines);
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
								ErrorMessage(1172, format, behaviorFile, numline);
							}
						}

						if (cmp2.size() != 0)
						{
							if (history2[animMulti2][optionMulti1].length() == 0)
							{
								vecstr lines = cmp2;
								process->animMulti = animMulti2;

								for (auto& blocklist : cmp2_block)
								{
									for (auto& blok : blocklist.second)
									{
										(process->*blok->func)(*blok, lines);
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
								ErrorMessage(1172, format, behaviorFile, numline);
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
								return next->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, next);
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
								return next->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, next);
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
								return next->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, next);
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
								return next->isTrue(process, format, behaviorFile, numline, isGroup, isMaster, next);
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

void condt::specialCondition(string condition, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster, OptionList& optionlist)
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

void condt::conditionProcess(string condition, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster)
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

void condt::singleCondition(string condition, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster, OptionList& optionlist)
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

condt::condt(string condition, string format, string behaviorFile, string originalCondition, string multiOption, int numline, bool isGroup, bool isMaster,
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

		nestedcond = make_shared<condt>(inHouse, format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);

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

			next = make_shared<condt>(outHouse.substr(1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
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
				next = make_shared<condt>(condition.substr(c_or + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
			}
		}

		// AND exist but not OR
		else if (c_or == NOT_FOUND)
		{
			singleCondition(condition.substr(0, c_and), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
			next = make_shared<condt>(condition.substr(c_and + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
		}

		// AND & OR exist
		else
		{
			// OR before AND
			if (c_or < c_and)
			{
				OR = true;
				singleCondition(condition.substr(0, c_or), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
				next = make_shared<condt>(condition.substr(c_or + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
			}

			// AND before OR
			else
			{
				singleCondition(condition.substr(0, c_and), format, behaviorFile, multiOption, numline, isGroup, isMaster, optionlist);
				next = make_shared<condt>(condition.substr(c_and + 1), format, behaviorFile, original, multiOption, numline, isGroup, isMaster, optionlist);
			}
		}
	}
}

multichoice::multichoice(string cond, string format, string behaviorFile, string multiOption, int numline, bool isGroup, bool isMaster, OptionList& optionlist,
	size_t posA, size_t posB)
{
	if (cond.length() > 0)
	{
		condition = make_shared<condt>(cond, format, behaviorFile, cond, multiOption, numline, isGroup, isMaster, optionlist);
	}
	else
	{
		condition = make_shared<condt>(format, format, behaviorFile, cond, multiOption, numline, isGroup, isMaster, optionlist);
	}

	locateA = posA;
	locateB = posB - 1;
}

void ProcessFunction(string change, string line, string format, string behaviorFile, string multiOption, bool& isEnd, int numline, size_t curPos,
	OptionList& optionlist, map<int, vector<shared_ptr<range>>>& lineblocks, vector<addOnInfo>& addInfo, bool& isTrueMulti, bool isGroup, bool isMaster, bool isMC,
	proc& process)
{
	if (isMaster && multiOption != format + "_group")
	{
		if (change.find(format + "[") != NOT_FOUND) ErrorMessage(1204, format, behaviorFile, numline, change);

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(format + "_group\\[(.*?)\\]")); itr != boost::sregex_iterator(); ++itr)
		{
			ErrorMessage(1201, format, behaviorFile, numline);
		}
	}
	else if (!isGroup && change.find(format + "_group[") != NOT_FOUND) ErrorMessage(1204, format, behaviorFile, numline, change);

	string shortcut = isMaster ? format + "_group\\[\\]" : format;

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

					if (change.find("(S" + ID + "+") == NOT_FOUND)
					{
						ID = "";
					}

					number = boost::regex_replace(string(equation.substr(3 + ID.length())), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
				}

				if (equation != "(S" + ID + "+" + number + ")" && isOnlyNumber(number))
				{
					range blok(curPos + nextpos, curPos + nextpos + equation.length(), &proc::computation);

					if (isMC)
					{
						lineblocks[blok.size].push_back(make_shared<range>(blok));
					}
					else
					{
						process.installBlock(blok, numline);
					}
				}
				
				if (equation.find("N") != NOT_FOUND || equation.find("B") != NOT_FOUND)
				{

				}

				if (error) throw nemesis::exception();

				nextpos = change.find("(", nextpos + 1);
			}
		}
	}

	if (change.find("END", 0) != NOT_FOUND)
	{
		boost::regex expr(shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[END\\]");

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), expr); itr != boost::sregex_iterator(); ++itr)
		{
			bool number = false;
			string first = itr->str(1);
			void (proc::*func)(range, vecstr&);

			if (first.length() == 0)
			{
				isTrueMulti = true;
				func = isGroup ? &proc::endMultiMaster : &proc::endMultiGroup;
			}
			else if (first == "F")
			{
				func = isGroup ? &proc::endFirstMaster : &proc::endFirstGroup;
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::endNextGroup;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::endBackGroup;
			}
			else if (first == "L")
			{
				func = isGroup ? &proc::endLastMaster : &proc::endLastGroup;
			}
			else
			{
				number = true;
				func = isGroup ? &proc::endNumMaster : &proc::endNumGroup;
			}

			shared_ptr<range> blok;

			if (number)
			{
				blok = make_shared<range>(curPos + itr->position(), curPos + itr->position() + itr->str().length(), vector<int>{ stoi(first) }, vecstr{ change }, func);
			}
			else
			{
				blok = make_shared<range>(curPos + itr->position(), curPos + itr->position() + itr->str().length(), vecstr{ change }, func);
			}
			
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		expr = boost::regex("(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)(END)");

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), expr); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			range blok(curPos + itr->position(), curPos + itr->position() + itr->str().length(), isGroup ? &proc::endMultiMaster : &proc::endSingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}

		if (error) throw nemesis::exception();

		isEnd = true;
	}

	// set state ID
	if (change.find("(S", 0) != NOT_FOUND)
	{
		string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[\\(S([0-9]*)\\+([0-9]+)\\)\\]";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			string first = itr->str(1);
			void (proc::*func)(range, vecstr&);

			if (first.length() == 0)
			{
				isTrueMulti = true;
				func = isGroup ? &proc::stateMultiMaster : &proc::stateMultiGroup;
			}
			else if (first == "F")
			{
				func = isGroup ? &proc::stateFirstMaster : &proc::stateFirstGroup;
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::stateNextGroup;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::stateBackGroup;
			}
			else if (first == "L")
			{
				func = isGroup ? &proc::stateLastMaster : &proc::stateLastGroup;
			}
			else
			{
				func = isGroup ? &proc::stateNumMaster : &proc::stateNumGroup;
			}

			int intID;
			string ID = itr->str(2);
			string number = itr->str(3);
			size_t post = curPos + itr->position();
			string full = itr->str();

			if (!isOnlyNumber(number))
			{
				ErrorMessage(1152, format, behaviorFile, numline, full);
			}

			ID.length() == 0 ? intID = 0 : intID = stoi(ID) - 1;
			string compare = line.substr(post, full.length());

			if (format + "[" + first + "][(S" + ID + "+" + number + ")]" == compare || format + "_group[][" + first + "][(S" + ID + "+" + number + ")]" == compare)
			{
				shared_ptr<range> blok = first.length() > 0 && isOnlyNumber(first) ?
					make_shared<range>(post, post + full.length(), vector<int>{ intID, stoi(number), stoi(first) }, vecstr{ full }, func) :
					make_shared<range>(post, post + full.length(), vector<int>{ intID, stoi(number) }, vecstr{ full }, func);
				isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
			}
			else
			{
				ErrorMessage(1137, format, behaviorFile, numline, full);
			}

			if (error) throw nemesis::exception();
		}

		if (isMaster)
		{
			expstr = shortcut + "\\[\\(S([0-9]*)\\+([0-9]+)\\)\\]";

			for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
			{
				int intID;
				string ID = itr->str(1);
				string number = itr->str(2);
				size_t post = curPos + itr->position();
				string full = itr->str();
				ID.length() == 0 ? intID = 0 : intID = stoi(ID) - 1;
				shared_ptr<range> blok = make_shared<range>(post, post + full.length(), vector<int>{ intID, stoi(number) }, vecstr{ full },
					&proc::stateMultiMasterToGroup);
				isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);

				if (error) throw nemesis::exception();
			}
		}

		expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)(?<!" + shortcut +
			"\\[)\\(S([0-9]*)\\+([0-9]+)\\)";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			int intID;
			size_t post = curPos + itr->position();
			string first = itr->str(1);
			first.length() == 0 ? intID = 0 : intID = stoi(first) - 1;
			range blok(post, post + itr->str().length(), vector<int>{ intID, stoi(itr->str(2)) }, &proc::stateSingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}
	}

	if (change.find("FilePath", 0) != NOT_FOUND)
	{
		string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[FilePath\\]";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			bool number = false;
			size_t post = curPos + itr->position();
			string first = itr->str(1);
			void (proc::*func)(range, vecstr&);

			if (first.length() == 0)
			{
				isTrueMulti = true;
				func = isGroup ? &proc::filepathMultiMaster : &proc::filepathMultiGroup;
			}
			else if (first == "F")
			{
				func = isGroup ? &proc::filepathFirstMaster : &proc::filepathFirstGroup;
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line); 
				func = &proc::filepathNextGroup;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line); 
				func = &proc::filepathBackGroup;
			}
			else if (first == "L")
			{
				func = isGroup ? &proc::filepathLastMaster : &proc::filepathLastGroup;
			}
			else
			{
				number = true;
				func = isGroup ? &proc::filepathNumMaster : &proc::filepathNumGroup;
			}

			shared_ptr<range> blok;
			number ? blok = make_shared<range>(post, post + itr->str().length(), vector<int>{ stoi(first) }, vecstr{ change }, func) : 
				blok = make_shared<range>(post, post + itr->str().length(), func);
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)(FilePath)";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			range blok(post, post + 8, isGroup ? &proc::filepathMultiMaster : &proc::filepathSingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}
	}

	if (change.find("FileName", 0) != NOT_FOUND)
	{
		string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[FileName\\]";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			bool number = false;
			size_t post = curPos + itr->position();
			string first = itr->str(1);
			void (proc::*func)(range, vecstr&);

			if (first.length() == 0)
			{
				isTrueMulti = true;
				func = isGroup ? &proc::filenameMultiMaster : &proc::filenameMultiGroup;
			}
			else if (first == "F")
			{
				func = isGroup ? &proc::filenameFirstMaster : &proc::filenameFirstGroup;
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::filenameNextGroup;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::filenameBackGroup;
			}
			else if (first == "L")
			{
				func = isGroup ? &proc::filenameLastMaster : &proc::filenameLastGroup;
			}
			else
			{
				number = true;
				func = isGroup ? &proc::filenameNumMaster : &proc::filenameNumGroup;
			}

			shared_ptr<range> blok;
			number ? blok = make_shared<range>(post, post + itr->str().length(), vector<int>{ stoi(first) }, vecstr{ change }, func) :
				blok = make_shared<range>(post, post + itr->str().length(), func);
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)(FileName)";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			range blok(post, post + 8, isGroup ? &proc::filenameMultiMaster : &proc::filenameSingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}

		if (error) throw nemesis::exception();
	}

	if (change.find("Path", 0) != NOT_FOUND)
	{
		string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[Path\\]";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			size_t post = curPos + itr->position();
			range blok(post, post + itr->str().length(), &proc::pathSingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}

		expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)(Path)";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			size_t post = curPos + itr->position();
			range blok(post, post + 4, &proc::pathSingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}

		if (error) throw nemesis::exception();
	}

	if (change.find("@AnimObject/", 0) != NOT_FOUND)
	{
		int counter = 0;
		string expstr = shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[@AnimObject\\/([0-9]+)\\](\\[[0-9]+\\]|)";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			string first = itr->str(1);
			string second = itr->str(2);
			string optionMulti = itr->str(3);
			string full = itr->str();
			size_t post = curPos + itr->position();
			void (proc::*func)(range, vecstr&);
			vector<int> container;
			++counter;
			string templine;
			bool empty = true;

			if (full.length() == 0)
			{
				string errorformat = format;
				if (isMaster) errorformat.append("_master");
				else if (isGroup) errorformat.append("_group");
				ErrorMessage(1157, errorformat, behaviorFile, numline, change);
			}

			if (first.length() == 0)
			{
				isTrueMulti = true;

				if (isGroup)
				{
					empty ? func = &proc::AOMultiMasterA : func = &proc::AOMultiMasterB;
				}
				else
				{
					empty ? func = &proc::AOMultiGroupA : func = &proc::AOMultiGroupB;
				}
			}
			else if (first == "F")
			{
				if (isGroup)
				{
					empty ? func = &proc::AOFirstMasterA : func = &proc::AOFirstMasterB;
				}
				else
				{
					empty ? func = &proc::AOFirstGroupA : func = &proc::AOFirstGroupB;
				}
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				empty ? func = &proc::AONextGroupA : func = &proc::AONextGroupB;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				empty ? func = &proc::AOBackGroupA : func = &proc::AOBackGroupB;
			}
			else if (first == "L")
			{
				if (isGroup)
				{
					empty ? func = &proc::AOLastMasterA : func = &proc::AOLastMasterB;
				}
				else
				{
					empty ? func = &proc::AOLastGroupA : func = &proc::AOLastGroupB;
				}
			}
			else
			{
				container.push_back(stoi(first));

				if (isGroup)
				{
					empty ? func = &proc::AONumMasterA : func = &proc::AONumMasterB;
				}
				else
				{
					empty ? func = &proc::AONumGroupA : func = &proc::AONumGroupB;
				}
			}

			container.push_back(stoi(second));

			if (optionMulti.length() != 0)
			{
				empty = false;
				optionMulti.pop_back();
				optionMulti = optionMulti.substr(1);
				container.push_back(stoi(optionMulti));
			}

			range blok(post, post + full.length(), container, vecstr{ change }, func);

			if (isMC)
			{
				lineblocks[blok.size].push_back(make_shared<range>(blok));
				empty ? addInfo.push_back(addOnInfo("@AnimObject/" + second, "")) : addInfo.push_back(addOnInfo("@AnimObject/" + second, "", stoi(optionMulti)));
			}
			else
			{
				process.installBlock(blok, numline);
			}

			if (error) throw nemesis::exception();
		}

		expstr = "(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut + "\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)@AnimObject\\/([0-9]+)(\\[[0-9]+\\]|)";

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(expstr)); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			string templine = itr->str(2);

			if (templine.length() == 0)
			{
				range blok(post, post + itr->str().length(), vector<int>{ stoi(itr->str(1)) }, isGroup ? &proc::AOMultiMasterA : &proc::AOSingleA);

				if (isMC)
				{
					lineblocks[blok.size].push_back(make_shared<range>(blok));
					addInfo.push_back(addOnInfo("@AnimObject/" + itr->str(1), ""));
				}
				else
				{
					process.installBlock(blok, numline);
				}
			}
			else
			{
				templine.pop_back();
				templine = templine.substr(1);
				range blok(post, post + itr->str().length(), vector<int>{ stoi(itr->str(1)), stoi(templine) }, isGroup ? &proc::AOMultiMasterB : &proc::AOSingleB);

				if (isMC)
				{
					lineblocks[blok.size].push_back(make_shared<range>(blok));
					addInfo.push_back(addOnInfo("@AnimObject/" + itr->str(1), "", stoi(templine)));
				}
				else
				{
					process.installBlock(blok, numline);
				}
			}
		}

		if (error) throw nemesis::exception();
	}

	if (change.find("main_anim_event", 0) != NOT_FOUND)
	{
		mainAnimEventInstall(format, behaviorFile, change, numline, curPos, boost::regex("\\{" + shortcut + "\\[(F|N|B|L|[0-9]*)\\]\\[main_anim_event\\]\\}"), isGroup,
			isMaster, process);
		mainAnimEventInstall(format, behaviorFile, change, numline, curPos, boost::regex("(?<!\\{)" + shortcut +
			"\\[(F|N|B|L|[0-9]*)\\]\\[main_anim_event\\](?=[^\\}]|$)"), isGroup, isMaster, process);

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex("(\\{main_anim_event\\})"));
			itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			range blok(post, post + itr->str().length(), isGroup ? &proc::MAEMultiMaster : &proc::MAESingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex("(?<!" + shortcut + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" + shortcut +
			"\\[\\]\\[)(?<!" + shortcut + "\\[\\d\\d\\]\\[)(?<!\\{)(main_anim_event)(?=[^\\}]|$)")); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			range blok(post, post + itr->str().length(), isGroup ? &proc::MAEMultiMaster : &proc::MAESingle);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}
	}

	size_t pos = change.find("[");

	if (pos != NOT_FOUND && change.find("]", pos) != NOT_FOUND)
	{
		for (auto it = optionlist.addOn.begin(); it != optionlist.addOn.end(); ++it)
		{
			for(auto& addname : it->second)
			{
				// include other anim group
				// cont here

				for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(format + "\\[(F|N|B|L|[0-9]*)\\]\\[" + it->first +
					"(\\*|)\\]\\[" + addname + "\\](\\[[0-9]+\\]|)"));
					itr != boost::sregex_iterator(); ++itr)
				{
					bool number = false;
					string header;
					string first = itr->str(1);
					size_t addpos = curPos + itr->position();
					void (proc::*func)(range, vecstr&);

					if (first.length() == 0)
					{
						isTrueMulti = true;
						func = isGroup ? &proc:: addOnMultiMaster : &proc::addOnMultiGroup;
					}
					else if (first == "F")
					{
						func = isGroup ? &proc::addOnFirstMaster : &proc::addOnFirstGroup;
					}
					else if (first == "N")
					{
						if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
						else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
						func = &proc::addOnNextGroup;
					}
					else if (first == "B")
					{
						if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
						else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
						func = &proc::addOnBackGroup;
					}
					else if (first == "L")
					{
						func = isGroup ? &proc::addOnLastMaster : &proc::addOnLastGroup;
					}
					else
					{
						number = true;
						func = isGroup ? &proc::addOnNumMaster : &proc::addOnNumGroup;
					}

					if (itr->str(2).length() == 0)
					{
						header = it->first;
					}
					else
					{
						header = optionlist.mixOptRegis[it->first];
					}

					if (number)
					{
						range blok(addpos, addpos + itr->str().length(), vector<int>{ stoi(first) }, vecstr{ header, addname, change }, func);

						if (isMC)
						{
							lineblocks[blok.size].push_back(make_shared<range>(blok));
							string o_multi = itr->str(3);

							if (o_multi.length() > 0)
							{
								addInfo.push_back(addOnInfo(header, addname, stoi(o_multi)));
							}
							else
							{
								addInfo.push_back(addOnInfo(header, addname));
							}
						}
						else
						{
							process.installBlock(blok, numline);
						}
					}
					else
					{
						range blok(addpos, addpos + itr->str().length(), vecstr{ header, addname, change }, func);

						if (isMC)
						{
							lineblocks[blok.size].push_back(make_shared<range>(blok));
							string o_multi = itr->str(3);

							if (o_multi.length() > 0)
							{
								addInfo.push_back(addOnInfo(header, addname, stoi(o_multi)));
							}
							else
							{
								addInfo.push_back(addOnInfo(header, addname));
							}
						}
						else
						{
							process.installBlock(blok, numline);
						}
					}
				}

				for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(it->first + "(\\*|)\\[" + addname + "\\](\\[[0-9]+\\]|)"));
					itr != boost::sregex_iterator(); ++itr)
				{
					if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

					string header;
					string first = itr->str(1);
					size_t addpos = curPos + itr->position();

					if (first.length() == 0)
					{
						header = it->first;
					}
					else
					{
						header = optionlist.mixOptRegis[it->first];
					}

					range blok(addpos, addpos + itr->str().length(), vecstr{ header, addname, change }, isGroup ? &proc::addOnMultiMaster : &proc::addOnSingle);

					if (isMC)
					{
						lineblocks[blok.size].push_back(make_shared<range>(blok));
						string o_multi = itr->str(2);

						if (o_multi.length() > 0)
						{
							addInfo.push_back(addOnInfo(header, addname, stoi(o_multi)));
						}
						else
						{
							addInfo.push_back(addOnInfo(header, addname));
						}
					}
					else
					{
						process.installBlock(blok, numline);
					}
				}
			}
		}
	}

	if (change.find("LastState") != NOT_FOUND)
	{
		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex("LastState([0-9]*)")); itr != boost::sregex_iterator();
			++itr)
		{
			if (isMaster) ErrorMessage(1206, format + "_master", behaviorFile, numline, itr->str());
			if (isGroup) ErrorMessage(1206, format + "_group", behaviorFile, numline, itr->str());

			size_t post = curPos + itr->position();
			range blok(post, post + itr->str().length(), vecstr{ itr->str(1) }, &proc::lastState);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
		}
	}

	if (change.find("eventID[") != NOT_FOUND)
	{
		pos = change.find("eventID[");

		while (pos != NOT_FOUND)
		{
			size_t eventpos = pos + 8;
			size_t curcounter = 1;
			size_t post = curPos + pos;

			for (unsigned int ch = eventpos; ch < change.length(); ++ch)
			{
				if (change[ch] == '[') ++curcounter;
				else if (change[ch] == ']')
				{
					--curcounter;

					if (curcounter == 0)
					{
						++eventpos;
						eventpos += curPos;
						break;
					}
				}

				++eventpos;
			}

			range blok(post, eventpos, vector<int> { int(post + 8), int(eventpos - 2) }, &proc::eventID);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
			pos = change.find("eventID[", pos + 1);
		}
	}

	if (change.find("variableID[") != NOT_FOUND)
	{
		pos = change.find("variableID[");

		while (pos != NOT_FOUND)
		{
			size_t varpos = pos + 11;
			size_t curcounter = 1;
			size_t post = curPos + pos;

			for (unsigned int ch = varpos; ch < change.length(); ++ch)
			{
				if (change[ch] == '[') ++curcounter;
				else if (change[ch] == ']')
				{
					--curcounter;

					if (curcounter == 0)
					{
						++varpos;
						varpos += curPos;
						break;
					}
				}

				++varpos;
			}

			range blok(post, varpos, vector<int> { int(post + 11), int(varpos - 2) }, &proc::variableID);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
			pos = change.find("variableID[", pos + 1);
		}
	}

	if (change.find("animOrder[") != NOT_FOUND)
	{
		pos = change.find("animOrder[");

		while (pos != NOT_FOUND)
		{
			size_t animpos = pos + 10;
			size_t curcounter = 1;
			size_t post = curPos + pos;

			for (unsigned int ch = animpos; ch < change.length(); ++ch)
			{
				if (change[ch] == '[') ++curcounter;
				else if (change[ch] == ']')
				{
					--curcounter;

					if (curcounter == 0)
					{
						++animpos;
						animpos += curPos;
						break;
					}
				}

				++animpos;
			}

			range blok(post, animpos, { int(post + 10), int(animpos - 2) }, &proc::animOrder);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
			pos = change.find("animOrder[", pos + 1);
		}
	}

	if (change.find("crc32[") != NOT_FOUND)
	{
		__int64 bracketCount = count(change.begin(), change.end(), '[');
		__int64 altBracketCount = count(change.begin(), change.end(), ']');

		if (bracketCount != altBracketCount)
		{
			isGroup ? ErrorMessage(1181, format + "_group", behaviorFile, numline, change) : ErrorMessage(1181, format, behaviorFile, numline, change);
		}

		pos = -1;
		pos = change.find("crc32[");

		while (pos != NOT_FOUND)
		{
			size_t encode = pos + 6;
			size_t curcounter = 1;

			for (unsigned int ch = encode; ch < change.length(); ++ch)
			{
				if (change[ch] == '[') ++curcounter;
				else if (change[ch] == ']')
				{
					--curcounter;

					if (curcounter == 0)
					{
						++encode;
						break;
					}
				}

				++encode;
			}

			range blok(curPos + pos, curPos + encode, &proc::crc32);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
			pos = change.find("crc32[", pos + 1);
		}
	}

	if (change.find("import[") != NOT_FOUND)
	{
		pos = change.find("import[");

		while (pos != NOT_FOUND)
		{
			string import;
			size_t curcounter = 0;
			size_t post = curPos + pos;

			for (unsigned int ch = pos; ch < change.length(); ++ch)
			{
				if (change[ch] == '[') ++curcounter;
				else if (change[ch] == ']')
				{
					--curcounter;

					if (curcounter == 0)
					{
						if (ch == change.length() - 1 || change[ch + 1] != '[')
						{
							import.push_back(change[ch]);
							break;
						}
					}
				}

				import.push_back(change[ch]);
			}

			__int64 bracketCount = count(import.begin(), import.end(), '[');
			__int64 altBracketCount = count(import.begin(), import.end(), ']');

			if (bracketCount != altBracketCount)
			{
				isGroup ? ErrorMessage(1139, format + "_group", behaviorFile, numline, import) : ErrorMessage(1139, format, behaviorFile, numline, import);
			}

			range blok(post, post + import.length(), &proc::import);
			isMC ? lineblocks[blok.size].push_back(make_shared<range>(blok)) : process.installBlock(blok, numline);
			pos = change.find("import[", pos + 1);
		}
	}

	if (change.find("MD", 0) != NOT_FOUND)
	{
		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(format + "\\[(F|N|B|L|[0-9]*)\\]\\[MD\\]"));
			itr != boost::sregex_iterator(); ++itr)
		{
			bool number = false;
			string first = itr->str(1);
			size_t post = curPos + itr->position();
			void (proc::*func)(range, vecstr&);

			if (first.length() == 0)
			{
				isTrueMulti = true;
				func = isGroup ? &proc::motionDataMultiMaster : &proc::motionDataMultiGroup;
			}
			else if (first == "F")
			{
				func = isGroup ? &proc::motionDataFirstMaster : &proc::motionDataFirstGroup;
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::motionDataNextGroup;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::motionDataBackGroup;
			}
			else if (first == "L")
			{
				func = isGroup ? &proc::motionDataLastMaster : &proc::motionDataLastGroup;
			}
			else
			{
				number = true;
				func = isGroup ? &proc::motionDataNumMaster : &proc::motionDataNumGroup;
			}

			shared_ptr<range> blok = number ? make_shared<range>(post, post + itr->str().length(), vector<int>{ stoi(first) }, vecstr{ change }, func) :
				make_shared<range>(post, post + itr->str().length(), vecstr{ change }, func);
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex("(?<!" + format + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" +
			format + "\\[\\]\\[)(?<!" + format + "\\[\\d\\d\\]\\[)(MD)")); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			shared_ptr<range> blok = make_shared<range>(post, post + itr->str().length(), vecstr{ change }, isGroup ? &proc::motionDataMultiMaster :
				&proc::motionDataSingle);
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		if (error) throw nemesis::exception();
	}

	if (change.find("RD", 0) != NOT_FOUND)
	{
		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex(format + "\\[(F|N|B|L|[0-9]*)\\]\\[RD\\]"));
			itr != boost::sregex_iterator(); ++itr)
		{
			bool number = false;
			string first = itr->str(1);
			size_t post = curPos + itr->position();
			void (proc::*func)(range, vecstr&);

			if (first.length() == 0)
			{
				isTrueMulti = true;
				func = isGroup ? &proc::rotationDataMultiMaster : &proc::rotationDataMultiGroup;
			}
			else if (first == "F")
			{
				func = isGroup ? &proc::rotationDataFirstMaster : &proc::rotationDataFirstGroup;
			}
			else if (first == "N")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::rotationDataNextGroup;
			}
			else if (first == "B")
			{
				if (isMaster) ErrorMessage(1056, format + "_master", behaviorFile, numline, line);
				else if (isGroup) ErrorMessage(1056, format + "_group", behaviorFile, numline, line);
				func = &proc::rotationDataBackGroup;
			}
			else if (first == "L")
			{
				func = isGroup ? &proc::rotationDataLastMaster : &proc::rotationDataLastGroup;
			}
			else
			{
				number = true;
				func = isGroup ? &proc::rotationDataNumMaster : &proc::rotationDataNumGroup;
			}

			shared_ptr<range> blok = number ? make_shared<range>(post, post + itr->str().length(), vector<int>{ stoi(first) }, vecstr{ change }, func) :
				make_shared<range>(post, post + itr->str().length(), func);
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		for (auto& itr = boost::sregex_iterator(change.begin(), change.end(), boost::regex("(?<!" + format + "\\[[F|N|B|L|\\d]\\]\\[)(?<!" +
			format + "\\[\\]\\[)(?<!" + format + "\\[\\d\\d\\]\\[)(RD)")); itr != boost::sregex_iterator(); ++itr)
		{
			if (isGroup && multiOption != format) ErrorMessage(1146, format, behaviorFile, numline);

			size_t post = curPos + itr->position();
			shared_ptr<range> blok = make_shared<range>(post, post + itr->str().length(), vecstr{ change }, isGroup ? &proc::rotationDataMultiMaster :
				&proc::rotationDataSingle);
			isMC ? lineblocks[blok->size].push_back(blok) : process.installBlock(*blok, numline);
		}

		if (error) throw nemesis::exception();
	}
}

void GetMultiFromAddOn(addOnInfo& addinfo, string format, string behaviorFile, string original, int numline, int animMulti, bool isGroup,
	vector<shared_ptr<AnimationInfo>>& groupAnimInfo, int& optionMulti, int& endMulti)
{
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
					isGroup ? ErrorMessage(1148, format + "_group", behaviorFile, numline, original) : ErrorMessage(1148, format, behaviorFile, numline, original);
				}
			}
		}
	}
}
