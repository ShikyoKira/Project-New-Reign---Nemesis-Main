#include "update/animdata/animdatapackcond.h"

using namespace std;

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
    vector<pair<const string*, const nemesis::CondVar<AnimDataPack_Condt>*>> modcodelist;

    for (auto& cond : linkedanimdata.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(
                    make_pair<const string*, const nemesis::CondVar<AnimDataPack_Condt>*>(&cond.conditions, &cond));
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
        if (linkedanimdata.raw)
        {
            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.push_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                getLinkedLines(modcode.second->rawlist[0], list.back().second);

                if (list.back().second.size() > 0) list.back().second.pop_back();
            }

            for (auto& each : list)
            {
                storeline.push_back("<!-- NEW *" + each.first + "* -->");
                storeline.insert(storeline.end(), each.second.begin(), each.second.end());
            }

            if (list.size() > 0)
            {
                storeline.push_back("<!-- ORIGINAL -->");
                size_t t = storeline.size();
                linkedanimdata.raw->getlines(storeline);

                if (t < storeline.size()) storeline.pop_back();

                storeline.push_back("<!-- CLOSE -->");
                storeline.push_back("");
            }
            else
            {
                linkedanimdata.raw->getlines(storeline);
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                storeline.push_back("<!-- NEW *" + *modcode.first + "* -->");
                getLinkedLines(*modcode.second, storeline);
                storeline.push_back("<!-- CLOSE -->");
            }
        }
    }
    else if (linkedanimdata.raw)
    {
        linkedanimdata.raw->getlines(storeline);
    }
    else
    {
        storeline.push_back("//* delete this line *//");
    }
}
