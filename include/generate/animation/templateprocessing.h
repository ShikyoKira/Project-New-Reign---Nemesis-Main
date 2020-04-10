#ifndef TEMPLATEPROCESSING_H_
#define TEMPLATEPROCESSING_H_

#include <unordered_map>

#include "generate/animation/animationthread.h"

#include "utilities/regex.h"

struct proc;
struct multichoice;

struct range
{
    size_t front;
    size_t back;
    size_t size;
    VecStr olddata;
    std::vector<int> olddataint;
    void (proc::*func)(range, VecStr&);

    range()
    {}
    range(size_t n_front, size_t n_back, void (proc::*n_func)(range, VecStr&));
    range(size_t n_front, size_t n_back, VecStr n_olddata, void (proc::*n_func)(range, VecStr&));
    range(size_t n_front, size_t n_back, std::vector<int> n_olddataint, void (proc::*n_func)(range, VecStr&));
    range(size_t n_front,
          size_t n_back,
          std::vector<int> n_olddataint,
          VecStr n_olddata,
          void (proc::*n_func)(range, VecStr&));
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
    int animMulti   = -1;
    int groupMulti  = 0;
    int order;
    int lastorder;

    bool isMC     = false;
    bool isGroup  = false;
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
    std::unordered_map<int, block> lineblocks; // numline, blocksize (order), blocks of same size
    std::unordered_map<int, std::vector<range>> choiceblocks; // numline, blocks

    std::unordered_map<int, std::vector<multichoice>> multiChoice; // numline, list of multichoice

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

    void installBlock(range blok, int curline);
    void installBlock(range blok, int curline, std::vector<multichoice> n_condiiton);

    // processes
    void relativeNegative(range blok, VecStr& blocks);
    void compute(range blok, VecStr& blocks);
    void rangeCompute(range blok, VecStr& blocks);
    void upCounter(range blok, VecStr& blocks);
    void upCounterPlus(range blok, VecStr& blocks);
    void animCount(range blok, VecStr& blocks);
    void multiChoiceRegis(range blok, VecStr& blocks);
    void groupIDRegis(range blok, VecStr& blocks);
    void IDRegis(range blok, VecStr& blocks);
    void IDRegisAnim(range blok, VecStr& blocks);
    void IDRegisGroup(range blok, VecStr& blocks);
    void IDRegisMaster(range blok, VecStr& blocks);
    void computation(range blok, VecStr& blocks);

    // end functions
    // group
    void endMultiGroup(range blok, VecStr& blocks);
    void endFirstGroup(range blok, VecStr& blocks);
    void endNextGroup(range blok, VecStr& blocks);
    void endBackGroup(range blok, VecStr& blocks);
    void endLastGroup(range blok, VecStr& blocks);
    void endNumGroup(range blok, VecStr& blocks);

    // master
    void endMultiMaster(range blok, VecStr& blocks);
    void endFirstMaster(range blok, VecStr& blocks);
    void endLastMaster(range blok, VecStr& blocks);
    void endNumMaster(range blok, VecStr& blocks);

    void endSingle(range blok, VecStr& blocks);

    // state functions
    // group
    void stateMultiGroup(range blok, VecStr& blocks);
    void stateFirstGroup(range blok, VecStr& blocks);
    void stateNextGroup(range blok, VecStr& blocks);
    void stateBackGroup(range blok, VecStr& blocks);
    void stateLastGroup(range blok, VecStr& blocks);
    void stateNumGroup(range blok, VecStr& blocks);

    // master to group
    void stateMultiMasterToGroup(range blok, VecStr& blocks);
    /*void stateFirstMasterToGroup(range blok, VecStr& blocks);
	void stateLastMasterToGroup(range blok, VecStr& blocks);		Not needed
	void stateNumMasterToGroup(range blok, VecStr& blocks);*/

    // master
    void stateMultiMaster(range blok, VecStr& blocks);
    void stateFirstMaster(range blok, VecStr& blocks);
    void stateLastMaster(range blok, VecStr& blocks);
    void stateNumMaster(range blok, VecStr& blocks);

    void stateSingle(range blok, VecStr& blocks);

    // filepath functions
    // group
    void filepathMultiGroup(range blok, VecStr& blocks);
    void filepathFirstGroup(range blok, VecStr& blocks);
    void filepathNextGroup(range blok, VecStr& blocks);
    void filepathBackGroup(range blok, VecStr& blocks);
    void filepathLastGroup(range blok, VecStr& blocks);
    void filepathNumGroup(range blok, VecStr& blocks);

    // master
    void filepathMultiMaster(range blok, VecStr& blocks);
    void filepathFirstMaster(range blok, VecStr& blocks);
    void filepathLastMaster(range blok, VecStr& blocks);
    void filepathNumMaster(range blok, VecStr& blocks);

    void filepathSingle(range blok, VecStr& blocks);

    // filename functions
    // group
    void filenameMultiGroup(range blok, VecStr& blocks);
    void filenameFirstGroup(range blok, VecStr& blocks);
    void filenameNextGroup(range blok, VecStr& blocks);
    void filenameBackGroup(range blok, VecStr& blocks);
    void filenameLastGroup(range blok, VecStr& blocks);
    void filenameNumGroup(range blok, VecStr& blocks);

    // master
    void filenameMultiMaster(range blok, VecStr& blocks);
    void filenameFirstMaster(range blok, VecStr& blocks);
    void filenameLastMaster(range blok, VecStr& blocks);
    void filenameNumMaster(range blok, VecStr& blocks);

    void filenameSingle(range blok, VecStr& blocks);

    // path functions
    void pathSingle(range blok, VecStr& blocks);

    // AnimObject functions
    // group
    void AOMultiGroupA(range blok, VecStr& blocks);
    void AOMultiGroupB(range blok, VecStr& blocks);
    void AOFirstGroupA(range blok, VecStr& blocks);
    void AOFirstGroupB(range blok, VecStr& blocks);
    void AONextGroupA(range blok, VecStr& blocks);
    void AONextGroupB(range blok, VecStr& blocks);
    void AOBackGroupA(range blok, VecStr& blocks);
    void AOBackGroupB(range blok, VecStr& blocks);
    void AOLastGroupA(range blok, VecStr& blocks);
    void AOLastGroupB(range blok, VecStr& blocks);
    void AONumGroupA(range blok, VecStr& blocks);
    void AONumGroupB(range blok, VecStr& blocks);

    // master
    void AOMultiMasterA(range blok, VecStr& blocks);
    void AOMultiMasterB(range blok, VecStr& blocks);
    void AOFirstMasterA(range blok, VecStr& blocks);
    void AOFirstMasterB(range blok, VecStr& blocks);
    void AOLastMasterA(range blok, VecStr& blocks);
    void AOLastMasterB(range blok, VecStr& blocks);
    void AONumMasterA(range blok, VecStr& blocks);
    void AONumMasterB(range blok, VecStr& blocks);

    void AOSingleA(range blok, VecStr& blocks);
    void AOSingleB(range blok, VecStr& blocks);

    // main_anim_event functions
    // group
    void MAEMultiGroup(range blok, VecStr& blocks);
    void MAEFirstGroup(range blok, VecStr& blocks);
    void MAENextGroup(range blok, VecStr& blocks);
    void MAEBackGroup(range blok, VecStr& blocks);
    void MAELastGroup(range blok, VecStr& blocks);
    void MAENumGroup(range blok, VecStr& blocks);

    //master
    void MAEMultiMaster(range blok, VecStr& blocks);
    void MAEFirstMaster(range blok, VecStr& blocks);
    void MAELastMaster(range blok, VecStr& blocks);
    void MAENumMaster(range blok, VecStr& blocks);

    void MAESingle(range blok, VecStr& blocks);

    // addOn functions
    // group
    void addOnMultiGroup(range blok, VecStr& blocks);
    void addOnFirstGroup(range blok, VecStr& blocks);
    void addOnNextGroup(range blok, VecStr& blocks);
    void addOnBackGroup(range blok, VecStr& blocks);
    void addOnLastGroup(range blok, VecStr& blocks);
    void addOnNumGroup(range blok, VecStr& blocks);

    // master
    void addOnMultiMaster(range blok, VecStr& blocks);
    void addOnFirstMaster(range blok, VecStr& blocks);
    void addOnLastMaster(range blok, VecStr& blocks);
    void addOnNumMaster(range blok, VecStr& blocks);

    void addOnSingle(range blok, VecStr& blocks);

    // last state functions
    void lastState(range blok, VecStr& blocks);

    // event ID functions
    void eventID(range blok, VecStr& blocks);

    // variable ID functions
    void variableID(range blok, VecStr& blocks);

    // crc32 functions
    void crc32(range blok, VecStr& blocks);

    // import functions
    void import(range blok, VecStr& blocks);

    // motion data functions
    // group
    void motionDataMultiGroup(range blok, VecStr& blocks);
    void motionDataFirstGroup(range blok, VecStr& blocks);
    void motionDataNextGroup(range blok, VecStr& blocks);
    void motionDataBackGroup(range blok, VecStr& blocks);
    void motionDataLastGroup(range blok, VecStr& blocks);
    void motionDataNumGroup(range blok, VecStr& blocks);

    // master
    void motionDataMultiMaster(range blok, VecStr& blocks);
    void motionDataFirstMaster(range blok, VecStr& blocks);
    void motionDataLastMaster(range blok, VecStr& blocks);
    void motionDataNumMaster(range blok, VecStr& blocks);

    void motionDataSingle(range blok, VecStr& blocks);

    // rotation data functions
    // group
    void rotationDataMultiGroup(range blok, VecStr& blocks);
    void rotationDataFirstGroup(range blok, VecStr& blocks);
    void rotationDataNextGroup(range blok, VecStr& blocks);
    void rotationDataBackGroup(range blok, VecStr& blocks);
    void rotationDataLastGroup(range blok, VecStr& blocks);
    void rotationDataNumGroup(range blok, VecStr& blocks);

    // master
    void rotationDataMultiMaster(range blok, VecStr& blocks);
    void rotationDataFirstMaster(range blok, VecStr& blocks);
    void rotationDataLastMaster(range blok, VecStr& blocks);
    void rotationDataNumMaster(range blok, VecStr& blocks);

    void rotationDataSingle(range blok, VecStr& blocks);

    // animOrder functions
    void animOrder(range blok, VecStr& blocks);

    // register animation
    void regisAnim(range blok, VecStr& blocks);

    // register behavior
    void regisBehavior(range blok, VecStr& blocks);

    // negative local
    void localNegative(range blok, VecStr& blocks);

    // getline
    void blocksCompile(VecStr blocks);

    // utilities
    bool isThisMaster();
    bool clearBlocks(range& blok, VecStr& blocks);
    void blockCheck(size_t front, size_t back);
    std::string combineBlocks(range& blok, VecStr& blocks);
    std::string combineBlocks(size_t front, size_t back, VecStr& blocks);
};

#endif
