
#include "Global.h"

#include "utilities/linkedvar.h"
#include "utilities/conditionsyntax.h"
#include "utilities/conditiondetails.h"

using namespace std;
namespace ns = nemesis::syntax;

void combineLines(std::string& last, VecStr& temp, VecStr& combined)
{
    if (temp.empty()) return;

    VecStr templaststack;
    string templast;

    if (!last.empty() && temp.front() == last)
    {
        templaststack.push_back(last);
        temp.erase(temp.begin());
        combined.pop_back();
        last = "";
    }

    combined.reserve(combined.size() + temp.size());

    if (temp.back().find(ns::Close()) != NOT_FOUND)
    {
        for (auto& line : temp)
        {
            combined.push_back(line);

            if (line.find(ns::ModCode()) != NOT_FOUND || line.find(ns::ForEach()) != NOT_FOUND)
            {
                templaststack.push_back(line);
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
            {
                templast = templaststack.back();
                templaststack.pop_back();
            }
        }

        last = templast;
    }
    else
    {
        combined.insert(combined.end(), temp.begin(), temp.end());
        last = "";
    }
}

SPtr<VecStr> getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline)
{
    std::shared_ptr<VecStr> storeline = std::make_shared<VecStr>();
    vector<const nemesis::CondVar<string>*> modcodelist;

    for (auto& cond : linkedline.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.push_back(&cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                storeline->push_back(ns::ForEach(cond.conditions) + "\n");

                for (auto& each : cond.rawlist)
                {
                    storeline->push_back(*each.raw + "\n");
                }

                storeline->push_back(ns::Close() + "\n");
                break;
            }
        }
    }

    if (!modcodelist.empty())
    {
        for (auto& modcode : modcodelist)
        {
            storeline->push_back(*modcode->rawlist[0].raw + ns::Spaces() + ns::Aster(modcode->conditions)
                                 + "\n");
        }

        if (linkedline.raw) storeline->push_back(*linkedline.raw + ns::Spaces() + ns::LowerOriginal());
    }
    else if (linkedline.raw)
    {
        storeline->push_back(*linkedline.raw);
    }

    return storeline;
}

void getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline, VecStr& storeline)
{
    vector<const nemesis::CondVar<string>*> modcodelist;

    for (auto& cond : linkedline.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.push_back(&cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                storeline.push_back(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    storeline.push_back(*each.raw);
                }

                storeline.push_back(ns::Close());
                break;
            }
        }
    }

    if (!modcodelist.empty())
    {
        for (auto& modcode : modcodelist)
        {
            storeline.push_back(*modcode->rawlist[0].raw + ns::Spaces() + ns::Aster(modcode->conditions));
        }

        if (linkedline.raw) storeline.push_back(*linkedline.raw + ns::Spaces() + ns::LowerOriginal());
    }
    else if (linkedline.raw)
    {
        storeline.push_back(*linkedline.raw);
    }
}

void getLinkedLines(const nemesis::LinkedVar<nemesis::Line>& linkedline, VecNstr& storeline)
{
    vector<const nemesis::CondVar<nemesis::Line>*> modcodelist;

    for (auto& cond : linkedline.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.emplace_back(&cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                storeline.emplace_back(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    storeline.push_back(*each.raw);
                }

                storeline.emplace_back(ns::Close());
                break;
            }
        }
    }

    if (!modcodelist.empty())
    {
        for (auto& modcode : modcodelist)
        {
            storeline.emplace_back(modcode->rawlist[0].raw->ToString() + ns::Spaces()
                                   + ns::Aster(modcode->conditions));
        }

        if (linkedline.raw)
        {
            storeline.emplace_back(linkedline.raw->ToString() + ns::Spaces() + ns::LowerOriginal());
        }
    }
    else if (linkedline.raw)
    {
        storeline.emplace_back(*linkedline.raw);
    }
}

void getLinkedLines(vector<nemesis::LinkedVar<string>> linkedlist, VecStr& storeline)
{
    for (auto& each : linkedlist)
    {
        vector<pair<const string*, const nemesis::CondVar<string>*>> modcodelist;

        for (auto& cond : each.nestedcond)
        {
            switch (cond.conditionType)
            {
                case nemesis::CondType::MOD_CODE:
                {
                    modcodelist.emplace_back(
                        make_pair<const string*, const nemesis::CondVar<string>*>(&cond.conditions, &cond));
                    break;
                }
                case nemesis::CondType::FOREACH:
                {
                    storeline.push_back(ns::ForEach(cond.conditions));
                    getLinkedLines(cond.rawlist, storeline);
                    storeline.push_back(ns::Close());
                    break;
                }
            }
        }

        if (!modcodelist.empty())
        {
            if (each.raw)
            {
                vector<pair<string, VecStr>> list;

                for (auto& modcode : modcodelist)
                {
                    list.emplace_back(pair<string, VecStr>());
                    list.back().first = *modcode.first;
                    getLinkedLines(modcode.second->rawlist, list.back().second);
                }

                for (auto& each : list)
                {
                    storeline.push_back(ns::ModCode(each.first));
                    storeline.insert(storeline.end(), each.second.begin(), each.second.end());
                }

                if (!list.empty())
                {
                    storeline.push_back(ns::Original());
                    storeline.push_back(*each.raw);
                    storeline.push_back(ns::Close());
                }
                else
                {
                    storeline.push_back(*each.raw);
                }
            }
            else
            {
                for (auto& modcode : modcodelist)
                {
                    storeline.push_back(ns::ModCode(*modcode.first));
                    getLinkedLines(modcode.second->rawlist, storeline);
                    storeline.push_back(ns::Close());
                }
            }
        }
        else if (each.raw)
        {
            storeline.push_back(*each.raw);
        }
        else
        {
            storeline.push_back(ns::DeleteLine());
        }
    }
}

nemesis::LinkedString nemesis::ToLinkedString(const nemesis::Line& line)
{
    return nemesis::LinkedString(line, line.GetLineNumber());
}

nemesis::LinkedWstring nemesis::ToLinkedWstring(const nemesis::Wline& line)
{
    return nemesis::LinkedWstring(line, line.GetLineNumber());
}
