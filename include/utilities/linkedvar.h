#pragma once

#include "core/condition.h"

#include "utilities/line.h"
#include "utilities/conditions.h"
#include "utilities/conditioninfo.h"
#include "utilities/conditiondetails.h"

#include "scope/scopeinfo.h"

namespace nemesis
{
    struct ScopeInfo;
    struct CondDetails;

	template<typename Type>
	struct CondVar;

	template<typename Type>
	struct LinkedVar
	{
		bool preCompile;
		bool hasProcess;
		size_t linecount;
        SPtr<Type> raw;
        VecStr lineblocks;

        Vec<nemesis::CondVar<Type>> nestedcond;

		LinkedVar() : preCompile(true), hasProcess(false), linecount(0)
        {
		}

		LinkedVar(const Type& _raw) : preCompile(true), hasProcess(false)
        {
            if constexpr (std::is_same_v<Type, nemesis::Line>)
            {
                raw       = std::make_shared<nemesis::Line>(_raw);
                linecount = _raw.GetLineNumber();
                return;
            }

            raw       = std::make_shared<Type>(_raw);
            linecount = 0;
		}

		LinkedVar(const Type& _raw, size_t num) : preCompile(true), hasProcess(false), linecount(num)
		{
            raw = std::make_shared<Type>(_raw);
		}

		LinkedVar(const Type& _raw, bool process, size_t num) : preCompile(true), hasProcess(process), linecount(num)
		{
			if (!process)
            {
                raw = std::make_shared<Type>(_raw);
                return;
            }

            lineblocks.reserve(_raw.length());

            for (auto& ch : _raw)
            {
                lineblocks.emplace_back(std::string(1, ch));
            }
		}

		LinkedVar(const nemesis::CondVar<Type>& cond)
            : preCompile(true)
            , hasProcess(false)
            , linecount(0)
		{
			nestedcond.emplace_back(cond);
		}

		LinkedVar(const nemesis::CondVar<Type>& cond, size_t num)
            : preCompile(true)
            , hasProcess(false)
            , linecount(num)
		{
			nestedcond.emplace_back(cond);
		}

        LinkedVar(const nemesis::LinkedVar<Type>& linkedvalue) = default;

		nemesis::CondVar<Type>& addCond()
		{
			nestedcond.emplace_back(nemesis::CondVar<Type>());
			return nestedcond.back();
		}

		nemesis::CondVar<Type>& addCond(const CondType& type)
		{
            nestedcond.emplace_back(nemesis::CondVar<Type>(type));
			return nestedcond.back();
		}

        nemesis::CondVar<Type>& addCond(const std::string& condition, const CondType& type)
        {
            nestedcond.emplace_back(nemesis::CondVar<Type>(condition, type));
            return nestedcond.back();
        }

		nemesis::CondVar<Type>& addCond(const CondVar<Type>& cond)
		{
			nestedcond.emplace_back(cond);
			return nestedcond.back();
		}

        nemesis::CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw,
                               const nemesis::CondDetails& conditions,
                               size_t linecount = 0)
        {
            nestedcond.emplace_back(nemesis::CondVar<Type>(nestraw, conditions));
            nestedcond.back().rawlist.back().linecount = linecount;
            return nestedcond.back();
        }

        nemesis::CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw,
                               const std::string& conditions,
                               const CondType& type,
                               size_t linecount = 0)
        {
            nestedcond.emplace_back(nemesis::CondVar<Type>(nestraw, conditions, type));
            nestedcond.back().rawlist.back().linecount = linecount;
            return nestedcond.back();
        }

		nemesis::CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw)
		{
			nestedcond.emplace_back(nestraw);
			return nestedcond.back();
		}

		nemesis::CondVar<Type>& backCond()
		{
			return nestedcond.back();
		}

		std::string data()
        {
            if (!raw) {
                std::string l;

                for (const auto& c : lineblocks) {
                    l.append(c);
                }

                return l;
            }

            return *raw;
        }

        SPtr<Type> operator=(const Type& _raw)
		{
            return raw = std::make_shared<Type>(_raw);
		}

		SPtr<Type> operator+(const Type& _raw)
		{
            return std::make_shared<Type>(*raw + _raw);
		}

		bool operator==(const Type& _raw)
		{
            if (preCompile) return data() == _raw;
                
            std::string l;

            for (const auto& c : lineblocks) {
                l.append(c);
            }

            return l == _raw;
		}

		bool operator!=(const Type& _raw)
		{
            if (preCompile) return data() != _raw;

            std::string l;

            for (const auto& c : lineblocks) {
                l.append(c);
            }

            return l != _raw;
		}

        inline bool LinkedVar<Type>::HasCondition() const
        {
            return !nestedcond.empty();
        }
        
        virtual bool LinkedVar<Type>::HasProcess() const
        {
            return !lineblocks.empty();
        }

        virtual Type Process(nemesis::ScopeInfo& scopeinfo) const
        {
            return *raw;
        }
        
        template<typename _Ty>
        _Ty Process(nemesis::ScopeInfo& scopeinfo) const
        {
            return static_cast<_Ty>(Process(scopeinfo));
        }

        virtual Vec<Type> CompileToList(nemesis::ScopeInfo& scopeinfo) const
        {
            if (!HasCondition()) return Vec<Type>{Process(scopeinfo)};

            for (const nemesis::CondVar<Type>& cond : nestedcond)
            {
                switch (cond.conditionType)
                {
                    case nemesis::CondType::FOREACH:
                    {
                        Vec<Type> list;

                        for (auto cur = scopeinfo.ExeQuery(*cond.condition); !cur || !cur->IsEnd();
                             cur->Next())
                        {
                            for (auto& innerline : cond.rawlist)
                            {
                                Vec<Type> nested = innerline.CompileToList(scopeinfo);
                                list.insert(list.end(), nested.begin(), nested.end());
                            }
                        }

                        return list;
                    }
                    default:
                    {
                        if (!cond.IsTrue(scopeinfo)) break;

                        return cond.CompileList(scopeinfo);
                    }
                }
            }

            return Vec<Type>();
        }

        template<typename _Ty>
        Vec<_Ty> CompileToList(nemesis::ScopeInfo& scopeinfo) const
        {
            if (!HasCondition()) return Vec<_Ty>{Process<_Ty>(scopeinfo)};

            for (const nemesis::CondVar<Type>& cond : nestedcond)
            {
                switch (cond.conditionType)
                {
                    case nemesis::CondType::FOREACH:
                    {
                        Vec<_Ty> list;

                        for (auto cur = scopeinfo.ExeQuery(*cond.condition); !cur || !cur->IsEnd();
                             cur->Next())
                        {
                            for (auto& innerline : cond.rawlist)
                            {
                                Vec<_Ty> nested = innerline.CompileToList<_Ty>(scopeinfo);
                                list.insert(list.end(), nested.begin(), nested.end());
                            }
                        }

                        return list;
                    }
                    default:
                    {
                        if (!cond.IsTrue(scopeinfo)) break;

                        return cond.CompileList<_Ty>(scopeinfo);
                    }
                }
            }

            return Vec<_Ty>();
        }

        size_t GetLineNum() const noexcept
        {
            return linecount;
        }

        Type* GetRaw()
        {
            return raw.get();
        }
	};

	template<typename Type>
	struct CondVar
	{
        nemesis::CondType conditionType;
		bool isMulti = false;
		bool isOrder = false;
		size_t curnum = 0;
		size_t linenum;
		std::string conditions;
        SPtr<nemesis::Condition> condition;
        SPtr<nemesis::Condt> next;
		Vec<nemesis::LinkedVar<Type>> rawlist;

    public:
		CondVar() = default;

        CondVar(nemesis::CondType condtype)
        {
            conditionType = condtype;
        }

        CondVar(const nemesis::ConditionInfo& _condition)
        {
            conditions    = _condition.GetCondition();
            conditionType = _condition.GetType();
        }

        CondVar(const nemesis::CondDetails& _condition)
        {
            conditions    = _condition.condition;
            conditionType = _condition.type;
        }

        CondVar(const std::string& _conditions, nemesis::CondType condtype)
        {
            conditions    = _conditions;
            conditionType = condtype;
        }

        CondVar(const char* _conditions, nemesis::CondType condtype)
        {
            conditions    = _conditions;
            conditionType = condtype;
        }

        CondVar(const Type& _raw)
        {
            rawlist.emplace_back(_raw);
        }

        CondVar(const Type& _raw, const nemesis::CondDetails& _condition)
        {
            rawlist.emplace_back(_raw);
            conditions    = _condition.condition;
            conditionType = _condition.type;
        }

        CondVar(const Type& _raw, const nemesis::ConditionInfo& conditioninfo)
        {
            rawlist.emplace_back(_raw);
            conditions    = conditioninfo.GetCondition();
            conditionType = conditioninfo.GetType();
        }

        CondVar(const Type& _raw, std::string _conditions, nemesis::CondType condtype)
        {
            rawlist.emplace_back(_raw);
            conditions    = _conditions;
            conditionType = condtype;
        }

        CondVar(const Type& _raw, std::string_view _conditions, nemesis::CondType condtype)
        {
            rawlist.emplace_back(_raw);
            conditions    = std::string(_conditions);
            conditionType = condtype;
        }

		CondVar(const nemesis::LinkedVar<Type>& _raw)
		{
			rawlist.emplace_back(_raw);
        }

        CondVar(const nemesis::LinkedVar<Type>& _raw, const nemesis::CondDetails& _condition)
        {
            rawlist.emplace_back(_raw);
            conditions    = _condition.condition;
            conditionType = _condition.type;
        }

        CondVar(const nemesis::LinkedVar<Type>& _raw, const nemesis::ConditionInfo& conditioninfo)
        {
            rawlist.emplace_back(_raw);
            conditions    = conditioninfo.GetCondition();
            conditionType = conditioninfo.GetType();
        }

		CondVar(const nemesis::LinkedVar<Type>& _raw, std::string _conditions, nemesis::CondType condtype)
		{
            rawlist.emplace_back(_raw);
            conditions    = _conditions;
            conditionType = condtype;
		}

		CondVar(const nemesis::LinkedVar<Type>& _raw,
                std::string_view _conditions,
                nemesis::CondType condtype)
		{
            rawlist.emplace_back(_raw);
            conditions    = _conditions;
            conditionType = condtype;
		}
        
		CondVar(nemesis::CondType condtype, SPtr<nemesis::Condition> condition_ptr)
		{
            conditions    = condition_ptr->GetExpression();
            conditionType = condtype;
            condition     = condition_ptr;
		}

        bool IsTrue(nemesis::ScopeInfo& scopeinfo) const
        {
            switch (conditionType)
            {
                case nemesis::CondType::IF:
                case nemesis::CondType::ELSEIF:
                    return condition->IsTrue(scopeinfo);
                case nemesis::CondType::ELSE:
                    return true;
                default:
                    ErrorMessage(1205, scopeinfo.GetFormat(), scopeinfo.GetFile(), linenum);
                    return false;
            }
        }

        Vec<Type> CompileList(nemesis::ScopeInfo& scopeinfo) const
        {
            Vec<Type> list;

            for (const nemesis::LinkedVar<Type>& raw : rawlist)
            {
                Vec<Type> templist = raw.CompileToList(scopeinfo);
                list.insert(list.end(), templist.begin(), templist.end());
            }

            return list;
        }

        template<typename _Ty>
        Vec<_Ty> CompileList(nemesis::ScopeInfo& scopeinfo) const
        {
            Vec<_Ty> list;

            for (const nemesis::LinkedVar<Type>& raw : rawlist)
            {
                Vec<_Ty> templist = raw.CompileToList<_Ty>(scopeinfo);
                list.insert(list.end(), templist.begin(), templist.end());
            }

            return list;
        }
	};

    using LinkedString  = nemesis::LinkedVar<std::string>;
    using LinkedWstring = nemesis::LinkedVar<std::wstring>;

    nemesis::LinkedString ToLinkedString(const nemesis::Line& line);
    nemesis::LinkedWstring ToLinkedWstring(const nemesis::Wline& line);
} // namespace nemesis


void combineLines(std::string& last, VecStr& temp, VecStr& combined);
SPtr<VecStr> getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline);
void getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline, VecStr& storeline);
void getLinkedLines(const nemesis::LinkedVar<nemesis::Line>& linkedline, VecNstr& storeline);
void getLinkedLines(Vec<nemesis::LinkedVar<std::string>> linkedlist, VecStr& storeline);

//template <typename _Type>
//void getLinkedLines(const nemesis::LinkedVar<_Type>& linkedtype, VecStr& storeline, bool removeEmpty = false)
//{
//    vector<const nemesis::CondVar<_Type>&> tempstore;
//
//    for (auto& cond : linkedtype.nestedcond)
//    {
//        switch (cond.conditionType)
//        {
//            case nemesis::CondType::MOD_CODE:
//            {
//                tempstore.emplace_back(cond);
//                break;
//            }
//            case nemesis::CondType::FOREACH:
//            {
//                storeline.emplace_back(nemesis::syntax::ForEach(cond.conditions));
//
//                for (auto& each : cond.rawlist)
//                {
//                    getLinkedLines(each, storeline);
//                }
//
//                storeline.emplace_back(nemesis::syntax::Close());
//                break;
//            }
//        }
//    }
//
//    if (tempstore.size() > 0)
//    {
//        if (linkedtype.raw)
//        {
//            std::vector<VecStr> vec2;
//
//            for (auto& modcode : modcodelist)
//            {
//                vec2.emplace_back(VecStr());
//                getLinkedLines(modcode->rawlist[0], vec2.back());
//
//                for (auto& line : vec2.back())
//                {
//                    line.append(nemesis::syntax::Spaces() + nemesis::syntax::Stars(modcode->conditions) + "\n");
//                }
//            }
//
//            vec2.emplace_back(VecStr());
//            linkedtype.raw->getlines(vec2.back());
//
//            for (auto& line : vec2.back())
//            {
//                line.append(nemesis::syntax::Spaces() + nemesis::syntax::LowerOriginal() + "\n");
//            }
//
//            for (int i = 0; i < vec2.back().size(); ++i)
//            {
//                for (int k = 0; k < vec2.size(); ++k)
//                {
//                    storeline.emplace_back(vec2[k][i]);
//                }
//            }
//        }
//        else
//        {
//            for (auto& modcode : modcodelist)
//            {
//                storeline.emplace_back(nemesis::syntax::ModCode(modcode.first));
//
//                for (auto& each : modcode.second)
//                {
//                    func(each->rawlist[0], storeline);
//                }
//
//                storeline.emplace_back(nemesis::syntax::Close());
//            }
//        }
//    }
//    else if (linkedtype.raw)
//    {
//        getLinkedLines(*linkedtype.raw, storeline);
//    }
//    else if (removeEmpty)
//    {
//        storeline.emplace_back(nemesis::syntax::DeleteLine());
//    }
//}
