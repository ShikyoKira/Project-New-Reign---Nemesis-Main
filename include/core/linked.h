#pragma once

#include "utilities/types.h"
#include "utilities/conditionsyntax.h"

#include "core/linkedcond.h"

namespace nemesis
{
    template <typename _Ty, typename _LnkTy>
    struct Linked
    {
    protected:
        using LinkedCondition = nemesis::LinkedCond<_Ty, _LnkTy>;

        SPtr<_Ty> raw;
        Vec<SPtr<LinkedCondition>> conditions;

    public:
        Linked() = default;

        Linked(const _Ty& value)
        {
            raw = std::make_shared<_Ty>(value);
        }

        void SetRaw(const _Ty& value)
        {
            if (raw) std::runtime_error("Cannot set new raw value because current raw value is not nullptr");

            raw = std::make_shared<_Ty>(value);
        }

        void AddCondition(const nemesis::ConditionInfo& conditioninfo, const nemesis::File& file)
        {
            conditions.emplace_back(std::make_shared<LinkedCondition>(conditioninfo, file));
        }
        
        void AddCondition(SPtr<nemesis::Condition> condition, CondType type)
        {
            conditions.emplace_back(std::make_shared<LinkedCondition>(condition, type));
        }
        
        void AddCondition(LinkedCondition& linkedcond)
        {
            try
            {
                conditions.emplace_back(linkedcond.shared_from_this());
            }
            catch (const std::exception&)
            {
                conditions.emplace_back(std::make_shared<LinkedCondition>(linkedcond));
            }
        }
        
        void AddCondition(const LinkedCondition& linkedcond)
        {
            conditions.emplace_back(std::make_shared<LinkedCondition>(linkedcond));
        }

        void RemoveCondition(size_t index)
        {
            conditions.erase(conditions.begin() + index);
        }

        LinkedCondition& GetFrontCondition()
        {
            return *conditions.front();
        }

        LinkedCondition& GetLastCondition()
        {
            return *conditions.back();
        }

        LinkedCondition& GetCondition(size_t index)
        {
            return *conditions[index];
        }

        size_t GetConditionSize() const noexcept
        {
            return conditions.size();
        }

        _Ty* GetRawPtr()
        {
            return raw.get();
        }
        
        const _Ty* GetRawPtr() const
        {
            return raw.get();
        }

        bool HasCondition() const noexcept
        {
            return !conditions.empty();
        }

        virtual const _Ty& CompileCurrentData(nemesis::ScopeInfo& scopeinfo) const
        {
            return *raw;
        }

        template<typename _SpcTy>
        _SpcTy CompileCurrentData(nemesis::ScopeInfo& scopeinfo) const
        {
            return static_cast<_SpcTy>(CompileCurrentData(scopeinfo));
        }

        virtual std::string RawToString() const
        {
            if constexpr (std::is_base_of_v<nemesis::Line, _Ty>) return raw->ToString();

            if constexpr (std::is_base_of_v<std::string, _Ty>) return static_cast<std::string>(*raw);

            throw std::runtime_error("Invalid raw to string return value, use override to get a valid return value");
        }

        Vec<const _Ty*> GetCompiledData(nemesis::ScopeInfo& scopeinfo) const
        {
            if (raw)
            {
                if constexpr (std::is_base_of_v<nemesis::Line, _Ty>)
                {
                    scopeinfo.SetCurrentLine(GetRawPtr());
                }
            }

            if (!HasCondition()) return Vec<const _Ty*>{&CompileCurrentData(scopeinfo)};

            for (auto& condition : conditions)
            {
                scopeinfo.SetCurrentCondition(condition->GetCondition());

                switch (condition->GetType())
                {
                    case nemesis::CondType::FOREACH:
                    {
                        Vec<const _Ty*> list;

                        for (auto cur = scopeinfo.ExeQuery(*condition->GetCondition()); cur && !cur->IsEnd();
                             cur->Next())
                        {
                            Vec<const _Ty*> nested = condition->CompileDataList(scopeinfo);
                            list.insert(list.end(), nested.begin(), nested.end());
                        }

                        return list;
                    }
                    default:
                    {
                        if (!condition->IsTrue(scopeinfo)) break;

                        return condition->CompileDataList(scopeinfo);
                    }
                }
            }

            if (!raw) return Vec<const _Ty*>();

            return Vec<const _Ty*>{&CompileCurrentData(scopeinfo)};
        }

        template <typename _SpcTy>
        Vec<_SpcTy> GetCompiledData(nemesis::ScopeInfo& scopeinfo) const
        {
            if (raw)
            {
                if constexpr (std::is_base_of_v<nemesis::Line, _Ty>)
                {
                    scopeinfo.SetCurrentLine(GetRawPtr());
                }
            }

            if (!HasCondition()) return Vec<_SpcTy>{CompileCurrentData<_SpcTy>(scopeinfo)};

            for (auto& condition : conditions)
            {
                scopeinfo.SetCurrentCondition(condition->GetCondition());

                switch (condition->GetType())
                {
                    case nemesis::CondType::FOREACH:
                    {
                        Vec<_SpcTy> list;

                        for (auto cur = scopeinfo.ExeQuery(*condition->GetCondition()); cur && !cur->IsEnd();
                             cur->Next())
                        {
                            Vec<_SpcTy> nested = condition->CompileDataList<_SpcTy>(scopeinfo);
                            list.insert(list.end(), nested.begin(), nested.end());
                        }

                        return list;
                    }
                    default:
                    {
                        if (!condition->IsTrue(scopeinfo)) break;

                        return condition->CompileDataList<_SpcTy>(scopeinfo);
                    }
                }
            }

            if (!raw) return Vec<_SpcTy>();

            return Vec<_SpcTy>{CompileCurrentData<_SpcTy>(scopeinfo)};
        }

        virtual void SetRawDataTo(VecNstr& lines) const
        {
            lines.emplace_back(RawToString());
        }
        
        virtual void SetNewDataTo(VecNstr& lines, const LinkedCondition& cond) const
        {
            lines.emplace_back(nemesis::syntax::ModCode(cond.GetExpression()));
            cond.GetRawDataList(lines, &_LnkTy::GetRawData);
            lines.emplace_back(nemesis::syntax::Close());
        }
        
        virtual void SetNewConditionedDataTo(VecNstr& lines, const nemesis::Line& newline, const LinkedCondition& cond) const
        {
            lines.emplace_back(newline + nemesis::syntax::Spaces()
                               + nemesis::syntax::Aster(cond.GetExpression()));
        }
        
        virtual void SetRawConditionedDataTo(VecNstr& lines) const
        {
            lines.emplace_back(RawToString() + nemesis::syntax::Spaces() + nemesis::syntax::LowerOriginal());
        }

        virtual void GetRawData(VecNstr& lines) const
        {
            Vec<LinkedCondition*> modlist;

            for (auto& condition : conditions)
            {
                switch (condition->GetType())
                {
                    case nemesis::CondType::MOD_CODE:
                    {
                        modlist.emplace_back(condition.get());
                        break;
                    }
                    case nemesis::CondType::FOREACH:
                    {
                        lines.emplace_back(nemesis::syntax::ForEach(condition->GetExpression()));
                        condition->GetRawDataList(lines, &_LnkTy::GetRawData);
                        lines.emplace_back(nemesis::syntax::Close());
                        break;
                    }
                }
            }

            if (modlist.empty())
            {
                if (!raw) return;

                SetRawDataTo(lines);
                return;
            }

            if (!raw)
            {
                for (LinkedCondition*& cond : modlist)
                {
                    SetNewDataTo(lines, *cond);
                }

                return;
            }

            Vec<VecNstr> mod_list_list;
            mod_list_list.reserve(modlist.size());

            for (auto& cond : modlist)
            {
                mod_list_list.emplace_back();
                auto& mod_lines = mod_list_list.back();
                cond->GetRawDataList(mod_lines, &_LnkTy::GetRawData);
                nemesis::Line mline = mod_lines.empty() ? nemesis::syntax::DeleteLine() : mod_lines.front();
                SetNewConditionedDataTo(lines, mline, *cond);
            }

            SetRawConditionedDataTo(lines);

            for (size_t i = 0; i < mod_list_list.size(); ++i)
            {
                auto& mod_lines = mod_list_list[i];

                if (mod_lines.size() <= 1) continue;

                lines.emplace_back(nemesis::syntax::ModCode(modlist[i]->GetExpression()));
                lines.insert(lines.end(), mod_lines.begin() + 1, mod_lines.end());
                lines.emplace_back(nemesis::syntax::Close());
            }
        }
    };
}
