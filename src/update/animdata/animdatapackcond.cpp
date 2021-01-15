#include <functional>

#include "utilities/conditionsyntax.h"

#include "update/animdata/animdatapackcond.h"

using namespace std;
namespace ns = nemesis::syntax;

AnimDataPack_Condt::AnimDataPack_Condt(const VecStr& storeline, size_t linenum)
{
    short type = 0;

    for (unsigned int i = 0; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        switch (type)
        {
            case 0:
            {
                name = line;
                ++type;
                break;
            }
            case 1:
            {
                uniquecode           = line;
                uniquecode.linecount = linenum + i;
                ++type;
                break;
            }
            case 2:
            {
                unknown1           = line;
                unknown1.linecount = linenum + i;
                ++type;
                break;
            }
            case 3:
            {
                unknown2           = line;
                unknown2.linecount = linenum + i;
                ++type;
                break;
            }
            case 4:
            {
                unknown3           = line;
                unknown3.linecount = linenum + i;
                ++type;
                break;
            }
            case 5:
            {
                // event count
                if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

                ++type;
                break;
            }
            case 6:
            {
                if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

                eventname.push_back(nemesis::LinkedVar(line, linenum + i));
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
    storeline.push_back(name);

    // unique code
    getLinkedLines(uniquecode, storeline);

    // unknown variables
    getLinkedLines(unknown1, storeline);
    getLinkedLines(unknown2, storeline);
    getLinkedLines(unknown3, storeline);

    // event name count
    storeline.push_back(to_string(eventname.size()));

    // event name list
    for (auto& each : eventname)
    {
        getLinkedLines(each, storeline);
    }

    storeline.push_back("");
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

    if (tempstore.size() > 0)
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
