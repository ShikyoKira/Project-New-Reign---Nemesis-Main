#ifndef ANIMATIONSETDATA_H_
#define ANIMATIONSETDATA_H_

#include <boost\algorithm\string.hpp>
#include "furniture.h"
#include "gettemplate.h"
#include "generator_utility.h"
#include "functions\writetextfile.h"
#include "add animation\furniture.h"
#include "alphanum.hpp"

struct attackdata
{
	bool isNew = false;
	bool isMulti = false;
	std::string data;
};

struct equip
{
	bool isNew = false;
	bool isMulti = false;
	std::string name;
};

struct typepack
{
	bool isNew = false;
	bool isMulti = false;
	std::string name;
	std::string equiptype1;
	std::string equiptype2;
};

struct animpack
{
	bool isNew = false;
	bool isMulti = false;
	std::string name;
	std::string unknown;
	std::vector<attackdata> attack;
};

struct crc32
{
	bool isNew = false;
	bool isMulti = false;
	std::string filepath;
	std::string filename;
	std::string fileformat;
};

struct datapack
{
	std::vector<equip> equiplist;		// anim data set, equip list
	std::vector<typepack> typelist;		// anim data set, type list
	std::vector<animpack> animlist;		// anim data set, list of animpack
	std::vector<crc32> crc32list;		// anim data set, crc32 list
};

struct AnimationDataProject
{
	bool isNew = false;					// is this new set of animdata data
	std::string mod;					// modcode
	std::map<std::string, datapack, alphanum_less> datalist;		// anim data set

	AnimationDataProject() {}
	AnimationDataProject(int& startline, vecstr& animdatafile, std::string filename, std::string projectname);
};

struct ASDFunct
{
	bool isNew = false;
	bool isCondition = false;
	bool isConditionOri = false;
	bool nextCondition = false;
	bool skip = false;

	int conditionOpen = false;
};

namespace ASDFormat
{
	enum position
	{
		V3,
		equipcount,
		equiplist,
		typecount,
		typelist,
		unknown1,
		unknown2,
		animpackcount,
		animpackname,
		unknown3,
		attackcount,
		attacklist,
		crc32count,
		crc32list,
		xerror,
		null
	};
}

struct MasterAnimSetData
{
	vecstr projectList;															// order of the project
	std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>> newAnimSetData;	// project, header, vector<string>; memory to access each node
};

extern void combineExtraction(vecstr& storeline, std::map<int, vecstr> extract, std::string project, std::string header);

ASDFormat::position ASDPosition(vecstr animData, std::string character, std::string header, std::string modcode, int linecount, bool muteError);
ASDFormat::position ASDConvert(int position, bool muteError);

int PositionLineCondition(int& i, double curID, int linecount, vecstr animDataSet, std::unordered_map<int, ASDFunct>& marker, std::string modcode, std::string header,
	bool last, bool muteError);

#endif