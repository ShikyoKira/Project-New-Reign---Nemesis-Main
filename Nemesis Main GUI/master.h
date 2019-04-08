#ifndef MASTER_H_
#define MASTER_H_

#include <map>
#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include <cstring>
#include <fstream>
#include <iostream>
#include <direct.h>
#include <algorithm>
#include <unordered_map>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include <QtCore\QTimer>
#include <QtCore\QObject>
#include <QtWidgets\QTreeView>
#include <QtWidgets\QPushButton>
#include "Global.h"
#include "alphanum.hpp"
#include "filechecker.h"
#include "behaviorcheck.h"
#include "skyrimdirectory.h"
#include "dataunification.h"
#include "functions\renew.h"
#include "logging\debugmsg.h"
#include "behaviorgenerator.h"
#include "add animation\import.h"
#include "add animation\furniture.h"
#include "add animation\grouptemplate.h"
#include "add animation\playerexclusive.h"

typedef std::unordered_map<std::string, SSMap> SSSMap;
typedef std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, std::set<std::string>>>> StateIDList;

class NemesisMainGUI;
class UpdateLock;

extern std::unordered_map<std::string, vecstr> modinfo;
extern std::mutex processlock;
extern std::condition_variable cv;
extern bool processdone;

struct arguPack
{
	arguPack(std::string n_directory, std::string n_modcode, vecstr n_behaviorfilelist, std::unordered_map<std::string, std::map<std::string, vecstr>>& n_newFile,
		std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>>& n_childrenState, SSSMap& n_stateID, SSSMap& n_parent,
		StateIDList& n_modStateList, StateIDList& n_duplicatedStateList, MasterAnimData& n_animData, MasterAnimSetData& n_animSetData,
		std::unordered_map<std::string, std::string>& n_lastUpdate, std::shared_ptr<UpdateLock> n_modUpdate)
		: newFile(n_newFile), childrenState(n_childrenState), stateID(n_stateID), parent(n_parent), modStateList(n_modStateList), duplicatedStateList(n_duplicatedStateList),
		animData(n_animData), animSetData(n_animSetData), lastUpdate(n_lastUpdate)
	{
		directory = n_directory;
		modcode = n_modcode;
		behaviorfilelist = n_behaviorfilelist;
		modUpdate = n_modUpdate;
	}

	std::string directory;
	std::string modcode;
	vecstr behaviorfilelist;
	std::shared_ptr<UpdateLock> modUpdate;
	std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile;
	std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState;
	SSSMap& stateID;
	SSSMap& parent;
	StateIDList& modStateList;
	StateIDList& duplicatedStateList;
	MasterAnimData& animData;
	MasterAnimSetData& animSetData;
	std::unordered_map<std::string, std::string>& lastUpdate;
};

class UpdateFilesStart : public QObject
{
	Q_OBJECT

public:
	bool cmdline = false;

	UpdateFilesStart();
	virtual ~UpdateFilesStart();
	void milestoneStart(std::string directory);
	void message(std::string input);
	void GetFileLoop(std::string newPath);
	bool VanillaUpdate(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile,
		std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState, SSSMap& stateID, SSSMap& n_parent, MasterAnimData& animData,
		MasterAnimSetData& animSetData);
	bool GetPathLoop(std::string newPath, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile,
		std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState, SSSMap& stateID, SSSMap& n_parent, MasterAnimData& animData,
		MasterAnimSetData& animSetData, bool isFirstPerson);
	bool VanillaDisassemble(std::string path, std::string filename, std::map<std::string, vecstr>& newFile,
		std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState, SSMap& stateID, SSMap& n_parent);
	bool AnimDataDisassemble(std::string path, MasterAnimData& animData);
	bool AnimSetDataDisassemble(std::string path, MasterAnimSetData& animSetData);
	bool newAnimUpdate(std::string sourcefolder, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData,
		std::unordered_map<std::string, std::string>& lastUpdate);
	void SeparateMod(arguPack& pack);
	void JoiningEdits(std::string directory, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile,
		std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState, SSSMap& stateID, SSSMap& n_parent, MasterAnimData& animData,
		MasterAnimSetData& animSetData, std::unordered_map<std::string, std::string>& lastUpdate);
	void CombiningFiles(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData);
	void unregisterProcess();

public slots :
	void UpdateFiles();

signals:
	void progressMax(int);
	void progressUp();
	void end();
	void enable(bool);
	void enableCheck(bool);
	void hide(bool);
	void incomingMessage(QString);

private:
	int filenum;

	// timer
	boost::posix_time::ptime start_time;
};

class BehaviorStart : public QObject
{
	Q_OBJECT

public:
	BehaviorStart();
	virtual ~BehaviorStart();
	void milestoneStart();
	void addBehaviorPick(BehaviorStart* newProces, NemesisMainGUI* newWidget, vecstr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
	void addBehaviorPick(BehaviorStart* newProcess, vecstr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
	void message(std::string input);

public slots:
	void newMilestone();
	void increaseAnimCount();
	void GenerateBehavior();
	void unregisterProcess(bool skip = false);
	void EndAttempt();

signals:
	void totalAnim(int);
	void progressMax(int);
	void progressUp();
	void end();
	void enable(bool);
	void enableCheck(bool);
	void hide(bool);
	void incomingMessage(QString);

private:
	bool cmdline = false;
	int animCount = 0;
	int filenum;
	std::unordered_map<std::string, vecstr> coreModList;		// core filename, list of modID;

	vecstr behaviorPriority;
	std::unordered_map<std::string, bool> chosenBehavior;
	BehaviorStart* behaviorProcess;
	NemesisMainGUI* widget;
	
	std::string* directory2;
	vecstr* filelist2;
	getTemplate* BehaviorTemplate2;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>* newAnimation2;
	std::vector<std::unique_ptr<registerAnimation>>* animationList2;
	mapSetString* newAnimEvent2;
	mapSetString* newAnimVariable2;
	std::unordered_map<std::string, var>* AnimVar2;
	std::unordered_map<std::string, std::unordered_map<int, bool>>* ignoreFunction2;

	// timer
	boost::posix_time::ptime start_time;
};

class BehaviorSub : public QObject
{
	Q_OBJECT

public:
	bool isCharacter;

	void addInfo(std::string& newDirectory, vecstr& newfilelist, int newCurList, vecstr& newBehaviorPriority, std::unordered_map<std::string, bool>& newChosenBehavior, getTemplate& newBehaviorTemplate, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& addAnimation, std::unordered_map<std::string, var>& newAnimVar, mapSetString& addAnimEvent, mapSetString& addAnimVariable, std::unordered_map<std::string, std::unordered_map<int, bool>>& newIgnoreFunction, bool newIsCharacter, std::string newModID);
	void addAnimation();
	void CompilingBehavior();
	void CompilingAnimData();
	void CompilingASD();

public slots:
	void BehaviorCompilation();
	void AnimDataCompilation();
	void ASDCompilation();
	
signals:
	void newAnim();
	void progressAdd();
	void done();

private:
	int base;
	int animCounter = 0;

	std::string modID;
	std::string directory;
	vecstr filelist;
	int curList;
	vecstr behaviorPriority;
	std::unordered_map<std::string, bool> chosenBehavior;
	getTemplate BehaviorTemplate;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>> newAnimation;
	mapSetString newAnimEvent;
	mapSetString newAnimVariable;
	std::unordered_map<std::string, var> AnimVar;
	std::unordered_map<std::string, std::unordered_map<int, bool>> ignoreFunction;
};

class DummyLog : public QObject
{
	Q_OBJECT

public:
	void message(std::string input);

signals:
	void incomingMessage(QString);
};

class InstallScripts : public QObject
{
	Q_OBJECT

public slots:
	void Run();

signals:
	void end();
};

class Terminator : public QObject
{
	Q_OBJECT

public:
	void exitSignal();

signals:
	void end();
};

bool isRunning(Terminator*& curEvent);
bool readMod(std::string& errormod, std::string& errormsg);
vecstr getHiddenMods();

#endif