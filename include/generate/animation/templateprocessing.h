#ifndef TEMPLATEPROCESSING_H_
#define TEMPLATEPROCESSING_H_

#include <unordered_map>
#include <unordered_set>

#include "utilities/scope.h"
#include "utilities/regex.h"
#include "utilities/conditions.h"

#include "generate/animation/animationthread.h"


struct proc;

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
	// template info

	size_t stateCount = 0;

	const std::vector<std::unordered_map<std::string, bool>>* groupOptionPicked;
	const std::vector<std::vector<std::unordered_map<std::string, bool>>>* masterOptionPicked;
	
	bool isGroup = false;
	bool isMaster = false;
	
	std::string format;
    std::string masterformat;
    std::string behaviorFile;

	std::unordered_set<int> hasMC;
	std::unordered_map<int, std::vector<int>> brackets;
	std::unordered_map<int, std::vector<void (proc::*)()>> process;
	std::unordered_map<int, block> lineblocks;							// numline, blocksize (order), blocks of same size
	std::unordered_map<int, std::vector<nemesis::scope>> choiceblocks;			// numline, blocks

	std::unordered_map<int, std::vector<nemesis::MultiChoice>> multiChoice;		// numline, list of MultiChoice

    proc()
    {}

	void installBlock(nemesis::scope blok, int curline);
	void installBlock(nemesis::scope blok, int curline, std::vector<nemesis::MultiChoice> n_condiiton);

	// processes
	void relativeNegative(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void compute(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rangeCompute(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void upCounter(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void upCounterPlus(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void animCount(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void multiChoiceRegis(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void groupIDRegis(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void IDRegis(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void IDRegisAnim(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void IDRegisGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void IDRegisMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void computation(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// end functions
	// group
	void endMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void endMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void endNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void endSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;


	// state functions
	// group
	void stateMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master to group
	void stateMultiMasterToGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	/*void stateFirstMasterToGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateLastMasterToGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;		Not needed
	void stateNumMasterToGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;*/

	// master
	void stateMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void stateNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void stateSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;


	// filepath functions
	// group
	void filepathMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void filepathMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filepathNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void filepathSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// filename functions
	// group
	void filenameMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void filenameMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void filenameNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void filenameSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// path functions
	void pathSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// AnimObject functions
	// group
	void AOMultiGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOMultiGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOFirstGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOFirstGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AONextGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AONextGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOBackGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOBackGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOLastGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOLastGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AONumGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AONumGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void AOMultiMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOMultiMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOFirstMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOFirstMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOLastMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOLastMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AONumMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AONumMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void AOSingleA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void AOSingleB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// main_anim_event functions
	// group
	void MAEMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAEFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAENextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAEBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAELastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAENumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	//master
	void MAEMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAEFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAELastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void MAENumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void MAESingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// addOn functions
	// group
	void addOnMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void addOnMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void addOnNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void addOnSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// last state functions
	void lastState(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// event ID functions
	void eventID(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// variable ID functions
	void variableID(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// crc32 functions
	void crc32(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// import functions
	void import(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// motion data functions
	// group
	void motionDataMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void motionDataMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void motionDataNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void motionDataSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// rotation data functions
	// group
	void rotationDataMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// master
	void rotationDataMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void rotationDataNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	void rotationDataSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// animOrder functions
	void animOrder(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// register animation
	void regisAnim(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// register behavior
	void regisBehavior(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// negative local
	void localNegative(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;

	// getline
    void blocksCompile(VecStr blocks, AnimThreadInfo& curAnimInfo) const;
	
	// utilities
	bool isThisMaster();
    bool clearBlocks(nemesis::scope& blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const;
	void blockCheck(size_t front, size_t back, int numline);
	std::string combineBlocks(nemesis::scope& blok, VecStr& blocks) const;
	std::string combineBlocks(size_t front, size_t back, VecStr& blocks) const;
};

#endif
