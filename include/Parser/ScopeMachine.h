#pragma once

#include "utilities/types.h"
#include "utilities/conditioninfo.h"
#include "utilities/conditionscope.h"

#include "Parser/ParsableObject.h"

namespace nemesis
{
    template<typename _ParObjTy, typename _Itr>
	struct ScopeMachine
    {
        struct Scope
        {
        private:
            SPtr<ConditionInfo> conditioninfo;
            Vec<_Itr> vec_itr;

            void AddItrLines(_Itr itr)
            {
                vec_itr.emplace_back(itr);
            }

        public:
            Scope(SPtr<ConditionInfo> conditioninfo)
                : conditioninfo(conditioninfo)
            {
            }

            ConditionInfo& GetCondition()
            {
                return *conditioninfo;
            }

            const ConditionInfo& GetCondition()
            {
                return *conditioninfo;
            }

            _ParObjTy ToObject()
            {
                _ParObjTy par_obj;

                for (auto& itr : vec_itr)
                {
                    par_obj.AppendLine(*itr);
                }

                return par_obj;
            }

            Vec<nemesis::Line> ToLines()
            {
                Vec<nemesis::Line> lines;

                for (auto& itr : vec_itr)
                {
                    lines.emplace_back(*itr);
                }

                return lines;
            }

            friend nemesis::ScopeMachine<_ParObjTy, _Itr>;
        };

    private:
        bool success;
        Vec<int> original_indexes;
        Vec<Scope> scopes;

        ScopeMachine(_Itr itr, const std::string_view& modcode = "Base")
        {
            ConditionScope cscope(modcode, "");
            auto condinfo = cscope.TryGetConditionInfo(*itr);
            
            if (condinfo->GetType() == nemesis::CondType::NONE)
            {
                success = false;
                return;
            }

            success = true;

            while (!cscope.Empty())
            {

            }
        }

        bool IsSuccess() const noexcept
        {
            return success;
        }

        size_t Size() const  noexcept
        {
            return scopes.size();
        }

        nemesis::ScopeMachine::Scope ScopeAt(size_t index)
        {
            return scopes.at(index);
        }

        const nemesis::ScopeMachine::Scope ScopeAt(size_t index) const
        {
            return scopes.at(index);
        }
        
        static_assert(std::_Is_iterator_v<_Itr>, "Non-iterator type is passed to _Itr");
        static_assert(std::is_base_of_v<ParsableObject, _ParObjTy>, "Non-ParsableObject is passed to _ParObjTy");
    };
}
