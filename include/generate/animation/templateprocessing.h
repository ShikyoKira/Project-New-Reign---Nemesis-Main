#ifndef TEMPLATEPROCESSING_H_
#define TEMPLATEPROCESSING_H_

#include <unordered_map>

#include <boost/regex.hpp>

#include "utilities/scope.h"

#include "generate/animation/animationthread.h"

struct proc;

namespace nemesis
{
	struct MultiChoice;
	struct scope;
}

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

struct block
{
	std::map<int, std::vector<nemesis::scope>> blocksize;
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
	std::unordered_map<int, std::vector<nemesis::scope>> choiceblocks;			// numline, blocks

	std::unordered_map<int, std::vector<nemesis::MultiChoice>> multiChoice;		// numline, list of MultiChoice

	proc() {}

	void Register(std::string n_format, std::string masterformat, std::string n_behaviorFile, std::string n_filepath, std::string n_filename, 
		std::string n_mainAnimEvent, std::string& n_strID, std::string n_zeroEvent, std::string n_zeroVariable, bool n_hasGroup, bool& n_negative, bool& n_isEnd,
		bool& n_norElement, bool& n_elementCatch, bool hasDuration, double duration, int& n_openRange, int& n_counter, size_t& n_elementLine, int n_furnitureCount,
		id& n_eventid, id& n_variableid, std::vector<int> n_fixedStateID, std::vector<int> n_stateCountMultiplier, NewAnimLock* n_animLock, int n_order,
		int n_lastorder, SSMap& n_IDExist, std::unordered_map<int, vecstr>& n_AnimObject, ImportContainer& n_addition, ImportContainer* n_newImport,
		std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& n_groupAddition,
		std::vector<std::unordered_map<std::string, bool>>& n_groupOptionPicked, std::shared_ptr<group> n_groupFunction, std::shared_ptr<vecstr> n_generatedlines,
		NewAnimation* n_curAnim);

	void installBlock(nemesis::scope blok, int curline);
	void installBlock(nemesis::scope blok, int curline, std::vector<nemesis::MultiChoice> n_condiiton);

	// processes
	void relativeNegative(nemesis::scope blok, vecstr& blocks);
	void compute(nemesis::scope blok, vecstr& blocks);
	void rangeCompute(nemesis::scope blok, vecstr& blocks);
	void upCounter(nemesis::scope blok, vecstr& blocks);
	void upCounterPlus(nemesis::scope blok, vecstr& blocks);
	void animCount(nemesis::scope blok, vecstr& blocks);
	void multiChoiceRegis(nemesis::scope blok, vecstr& blocks);
	void groupIDRegis(nemesis::scope blok, vecstr& blocks);
	void IDRegis(nemesis::scope blok, vecstr& blocks);
	void IDRegisAnim(nemesis::scope blok, vecstr& blocks);
	void IDRegisGroup(nemesis::scope blok, vecstr& blocks);
	void IDRegisMaster(nemesis::scope blok, vecstr& blocks);
	void computation(nemesis::scope blok, vecstr& blocks);

	// end functions
	// group
	void endMultiGroup(nemesis::scope blok, vecstr& blocks);
	void endFirstGroup(nemesis::scope blok, vecstr& blocks);
	void endNextGroup(nemesis::scope blok, vecstr& blocks);
	void endBackGroup(nemesis::scope blok, vecstr& blocks);
	void endLastGroup(nemesis::scope blok, vecstr& blocks);
	void endNumGroup(nemesis::scope blok, vecstr& blocks);

	// master
	void endMultiMaster(nemesis::scope blok, vecstr& blocks);
	void endFirstMaster(nemesis::scope blok, vecstr& blocks);
	void endLastMaster(nemesis::scope blok, vecstr& blocks);
	void endNumMaster(nemesis::scope blok, vecstr& blocks);

	void endSingle(nemesis::scope blok, vecstr& blocks);


	// state functions
	// group
	void stateMultiGroup(nemesis::scope blok, vecstr& blocks);
	void stateFirstGroup(nemesis::scope blok, vecstr& blocks);
	void stateNextGroup(nemesis::scope blok, vecstr& blocks);
	void stateBackGroup(nemesis::scope blok, vecstr& blocks);
	void stateLastGroup(nemesis::scope blok, vecstr& blocks);
	void stateNumGroup(nemesis::scope blok, vecstr& blocks);

	// master to group
	void stateMultiMasterToGroup(nemesis::scope blok, vecstr& blocks);
	/*void stateFirstMasterToGroup(nemesis::scope blok, vecstr& blocks);
	void stateLastMasterToGroup(nemesis::scope blok, vecstr& blocks);		Not needed
	void stateNumMasterToGroup(nemesis::scope blok, vecstr& blocks);*/

	// master
	void stateMultiMaster(nemesis::scope blok, vecstr& blocks);
	void stateFirstMaster(nemesis::scope blok, vecstr& blocks);
	void stateLastMaster(nemesis::scope blok, vecstr& blocks);
	void stateNumMaster(nemesis::scope blok, vecstr& blocks);

	void stateSingle(nemesis::scope blok, vecstr& blocks);


	// filepath functions
	// group
	void filepathMultiGroup(nemesis::scope blok, vecstr& blocks);
	void filepathFirstGroup(nemesis::scope blok, vecstr& blocks);
	void filepathNextGroup(nemesis::scope blok, vecstr& blocks);
	void filepathBackGroup(nemesis::scope blok, vecstr& blocks);
	void filepathLastGroup(nemesis::scope blok, vecstr& blocks);
	void filepathNumGroup(nemesis::scope blok, vecstr& blocks);

	// master
	void filepathMultiMaster(nemesis::scope blok, vecstr& blocks);
	void filepathFirstMaster(nemesis::scope blok, vecstr& blocks);
	void filepathLastMaster(nemesis::scope blok, vecstr& blocks);
	void filepathNumMaster(nemesis::scope blok, vecstr& blocks);

	void filepathSingle(nemesis::scope blok, vecstr& blocks);


	// filename functions
	// group
	void filenameMultiGroup(nemesis::scope blok, vecstr& blocks);
	void filenameFirstGroup(nemesis::scope blok, vecstr& blocks);
	void filenameNextGroup(nemesis::scope blok, vecstr& blocks);
	void filenameBackGroup(nemesis::scope blok, vecstr& blocks);
	void filenameLastGroup(nemesis::scope blok, vecstr& blocks);
	void filenameNumGroup(nemesis::scope blok, vecstr& blocks);

	// master
	void filenameMultiMaster(nemesis::scope blok, vecstr& blocks);
	void filenameFirstMaster(nemesis::scope blok, vecstr& blocks);
	void filenameLastMaster(nemesis::scope blok, vecstr& blocks);
	void filenameNumMaster(nemesis::scope blok, vecstr& blocks);

	void filenameSingle(nemesis::scope blok, vecstr& blocks);


	// path functions
	void pathSingle(nemesis::scope blok, vecstr& blocks);

	// AnimObject functions
	// group
	void AOMultiGroupA(nemesis::scope blok, vecstr& blocks);
	void AOMultiGroupB(nemesis::scope blok, vecstr& blocks);
	void AOFirstGroupA(nemesis::scope blok, vecstr& blocks);
	void AOFirstGroupB(nemesis::scope blok, vecstr& blocks);
	void AONextGroupA(nemesis::scope blok, vecstr& blocks);
	void AONextGroupB(nemesis::scope blok, vecstr& blocks);
	void AOBackGroupA(nemesis::scope blok, vecstr& blocks);
	void AOBackGroupB(nemesis::scope blok, vecstr& blocks);
	void AOLastGroupA(nemesis::scope blok, vecstr& blocks);
	void AOLastGroupB(nemesis::scope blok, vecstr& blocks);
	void AONumGroupA(nemesis::scope blok, vecstr& blocks);
	void AONumGroupB(nemesis::scope blok, vecstr& blocks);

	// master
	void AOMultiMasterA(nemesis::scope blok, vecstr& blocks);
	void AOMultiMasterB(nemesis::scope blok, vecstr& blocks);
	void AOFirstMasterA(nemesis::scope blok, vecstr& blocks);
	void AOFirstMasterB(nemesis::scope blok, vecstr& blocks);
	void AOLastMasterA(nemesis::scope blok, vecstr& blocks);
	void AOLastMasterB(nemesis::scope blok, vecstr& blocks);
	void AONumMasterA(nemesis::scope blok, vecstr& blocks);
	void AONumMasterB(nemesis::scope blok, vecstr& blocks);

	void AOSingleA(nemesis::scope blok, vecstr& blocks);
	void AOSingleB(nemesis::scope blok, vecstr& blocks);

	// main_anim_event functions
	// group
	void MAEMultiGroup(nemesis::scope blok, vecstr& blocks);
	void MAEFirstGroup(nemesis::scope blok, vecstr& blocks);
	void MAENextGroup(nemesis::scope blok, vecstr& blocks);
	void MAEBackGroup(nemesis::scope blok, vecstr& blocks);
	void MAELastGroup(nemesis::scope blok, vecstr& blocks);
	void MAENumGroup(nemesis::scope blok, vecstr& blocks);

	//master
	void MAEMultiMaster(nemesis::scope blok, vecstr& blocks);
	void MAEFirstMaster(nemesis::scope blok, vecstr& blocks);
	void MAELastMaster(nemesis::scope blok, vecstr& blocks);
	void MAENumMaster(nemesis::scope blok, vecstr& blocks);

	void MAESingle(nemesis::scope blok, vecstr& blocks);

	// addOn functions
	// group
	void addOnMultiGroup(nemesis::scope blok, vecstr& blocks);
	void addOnFirstGroup(nemesis::scope blok, vecstr& blocks);
	void addOnNextGroup(nemesis::scope blok, vecstr& blocks);
	void addOnBackGroup(nemesis::scope blok, vecstr& blocks);
	void addOnLastGroup(nemesis::scope blok, vecstr& blocks);
	void addOnNumGroup(nemesis::scope blok, vecstr& blocks);

	// master
	void addOnMultiMaster(nemesis::scope blok, vecstr& blocks);
	void addOnFirstMaster(nemesis::scope blok, vecstr& blocks);
	void addOnLastMaster(nemesis::scope blok, vecstr& blocks);
	void addOnNumMaster(nemesis::scope blok, vecstr& blocks);

	void addOnSingle(nemesis::scope blok, vecstr& blocks);

	// last state functions
	void lastState(nemesis::scope blok, vecstr& blocks);

	// event ID functions
	void eventID(nemesis::scope blok, vecstr& blocks);

	// variable ID functions
	void variableID(nemesis::scope blok, vecstr& blocks);

	// crc32 functions
	void crc32(nemesis::scope blok, vecstr& blocks);

	// import functions
	void import(nemesis::scope blok, vecstr& blocks);

	// motion data functions
	// group
	void motionDataMultiGroup(nemesis::scope blok, vecstr& blocks);
	void motionDataFirstGroup(nemesis::scope blok, vecstr& blocks);
	void motionDataNextGroup(nemesis::scope blok, vecstr& blocks);
	void motionDataBackGroup(nemesis::scope blok, vecstr& blocks);
	void motionDataLastGroup(nemesis::scope blok, vecstr& blocks);
	void motionDataNumGroup(nemesis::scope blok, vecstr& blocks);

	// master
	void motionDataMultiMaster(nemesis::scope blok, vecstr& blocks);
	void motionDataFirstMaster(nemesis::scope blok, vecstr& blocks);
	void motionDataLastMaster(nemesis::scope blok, vecstr& blocks);
	void motionDataNumMaster(nemesis::scope blok, vecstr& blocks);

	void motionDataSingle(nemesis::scope blok, vecstr& blocks);

	// rotation data functions
	// group
	void rotationDataMultiGroup(nemesis::scope blok, vecstr& blocks);
	void rotationDataFirstGroup(nemesis::scope blok, vecstr& blocks);
	void rotationDataNextGroup(nemesis::scope blok, vecstr& blocks);
	void rotationDataBackGroup(nemesis::scope blok, vecstr& blocks);
	void rotationDataLastGroup(nemesis::scope blok, vecstr& blocks);
	void rotationDataNumGroup(nemesis::scope blok, vecstr& blocks);

	// master
	void rotationDataMultiMaster(nemesis::scope blok, vecstr& blocks);
	void rotationDataFirstMaster(nemesis::scope blok, vecstr& blocks);
	void rotationDataLastMaster(nemesis::scope blok, vecstr& blocks);
	void rotationDataNumMaster(nemesis::scope blok, vecstr& blocks);

	void rotationDataSingle(nemesis::scope blok, vecstr& blocks);

	// animOrder functions
	void animOrder(nemesis::scope blok, vecstr& blocks);

	// register animation
	void regisAnim(nemesis::scope blok, vecstr& blocks);

	// register behavior
	void regisBehavior(nemesis::scope blok, vecstr& blocks);

	// negative local
	void localNegative(nemesis::scope blok, vecstr& blocks);

	// getline
	void blocksCompile(vecstr blocks);
	
	// utilities
	bool isThisMaster();
	bool clearBlocks(nemesis::scope& blok, vecstr& blocks);
	void blockCheck(size_t front, size_t back);
	std::string combineBlocks(nemesis::scope& blok, vecstr& blocks);
	std::string combineBlocks(size_t front, size_t back, vecstr& blocks);
};

#endif
