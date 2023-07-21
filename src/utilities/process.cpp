#include "utilities/process.h"
#include "utilities/template.h"
#include "utilities/constants.h"
#include "utilities/atomiclock.h"
#include "utilities/lineprocess.h"
#include "utilities/animqueryfile.h"

#include "core/preprocessline.h"

#include "generate/alternateanimation.h"

#include "generate/animation/grouptemplate.h"
#include "generate/animation/templatetree.h"

#include "scope/scopeinfo.h"

using namespace std;

const nemesis::regex nemesis::Process::elementrgx
    = nemesis::regex(R"((.*<hkparam name\=".+" numelements\=").+(">.*))");

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
                int animnum,
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
    //    if (animnum == -1) ErrorMessage(1057, GetFormat(), GetBehaviorFile(), numline, original);

    //    if (masterFunction->grouplist[groupMulti]->singlelist[animnum]->GetFormat().find(stateID)
    //        == masterFunction->grouplist[groupMulti]->singlelist[animnum]->GetFormat().end())
    //    {
    //        ErrorMessage(1127, GetFormat(), GetBehaviorFile(), numline);
    //    }

    //    state = masterFunction->grouplist[groupMulti]->singlelist[animnum]->GetFormat()[stateID];
    //}
}

void nemesis::Process::ClearBlocks(VecStr& blocks) const
{
    ClearBlocks(begin, end, blocks);
}

void nemesis::Process::ClearBlocks(size_t ibegin, size_t iend, VecStr& blocks) const
{
    for (size_t i = ibegin; i < iend; ++i)
    {
        blocks[i].clear();
    }
}

string nemesis::Process::CombineBlocks(VecStr& blocks) const
{
    return CombineBlocks(begin, end, blocks);
}

string nemesis::Process::CombineBlocks(size_t ibegin, size_t iend, VecStr& blocks) const
{
    string option;

    for (size_t i = ibegin; i < iend; ++i)
    {
        option.append(blocks[i]);
    }

    return option;
}

void nemesis::Process::MotionValidation(nemesis::ScopeInfo& scopeinfo) const
{
    if (!scopeinfo.exporter->IsBehavior()) return;

    ErrorMessage(1096, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
}

void nemesis::Process::RotationValidation(nemesis::ScopeInfo& scopeinfo) const
{
    if (!scopeinfo.exporter->IsBehavior()) return;

    ErrorMessage(1097, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
}

std::string nemesis::Process::IDRegisByType(nemesis::ScopeInfo& scopeinfo, nemesis::File::FileType filetype) const
{
    auto* exporter               = scopeinfo.GetExporter();
    auto* templtclass            = exporter->GetTemplateCategory(fixedHkxVariableList.front());
    auto* animquery              = scopeinfo.GetAnim(templtclass);
    const nemesis::File* fileptr = nullptr;

    for (auto& templt : templtclass->GetTemplateList())
    {
        if (templt->GetType() != filetype) continue;

        fileptr = templt.get();
        break;
    }

    auto ss = exporter->GetID(fixedHkxVariableList.back(), fileptr, animquery);
    return std::string(ss);
}

void nemesis::Process::ExeFirstAnimFromScope(VecStr& blocks,
                                             nemesis::ScopeInfo& scopeinfo, funcptr func) const
{
    scopeinfo.ExeTempNumAnim(0, GetTemplateCategory(), [&]() { (this->*func)(blocks, scopeinfo); });
}

void nemesis::Process::ExeNextAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const
{
    scopeinfo.ExeTempNextAnim(GetTemplateCategory(), [&]() { (this->*func)(blocks, scopeinfo); });
}

void nemesis::Process::ExeBackAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const
{
    scopeinfo.ExeTempBackAnim(GetTemplateCategory(), [&]() { (this->*func)(blocks, scopeinfo); });
}

void nemesis::Process::ExeLastAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const
{
    scopeinfo.ExeTempLastAnim(GetTemplateCategory(), [&]() { (this->*func)(blocks, scopeinfo); });
}

void nemesis::Process::ExeNumAnimFromScope(size_t num,
                                           VecStr& blocks,
                                           nemesis::ScopeInfo& scopeinfo,
                                           funcptr func) const
{
    scopeinfo.ExeTempNumAnim(num, GetTemplateCategory(), [&]() { (this->*func)(blocks, scopeinfo); });
}

nemesis::Process::~Process() = default;

void nemesis::Process::AddFunction(funcptr pfunction) noexcept
{
    this->pfunction = pfunction;
}

void nemesis::Process::AddProcessLinePtr(nemesis::PreprocessLine* linkedline) noexcept
{
    plinkedline = linkedline;
}

void nemesis::Process::AddAnimVarPtr(UPtr<nemesis::AnimVarPtr> animvarptr) noexcept
{
    varptr = std::move(animvarptr);
}

void nemesis::Process::SetBegin(size_t begin) noexcept
{
    this->begin = begin;
}

void nemesis::Process::SetEnd(size_t end) noexcept
{
    this->end = end;
}

void nemesis::Process::SetFixedVar(const VecStr& fixedHkxVariableList) noexcept
{
    this->fixedHkxVariableList = fixedHkxVariableList;
}

void nemesis::Process::SetFixedVarInt(const Vec<int>& fixedvarintlist) noexcept
{
    this->fixedvarintlist = fixedvarintlist;
}

void nemesis::Process::Compile(VecStr& blocks, nemesis::ScopeInfo& scopeinfo)
{
    if (IsInFailedRange(scopeinfo)) return;

    (this->*pfunction)(blocks, scopeinfo);
}

bool nemesis::Process::IsInFailedRange(nemesis::ScopeInfo& scopeinfo) const
{
    for (auto& failptr : scopeinfo.failed)
    {
        if (failptr->pointA <= begin && end <= failptr->pointB) return true;
    }

    return false;
}

void nemesis::Process::ClearMultiChoice(nemesis::ScopeInfo& scopeinfo) const
{
    scopeinfo.failed.clear();
    scopeinfo.captured = nullptr;
}

size_t nemesis::Process::GetSize() const noexcept
{
    return end - begin;
}

size_t nemesis::Process::GetBegin() const noexcept
{
    return begin;
}

size_t nemesis::Process::GetEnd() const noexcept
{
    return end;
}

size_t nemesis::Process::GetCurrentLineNum() const noexcept
{
    return plinkedline->GetLineNumber();
}

string_view nemesis::Process::GetFormat() const
{
    return plinkedline->GetFile()->GetFileClassName();
}

std::filesystem::path nemesis::Process::GetBehaviorFile() const
{
    return plinkedline->GetFile()->GetFilePath();
}

const nemesis::TemplateCategory* nemesis::Process::GetTemplateCategory() const
{
    return &plinkedline->GetTemplate()->GetTemplateCategory();
}

void nemesis::Process::RelativeNegative(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin]        = scopeinfo.isend || scopeinfo.negative ? "true" : "false";
    scopeinfo.negative = false;
    scopeinfo.isend      = false;
}

void nemesis::Process::Compute(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    scopeinfo.catchingelement = true;
    scopeinfo.norelment       = true;
}

void nemesis::Process::RangeCompute(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    *scopeinfo.elementline = nemesis::regex_replace(
        *scopeinfo.elementline, elementrgx, "$1" + to_string(scopeinfo.elementcounter) + "$2");
    scopeinfo.norelment      = false;
    scopeinfo.elementcounter = 0;
    scopeinfo.elementline    = nullptr;
}

void nemesis::Process::UpCounter(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ++scopeinfo.elementcounter;
}

void nemesis::Process::UpCounterPlus(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string full = CombineBlocks(0, blocks.size() - 1, blocks);
    scopeinfo.elementcounter += count(full.begin(), full.end(), '#');
}

void nemesis::Process::AnimCount(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = to_string(scopeinfo.GetCurrentQuery()->GetBehaviorIndex());
}

void nemesis::Process::MultiChoicePre(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    // Can't use ClearBlocks because MultiChoice has to be done last
    //ClearBlocks(blocks);

    auto& mc = plinkedline->GetProcess()->GetMultiChoice();
    auto& list = mc.GetChoiceList();

    for (size_t i = 0; i < list.size(); ++i)
    {
        auto& choice = list[i];

        if (!choice.IsTrue(scopeinfo))
        {
            scopeinfo.failed.push_back(&choice);
            continue;
        }

        scopeinfo.captured = &choice;

        while (++i < list.size())
        {
            scopeinfo.failed.emplace_back(&list[i]);
        }

        break;
    }
}

void nemesis::Process::MultiChoicePost(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    auto start    = fixedvarintlist.front();
    auto finish   = fixedvarintlist.back();
    auto captured = scopeinfo.captured;
    ClearBlocks(start, finish, blocks);

    blocks[start] = CombineBlocks(captured->pointA, captured->pointB, blocks);
    ClearMultiChoice(scopeinfo);
}

void nemesis::Process::GroupIDRegis(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    /*
    string oldID = masterformat + "_group$" + fixedHkxVariableList[0];
    Lockless lock(scopeinfo.animLock->subIDLock);

    if (scopeinfo.groupFunction->functionIDs.find(oldID) != scopeinfo.groupFunction->functionIDs.end())
    {
        blocks[begin] = scopeinfo.groupFunction->functionIDs[oldID];
    }
    else
    {
        string strID                                  = scopeinfo.curAnim->newID();
        scopeinfo.groupFunction->functionIDs[oldID] = strID;
        blocks[begin]                                 = strID;
    }
    */
}

void nemesis::Process::IDRegis(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    auto* exporter  = scopeinfo.GetExporter();
    auto* animquery = scopeinfo.GetCurrentQuery();

    blocks[begin] = std::string(exporter->GetID(fixedHkxVariableList.front(), plinkedline->GetFile(), animquery));

    /*
    string ID;
    string oldID = masterformat + "$" + fixedHkxVariableList[0];

    if (scopeinfo.IDExist[oldID].length() > 0)
    {
        ID = scopeinfo.IDExist[oldID];
    }
    else
    {
        ID                         = scopeinfo.curAnim->newID();
        scopeinfo.IDExist[oldID] = ID;
    }

    scopeinfo.curAnim->subFunctionIDs->GetFormat()[oldID] = ID;
    blocks[begin]                                      = ID;
    */
}

void nemesis::Process::IDRegisAnim(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = IDRegisByType(scopeinfo, nemesis::File::FileType::SINGLE);

    /*
    string ID;
    string oldID = masterformat + "$" + fixedHkxVariableList[0];
    int n_groupMulti;

    if (isMaster && scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[1]);
    }

    n_groupMulti = scopeinfo.groupMulti;

    if (scopeinfo.animnum == -1)
    {
        ErrorMessage(1057, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    if (scopeinfo.masterFunction->grouplist[n_groupMulti]->singlelist[scopeinfo.animnum]->format.find(
            oldID)
        != scopeinfo.masterFunction->grouplist[n_groupMulti]
               ->singlelist[scopeinfo.animnum]
               ->format.end())
    {
        ID = scopeinfo.masterFunction->grouplist[n_groupMulti]
                 ->singlelist[scopeinfo.animnum]
                 ->format[oldID];
        scopeinfo.IDExist[oldID] = ID;
    }
    else
    {
        if (scopeinfo.IDExist[oldID].length() > 0)
        {
            ID = scopeinfo.IDExist[oldID];
        }
        else
        {
            ID                         = scopeinfo.curGroup->newID();
            scopeinfo.IDExist[oldID] = ID;
        }

        scopeinfo.masterFunction->grouplist[n_groupMulti]->singlelist[scopeinfo.animnum]->format[oldID]
            = ID;
    }

    blocks[begin] = ID;
    */
}

void nemesis::Process::IDRegisGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = IDRegisByType(scopeinfo, nemesis::File::FileType::GROUP);

    /*
    string ID;
    string oldID = masterformat + "_group$" + fixedHkxVariableList[0];
    int n_groupMulti;

    if (isMaster && scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[1]);
    }

    n_groupMulti = scopeinfo.groupMulti;

    if (scopeinfo.masterFunction->grouplist[n_groupMulti]->functionIDs.find(oldID)
        != scopeinfo.masterFunction->grouplist[n_groupMulti]->functionIDs.end())
    {
        ID                         = scopeinfo.masterFunction->grouplist[n_groupMulti]->functionIDs[oldID];
        scopeinfo.IDExist[oldID] = ID;
    }
    else
    {
        if (scopeinfo.IDExist[oldID].length() > 0)
        {
            ID = scopeinfo.IDExist[oldID];
        }
        else
        {
            ID                         = scopeinfo.curGroup->newID();
            scopeinfo.IDExist[oldID] = ID;
        }

        scopeinfo.masterFunction->grouplist[n_groupMulti]->functionIDs[oldID] = ID;
    }

    blocks[begin] = ID;
    */
}

void nemesis::Process::IDRegisMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = IDRegisByType(scopeinfo, nemesis::File::FileType::MASTER);

    /*
    string ID;
    string oldID = masterformat + "_master$" + fixedHkxVariableList[0];

    if (scopeinfo.masterFunction->functionIDs.find(oldID) != scopeinfo.masterFunction->functionIDs.end())
    {
        ID                         = scopeinfo.masterFunction->functionIDs[oldID];
        scopeinfo.IDExist[oldID] = ID;
    }
    else
    {
        if (scopeinfo.IDExist[oldID].length() > 0)
        {
            ID = scopeinfo.IDExist[oldID];
        }
        else
        {
            ID                         = scopeinfo.curGroup->newID();
            scopeinfo.IDExist[oldID] = ID;
        }

        scopeinfo.masterFunction->functionIDs[oldID] = ID;
    }

    blocks[begin] = ID;
    */
}

void nemesis::Process::Computation(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string equation       = CombineBlocks(blocks);
    size_t equationLength = equation.length();
    ClearBlocks(blocks);

    /*
    if (equation.find("L") != NOT_FOUND)
    {
        if (isMaster)
        {
            ErrorMessage(1206, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
        }

        if (isGroup)
        {
            ErrorMessage(1206, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
        }

        int maths2 = count(equation.begin(), equation.end(), 'L');

        for (__int64 k = 0; k < maths2; ++k)
        {
            equation.replace(equation.find("L"), 1, to_string(scopeinfo.lastorder));
        }
    }

    if (equation.find("N") != NOT_FOUND)
    {
        if (isMaster)
        {
            ErrorMessage(1206, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
        }

        if (isGroup)
        {
            ErrorMessage(1206, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
        }

        int maths2 = count(equation.begin(), equation.end(), 'N');
        string nextorder;
        nextorder
            = scopeinfo.curAnim->isLast() ? to_string(scopeinfo.order) : to_string(scopeinfo.order + 1);

        for (__int64 k = 0; k < maths2; ++k)
        {
            equation.replace(equation.find("N"), 1, nextorder);
        }
    }

    if (equation.find("B") != NOT_FOUND)
    {
        if (isMaster)
        {
            ErrorMessage(1206, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
        }
        if (isGroup)
        {
            ErrorMessage(1206, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
        }

        int maths2 = count(equation.begin(), equation.end(), 'B');
        string preorder;
        preorder = scopeinfo.order == 0 ? to_string(scopeinfo.order) : to_string(scopeinfo.order - 1);

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

    nemesis::calculate(equation, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    blocks[begin] = equation;
    */
}

void nemesis::Process::ImportNode(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string import_input = CombineBlocks(blocks);

    ClearBlocks(blocks);

    auto* exporter = scopeinfo.GetExporter();
    blocks[begin]  = std::string(exporter->GetImportID(import_input));
}

void nemesis::Process::ImportIndex(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin]  = scopeinfo.GetCurrentImport()->GetParameter(fixedvarintlist.front());
}

void nemesis::Process::EndMulti(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //EndSingle(blocks, scopeinfo.GetTempAnim(fixedvarintlist.front(), GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNumAnim(fixedvarintlist.front(), GetTemplateCategory(), [&]()
    {
        EndSingle(blocks, scopeinfo);
    });
}

void nemesis::Process::EndFirst(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //EndSingle(blocks, scopeinfo.GetTempAnim(0, GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNumAnim(0, GetTemplateCategory(), [&]() { EndSingle(blocks, scopeinfo); });
}

void nemesis::Process::EndNext(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //EndSingle(blocks, scopeinfo.GetTempNextAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNextAnim(GetTemplateCategory(), [&]() { EndSingle(blocks, scopeinfo); });
}

void nemesis::Process::EndBack(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //EndSingle(blocks, scopeinfo.GetTempBackAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempBackAnim(GetTemplateCategory(), [&]() { EndSingle(blocks, scopeinfo); });
}

void nemesis::Process::EndLast(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //EndSingle(blocks, scopeinfo.GetTempLastAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempLastAnim(GetTemplateCategory(), [&]() { EndSingle(blocks, scopeinfo); });
}

void nemesis::Process::EndNum(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //EndSingle(blocks, scopeinfo.GetTempAnim(fixedvarintlist[0], GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNumAnim(fixedvarintlist.front(), GetTemplateCategory(), [&]()
    {
        EndSingle(blocks, scopeinfo);
    });
}

/*
void nemesis::Process::EndMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.animnum == -1)
    {
        ErrorMessage(1057, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    if (scopeinfo.groupnum == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    auto& rootquery = scopeinfo.GetGroupNumQuery();
    auto& curquery  = rootquery.GetGroupedQuery(scopeinfo.animnum);
    End(blocks, scopeinfo, curquery);
}

void nemesis::Process::EndFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupnum == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    auto& rootquery = scopeinfo.GetGroupNumQuery();
    auto& curquery  = rootquery.GetGroupedQuery(0);
    End(blocks, scopeinfo, curquery);
}

void nemesis::Process::EndLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupnum == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    auto& rootquery = scopeinfo.GetGroupNumQuery();
    auto& curquery  = rootquery.GetGroupLast();
    End(blocks, scopeinfo, curquery);
}

void nemesis::Process::EndNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupnum == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    size_t num      = fixedvarintlist[0];
    auto& rootquery = scopeinfo.GetGroupNumQuery();

    if (num >= rootquery.GetGroupSize())
    {
        ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    auto& curquery = rootquery.GetGroupedQuery(num);
    End(blocks, scopeinfo, curquery);
}
*/

void nemesis::Process::EndSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    auto anim = scopeinfo.GetAnim(GetTemplateCategory());
    auto option = anim->GetOptionPtr(duration::name);

    if (option)
    {
        blocks[begin] = option->GetVariable(duration::variable);
        return;
    }

    scopeinfo.isend = true;
    auto& models    = anim->GetAnimClass().GetOptionModels();
    blocks[begin]   = models.GetModel(duration::name)->GetVariablePtr(duration::variable)->GetDefaultValue();
}

void nemesis::Process::StateMulti(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    auto templt    = plinkedline->GetTemplate();
    auto multiplier = templt->GetStateMultiplier();

    //if (templt->IsMaster())
    //    ;

    //blocks[begin] = to_string(
    //    scopeinfo.fixedStateID[fixedvarintlist[0]]
    //    + ((scopeinfo.animnum - scopeinfo.order) * scopeinfo.stateCountMultiplier[fixedvarintlist[0]])
    //    + fixedvarintlist[1]);
}

void nemesis::Process::StateFirst(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin] = to_string(scopeinfo.fixedStateID[fixedvarintlist[0]]
    //                          - (scopeinfo.order * scopeinfo.stateCountMultiplier[fixedvarintlist[0]])
    //                          + fixedvarintlist[1]);
}

void nemesis::Process::StateNext(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //scopeinfo.curAnim->isLast()
    //    ? blocks[begin] = to_string(scopeinfo.fixedStateID[fixedvarintlist[0]] + fixedvarintlist[1])
    //    : blocks[begin]
    //      = to_string(scopeinfo.fixedStateID[fixedvarintlist[0]]
    //                  + scopeinfo.stateCountMultiplier[fixedvarintlist[0]] + fixedvarintlist[1]);
}

void nemesis::Process::StateBack(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //scopeinfo.order == 0
    //    ? blocks[begin] = to_string(scopeinfo.fixedStateID[fixedvarintlist[0]] + fixedvarintlist[1])
    //    : blocks[begin]
    //      = to_string(scopeinfo.fixedStateID[fixedvarintlist[0]]
    //                  - scopeinfo.stateCountMultiplier[fixedvarintlist[0]] + fixedvarintlist[1]);
}

void nemesis::Process::StateLast(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin] = to_string(
    //    scopeinfo.fixedStateID[fixedvarintlist[0]]
    //    + ((scopeinfo.lastorder - scopeinfo.order) * scopeinfo.stateCountMultiplier[fixedvarintlist[0]])
    //    + fixedvarintlist[1]);
}

void nemesis::Process::StateNum(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    size_t num = fixedvarintlist[0];

    //if (fixedvarintlist[2] >= int(scopeinfo.curAnim->GetGroupAnimInfo().size()))
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin]
    //    = to_string(scopeinfo.fixedStateID[num]
    //                + ((fixedvarintlist[2] - scopeinfo.order) * scopeinfo.stateCountMultiplier[num])
    //                + fixedvarintlist[1]);
}

void nemesis::Process::StateMultiMasterToGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    string state;
    //stateInput(state,
    //           (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
    //               + to_string(fixedvarintlist[1]) + ")",
    //           GetFormat(),
    //           GetBehaviorFile().string(),
    //           fixedHkxVariableList[0],
    //           true,
    //           scopeinfo.groupMulti,
    //           scopeinfo.animnum,
    //           GetCurrentLineNum(),
    //           scopeinfo.stateCount,
    //           scopeinfo.masterFunction);
    blocks[begin] = state;
}

/*
void nemesis::Process::StateMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedHkxVariableList[0],
               false,
               scopeinfo.groupMulti,
               scopeinfo.animnum,
               GetCurrentLineNum(),
               scopeinfo.stateCount,
               scopeinfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedHkxVariableList[0],
               false,
               scopeinfo.groupMulti,
               0,
               GetCurrentLineNum(),
               scopeinfo.stateCount,
               scopeinfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedHkxVariableList[0],
               false,
               scopeinfo.groupMulti,
               scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]->singlelist.size() - 1,
               GetCurrentLineNum(),
               scopeinfo.stateCount,
               scopeinfo.masterFunction);
    blocks[begin] = state;
}

void nemesis::Process::StateNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    size_t num = fixedvarintlist[2];

    if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    {
        ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    string state;
    stateInput(state,
               (fixedvarintlist[0] == 0 ? "(S+" : "(S" + to_string(fixedvarintlist[0] + 1) + "+")
                   + to_string(fixedvarintlist[1]) + ")",
               GetFormat(),
               GetBehaviorFile().string(),
               fixedHkxVariableList[0],
               false,
               scopeinfo.groupMulti,
               num,
               GetCurrentLineNum(),
               scopeinfo.stateCount,
               scopeinfo.masterFunction);
    blocks[begin] = state;
}
*/

void nemesis::Process::StateSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    std::string state = varptr->GetResult(scopeinfo);
    blocks[begin] = state;
}

void nemesis::Process::FilepathMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    FilepathSingle(blocks, scopeinfo);
}

void nemesis::Process::FilepathFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //FilepathSingle(blocks, scopeinfo.GetTempAnim(0, GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNumAnim(0, GetTemplateCategory(), [&]() { FilepathSingle(blocks, scopeinfo); });
}

void nemesis::Process::FilepathNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //FilepathSingle(blocks, scopeinfo.GetTempNextAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNextAnim(GetTemplateCategory(), [&]() { FilepathSingle(blocks, scopeinfo); });
}

void nemesis::Process::FilepathBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //FilepathSingle(blocks, scopeinfo.GetTempNextAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNextAnim(GetTemplateCategory(), [&]() { FilepathSingle(blocks, scopeinfo); });
}

void nemesis::Process::FilepathLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //FilepathSingle(blocks, scopeinfo.GetTempLastAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempLastAnim(GetTemplateCategory(), [&]() { FilepathSingle(blocks, scopeinfo); });
}

void nemesis::Process::FilepathNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //FilepathSingle(blocks, scopeinfo.GetTempAnim(fixedvarintlist[0], GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNumAnim(fixedvarintlist.front(), GetTemplateCategory(), [&]()
    {
        FilepathSingle(blocks, scopeinfo);
    });
}

/*
void nemesis::Process::FilepathMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.animnum == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());

    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    blocks[begin] = scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                        ->singlelist[scopeinfo.animnum]
                        ->format["FilePath"];
}

void nemesis::Process::FilepathFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    blocks[begin] = scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                        ->singlelist[0]
                        ->format["FilePath"];
}

void nemesis::Process::FilepathLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    blocks[begin] = scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                        ->singlelist.back()
                        ->format["FilePath"];
}

void nemesis::Process::FilepathNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    blocks[begin] = scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                        ->singlelist[fixedvarintlist[0]]
                        ->format["FilePath"];
}
*/

void nemesis::Process::FilepathSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    auto anim = scopeinfo.GetAnim(GetTemplateCategory());

    ClearBlocks(blocks);

    blocks[begin] = anim->GetAnimPath().string();
}

void nemesis::Process::FilenameMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    FilenameSingle(blocks, scopeinfo);
}

void nemesis::Process::FilenameFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ExeFirstAnimFromScope(blocks, scopeinfo, &nemesis::Process::FilenameSingle);
}

void nemesis::Process::FilenameNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //auto tempanim = scopeinfo.GetTempNextAnim();
    //scopeinfo.SetTempAnim(tempanim);
    //FilenameSingle(blocks, scopeinfo.GetTempNextAnim(GetTemplateCategory())->GetScopeInfo());
}

void nemesis::Process::FilenameBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //auto tempanim = scopeinfo.GetTempBackAnim();
    //scopeinfo.SetTempAnim(tempanim);
    //FilenameSingle(blocks, scopeinfo.GetTempBackAnim(GetTemplateCategory())->GetScopeInfo());
}

void nemesis::Process::FilenameLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //auto tempanim = scopeinfo.GetTempLastAnim();
    //scopeinfo.SetTempAnim(tempanim);
    //FilenameSingle(blocks, scopeinfo.GetTempLastAnim(GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempLastAnim(GetTemplateCategory(), [&]() { FilenameSingle(blocks, scopeinfo); });
}

void nemesis::Process::FilenameNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //auto tempanim = scopeinfo.GetTempAnim(fixedvarintlist[0]);
    //scopeinfo.SetTempAnim(tempanim);
    //FilenameSingle(blocks, scopeinfo.GetTempAnim(fixedvarintlist[0], GetTemplateCategory())->GetScopeInfo());
    scopeinfo.ExeTempNumAnim(fixedvarintlist.front(), GetTemplateCategory(), [&]()
    {
        FilenameSingle(blocks, scopeinfo);
    });
}

/*
void nemesis::Process::FilenameMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    if (scopeinfo.animnum == -1)
    {
        ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    }

    ClearBlocks(blocks);

    string* tempfile = &scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                            ->singlelist[scopeinfo.animnum]
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    string* tempfile = &scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                            ->singlelist[0]
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    string* tempfile = &scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                            ->singlelist.back()
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}

void nemesis::Process::FilenameNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (scopeinfo.groupMulti == -1)
    {
        ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    }

    ClearBlocks(blocks);

    string* tempfile = &scopeinfo.masterFunction->grouplist[scopeinfo.groupMulti]
                            ->singlelist[fixedvarintlist[0]]
                            ->format["FileName"];
    blocks[begin] = tempfile->substr(0, tempfile->find_last_of("."));
}
*/

void nemesis::Process::FilenameSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    auto anim = scopeinfo.GetAnim(GetTemplateCategory());

    ClearBlocks(blocks);

    blocks[begin] = anim->GetAnimPath().stem().string();
}

void nemesis::Process::PathSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    auto anim = scopeinfo.GetAnim(GetTemplateCategory());

    ClearBlocks(blocks);

    blocks[begin] = anim->GetAnimPath().parent_path().string();
}

void nemesis::Process::AOMulti(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum]
    //                    ->AnimObject[fixedvarintlist[0]][scopeinfo.optionMulti];
}

void nemesis::Process::AOMultiTarget(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum]
    //                    ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AOFirstGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (curaniminfo.optionmulti == -1)
    //{
    //    errormessage(1126, getformat(), getbehaviorfile(), getcurrentlinenum(), combineblocks(blocks));
    //}

    //if (!clearblocks(blocks, curaniminfo)) return;

    //blocks[begin]
    //    = curaniminfo.curanim->getgroupaniminfo()[0]->animobject[fixedvarintlist[0]][curaniminfo.optionmulti];
}

void nemesis::Process::AOFirstGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin]
    //    = scopeinfo.curAnim->GetGroupAnimInfo()[0]->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AONextGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{/*
    if (scopeinfo.optionMulti == -1)
    {
        ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    }

    ClearBlocks(blocks);

    VecStr list;

    scopeinfo.curAnim->isLast()
        ? list = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order]->AnimObject[fixedvarintlist[0]]
        : list
          = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order + 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > scopeinfo.optionMulti) blocks[begin] = list[scopeinfo.optionMulti];*/
}

void nemesis::Process::AONextGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    VecStr list;
    //scopeinfo.curAnim->isLast()
    //    ? list = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order]->AnimObject[fixedvarintlist[0]]
    //    : list
    //      = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order + 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > fixedvarintlist[1])
    {
        blocks[begin] = list[fixedvarintlist[1]];
    }
}

void nemesis::Process::AOBackGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //ClearBlocks(blocks);

    //VecStr list;
    //scopeinfo.order == 0
    //    ? list = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order]->AnimObject[fixedvarintlist[0]]
    //    : list
    //      = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order - 1]->AnimObject[fixedvarintlist[0]];

    //if (int(list.size()) > scopeinfo.optionMulti) blocks[begin] = list[scopeinfo.optionMulti];
}

void nemesis::Process::AOBackGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    VecStr list;
    //scopeinfo.order == 0
    //    ? list = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order]->AnimObject[fixedvarintlist[0]]
    //    : list
    //      = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order - 1]->AnimObject[fixedvarintlist[0]];

    if (int(list.size()) > fixedvarintlist[1]) blocks[begin] = list[fixedvarintlist[1]];
}

void nemesis::Process::AOLastGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder]
    //                    ->AnimObject[fixedvarintlist[0]][scopeinfo.optionMulti];
}

void nemesis::Process::AOLastGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder]
    //                    ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AONumGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[num]
    //                    ->AnimObject[fixedvarintlist[1]][scopeinfo.optionMulti];
}

void nemesis::Process::AONumGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin]
    //    = scopeinfo.curAnim->GetGroupAnimInfo()[num]->AnimObject[fixedvarintlist[1]][fixedvarintlist[2]];
}

void nemesis::Process::AOMultiMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1 || scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum]
    //                    ->AnimObject[fixedvarintlist[0]][scopeinfo.optionMulti];
}

void nemesis::Process::AOMultiMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum]
    //                    ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AOFirstMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0]
    //                    ->AnimObject[fixedvarintlist[0]][scopeinfo.optionMulti];
}

void nemesis::Process::AOFirstMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0]
    //                    ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AOLastMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder]
    //                    ->AnimObject[fixedvarintlist[0]][scopeinfo.optionMulti];
}

void nemesis::Process::AOLastMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder]
    //                    ->AnimObject[fixedvarintlist[0]][fixedvarintlist[1]];
}

void nemesis::Process::AONumMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.optionMulti == -1)
    //{
    //    ErrorMessage(1126, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), CombineBlocks(blocks));
    //}

    //size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti].size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num]
    //                    ->AnimObject[fixedvarintlist[1]][scopeinfo.optionMulti];
}

void nemesis::Process::AONumMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti].size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num]
    //                    ->AnimObject[fixedvarintlist[1]][fixedvarintlist[2]];
}

void nemesis::Process::AOSingleA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //int num       = fixedvarintlist.at(fixedvarintlist.size() - 2);
    //auto templist = scopeinfo.GetTempAnimObjectList(num);
    //scopeinfo.SetTempAnimObjectList(templist);
    //auto animobj = scopeinfo.AnimObject.find(fixedvarintlist[0]);

    //if (animobj != scopeinfo.AnimObject.end())
    //{
    //    blocks[begin] = animobj->second[scopeinfo.optionMulti];
    //}
}

void nemesis::Process::AOSingleB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //ClearBlocks(blocks);

    //auto animobj = scopeinfo.AnimObject.find(fixedvarintlist[0]);

    //if (animobj != scopeinfo.AnimObject.end())
    //{
    //    blocks[begin] = animobj->second[fixedvarintlist[1]];
    //}
}

void nemesis::Process::AOSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //string tet = scopeinfo.GetAnimObject();
    //blocks[begin] = scopeinfo.GetAnimObject();
}

void nemesis::Process::MAEMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum]->mainAnimEvent;
}

void nemesis::Process::MAEFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[0]->mainAnimEvent;
}

void nemesis::Process::MAENextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //scopeinfo.curAnim->isLast()
    //    ? blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order]->mainAnimEvent
    //    : blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order + 1]->mainAnimEvent;
}

void nemesis::Process::MAEBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //scopeinfo.order == 0
    //    ? blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order]->mainAnimEvent
    //    : blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order - 1]->mainAnimEvent;
}

void nemesis::Process::MAELastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder]->mainAnimEvent;
}

void nemesis::Process::MAENumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[num]->mainAnimEvent;
}

void nemesis::Process::MAEMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1)
    //{
    //    ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin]
    //    = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum]->mainAnimEvent;
}

void nemesis::Process::MAEFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0]->mainAnimEvent;
}

void nemesis::Process::MAELastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin]
    //    = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder]->mainAnimEvent;
}

void nemesis::Process::MAENumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti].size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //blocks[begin] = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num]->mainAnimEvent;
}

void nemesis::Process::MAESingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = varptr->GetResult(scopeinfo);
}

void nemesis::Process::AnimVarPtr(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = varptr->GetResult(scopeinfo);
}

void nemesis::Process::AddOnMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());

    //ClearBlocks(blocks);

    //VecStr* list = &scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum]
    //                    ->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum]
    //                        ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //ClearBlocks(blocks);

    //VecStr* list
    //    = &scopeinfo.curAnim->GetGroupAnimInfo()[0]->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output
    //        = scopeinfo.curAnim->GetGroupAnimInfo()[0]->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //int curorder = scopeinfo.order;

    //if (!scopeinfo.curAnim->isLast()) ++curorder;

    //VecStr* list
    //    = &scopeinfo.curAnim->GetGroupAnimInfo()[curorder]->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output
    //        = scopeinfo.curAnim->GetGroupAnimInfo()[curorder]->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //int curorder = scopeinfo.order;

    //if (scopeinfo.order > 0) --curorder;

    //VecStr* list
    //    = &scopeinfo.curAnim->GetGroupAnimInfo()[curorder]->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output
    //        = scopeinfo.curAnim->GetGroupAnimInfo()[curorder]->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    //VecStr* list = &scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder]
    //                    ->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    if (scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder]
    //            ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]]
    //            .empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder]
    //                        ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //VecStr* list
    //    = &scopeinfo.curAnim->GetGroupAnimInfo()[num]->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output
    //        = scopeinfo.curAnim->GetGroupAnimInfo()[num]->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.animnum == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //}

    //ClearBlocks(blocks);

    //VecStr* list = &scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum]
    //                    ->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum]
    //                        ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //}

    //ClearBlocks(blocks);

    //VecStr* list = &scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0]
    //                    ->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0]
    //                        ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //}

    //ClearBlocks(blocks);

    //VecStr* list = &scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder]
    //                    ->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder]
    //                        ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    //size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti].size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    //ClearBlocks(blocks);

    //VecStr* list = &scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num]
    //                    ->groupAddition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //if (list->empty() || scopeinfo.optionMulti == -1)
    //{
    //    string output = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num]
    //                        ->addition[fixedHkxVariableList[0]][fixedHkxVariableList[1]];

    //    if (output.empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
    //    }

    //    blocks[begin] = output;
    //}
    //else if (int(list->size()) > scopeinfo.optionMulti)
    //{
    //    if ((*list)[scopeinfo.optionMulti].empty())
    //    {
    //        ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
    //    }

    //    blocks[begin] = (*list)[scopeinfo.optionMulti];
    //}
    //else
    //{
    //    ErrorMessage(1141);
    //}
}

void nemesis::Process::AddOnSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);
    blocks[begin] = varptr->GetResult(scopeinfo);
    blocks[fixedvarintlist.front()] = "";
    blocks[fixedvarintlist.back()]  = "";

    /*auto grpadd = scopeinfo.groupAddition.find(fixedHkxVariableList[0]);

    if (scopeinfo.optionMulti == -1 || grpadd == scopeinfo.groupAddition.end()
        || grpadd->second.find(fixedHkxVariableList[1]) == grpadd->second.end()
        || grpadd->second.at(fixedHkxVariableList[1]).empty())
    {
        string_view sv;

        try
        {
            sv = scopeinfo.addition.at(fixedHkxVariableList[0]).at(fixedHkxVariableList[1]);
        }
        catch (const std::exception&)
        {
            ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
        }

        if (sv.empty())
        {
            ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[2]);
        }

        blocks[begin] = sv.data();
    }
    else
    {
        const VecStr& addRef = grpadd->second.at(fixedHkxVariableList[1]);

        if (static_cast<int>(addRef.size()) > scopeinfo.optionMulti)
        {
            string_view sv = addRef[scopeinfo.optionMulti];

            if (sv.empty())
            {
                ErrorMessage(1117, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), *scopeinfo.line);
            }

            blocks[begin] = sv.data();
        }
        else
        {
            ErrorMessage(1141);
        }
    }
    */
}

void nemesis::Process::LastState(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    ClearBlocks(blocks);

    size_t ID = 0;

    //if (scopeinfo.fixedStateID.size() > 1)
    //{
    //    if (!fixedHkxVariableList[0].empty())
    //    {
    //        ID = stoi(fixedHkxVariableList[0]) - 1;

    //        if (ID >= scopeinfo.fixedStateID.size())
    //        {
    //            ErrorMessage(
    //                1168, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), "LastState" + fixedHkxVariableList[0]);
    //        }
    //    }
    //}

    //blocks[begin] = to_string(scopeinfo.lastorder - scopeinfo.order + scopeinfo.fixedStateID[ID]);
}

void nemesis::Process::HkxEvent(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string eventname = CombineBlocks(fixedvarintlist[0], fixedvarintlist[1], blocks);

    ClearBlocks(blocks);

    auto& HkxEvent              = scopeinfo.GetHkxEvent(eventname);
    blocks[begin]              = std::to_string(HkxEvent.GetId());
    blocks[fixedvarintlist[2]] = "";
    blocks[fixedvarintlist[1]] = "";
}

void nemesis::Process::VariableID(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string variablename = CombineBlocks(fixedvarintlist[0], fixedvarintlist[1], blocks);

    ClearBlocks(blocks);

    auto& HkxVariable                = scopeinfo.GetVariableID(variablename);
    blocks[begin]              = std::to_string(HkxVariable.GetId());
    blocks[fixedvarintlist[2]] = "";
    blocks[fixedvarintlist[1]] = "";
}

void nemesis::Process::Crc32(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string crc32line = nemesis::to_lower_copy(CombineBlocks(blocks));

    ClearBlocks(blocks);

    blocks[begin] = to_string(CRC32Convert(crc32line));
}

void nemesis::Process::Import(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string import = CombineBlocks(blocks);

    ClearBlocks(blocks);

    /*
    auto itr = scopeinfo.IDExist.find(import);

    if (itr != scopeinfo.IDExist.end())
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
            ? ErrorMessage(1139, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), import)
            : keyword = keyword.substr(0, keyword.length() - 4);
    }

    Lockless lock(scopeinfo.animLock->exportLock);

    try
    {
        tempID = scopeinfo.newImport->at(file).at(keyword);
    }
    catch (const std::exception&)
    {
    }

    //if (tempID.empty())
    //{
    //    tempID = (isMaster || isGroup ? scopeinfo.curGroup->newID() : scopeinfo.curAnim->newID());
    //    scopeinfo.IDExist[import]             = tempID;
    //    (*scopeinfo.newImport)[file][keyword] = tempID;
    //}

    blocks[begin] = tempID;*/
}

void nemesis::Process::MotionDataMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);

    //if (scopeinfo.animnum == -1)
    //{
    //    ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //}

    //ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);
    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[0];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);
    ClearBlocks(blocks);

    //int nextorder;
    //scopeinfo.curAnim->isLast() ? nextorder = scopeinfo.order : nextorder = scopeinfo.order + 1;
    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[nextorder];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);
    ClearBlocks(blocks);

    //int previousorder;
    //scopeinfo.order == 0 ? previousorder = scopeinfo.order : previousorder = scopeinfo.order - 1;
    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[previousorder];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);
    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);
    size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[num];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);
    //if (scopeinfo.animnum == -1)
    //{
    //    ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo
    //    = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo
    //    = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti].size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::MotionDataSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    MotionValidation(scopeinfo);

    if (GetBehaviorFile() == fixedHkxVariableList[0])
    {
        ErrorMessage(1134, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    }

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order];
    string motionData;

    //if (curInfo->motionData.empty())
    //{
    //    WarningMessage(1018, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    motionData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    motionData = to_string(curInfo->motionData.size()) + "\n";

    //    for (auto& motion : curInfo->motionData)
    //    {
    //        motionData.append(motion + "\n");
    //    }
    //}

    blocks[begin] = motionData;
}

void nemesis::Process::RotationDataMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    //if (scopeinfo.animnum == -1) ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.animnum];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[0];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    ClearBlocks(blocks);

    //int nextorder;
    //scopeinfo.curAnim->isLast() ? nextorder = scopeinfo.order : nextorder = scopeinfo.order + 1;
    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[nextorder];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    ClearBlocks(blocks);

    //int previousorder;
    //scopeinfo.order == 0 ? previousorder = scopeinfo.order : previousorder = scopeinfo.order - 1;
    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[previousorder];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.lastorder];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curAnim->GetGroupAnimInfo().size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[num];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    //if (scopeinfo.animnum == -1)
    //{
    //    ErrorMessage(1146, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //}

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo
    //    = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.animnum];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][0];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo
    //    = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][scopeinfo.lastorder];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    //if (scopeinfo.groupMulti == -1)
    //{
    //    ErrorMessage(1202, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    size_t num = fixedvarintlist[0];

    //if (num >= scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti].size())
    //{
    //    ErrorMessage(1148, GetFormat(), GetBehaviorFile(), GetCurrentLineNum(), fixedHkxVariableList[0]);
    //}

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curGroup->groupAnimInfo[scopeinfo.groupMulti][num];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::RotationDataSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    RotationValidation(scopeinfo);

    if (GetBehaviorFile() == fixedHkxVariableList[0])
    {
        ErrorMessage(1134, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    }

    ClearBlocks(blocks);

    //shared_ptr<AnimationInfo> curInfo = scopeinfo.curAnim->GetGroupAnimInfo()[scopeinfo.order];
    string rotationData;

    //if (curInfo->rotationData.empty())
    //{
    //    WarningMessage(1019, GetFormat(), GetBehaviorFile(), GetCurrentLineNum());
    //    rotationData
    //        = "1\n" + (curInfo->hasDuration ? StrDoubleDecimal(curInfo->duration, 6) : "0") + " 0 0 0 1";
    //}
    //else
    //{
    //    rotationData = to_string(curInfo->rotationData.size()) + "\n";

    //    for (auto& rotation : curInfo->rotationData)
    //    {
    //        rotationData.append(rotation + "\n");
    //    }
    //}

    blocks[begin] = rotationData;
}

void nemesis::Process::AnimOrder(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string animPath = CombineBlocks(fixedvarintlist[0], fixedvarintlist[1], blocks);

    ClearBlocks(blocks);

    //auto& ptr = charAnimDataInfo.find(
    //    nemesis::to_lower_copy(scopeinfo.project.substr(0, scopeinfo.project.rfind(".txt"))));

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

void nemesis::Process::RegisAnim(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string animPath = CombineBlocks(blocks);
    addUsedAnim(GetBehaviorFile().string(), animPath);
}

void nemesis::Process::RegisBehavior(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    string behaviorName = CombineBlocks(blocks);
    nemesis::to_lower(behaviorName);
    behaviorJoints[behaviorName].push_back(nemesis::to_lower_copy(GetBehaviorFile().string()));
}

void nemesis::Process::LocalNegative(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    if (CombineBlocks(blocks)[0] == '-') scopeinfo.negative = true;
}

void nemesis::Process::RemoveProcessSyntax(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const
{
    blocks[fixedvarintlist.front()] = "";
    blocks[fixedvarintlist.back()]  = "";
}
