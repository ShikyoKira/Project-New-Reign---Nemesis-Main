#ifndef ANIMATIONDATA_H_
#define ANIMATIONDATA_H_

#include "generator_utility.h"
#include "animationutility.h"
#include "add animation\furniture.h"
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

struct clipinfo
{
	double duration;
	std::vector<int> uniqueID;
	vecstr motiondata;
	vecstr rotationdata;
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
		totallinecount = 1,
		behaviorfilecount = 2,
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

extern void AnimDataCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::vector<std::unique_ptr<registerAnimation>>& animationList, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, std::unordered_map<std::string, var> AnimVar, std::unordered_map<std::string, std::vector<std::string>> modAnimBehavior);
extern AnimDataFormat::position AnimDataPosition(vecstr animData, std::string character, std::string header, std::string modcode, int linecount, int type, bool silentError = false);

AnimDataFormat::position AnimDataConvert(int type, int position, bool silentError);
std::unordered_map<std::string, vecstr> newAnimationList();

#endif