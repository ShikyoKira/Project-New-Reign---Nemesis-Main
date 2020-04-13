#ifndef ANIMATIONUTILITY_H_
#define ANIMATIONUTILITY_H_

struct JointTemplate;
struct GroupTemplate;
struct ExistingFunction;
struct NodePackedParameters;

#include <memory>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, int> ID;

struct AnimationUtility
{
    ID eventid;
    ID variableid;
    std::vector<int> fixedStateID;
    std::vector<int> stateCountMultiplier;
    bool hasGroup   = false;
    int optionMulti = -1;
    int animMulti   = -1;
    int groupMulti  = -1;
    std::string multiOption;
    std::string originalCondition;
    JointTemplate* currentProcess{};
    std::shared_ptr<NodePackedParameters> nodeProcess;
    bool isExisting = false;

    AnimationUtility()
    {}
    AnimationUtility(std::string condition,
                     ID eventid,
                     ID variableid,
                     std::vector<int> stateID,
                     std::vector<int> stateCountMultiplier,
                     bool hasGroup,
                     int optionMulti         = -1,
                     int animMulti           = -1,
                     std::string multiOption = "");
};

class NewStateID
{
    std::vector<std::shared_ptr<int>> lastState;

public:
    newStateID();
    void push_back(std::shared_ptr<int> num);
    void reset();
    std::shared_ptr<int>& operator[](uint number);
    uint size();
    bool stateUpdate(int ID,
                     std::string format,
                     std::string behaviorFile,
                     int linecount,
                     const std::string& state,
                     bool hasGroup = false);
};

#endif
