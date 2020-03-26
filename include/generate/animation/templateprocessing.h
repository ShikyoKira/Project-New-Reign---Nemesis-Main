#ifndef TEMPLATEPROCESSING_H_
#define TEMPLATEPROCESSING_H_

#include <unordered_map>

#include <boost/regex.hpp>

#include "generate/animation/animationthread.h"

struct proc;
struct multichoice;

namespace nemesis
{
	struct smatch
	{
		std::vector<size_t> positionlist;
		vecstr match;

		std::string operator[](int number);
		size_t position(int number);
		size_t size();
	};

	bool regex_search(std::string line, nemesis::smatch& match, boost::regex rgx);
}

struct range
{
	size_t front;
	size_t back;
	size_t size;
	vecstr olddata;
	std::vector<int> olddataint;
	void (proc::*func)(range, vecstr&);

	range() {}
	range(size_t n_front, size_t n_back, void (proc::*n_func)(range, vecstr&));
	range(size_t n_front, size_t n_back, vecstr n_olddata, void (proc::*n_func)(range, vecstr&));
	range(size_t n_front, size_t n_back, std::vector<int> n_olddataint, void (proc::*n_func)(range, vecstr&));
	range(size_t n_front, size_t n_back, std::vector<int> n_olddataint, vecstr n_olddata, void (proc::*n_func)(range, vecstr&));
};

struct block
{
	std::map<int, std::vector<range>> blocksize;
};

struct choice_c
{
	size_t front;
	size_t back;

	choice_c(size_t posA, size_t posB);
};

struct proc
{
	NewAnimLock* animLock;

	// template info
	bool hasGroup;
	bool* negative;
	bool* isEnd;
	bool* norElement;
	bool* elementCatch;
	bool hasDuration;
	
	int* openRange;
	int* counter;

	size_t numline;
	size_t stateCount = 0;
	size_t* elementLine;

	int furnitureCount;

	double duration;

	id* eventid;
	id* variableid;
	
	std::string format;
	std::string masterformat;
	std::string behaviorFile;
	std::string filepath;
	std::string filename;
	std::string mainAnimEvent;
	std::string multiOption = "";
	std::string project;
	std::string header;

	std::vector<int> fixedStateID;
	std::vector<int> stateCountMultiplier;

	SSMap* IDExist;
	ImportContainer* addition;
	ImportContainer* newImport;

	std::unordered_map<int, vecstr>* AnimObject;
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>* groupAddition;
	std::vector<std::unordered_map<std::string, bool>>* groupOptionPicked;
	std::vector<std::vector<std::unordered_map<std::string, bool>>>* masterOptionPicked;
	
	std::shared_ptr<group> groupFunction;
	std::shared_ptr<master> masterFunction;

	std::shared_ptr<vecstr> generatedlines;
	
	NewAnimation* curAnim;
	GroupTemplate* curGroup;

	int optionMulti = -1;
	int animMulti = -1;
	int groupMulti = 0;
	int order;
	int lastorder;

	bool isMC = false;
	bool isGroup = false;
	bool isMaster = false;
	
	std::string* line;
	std::string* strID;
	std::string zeroEvent;
	std::string zeroVariable;

	std::vector<choice_c> failed;
	std::shared_ptr<choice_c> captured;

	std::unordered_map<int, bool> hasMC;
	std::unordered_map<int, std::vector<int>> brackets;
	std::unordered_map<int, std::vector<void (proc::*)()>> process;
	std::unordered_map<int, block> lineblocks;							// numline, blocksize (order), blocks of same size
	std::unordered_map<int, std::vector<range>> choiceblocks;			// numline, blocks

	std::unordered_map<int, std::vector<multichoice>> multiChoice;		// numline, list of multichoice

	proc() {}

	void Register(std::string n_format, std::string masterformat, std::string n_behaviorFile, std::string n_filepath, std::string n_filename, 
		std::string n_mainAnimEvent, std::string& n_strID, std::string n_zeroEvent, std::string n_zeroVariable, bool n_hasGroup, bool& n_negative, bool& n_isEnd,
		bool& n_norElement, bool& n_elementCatch, bool hasDuration, double duration, int& n_openRange, int& n_counter, size_t& n_elementLine, int n_furnitureCount,
		id& n_eventid, id& n_variableid, std::vector<int> n_fixedStateID, std::vector<int> n_stateCountMultiplier, NewAnimLock* n_animLock, int n_order,
		int n_lastorder, SSMap& n_IDExist, std::unordered_map<int, vecstr>& n_AnimObject, ImportContainer& n_addition, ImportContainer* n_newImport,
		std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& n_groupAddition,
		std::vector<std::unordered_map<std::string, bool>>& n_groupOptionPicked, std::shared_ptr<group> n_groupFunction, std::shared_ptr<vecstr> n_generatedlines,
		NewAnimation* n_curAnim);

	void installBlock(range blok, int curline);
	void installBlock(range blok, int curline, std::vector<multichoice> n_condiiton);

	// processes
	void relativeNegative(range blok, vecstr& blocks);
	void compute(range blok, vecstr& blocks);
	void rangeCompute(range blok, vecstr& blocks);
	void upCounter(range blok, vecstr& blocks);
	void upCounterPlus(range blok, vecstr& blocks);
	void animCount(range blok, vecstr& blocks);
	void multiChoiceRegis(range blok, vecstr& blocks);
	void groupIDRegis(range blok, vecstr& blocks);
	void IDRegis(range blok, vecstr& blocks);
	void IDRegisAnim(range blok, vecstr& blocks);
	void IDRegisGroup(range blok, vecstr& blocks);
	void IDRegisMaster(range blok, vecstr& blocks);
	void computation(range blok, vecstr& blocks);

	// end functions
	// group
	void endMultiGroup(range blok, vecstr& blocks);
	void endFirstGroup(range blok, vecstr& blocks);
	void endNextGroup(range blok, vecstr& blocks);
	void endBackGroup(range blok, vecstr& blocks);
	void endLastGroup(range blok, vecstr& blocks);
	void endNumGroup(range blok, vecstr& blocks);

	// master
	void endMultiMaster(range blok, vecstr& blocks);
	void endFirstMaster(range blok, vecstr& blocks);
	void endLastMaster(range blok, vecstr& blocks);
	void endNumMaster(range blok, vecstr& blocks);

	void endSingle(range blok, vecstr& blocks);


	// state functions
	// group
	void stateMultiGroup(range blok, vecstr& blocks);
	void stateFirstGroup(range blok, vecstr& blocks);
	void stateNextGroup(range blok, vecstr& blocks);
	void stateBackGroup(range blok, vecstr& blocks);
	void stateLastGroup(range blok, vecstr& blocks);
	void stateNumGroup(range blok, vecstr& blocks);

	// master to group
	void stateMultiMasterToGroup(range blok, vecstr& blocks);
	/*void stateFirstMasterToGroup(range blok, vecstr& blocks);
	void stateLastMasterToGroup(range blok, vecstr& blocks);		Not needed
	void stateNumMasterToGroup(range blok, vecstr& blocks);*/

	// master
	void stateMultiMaster(range blok, vecstr& blocks);
	void stateFirstMaster(range blok, vecstr& blocks);
	void stateLastMaster(range blok, vecstr& blocks);
	void stateNumMaster(range blok, vecstr& blocks);

	void stateSingle(range blok, vecstr& blocks);


	// filepath functions
	// group
	void filepathMultiGroup(range blok, vecstr& blocks);
	void filepathFirstGroup(range blok, vecstr& blocks);
	void filepathNextGroup(range blok, vecstr& blocks);
	void filepathBackGroup(range blok, vecstr& blocks);
	void filepathLastGroup(range blok, vecstr& blocks);
	void filepathNumGroup(range blok, vecstr& blocks);

	// master
	void filepathMultiMaster(range blok, vecstr& blocks);
	void filepathFirstMaster(range blok, vecstr& blocks);
	void filepathLastMaster(range blok, vecstr& blocks);
	void filepathNumMaster(range blok, vecstr& blocks);

	void filepathSingle(range blok, vecstr& blocks);


	// filename functions
	// group
	void filenameMultiGroup(range blok, vecstr& blocks);
	void filenameFirstGroup(range blok, vecstr& blocks);
	void filenameNextGroup(range blok, vecstr& blocks);
	void filenameBackGroup(range blok, vecstr& blocks);
	void filenameLastGroup(range blok, vecstr& blocks);
	void filenameNumGroup(range blok, vecstr& blocks);

	// master
	void filenameMultiMaster(range blok, vecstr& blocks);
	void filenameFirstMaster(range blok, vecstr& blocks);
	void filenameLastMaster(range blok, vecstr& blocks);
	void filenameNumMaster(range blok, vecstr& blocks);

	void filenameSingle(range blok, vecstr& blocks);


	// path functions
	void pathSingle(range blok, vecstr& blocks);

	// AnimObject functions
	// group
	void AOMultiGroupA(range blok, vecstr& blocks);
	void AOMultiGroupB(range blok, vecstr& blocks);
	void AOFirstGroupA(range blok, vecstr& blocks);
	void AOFirstGroupB(range blok, vecstr& blocks);
	void AONextGroupA(range blok, vecstr& blocks);
	void AONextGroupB(range blok, vecstr& blocks);
	void AOBackGroupA(range blok, vecstr& blocks);
	void AOBackGroupB(range blok, vecstr& blocks);
	void AOLastGroupA(range blok, vecstr& blocks);
	void AOLastGroupB(range blok, vecstr& blocks);
	void AONumGroupA(range blok, vecstr& blocks);
	void AONumGroupB(range blok, vecstr& blocks);

	// master
	void AOMultiMasterA(range blok, vecstr& blocks);
	void AOMultiMasterB(range blok, vecstr& blocks);
	void AOFirstMasterA(range blok, vecstr& blocks);
	void AOFirstMasterB(range blok, vecstr& blocks);
	void AOLastMasterA(range blok, vecstr& blocks);
	void AOLastMasterB(range blok, vecstr& blocks);
	void AONumMasterA(range blok, vecstr& blocks);
	void AONumMasterB(range blok, vecstr& blocks);

	void AOSingleA(range blok, vecstr& blocks);
	void AOSingleB(range blok, vecstr& blocks);

	// main_anim_event functions
	// group
	void MAEMultiGroup(range blok, vecstr& blocks);
	void MAEFirstGroup(range blok, vecstr& blocks);
	void MAENextGroup(range blok, vecstr& blocks);
	void MAEBackGroup(range blok, vecstr& blocks);
	void MAELastGroup(range blok, vecstr& blocks);
	void MAENumGroup(range blok, vecstr& blocks);

	//master
	void MAEMultiMaster(range blok, vecstr& blocks);
	void MAEFirstMaster(range blok, vecstr& blocks);
	void MAELastMaster(range blok, vecstr& blocks);
	void MAENumMaster(range blok, vecstr& blocks);

	void MAESingle(range blok, vecstr& blocks);

	// addOn functions
	// group
	void addOnMultiGroup(range blok, vecstr& blocks);
	void addOnFirstGroup(range blok, vecstr& blocks);
	void addOnNextGroup(range blok, vecstr& blocks);
	void addOnBackGroup(range blok, vecstr& blocks);
	void addOnLastGroup(range blok, vecstr& blocks);
	void addOnNumGroup(range blok, vecstr& blocks);

	// master
	void addOnMultiMaster(range blok, vecstr& blocks);
	void addOnFirstMaster(range blok, vecstr& blocks);
	void addOnLastMaster(range blok, vecstr& blocks);
	void addOnNumMaster(range blok, vecstr& blocks);

	void addOnSingle(range blok, vecstr& blocks);

	// last state functions
	void lastState(range blok, vecstr& blocks);

	// event ID functions
	void eventID(range blok, vecstr& blocks);

	// variable ID functions
	void variableID(range blok, vecstr& blocks);

	// crc32 functions
	void crc32(range blok, vecstr& blocks);

	// import functions
	void import(range blok, vecstr& blocks);

	// motion data functions
	// group
	void motionDataMultiGroup(range blok, vecstr& blocks);
	void motionDataFirstGroup(range blok, vecstr& blocks);
	void motionDataNextGroup(range blok, vecstr& blocks);
	void motionDataBackGroup(range blok, vecstr& blocks);
	void motionDataLastGroup(range blok, vecstr& blocks);
	void motionDataNumGroup(range blok, vecstr& blocks);

	// master
	void motionDataMultiMaster(range blok, vecstr& blocks);
	void motionDataFirstMaster(range blok, vecstr& blocks);
	void motionDataLastMaster(range blok, vecstr& blocks);
	void motionDataNumMaster(range blok, vecstr& blocks);

	void motionDataSingle(range blok, vecstr& blocks);

	// rotation data functions
	// group
	void rotationDataMultiGroup(range blok, vecstr& blocks);
	void rotationDataFirstGroup(range blok, vecstr& blocks);
	void rotationDataNextGroup(range blok, vecstr& blocks);
	void rotationDataBackGroup(range blok, vecstr& blocks);
	void rotationDataLastGroup(range blok, vecstr& blocks);
	void rotationDataNumGroup(range blok, vecstr& blocks);

	// master
	void rotationDataMultiMaster(range blok, vecstr& blocks);
	void rotationDataFirstMaster(range blok, vecstr& blocks);
	void rotationDataLastMaster(range blok, vecstr& blocks);
	void rotationDataNumMaster(range blok, vecstr& blocks);

	void rotationDataSingle(range blok, vecstr& blocks);

	// animOrder functions
	void animOrder(range blok, vecstr& blocks);

	// register animation
	void regisAnim(range blok, vecstr& blocks);

	// register behavior
	void regisBehavior(range blok, vecstr& blocks);

	// negative local
	void localNegative(range blok, vecstr& blocks);

	// getline
	void blocksCompile(vecstr blocks);
	
	// utilities
	bool isThisMaster();
	bool clearBlocks(range& blok, vecstr& blocks);
	void blockCheck(size_t front, size_t back);
	std::string combineBlocks(range& blok, vecstr& blocks);
	std::string combineBlocks(size_t front, size_t back, vecstr& blocks);
};

#endif
