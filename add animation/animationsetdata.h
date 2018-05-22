#ifndef ANIMATIONSETDATA_H_
#define ANIMATIONSETDATA_H_

#include <boost\algorithm\string.hpp>
#include "furniture.h"
#include "gettemplate.h"
#include "generator_utility.h"
#include "functions\functionwriter.h"
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

	std::unordered_map<std::string, std::string> equiptype1Mod;							// modcode, modified equiptype1; modified equiptype1 by the mod
	std::unordered_map<std::string, std::string> equiptype2Mod;							// modcode, modified equiptype2; modified equiptype2 by the mod
};

struct animpack
{
	bool isNew = false;
	bool isMulti = false;
	std::string name;
	std::string unknown;
	std::vector<attackdata> attack;

	std::unordered_map<std::string, std::string> unknownMod;									// modcode, modified unknown; modified unknown by the mod
	std::unordered_map<std::string, std::unordered_map<bool, std::set<attackdata>>> attackMod;	// modcode, add/minus, list of attack; add or delete equip list
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

	std::unordered_map<std::string, std::unordered_map<bool, std::vector<equip>>> equipMod;		// modcode, add/minus, list of equip; add or delete equip list
	std::unordered_map<std::string, std::unordered_map<bool, std::vector<typepack>>> typeMod;	// modcode, add/minus, list of typelist; add or delete typepack list
	std::unordered_map<std::string, std::unordered_map<bool, std::vector<animpack>>> animMod;	// modcode, add/minus, list of animlist; add or delete animpack list
	std::unordered_map<std::string, std::unordered_map<bool, std::vector<crc32>>> crc32Mod;		// modcode, add/minus, list of crc32; add or delete crc32 list
	
	std::unordered_map<std::string, std::unordered_map<std::string, typepack>> alterTypePackMod;	// original typepack name, modcode, typepack; extraction of modded version
	std::unordered_map<std::string, std::unordered_map<std::string, animpack>> alterAnimPackMod;	// original animpack name, modcode, animpack; extraction of modded version
};

struct AnimationDataProject
{
	bool isNew = false;					// is this new set of animdata data
	std::string mod;					// modcode
	std::map<std::string, datapack, alphanum_less<std::string>> datalist;		// anim data set

	std::unordered_map<std::string, std::unordered_map<bool, std::map<std::string, datapack, alphanum_less<std::string>>>> dataMod;		// modcode, add/minus, list of data pack

	AnimationDataProject() {}
	AnimationDataProject(int& startline, vecstr& animdatafile, std::string filename);
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

void ASDCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, std::unordered_map<std::string, var> AnimVar);

extern void combineExtraction(vecstr& storeline, std::map<int, vecstr> extract, std::string project, std::string header);

ASDFormat::position ASDPosition(vecstr animData, std::string character, std::string header, std::string modcode, int linecount, bool muteError);
ASDFormat::position ASDConvert(int position, bool muteError);

void DataPackProcess(std::map<std::string, datapack, alphanum_less<std::string>>& storeline, int& startline, vecstr& animdatafile, std::string filename);
void EquipPackProcess(std::vector<equip>& storeline, int& startline, vecstr& animdatafile, std::string filename, std::string header);
void TypePackProcess(std::vector<typepack>& storeline, int& startline, vecstr& animdatafile, std::string filename, std::string header);
void AnimPackProcess(std::vector<animpack>& storeline, int& startline, vecstr& animdatafile, std::string filename, std::string header);
void CRC32Process(std::vector<crc32>& storeline, int& startline, vecstr& animdatafile, std::string filename, std::string header);

int PositionLineCondition(int& i, double curID, int linecount, vecstr animDataSet, std::unordered_map<int, ASDFunct>& marker, std::string modcode, std::string header, bool last, bool muteError);

#endif