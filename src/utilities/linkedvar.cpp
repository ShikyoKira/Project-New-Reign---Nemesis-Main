#include "utilities/linkedvar.h"

using namespace std;

std::shared_ptr<VecStr> getlinkedline(const nemesis::LinkedVar<std::string>& linkedline)
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
            }
            case nemesis::FOREACH:
            {
                storeline->push_back("<!-- FOREACH ^" + cond.conditions + "^ -->\n");

                for (auto& each : cond.rawlist)
                {
                    storeline->push_back(*each.raw + "\n");
                }

                storeline->push_back("<!-- CLOSE -->\n");
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

void getlinkedline(const nemesis::LinkedVar<std::string>& linkedline, VecStr& storeline)
{
    vector<const nemesis::CondVar<string>*> modcodelist;

    for (auto& cond : linkedline.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(&cond);
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->\n");

                for (auto& each : cond.rawlist)
                {
                    storeline.push_back(*each.raw + "\n");
                }

                storeline.push_back("<!-- CLOSE -->\n");
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        for (auto& modcode : modcodelist)
        {
            storeline.push_back(*modcode->rawlist[0].raw + "\t\t\t\t\t<!-- *" + modcode->conditions
                                 + "* -->\n");
        }

        if (linkedline.raw) storeline.push_back(*linkedline.raw + "\t\t\t\t\t<!-- original -->");
    }
    else if (linkedline.raw)
    {
        storeline.push_back(*linkedline.raw);
    }
}
