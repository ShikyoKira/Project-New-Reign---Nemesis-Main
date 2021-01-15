#include "utilities/process.h"
#include "utilities/atomiclock.h"

#include "generate/alternateanimation.h"
#include "generate/animation/animthreadinfo.h"
#include "generate/animation/grouptemplate.h"
#include "generate/animation/templatetree.h"

using namespace std;

string StrDoubleDecimal(double num, int decimal)
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

    stringstream ss;
    ss << fixed << setprecision(decimal);
    ss << static_cast<int>(num);
    return ss.str();
}

void stateInput(const string& state,
                const string& stateID,
                const string& format,
                const string& behaviorfile,
                const string& original,
                bool isMaster,
                int groupMulti,
                int animMulti,
                int numline,
                const size_t& stateCount,
                const shared_ptr<master>& masterFunction)
{
    if (groupMulti == -1) ErrorMessage(1202, format, behaviorfile, numline, original);

    //if (isMaster)
    //{
    //    if (masterFunction->grouplist[groupMulti]->functionIDs.find(stateID)
    //        == masterFunction->grouplist[groupMulti]->functionIDs.end())
    //    {
    //        ErrorMessage(1127, GetFormat(), GetBehaviorFile(), numline);
    //    }

    //    state = masterFunction->grouplist[groupMulti]->functionIDs[stateID];
    //}
    //else
    //{
    //    if (animMulti == -1) ErrorMessage(1057, GetFormat(), GetBehaviorFile(), numline, original);

    //    if (masterFunction->grouplist[groupMulti]->singlelist[animMulti]->GetFormat().find(stateID)
    //        == masterFunction->grouplist[groupMulti]->singlelist[animMulti]->GetFormat().end())
    //    {
    //        ErrorMessage(1127, GetFormat(), GetBehaviorFile(), numline);
    //    }

    //    state = masterFunction->grouplist[groupMulti]->singlelist[animMulti]->GetFormat()[stateID];
    //}
}

bool nemesis::Process::ClearBlocks(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!curAnimInfo.failed.empty())
    {
        for (auto& fail : curAnimInfo.failed)
        {
            // fail_front < front < back < fail_back
            if (fail.front <= begin && end <= fail.back) return false;
        }
    }

    for (uint i = begin; i <= end; ++i)
    {
        blocks[i].clear();
    }

    return true;
}

string nemesis::Process::CombineBlocks(VecStr& blocks) const
{
    return CombineBlocks(begin, end, blocks);
}

string nemesis::Process::CombineBlocks(uint ibegin, uint iend, VecStr& blocks) const
{
    string option;

    for (uint i = ibegin; i <= iend; ++i)
    {
        option.append(blocks[i]);
    }

    return option;
}

void nemesis::Process::MotionValidation(const AnimThreadInfo& curAnimInfo) const
{
    if (!curAnimInfo.fixedStateID.empty() || !curAnimInfo.eventid.empty() || !curAnimInfo.variableid.empty())
    {
        ErrorMessage(1096, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }
}

void nemesis::Process::RotationValidation(const AnimThreadInfo& curAnimInfo) const
{
    if (!curAnimInfo.fixedStateID.empty() || !curAnimInfo.eventid.empty() || !curAnimInfo.variableid.empty())
    {
        ErrorMessage(1097, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }
}

//void nemesis::Process::blockCheck(int numline)
//{
//    if (front < front)
//    {
//        // b_front < front < b_back < back
//        if (front < back && back < back) ErrorMessage(1193, GetFormat(), GetBehaviorFile(), numline);
//    }
//    // front < b_front < back < b_back
//    else if (front < back && back < back)
//    {
//        ErrorMessage(1193, GetFormat(), GetBehaviorFile(), numline);
//    }
//}

nemesis::Process::~Process() = default;

void nemesis::Process::AddFunction(funcptr pFunction)
{
    this->pFunction = pFunction;
}

void nemesis::Process::AddTemplateLinePtr(std::shared_ptr<nemesis::TemplateLine> pTemplateLine)
{
    this->pTemplateLine = pTemplateLine;
}

void nemesis::Process::SetBegin(uint begin)
{
    this->begin = begin;
}

void nemesis::Process::SetEnd(uint end)
{
    this->end = end;
}

void nemesis::Process::SetFixedVar(const VecStr& fixedvarlist)
{
    this->fixedvarlist = fixedvarlist;
}

void nemesis::Process::SetFixedVarInt(const std::vector<int>& fixedvarintlist)
{
    this->fixedvarintlist = fixedvarintlist;
}

const uint& nemesis::Process::GetSize() const noexcept
{
    return end - begin;
}

const uint& nemesis::Process::GetBegin() const noexcept
{
    return begin;
}

const uint& nemesis::Process::GetEnd() const noexcept
{
    return end;
}

const string& nemesis::Process::GetFormat() const
{
    return pTemplateLine.lock()->GetTemplate()->GetTemplateClassName();
}

const std::filesystem::path& nemesis::Process::GetBehaviorFile() const
{
    return pTemplateLine.lock()->GetTemplate()->GetFilePath();
}

//void nemesis::Process::installBlock(int curline)
//{
//    blockCheck(begin, end, curline);
//    lineblocks[curline].blocksize[getSize()].push_back(blok);
//}
//
//void nemesis::Process::installBlock(int curline, vector<nemesis::MultiChoice> m_condiiton)
//{
//    blockCheck(begin, end, curline);
//    hasMC.insert(curline);
//    multiChoice[curline] = m_condiiton;
//    choiceblocks[curline].push_back(blok);
//}

void nemesis::Process::RelativeNegative(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]        = curAnimInfo.isEnd || curAnimInfo.negative ? "true" : "false";
    curAnimInfo.negative = false;
    curAnimInfo.isEnd    = false;
}

void nemesis::Process::Compute(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    curAnimInfo.elementCatch = true;
    curAnimInfo.norElement   = true;
}

void nemesis::Process::RangeCompute(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    (*curAnimInfo.generatedlines)[curAnimInfo.elementLine]
        = nemesis::regex_replace(string((*curAnimInfo.generatedlines)[curAnimInfo.elementLine]),
                                 nemesis::regex(R"((.*<hkparam name\=".+" numelements\=").+(">.*))"),
                                 string("\\1" + to_string(curAnimInfo.counter) + "\\2"));
    curAnimInfo.norElement  = false;
    curAnimInfo.counter     = 0;
    curAnimInfo.elementLine = -1;
}

void nemesis::Process::UpCounter(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    ++curAnimInfo.counter;
}

void nemesis::Process::UpCounterPlus(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string full = CombineBlocks(0, blocks.size() - 1, blocks);
    curAnimInfo.counter += count(full.begin(), full.end(), '#');
}

void nemesis::Process::AnimCount(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (ClearBlocks(blocks, curAnimInfo)) blocks[begin] = to_string(curAnimInfo.furnitureCount);
}

void nemesis::Process::MultiChoiceRegis(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string result = !curAnimInfo.captured
                        ? "null"
                        : CombineBlocks(curAnimInfo.captured->front, curAnimInfo.captured->back, blocks);

    ClearBlocks(blocks, curAnimInfo);
    blocks[begin] = result;
}

void nemesis::Process::GroupIDRegis(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //string oldID = masterformat + "_group$" + fixedvarlist[0];
    //Lockless lock(curAnimInfo.animLock->subIDLock);

    //if (curAnimInfo.groupFunction->functionIDs.find(oldID) != curAnimInfo.groupFunction->functionIDs.end())
    //{
    //    blocks[begin] = curAnimInfo.groupFunction->functionIDs[oldID];
    //}
    //else
    //{
    //    string strID                                  = curAnimInfo.curAnim->newID();
    //    curAnimInfo.groupFunction->functionIDs[oldID] = strID;
    //    blocks[begin]                                 = strID;
    //}
}

void nemesis::Process::IDRegis(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //string ID;
    //string oldID = masterformat + "$" + fixedvarlist[0];

    //if (curAnimInfo.IDExist[oldID].length() > 0)
    //{
    //    ID = curAnimInfo.IDExist[oldID];
    //}
    //else
    //{
    //    ID                         = curAnimInfo.curAnim->newID();
    //    curAnimInfo.IDExist[oldID] = ID;
    //}

    //curAnimInfo.curAnim->subFunctionIDs->GetFormat()[oldID] = ID;
    //blocks[begin]                                      = ID;
}

void nemesis::Process::IDRegisAnim(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //string ID;
    //string oldID = masterformat + "$" + fixedvarlist[0];
    //int n_groupMulti;

    //if (isMaster && curAnimInfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[1]);
    //}

    //n_groupMulti = curAnimInfo.groupMulti;

    //if (curAnimInfo.animMulti == -1)
    //{
    //    ErrorMessage(1057, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //}

    //if (curAnimInfo.masterFunction->grouplist[n_groupMulti]->singlelist[curAnimInfo.animMulti]->format.find(
    //        oldID)
    //    != curAnimInfo.masterFunction->grouplist[n_groupMulti]
    //           ->singlelist[curAnimInfo.animMulti]
    //           ->format.end())
    //{
    //    ID = curAnimInfo.masterFunction->grouplist[n_groupMulti]
    //             ->singlelist[curAnimInfo.animMulti]
    //             ->format[oldID];
    //    curAnimInfo.IDExist[oldID] = ID;
    //}
    //else
    //{
    //    if (curAnimInfo.IDExist[oldID].length() > 0)
    //    {
    //        ID = curAnimInfo.IDExist[oldID];
    //    }
    //    else
    //    {
    //        ID                         = curAnimInfo.curGroup->newID();
    //        curAnimInfo.IDExist[oldID] = ID;
    //    }

    //    curAnimInfo.masterFunction->grouplist[n_groupMulti]->singlelist[curAnimInfo.animMulti]->format[oldID]
    //        = ID;
    //}

    //blocks[begin] = ID;
}

void nemesis::Process::IDRegisGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //string ID;
    //string oldID = masterformat + "_group$" + fixedvarlist[0];
    //int n_groupMulti;

    //if (isMaster && curAnimInfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[1]);
    //}

    //n_groupMulti = curAnimInfo.groupMulti;

    //if (curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs.find(oldID)
    //    != curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs.end())
    //{
    //    ID                         = curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs[oldID];
    //    curAnimInfo.IDExist[oldID] = ID;
    //}
    //else
    //{
    //    if (curAnimInfo.IDExist[oldID].length() > 0)
    //    {
    //        ID = curAnimInfo.IDExist[oldID];
    //    }
    //    else
    //    {
    //        ID                         = curAnimInfo.curGroup->newID();
    //        curAnimInfo.IDExist[oldID] = ID;
    //    }

    //    curAnimInfo.masterFunction->grouplist[n_groupMulti]->functionIDs[oldID] = ID;
    //}

    //blocks[begin] = ID;
}

void nemesis::Process::IDRegisMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //string ID;
    //string oldID = masterformat + "_master$" + fixedvarlist[0];

    //if (curAnimInfo.masterFunction->functionIDs.find(oldID) != curAnimInfo.masterFunction->functionIDs.end())
    //{
    //    ID                         = curAnimInfo.masterFunction->functionIDs[oldID];
    //    curAnimInfo.IDExist[oldID] = ID;
    //}
    //else
    //{
    //    if (curAnimInfo.IDExist[oldID].length() > 0)
    //    {
    //        ID = curAnimInfo.IDExist[oldID];
    //    }
    //    else
    //    {
    //        ID                         = curAnimInfo.curGroup->newID();
    //        curAnimInfo.IDExist[oldID] = ID;
    //    }

    //    curAnimInfo.masterFunction->functionIDs[oldID] = ID;
    //}

    //blocks[begin] = ID;
}

void nemesis::Process::Computation(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string equation       = CombineBlocks(blocks);
    size_t equationLength = equation.length();

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //if (equation.find("L") != NOT_FOUND)
    //{
    //    if (isMaster)
    //    {
    //        ErrorMessage(1206, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //    }

    //    if (isGroup)
    //    {
    //        ErrorMessage(1206, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //    }

    //    int maths2 = count(equation.begin(), equation.end(), 'L');

    //    for (__int64 k = 0; k < maths2; ++k)
    //    {
    //        equation.replace(equation.find("L"), 1, to_string(curAnimInfo.lastorder));
    //    }
    //}

    //if (equation.find("N") != NOT_FOUND)
    //{
    //    if (isMaster)
    //    {
    //        ErrorMessage(1206, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //    }

    //    if (isGroup)
    //    {
    //        ErrorMessage(1206, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //    }

    //    int maths2 = count(equation.begin(), equation.end(), 'N');
    //    string nextorder;
    //    nextorder
    //        = curAnimInfo.curAnim->isLast() ? to_string(curAnimInfo.order) : to_string(curAnimInfo.order + 1);

    //    for (__int64 k = 0; k < maths2; ++k)
    //    {
    //        equation.replace(equation.find("N"), 1, nextorder);
    //    }
    //}

    //if (equation.find("B") != NOT_FOUND)
    //{
    //    if (isMaster)
    //    {
    //        ErrorMessage(1206, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //    }
    //    if (isGroup)
    //    {
    //        ErrorMessage(1206, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    //    }

    //    int maths2 = count(equation.begin(), equation.end(), 'B');
    //    string preorder;
    //    preorder = curAnimInfo.order == 0 ? to_string(curAnimInfo.order) : to_string(curAnimInfo.order - 1);

    //    for (__int64 k = 0; k < maths2; ++k)
    //    {
    //        equation.replace(equation.find("B"), 1, preorder);
    //    }
    //}

    //if (equation.find("F") != NOT_FOUND)
    //{
    //    int maths2 = count(equation.begin(), equation.end(), 'F');

    //    for (__int64 k = 0; k < maths2; ++k)
    //    {
    //        equation.replace(equation.find("F"), 1, "0");
    //    }
    //}

    //nemesis::calculate(equation, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    //blocks[begin] = equation;
}

void nemesis::Process::EndMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1057, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->hasDuration)
    {
        blocks[begin]
            = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->duration);
    }
    else
    {
        blocks[begin] = "0.000000";
        curAnimInfo.isEnd  = true;
    }
}

void nemesis::Process::EndFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curAnim->GetGroupAnimInfo()[0]->hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[0]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int num = curAnimInfo.order;

    if (!curAnimInfo.curAnim->isLast()) num++;

    if (curAnimInfo.curAnim->GetGroupAnimInfo()[num]->hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[num]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int num = curAnimInfo.order;

    if (curAnimInfo.order != 0) num--;

    if (curAnimInfo.curAnim->GetGroupAnimInfo()[num]->hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[num]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.curAnim->GetGroupAnimInfo().size()]->hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

        if (curAnimInfo.curAnim->GetGroupAnimInfo()[num]->hasDuration)
        {
            blocks[begin] = to_string(curAnimInfo.curAnim->GetGroupAnimInfo()[num]->duration);
        }
        else
        {
            blocks[begin] = "0.000000";
            curAnimInfo.isEnd  = true;
        }
}

void nemesis::Process::EndMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1057, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]->hasDuration)
    {
        blocks[begin] = to_string(
            curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]->hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curGroup
            ->groupAnimInfo[curAnimInfo.groupMulti]
                           [curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size()]
            ->hasDuration)
    {
        blocks[begin] = to_string(
            curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]->hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]->duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::EndSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.hasDuration)
    {
        blocks[begin] = to_string(curAnimInfo.duration);
    }
    else
    {
        blocks[begin]     = "0.000000";
        curAnimInfo.isEnd = true;
    }
}

void nemesis::Process::StateMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = to_string(
        curAnimInfo.fixedStateID[fixedvarintlist[0]]
        + ((curAnimInfo.animMulti - curAnimInfo.order) * curAnimInfo.stateCountMultiplier[fixedvarintlist[0]])
        + fixedvarintlist[1]);
}

void nemesis::Process::StateFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = to_string(curAnimInfo.fixedStateID[fixedvarintlist[0]]
                              - (curAnimInfo.order * curAnimInfo.stateCountMultiplier[fixedvarintlist[0]])
                              + fixedvarintlist[1]);
}

void nemesis::Process::StateNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    curAnimInfo.curAnim->isLast()
        ? blocks[begin] = to_string(curAnimInfo.fixedStateID[fixedvarintlist[0]] + fixedvarintlist[1])
        : blocks[begin]
          = to_string(curAnimInfo.fixedStateID[fixedvarintlist[0]]
                      + curAnimInfo.stateCountMultiplier[fixedvarintlist[0]] + fixedvarintlist[1]);
}

void nemesis::Process::StateBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    curAnimInfo.order == 0
        ? blocks[begin] = to_string(curAnimInfo.fixedStateID[fixedvarintlist[0]] + fixedvarintlist[1])
        : blocks[begin]
          = to_string(curAnimInfo.fixedStateID[fixedvarintlist[0]]
                      - curAnimInfo.stateCountMultiplier[fixedvarintlist[0]] + fixedvarintlist[1]);
}

void nemesis::Process::StateLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = to_string(
        curAnimInfo.fixedStateID[fixedvarintlist[0]]
        + ((curAnimInfo.lastorder - curAnimInfo.order) * curAnimInfo.stateCountMultiplier[fixedvarintlist[0]])
        + fixedvarintlist[1]);
}

void nemesis::Process::StateNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (fixedvarintlist[2] >= int(curAnimInfo.curAnim->GetGroupAnimInfo().size()))
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = to_string(curAnimInfo.fixedStateID[num]
                    + ((fixedvarintlist[2] - curAnimInfo.order) * curAnimInfo.stateCountMultiplier[num])
                    + fixedvarintlist[1]);
}

void nemesis::Process::StateMultiMasterToGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedvarlist[0],
               true,
               curAnimInfo.groupMulti,
               curAnimInfo.animMulti,
               curAnimInfo.numline,
               curAnimInfo.stateCount,
               curAnimInfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedvarlist[0],
               false,
               curAnimInfo.groupMulti,
               curAnimInfo.animMulti,
               curAnimInfo.numline,
               curAnimInfo.stateCount,
               curAnimInfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedvarlist[0],
               false,
               curAnimInfo.groupMulti,
               0,
               curAnimInfo.numline,
               curAnimInfo.stateCount,
               curAnimInfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedvarlist[0],
               false,
               curAnimInfo.groupMulti,
               curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]->singlelist.size() - 1,
               curAnimInfo.numline,
               curAnimInfo.stateCount,
               curAnimInfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[2];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedvarlist[0],
               false,
               curAnimInfo.groupMulti,
               num,
               curAnimInfo.numline,
               curAnimInfo.stateCount,
               curAnimInfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string state    = to_string(curAnimInfo.fixedStateID[fixedvarintlist[0]] + fixedvarintlist[1]);
    string original = "(S+" + to_string(fixedvarintlist[1]) + ")";

    //if (isMaster)
    //{
    //    curAnimInfo.masterFunction->functionIDs[original] = state;
    //}
    //else if (isGroup)
    //{
    //    curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]->functionIDs[original] = state;
    //}

    blocks[begin] = state;
}

void nemesis::Process::FilepathMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->filename;
}

void nemesis::Process::FilepathFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[0]->filename;
}

void nemesis::Process::FilepathNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    curAnimInfo.curAnim->isLast()
        ? blocks[begin] = curAnimInfo.filepath + curAnimInfo.filename
        : blocks[begin]
          = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->filename;
}

void nemesis::Process::FilepathBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    curAnimInfo.order == 0
        ? blocks[begin] = curAnimInfo.filepath + curAnimInfo.filename
        : blocks[begin]
          = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->filename;
}

void nemesis::Process::FilepathLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->filename;
}

void nemesis::Process::FilepathNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.filepath + curAnimInfo.curAnim->GetGroupAnimInfo()[num]->filename;
}

void nemesis::Process::FilepathMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                        ->singlelist[curAnimInfo.animMulti]
                        ->format["FilePath"];
}

void nemesis::Process::FilepathFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                        ->singlelist[0]
                        ->format["FilePath"];
}

void nemesis::Process::FilepathLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                        ->singlelist.back()
                        ->format["FilePath"];
}

void nemesis::Process::FilepathNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                        ->singlelist[fixedvarintlist[0]]
                        ->format["FilePath"];
}

void nemesis::Process::FilepathSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.filename == CombineBlocks(blocks))
    {
        ErrorMessage(1134, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.filepath + curAnimInfo.filename;
}

void nemesis::Process::FilenameMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->filename;
    blocks[begin]    = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.curAnim->GetGroupAnimInfo()[0]->filename;
    blocks[begin]    = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.curAnim->isLast())
    {
        blocks[begin] = curAnimInfo.filename.substr(0, curAnimInfo.filename.find_last_of("."));
    }
    else
    {
        string* tempfile = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->filename;
        blocks[begin]    = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void nemesis::Process::FilenameBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    if (curAnimInfo.order == 0)
    {
        blocks[begin] = curAnimInfo.filename.substr(0, curAnimInfo.filename.find_last_of("."));
    }
    else
    {
        string* tempfile = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->filename;
        blocks[begin]    = tempfile->substr(0, tempfile->find_last_of("."));
    }
}

void nemesis::Process::FilenameLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->filename;
    blocks[begin]    = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.curAnim->GetGroupAnimInfo()[num]->filename;
    blocks[begin]    = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                            ->singlelist[curAnimInfo.animMulti]
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                            ->singlelist[0]
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                            ->singlelist.back()
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    string* tempfile = &curAnimInfo.masterFunction->grouplist[curAnimInfo.groupMulti]
                            ->singlelist[fixedvarintlist[0]]
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.filename.substr(0, curAnimInfo.filename.find_last_of("."));
}

void nemesis::Process::PathSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //if (isGroup || isMaster)
    //{
    //    blocks[begin] = curAnimInfo.masterFunction->grouplist[0]->singlelist[0]->GetFormat()["Path"];
    //}
    //else
    //{
    //    blocks[begin] = curAnimInfo.filepath.substr(0, curAnimInfo.filepath.length() - 1);
    //}
}

void nemesis::Process::AOMultiGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1 || curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                        ->AnimObject[fixedvarintlist[0]][curAnimInfo.optionMulti];
}

void nemesis::Process::AOMultiGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                        ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AOFirstGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->AnimObject[fixedvarintlist[0]][curAnimInfo.optionMulti];
}

void nemesis::Process::AOFirstGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AONextGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr list;

    curAnimInfo.curAnim->isLast()
        ? list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[fixedvarintlist[0]]
        : list
          = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > curAnimInfo.optionMulti) blocks[begin] = list[curAnimInfo.optionMulti];
}

void nemesis::Process::AONextGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr list;
    curAnimInfo.curAnim->isLast()
        ? list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[fixedvarintlist[0]]
        : list
          = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > fixedvarintlist[1])
    {
        blocks[begin] = list[fixedvarintlist[1]];
    }
}

void nemesis::Process::AOBackGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr list;
    curAnimInfo.order == 0
        ? list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[fixedvarintlist[0]]
        : list
          = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > curAnimInfo.optionMulti) blocks[begin] = list[curAnimInfo.optionMulti];
}

void nemesis::Process::AOBackGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr list;
    curAnimInfo.order == 0
        ? list = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->AnimObject[fixedvarintlist[0]]
        : list
          = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > fixedvarintlist[1]) blocks[begin] = list[fixedvarintlist[1]];
}

void nemesis::Process::AOLastGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                        ->AnimObject[fixedvarintlist[0]][curAnimInfo.optionMulti];
}

void nemesis::Process::AOLastGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                        ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AONumGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[num]
                        ->AnimObject[fixedvarintlist[1]][curAnimInfo.optionMulti];
}

void nemesis::Process::AONumGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.curAnim->GetGroupAnimInfo()[num]->AnimObject[fixedvarintlist[1]][fixedvarintlist[2]];
}

void nemesis::Process::AOMultiMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1 || curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                        ->AnimObject[fixedvarintlist[0]][curAnimInfo.optionMulti];
}

void nemesis::Process::AOMultiMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                        ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AOFirstMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                        ->AnimObject[fixedvarintlist[0]][curAnimInfo.optionMulti];
}

void nemesis::Process::AOFirstMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                        ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AOLastMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                        ->AnimObject[fixedvarintlist[0]][curAnimInfo.optionMulti];
}

void nemesis::Process::AOLastMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                        ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AONumMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                        ->AnimObject[fixedvarintlist[1]][curAnimInfo.optionMulti];
}

void nemesis::Process::AONumMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                        ->AnimObject[fixedvarintlist[1]][fixedvarintlist[2]];
}

void nemesis::Process::AOSingleA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, CombineBlocks(blocks));
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    auto animobj = curAnimInfo.AnimObject.find(fixedvarintlist[0]);

    if (animobj != curAnimInfo.AnimObject.end())
    {
        blocks[begin] = animobj->second[curAnimInfo.optionMulti];
    }
}

void nemesis::Process::AOSingleB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    auto animobj = curAnimInfo.AnimObject.find(fixedvarintlist[0]);

    if (animobj != curAnimInfo.AnimObject.end())
    {
        blocks[begin] = animobj->second[fixedvarintlist[1]];
    }
}

void nemesis::Process::MAEMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]->mainAnimEvent;
}

void nemesis::Process::MAEFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->mainAnimEvent;
}

void nemesis::Process::MAENextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    curAnimInfo.curAnim->isLast()
        ? blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->mainAnimEvent
        : blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order + 1]->mainAnimEvent;
}

void nemesis::Process::MAEBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    curAnimInfo.order == 0
        ? blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order]->mainAnimEvent
        : blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order - 1]->mainAnimEvent;
}

void nemesis::Process::MAELastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]->mainAnimEvent;
}

void nemesis::Process::MAENumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[num]->mainAnimEvent;
}

void nemesis::Process::MAEMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]->mainAnimEvent;
}

void nemesis::Process::MAEFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]->mainAnimEvent;
}

void nemesis::Process::MAELastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin]
        = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]->mainAnimEvent;
}

void nemesis::Process::MAENumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]->mainAnimEvent;
}

void nemesis::Process::MAESingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (ClearBlocks(blocks, curAnimInfo)) blocks[begin] = curAnimInfo.mainAnimEvent;
}

void nemesis::Process::AddOnMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                        ->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti]
                            ->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list
        = &curAnimInfo.curAnim->GetGroupAnimInfo()[0]->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output
            = curAnimInfo.curAnim->GetGroupAnimInfo()[0]->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int curorder = curAnimInfo.order;

    if (!curAnimInfo.curAnim->isLast()) ++curorder;

    VecStr* list
        = &curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output
            = curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int curorder = curAnimInfo.order;

    if (curAnimInfo.order > 0) --curorder;

    VecStr* list
        = &curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output
            = curAnimInfo.curAnim->GetGroupAnimInfo()[curorder]->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list = &curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                        ->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        if (curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                ->addition[fixedvarlist[0]][fixedvarlist[1]]
                .empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder]
                            ->addition[fixedvarlist[0]][fixedvarlist[1]];
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list
        = &curAnimInfo.curAnim->GetGroupAnimInfo()[num]->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output
            = curAnimInfo.curAnim->GetGroupAnimInfo()[num]->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[2]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                        ->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti]
                            ->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[2]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                        ->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0]
                            ->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[2]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                        ->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder]
                            ->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    VecStr* list = &curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                        ->groupAddition[fixedvarlist[0]][fixedvarlist[1]];

    if (list->empty() || curAnimInfo.optionMulti == -1)
    {
        string output = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num]
                            ->addition[fixedvarlist[0]][fixedvarlist[1]];

        if (output.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = output;
    }
    else if (int(list->size()) > curAnimInfo.optionMulti)
    {
        if ((*list)[curAnimInfo.optionMulti].empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
        }

        blocks[begin] = (*list)[curAnimInfo.optionMulti];
    }
    else
    {
        ErrorMessage(1141);
    }
}

void nemesis::Process::AddOnSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    auto grpadd = curAnimInfo.groupAddition.find(fixedvarlist[0]);

    if (curAnimInfo.optionMulti == -1 || grpadd == curAnimInfo.groupAddition.end()
        || grpadd->second.find(fixedvarlist[1]) == grpadd->second.end()
        || grpadd->second.at(fixedvarlist[1]).empty())
    {
        string_view sv;

        try
        {
            sv = curAnimInfo.addition.at(fixedvarlist[0]).at(fixedvarlist[1]);
        }
        catch (const std::exception&)
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        if (sv.empty())
        {
            ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[2]);
        }

        blocks[begin] = sv.data();
    }
    else
    {
        const VecStr& addRef = grpadd->second.at(fixedvarlist[1]);

        if (static_cast<int>(addRef.size()) > curAnimInfo.optionMulti)
        {
            string_view sv = addRef[curAnimInfo.optionMulti];

            if (sv.empty())
            {
                ErrorMessage(1117, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, *curAnimInfo.line);
            }

            blocks[begin] = sv.data();
        }
        else
        {
            ErrorMessage(1141);
        }
    }
}

void nemesis::Process::LastState(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (!ClearBlocks(blocks, curAnimInfo)) return;

    size_t ID = 0;

    if (curAnimInfo.fixedStateID.size() > 1)
    {
        if (!fixedvarlist[0].empty())
        {
            ID = stoi(fixedvarlist[0]) - 1;

            if (ID >= curAnimInfo.fixedStateID.size())
            {
                ErrorMessage(
                    1168, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, "LastState" + fixedvarlist[0]);
            }
        }
    }

    blocks[begin] = to_string(curAnimInfo.lastorder - curAnimInfo.order + curAnimInfo.fixedStateID[ID]);
}

void nemesis::Process::EventID(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string eventname = CombineBlocks(fixedvarintlist[0], fixedvarintlist[1], blocks);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    auto eventitr = curAnimInfo.eventid.find(eventname);

    if (eventitr == curAnimInfo.eventid.end() && eventname != curAnimInfo.zeroEvent)
    {
        ErrorMessage(1131, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, eventname);
    }

    blocks[begin] = to_string(eventitr->second);
}

void nemesis::Process::VariableID(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string variablename = CombineBlocks(fixedvarintlist[0], fixedvarintlist[1], blocks);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    auto variableitr = curAnimInfo.variableid.find(variablename);

    if (variableitr == curAnimInfo.variableid.end() && variablename != curAnimInfo.zeroVariable)
    {
        ErrorMessage(1132, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, variablename);
    }

    blocks[begin] = to_string(variableitr->second);
}

void nemesis::Process::Crc32(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string crc32line = nemesis::to_lower_copy(CombineBlocks(blocks));

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    blocks[begin] = to_string(CRC32Convert(crc32line));
}

void nemesis::Process::Import(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string import = CombineBlocks(blocks);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    auto itr = curAnimInfo.IDExist.find(import);

    if (itr != curAnimInfo.IDExist.end())
    {
        blocks[begin] = itr->second;
        return;
    }

    size_t pos  = import.find("[") + 1;
    string file = import.substr(pos, import.find("]", pos) - pos);
    string keyword;
    string tempID;

    if (import.find("[", pos) != NOT_FOUND)
    {
        pos                = import.find("[", pos);
        string tempKeyword = import.substr(pos, import.find_last_of("]") + 1 - pos);
        int openBrack      = 0;

        for (char curChar : tempKeyword)
        {
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
            ? ErrorMessage(1139, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, import)
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

    //if (tempID.empty())
    //{
    //    tempID = (isMaster || isGroup ? curAnimInfo.curGroup->newID() : curAnimInfo.curAnim->newID());
    //    curAnimInfo.IDExist[import]             = tempID;
    //    (*curAnimInfo.newImport)[file][keyword] = tempID;
    //}

    blocks[begin] = tempID;
}

void nemesis::Process::MotionDataMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[0];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int nextorder;
    curAnimInfo.curAnim->isLast() ? nextorder = curAnimInfo.order : nextorder = curAnimInfo.order + 1;
    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[nextorder];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int previousorder;
    curAnimInfo.order == 0 ? previousorder = curAnimInfo.order : previousorder = curAnimInfo.order - 1;
    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[previousorder];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[num];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo
        = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo
        = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    MotionValidation(curAnimInfo);

    if (curAnimInfo.filename == fixedvarlist[0])
    {
        ErrorMessage(1134, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order];
    string motionData;

    if (curInfo->motionData.empty())
    {
        WarningMessage(1018, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        motionData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        motionData = to_string(curInfo->motionData.size()) + "\n";

        for (auto& motion : curInfo->motionData)
        {
            motionData.append(motion + "\n");
        }
    }

    blocks[begin] = motionData;
}

void nemesis::Process::RotationDataMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (curAnimInfo.animMulti == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.animMulti];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[0];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int nextorder;
    curAnimInfo.curAnim->isLast() ? nextorder = curAnimInfo.order : nextorder = curAnimInfo.order + 1;
    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[nextorder];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    int previousorder;
    curAnimInfo.order == 0 ? previousorder = curAnimInfo.order : previousorder = curAnimInfo.order - 1;
    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[previousorder];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.lastorder];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[num];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (curAnimInfo.animMulti == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo
        = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.animMulti];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][0];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo
        = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][curAnimInfo.lastorder];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (curAnimInfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), curAnimInfo.numline, fixedvarlist[0]);
    }

    size_t num = fixedvarintlist[0];

    if (num >= curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti].size())
    {
        ErrorMessage(1148, GetFormat(), curAnimInfo.filename, curAnimInfo.numline, fixedvarlist[0]);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curGroup->groupAnimInfo[curAnimInfo.groupMulti][num];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    RotationValidation(curAnimInfo);

    if (curAnimInfo.filename == fixedvarlist[0])
    {
        ErrorMessage(1134, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
    }

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    shared_ptr<AnimationInfo> curInfo = curAnimInfo.curAnim->GetGroupAnimInfo()[curAnimInfo.order];
    string rotationData;

    if (curInfo->rotationData.empty())
    {
        WarningMessage(1019, GetFormat(), GetBehaviorFile(), curAnimInfo.numline);
        rotationData
            = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    }
    else
    {
        rotationData = to_string(curInfo->rotationData.size()) + "\n";

        for (auto& rotation : curInfo->rotationData)
        {
            rotationData.append(rotation + "\n");
        }
    }

    blocks[begin] = rotationData;
}

void nemesis::Process::AnimOrder(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string animPath = CombineBlocks(fixedvarintlist[0], fixedvarintlist[1], blocks);

    if (!ClearBlocks(blocks, curAnimInfo)) return;

    //auto& ptr = charAnimDataInfo.find(
    //    nemesis::to_lower_copy(curAnimInfo.project.substr(0, curAnimInfo.project.rfind(".txt"))));

    //if (ptr != charAnimDataInfo.end())
    //{
    //    auto& ptr2
    //        = ptr->second.find(nemesis::to_lower_copy(std::filesystem::path(animPath).filename().string()));

    //    if (ptr2 != ptr->second.end())
    //    {
    //        blocks[begin] = to_string(ptr2->second->GetOrder());
    //    }
    //}
}

void nemesis::Process::RegisAnim(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string animPath = CombineBlocks(blocks);
    addUsedAnim(GetBehaviorFile().string(), animPath);
}

void nemesis::Process::RegisBehavior(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    string behaviorName = CombineBlocks(blocks);
    nemesis::to_lower(behaviorName);
    behaviorJoints[behaviorName].push_back(nemesis::to_lower_copy(GetBehaviorFile().string()));
}

void nemesis::Process::LocalNegative(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const
{
    if (CombineBlocks(blocks)[0] == '-') curAnimInfo.negative = true;
}
