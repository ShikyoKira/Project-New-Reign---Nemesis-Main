#include "update/animdata/infodatapackcond.h"

using namespace std;

InfoDataPack_Condt::InfoDataPack_Condt(const VecStr& storeline, size_t linenum)
{
    short type = 0;

    for (unsigned int i = 0; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        switch (type)
        {
            case 0:
            {
                uniquecode = line;
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
                if (i + 1 < storeline.size())
                {
                    const string& nextline = storeline[i + 1];

                    if (count(nextline.begin(), nextline.end(), ' ') == 0 && isOnlyNumber(nextline)) ++type;
                }

                ++type;
                break;
            }
            case 3:
            {
                if (i + 1 < storeline.size())
                {
                    const string& nextline = storeline[i + 1];

                    if (count(nextline.begin(), nextline.end(), ' ') == 0 && isOnlyNumber(nextline)) ++type;
                }

                motiondata.push_back(line);
                break;
            }
            case 4:
            {
                // rotation data counter
                if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

                ++type;

                break;
            }
            case 5:
            {
                if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

                rotationdata.push_back(line);
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

void getlinkedline(const nemesis::LinkedVar<InfoDataPack_Condt>& linkedinfodata, VecStr& storeline)
{
    vector<pair<const string*, const nemesis::CondVar<InfoDataPack_Condt>*>> modcodelist;

    for (auto& cond : linkedinfodata.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(make_pair<const string*, const nemesis::CondVar<InfoDataPack_Condt>*>(
                    &cond.conditions, &cond));
                break;
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");

                for (auto& each : cond.rawlist)
                {
                    getlinkedline(each, storeline);
                }

                storeline.push_back("<!-- CLOSE -->");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        if (linkedinfodata.raw)
        {
            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.push_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                getlinkedline(modcode.second->rawlist[0], list.back().second);

                if (list.back().second.size() > 0) list.back().second.pop_back();
            }

            for (auto& each : list)
            {
                storeline.push_back("<!-- NEW *" + each.first + "* -->");

                for (auto& eachline : each.second)
                {
                    storeline.push_back(eachline);
                }
            }

            if (list.size() > 0)
            {
                storeline.push_back("<!-- ORIGINAL -->");
                size_t t = storeline.size();
                linkedinfodata.raw->getlines(storeline);

                if (t < storeline.size()) storeline.pop_back();

                storeline.push_back("<!-- CLOSE -->");
                storeline.push_back("");
            }
            else
            {
                linkedinfodata.raw->getlines(storeline);
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                storeline.push_back("<!-- NEW *" + *modcode.first + "* -->");
                getlinkedline(modcode.second->rawlist[0], storeline);
                storeline.push_back("<!-- CLOSE -->");
            }
        }
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
