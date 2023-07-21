#pragma once

#include "base/file.h"

#include "utilities/types.h"
#include "utilities/condtype.h"
#include "utilities/conditioninfo.h"

#include "core/condition.h"

#include "scope/scopeinfo.h"

namespace nemesis
{
    template <typename _Ty, typename _LnkTy>
    struct LinkedCond : std::enable_shared_from_this<LinkedCond<_Ty, _LnkTy>>
    {
    private:
        using LinkedFunc = void (_LnkTy::*)(VecNstr&) const;

    protected:
        SPtr<nemesis::Condition> condition;
        Vec<SPtr<_LnkTy>> datalist;

    public:
        LinkedCond(SPtr<nemesis::Condition> condition)
            : condition(condition)
        {
        }
        
        LinkedCond(const nemesis::ConditionInfo& conditioninfo, const nemesis::File& file)
        {
            condition = std::make_shared<nemesis::Condition>(
                conditioninfo.GetCondition(), conditioninfo.GetRefLine(), file, condition->GetType());
        }

        std::string GetExpression() const
        {
            return condition->GetExpression();
        }

        CondType GetType() const
        {
            return condition->GetType();
        }

        nemesis::Condition* GetCondition() const
        {
            return condition.get();
        }

        Vec<SPtr<_LnkTy>>& GetDataList()
        {
            return datalist;
        }

        bool IsTrue(nemesis::ScopeInfo& scopeinfo) const
        {
            if (!condition) return true;

            return condition->IsTrue(scopeinfo);
        }

        VecNstr CompileLines(nemesis::ScopeInfo& scopeinfo) const
        {
        }

        Vec<const _Ty*> CompileDataList(nemesis::ScopeInfo& scopeinfo) const
        {
            Vec<const _Ty*> list;

            for (const SPtr<_LnkTy>& data : datalist)
            {
                Vec<const _Ty*> templist = data->GetCompiledData(scopeinfo);
                list.insert(list.end(), templist.begin(), templist.end());
            }

            return list;
        }

        template <typename _SpcTy>
        Vec<_SpcTy> CompileDataList(nemesis::ScopeInfo& scopeinfo) const
        {
            Vec<_SpcTy> list;

            for (const SPtr<_LnkTy>& data : datalist)
            {
                Vec<_SpcTy> templist = data->GetCompiledData<_SpcTy>(scopeinfo);
                list.insert(list.end(), templist.begin(), templist.end());
            }

            return list;
        }

        void GetRawDataList(VecNstr& rawdata, LinkedFunc get_rawdata) const
        {
            for (const SPtr<_LnkTy>& data : datalist)
            {
                //(data->*get_rawdata)(rawdata);
            }
        }
    };
}
