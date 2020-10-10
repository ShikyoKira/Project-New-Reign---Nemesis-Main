#include "animsetdatacrc32packcond.h"

using namespace std;

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack::crc32>& linkedcrc32, VecStr& storeline);

void AnimSetCRC32Pack::getlines(VecStr& storeline)
{
    getLinkedLines(filepath, storeline);
    getLinkedLines(filename, storeline);
    getLinkedLines(extension, storeline);
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack>& linkedcrc32, VecStr& storeline)
{
    vector<pair<const string*, const nemesis::CondVar<AnimSetCRC32Pack>*>> modcodelist;

    for (auto& cond : linkedcrc32.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(
                    make_pair<const string*, const nemesis::CondVar<AnimSetCRC32Pack>*>(
                        &cond.conditions, &cond));
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
        if (linkedcrc32.raw)
        {
            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.push_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                getLinkedLines(modcode.second->rawlist[0], list.back().second);
            }

            for (auto& each : list)
            {
                storeline.push_back("<!-- NEW *" + each.first + "* -->");
                storeline.insert(storeline.end(), each.second.begin(), each.second.end());
            }

            if (list.size() > 0)
            {
                storeline.push_back("<!-- ORIGINAL -->");
                linkedcrc32.raw->getlines(storeline);
                storeline.push_back("<!-- CLOSE -->");
            }
            else
            {
                linkedcrc32.raw->getlines(storeline);
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                storeline.push_back("<!-- NEW *" + *modcode.first + "* -->");
                getLinkedLines(modcode.second->rawlist[0], storeline);
                storeline.push_back("<!-- CLOSE -->");
            }
        }
    }
    else if (linkedcrc32.raw)
    {
        linkedcrc32.raw->getlines(storeline);
    }
}
