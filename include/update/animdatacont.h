#pragma once

#include "utilities/linkedvar.h"

struct AnimDataPack_Condt
{
	using ModCode = std::string;

	// anim data
	std::string name;
	nemesis::LinkedVar<std::string> uniquecode;
	nemesis::LinkedVar<std::string> unknown1;
	nemesis::LinkedVar<std::string> unknown2;
	nemesis::LinkedVar<std::string> unknown3;

	std::vector<nemesis::LinkedVar<std::string>> eventname;

	AnimDataPack_Condt() {}
	AnimDataPack_Condt(const VecStr& storeline, size_t linenum);
};

struct InfoDataPack_Condt
{
	using ModCode = std::string;

	std::string uniquecode;
	std::string duration;

	VecStr motiondata;
	VecStr rotationdata;

	InfoDataPack_Condt() {}
	InfoDataPack_Condt(const VecStr& storeline, size_t linenum);
};

struct AnimDataProject_Condt
{
	using Header = std::string;
	using ModCode = std::string;

	template<typename Pack>
	using DataPackCondt = nemesis::LinkedVar<std::pair<Header, nemesis::LinkedVar<Pack>>>;

	nemesis::LinkedVar<std::string> projectActive;
	std::vector<nemesis::LinkedVar<std::string>> behaviorlist;
	nemesis::LinkedVar<std::string> childActive;

	std::vector<DataPackCondt<std::shared_ptr<AnimDataPack_Condt>>> animdatalist;
	std::vector<DataPackCondt<std::shared_ptr<InfoDataPack_Condt>>> infodatalist;

	AnimDataProject_Condt() {}
	AnimDataProject_Condt(const VecStr& storeline);

	void update(const ModCode& modcode, const VecStr& storeline, size_t linenum);
	void modify(const ModCode& modcode, const VecStr& storeline);

	VecStr agetline();
	VecStr igetLine(const Header& projName);

	AnimDataPack_Condt& aadd(const Header& header, const ModCode& modcode = "original", nemesis::CondType type = nemesis::NONE);
	AnimDataPack_Condt& aadd(const Header& header, const ModCode& modcode, const VecStr& storeline, size_t linenum, nemesis::CondType type = nemesis::NONE);
	InfoDataPack_Condt& iadd(const Header& header, const ModCode& modcode = "original", nemesis::CondType type = nemesis::NONE);
	InfoDataPack_Condt& iadd(const Header& header, const ModCode& modcode, const VecStr& storeline, size_t linenum, nemesis::CondType type = nemesis::NONE);

	nemesis::LinkedVar<std::shared_ptr<AnimDataPack_Condt>>* afindlist(const Header& header);
	nemesis::LinkedVar<std::shared_ptr<InfoDataPack_Condt>>* ifindlist(const Header& header);

	AnimDataPack_Condt* afind(const Header& header, const ModCode& modcode = "original");
	InfoDataPack_Condt* ifind(const Header& header, const ModCode& modcode = "original");

private:

};

struct MasterAnimData
{
	using ModCode = std::string;
	using ProjectName = std::string;
	using ProjectNameLinked = nemesis::LinkedVar<ProjectName>;
	using ProjectPtr = std::shared_ptr<AnimDataProject_Condt>;
	using ProjectData = nemesis::LinkedVar<std::pair<ProjectNameLinked, ProjectPtr>>;
	template<typename T> using List = std::vector<T>;

private:
	std::unordered_map<ProjectName, unsigned int> projectIndexMap;
	std::unordered_map<ProjectName, unsigned int> projectCounter;

public:
	List<ProjectData> projectlist;

	unsigned int getIndex(const ProjectName& projName);

	ProjectPtr find(const ProjectName& projName, const ModCode& modcode = "original");

	ProjectPtr add(const ProjectName& projName, size_t num = 0, const ModCode& modcode = "original", nemesis::CondType type = nemesis::NONE);
	ProjectPtr add(const ProjectName& projName, const VecStr& storeline, const ModCode& modcode);

	void projectListUpdate(const ModCode& modcode, const std::string& filepath, const VecStr& storeline, bool isTemplate = false);

#if HIDE
	struct AnimDataList : VecStr
	{
	private:
		size_t TotalLines = 0;

	public:
		size_t length()
		{
			return TotalLines;
		}

		void clear()
		{
			TotalLines = 0;
			VecStr::clear();
		}

		void push_back(std::string line)
		{
			if (line.find("<!--") == NOT_FOUND) TotalLines++;

			VecStr::emplace_back(line);
		}

		void pop_back()
		{
			if (VecStr::back().find("<!--") == NOT_FOUND) TotalLines--;

			VecStr::pop_back();
		}

		void insert(VecStr::iterator pos, const std::string& line)
		{
			if (line.find("<!--") == NOT_FOUND) TotalLines--;

			VecStr::insert(pos, line);
		}

		void insert(VecStr::iterator pos, VecStr::iterator whr, VecStr::iterator til)
		{
			for (auto whe = whr; whe < til; ++whe)
			{
				if (whe->find("<!--") == NOT_FOUND) TotalLines++;
			}

			VecStr::insert(pos, whr, til);
		}
	};

	std::unordered_map<std::string, std::unordered_map<std::string, AnimDataList>> newAnimData;		// character, unique code, vector<string>; memory to access each node
	VecStr animDataChar;											// order of the character
	std::unordered_map<std::string, VecStr> animDataHeader;			// order of the character's header
	std::unordered_map<std::string, VecStr> animDataInfo;			// order of the character's info data
#endif
};
