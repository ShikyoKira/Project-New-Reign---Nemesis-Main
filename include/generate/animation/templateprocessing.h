#ifndef TEMPLATEPROCESSING_H_
#define TEMPLATEPROCESSING_H_

#include <unordered_map>

#include <boost/regex.hpp>

#include "utilities/scope.h"

#include "generate/animation/animationthread.h"

#include "utilities/regex.h"

struct proc;

namespace nemesis
{
	struct MultiChoice;
	struct scope;
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

    ID* eventid;
    ID* variableid;

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

	std::unordered_map<int, VecStr>* AnimObject;
	std::unordered_map<std::string, std::unordered_map<std::string, VecStr>>* groupAddition;
	std::vector<std::unordered_map<std::string, bool>>* groupOptionPicked;
	std::vector<std::vector<std::unordered_map<std::string, bool>>>* masterOptionPicked;
	
	std::shared_ptr<group> groupFunction;
	std::shared_ptr<master> masterFunction;

	std::shared_ptr<VecStr> generatedlines;
	
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

    proc()
    {}

    void Register(std::string n_format,
                  std::string masterformat,
                  std::string n_behaviorFile,
                  std::string n_filepath,
                  std::string n_filename,
                  std::string n_mainAnimEvent,
                  std::string& n_strID,
                  std::string n_zeroEvent,
                  std::string n_zeroVariable,
                  bool n_hasGroup,
                  bool& n_negative,
                  bool& n_isEnd,
                  bool& n_norElement,
                  bool& n_elementCatch,
                  bool hasDuration,
                  double duration,
                  int& n_openRange,
                  int& n_counter,
                  size_t& n_elementLine,
                  int n_furnitureCount,
                  ID& n_eventid,
                  ID& n_variableid,
                  std::vector<int> n_fixedStateID,
                  std::vector<int> n_stateCountMultiplier,
                  NewAnimLock* n_animLock,
                  int n_order,
                  int n_lastorder,
                  SSMap& n_IDExist,
                  std::unordered_map<int, VecStr>& n_AnimObject,
                  ImportContainer& n_addition,
                  ImportContainer* n_newImport,
                  std::unordered_map<std::string, std::unordered_map<std::string, VecStr>>& n_groupAddition,
                  std::vector<std::unordered_map<std::string, bool>>& n_groupOptionPicked,
                  std::shared_ptr<group> n_groupFunction,
                  std::shared_ptr<VecStr> n_generatedlines,
                  NewAnimation* n_curAnim);

	void installBlock(nemesis::scope blok, int curline);
	void installBlock(nemesis::scope blok, int curline, std::vector<nemesis::MultiChoice> n_condiiton);

	// processes
	void relativeNegative(nemesis::scope blok, VecStr& blocks);
	void compute(nemesis::scope blok, VecStr& blocks);
	void rangeCompute(nemesis::scope blok, VecStr& blocks);
	void upCounter(nemesis::scope blok, VecStr& blocks);
	void upCounterPlus(nemesis::scope blok, VecStr& blocks);
	void animCount(nemesis::scope blok, VecStr& blocks);
	void multiChoiceRegis(nemesis::scope blok, VecStr& blocks);
	void groupIDRegis(nemesis::scope blok, VecStr& blocks);
	void IDRegis(nemesis::scope blok, VecStr& blocks);
	void IDRegisAnim(nemesis::scope blok, VecStr& blocks);
	void IDRegisGroup(nemesis::scope blok, VecStr& blocks);
	void IDRegisMaster(nemesis::scope blok, VecStr& blocks);
	void computation(nemesis::scope blok, VecStr& blocks);

	// end functions
	// group
	void endMultiGroup(nemesis::scope blok, VecStr& blocks);
	void endFirstGroup(nemesis::scope blok, VecStr& blocks);
	void endNextGroup(nemesis::scope blok, VecStr& blocks);
	void endBackGroup(nemesis::scope blok, VecStr& blocks);
	void endLastGroup(nemesis::scope blok, VecStr& blocks);
	void endNumGroup(nemesis::scope blok, VecStr& blocks);

	// master
	void endMultiMaster(nemesis::scope blok, VecStr& blocks);
	void endFirstMaster(nemesis::scope blok, VecStr& blocks);
	void endLastMaster(nemesis::scope blok, VecStr& blocks);
	void endNumMaster(nemesis::scope blok, VecStr& blocks);

	void endSingle(nemesis::scope blok, VecStr& blocks);


	// state functions
	// group
	void stateMultiGroup(nemesis::scope blok, VecStr& blocks);
	void stateFirstGroup(nemesis::scope blok, VecStr& blocks);
	void stateNextGroup(nemesis::scope blok, VecStr& blocks);
	void stateBackGroup(nemesis::scope blok, VecStr& blocks);
	void stateLastGroup(nemesis::scope blok, VecStr& blocks);
	void stateNumGroup(nemesis::scope blok, VecStr& blocks);

	// master to group
	void stateMultiMasterToGroup(nemesis::scope blok, VecStr& blocks);
	/*void stateFirstMasterToGroup(nemesis::scope blok, VecStr& blocks);
	void stateLastMasterToGroup(nemesis::scope blok, VecStr& blocks);		Not needed
	void stateNumMasterToGroup(nemesis::scope blok, VecStr& blocks);*/

	// master
	void stateMultiMaster(nemesis::scope blok, VecStr& blocks);
	void stateFirstMaster(nemesis::scope blok, VecStr& blocks);
	void stateLastMaster(nemesis::scope blok, VecStr& blocks);
	void stateNumMaster(nemesis::scope blok, VecStr& blocks);

	void stateSingle(nemesis::scope blok, VecStr& blocks);


	// filepath functions
	// group
	void filepathMultiGroup(nemesis::scope blok, VecStr& blocks);
	void filepathFirstGroup(nemesis::scope blok, VecStr& blocks);
	void filepathNextGroup(nemesis::scope blok, VecStr& blocks);
	void filepathBackGroup(nemesis::scope blok, VecStr& blocks);
	void filepathLastGroup(nemesis::scope blok, VecStr& blocks);
	void filepathNumGroup(nemesis::scope blok, VecStr& blocks);

	// master
	void filepathMultiMaster(nemesis::scope blok, VecStr& blocks);
	void filepathFirstMaster(nemesis::scope blok, VecStr& blocks);
	void filepathLastMaster(nemesis::scope blok, VecStr& blocks);
	void filepathNumMaster(nemesis::scope blok, VecStr& blocks);

	void filepathSingle(nemesis::scope blok, VecStr& blocks);

	// filename functions
	// group
	void filenameMultiGroup(nemesis::scope blok, VecStr& blocks);
	void filenameFirstGroup(nemesis::scope blok, VecStr& blocks);
	void filenameNextGroup(nemesis::scope blok, VecStr& blocks);
	void filenameBackGroup(nemesis::scope blok, VecStr& blocks);
	void filenameLastGroup(nemesis::scope blok, VecStr& blocks);
	void filenameNumGroup(nemesis::scope blok, VecStr& blocks);

	// master
	void filenameMultiMaster(nemesis::scope blok, VecStr& blocks);
	void filenameFirstMaster(nemesis::scope blok, VecStr& blocks);
	void filenameLastMaster(nemesis::scope blok, VecStr& blocks);
	void filenameNumMaster(nemesis::scope blok, VecStr& blocks);

	void filenameSingle(nemesis::scope blok, VecStr& blocks);

	// path functions
	void pathSingle(nemesis::scope blok, VecStr& blocks);

	// AnimObject functions
	// group
	void AOMultiGroupA(nemesis::scope blok, VecStr& blocks);
	void AOMultiGroupB(nemesis::scope blok, VecStr& blocks);
	void AOFirstGroupA(nemesis::scope blok, VecStr& blocks);
	void AOFirstGroupB(nemesis::scope blok, VecStr& blocks);
	void AONextGroupA(nemesis::scope blok, VecStr& blocks);
	void AONextGroupB(nemesis::scope blok, VecStr& blocks);
	void AOBackGroupA(nemesis::scope blok, VecStr& blocks);
	void AOBackGroupB(nemesis::scope blok, VecStr& blocks);
	void AOLastGroupA(nemesis::scope blok, VecStr& blocks);
	void AOLastGroupB(nemesis::scope blok, VecStr& blocks);
	void AONumGroupA(nemesis::scope blok, VecStr& blocks);
	void AONumGroupB(nemesis::scope blok, VecStr& blocks);

	// master
	void AOMultiMasterA(nemesis::scope blok, VecStr& blocks);
	void AOMultiMasterB(nemesis::scope blok, VecStr& blocks);
	void AOFirstMasterA(nemesis::scope blok, VecStr& blocks);
	void AOFirstMasterB(nemesis::scope blok, VecStr& blocks);
	void AOLastMasterA(nemesis::scope blok, VecStr& blocks);
	void AOLastMasterB(nemesis::scope blok, VecStr& blocks);
	void AONumMasterA(nemesis::scope blok, VecStr& blocks);
	void AONumMasterB(nemesis::scope blok, VecStr& blocks);

	void AOSingleA(nemesis::scope blok, VecStr& blocks);
	void AOSingleB(nemesis::scope blok, VecStr& blocks);

	// main_anim_event functions
	// group
	void MAEMultiGroup(nemesis::scope blok, VecStr& blocks);
	void MAEFirstGroup(nemesis::scope blok, VecStr& blocks);
	void MAENextGroup(nemesis::scope blok, VecStr& blocks);
	void MAEBackGroup(nemesis::scope blok, VecStr& blocks);
	void MAELastGroup(nemesis::scope blok, VecStr& blocks);
	void MAENumGroup(nemesis::scope blok, VecStr& blocks);

	//master
	void MAEMultiMaster(nemesis::scope blok, VecStr& blocks);
	void MAEFirstMaster(nemesis::scope blok, VecStr& blocks);
	void MAELastMaster(nemesis::scope blok, VecStr& blocks);
	void MAENumMaster(nemesis::scope blok, VecStr& blocks);

	void MAESingle(nemesis::scope blok, VecStr& blocks);

	// addOn functions
	// group
	void addOnMultiGroup(nemesis::scope blok, VecStr& blocks);
	void addOnFirstGroup(nemesis::scope blok, VecStr& blocks);
	void addOnNextGroup(nemesis::scope blok, VecStr& blocks);
	void addOnBackGroup(nemesis::scope blok, VecStr& blocks);
	void addOnLastGroup(nemesis::scope blok, VecStr& blocks);
	void addOnNumGroup(nemesis::scope blok, VecStr& blocks);

	// master
	void addOnMultiMaster(nemesis::scope blok, VecStr& blocks);
	void addOnFirstMaster(nemesis::scope blok, VecStr& blocks);
	void addOnLastMaster(nemesis::scope blok, VecStr& blocks);
	void addOnNumMaster(nemesis::scope blok, VecStr& blocks);

	void addOnSingle(nemesis::scope blok, VecStr& blocks);

	// last state functions
	void lastState(nemesis::scope blok, VecStr& blocks);

	// event ID functions
	void eventID(nemesis::scope blok, VecStr& blocks);

	// variable ID functions
	void variableID(nemesis::scope blok, VecStr& blocks);

	// crc32 functions
	void crc32(nemesis::scope blok, VecStr& blocks);

	// import functions
	void import(nemesis::scope blok, VecStr& blocks);

	// motion data functions
	// group
	void motionDataMultiGroup(nemesis::scope blok, VecStr& blocks);
	void motionDataFirstGroup(nemesis::scope blok, VecStr& blocks);
	void motionDataNextGroup(nemesis::scope blok, VecStr& blocks);
	void motionDataBackGroup(nemesis::scope blok, VecStr& blocks);
	void motionDataLastGroup(nemesis::scope blok, VecStr& blocks);
	void motionDataNumGroup(nemesis::scope blok, VecStr& blocks);

	// master
	void motionDataMultiMaster(nemesis::scope blok, VecStr& blocks);
	void motionDataFirstMaster(nemesis::scope blok, VecStr& blocks);
	void motionDataLastMaster(nemesis::scope blok, VecStr& blocks);
	void motionDataNumMaster(nemesis::scope blok, VecStr& blocks);

	void motionDataSingle(nemesis::scope blok, VecStr& blocks);

	// rotation data functions
	// group
	void rotationDataMultiGroup(nemesis::scope blok, VecStr& blocks);
	void rotationDataFirstGroup(nemesis::scope blok, VecStr& blocks);
	void rotationDataNextGroup(nemesis::scope blok, VecStr& blocks);
	void rotationDataBackGroup(nemesis::scope blok, VecStr& blocks);
	void rotationDataLastGroup(nemesis::scope blok, VecStr& blocks);
	void rotationDataNumGroup(nemesis::scope blok, VecStr& blocks);

	// master
	void rotationDataMultiMaster(nemesis::scope blok, VecStr& blocks);
	void rotationDataFirstMaster(nemesis::scope blok, VecStr& blocks);
	void rotationDataLastMaster(nemesis::scope blok, VecStr& blocks);
	void rotationDataNumMaster(nemesis::scope blok, VecStr& blocks);

	void rotationDataSingle(nemesis::scope blok, VecStr& blocks);

	// animOrder functions
	void animOrder(nemesis::scope blok, VecStr& blocks);

	// register animation
	void regisAnim(nemesis::scope blok, VecStr& blocks);

	// register behavior
	void regisBehavior(nemesis::scope blok, VecStr& blocks);

	// negative local
	void localNegative(nemesis::scope blok, VecStr& blocks);

	// getline
	void blocksCompile(VecStr blocks);
	
	// utilities
	bool isThisMaster();
	bool clearBlocks(nemesis::scope& blok, VecStr& blocks);
	void blockCheck(size_t front, size_t back);
	std::string combineBlocks(nemesis::scope& blok, VecStr& blocks);
	std::string combineBlocks(size_t front, size_t back, VecStr& blocks);
};

#endif
