#ifndef BEHAVIORPROCESS_H_
#define BEHAVIORPROCESS_H_

#include <unordered_set>

#include <QtCore/QObject>

#include "Global.h"

#include "ui/ProgressUp.h"

#include "generate/hkxcompiler.h"

class NewAnimation;
class NemesisEngine;

struct var;
class NemesisInfo;
struct TemplateInfo;
struct registerAnimation;

namespace nemesis
{
    struct AnimTemplate;
    struct AnimQueryList;
    struct HkxBehavior;
}

class BehaviorStart : public QObject
{
    Q_OBJECT

public:
    using MapSetString = UMap<std::string, Set<std::string>>;

    const VecStr behaviorPriority;
    const UMap<std::string, bool> chosenBehavior;

    BehaviorStart(const NemesisInfo* _ini);
    virtual ~BehaviorStart();
    void milestoneStart();
    void addBehaviorPick(NemesisEngine* newWidget,
                         VecStr behaviorOrder,
                         UMap<std::string, bool> behaviorPick);
    void addBehaviorPick(VecStr behaviorOrder, UMap<std::string, bool> behaviorPick);
    void message(std::string input);
    void message(std::wstring input);
    void GenerateBehavior(UPtr<std::thread>& checkThread);

    void GenerateAnimTemplate();
    void GenerateQueryList();
    void GetBehaviorList();
    void ExportBehaviorList();

    void BehaviorInitialize(const std::wstring& behaviorfile, SPtr<nemesis::HkxBehavior>& behaviorptr);
    void CheckQueryInAnimDir(const std::filesystem::path& animdir);
    void TryAddModQueryList(const std::filesystem::path& moddir);

    std::atomic_flag& getNewAnimFlag();

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
    void criticalError(QString, QString);

private:
    bool cmdline  = false;
    bool running  = true;
    int timeout_timer = 0;
    int animCount = 0;
    int filenum;
    UMap<std::wstring, VecWstr> coreModList; // core filename, list of modID;

    UPtr<nemesis::AnimTemplate> animtemp;
    VecSPtr<nemesis::AnimQueryList> querylist;
    VecSPtr<nemesis::HkxBehavior> behaviorlist;

    ProgressUp behaviorProcess;

    std::string* directory2;
    VecStr* filelist2;
    TemplateInfo* BehaviorTemplate2;
    UMap<std::string, std::vector<std::shared_ptr<NewAnimation>>>* newAnimation2;
    std::vector<std::unique_ptr<registerAnimation>>* animationList2;
    MapSetString* newAnimEvent2;
    MapSetString* newAnimVariable2;
    UMap<std::string, var>* AnimVar2;
    UMap<std::string, UMap<int, bool>>* ignoreFunction2;

    std::atomic_flag upFlag{};
    std::atomic_flag newAnimFlag{};

    // timer
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

public:
    std::atomic_flag postBehaviorFlag{};
    UMap<std::wstring, USetWstr> postBhvrRefBy;

    // nemesis ini
    const NemesisInfo* nemesisInfo;
};

#endif
