#ifndef PAIRED_H_
#define PAIRED_H_

#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<std::string> VecStr;
typedef std::unordered_map<std::string, std::string> AOConvert;
typedef std::unordered_map<std::string, int> ID;

class Paired
{
private:
    VecStr pairedlines;

    VecStr eventID;
    VecStr variableID;
    std::string nextState;
    std::unordered_map<int, std::string> AObject;
    AOConvert NewAnimObject;
    int nextFunctionID{};
    int blend = 0;
    double duration;
    bool AC        = false;
    bool AC0       = false;
    bool AC1       = false;
    bool Loop      = true;
    bool AO        = false;
    bool PAO       = false;
    bool HT        = false;
    bool Known     = false;
    bool MD        = false;
    bool nextTrans = false;
    AOConvert TriggerTime;
    AOConvert TriggerTime1;

    bool error = false;

public:
    Paired(const std::string& line,
           VecStr pairedformat,
           int pairedcount,
           AOConvert import,
           const std::string& filepath,
           int stateID);
    bool HasError();
    VecStr GetPairedLine(int id, AOConvert AnimObjectNames, ID eventid, const ID& variableid);
    AOConvert GetAnimObjectName();
    VecStr GetEventID();
    VecStr GetVariableID();
    std::string mainAnimEvent;
};

#endif
