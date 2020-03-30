#include "Global.h"

#include "utilities/compute.h"

#include "generate/alternateanimation.h"
#include "generate/animationdatatracker.h"

#include "generate/animation/grouptemplate.h"
#include "generate/animation/singletemplate.h"
#include "generate/animation/templateprocessing.h"
#include "generate/animation/templatetree.h"

using namespace std;

string strDoubleDecimal(double num, int decimal)
{
    if (num != static_cast<int>(num))
    {
        stringstream ss;
        ss << fixed << setprecision(decimal);
        ss << num;
        string strNum = ss.str();
        strNum.erase(strNum.find_last_not_of('0') + 1, NOT_FOUND);
        return strNum;
    }
    else
    {
        stringstream ss;
        ss << fixed << setprecision(decimal);
        ss << static_cast<int>(num);
        return ss.str();
    }
}

void stateInput(string& state,
                string stateID,
                string format,
                string behaviorFile,
                string original,
                bool isMaster,
                int groupMulti,
                int animMulti,
                int numline,
                size_t& stateCount,
                shared_ptr<master>& masterFunction)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, original);

    if (isMaster)
    {
        if (masterFunction->grouplist[groupMulti]->functionIDs.find(stateID)
            != masterFunction->grouplist[groupMulti]->functionIDs.end())
        { state = masterFunction->grouplist[groupMulti]->functionIDs[stateID]; }
        else
        {
            ErrorMessage(1127, format, behaviorFile, numline);
        }
    }
    else
    {
        if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, original);

        if (masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format.find(stateID)
            != masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format.end())
        { state = masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format[stateID]; }
        else
        {
            ErrorMessage(1127, format, behaviorFile, numline);
        }
    }
}

string nemesis::smatch::operator[](int number)
{
    return match[number];
}

size_t nemesis::smatch::position(int number)
{
    return positionlist[number];
}

size_t nemesis::smatch::size()
{
    return match.size();
}

namespace nemesis
{
    bool regex_search(string line, nemesis::smatch& n_match, boost::regex rgx)
    {
        n_match = nemesis::smatch();
        boost::smatch match;

        if (!boost::regex_search(string(line), match, rgx)) return false;

        if (match.size() > 1)
        {
            n_match.match.reserve(match.size());
            n_match.positionlist.reserve(match.size());
            n_match.positionlist.push_back(match.position());
            n_match.match.push_back(line.substr(match.position(), match[0].length()));

            for (unsigned int i = 1; i < match.size(); ++i)
            {
                n_match.positionlist.push_back(match.position(i));
                n_match.match.push_back(line.substr(match.position(i), match[i].length()));
            }
        }

        return true;
    }
} // namespace nemesis

range::range(size_t n_front, size_t n_back, void (proc::*n_func)(range, vecstr&))
{
    front = n_front;
    back  = n_back - 1;
    size  = back - front;
    func  = n_func;
}

range::range(size_t n_front, size_t n_back, vecstr n_olddata, void (proc::*n_func)(range, vecstr&))
{
    front   = n_front;
    back    = n_back - 1;
    size    = back - front;
    olddata = n_olddata;
    func    = n_func;
}

range::range(size_t n_front, size_t n_back, vector<int> n_olddataint, void (proc::*n_func)(range, vecstr&))
{
    front      = n_front;
    back       = n_back - 1;
    size       = back - front;
    olddataint = n_olddataint;
    func       = n_func;
}

range::range(size_t n_front,
             size_t n_back,
             vector<int> n_olddataint,
             vecstr n_olddata,
             void (proc::*n_func)(range, vecstr&))
{
    front      = n_front;
    back       = n_back - 1;
    size       = back - front;
    olddataint = n_olddataint;
    olddata    = n_olddata;
    func       = n_func;
}

void proc::Register(string n_format,
                    string n_masterformat,
                    string n_behaviorFile,
                    string n_filepath,
                    string n_filename,
                    string n_mainAnimEvent,
                    string& n_strID,
                    string n_zeroEvent,
                    string n_zeroVariable,
                    bool n_hasGroup,
                    bool& n_negative,
                    bool& n_isEnd,
                    bool& n_norElement,
                    bool& n_elementCatch,
                    bool n_hasDuration,
                    double n_duration,
                    int& n_openRange,
                    int& n_counter,
                    size_t& n_elementLine,
                    int n_furnitureCount,
                    id& n_eventid,
                    id& n_variableid,
                    vector<int> n_fixedStateID,
                    vector<int> n_stateCountMultiplier,
                    NewAnimLock* n_animLock,
                    int n_order,
                    int n_lastorder,
                    SSMap& n_IDExist,
                    unordered_map<int, vecstr>& n_AnimObject,
                    ImportContainer& n_addition,
                    ImportContainer* n_newImport,
                    unordered_map<string, unordered_map<string, vecstr>>& n_groupAddition,
                    vector<unordered_map<string, bool>>& n_groupOptionPicked,
                    shared_ptr<group> n_groupFunction,
                    shared_ptr<vecstr> n_generatedlines,
                    NewAnimation* n_curAnim)
{
    format               = n_format;
    masterformat         = n_masterformat;
    behaviorFile         = n_behaviorFile;
    filepath             = n_filepath;
    filename             = n_filename;
    mainAnimEvent        = n_mainAnimEvent;
    strID                = &n_strID;
    zeroEvent            = n_zeroEvent;
    zeroVariable         = n_zeroVariable;
    hasGroup             = n_hasGroup;
    negative             = &n_negative;
    isEnd                = &n_isEnd;
    norElement           = &n_norElement;
    elementCatch         = &n_elementCatch;
    hasDuration          = n_hasDuration;
    duration             = n_duration;
    openRange            = &n_openRange;
    counter              = &n_counter;
    elementLine          = &n_elementLine;
    furnitureCount       = n_furnitureCount;
    eventid              = &n_eventid;
    variableid           = &n_variableid;
    fixedStateID         = n_fixedStateID;
    stateCountMultiplier = n_stateCountMultiplier;
    animLock             = n_animLock;
    order                = n_order;
    lastorder            = n_lastorder;
    IDExist              = &n_IDExist;
    AnimObject           = &n_AnimObject;
    addition             = &n_addition;
    newImport            = n_newImport;
    groupAddition        = &n_groupAddition;
    groupOptionPicked    = &n_groupOptionPicked;
    groupFunction        = n_groupFunction;
    generatedlines       = n_generatedlines;
    curAnim              = n_curAnim;
}

void proc::installBlock(range blok, int curline)
{
    blockCheck(blok.front, blok.back);
    lineblocks[curline].blocksize[blok.size].push_back(blok);
}

void proc::installBlock(range blok, int curline, vector<multichoice> m_condiiton)
{
    blockCheck(blok.front, blok.back);
    hasMC[curline]       = true;
    multiChoice[curline] = m_condiiton;
    choiceblocks[curline].push_back(blok);
}

void proc::relativeNegative(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (*isEnd || *negative) { blocks[blok.front] = "true"; }
        else
        {
            blocks[blok.front] = "false";
        }

        *negative = false;
        *isEnd    = false;
    }
}

void proc::compute(range blok, vecstr& blocks)
{
    *elementCatch = true;
    *norElement   = true;
}

void proc::rangeCompute(range blok, vecstr& blocks)
{
    (*generatedlines)[*elementLine]
        = boost::regex_replace(string((*generatedlines)[*elementLine]),
                               boost::regex("(.*<hkparam name\\=\".+\" numelements\\=\").+(\">.*)"),
                               string("\\1" + to_string(*counter) + "\\2"));
    *norElement  = false;
    *counter     = 0;
    *elementLine = -1;
}

void proc::upCounter(range blok, vecstr& blocks)
{
    ++(*counter);
}

void proc::upCounterPlus(range blok, vecstr& blocks)
{
    string full = combineBlocks(0, blocks.size() - 1, blocks);
    (*counter) += count(full.begin(), full.end(), '#');
}

void proc::animCount(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = to_string(furnitureCount);
}

void proc::multiChoiceRegis(range blok, vecstr& blocks)
{
    if (!captured) { blocks[blok.front] = "null"; }
    else
    {
        string result = combineBlocks(captured->front, captured->back, blocks);
        clearBlocks(blok, blocks);
        blocks[blok.front] = result;
    }
}

void proc::groupIDRegis(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string oldID = masterformat + "_group$" + blok.olddata[0];
        animLock->lockID();

        if (groupFunction->functionIDs.find(oldID) != groupFunction->functionIDs.end())
        { blocks[blok.front] = groupFunction->functionIDs[oldID]; }
        else
        {
            groupFunction->functionIDs[oldID] = *strID;
            blocks[blok.front]                = *strID;
            curAnim->newID();
        }

        animLock->releaseID();
    }
}

void proc::IDRegis(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string ID;
        string oldID = masterformat + "$" + blok.olddata[0];

        if ((*IDExist)[oldID].length() > 0) { ID = (*IDExist)[oldID]; }
        else
        {
            (*IDExist)[oldID] = *strID;
            ID                = *strID;
            curAnim->newID();
        }

        curAnim->subFunctionIDs->format[oldID] = ID;
        blocks[blok.front]                     = ID;
    }
}

void proc::IDRegisAnim(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string ID;
        string oldID = masterformat + "$" + blok.olddata[0];
        int n_groupMulti;

        if (isMaster && groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[1]);

        n_groupMulti = groupMulti;

        if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, blok.olddata[0]);

        if (masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format.find(oldID)
            != masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format.end())
        {
            ID                = masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format[oldID];
            (*IDExist)[oldID] = ID;
        }
        else
        {
            if ((*IDExist)[oldID].length() > 0) { ID = (*IDExist)[oldID]; }
            else
            {
                (*IDExist)[oldID] = *strID;
                ID                = *strID;
                curGroup->newID();
            }

            masterFunction->grouplist[n_groupMulti]->singlelist[animMulti]->format[oldID] = ID;
        }

        blocks[blok.front] = ID;
    }
}

void proc::IDRegisGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string ID;
        string oldID = masterformat + "_group$" + blok.olddata[0];
        int n_groupMulti;

        if (isMaster && groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[1]);

        n_groupMulti = groupMulti;

        if (masterFunction->grouplist[n_groupMulti]->functionIDs.find(oldID)
            != masterFunction->grouplist[n_groupMulti]->functionIDs.end())
        {
            ID                = masterFunction->grouplist[n_groupMulti]->functionIDs[oldID];
            (*IDExist)[oldID] = ID;
        }
        else
        {
            if ((*IDExist)[oldID].length() > 0) { ID = (*IDExist)[oldID]; }
            else
            {
                (*IDExist)[oldID] = *strID;
                ID                = *strID;
                curGroup->newID();
            }

            masterFunction->grouplist[n_groupMulti]->functionIDs[oldID] = ID;
        }

        blocks[blok.front] = ID;
    }
}

void proc::IDRegisMaster(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string ID;
        string oldID = masterformat + "_master$" + blok.olddata[0];

        if (masterFunction->functionIDs.find(oldID) != masterFunction->functionIDs.end())
        {
            ID                = masterFunction->functionIDs[oldID];
            (*IDExist)[oldID] = ID;
        }
        else
        {
            if ((*IDExist)[oldID].length() > 0) { ID = (*IDExist)[oldID]; }
            else
            {
                (*IDExist)[oldID] = *strID;
                ID                = *strID;
                curGroup->newID();
            }

            masterFunction->functionIDs[oldID] = ID;
        }

        blocks[blok.front] = ID;
    }
}

void proc::computation(range blok, vecstr& blocks)
{
    string equation       = combineBlocks(blok, blocks);
    size_t equationLength = equation.length();

    if (clearBlocks(blok, blocks))
    {
        if (equation.find("L") != NOT_FOUND)
        {
            if (isMaster) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);
            if (isGroup) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);

            int maths2 = count(equation.begin(), equation.end(), 'L');

            for (__int64 k = 0; k < maths2; ++k)
            {
                equation.replace(equation.find("L"), 1, to_string(lastorder));
            }
        }

        if (equation.find("N") != NOT_FOUND)
        {
            if (isMaster) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);
            if (isGroup) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);

            int maths2 = count(equation.begin(), equation.end(), 'N');
            string nextorder;
            nextorder = curAnim->isLast() ? to_string(order) : to_string(order + 1);

            for (__int64 k = 0; k < maths2; ++k)
            {
                equation.replace(equation.find("N"), 1, nextorder);
            }
        }

        if (equation.find("B") != NOT_FOUND)
        {
            if (isMaster) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);
            if (isGroup) ErrorMessage(1206, format, behaviorFile, numline, blok.olddata[0]);

            int maths2 = count(equation.begin(), equation.end(), 'B');
            string preorder;
            preorder = order == 0 ? to_string(order) : to_string(order - 1);

            for (__int64 k = 0; k < maths2; ++k)
            {
                equation.replace(equation.find("B"), 1, preorder);
            }
        }

        if (equation.find("F") != NOT_FOUND)
        {
            int maths2 = count(equation.begin(), equation.end(), 'F');

            for (__int64 k = 0; k < maths2; ++k)
            {
                equation.replace(equation.find("F"), 1, "0");
            }
        }

        nemesis::calculate(equation, format, behaviorFile, numline);
        blocks[blok.front] = equation;
    }
}

void proc::endMultiGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, blok.olddata[0]);

        if (curAnim->GetGroupAnimInfo()[animMulti]->hasDuration)
        { blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[animMulti]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endFirstGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (curAnim->GetGroupAnimInfo()[0]->hasDuration)
        { blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[0]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endNextGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        int num = order;

        if (!curAnim->isLast()) num++;

        if (curAnim->GetGroupAnimInfo()[num]->hasDuration)
        { blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[num]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endBackGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        int num = order;

        if (order != 0) num--;

        if (curAnim->GetGroupAnimInfo()[num]->hasDuration)
        { blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[num]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endLastGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (curAnim->GetGroupAnimInfo()[curAnim->GetGroupAnimInfo().size()]->hasDuration)
        { blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[lastorder]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endNumGroup(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        if (curAnim->GetGroupAnimInfo()[num]->hasDuration)
        { blocks[blok.front] = to_string(curAnim->GetGroupAnimInfo()[num]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endMultiMaster(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, blok.olddata[0]);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        if (curGroup->groupAnimInfo[groupMulti][animMulti]->hasDuration)
        { blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][animMulti]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endFirstMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        if (curGroup->groupAnimInfo[groupMulti][0]->hasDuration)
        { blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][0]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endLastMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        if (curGroup->groupAnimInfo[groupMulti][curGroup->groupAnimInfo[groupMulti].size()]->hasDuration)
        { blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][lastorder]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endNumMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        if (curGroup->groupAnimInfo[groupMulti][num]->hasDuration)
        { blocks[blok.front] = to_string(curGroup->groupAnimInfo[groupMulti][num]->duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::endSingle(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (hasDuration) { blocks[blok.front] = to_string(duration); }
        else
        {
            blocks[blok.front] = "0.000000";
            *isEnd             = true;
        }
    }
}

void proc::stateMultiGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
        blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]]
                                       + ((animMulti - order) * stateCountMultiplier[blok.olddataint[0]])
                                       + blok.olddataint[1]);
}

void proc::stateFirstGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = to_string(fixedStateID[blok.olddataint[0]] - (order * stateCountMultiplier[blok.olddataint[0]])
                        + blok.olddataint[1]);
}

void proc::stateNextGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        curAnim->isLast()
            ? blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + blok.olddataint[1])
            : blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]]
                                             + stateCountMultiplier[blok.olddataint[0]] + blok.olddataint[1]);
    }
}

void proc::stateBackGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        order == 0
            ? blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]] + blok.olddataint[1])
            : blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]]
                                             - stateCountMultiplier[blok.olddataint[0]] + blok.olddataint[1]);
    }
}

void proc::stateLastGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        blocks[blok.front] = to_string(fixedStateID[blok.olddataint[0]]
                                       + ((lastorder - order) * stateCountMultiplier[blok.olddataint[0]])
                                       + blok.olddataint[1]);
    }
}

void proc::stateNumGroup(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (blok.olddataint[2] >= int(curAnim->GetGroupAnimInfo().size()))
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        blocks[blok.front]
            = to_string(fixedStateID[num] + ((blok.olddataint[2] - order) * stateCountMultiplier[num])
                        + blok.olddataint[1]);
    }
}

void proc::stateMultiMasterToGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   true,
                   groupMulti,
                   animMulti,
                   numline,
                   stateCount,
                   masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateMultiMaster(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   groupMulti,
                   animMulti,
                   numline,
                   stateCount,
                   masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateFirstMaster(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   groupMulti,
                   0,
                   numline,
                   stateCount,
                   masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateLastMaster(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   groupMulti,
                   masterFunction->grouplist[groupMulti]->singlelist.size() - 1,
                   numline,
                   stateCount,
                   masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateNumMaster(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[2];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   groupMulti,
                   num,
                   numline,
                   stateCount,
                   masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateSingle(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string state    = to_string(fixedStateID[blok.olddataint[0]] + blok.olddataint[1]);
        string original = "(S+" + to_string(blok.olddataint[1]) + ")";

        if (isMaster)
            masterFunction->functionIDs[original] = state;
        else if (isGroup)
            masterFunction->grouplist[groupMulti]->functionIDs[original] = state;

        blocks[blok.front] = state;
    }
}

void proc::filepathMultiGroup(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[animMulti]->filename;
}

void proc::filepathFirstGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[0]->filename;
}

void proc::filepathNextGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        curAnim->isLast() ? blocks[blok.front] = filepath + filename
                          : blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[order + 1]->filename;
    }
}

void proc::filepathBackGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        order == 0 ? blocks[blok.front] = filepath + filename
                   : blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[order - 1]->filename;
    }
}

void proc::filepathLastGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
        blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[lastorder]->filename;
}

void proc::filepathNumGroup(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + curAnim->GetGroupAnimInfo()[num]->filename;
}

void proc::filepathMultiMaster(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format["FilePath"];
}

void proc::filepathFirstMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist[0]->format["FilePath"];
}

void proc::filepathLastMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = masterFunction->grouplist[groupMulti]->singlelist.back()->format["FilePath"];
}

void proc::filepathNumMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = masterFunction->grouplist[groupMulti]->singlelist[blok.olddataint[0]]->format["FilePath"];
}

void proc::filepathSingle(range blok, vecstr& blocks)
{
    if (filename == combineBlocks(blok, blocks)) ErrorMessage(1134, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks)) blocks[blok.front] = filepath + filename;
}

void proc::filenameMultiGroup(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        string* tempfile   = &curAnim->GetGroupAnimInfo()[animMulti]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameFirstGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string* tempfile   = &curAnim->GetGroupAnimInfo()[0]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameNextGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (curAnim->isLast()) { blocks[blok.front] = filename.substr(0, filename.find_last_of(".")); }
        else
        {
            string* tempfile   = &curAnim->GetGroupAnimInfo()[order + 1]->filename;
            blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
        }
    }
}

void proc::filenameBackGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (order == 0) { blocks[blok.front] = filename.substr(0, filename.find_last_of(".")); }
        else
        {
            string* tempfile   = &curAnim->GetGroupAnimInfo()[order - 1]->filename;
            blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
        }
    }
}

void proc::filenameLastGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        string* tempfile   = &curAnim->GetGroupAnimInfo()[lastorder]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameNumGroup(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        string* tempfile   = &curAnim->GetGroupAnimInfo()[num]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameMultiMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        string* tempfile = &masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameFirstMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        string* tempfile   = &masterFunction->grouplist[groupMulti]->singlelist[0]->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameLastMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        string* tempfile   = &masterFunction->grouplist[groupMulti]->singlelist.back()->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameNumMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        string* tempfile
            = &masterFunction->grouplist[groupMulti]->singlelist[blok.olddataint[0]]->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameSingle(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = filename.substr(0, filename.find_last_of("."));
}

void proc::pathSingle(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if (isGroup)
            blocks[blok.front] = masterFunction->grouplist[0]->singlelist[0]->format["Path"];
        else if (isMaster)
            blocks[blok.front] = masterFunction->grouplist[0]->singlelist[0]->format["Path"];
        else
            blocks[blok.front] = filepath.substr(0, filepath.length() - 1);
    }
}

void proc::AOMultiGroupA(range blok, vecstr& blocks)
{
    if (animMulti == -1 || optionMulti == -1)
        ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curAnim->GetGroupAnimInfo()[animMulti]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOMultiGroupB(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curAnim->GetGroupAnimInfo()[animMulti]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AOFirstGroupA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curAnim->GetGroupAnimInfo()[0]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOFirstGroupB(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curAnim->GetGroupAnimInfo()[0]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AONextGroupA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) { ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks)); }

    if (clearBlocks(blok, blocks))
    {
        vecstr list;

        curAnim->isLast() ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]]
                          : list = curAnim->GetGroupAnimInfo()[order + 1]->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > optionMulti) blocks[blok.front] = list[optionMulti];
    }
}

void proc::AONextGroupB(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        vecstr list;
        curAnim->isLast() ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]]
                          : list = curAnim->GetGroupAnimInfo()[order + 1]->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > blok.olddataint[1]) { blocks[blok.front] = list[blok.olddataint[1]]; }
    }
}

void proc::AOBackGroupA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks))
    {
        vecstr list;
        order == 0 ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]]
                   : list = curAnim->GetGroupAnimInfo()[order - 1]->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > optionMulti) blocks[blok.front] = list[optionMulti];
    }
}

void proc::AOBackGroupB(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        vecstr list;
        order == 0 ? list = curAnim->GetGroupAnimInfo()[order]->AnimObject[blok.olddataint[0]]
                   : list = curAnim->GetGroupAnimInfo()[order - 1]->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > blok.olddataint[1]) blocks[blok.front] = list[blok.olddataint[1]];
    }
}

void proc::AOLastGroupA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curAnim->GetGroupAnimInfo()[lastorder]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOLastGroupB(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curAnim->GetGroupAnimInfo()[lastorder]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AONumGroupA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curAnim->GetGroupAnimInfo()[num]->AnimObject[blok.olddataint[1]][optionMulti];
}

void proc::AONumGroupB(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curAnim->GetGroupAnimInfo()[num]->AnimObject[blok.olddataint[1]][blok.olddataint[2]];
}

void proc::AOMultiMasterA(range blok, vecstr& blocks)
{
    if (animMulti == -1 || optionMulti == -1)
        ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curGroup->groupAnimInfo[groupMulti][animMulti]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOMultiMasterB(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][animMulti]
                                 ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AOFirstMasterA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curGroup->groupAnimInfo[groupMulti][0]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOFirstMasterB(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curGroup->groupAnimInfo[groupMulti][0]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AOLastMasterA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curGroup->groupAnimInfo[groupMulti][lastorder]->AnimObject[blok.olddataint[0]][optionMulti];
}

void proc::AOLastMasterB(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][lastorder]
                                 ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::AONumMasterA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curGroup->groupAnimInfo[groupMulti][num]->AnimObject[blok.olddataint[1]][optionMulti];
}

void proc::AONumMasterB(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front]
            = curGroup->groupAnimInfo[groupMulti][num]->AnimObject[blok.olddataint[1]][blok.olddataint[2]];
}

void proc::AOSingleA(range blok, vecstr& blocks)
{
    if (optionMulti == -1) ErrorMessage(1126, format, behaviorFile, numline, combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks)) blocks[blok.front] = (*AnimObject)[blok.olddataint[0]][optionMulti];
}

void proc::AOSingleB(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = (*AnimObject)[blok.olddataint[0]][blok.olddataint[1]];
}

void proc::MAEMultiGroup(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[animMulti]->mainAnimEvent;
}

void proc::MAEFirstGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[0]->mainAnimEvent;
}

void proc::MAENextGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        curAnim->isLast() ? blocks[blok.front] = curAnim->GetGroupAnimInfo()[order]->mainAnimEvent
                          : blocks[blok.front] = curAnim->GetGroupAnimInfo()[order + 1]->mainAnimEvent;
    }
}

void proc::MAEBackGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        order == 0 ? blocks[blok.front] = curAnim->GetGroupAnimInfo()[order]->mainAnimEvent
                   : blocks[blok.front] = curAnim->GetGroupAnimInfo()[order - 1]->mainAnimEvent;
    }
}

void proc::MAELastGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[lastorder]->mainAnimEvent;
}

void proc::MAENumGroup(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks)) blocks[blok.front] = curAnim->GetGroupAnimInfo()[num]->mainAnimEvent;
}

void proc::MAEMultiMaster(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][animMulti]->mainAnimEvent;
}

void proc::MAEFirstMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks)) blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][0]->mainAnimEvent;
}

void proc::MAELastMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][lastorder]->mainAnimEvent;
}

void proc::MAENumMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
        blocks[blok.front] = curGroup->groupAnimInfo[groupMulti][num]->mainAnimEvent;
}

void proc::MAESingle(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks)) blocks[blok.front] = mainAnimEvent;
}

void proc::addOnMultiGroup(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        vecstr* list
            = &curAnim->GetGroupAnimInfo()[animMulti]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output
                = curAnim->GetGroupAnimInfo()[animMulti]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnFirstGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        vecstr* list = &curAnim->GetGroupAnimInfo()[0]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output = curAnim->GetGroupAnimInfo()[0]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnNextGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        int curorder = order;

        if (!curAnim->isLast()) ++curorder;

        vecstr* list
            = &curAnim->GetGroupAnimInfo()[curorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output = curAnim->GetGroupAnimInfo()[curorder]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnBackGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        int curorder = order;

        if (order > 0) --curorder;

        vecstr* list
            = &curAnim->GetGroupAnimInfo()[curorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output = curAnim->GetGroupAnimInfo()[curorder]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnLastGroup(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        vecstr* list
            = &curAnim->GetGroupAnimInfo()[lastorder]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            if (curAnim->GetGroupAnimInfo()[lastorder]->addition[blok.olddata[0]][blok.olddata[1]].length()
                == 0)
                ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front]
                = curAnim->GetGroupAnimInfo()[lastorder]->addition[blok.olddata[0]][blok.olddata[1]];
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnNumGroup(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        vecstr* list = &curAnim->GetGroupAnimInfo()[num]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output = curAnim->GetGroupAnimInfo()[num]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnMultiMaster(range blok, vecstr& blocks)
{
    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[2]);

    if (clearBlocks(blok, blocks))
    {
        vecstr* list = &curGroup->groupAnimInfo[groupMulti][animMulti]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output
                = curGroup->groupAnimInfo[groupMulti][animMulti]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnFirstMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[2]);

    if (clearBlocks(blok, blocks))
    {
        vecstr* list
            = &curGroup->groupAnimInfo[groupMulti][0]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output
                = curGroup->groupAnimInfo[groupMulti][0]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnLastMaster(range blok, vecstr& blocks)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[2]);

    if (clearBlocks(blok, blocks))
    {
        vecstr* list = &curGroup->groupAnimInfo[groupMulti][lastorder]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output
                = curGroup->groupAnimInfo[groupMulti][lastorder]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnNumMaster(range blok, vecstr& blocks)
{
    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        vecstr* list
            = &curGroup->groupAnimInfo[groupMulti][num]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || optionMulti == -1)
        {
            string output
                = curGroup->groupAnimInfo[groupMulti][num]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0) ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > optionMulti)
        {
            if ((*list)[optionMulti].length() == 0) ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*list)[optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnSingle(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        if ((*groupAddition)[blok.olddata[0]][blok.olddata[1]].size() == 0 || optionMulti == -1)
        {
            if ((*addition)[blok.olddata[0]][blok.olddata[1]].length() == 0)
                ErrorMessage(1117, format, filename, numline, blok.olddata[2]);

            blocks[blok.front] = (*addition)[blok.olddata[0]][blok.olddata[1]];
        }
        else if (int((*groupAddition)[blok.olddata[0]][blok.olddata[1]].size()) > optionMulti)
        {
            if ((*groupAddition)[blok.olddata[0]][blok.olddata[1]][optionMulti].length() == 0)
                ErrorMessage(1117, format, filename, numline, line);

            blocks[blok.front] = (*groupAddition)[blok.olddata[0]][blok.olddata[1]][optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::lastState(range blok, vecstr& blocks)
{
    if (clearBlocks(blok, blocks))
    {
        size_t ID = 0;

        if (fixedStateID.size() > 1)
        {
            if (blok.olddata[0].length() > 0)
            {
                ID = stoi(blok.olddata[0]) - 1;

                if (ID >= fixedStateID.size())
                    ErrorMessage(1168, format, behaviorFile, numline, "LastState" + blok.olddata[0]);
            }
        }

        blocks[blok.front] = to_string(lastorder - order + fixedStateID[ID]);
    }
}

void proc::eventID(range blok, vecstr& blocks)
{
    string eventname = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

    if (clearBlocks(blok, blocks))
    {
        string newEventID = to_string((*eventid)[eventname]);

        if (newEventID == "0" && eventname != zeroEvent)
        {
            if (format == "BASE") { ErrorMessage(1166); }
            else
            {
                ErrorMessage(1131, format, filename, numline, eventname);
            }
        }

        blocks[blok.front] = newEventID;
    }
}

void proc::variableID(range blok, vecstr& blocks)
{
    string variablename = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

    if (clearBlocks(blok, blocks))
    {
        string newVarID = to_string((*variableid)[variablename]);

        if (newVarID == "0" && variablename != zeroVariable)
        {
            if (format == "BASE") { ErrorMessage(1166); }
            else
            {
                ErrorMessage(1132, format, filename, numline, variablename);
            }
        }

        blocks[blok.front] = newVarID;
    }
}

void proc::crc32(range blok, vecstr& blocks)
{
    string crc32line = nemesis::to_lower_copy(combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks)) blocks[blok.front] = to_string(CRC32Convert(crc32line));
}

void proc::import(range blok, vecstr& blocks)
{
    string import = combineBlocks(blok, blocks);

    if (clearBlocks(blok, blocks))
    {
        if ((*IDExist)[import].length() == 0)
        {
            size_t pos     = import.find("[") + 1;
            string file    = import.substr(pos, import.find("]", pos) - pos);
            string keyword = "";
            string tempID;

            if (import.find("[", pos) != NOT_FOUND)
            {
                pos                = import.find("[", pos);
                string tempKeyword = import.substr(pos, import.find_last_of("]") + 1 - pos);
                int openBrack      = 0;

                for (unsigned int j = 0; j < tempKeyword.length(); ++j)
                {
                    char curChar = tempKeyword[j];

                    if (curChar == '[')
                        ++openBrack;
                    else if (curChar == ']')
                    {
                        --openBrack;

                        if (openBrack == 0) { keyword.append("!~^!"); }
                    }
                    else
                        keyword.push_back(curChar);
                }

                pos = keyword.rfind("!~^!");

                if (openBrack != 0 || pos == NOT_FOUND || pos != (keyword.length() - 4))
                    ErrorMessage(1139, format, behaviorFile, numline, import);
                else
                    keyword = keyword.substr(0, keyword.length() - 4);
            }

            animLock->lockExport();

            if ((*newImport)[file][keyword].length() > 0) { tempID = (*newImport)[file][keyword]; }
            else
            {
                tempID                      = *strID;
                (*IDExist)[import]          = tempID;
                (*newImport)[file][keyword] = tempID;
                isMaster || isGroup ? curGroup->newID() : curAnim->newID();
            }

            animLock->releaseExport();
            blocks[blok.front] = tempID;
        }
        else
        {
            blocks[blok.front] = (*IDExist)[import];
        }
    }
}

void proc::motionDataMultiGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[animMulti];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataFirstGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[0];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataNextGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        int nextorder;
        curAnim->isLast() ? nextorder = order : nextorder = order + 1;
        shared_ptr<AnimationInfo> curInfo                 = curAnim->GetGroupAnimInfo()[nextorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataBackGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        int previousorder;
        order == 0 ? previousorder = order : previousorder = order - 1;
        shared_ptr<AnimationInfo> curInfo                  = curAnim->GetGroupAnimInfo()[previousorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataLastGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[lastorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataNumGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[num];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataMultiMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][animMulti];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataFirstMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][0];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataLastMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][lastorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataNumMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][num];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::motionDataSingle(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1096, format, behaviorFile, numline);

    if (filename == blok.olddata[0]) ErrorMessage(1134, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[order];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, numline);
            motionData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            motionData = to_string(curInfo->motionData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->motionData.size(); ++j)
            {
                motionData.append(curInfo->motionData[j] + "\n");
            }
        }

        blocks[blok.front] = motionData;
    }
}

void proc::rotationDataMultiGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[animMulti];
        string rotationData;

        if (curInfo->rotationData.size())
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataFirstGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[0];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataNextGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        int nextorder;
        curAnim->isLast() ? nextorder = order : nextorder = order + 1;
        shared_ptr<AnimationInfo> curInfo                 = curAnim->GetGroupAnimInfo()[nextorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataBackGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        int previousorder;
        order == 0 ? previousorder = order : previousorder = order - 1;
        shared_ptr<AnimationInfo> curInfo                  = curAnim->GetGroupAnimInfo()[previousorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataLastGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[lastorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataNumGroup(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    size_t num = blok.olddataint[0];

    if (num >= curAnim->GetGroupAnimInfo().size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[num];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataMultiMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (animMulti == -1) ErrorMessage(1146, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][animMulti];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataFirstMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][0];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataLastMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][lastorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataNumMaster(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (groupMulti == -1) ErrorMessage(1202, format, behaviorFile, numline, blok.olddata[0]);

    size_t num = blok.olddataint[0];

    if (num >= curGroup->groupAnimInfo[groupMulti].size())
        ErrorMessage(1148, format, filename, numline, blok.olddata[0]);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curGroup->groupAnimInfo[groupMulti][num];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::rotationDataSingle(range blok, vecstr& blocks)
{
    if (fixedStateID.size() != 0 || eventid->size() != 0 || variableid->size() != 0)
        ErrorMessage(1097, format, behaviorFile, numline);

    if (filename == blok.olddata[0]) ErrorMessage(1134, format, behaviorFile, numline);

    if (clearBlocks(blok, blocks))
    {
        shared_ptr<AnimationInfo> curInfo = curAnim->GetGroupAnimInfo()[order];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, numline);
            rotationData
                = "1\n" + (curInfo->hasDuration ? strDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
        }
        else
        {
            rotationData = to_string(curInfo->rotationData.size()) + "\n";

            for (unsigned int j = 0; j < curInfo->rotationData.size(); ++j)
            {
                rotationData.append(curInfo->rotationData[j] + "\n");
            }
        }

        blocks[blok.front] = rotationData;
    }
}

void proc::animOrder(range blok, vecstr& blocks)
{
    string animPath = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

    if (clearBlocks(blok, blocks))
    {
        const auto& ptr
            = charAnimDataInfo.find(nemesis::to_lower_copy(project.substr(0, project.rfind(".txt"))));

        if (ptr != charAnimDataInfo.end())
        {
            const auto& ptr2 = ptr->second.find(
                nemesis::to_lower_copy(std::filesystem::path(animPath).filename().string()));

            if (ptr2 != ptr->second.end()) { blocks[blok.front] = to_string(ptr2->second->GetOrder()); }
        }
    }
}

void proc::regisAnim(range blok, vecstr& blocks)
{
    string animPath = combineBlocks(blok, blocks);
    nemesis::to_lower(animPath);
    addUsedAnim(behaviorFile, animPath);
}

void proc::regisBehavior(range blok, vecstr& blocks)
{
    string behaviorName = combineBlocks(blok, blocks);
    nemesis::to_lower(behaviorName);
    behaviorJoints[behaviorName].push_back(behaviorFile);
}

void proc::localNegative(range blok, vecstr& blocks)
{
    if (combineBlocks(blok, blocks)[0] == '-') *negative = true;
}

void proc::blocksCompile(vecstr blocks)
{
    for (auto& bracket : brackets[numline])
    {
        blocks[bracket] = "";
    }

    if (hasMC[numline])
    {
        bool done = false;

        for (auto& choice : multiChoice[numline])
        {
            if (done) { failed.push_back(choice_c(choice.locateA, choice.locateB)); }
            else
            {
                if (choice.condition->isTrue(
                        this, format, behaviorFile, numline, isGroup, isMaster, choice.condition))
                {
                    done     = true;
                    captured = make_shared<choice_c>(choice.locateA, choice.locateB);
                }
                else
                {
                    failed.push_back(choice_c(choice.locateA, choice.locateB));
                }
            }
        }
    }

    for (auto& bloklist : lineblocks[numline].blocksize)
    {
        for (auto& blok : bloklist.second)
        {
            (this->*blok.func)(blok, blocks);
        }
    }

    if (hasMC[numline])
    {
        for (auto& blok : choiceblocks[numline])
        {
            (this->*blok.func)(blok, blocks);
        }

        failed.clear();
        captured.reset();
    }

    line->clear();

    for (auto& each : blocks)
    {
        line->append(each);
    }
}

bool proc::isThisMaster()
{
    return groupMulti > -1;
}

bool proc::clearBlocks(range& blok, vecstr& blocks)
{
    if (failed.size() > 0)
    {
        for (auto& fail : failed)
        {
            // fail_front < front < back < fail_back
            if (fail.front <= blok.front && blok.back <= fail.back) return false;
        }
    }

    for (size_t i = blok.front; i <= blok.back; ++i)
    {
        blocks[i].clear();
    }

    return true;
}

void proc::blockCheck(size_t front, size_t back)
{
    for (auto& blocklist : lineblocks[numline].blocksize)
    {
        for (auto& blok : blocklist.second)
        {
            if (blok.front < front)
            {
                // b_front < front < b_back < back
                if (front < blok.back) ErrorMessage(1193, format, behaviorFile, numline);
            }
            // front < b_front < back < b_back
            else if (blok.front < back)
                ErrorMessage(1193, format, behaviorFile, numline);
        }
    }
}

string proc::combineBlocks(range& blok, vecstr& blocks)
{
    string option;

    for (unsigned int i = blok.front; i <= blok.back; ++i)
    {
        option.append(blocks[i]);
    }

    return option;
}

string proc::combineBlocks(size_t front, size_t back, vecstr& blocks)
{
    string option;

    for (unsigned int i = front; i <= back; ++i)
    {
        option.append(blocks[i]);
    }

    return option;
}

choice_c::choice_c(size_t posA, size_t posB)
{
    front = posA;
    back  = posB;
}
