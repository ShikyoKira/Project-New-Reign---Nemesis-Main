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
	void addBehaviorPick(vecstr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
	void message(std::string input);

public slots:
	void milestoneUp();
	void GenerateBehavior();
	void BehaviorCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, mapSetString newAnimEvent, mapSetString newAnimVariable, std::unordered_map<std::string, var> AnimVar, std::unordered_map<std::string, std::unordered_map<int, bool>> ignoreFunction, bool isCharacter);
	void AnimDataCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::vector<std::unique_ptr<registerAnimation>>& animationList, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation);
	void ASDCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, std::unordered_map<std::string, var> AnimVar);
	void hkxcmdOutput(std::string filename, std::string hkxfile);
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

	vecstr behaviorPriority;
	std::unordered_map<std::string, bool> chosenBehavior;
	std::mutex pLock;
};


bool readMod(std::string& errormod);

#endif