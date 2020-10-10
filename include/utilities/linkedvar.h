#pragma once

#include "utilities/conditions.h"
#include "utilities/conditiondetails.h"

namespace nemesis
{
	template<typename Type>
	struct CondVar;

	template<typename Type>
	struct LinkedVar
	{
		bool preCompile;
		bool hasProcess;
		size_t linecount;
		std::shared_ptr<Type> raw;
		VecStr lineblocks;
		std::vector<CondVar<Type>> nestedcond;

		LinkedVar() : preCompile(true), hasProcess(false), linecount(0)
        {
		}

		LinkedVar(const Type& _raw) : preCompile(true), hasProcess(false), linecount(0)
		{
            raw = std::make_shared<Type>(_raw);
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
                    lineblocks.push_back(std::string(1, ch));
                }
            }
			else
            {
                raw = std::make_shared<Type>(_raw);
			}
		}

		LinkedVar(const CondVar<Type>& cond) : preCompile(true), hasProcess(false), linecount(0)
		{
			nestedcond.push_back(cond);
		}

		LinkedVar(const CondVar<Type>& cond, size_t num) : preCompile(true), hasProcess(false), linecount(num)
		{
			nestedcond.push_back(cond);
		}

		CondVar<Type>& addCond()
		{
			nestedcond.push_back(nemesis::CondVar<Type>());
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const CondType& type)
		{
			nestedcond.push_back(nemesis::CondVar<Type>());
			nestedcond.back().conditionType = type;
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const CondVar<Type>& cond, size_t linecount = 0)
		{
			nestedcond.push_back(cond);
			return nestedcond.back();
		}

        CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw,
                               const nemesis::CondDetails& conditions,
                               size_t linecount = 0)
        {
            nestedcond.push_back(nemesis::CondVar<Type>(nestraw, conditions));
            nestedcond.back().rawlist.back().linecount = linecount;
            return nestedcond.back();
        }

        CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw,
                               const std::string& conditions,
                               const CondType& type,
                               size_t linecount = 0)
        {
            nestedcond.push_back(nemesis::CondVar<Type>(nestraw, conditions, type));
            nestedcond.back().rawlist.back().linecount = linecount;
            return nestedcond.back();
        }

		CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw)
		{
			nestedcond.push_back(nestraw);
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

                for (auto c : lineblocks) {
                    l.append(c);
                }

                return l;
            }

            return *raw;
        }

        std::shared_ptr<Type> operator=(const Type& _raw)
		{
            return raw = std::make_shared<Type>(_raw);
		}

		std::shared_ptr<Type> operator+(const Type& _raw)
		{
            return std::make_shared<Type>(*raw + _raw);
		}

		bool operator==(const Type& _raw)
		{
            if (preCompile)
                return data() == _raw;

            std::string l;

            for (auto c : lineblocks) {
                l.append(c);
            }

            return l == _raw;
		}

		bool operator!=(const Type& _raw)
		{
            if (preCompile)
                return data() != _raw;

            std::string l;

            for (auto c : lineblocks) {
                l.append(c);
            }

            return l != _raw;
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
		std::shared_ptr<Condt> next;
		std::vector<LinkedVar<Type>> rawlist;

		CondVar() {}

        CondVar(std::string _conditions, nemesis::CondType condtype)
        {
            rawlist.push_back(Type());
            conditions    = _conditions;
            conditionType = condtype;
        }

        CondVar(const Type& _raw)
        {
            rawlist.push_back(_raw);
        }

        CondVar(const Type& _raw, const nemesis::CondDetails& _condition)
        {
            rawlist.push_back(_raw);
            conditions    = _condition.condition;
            conditionType = _condition.type;
        }

        CondVar(const Type& _raw, std::string _conditions, nemesis::CondType condtype)
        {
            rawlist.push_back(_raw);
            conditions    = _conditions;
            conditionType = condtype;
        }

		CondVar(const LinkedVar<Type>& _raw)
		{
			rawlist.push_back(_raw);
        }

        CondVar(const LinkedVar<Type>& _raw, const nemesis::CondDetails& _condition)
        {
            rawlist.push_back(_raw);
            conditions    = _condition.condition;
            conditionType = _condition.type;
        }

		CondVar(const LinkedVar<Type>& _raw, std::string _conditions, nemesis::CondType condtype)
		{
			rawlist.push_back(_raw);
			conditions = _conditions;
			conditionType = condtype;
		}
	};
} // namespace nemesis


void combineLines(std::string& last, VecStr& temp, VecStr& combined);
std::shared_ptr<VecStr> getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline);
void getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline, VecStr& storeline);
void getLinkedLines(std::vector<nemesis::LinkedVar<std::string>> linkedlist, VecStr& storeline);

template <typename _Type>
void getLinkedLines(const nemesis::LinkedVar<_Type>& linkedtype, VecStr& storeline)
{
    unordered_map<string, vector<const nemesis::CondVar<_Type>*>> modcodelist;

    for (auto& cond : linkedtype.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist[cond.conditions].push_back(&cond);
                break;
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline);
                }

                storeline.push_back("<!-- CLOSE -->");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        if (linkedtype.raw)
        {
            std::vector<VecStr> vec2;

            for (auto& modcode : modcodelist)
            {
                vec2.push_back(VecStr());
                getLinkedLines(modcode->rawlist[0], vec2.back());

                for (auto& line : vec2.back())
                {
                    line.append("\t\t\t\t\t<!-- *" + modcode->conditions + "* -->\n");
                }
            }

            vec2.push_back(VecStr());
            linkedtype.raw->getlines(vec2.back());

            for (auto& line : vec2.back())
            {
                line.append("\t\t\t\t\t<!-- original -->\n");
            }

            for (int i = 0; i < vec2.back().size(); ++i)
            {
                for (int k = 0; k < vec2.size(); ++k)
                {
                    storeline.push_back(vec2[k][i]);
                }
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                storeline.push_back("<!-- NEW *" + modcode.first + "* -->");

                for (auto& each : modcode.second)
                {
                    func(each->rawlist[0], storeline);
                }

                storeline.push_back("<!-- CLOSE -->");
            }
        }
    }
    else if (linkedtype.raw)
    {
        getLinkedLines(*linkedtype.raw, storeline);
    }
    else
    {
        storeline.push_back("//* delete this line *//");
    }
}
