#ifndef BEHAVIORPROCESS_H_
#define BEHAVIORPROCESS_H_

#include <QtCore/QObject>

#include <boost/atomic.hpp>

#include "ui/ProgressUp.h"

class NemesisEngine;
struct TemplateInfo;
class NewAnimation;
struct registerAnimation;
struct var;

class BehaviorStart : public QObject
{
	Q_OBJECT

public:
	typedef std::unordered_map<std::string, std::set<std::string>> mapSetString;

	BehaviorStart();
	virtual ~BehaviorStart();
	void milestoneStart();
	void addBehaviorPick(NemesisEngine* newWidget, vecstr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
	void addBehaviorPick(vecstr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
	void message(std::string input);
	void GenerateBehavior(std::thread*& checkThread);

public slots:
	void newMilestone();
	void increaseAnimCount();
	void InitializeGeneration();
	void unregisterProcess(bool skip = false);
	void EndAttempt();

signals:
	void totalAnim(int);
	void progressMax(int);
	void progressUp();
	void end();
	void disable(bool);
	void disableCheck(bool);
	void hide(bool);
	void incomingMessage(QString);
	void disableLaunch();

private:
	bool cmdline = false;
	int animCount = 0;
	int filenum;
	std::unordered_map<std::string, vecstr> coreModList;		// core filename, list of modID;

	vecstr behaviorPriority;
	std::unordered_map<std::string, bool> chosenBehavior;
	ProgressUp behaviorProcess;

	std::string* directory2;
	vecstr* filelist2;
	TemplateInfo* BehaviorTemplate2;
	std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>>* newAnimation2;
	std::vector<std::unique_ptr<registerAnimation>>* animationList2;
	mapSetString* newAnimEvent2;
	mapSetString* newAnimVariable2;
	std::unordered_map<std::string, var>* AnimVar2;
	std::unordered_map<std::string, std::unordered_map<int, bool>>* ignoreFunction2;

	boost::atomic_flag upFlag = BOOST_ATOMIC_FLAG_INIT;

	// timer
	boost::posix_time::ptime start_time;
};


#endif
