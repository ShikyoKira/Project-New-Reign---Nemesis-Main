#ifndef BEHAVIORSUBPROCESS_H_
#define BEHAVIORSUBPROCESS_H_

#include <memory>

#include <QtCore/QObject>

#include "behaviortype.h"

#include "generate/animation/newanimation.h"

class BehaviorStart;

struct var;
class NemesisInfo;
struct TemplateInfo;

class BehaviorSub : public QObject
{
    Q_OBJECT

public:
    BehaviorSub(const NemesisInfo* _ini);

    void addInfo(const std::string& newDirectory,
                 const std::string& curfile,
                 std::shared_ptr<TemplateInfo> newBehaviorTemplate,
                 std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>> addAnimation,
                 std::unordered_map<std::string, var> newAnimVar,
                 mapSetString addAnimEvent,
                 mapSetString addAnimVariable,
                 std::unordered_map<std::string, std::unordered_map<int, bool>> newIgnoreFunction,
                 bool newIsCharacter,
                 const std::string& newModID,
                 BehaviorStart* newProcess);
    void checkAnimation();
    void addAnimation();
    void modPick(std::unordered_map<std::string, std::vector<std::pair<uint, std::shared_ptr<std::string>>>>&
                     modEditStore,
                 std::vector<std::pair<uint, std::string>>& catalyst,
                 std::vector<std::pair<uint, std::string>>& modLine,
                 bool& hasDeleted);
    bool modPickProcess(
        std::unordered_map<std::string, std::vector<std::pair<uint, std::shared_ptr<std::string>>>>&
            modEditStore,
        std::vector<std::pair<uint, std::string>>& catalyst,
        std::vector<std::pair<uint, std::string>>& modLine,
        bool& hasDeleted);
    void CompilingBehavior();
    void CompilingAnimData();
    void CompilingASD();

    bool isCharacter;

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

    std::string modID, directory, curfilefromlist;
    std::shared_ptr<TemplateInfo> BehaviorTemplate;
    std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>> newAnimation;
    mapSetString newAnimEvent;
    mapSetString newAnimVariable;
    std::unordered_map<std::string, var> AnimVar;
    std::unordered_map<std::string, std::unordered_map<int, bool>> ignoreFunction;
    BehaviorStart* process;
    const NemesisInfo* nemesisInfo;

    void (BehaviorSub::*tryAddAnim)() = &BehaviorSub::checkAnimation;
};

#endif
