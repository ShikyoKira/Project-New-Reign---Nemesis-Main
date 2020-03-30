#ifndef BEHAVIORSUBPROCESS_H_
#define BEHAVIORSUBPROCESS_H_

#include <memory>

#include <QtCore/QObject>

#include "behaviortype.h"

#include "generate/animation/newanimation.h"

class BehaviorStart;
struct TemplateInfo;
struct var;

class BehaviorSub : public QObject
{
    Q_OBJECT

public:
    bool isCharacter;

    void addInfo(std::string& newDirectory,
                 VecStr& newfilelist,
                 int newCurList,
                 VecStr& newBehaviorPriority,
                 std::unordered_map<std::string, bool>& newChosenBehavior,
                 std::shared_ptr<TemplateInfo> newBehaviorTemplate,
                 std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>>& addAnimation,
                 std::unordered_map<std::string, var>& newAnimVar,
                 mapSetString& addAnimEvent,
                 mapSetString& addAnimVariable,
                 std::unordered_map<std::string, std::unordered_map<int, bool>>& newIgnoreFunction,
                 bool newIsCharacter,
                 std::string newModID,
                 BehaviorStart* newProcess);
    void checkAnimation();
    void addAnimation();
    void modPick(std::unordered_map<std::string, std::vector<std::shared_ptr<std::string>>>& modEditStore,
                 VecStr& catalyst,
                 VecStr& modLine,
                 bool& hasDeleted);
    bool
    modPickProcess(std::unordered_map<std::string, std::vector<std::shared_ptr<std::string>>>& modEditStore,
                   VecStr& catalyst,
                   VecStr& modLine,
                   bool& hasDeleted);
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
    VecStr filelist;
    int curList;
    VecStr behaviorPriority;
    std::unordered_map<std::string, bool> chosenBehavior;
    std::shared_ptr<TemplateInfo> BehaviorTemplate;
    std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>> newAnimation;
    mapSetString newAnimEvent;
    mapSetString newAnimVariable;
    std::unordered_map<std::string, var> AnimVar;
    std::unordered_map<std::string, std::unordered_map<int, bool>> ignoreFunction;
    BehaviorStart* process;

    void (BehaviorSub::*tryAddAnim)() = &BehaviorSub::checkAnimation;
};

#endif
