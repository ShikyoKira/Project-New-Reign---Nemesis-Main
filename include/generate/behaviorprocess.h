#ifndef BEHAVIORPROCESS_H_
#define BEHAVIORPROCESS_H_

#include "Global.h"

#include <QtCore/QObject>

#include "ui/ProgressUp.h"

class NewAnimation;
class NemesisEngine;

struct var;
struct NemesisInfo;
struct TemplateInfo;
struct registerAnimation;

class BehaviorStart : public QObject
{
    Q_OBJECT

public:
    typedef std::unordered_map<std::string, std::set<std::string>> mapSetString;

    BehaviorStart(const NemesisInfo* _ini);
    virtual ~BehaviorStart();
    void milestoneStart();
    void addBehaviorPick(NemesisEngine* newWidget,
                         VecStr behaviorOrder,
                         std::unordered_map<std::string, bool> behaviorPick);
    void addBehaviorPick(VecStr behaviorOrder, std::unordered_map<std::string, bool> behaviorPick);
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
    bool cmdline  = false;
    int animCount = 0;
    int filenum;
    std::unordered_map<std::string, VecStr> coreModList; // core filename, list of modID;

    VecStr behaviorPriority;
    std::unordered_map<std::string, bool> chosenBehavior;
    ProgressUp behaviorProcess;

    std::string* directory2;
    VecStr* filelist2;
    TemplateInfo* BehaviorTemplate2;
    std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>>* newAnimation2;
    std::vector<std::unique_ptr<registerAnimation>>* animationList2;
    mapSetString* newAnimEvent2;
    mapSetString* newAnimVariable2;
    std::unordered_map<std::string, var>* AnimVar2;
    std::unordered_map<std::string, std::unordered_map<int, bool>>* ignoreFunction2;

    std::atomic_flag upFlag{};

    // nemesis ini
    const NemesisInfo* nemesisInfo;

    // timer
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
};

#endif
