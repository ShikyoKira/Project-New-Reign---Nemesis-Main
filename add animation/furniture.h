#ifndef FURNITURE_H_
#define FURNITURE_H_

#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <sstream>
#include <unordered_map>
#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>
#include "registeranimation.h"
#include "animationutility.h"
#include "animationthread.h"
#include "alphanum.hpp"
#include "templatetree.h"

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct MDException {};

class Furniture
{
private:
	std::string ZeroEvent;
	std::string ZeroVariable;

	std::string behaviorFile;
	std::string filepath;
	std::string filename;
	std::string format;
	std::string strID;
	vecstr clipname;
	vecstr eventID;
	vecstr variableID;
	SSMap IDExist;
	std::shared_ptr<single> subFunctionIDs;
	SSMap mixOptRegis;
	ImportContainer* newImport;
	newAnimLock* atomicLock;
	ImportContainer addition;
	bool isLastOrder = false;
	bool hasDuration = false;
	bool isEnd = false;
	bool known;
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
	std::unordered_map<std::string, vecstr> furniturelines;
	std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>> asdlines;
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> animdatalines;
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition;
	std::vector<std::shared_ptr<animationInfo>> groupAnimInfo;

	void multiChoice(std::string& line, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, animationutility utility);
	bool newCondition(std::string condition, vecstr& storeline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, animationutility utility);
	bool specialCondition(std::string condition, vecstr& storeline, int linecount, animationutility utility);
	bool GetFirstCondition(std::string firstCondition, vecstr optionInfo, int numline, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, bool isNot);
	bool firstConditionProcess(std::string condition, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, bool isNot, int numline, animationutility utility);
	bool secondConditionProcess(std::string condition, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline, animationutility utility);
	void stateReplacer(std::string& line, std::string statenum, int stateID, int linecount, bool hasGroup, std::string otherAnimOrder = "", bool otherAnim = false);
	void processing(std::string& newline, vecstr& storeline, std::string masterFormat, int linecount, id eventid, id variableid, std::vector<int> stateID, std::vector<int> stateCountMultiplier, bool hasGroup, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
	void AnimDataLineProcess(vecstr originallines, vecstr& newlines, std::string format, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, std::vector<int> ASD = { 0 });

public:
	Furniture(std::unordered_map<std::string, vecstr> furnitureformat, std::string formatname, int furniturecount, std::string filepath, animationInfo& behaviorOption);
	std::string GetFilePath();
	vecstr GetEventID();
	vecstr GetVariableID();
	SSMap GetMixOpt();
	ImportContainer GetAddition();
	std::vector<std::shared_ptr<animationInfo>> GetGroupAnimInfo();
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> GetGroupAddition();
	inline void newID();
	void GetFurnitureLine(std::shared_ptr<vecstr> generatedlines, std::string behaviorFile, int ID, ImportContainer& import, id eventid, id variableid, std::vector<int>& stateID, std::vector<int> stateCountMultiplier, bool hasGroup, bool isCore, std::shared_ptr<group> groupFunction, std::shared_ptr<single> singleFunction, newAnimLock& animLock);
	void GetAnimSetData(std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newASDLines);
	void GetAnimData(std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimDataLines);
	void existingASDProcess(vecstr ASDLines, std::map<int, vecstr>& extract, std::vector<int> ASD = { 0 });
	void addAnimData(std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> animdata);
	void addAnimSetData(std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>> animsetdata);
	void storeAnimObject(vecstr animobjects, std::string listFilename, int lineCount);
	void addGroupAnimInfo(std::vector<std::shared_ptr<animationInfo>> animInfo);
	void setZeroEvent(std::string eventname);
	void setZeroVariable(std::string variablename);
	void setLastOrder(int order);
	void setOrder(int curOrder);
	int getOrder();
	int getNextID(std::string behavior);
	bool isLast();
	bool isKnown();
	std::string addOnConverter(std::string curline, vecstr elements);

	std::string mainAnimEvent;
	std::string coreModID;
};

template <class T>
inline int numDigits(T number);
inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened);
inline bool clearGroupNum(std::string option2, std::unordered_map<std::string, bool> optionPicked, bool isNot, std::unordered_map<std::string, bool> groupOption);
std::string getOption(std::string curline, bool multi);
extern void motionDataReplacer(std::string& change, std::string format, std::string filename, std::string behaviorFile, int linecount, std::vector<std::shared_ptr<animationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool& isChange);
extern void rotationDataReplacer(std::string& change, std::string format, std::string filename, std::string behaviorFile, int linecount, std::vector<std::shared_ptr<animationInfo>>& groupAnimInfo, int order, int lastOrder, bool isLastOrder, int animMulti, bool& isChange);
extern bool isEquation(std::string& line);
extern int openEndBracket(std::string& line, char openBrac, char closeBrac, std::string format, std::string filename, int linecount);
extern void CRC32Replacer(std::string& line, std::string format, std::string behaviorFile, int linecount);
extern void addOnReplacer(std::string& line, std::string filename, std::unordered_map<std::string, vecstr> addOn, ImportContainer addition, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition, SSMap mixOpt, unsigned int count, std::string format, int numline);
extern void animObjectReplacer(std::string& line, std::string filename, std::unordered_map<int, vecstr> AnimObjec, std::string format, int linecount, int optionMulti, bool otherAnim = false, std::string animNum = "");
extern void eventIDReplacer(std::string& line, std::string format, std::string filename, id eventid, std::string firstEvent, int linecount);
extern void variableIDReplacer(std::string& line, std::string format, std::string filename, id variableid, std::string firstVariable, int linecount);
extern vecstr GetOptionInfo(std::string line, std::string format, std::string filename, int numline, size_t lastOrder, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, bool allowNoFixAnim, bool isCondition, int order = -1, int animMulti = -1, int optionMulti = -1);

#endif