#include <functional>

#include "utilities/conditionsyntax.h"

#include "update/animdata/infodatapackcond.h"

using namespace std;
namespace ns = nemesis::syntax;

InfoDataPack_Condt::InfoDataPack_Condt(const VecNstr& storeline)
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
                uniquecode = line.ToString();
                ++type;
                break;
            }
            case 1:
            {
                duration = line;
                ++type;
                break;
            }
            case 2:
            {
                // motion data counter
                if (linecounter < storeline.size())
                {
                    const string& nextline = storeline[linecounter];

                    if (count(nextline.begin(), nextline.end(), ' ') == 0 && isOnlyNumber(nextline)) ++type;
                }

                ++type;
                break;
            }
            case 3:
            {
                if (linecounter < storeline.size())
                {
                    const string& nextline = storeline[linecounter];

                    if (count(nextline.begin(), nextline.end(), ' ') == 0 && isOnlyNumber(nextline)) ++type;
                }

                motiondata.emplace_back(line);
                break;
            }
            case 4:
            {
                // rotation data counter
                if (linecounter < storeline.size() && storeline[linecounter].empty()) ++type;

                ++type;

                break;
            }
            case 5:
            {
                if (linecounter < storeline.size() && storeline[linecounter].empty()) ++type;

                rotationdata.emplace_back(line);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void InfoDataPack_Condt::getlines(VecStr& storeline)
{
    // unique code
    storeline.push_back(uniquecode);

    // duration
    storeline.push_back(duration);

    // motion data count
    storeline.push_back(to_string(motiondata.size()));

    for (auto& data : motiondata)
    {
        storeline.push_back(data);
    }

    // rotation data count
    storeline.push_back(to_string(rotationdata.size()));

    for (auto& data : rotationdata)
    {
        storeline.push_back(data);
    }

    storeline.push_back("");
}

void getLinkedLines(const nemesis::LinkedVar<InfoDataPack_Condt>& linkedinfodata, VecStr& storeline)
{
    VecStr tempstore;
    function<void(const string&, const nemesis::CondVar<InfoDataPack_Condt>&)> modcodelambda;

    if (linkedinfodata.raw)
    {
        modcodelambda = [&](const string& condition, const nemesis::CondVar<InfoDataPack_Condt>& pack) 
        {
            tempstore.push_back(ns::ModCode(condition));
            size_t t = tempstore.size();
            getLinkedLines(pack.rawlist[0], tempstore);

            if (t < tempstore.size()) tempstore.pop_back();
        };
    }
    else
    {
        modcodelambda = [&](const string& condition, const nemesis::CondVar<InfoDataPack_Condt>& pack)
        {
            tempstore.push_back(ns::ModCode(condition));
            getLinkedLines(pack, tempstore);
            tempstore.push_back(ns::Close());
        };
    }

    for (auto& cond : linkedinfodata.nestedcond)
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

        if (!linkedinfodata.raw) return;

        storeline.push_back(ns::Original());
        size_t t = storeline.size();
        linkedinfodata.raw->getlines(storeline);

        if (t < storeline.size()) storeline.pop_back();

        storeline.push_back(ns::Close());
        storeline.push_back("");
    }
    else if (linkedinfodata.raw)
    {
        linkedinfodata.raw->getlines(storeline);
    }
    else
    {
        storeline.push_back("//* delete this line *//");
    }
}
