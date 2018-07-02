#ifndef MASTER_H_
#define MASTER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <cstring>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <atomic>
#include <unordered_map>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include <QtCore/QObject>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtCore/QTimer>
#include "Global.h"
#include "alphanum.hpp"
#include "filechecker.h"
#include "behaviorcheck.h"
#include "skyrimdirectory.h"
#include "dataunification.h"
#include "functions/renew.h"
#include "behaviorgenerator.h"
#include "add animation\furniture.h"
#include "add animation\grouptemplate.h"
#include "add animation\import.h"
#include "logging\debugmsg.h"

extern std::unordered_map<std::string, vecstr> modinfo;

class UpdateFilesStart : public QObject
{
	Q_OBJECT

public:
	UpdateFilesStart();
	virtual ~UpdateFilesStart();
	void milestoneStart();
	void message(std::string input);

public slots :
	void UpdateFiles();
	bool VanillaUpdate(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList);
	bool GetPathLoop(std::string newPath, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList);
	bool VanillaDisassemble(std::string path, std::string filename, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
	bool AnimDataDisassemble(std::string path, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);
	bool AnimDataSetDisassemble(std::string path, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList);
	bool newAnimUpdate(std::string sourcefolder, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, std::unordered_map<std::string, std::set<std::string>>& oriADH, std::unordered_map<std::string, std::set<std::string>>& oriASDH, std::unordered_map<std::string, std::string>& lastUpdate);
	void SeparateMod(std::string directory, std::string modecode, vecstr behaviorfilelist, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, std::unordered_map<std::string, std::set<std::string>>& oriADH, std::unordered_map<std::string, std::set<std::string>>& oriASDH, std::unordered_map<std::string, std::string>& lastUpdate);
	void JoiningEdits(std::string directory, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, std::unordered_map<std::string, std::set<std::string>>& oriADH, std::unordered_map<std::string, std::set<std::string>>& oriASDH, std::unordered_map<std::string, std::string>& lastUpdate);
	void CombiningFiles(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList);
	void milestoneUp();
	void unregisterProcess();

signals:
	void progress(int);
	void end();
	void enable(bool);
	void enableCheck(bool);
	void hide(bool);
	void progressUp();
	void incomingMessage(QString);

private:
	int filenum;
	std::atomic<int> progressPercentage = 0;
	std::mutex pLock;
};

class BehaviorStart : public QObject
{
	Q_OBJECT

public:
	BehaviorStart();
	virtual ~BehaviorStart();
	void milestoneStart();
	void addBehaviorPick(BehaviorStart* newProces, vecstr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
	void message(std::string input);

public slots:
	void increaseAnimCount();
	void milestoneUp();
	void GenerateBehavior();
	void unregisterProcess();
	void EndAttempt();

signals:
	void totalAnim(int);
	void progress(int);
	void end();
	void enable(bool);
	void enableCheck(bool);
	void hide(bool);
	void progressUp();
	void incomingMessage(QString);

private:
	int runningThread = 1;
	int animCount = 0;
	int filenum;
	std::atomic<int> progressPercentage = 0;
	std::unordered_map<std::string, vecstr> coreModList;		// core filename, list of modID;

	vecstr behaviorPriority;
	std::unordered_map<std::string, bool> chosenBehavior;
	std::mutex pLock;
	BehaviorStart* behaviorProcess;
	
	std::string* directory2;
	vecstr* filelist2;
	getTemplate* BehaviorTemplate2;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>* newAnimation2;
	std::vector<std::unique_ptr<registerAnimation>>* animationList2;
	mapSetString* newAnimEvent2;
	mapSetString* newAnimVariable2;
	std::unordered_map<std::string, var>* AnimVar2;
	std::unordered_map<std::string, std::unordered_map<int, bool>>* ignoreFunction2;
};

class BehaviorSub : public QObject
{
	Q_OBJECT

public:
	bool isCharacter;

	void addInfo(std::string& newDirectory, vecstr& newfilelist, int newCurList, vecstr& newBehaviorPriority, std::unordered_map<std::string, bool>& newChosenBehavior, getTemplate& newBehaviorTemplate, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& addAnimation, std::unordered_map<std::string, var>& newAnimVar, mapSetString& addAnimEvent, mapSetString& addAnimVariable, std::unordered_map<std::string, std::unordered_map<int, bool>>& newIgnoreFunction, bool newIsCharacter, std::string newModID);
	void addAnimation();
	void hkxcmdOutput(std::string filename, std::string hkxfile);

public slots:
	void BehaviorCompilation();
	void AnimDataCompilation();
	void ASDCompilation();
	
signals:
	void newAnim();
	void progressUp();
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

bool readMod(std::string& errormod);

#endif