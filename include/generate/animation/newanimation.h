#ifndef FURNITURE_H_
#define FURNITURE_H_

#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <sstream>
#include <map>
#include <unordered_map>

#include <boost/regex.hpp>

#include "generate/AnimationUtility.h"

#include "utilities/alphanum.hpp"

#include "generate/animation/animationthread.h"
#include "generate/animation/registeranimation.h"

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;
typedef std::unordered_map<std::string, int> id;


class AnimTemplate;
struct condset;
struct proc;

struct MDException {};

class NewAnimation
{
private:
	std::string zeroEvent;
	std::string zeroVariable;

	std::string behaviorFile;
	std::string filepath;
	std::string filename;
	std::string format;
	std::string strID;

	// vecstr clipname;		obsolete
	vecstr eventID;
	vecstr variableID;
	SSMap IDExist;
	SSMap mixOptRegis;
	ImportContainer* newImport;
	NewAnimLock* atomicLock;
	ImportContainer addition;

	bool isLastOrder = false;
	bool hasDuration = false;
	bool isEnd = false;
	bool elementCatch = false;
	bool known;

	size_t templatesize;
	int furnitureCount = 0;
	int nextFunctionID;
	int order = 0;
	int lastOrder = 0;
	double duration = 0;

	std::unordered_map<int, vecstr> AnimObject;
	std::unordered_map<std::string, bool> optionPicked;
	std::unordered_map<std::string, int> optionPickedCount;
	std::unordered_map<std::string, vecstr> addOn;
	std::unordered_map<std::string, vecstr> linkedOption;
	std::unordered_map<std::string, vecstr> mixOptRever;
	std::unordered_map<std::string, vecstr> rawtemplate;
	//std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>> asdlines;
	//std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> animdatalines;
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition;
	std::vector<std::shared_ptr<AnimationInfo>> groupAnimInfo;

	std::shared_ptr<std::unordered_map<std::string, AnimTemplate>> animtemplate;
	std::unordered_map<std::string, std::unordered_map<std::string, AnimTemplate>> animdatatemplate;
	std::unordered_map<std::string, std::map<std::string, AnimTemplate, alphanum_less>> asdtemplate;

public:
	std::shared_ptr<single> subFunctionIDs;
	std::string mainAnimEvent;
	std::string coreModID;

private:
	bool singleCondition(std::string condition, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, AnimationUtility utility);
	bool andLoop(std::string condition, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, AnimationUtility utility);
	bool andOrParenthesis(size_t c_and, size_t c_or, std::string condition, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
		int numline, AnimationUtility utility);
	bool andParenthesis(std::string condition, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, AnimationUtility utility);
	bool newCondition(std::string condition, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, AnimationUtility utility);
	bool specialCondition(std::string condition, vecstr& storeline, int linecount, AnimationUtility utility);
	bool GetFirstCondition(std::string firstCondition, vecstr optionInfo, int numline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, bool isNot);
	bool conditionProcess(std::string condition, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, bool isNot, int numline, AnimationUtility utility);
	void stateReplacer(std::string& line, std::string statenum, int stateID, int linecount, std::string otherAnimOrder = "", bool otherAnim = false);
	bool addOnConverter(int& optionMulti, int& endMulti, vecstr& optionInfo, std::shared_ptr<AnimationInfo> animInfo, bool second);
	void processing(std::string& newline, vecstr& storeline, std::string masterFormat, int linecount, id eventid, id variableid, std::vector<int> stateID,
		std::vector<int> stateCountMultiplier, bool hasGroup, int optionMulti = -1, int animMulti = -1);
	void AnimDataLineProcess(vecstr originallines, vecstr& newlines, std::string format, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
		std::vector<int> ASD = { 0 });

	void AnimDataLineProcess(AnimTemplate* originaltemplate, std::shared_ptr<vecstr> newlines, std::string format, std::string project, std::string header,
		std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, std::vector<int> ASD = { 0 });

public:
	NewAnimation(std::shared_ptr<std::unordered_map<std::string, AnimTemplate>> animlines, std::string formatname, std::unordered_map<std::string, vecstr> furnituretemplate,
		int furniturecount, std::string filepath, AnimationInfo& behaviorOption);

	std::string GetFilePath();
	std::string GetFormatName();
	vecstr GetEventID();
	vecstr GetVariableID();
	SSMap GetMixOpt();
	ImportContainer GetAddition();
	std::vector<std::shared_ptr<AnimationInfo>> GetGroupAnimInfo();
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> GetGroupAddition();

	inline void newID();

	void OutputCheck(std::shared_ptr<vecstr> generatedlines, proc& process, condset* curset, bool& norElement, int& openRange, size_t& elementLine, int& counter,
		id& eventid, id&variableid, std::vector<int> fixedStateID, std::vector<int> stateCountMultiplier, bool hasGroup, bool& negative,
		std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, std::shared_ptr<group> groupFunction, int optionMulti = -1, int animMulti = -1);
	void GetNewAnimationLine(std::shared_ptr<vecstr> generatedlines, std::string behaviorFile, int ID, ImportContainer& import, id eventid, id variableid,
		std::vector<int>& stateID, std::vector<int> stateCountMultiplier, bool hasGroup, bool isCore, std::shared_ptr<group> groupFunction,
		std::shared_ptr<single> singleFunction, NewAnimLock& animLock);
	void GetAnimSetData(std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newASDLines);
	void GetAnimData(std::unordered_map<std::string, std::map<std::string, vecstr>>& newAnimDataLines);
	void existingASDProcess(vecstr ASDLines, std::map<int, vecstr>& extract, std::vector<int> ASD = { 0 });
	void hasProcessing(std::string& line, bool& norElement, int& openRange, int i, std::shared_ptr<vecstr> generatedlines, size_t& elementLine, int& counter, id& eventid,
		id& variableid, std::vector<int> fixedStateID, std::vector<int> stateCountMultiplier, bool hasGroup, bool& negative,
		std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, std::shared_ptr<group> groupFunction, int optionMulti = -1, int animMulti = -1);
	void multiChoice(std::string& line, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, AnimationUtility utility);

	//void addAnimData(std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> animdata);
	void addAnimData(std::unordered_map<std::string, std::unordered_map<std::string, AnimTemplate>> animdata);

	//void addAnimSetData(std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>> animsetdata);
	void addAnimSetData(std::unordered_map<std::string, std::map<std::string, AnimTemplate, alphanum_less>> animsetdata);

	void storeAnimObject(vecstr animobjects, std::string listFilename, int lineCount);
	void addGroupAnimInfo(std::vector<std::shared_ptr<AnimationInfo>> animInfo);
	void setZeroEvent(std::string eventname);
	void setZeroVariable(std::string variablename);
	void setLastOrder(int order);
	void setOrder(int curOrder);

	int getOrder();
	int getNextID(std::string behavior);

	bool isLast();
	bool isKnown();
};

inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened)
{
	for (int k = condition - 1; k > 0; k--)
	{
		if (!IsConditionOpened[k])
		{
			return false;
		}
	}

	return true;
}

template <class T>
int numDigits(T number);
bool clearGroupNum(std::string option2, std::unordered_map<std::string, bool> optionPicked, bool isNot, std::unordered_map<std::string, bool> groupOption);
void motionDataReplacer(std::string& change, std::string format, std::string filename, std::string behaviorFile, int linecount,
	std::vector<std::shared_ptr<AnimationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool& isChange);
void rotationDataReplacer(std::string& change, std::string format, std::string filename, std::string behaviorFile, int linecount,
	std::vector<std::shared_ptr<AnimationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool& isChange);
bool isEquation(std::string& line);
int openEndBracket(std::string& line, char openBrac, char closeBrac, std::string format, std::string filename, int linecount);
void CRC32Replacer(std::string& line, std::string format, std::string behaviorFile, int linecount);
void addOnReplacer(std::string& line, std::string filename, std::unordered_map<std::string, vecstr> addOn, ImportContainer addition,
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition, SSMap mixOpt, unsigned int count, std::string format, int numline);
void animObjectReplacer(std::string& line, std::string filename, std::unordered_map<int, vecstr> AnimObjec, std::string format, int linecount,
	int optionMulti, bool otherAnim = false, std::string animNum = "");
void eventIDReplacer(std::string& line, std::string format, std::string filename, id eventid, std::string firstEvent, int linecount);
void variableIDReplacer(std::string& line, std::string format, std::string filename, id variableid, std::string firstVariable, int linecount);
vecstr GetOptionInfo(std::string line, std::string format, std::string filename, int numline, size_t lastOrder, std::vector<std::shared_ptr<AnimationInfo>> groupAnimInfo,
	bool allowNoFixAnim, bool isCondition, int order = -1, int animMulti = -1, int optionMulti = -1);

#endif