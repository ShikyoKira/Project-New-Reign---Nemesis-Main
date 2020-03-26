#ifndef ANIMATIONDATA_H_
#define ANIMATIONDATA_H_

#include "utilities/writetextfile.h"

#include "generate/animationsetdata.h"
#include "generate/generator_utility.h"

#include "generate/animation/newanimation.h"

typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct AnimDataFunct
{
	bool skip = false;
	bool isNew = false;
	bool isCondition = false;
	bool nextCondition = false;
	bool isConditionOri = false;

	int conditionOpen = false;
};

namespace AnimDataFormat
{
	enum position
	{
		totallinecount,
		behaviorfilecount,
		behaviorfilelist,
		animationname,
		uniquecode,
		eventnamecount,
		eventnamelist,
		totalcliplength,
		motiondatacount,
		motiondatalist,
		rotationdatacount,
		rotationdatalist,
		unknown2,
		unknown3,
		unknown4,
		unknown5,
		space,
		xerror,
		null
	};
}

struct AnimDataPack
{
	// anim data
	std::string name;
	std::string uniquecode;
	std::string unknown1;
	std::string unknown2;
	std::string unknown3;

	vecstr eventname;
};

struct InfoDataPack
{
	std::string uniquecode;
	std::string duration;

	vecstr motiondata;
	vecstr rotationdata;
};

struct AnimDataProject
{
	vecstr behaviorlist;
	std::string childActive;
	std::string projectActive;

	std::vector<AnimDataPack> animdatalist;
	std::vector<InfoDataPack> infodatalist;

	AnimDataProject() {}
	AnimDataProject(vecstr animdatafile, std::string filename, std::string filepath, std::string modcode = "nemesis");

	int GetAnimTotalLine();
	int GetInfoTotalLine();
};

struct MasterAnimData
{
	struct AnimDataList : vecstr
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
			vecstr::clear();
		}

		void push_back(std::string line)
		{
			if (line.find("<!--") == NOT_FOUND) TotalLines++;

			vecstr::emplace_back(line);
		}

		void pop_back()
		{
			if (vecstr::back().find("<!--") == NOT_FOUND) TotalLines--;

			vecstr::pop_back();
		}

		void insert(vecstr::iterator pos, const std::string& line)
		{
			if (line.find("<!--") == NOT_FOUND) TotalLines--;

			vecstr::insert(pos, line);
		}

		void insert(vecstr::iterator pos, vecstr::iterator whr, vecstr::iterator til)
		{
			for (auto whe = whr; whe < til; ++whe)
			{
				if (whe->find("<!--") == NOT_FOUND) TotalLines++;
			}

			vecstr::insert(pos, whr, til);
		}
	};

	std::unordered_map<std::string, std::unordered_map<std::string, AnimDataList>> newAnimData;		// character, unique code, vector<string>; memory to access each node
	vecstr animDataChar;											// order of the character
	std::unordered_map<std::string, vecstr> animDataHeader;			// order of the character's header
	std::unordered_map<std::string, vecstr> animDataInfo;			// order of the character's info data
};

AnimDataFormat::position AnimDataPosition(vecstr animData, std::string character, std::string header, std::string modcode, std::string filepath, int linecount, int type, bool muteError = false);
AnimDataFormat::position AnimDataConvert(int type, int position, bool muteError);

#endif