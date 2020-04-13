#include "update/animdatacont.h"

using namespace std;

unsigned int MasterAnimData::getIndex(const ProjectName& projName)
{
	auto itr = projectIndexMap.find(projName);

	if (itr == projectIndexMap.end()) ErrorMessage(3025);

	return itr->second;
}

MasterAnimData::ProjectPtr MasterAnimData::find(const ProjectName& projName, const ModCode& modcode)
{
	if (modcode == "original") return projectlist[getIndex(projName)].raw.second;
	
	auto itr = projectIndexMap.find(projName);

	if (itr == projectIndexMap.end()) return nullptr;

	return projectlist[itr->second].raw.second;
}

MasterAnimData::ProjectPtr MasterAnimData::add(const ProjectName& projName, size_t num, const ModCode& modcode, nemesis::CondType type)
{
	projectIndexMap[projName + "~" + to_string(++projectCounter[projName])] = projectlist.size();
	auto& ref = ProjectData(pair<ProjectNameLinked, ProjectPtr>(), num);

	if (modcode != "original")
	{
		projectlist.push_back(nemesis::CondVar(ref));
		projectlist.back().linecount = num;
		auto& curcond = projectlist.back().backCond();
		curcond.conditions = modcode;
		curcond.conditionType = type;
		curcond.rawlist.back().linecount = num;
		curcond.rawlist.back().raw.first = projName;
		curcond.rawlist.back().raw.second = make_shared<AnimDataProject_Condt>();
		return curcond.rawlist.back().raw.second;
	}

	projectlist.push_back(ref);
	projectlist.back().raw.first = projName;
	projectlist.back().raw.first.linecount = num;
	projectlist.back().raw.second = make_shared<AnimDataProject_Condt>();
	return projectlist.back().raw.second;
}

MasterAnimData::ProjectPtr MasterAnimData::add(const ProjectName& projName, const vecstr& storeline, const ModCode& modcode)
{
	int type = 0;

	ProjectPtr& proj = find(projName, "original");

	if (proj == nullptr)
	{
		add(projName, 0, modcode);
		proj = make_shared<AnimDataProject_Condt>(storeline);
		projectlist.back().raw.second = proj;
	}

	return proj;
}

void MasterAnimData::projectListUpdate(const ModCode& modcode, const string& filepath, const vecstr& storeline, bool isTemplate)
{
	size_t pos;

	if (isTemplate)
	{
		vector<ProjectNameLinked*> curPoint;
		vector<vecstr> holdinglist;
		unique_ptr<ProjectData> condStringPtr;
		int open = 0;

		for (unsigned int i = storeline.size() - 1; i >= 0; --i)
		{
			const string& line = storeline[i];
			pos = line.find("<!-- FOREACH ^");

			if (pos != NOT_FOUND && line.find("^ -->", pos) != NOT_FOUND)
			{
				pos += 14;
				string condition = line.substr(pos, line.find("^ -->", pos));

				if (condStringPtr != nullptr)
				{
					condStringPtr = make_unique<ProjectData>(nemesis::CondVar(*condStringPtr, condition, nemesis::FOREACH));
				}
				else
				{
					condStringPtr = make_unique<ProjectData>();
					condStringPtr->addCond(nemesis::FOREACH);
					condStringPtr->backCond().conditions = condition;
				}

				for (auto projname : holdinglist.back())
				{
					condStringPtr->backCond().rawlist.push_back(std::pair<ProjectNameLinked, ProjectPtr>());
				}

				holdinglist.pop_back();

				if (holdinglist.size() == 0)
				{
					projectlist.push_back(*condStringPtr);
				}
			}
			else if (line.find("<!-- CLOSE -->") != NOT_FOUND)
			{
				++open;
				holdinglist.push_back(vecstr());
			}
			else if (holdinglist.size() > 0)
			{
				holdinglist.back().insert(holdinglist.back().begin(), line);
			}
		}

		return;
	}

	vector<pair<string, unsigned int>> edits;
	vector<pair<string, unsigned int>> origs;

	bool edited = false;
	bool originalopen = false;

	for (unsigned int i = 1; i < storeline.size(); ++i)
	{
		const string& line = storeline[i];

		if (!edited)
		{
			pos = line.find("<!-- MOD_CODE");
			edited = pos != NOT_FOUND && line.find("OPEN -->", pos) != NOT_FOUND;
		}
		else if (line.find("<!-- ORIGINAL -->") != NOT_FOUND)
		{
			originalopen = true;
		}
		else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			edited = false;
			originalopen = false;

			if (origs.size() > 0)
			{
				if (origs.size() > edits.size()) ErrorMessage(3026, modcode, filepath);

				for (unsigned int k = 0; k < origs.size(); ++k)
				{
					auto& nestedcondt = projectlist[getIndex(origs[k].first)].raw.first.nestedcond;
					nestedcondt.push_back(nemesis::CondVar<string>(nemesis::LinkedVar<string>(edits[k].first, edits[k].second), modcode, nemesis::MOD_CODE));
				}
			}

			if (edits.size() > origs.size())
			{
				for (unsigned int k = origs.size(); k < edits.size(); ++k)
				{
					add(edits[k].first, edits[k].second, modcode, nemesis::MOD_CODE);
				}
			}
		}
		else if (originalopen)
		{
			origs.push_back(make_pair(line, i));
		}
		else
		{
			edits.push_back(make_pair(line, i));
		}
	}
}

AnimDataProject_Condt::AnimDataProject_Condt(const vecstr& storeline)
{
	short type = 0;

	for (unsigned int i = 1; i < storeline.size(); ++i)
	{
		const string& line = storeline[i];

		switch (type)
		{
			case 0:
				projectActive = line;
				++type;
				break;
			case 1:
				// behavior txt count
				++type;
				break;
			case 2:
				if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

				behaviorlist.push_back(line);
				break;
			case 3:
				childActive = line;
				++type;
				break;
			default:
				break;
		}
	}
}

void AnimDataProject_Condt::update(const ModCode& modcode, const vecstr& storeline, size_t linenum)
{
	short type = 0;

	if (modcode == "original")
	{
		for (unsigned int i = 1; i < storeline.size(); ++i)
		{
			const string& line = storeline[i];

			switch (type)
			{
			case 0:
				projectActive = line;
				projectActive.linecount = linenum + i;
				++type;
				break;
			case 1:
				// behavior txt count
				++type;
				break;
			case 2:
				if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

				behaviorlist.push_back(line);
				behaviorlist.back().linecount = linenum + i;
				break;
			case 3:
				childActive = line;
				childActive.linecount = linenum + i;
				++type;
				break;
			default:
				break;
			}
		}

		return;
	}
}

void AnimDataProject_Condt::modify(const ModCode& modcode, const vecstr& storeline)
{
	unsigned int bhvrCount = 0;
	short type = 0;
	bool edited = false;
	bool originalopen = false;
	bool editopen = false;

	for (unsigned int i = 1; i < storeline.size(); ++i)
	{
		const string& line = storeline[i];

		if (line.find("<!--") != NOT_FOUND)
		{
			if (!edited && line.find("<!-- MOD_CODE", 0) != NOT_FOUND && line.find("OPEN -->", 0) != NOT_FOUND)
			{
				edited = true;
			}
			else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
			{
				originalopen = true;
			}
			else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
			{
				originalopen = false;
				edited = false;
			}

			continue;
		}
		else if (originalopen)
		{
			continue;
		}

		switch (type)
		{
			case 0:
				if (edited) projectActive.addCond(nemesis::LinkedVar<string>(line), modcode, nemesis::MOD_CODE, i);

				++type;
				break;
			case 1:
				// behavior txt count
				++type;
				break;
			case 2:
				if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

				if (edited)
				{
					if (bhvrCount < behaviorlist.size()) behaviorlist[bhvrCount].addCond(line, modcode, nemesis::MOD_CODE);
					else behaviorlist.push_back(nemesis::CondVar<string>(line, modcode, nemesis::MOD_CODE));
				}

				++bhvrCount;
				break;
			case 3:
				if (edited) childActive.addCond(line, modcode, nemesis::MOD_CODE);
				++type;
				break;
			default:
				break;
		}
	}
}

vecstr AnimDataProject_Condt::agetline()
{
	vecstr storeline;
	
	

	return storeline;
}

vecstr AnimDataProject_Condt::igetLine(const Header& projName)
{
	return vecstr();
}

AnimDataPack_Condt& AnimDataProject_Condt::aadd(const Header& header, const ModCode& modcode, nemesis::CondType type)
{
	if (modcode != "original")
	{
		using DP = std::pair<Header, nemesis::LinkedVar<std::shared_ptr<AnimDataPack_Condt>>>;
		animdatalist.push_back(DataPackCondt<std::shared_ptr<AnimDataPack_Condt>>(nemesis::CondVar<DP>(DP(), modcode, type)));
		animdatalist.back().nestedcond.back().rawlist.back().raw.first = header;
		return *animdatalist.back().nestedcond.back().rawlist.back().raw.second.raw;
	}

	animdatalist.push_back(DataPackCondt<std::shared_ptr<AnimDataPack_Condt>>());
	animdatalist.back().raw.first = header;
	return *animdatalist.back().raw.second.raw;
}

AnimDataPack_Condt& AnimDataProject_Condt::aadd(const Header& header, const ModCode& modcode, const vecstr& storeline, size_t linenum, nemesis::CondType type)
{
	using DP = std::pair<Header, nemesis::LinkedVar<std::shared_ptr<AnimDataPack_Condt>>>;
	DP* refPair;

	if (modcode != "original")
	{
		animdatalist.push_back(DataPackCondt<std::shared_ptr<AnimDataPack_Condt>>(nemesis::CondVar<DP>(DP(), modcode, type)));
		animdatalist.back().linecount = linenum;
		auto& refLinked = animdatalist.back().nestedcond.back().rawlist.back();
		refLinked.linecount = linenum;
		refPair = &refLinked.raw;
	}
	else
	{
		animdatalist.push_back(DataPackCondt<std::shared_ptr<AnimDataPack_Condt>>());
		animdatalist.back().linecount = linenum;
		refPair = &animdatalist.back().raw;
	}

	refPair->first = header;
	refPair->second.raw = make_shared<AnimDataPack_Condt>(storeline, linenum);
	refPair->second.linecount = linenum;
	return *refPair->second.raw;
}

InfoDataPack_Condt& AnimDataProject_Condt::iadd(const Header& header, const ModCode& modcode, nemesis::CondType type)
{
	if (modcode != "original")
	{
		using DP = std::pair<Header, nemesis::LinkedVar<std::shared_ptr<InfoDataPack_Condt>>>;
		infodatalist.push_back(DataPackCondt<std::shared_ptr<InfoDataPack_Condt>>(nemesis::CondVar<DP>(DP(), modcode, type)));
		infodatalist.back().nestedcond.back().rawlist.back().raw.first = header;
		return *infodatalist.back().nestedcond.back().rawlist.back().raw.second.raw;
	}

	infodatalist.push_back(DataPackCondt<std::shared_ptr<InfoDataPack_Condt>>());
	infodatalist.back().raw.first = header;
	return *infodatalist.back().raw.second.raw;
}

InfoDataPack_Condt& AnimDataProject_Condt::iadd(const Header& header, const ModCode& modcode, const vecstr& storeline, size_t linenum, nemesis::CondType type)
{
	using DP = std::pair<Header, nemesis::LinkedVar<std::shared_ptr<InfoDataPack_Condt>>>;
	DP* refPair;

	if (modcode != "original")
	{
		infodatalist.push_back(DataPackCondt<std::shared_ptr<InfoDataPack_Condt>>(nemesis::CondVar<DP>(DP(), modcode, type)));
		infodatalist.back().linecount = linenum;
		auto& refLinked = infodatalist.back().nestedcond.back().rawlist.back();
		refLinked.linecount = linenum;
		refPair = &refLinked.raw;

	}
	else
	{
		infodatalist.push_back(DataPackCondt<std::shared_ptr<InfoDataPack_Condt>>());
		animdatalist.back().linecount = linenum;
		refPair = &infodatalist.back().raw;
	}

	refPair->first = header;
	refPair->second.raw = make_shared<InfoDataPack_Condt>(storeline, linenum);
	refPair->second.linecount = linenum;
	return *refPair->second.raw;
}

nemesis::LinkedVar<shared_ptr<AnimDataPack_Condt>>* AnimDataProject_Condt::afindlist(const Header& header)
{
	for (auto& each : animdatalist)
	{
		if (each.raw.first == header) return &each.raw.second;
	}

	return nullptr;
}

nemesis::LinkedVar<shared_ptr<InfoDataPack_Condt>>* AnimDataProject_Condt::ifindlist(const Header& header)
{
	for (auto& each : infodatalist)
	{
		if (each.raw.first == header) return &each.raw.second;
	}

	return nullptr;
}

AnimDataPack_Condt* AnimDataProject_Condt::afind(const Header& header, const ModCode& modcode)
{
	for (auto& each : animdatalist)
	{
		if (each.raw.first + "~" + each.raw.second.raw->uniquecode.raw == header) return &*each.raw.second.raw;
	}

	return nullptr;
}

InfoDataPack_Condt* AnimDataProject_Condt::ifind(const Header& header, const ModCode& modcode)
{
	for (auto& each : infodatalist)
	{
		if (each.raw.first == header)
		{
			return &*each.raw.second.raw;
		}
	}

	return nullptr;
}

AnimDataPack_Condt::AnimDataPack_Condt(const vecstr& storeline, size_t linenum)
{
	short type = 0;

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		const string& line = storeline[i];

		switch (type)
		{
			case 0:
				name = line;
				++type;
				break;
			case 1:
				uniquecode = line;
				uniquecode.linecount = linenum + i;
				++type;
				break;
			case 2:
				unknown1 = line;
				unknown1.linecount = linenum + i;
				++type;
				break;
			case 3:
				unknown2 = line;
				unknown2.linecount = linenum + i;
				++type;
				break;
			case 4:
				unknown3 = line;
				unknown3.linecount = linenum + i;
				++type;
				break;
			case 5:
				// event count
				if (storeline[i + 1].length() == 0) ++type;

				++type;
				break;
			case 6:
				if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

				eventname.push_back(nemesis::LinkedVar<std::string>(line, linenum + i));
			default:
				break;
		}
	}
}

InfoDataPack_Condt::InfoDataPack_Condt(const vecstr& storeline, size_t linenum)
{
	short type = 0;

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		const string& line = storeline[i];

		switch (type)
		{
			case 0:
				uniquecode = line;
				++type;
				break;
			case 1:
				duration = line;
				++type;
				break;
			case 2:
				// motion data counter
				++type;
				break;
			case 3:
				if (i + 1 < storeline.size())
				{
					const string& nextline = storeline[i + 1];

					if (count(nextline.begin(), nextline.end(), ' ') == 0 && isOnlyNumber(nextline)) ++type;
				}

				motiondata.push_back(line);
				break;
			case 4:
				// rotation data counter
				++type;
				break;
			case 5:
				if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

				motiondata.push_back(line);
				break;
			default:
				break;
		}
	}

}
