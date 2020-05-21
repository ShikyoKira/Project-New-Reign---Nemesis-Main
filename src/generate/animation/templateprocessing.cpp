#include "utilities/algorithm.h"
#include "utilities/atomiclock.h"
#include "utilities/compute.h"
#include "utilities/conditions.h"

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
            == masterFunction->grouplist[groupMulti]->functionIDs.end())
        {
            ErrorMessage(1127, format, behaviorFile, numline);
        }

        state = masterFunction->grouplist[groupMulti]->functionIDs[stateID];
    }
    else
    {
        if (animMulti == -1) ErrorMessage(1057, format, behaviorFile, numline, original);

        if (masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format.find(stateID)
            == masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format.end())
        {
            ErrorMessage(1127, format, behaviorFile, numline);
        }

        state = masterFunction->grouplist[groupMulti]->singlelist[animMulti]->format[stateID];
    }
}

void proc::installBlock(nemesis::scope blok, int curline)
{
    blockCheck(blok.front, blok.back, curline);
    lineblocks[curline].blocksize[blok.size].push_back(blok);
}

void proc::installBlock(nemesis::scope blok, int curline, vector<nemesis::MultiChoice> m_condiiton)
{
    blockCheck(blok.front, blok.back, curline);
    hasMC.insert(curline);
    multiChoice[curline] = m_condiiton;
    choiceblocks[curline].push_back(blok);
}

void proc::relativeNegative(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.isEnd || curAnimInfo.negative)
        {
            blocks[blok.front] = "true";
        }
        else
        {
            blocks[blok.front] = "false";
        }

        curAnimInfo.negative = false;
        curAnimInfo.isEnd    = false;
    }
}

void proc::compute(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    curAnimInfo.elementCatch = true;
    curAnimInfo.norElement   = true;
}

void proc::rangeCompute(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    (*curAnimInfo.generatedlines)[curAnimInfo.elementLine]
        = nemesis::regex_replace(string((*curAnimInfo.generatedlines)[curAnimInfo.elementLine]),
                                 nemesis::regex("(.*<hkparam name\\=\".+\" numelements\\=\").+(\">.*)"),
                                 string("\\1" + to_string(curAnimInfo.counter) + "\\2"));
    curAnimInfo.norElement  = false;
    curAnimInfo.counter     = 0;
    curAnimInfo.elementLine = -1;
}

void proc::upCounter(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    ++curAnimInfo.counter;
}

void proc::upCounterPlus(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string full = combineBlocks(0, blocks.size() - 1, blocks);
    curAnimInfo.counter += count(full.begin(), full.end(), '#');
}

void proc::animCount(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo)) blocks[blok.front] = to_string(curAnimInfo.furnitureCount);
}

void proc::multiChoiceRegis(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string result = !curAnimInfo.captured
                        ? "null"
                        : combineBlocks(curAnimInfo.captured->front, curAnimInfo.captured->back, blocks);

    clearBlocks(blok, blocks, curAnimInfo);
    blocks[blok.front] = result;
}

void proc::groupIDRegis(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string oldID = masterformat + "_group$" + blok.olddata[0];
        Lockless lock(curAnimInfo.animLock->subIDLock);

        if (curAnimInfo.groupFunction->functionIDs.find(oldID)
            != curAnimInfo.groupFunction->functionIDs.end())
        {
            blocks[blok.front] = curAnimInfo.groupFunction->functionIDs[oldID];
        }
        else
        {
            string strID                                  = curAnimInfo.curAnim->newID();
            curAnimInfo.groupFunction->functionIDs[oldID] = strID;
            blocks[blok.front]                            = strID;
        }
    }
}

void proc::IDRegis(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string ID;
        string oldID = masterformat + "$" + blok.olddata[0];

        if (curAnimInfo.IDExist[oldID].length() > 0)
        {
            ID = curAnimInfo.IDExist[oldID];
        }
        else
        {
            ID                         = curAnimInfo.curAnim->newID();
            curAnimInfo.IDExist[oldID] = ID;
        }

        curAnimInfo.curAnim->subFunctionIDs->format[oldID] = ID;
        blocks[blok.front]                                 = ID;
    }
}

void proc::IDRegisAnim(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string ID;
        string oldID = masterformat + "$" + blok.olddata[0];
        int n_groupMulti;

        if (isMaster && curAnimInfo.groupMulti == -1)
        {
            ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[1]);
        }

        n_groupMulti = curAnimInfo.groupMulti;

        if (curAnimInfo.animMulti == -1)
        {
            ErrorMessage(1057, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
        }

        if (curAnimInfo.masterFunction->grouplist[n_groupMulti]
                ->singlelist[curAnimInfo.animMulti]
                ->format.find(oldID)
            != curAnimInfo.masterFunction->grouplist[n_groupMulti]
                   ->singlelist[curAnimInfo.animMulti]
                   ->format.end())
        {
            ID = curAnimInfo.masterFunction->grouplist[n_groupMulti]
                     ->singlelist[curAnimInfo.animMulti]
                     ->format[oldID];
            curAnimInfo.IDExist[oldID] = ID;
        }
        else
        {
            if (curAnimInfo.IDExist[oldID].length() > 0)
            {
                ID = curAnimInfo.IDExist[oldID];
            }
            else
            {
                ID                         = curAnimInfo.curGroup->newID();
                curAnimInfo.IDExist[oldID] = ID;
            }

            curAnimInfo.masterFunction->grouplist[n_groupMulti]
                ->singlelist[curAnimInfo.animMulti]
                ->format[oldID]
                = ID;
        }

        blocks[blok.front] = ID;
    }
}

void proc::IDRegisGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string ID;
        string oldID = masterformat + "_group$" + blok.olddata[0];
        int n_groupMulti;

        if (isMaster && curAnimInfo.groupMulti == -1)
        {
            ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[1]);
        }

        n_groupMulti = curAnimInfo.groupMulti;

        if (curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs.find(oldID)
            != curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs.end())
        {
            ID = curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs[oldID];
            curAnimInfo.IDExist[oldID] = ID;
        }
        else
        {
            if (curAnimInfo.IDExist[oldID].length() > 0)
            {
                ID = curAnimInfo.IDExist[oldID];
            }
            else
            {
                ID                         = curAnimInfo.curGroup->newID();
                curAnimInfo.IDExist[oldID] = ID;
            }

            curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs[oldID] = ID;
        }

        blocks[blok.front] = ID;
    }
}

void proc::IDRegisMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string ID;
        string oldID = masterformat + "_master$" + blok.olddata[0];

        if (curAnimInfo.masterFunction->functionIDs.find(oldID)
            != curAnimInfo.masterFunction->functionIDs.end())
        {
            ID                         = curAnimInfo.masterFunction->functionIDs[oldID];
            curAnimInfo.IDExist[oldID] = ID;
        }
        else
        {
            if (curAnimInfo.IDExist[oldID].length() > 0)
            {
                ID = curAnimInfo.IDExist[oldID];
            }
            else
            {
                ID                         = curAnimInfo.curGroup->newID();
                curAnimInfo.IDExist[oldID] = ID;
            }

            curAnimInfo.masterFunction->functionIDs[oldID] = ID;
        }

        blocks[blok.front] = ID;
    }
}

void proc::computation(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string equation       = combineBlocks(blok, blocks);
    size_t equationLength = equation.length();

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (equation.find("L") != NOT_FOUND)
        {
            if (isMaster)
            {
                ErrorMessage(1206, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
            }

            if (isGroup)
            {
                ErrorMessage(1206, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
            }

            int maths2 = count(equation.begin(), equation.end(), 'L');

            for (__int64 k = 0; k < maths2; ++k)
            {
                equation.replace(equation.find("L"), 1, to_string(curAnimInfo.lastorder));
            }
        }

        if (equation.find("N") != NOT_FOUND)
        {
            if (isMaster)
            {
                ErrorMessage(1206, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
            }

            if (isGroup)
            {
                ErrorMessage(1206, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
            }

            int maths2 = count(equation.begin(), equation.end(), 'N');
            string nextorder;
            nextorder = curAnimInfo.curAnim->isLast() ? to_string(curAnimInfo.order)
                                                      : to_string(curAnimInfo.order + 1);

            for (__int64 k = 0; k < maths2; ++k)
            {
                equation.replace(equation.find("N"), 1, nextorder);
            }
        }

        if (equation.find("B") != NOT_FOUND)
        {
            if (isMaster)
            {
                ErrorMessage(1206, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
            }
            if (isGroup)
            {
                ErrorMessage(1206, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
            }

            int maths2 = count(equation.begin(), equation.end(), 'B');
            string preorder;
            preorder
                = curAnimInfo.order == 0 ? to_string(curAnimInfo.order) : to_string(curAnimInfo.order - 1);

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

        nemesis::calculate(equation, format, behaviorFile, curAnimInfo.numline);
        blocks[blok.front] = equation;
    }
}

void proc::endMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.animMulti == -1)
        {
            ErrorMessage(1057, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
        }

        if (curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->hasDuration)
        {
            blocks[blok.front]
                = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curAnim->GetGroupAnimInfo()[0]->hasDuration)
        {
            blocks[blok.front] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[0]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int num = curAnimInfo.order;

        if (!curAnimInfo.curAnim->isLast()) num++;

        if (curAnimInfo.curAnim->GetGroupAnimInfo()[num]->hasDuration)
        {
            blocks[blok.front] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[num]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int num = curAnimInfo.order;

        if (curAnimInfo.order != 0) num--;

        if (curAnimInfo.curAnim->GetGroupAnimInfo()[num]->hasDuration)
        {
            blocks[blok.front] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[num]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.curAnim->GetGroupAnimInfo().size()]
                ->hasDuration)
        {
            blocks[blok.front]
                = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curAnim->GetGroupAnimInfo()[num]->hasDuration)
        {
            blocks[blok.front] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[num]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1057, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]->hasDuration)
        {
            blocks[blok.front] = to_string(
                curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]->hasDuration)
        {
            blocks[blok.front]
                = to_string(curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curGroup
                ->groupAnimInfo[curAnimInfo.groupMulti]
                               [curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size()]
                ->hasDuration)
        {
            blocks[blok.front] = to_string(
                curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]->hasDuration)
        {
            blocks[blok.front]
                = to_string(curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]->duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::endSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.hasDuration)
        {
            blocks[blok.front] = to_string(curAnimInfo.duration);
        }
        else
        {
            blocks[blok.front] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
    }
}

void proc::stateMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]]
                                       + ((curAnimInfo.animMulti - curAnimInfo.order)
                                          * curAnimInfo.stateCountMultiplier[blok.olddataint[0]])
                                       + blok.olddataint[1]);
}

void proc::stateFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front]
            = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]]
                        - (curAnimInfo.order * curAnimInfo.stateCountMultiplier[blok.olddataint[0]])
                        + blok.olddataint[1]);
}

void proc::stateNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        curAnimInfo.curAnim->isLast()
            ? blocks[blok.front]
              = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]] + blok.olddataint[1])
            : blocks[blok.front]
              = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]]
                          + curAnimInfo.stateCountMultiplier[blok.olddataint[0]] + blok.olddataint[1]);
    }
}

void proc::stateBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        curAnimInfo.order == 0
            ? blocks[blok.front]
              = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]] + blok.olddataint[1])
            : blocks[blok.front]
              = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]]
                          - curAnimInfo.stateCountMultiplier[blok.olddataint[0]] + blok.olddataint[1]);
    }
}

void proc::stateLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]]
                                       + ((curAnimInfo.lastorder - curAnimInfo.order)
                                          * curAnimInfo.stateCountMultiplier[blok.olddataint[0]])
                                       + blok.olddataint[1]);
    }
}

void proc::stateNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (blok.olddataint[2] >= int(curAnimInfo.curAnim->GetGroupAnimInfo().size()))
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = to_string(curAnimInfo.fixedStateID[num]
                        + ((blok.olddataint[2] - curAnimInfo.order) * curAnimInfo.stateCountMultiplier[num])
                        + blok.olddataint[1]);
    }
}

void proc::stateMultiMasterToGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   true,
                   curAnimInfo.groupMulti,
                   curAnimInfo.animMulti,
                   curAnimInfo.numline,
                   curAnimInfo.stateCount,
                   curAnimInfo.masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   curAnimInfo.groupMulti,
                   curAnimInfo.animMulti,
                   curAnimInfo.numline,
                   curAnimInfo.stateCount,
                   curAnimInfo.masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   curAnimInfo.groupMulti,
                   0,
                   curAnimInfo.numline,
                   curAnimInfo.stateCount,
                   curAnimInfo.masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   curAnimInfo.groupMulti,
                   curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]->singlelist.size() - 1,
                   curAnimInfo.numline,
                   curAnimInfo.stateCount,
                   curAnimInfo.masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[2];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string state;
        stateInput(state,
                   (blok.olddataint[0] == 0 ? "(S+" : "(S" + to_string(blok.olddataint[0] + 1) + "+")
                       + to_string(blok.olddataint[1]) + ")",
                   format,
                   behaviorFile,
                   blok.olddata[0],
                   false,
                   curAnimInfo.groupMulti,
                   num,
                   curAnimInfo.numline,
                   curAnimInfo.stateCount,
                   curAnimInfo.masterFunction);
        blocks[blok.front] = state;
    }
}

void proc::stateSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string state    = to_string(curAnimInfo.fixedStateID[blok.olddataint[0]] + blok.olddataint[1]);
        string original = "(S+" + to_string(blok.olddataint[1]) + ")";

        if (isMaster)
            curAnimInfo.masterFunction->functionIDs[original] = state;
        else if (isGroup)
            curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]->functionIDs[original] = state;

        blocks[blok.front] = state;
    }
}

void proc::filepathMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->filename;
    }
}

void proc::filepathFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[0]->filename;
    }
}

void proc::filepathNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        curAnimInfo.curAnim->isLast()
            ? blocks[blok.front] = curAnimInfo.filepath + curAnimInfo.filename
            : blocks[blok.front] = curAnimInfo.filepath
                                   + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->filename;
    }
}

void proc::filepathBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        curAnimInfo.order == 0
            ? blocks[blok.front] = curAnimInfo.filepath + curAnimInfo.filename
            : blocks[blok.front] = curAnimInfo.filepath
                                   + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->filename;
    }
}

void proc::filepathLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->filename;
    }
}

void proc::filepathNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[num]->filename;
}

void proc::filepathMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                 ->singlelist[curAnimInfo.animMulti]
                                 ->format["FilePath"];
}

void proc::filepathFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                 ->singlelist[0]
                                 ->format["FilePath"];
}

void proc::filepathLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                 ->singlelist.back()
                                 ->format["FilePath"];
}

void proc::filepathNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                 ->singlelist[blok.olddataint[0]]
                                 ->format["FilePath"];
}

void proc::filepathSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.filename == combineBlocks(blok, blocks))
    {
        ErrorMessage(1134, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.filepath + curAnimInfo.filename;
}

void proc::filenameMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile   = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile   = &curAnimInfo.curAnim->GetGroupAnimInfo()[0]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.curAnim->isLast())
        {
            blocks[blok.front] = curAnimInfo.filename.substr(0, curAnimInfo.filename.find_last_of("."));
        }
        else
        {
            string* tempfile   = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->filename;
            blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
        }
    }
}

void proc::filenameBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.order == 0)
        {
            blocks[blok.front] = curAnimInfo.filename.substr(0, curAnimInfo.filename.find_last_of("."));
        }
        else
        {
            string* tempfile   = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->filename;
            blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
        }
    }
}

void proc::filenameLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile   = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile   = &curAnimInfo.curAnim->GetGroupAnimInfo()[num]->filename;
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                ->singlelist[curAnimInfo.animMulti]
                                ->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                ->singlelist[0]
                                ->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                ->singlelist.back()
                                ->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                                ->singlelist[blok.olddataint[0]]
                                ->format["FileName"];
        blocks[blok.front] = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void proc::filenameSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.filename.substr(0, curAnimInfo.filename.find_last_of("."));
}

void proc::pathSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (isGroup)
            blocks[blok.front] = curAnimInfo.masterFunction->grouplist[0]->singlelist[0]->format["Path"];
        else if (isMaster)
            blocks[blok.front] = curAnimInfo.masterFunction->grouplist[0]->singlelist[0]->format["Path"];
        else
            blocks[blok.front] = curAnimInfo.filepath.substr(0, curAnimInfo.filepath.length() - 1);
    }
}

void proc::AOMultiGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1 || curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                                 ->AnimObject[blok.olddataint[0]][curAnimInfo.optionMulti];
    }
}

void proc::AOMultiGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                                 ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
    }
}

void proc::AOFirstGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[0]
                                 ->AnimObject[blok.olddataint[0]][curAnimInfo.optionMulti];
    }
}

void proc::AOFirstGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
    }
}

void proc::AONextGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr list;

        curAnimInfo.curAnim->isLast()
            ? list
              = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[blok.olddataint[0]]
            : list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]
                         ->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > curAnimInfo.optionMulti) blocks[blok.front] = list[curAnimInfo.optionMulti];
    }
}

void proc::AONextGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr list;
        curAnimInfo.curAnim->isLast()
            ? list
              = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[blok.olddataint[0]]
            : list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]
                         ->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > blok.olddataint[1])
        {
            blocks[blok.front] = list[blok.olddataint[1]];
        }
    }
}

void proc::AOBackGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr list;
        curAnimInfo.order == 0
            ? list
              = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[blok.olddataint[0]]
            : list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]
                         ->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > curAnimInfo.optionMulti) blocks[blok.front] = list[curAnimInfo.optionMulti];
    }
}

void proc::AOBackGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr list;
        curAnimInfo.order == 0
            ? list
              = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[blok.olddataint[0]]
            : list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]
                         ->AnimObject[blok.olddataint[0]];

        if (int(list.size()) > blok.olddataint[1]) blocks[blok.front] = list[blok.olddataint[1]];
    }
}

void proc::AOLastGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                                 ->AnimObject[blok.olddataint[0]][curAnimInfo.optionMulti];
    }
}

void proc::AOLastGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                                 ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
    }
}

void proc::AONumGroupA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[num]
                                 ->AnimObject[blok.olddataint[1]][curAnimInfo.optionMulti];
    }
}

void proc::AONumGroupB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[num]
                                 ->AnimObject[blok.olddataint[1]][blok.olddataint[2]];
    }
}

void proc::AOMultiMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1 || curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                  ->AnimObject[blok.olddataint[0]][curAnimInfo.optionMulti];
    }
}

void proc::AOMultiMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                  ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
    }
}

void proc::AOFirstMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                                 ->AnimObject[blok.olddataint[0]][curAnimInfo.optionMulti];
    }
}

void proc::AOFirstMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                                 ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
    }
}

void proc::AOLastMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                  ->AnimObject[blok.olddataint[0]][curAnimInfo.optionMulti];
    }
}

void proc::AOLastMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                  ->AnimObject[blok.olddataint[0]][blok.olddataint[1]];
    }
}

void proc::AONumMasterA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                                 ->AnimObject[blok.olddataint[1]][curAnimInfo.optionMulti];
    }
}

void proc::AONumMasterB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                                 ->AnimObject[blok.olddataint[1]][blok.olddataint[2]];
    }
}

void proc::AOSingleA(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, format, behaviorFile, curAnimInfo.numline, combineBlocks(blok, blocks));
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        auto animobj = curAnimInfo.AnimObject.find(blok.olddataint[0]);

        if (animobj != curAnimInfo.AnimObject.end())
        {
            blocks[blok.front] = animobj->second[curAnimInfo.optionMulti];
        }
    }
}

void proc::AOSingleB(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        auto animobj = curAnimInfo.AnimObject.find(blok.olddataint[0]);

        if (animobj != curAnimInfo.AnimObject.end())
        {
            blocks[blok.front] = animobj->second[blok.olddataint[1]];
        }
    }
}

void proc::MAEMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);

    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->mainAnimEvent;
}

void proc::MAEFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->mainAnimEvent;
}

void proc::MAENextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        curAnimInfo.curAnim->isLast()
            ? blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->mainAnimEvent
            : blocks[blok.front]
              = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->mainAnimEvent;
    }
}

void proc::MAEBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        curAnimInfo.order == 0
            ? blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->mainAnimEvent
            : blocks[blok.front]
              = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->mainAnimEvent;
    }
}

void proc::MAELastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->mainAnimEvent;
    }
}

void proc::MAENumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[num]->mainAnimEvent;
    }
}

void proc::MAEMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                  ->mainAnimEvent;
    }
}

void proc::MAEFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]->mainAnimEvent;
    }
}

void proc::MAELastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front]
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                  ->mainAnimEvent;
    }
}

void proc::MAENumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        blocks[blok.front] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]->mainAnimEvent;
    }
}

void proc::MAESingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo)) blocks[blok.front] = curAnimInfo.mainAnimEvent;
}

void proc::addOnMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list
            = &curAnimInfo.curAnim->GetGroupAnimInfo()[0]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output
                = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int curorder = curAnimInfo.order;

        if (!curAnimInfo.curAnim->isLast()) ++curorder;

        VecStr* list = &curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int curorder = curAnimInfo.order;

        if (curAnimInfo.order > 0) --curorder;

        VecStr* list = &curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            if (curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                    ->addition[blok.olddata[0]][blok.olddata[1]]
                    .length()
                == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                                     ->addition[blok.olddata[0]][blok.olddata[1]];
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list
            = &curAnimInfo.curAnim->GetGroupAnimInfo()[num]->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output
                = curAnimInfo.curAnim->GetGroupAnimInfo()[num]->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[2]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[2]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[2]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                            ->groupAddition[blok.olddata[0]][blok.olddata[1]];

        if (list->size() == 0 || curAnimInfo.optionMulti == -1)
        {
            string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                                ->addition[blok.olddata[0]][blok.olddata[1]];

            if (output.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = output;
        }
        else if (int(list->size()) > curAnimInfo.optionMulti)
        {
            if ((*list)[curAnimInfo.optionMulti].length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[blok.front] = (*list)[curAnimInfo.optionMulti];
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void proc::addOnSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        auto grpadd = curAnimInfo.groupAddition.find(blok.olddata[0]);

        if (curAnimInfo.optionMulti == -1 || grpadd == curAnimInfo.groupAddition.end()
            || grpadd->second.find(blok.olddata[1]) == grpadd->second.end()
            || grpadd->second.at(blok.olddata[1]).size() == 0)
        {
            string_view sv;

            try
            {
                sv = curAnimInfo.addition.at(blok.olddata[0]).at(blok.olddata[1]);
            }
            catch (const std::exception&)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            if (sv.length() == 0)
            {
                ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[2]);
            }

            blocks[blok.front] = sv.data();
        }
        else
        {
            const VecStr& addRef = grpadd->second.at(blok.olddata[1]);

            if (static_cast<int>(addRef.size()) > curAnimInfo.optionMulti)
            {
                string_view sv = addRef[curAnimInfo.optionMulti];

                if (sv.length() == 0)
                {
                    ErrorMessage(1117, format, curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
                }

                blocks[blok.front] = sv.data();
            }
            else
            {
                ErrorMessage(1141);
            }
        }
    }
}

void proc::lastState(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        size_t ID = 0;

        if (curAnimInfo.fixedStateID.size() > 1)
        {
            if (blok.olddata[0].length() > 0)
            {
                ID = stoi(blok.olddata[0]) - 1;

                if (ID >= curAnimInfo.fixedStateID.size())
                {
                    ErrorMessage(
                        1168, format, behaviorFile, curAnimInfo.numline, "LastState" + blok.olddata[0]);
                }
            }
        }

        blocks[blok.front]
            = to_string(curAnimInfo.lastorder - curAnimInfo.order + curAnimInfo.fixedStateID[ID]);
    }
}

void proc::eventID(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string eventname = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        auto eventitr = curAnimInfo.eventid.find(eventname);

        if (eventitr == curAnimInfo.eventid.end() && eventname != curAnimInfo.zeroEvent)
        {
            if (format == "BASE") ErrorMessage(1165, eventname);

            ErrorMessage(1131, format, curAnimInfo.filename, curAnimInfo.numline, eventname);
        }

        blocks[blok.front] = to_string(eventitr->second);
    }
}

void proc::variableID(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string variablename = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        auto variableitr = curAnimInfo.variableid.find(variablename);

        if (variableitr == curAnimInfo.variableid.end() && variablename != curAnimInfo.zeroVariable)
        {
            if (format == "BASE") ErrorMessage(1166, variablename);

            ErrorMessage(1132, format, curAnimInfo.filename, curAnimInfo.numline, variablename);
        }

        blocks[blok.front] = to_string(variableitr->second);
    }
}

void proc::crc32(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string crc32line = nemesis::to_lower_copy(combineBlocks(blok, blocks));

    if (clearBlocks(blok, blocks, curAnimInfo)) blocks[blok.front] = to_string(CRC32Convert(crc32line));
}

void proc::import(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string import = combineBlocks(blok, blocks);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        if (curAnimInfo.IDExist[import].length() == 0)
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
                    {
                        ++openBrack;
                    }
                    else if (curChar == ']')
                    {
                        --openBrack;

                        if (openBrack == 0) keyword.append("!~^!");
                    }
                    else
                    {
                        keyword.push_back(curChar);
                    }
                }

                pos = keyword.rfind("!~^!");

                openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4
                    ? ErrorMessage(1139, format, behaviorFile, curAnimInfo.numline, import)
                    : keyword = keyword.substr(0, keyword.length() - 4);
            }

            Lockless lock(curAnimInfo.animLock->exportLock);

            try
            {
                tempID = curAnimInfo.newImport->at(file).at(keyword);
            }
            catch (const std::exception&)
            {
            }

            if (tempID.length() == 0)
            {
                tempID = (isMaster || isGroup ? curAnimInfo.curGroup->newID() : curAnimInfo.curAnim->newID());
                curAnimInfo.IDExist[import]             = tempID;
                (*curAnimInfo.newImport)[file][keyword] = tempID;
            }

            blocks[blok.front] = tempID;
        }
        else
        {
            blocks[blok.front] = curAnimInfo.IDExist[import];
        }
    }
}

void proc::motionDataMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[0];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int nextorder;
        curAnimInfo.curAnim->isLast() ? nextorder = curAnimInfo.order : nextorder = curAnimInfo.order + 1;
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[nextorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int previousorder;
        curAnimInfo.order == 0 ? previousorder = curAnimInfo.order : previousorder = curAnimInfo.order - 1;
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[previousorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[num];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::motionDataSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1096, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.filename == blok.olddata[0])
    {
        ErrorMessage(1134, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order];
        string motionData;

        if (curInfo->motionData.size() == 0)
        {
            WarningMessage(1018, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataMultiGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti];
        string rotationData;

        if (curInfo->rotationData.size())
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataFirstGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[0];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataNextGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int nextorder;
        curAnimInfo.curAnim->isLast() ? nextorder = curAnimInfo.order : nextorder = curAnimInfo.order + 1;
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[nextorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataBackGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        int previousorder;
        curAnimInfo.order == 0 ? previousorder = curAnimInfo.order : previousorder = curAnimInfo.order - 1;
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[previousorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataLastGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataNumGroup(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[num];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataMultiMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataFirstMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataLastMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo
            = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataNumMaster(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, format, behaviorFile, curAnimInfo.numline, blok.olddata[0]);
    }

    size_t num = blok.olddataint[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, format, curAnimInfo.filename, curAnimInfo.numline, blok.olddata[0]);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::rotationDataSingle(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.fixedStateID.size() != 0 || curAnimInfo.eventid.size() != 0
        || curAnimInfo.variableid.size() != 0)
    {
        ErrorMessage(1097, format, behaviorFile, curAnimInfo.numline);
    }

    if (curAnimInfo.filename == blok.olddata[0])
    {
        ErrorMessage(1134, format, behaviorFile, curAnimInfo.numline);
    }

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order];
        string rotationData;

        if (curInfo->rotationData.size() == 0)
        {
            WarningMessage(1019, format, behaviorFile, curAnimInfo.numline);
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

void proc::animOrder(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string animPath = combineBlocks(blok.olddataint[0], blok.olddataint[1], blocks);

    if (clearBlocks(blok, blocks, curAnimInfo))
    {
        auto& ptr = charAnimDataInfo.find(
            nemesis::to_lower_copy(curAnimInfo.project.substr(0, curAnimInfo.project.rfind(".txt"))));

        if (ptr != charAnimDataInfo.end())
        {
            auto& ptr2 = ptr->second.find(
                nemesis::to_lower_copy(std::filesystem::path(animPath).filename().string()));

            if (ptr2 != ptr->second.end())
            {
                blocks[blok.front] = to_string(ptr2->second->GetOrder());
            }
        }
    }
}

void proc::regisAnim(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string animPath = combineBlocks(blok, blocks);
    addUsedAnim(behaviorFile, animPath);
}

void proc::regisBehavior(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    string behaviorName = combineBlocks(blok, blocks);
    nemesis::to_lower(behaviorName);
    behaviorJoints[behaviorName].push_back(nemesis::to_lower_copy(behaviorFile));
}

void proc::localNegative(nemesis::scope blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (combineBlocks(blok, blocks)[0] == '-') curAnimInfo.negative = true;
}

void proc::blocksCompile(VecStr blocks, AnimThreadInfo& curAnimInfo) const
{
    auto bkitr = brackets.find(curAnimInfo.numline);

    if (bkitr != brackets.end())
    {
        for (auto& bracket : bkitr->second)
        {
            blocks[bracket] = "";
        }
    }

    auto mcitr = hasMC.find(curAnimInfo.numline);

    if (mcitr != hasMC.end())
    {
        bool done = false;

        for (auto& choice : multiChoice.at(curAnimInfo.numline))
        {
            if (done)
            {
                curAnimInfo.failed.push_back(choice_c(choice.locateA, choice.locateB));
            }
            else
            {
                if (choice.condition->isTrue(
                        curAnimInfo, *this, format, behaviorFile, curAnimInfo.numline, isGroup, isMaster))
                {
                    done                 = true;
                    curAnimInfo.captured = make_shared<choice_c>(choice.locateA, choice.locateB);
                }
                else
                {
                    curAnimInfo.failed.push_back(choice_c(choice.locateA, choice.locateB));
                }
            }
        }
    }

    auto lbitr = lineblocks.find(curAnimInfo.numline);

    if (lbitr != lineblocks.end())
    {
        for (auto& bloklist : lbitr->second.blocksize)
        {
            for (auto& blok : bloklist.second)
            {
                (this->*blok.func)(blok, blocks, curAnimInfo);
            }
        }
    }

    if (mcitr != hasMC.end())
    {
        for (auto& blok : choiceblocks.at(curAnimInfo.numline))
        {
            (this->*blok.func)(blok, blocks, curAnimInfo);
        }

        curAnimInfo.failed.clear();
        curAnimInfo.captured.reset();
    }

    curAnimInfo.line->clear();

    for (auto& each : blocks)
    {
        curAnimInfo.line->append(each);
    }
}

bool proc::isThisMaster()
{
    return isMaster;
}

bool proc::clearBlocks(nemesis::scope& blok, VecStr& blocks, AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.failed.size() > 0)
    {
        for (auto& fail : curAnimInfo.failed)
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

void proc::blockCheck(size_t front, size_t back, int numline)
{
    for (auto& blocklist : lineblocks[numline].blocksize)
    {
        for (auto& blok : blocklist.second)
        {
            if (blok.front < front)
            {
                // b_front < front < b_back < back
                if (front < blok.back && blok.back < back) ErrorMessage(1193, format, behaviorFile, numline);
            }
            // front < b_front < back < b_back
            else if (blok.front < back && back < blok.back)
            {
                ErrorMessage(1193, format, behaviorFile, numline);
            }
        }
    }
}

string proc::combineBlocks(nemesis::scope& blok, VecStr& blocks) const
{
    string option;

    for (unsigned int i = blok.front; i <= blok.back; ++i)
    {
        option.append(blocks[i]);
    }

    return option;
}

string proc::combineBlocks(size_t front, size_t back, VecStr& blocks) const
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
