#pragma once

#include <vector>

#include "utilities/template.h"

struct AnimThreadInfo;

namespace nemesis
{
    struct Process
    {
    private:
        std::weak_ptr<nemesis::TemplateLine> pTemplateLine;

        uint begin;
        uint end;
        VecStr fixedvarlist;
        std::vector<int> fixedvarintlist;

        using funcptr = void (Process::*)(VecStr&, const AnimThreadInfo&) const;
        funcptr pFunction;

        bool ClearBlocks(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        std::string CombineBlocks(VecStr& blocks) const;
        std::string CombineBlocks(uint ibegin, uint iend, VecStr& blocks) const;

        void MotionValidation(const AnimThreadInfo& curAnimInfo) const;
        void RotationValidation(const AnimThreadInfo& curAnimInfo) const;

    public:
        ~Process();

        void AddFunction(funcptr);
        void AddTemplateLinePtr(std::shared_ptr<nemesis::TemplateLine> pTemplateLine);
        void SetBegin(uint begin);
        void SetEnd(uint end);
        void SetFixedVar(const VecStr& fixedvarlist);
        void SetFixedVarInt(const std::vector<int>& fixedvarintlist);

        const uint& GetSize() const noexcept;
        const uint& GetBegin() const noexcept;
        const uint& GetEnd() const noexcept;
        const std::string& GetFormat() const;
        const std::filesystem::path& GetBehaviorFile() const;

        // processes
        void RelativeNegative(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void Compute(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RangeCompute(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void UpCounter(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void UpCounterPlus(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AnimCount(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MultiChoiceRegis(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void GroupIDRegis(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void IDRegis(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void IDRegisAnim(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void IDRegisGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void IDRegisMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void Computation(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // end functions
        // group
        void EndMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void EndMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void EndNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void EndSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // state functions
        // group
        void StateMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master to group
        void StateMultiMasterToGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void StateMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void StateNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void StateSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // file path functions
        // group
        void FilepathMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void FilepathMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilepathNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void FilepathSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // filename functions
        // group
        void FilenameMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void FilenameMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void FilenameNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void FilenameSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // path functions
        void PathSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // AnimObject functions
        // group
        void AOMultiGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOMultiGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOFirstGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOFirstGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AONextGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AONextGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOBackGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOBackGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOLastGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOLastGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AONumGroupA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AONumGroupB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void AOMultiMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOMultiMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOFirstMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOFirstMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOLastMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOLastMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AONumMasterA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AONumMasterB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void AOSingleA(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AOSingleB(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // main_anim_event functions
        // group
        void MAEMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAEFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAENextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAEBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAELastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAENumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        //master
        void MAEMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAEFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAELastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MAENumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void MAESingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // addOn functions
        // group
        void AddOnMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void AddOnMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void AddOnNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void AddOnSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // last state functions
        void LastState(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // event ID functions
        void EventID(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // variable ID functions
        void VariableID(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // crc32 functions
        void Crc32(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // import functions
        void Import(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // motion data functions
        // group
        void MotionDataMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void MotionDataMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void MotionDataNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void MotionDataSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // rotation data functions
        // group
        void RotationDataMultiGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataFirstGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataNextGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataBackGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataLastGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataNumGroup(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // master
        void RotationDataMultiMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataFirstMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataLastMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
        void RotationDataNumMaster(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        void RotationDataSingle(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // animOrder functions
        void AnimOrder(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // register animation
        void RegisAnim(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // register behavior
        void RegisBehavior(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;

        // negative local
        void LocalNegative(VecStr& blocks, const AnimThreadInfo& curAnimInfo) const;
    };
}
