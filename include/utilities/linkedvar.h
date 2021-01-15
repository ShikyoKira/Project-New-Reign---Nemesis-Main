#pragma once

#include "utilities/line.h"
#include "utilities/conditions.h"
#include "utilities/conditioninfo.h"
#include "utilities/conditiondetails.h"

namespace nemesis
{
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
		Vec<CondVar<Type>> nestedcond;

		LinkedVar() : preCompile(true), hasProcess(false), linecount(0)
        {
		}

		LinkedVar(const Type& _raw) : preCompile(true), hasProcess(false)
        {
            if constexpr (std::is_same_v<Type, nemesis::Line>)
            {
                auto& line = static_cast<nemesis::Line>(_raw);
                raw        = std::make_shared<nemesis::Line>(line);
                linecount  = line.GetLineNumber();
            }
            else
            {
                raw = std::make_shared<Type>(_raw);
                linecount = 0;
            }
		}

		LinkedVar(const Type& _raw, size_t num) : preCompile(true), hasProcess(false), linecount(num)
		{
            raw = std::make_shared<Type>(_raw);
		}

		LinkedVar(const Type& _raw, bool process, size_t num) : preCompile(true), hasProcess(process), linecount(num)
		{
			if (process)
			{
				lineblocks.reserve(_raw.length());

                for (auto &ch : _raw) {
                    lineblocks.emplace_back(std::string(1, ch));
                }
            }
			else
            {
                raw = std::make_shared<Type>(_raw);
			}
		}

		LinkedVar(const CondVar<Type>& cond) : preCompile(true), hasProcess(false), linecount(0)
		{
			nestedcond.emplace_back(cond);
		}

		LinkedVar(const CondVar<Type>& cond, size_t num) : preCompile(true), hasProcess(false), linecount(num)
		{
			nestedcond.emplace_back(cond);
		}

		CondVar<Type>& addCond()
		{
			nestedcond.emplace_back(nemesis::CondVar<Type>());
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const CondType& type)
		{
            nestedcond.emplace_back(nemesis::CondVar<Type>(type));
			return nestedcond.back();
		}

        CondVar<Type>& addCond(const std::string& condition, const CondType& type)
        {
            nestedcond.emplace_back(nemesis::CondVar<Type>(condition, type));
            return nestedcond.back();
        }

		CondVar<Type>& addCond(const CondVar<Type>& cond)
		{
			nestedcond.emplace_back(cond);
			return nestedcond.back();
		}

        CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw,
                               const nemesis::CondDetails& conditions,
                               size_t linecount = 0)
        {
            nestedcond.emplace_back(nemesis::CondVar<Type>(nestraw, conditions));
            nestedcond.back().rawlist.back().linecount = linecount;
            return nestedcond.back();
        }

        CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw,
                               const std::string& conditions,
                               const CondType& type,
                               size_t linecount = 0)
        {
            nestedcond.emplace_back(nemesis::CondVar<Type>(nestraw, conditions, type));
            nestedcond.back().rawlist.back().linecount = linecount;
            return nestedcond.back();
        }

		CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw)
		{
			nestedcond.emplace_back(nestraw);
			return nestedcond.back();
		}

		CondVar<Type>& backCond()
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
            if (preCompile)
                return data() == _raw;

            std::string l;

            for (const auto& c : lineblocks) {
                l.append(c);
            }

            return l == _raw;
		}

		bool operator!=(const Type& _raw)
		{
            if (preCompile)
                return data() != _raw;

            std::string l;

            for (const auto& c : lineblocks) {
                l.append(c);
            }

            return l != _raw;
		}

        inline bool LinkedVar<Type>::HasCondition() const
        {
            return nestedcond.empty();
        }
	};

	template<typename Type>
	struct CondVar
	{
		CondType conditionType;
		bool isMulti = false;
		bool isOrder = false;
		size_t curnum = 0;
		size_t linenum;
		std::string conditions;
        SPtr<Condt> next;
		Vec<LinkedVar<Type>> rawlist;

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

		CondVar(const LinkedVar<Type>& _raw)
		{
			rawlist.emplace_back(_raw);
        }

        CondVar(const LinkedVar<Type>& _raw, const nemesis::CondDetails& _condition)
        {
            rawlist.emplace_back(_raw);
            conditions    = _condition.condition;
            conditionType = _condition.type;
        }

        CondVar(const LinkedVar<Type>& _raw, const nemesis::ConditionInfo& conditioninfo)
        {
            rawlist.emplace_back(_raw);
            conditions    = conditioninfo.GetCondition();
            conditionType = conditioninfo.GetType();
        }

		CondVar(const LinkedVar<Type>& _raw, std::string _conditions, nemesis::CondType condtype)
		{
			rawlist.emplace_back(_raw);
			conditions = _conditions;
			conditionType = condtype;
		}

		CondVar(const LinkedVar<Type>& _raw, std::string_view _conditions, nemesis::CondType condtype)
		{
			rawlist.emplace_back(_raw);
			conditions = _conditions;
			conditionType = condtype;
		}
	};

    using LinkedString  = nemesis::LinkedVar<std::string>;
    using LinkedWstring = nemesis::LinkedVar<std::wstring>;
    using LinkedNString = nemesis::LinkedVar<nemesis::Line>;

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
