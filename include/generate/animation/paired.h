#ifndef PAIRED_H_
#define PAIRED_H_

#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, std::string> AOConvert;
typedef std::unordered_map<std::string, int> id;

class Paired
{
private:
    vecstr pairedlines;

    vecstr eventID;
    vecstr variableID;
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
           vecstr pairedformat,
           int pairedcount,
           AOConvert import,
           const std::string& filepath,
           int stateID);
    bool HasError();
    vecstr GetPairedLine(int ID, AOConvert AnimObjectNames, id eventid, const id& variableid);
    AOConvert GetAnimObjectName();
    vecstr GetEventID();
    vecstr GetVariableID();
    std::string mainAnimEvent;
};

#endif