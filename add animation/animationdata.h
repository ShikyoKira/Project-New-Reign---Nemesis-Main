#ifndef ANIMATIONDATA_H_
#define ANIMATIONDATA_H_

#include "generator_utility.h"
#include "animationutility.h"
#include "add animation\furniture.h"
#include "add animation\animationsetdata.h"
#include "functions\functionwriter.h"
#include <boost\process.hpp>

typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct IDCatcher
{
private:
	int ID;
	int line;

public:
	IDCatcher(int id, int curline);
	int getID();
	int getLine();
};

struct AnimDataFunct
{
	bool isNew = false;
	bool isCondition = false;
	bool isConditionOri = false;
	bool nextCondition = false;
	bool skip = false;

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
	std::string unknown1;
	vecstr behaviorlist;
	std::string unknown2;

	std::vector<AnimDataPack> animdatalist;
	std::vector<InfoDataPack> infodatalist;

	AnimDataProject() {}
	AnimDataProject(vecstr animdatafile, std::string filename, std::string filepath, std::string modcode = "nemesis");
	int GetAnimTotalLine();
	int GetInfoTotalLine();
};

void BehaviorListProcess(AnimDataProject& storeline, int& startline, vecstr& animdatafile, std::string project, std::string modcode);
void AnimDataProcess(std::vector<AnimDataPack>& storeline, int& startline, vecstr& animdatafile, std::string project, std::string modcode);
void InfoDataProcess(std::vector<InfoDataPack>& storeline, int& startline, vecstr& animdatafile, std::string project, std::string modcode);
extern void AnimDataCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::vector<std::unique_ptr<registerAnimation>>& animationList, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation);
extern AnimDataFormat::position AnimDataPosition(vecstr animData, std::string character, std::string header, std::string modcode, int linecount, int type, bool muteError = false);

AnimDataFormat::position AnimDataConvert(int type, int position, bool muteError);

#endif