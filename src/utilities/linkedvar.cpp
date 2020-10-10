#include "utilities/linkedvar.h"

using namespace std;

void combineLines(std::string& last, VecStr& temp, VecStr& combined)
{
    if (temp.size() == 0) return;

    VecStr templaststack;
    string templast;

    if (last.length() > 0 && temp.front() == last)
    {
        templaststack.push_back(last);
        temp.erase(temp.begin());
        combined.pop_back();
        last = "";
    }

    combined.reserve(combined.size() + temp.size());

    if (temp.back().find("<!-- CLOSE -->") != NOT_FOUND)
    {
        for (auto& line : temp)
        {
            combined.push_back(line);

            if (line.find("<!-- NEW") != NOT_FOUND || line.find("<!-- FOREACH") != NOT_FOUND)
            {
                templaststack.push_back(line);
            }
            else if (line.find("<!-- CLOSE -->") != NOT_FOUND)
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

std::shared_ptr<VecStr> getLinkedLines(const nemesis::LinkedVar<std::string>& linkedline)
{
    std::shared_ptr<VecStr> storeline = std::make_shared<VecStr>();
    vector<const nemesis::CondVar<string>*> modcodelist;

    for (auto& cond : linkedline.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(&cond);
                break;
            }
            case nemesis::FOREACH:
            {
                storeline->push_back("<!-- FOREACH ^" + cond.conditions + "^ -->\n");

                for (auto& each : cond.rawlist)
                {
                    storeline->push_back(*each.raw + "\n");
                }

                storeline->push_back("<!-- CLOSE -->\n");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        for (auto& modcode : modcodelist)
        {
            storeline->push_back(*modcode->rawlist[0].raw + "\t\t\t\t\t<!-- *" + modcode->conditions
                                 + "* -->\n");
        }

        if (linkedline.raw) storeline->push_back(*linkedline.raw + "\t\t\t\t\t<!-- original -->");
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
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(&cond);
                break;
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");

                for (auto& each : cond.rawlist)
                {
                    storeline.push_back(*each.raw + "");
                }

                storeline.push_back("<!-- CLOSE -->");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        for (auto& modcode : modcodelist)
        {
            storeline.push_back(*modcode->rawlist[0].raw + "\t\t\t\t\t<!-- *" + modcode->conditions
                                 + "* -->");
        }

        if (linkedline.raw) storeline.push_back(*linkedline.raw + "\t\t\t\t\t<!-- original -->");
    }
    else if (linkedline.raw)
    {
        storeline.push_back(*linkedline.raw);
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
                case nemesis::MOD_CODE:
                {
                    modcodelist.push_back(
                        make_pair<const string*, const nemesis::CondVar<string>*>(&cond.conditions, &cond));
                    break;
                }
                case nemesis::FOREACH:
                {
                    storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");
                    getLinkedLines(cond.rawlist, storeline);
                    storeline.push_back("<!-- CLOSE -->");
                    break;
                }
            }
        }

        if (modcodelist.size() > 0)
        {
            if (each.raw)
            {
                vector<pair<string, VecStr>> list;

                for (auto& modcode : modcodelist)
                {
                    list.push_back(pair<string, VecStr>());
                    list.back().first = *modcode.first;
                    getLinkedLines(modcode.second->rawlist, list.back().second);
                }

                for (auto& each : list)
                {
                    storeline.push_back("<!-- NEW *" + each.first + "* -->");
                    storeline.insert(storeline.end(), each.second.begin(), each.second.end());
                }

                if (list.size() > 0)
                {
                    storeline.push_back("<!-- ORIGINAL -->");
                    storeline.push_back(*each.raw);
                    storeline.push_back("<!-- CLOSE -->");
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
                    storeline.push_back("<!-- NEW *" + *modcode.first + "* -->");
                    getLinkedLines(modcode.second->rawlist, storeline);
                    storeline.push_back("<!-- CLOSE -->");
                }
            }
        }
        else if (each.raw)
        {
            storeline.push_back(*each.raw);
        }
        else
        {
            storeline.push_back("//* delete this line *//");
        }
    }
}
