#pragma once

#include "utilities/regex.h"

#include "core/animvarptr.h"

namespace nemesis
{
    struct ScopeInfo;
    struct TemplateCategory;
    struct PreprocessLine;

    struct Process
    {
    private:
        nemesis::PreprocessLine* plinkedline = nullptr;

        size_t begin;
        size_t end;
        VecStr fixedHkxVariableList;
        Vec<int> fixedvarintlist;
        UPtr<nemesis::AnimVarPtr> varptr;

        using funcptr = void (Process::*)(VecStr&, nemesis::ScopeInfo&) const;
        funcptr pfunction;

        void ClearBlocks(VecStr& blocks) const;
        void ClearBlocks(size_t ibegin, size_t iend, VecStr& blocks) const;
        std::string CombineBlocks(VecStr& blocks) const;
        std::string CombineBlocks(size_t ibegin, size_t iend, VecStr& blocks) const;

        void MotionValidation(nemesis::ScopeInfo& scopeinfo) const;
        void RotationValidation(nemesis::ScopeInfo& scopeinfo) const;

        std::string IDRegisByType(nemesis::ScopeInfo& scopeinfo, nemesis::File::FileType filetype) const;

        void ExeFirstAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const;
        void ExeNextAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const;
        void ExeBackAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const;
        void ExeLastAnimFromScope(VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const;
        void ExeNumAnimFromScope(size_t num, VecStr& blocks, nemesis::ScopeInfo& scopeinfo, funcptr func) const;

    public:
        ~Process();

        void AddFunction(funcptr pfunction) noexcept;
        void AddProcessLinePtr(nemesis::PreprocessLine* linkedline) noexcept;
        void AddAnimVarPtr(UPtr<nemesis::AnimVarPtr> animvarptr) noexcept;
        void SetBegin(size_t begin) noexcept;
        void SetEnd(size_t end) noexcept;
        void SetFixedVar(const VecStr& fixedHkxVariableList) noexcept;
        void SetFixedVarInt(const Vec<int>& fixedvarintlist) noexcept;

        void Compile(VecStr& blocks, nemesis::ScopeInfo& scopeinfo);
        
        bool IsInFailedRange(nemesis::ScopeInfo& scopeinfo) const;
        void ClearMultiChoice(nemesis::ScopeInfo& scopeinfo) const;

        size_t GetSize() const noexcept;
        size_t GetBegin() const noexcept;
        size_t GetEnd() const noexcept;
        size_t GetCurrentLineNum() const noexcept;
        std::string_view GetFormat() const;
        std::filesystem::path GetBehaviorFile() const;
        const nemesis::TemplateCategory* GetTemplateCategory() const;

        // processes
        void RelativeNegative(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void Compute(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RangeCompute(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void UpCounter(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void UpCounterPlus(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AnimCount(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void MultiChoicePre(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MultiChoicePost(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void GroupIDRegis(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void IDRegis(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void IDRegisAnim(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void IDRegisGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void IDRegisMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void Computation(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void ImportNode(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void ImportIndex(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // end functions
        // group
        void EndMulti(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndFirst(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndNext(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndBack(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndLast(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndNum(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        /*
        // master
        void EndMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void EndNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        */

        void EndSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void End(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // state functions
        // group
        void StateMulti(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateFirst(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateNext(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateBack(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateLast(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateNum(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // master to group
        void StateMultiMasterToGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        /*
        // master
        void StateMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void StateNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        */

        void StateSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // file path functions
        // group
        void FilepathMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        /*
        // master
        void FilepathMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilepathNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        */

        void FilepathSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // filename functions
        // group
        void FilenameMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        /*
        // master
        void FilenameMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void FilenameNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        */

        void FilenameSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // path functions
        void PathSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // AnimObject functions
        // group
        void AOMulti(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOMultiTarget(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOFirstGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOFirstGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AONextGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AONextGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOBackGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOBackGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOLastGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOLastGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AONumGroupA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AONumGroupB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // master
        void AOMultiMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOMultiMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOFirstMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOFirstMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOLastMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOLastMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AONumMasterA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AONumMasterB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void AOSingleA(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AOSingleB(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void AOSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // main_anim_event functions
        // group
        void MAEMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAEFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAENextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAEBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAELastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAENumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        //master
        void MAEMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAEFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAELastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MAENumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void MAESingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // anim variable functions
        void AnimVarPtr(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // addOn functions
        // group
        void AddOnMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // master
        void AddOnMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void AddOnNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void AddOnSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // last state functions
        void LastState(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // event ID functions
        void HkxEvent(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // variable ID functions
        void VariableID(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // crc32 functions
        void Crc32(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // import functions
        void Import(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // motion data functions
        // group
        void MotionDataMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // master
        void MotionDataMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void MotionDataNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void MotionDataSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // rotation data functions
        // group
        void RotationDataMultiGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataFirstGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataNextGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataBackGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataLastGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataNumGroup(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // master
        void RotationDataMultiMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataFirstMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataLastMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;
        void RotationDataNumMaster(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void RotationDataSingle(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // animOrder functions
        void AnimOrder(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // register animation
        void RegisAnim(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // register behavior
        void RegisBehavior(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        // negative local
        void LocalNegative(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

        void RemoveProcessSyntax(VecStr& blocks, nemesis::ScopeInfo& scopeinfo) const;

    private:
        const static nemesis::regex elementrgx;
    };
}
