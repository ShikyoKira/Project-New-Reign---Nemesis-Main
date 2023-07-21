#ifndef BEHAVIORPROCESS_H_
#define BEHAVIORPROCESS_H_

#include <unordered_set>

#include <QtCore/QObject>

#include "Global.h"

#include "ui/ProgressUp.h"

#include "generate/hkxcompiler.h"

#include "core/querymanager.h"

#include "animdata/animdata-singlefile.h"

class NewAnimation;
class NemesisEngine;

struct var;
class NemesisInfo;
struct TemplateDataBase;
struct registerAnimation;

namespace nemesis
{
    struct AnimTemplate;
    struct AnimQueryFile;
    struct HkxBehaviorFile;
}

class BehaviorStart : public QObject
{
    Q_OBJECT

public:
    using MapSetString = UMap<std::string, Set<std::string>>;

    const VecStr behaviorPriority;
    const UMap<std::string, bool> chosenBehavior;

    BehaviorStart();
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
    void GetBehaviorList();
    void LinkTemplateToBehaviors();
    void ParseBehaviorList();
    void GenerateQueryList();
    void AddQueriesToBehaviors();
    void ExportBehaviorList();

    void BehaviorInitialize(nemesis::HkxBehaviorFile* behaviorptr);
    void CheckQueryInAnimDir(const std::filesystem::path& animdir);
    void AddModQueryList(const std::filesystem::path& moddir);

    void SetTimeout(size_t ms);

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
    size_t animCount = 0;
    int filenum;
    UMap<std::wstring, VecWstr> coreModList; // core filename, list of modID;

    UPtr<nemesis::AnimTemplate> animtemp;
    VecSPtr<const nemesis::AnimQueryFile> querylist;
    UPtr<nemesis::QueryManager> querymanager = std::make_unique<nemesis::QueryManager>();
    VecSPtr<nemesis::HkxBehaviorFile> behaviorlist;
    UPtr<nemesis::animdata::SingleFile> adsf;

    SPtr<bool> running_ptr       = std::make_shared<bool>(true);
    SPtr<size_t> timeout_timer_ptr = std::make_shared<size_t>(0);

    ProgressUp behaviorProcess;

    std::string* directory2;
    VecStr* filelist2;
    TemplateDataBase* BehaviorTemplate2;
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
};

#endif
