#include <functional>

#include "utilities/conditionsyntax.h"

#include "update/animdata/animdatapackcond.h"

using namespace std;
namespace ns = nemesis::syntax;

AnimDataPack_Condt::AnimDataPack_Condt(const VecNstr& storeline)
{
    short type = 0;
    size_t linecounter = 0;

    for (auto& line : storeline)
    {
        linecounter += 1;

        switch (type)
        {
            case 0:
            {
                name = line.ToString();
                ++type;
                break;
            }
            case 1:
            {
                uniquecode           = line.ToString();
                uniquecode.linecount = line.GetLineNumber();
                ++type;
                break;
            }
            case 2:
            {
                unknown1           = line.ToString();
                unknown1.linecount = line.GetLineNumber();
                ++type;
                break;
            }
            case 3:
            {
                unknown2           = line.ToString();
                unknown2.linecount = line.GetLineNumber();
                ++type;
                break;
            }
            case 4:
            {
                unknown3           = line.ToString();
                unknown3.linecount = line.GetLineNumber();
                ++type;
                break;
            }
            case 5:
            {
                // event count
                if (linecounter < storeline.size() && storeline[linecounter].empty()) ++type;

                ++type;
                break;
            }
            case 6:
            {
                if (linecounter < storeline.size() && storeline[linecounter].empty()) ++type;

                eventname.emplace_back(nemesis::LinkedVar(line.ToString(), line.GetLineNumber()));
            }
            default:
            {
                break;
            }
        }
    }
}

void AnimDataPack_Condt::getlines(VecStr& storeline)
{
    // anim data name
    storeline.emplace_back(name);

    // unique code
    getLinkedLines(uniquecode, storeline);

    // unknown variables
    getLinkedLines(unknown1, storeline);
    getLinkedLines(unknown2, storeline);
    getLinkedLines(unknown3, storeline);

    // event name count
    storeline.emplace_back(to_string(eventname.size()));

    // event name list
    for (auto& each : eventname)
    {
        getLinkedLines(each, storeline);
    }

    storeline.emplace_back("");
}

void getLinkedLines(const nemesis::LinkedVar<AnimDataPack_Condt>& linkedanimdata, VecStr& storeline)
{
    VecStr tempstore;
    function<void(const string&, const nemesis::CondVar<AnimDataPack_Condt>&)> modcodelambda;

    if (linkedanimdata.raw)
    {
        modcodelambda = [&](const string& condition, const nemesis::CondVar<AnimDataPack_Condt>& pack)
        {
            tempstore.push_back(ns::ModCode(condition));
            size_t t = tempstore.size();
            getLinkedLines(pack.rawlist[0], tempstore);

            if (t < tempstore.size()) tempstore.pop_back();
        };
    }
    else
    {
        modcodelambda = [&](const string& condition, const nemesis::CondVar<AnimDataPack_Condt>& pack)
        {
            tempstore.push_back(ns::ModCode(condition));
            getLinkedLines(pack, tempstore);
            tempstore.push_back(ns::Close());
        };
    }

    for (auto& cond : linkedanimdata.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelambda(cond.conditions, cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                storeline.push_back(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline);
                }

                storeline.push_back(ns::Close());
                break;
            }
        }
    }

    if (!tempstore.empty())
    {
        storeline.insert(storeline.end(), tempstore.begin(), tempstore.end());

        if (!linkedanimdata.raw) return;

        storeline.push_back(ns::Original());
        size_t t = storeline.size();
        linkedanimdata.raw->getlines(storeline);

        if (t < storeline.size()) storeline.pop_back();

        storeline.push_back(ns::Close());
        storeline.push_back("");
    }
    else if (linkedanimdata.raw)
    {
        linkedanimdata.raw->getlines(storeline);
    }
    else
    {
        storeline.push_back(ns::DeleteLine());
    }
}
