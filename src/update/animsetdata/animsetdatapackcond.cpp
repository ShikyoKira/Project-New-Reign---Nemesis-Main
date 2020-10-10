#include "update/animsetdata/animsetdatapackcond.h"

using namespace std;

AnimSetPack::AnimSetPack(const VecStr& storeline)
{
    short type = 0;

    for (auto& line : storeline)
    {
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
                unknown = line;
                ++type;
                break;
            }
            case 2:
            {
                attackSetData.reserve(stoi(line));
                ++type;
            }
            case 3:
            {
                attackSetData.push_back(nemesis::LinkedVar<AttackData>(line));
            }
        }
    }
}

AnimSetPack::AnimSetPack(const VecStr& storeline, const string& path, const ModCode& modcode)
{
    short type = 0;
    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;
    int linenum       = 1;
    uint asdCount     = 0;

    for (auto& line : storeline)
    {
        if (line.find("<!--") != NOT_FOUND)
        {
            if (!edited && line.find("<!-- MOD_CODE ~" + modcode + "~ OPEN -->", 0) != NOT_FOUND)
            {
                edited = true;
            }
            else if (line.find("<!-- ORIGINAL -->", 0) != NOT_FOUND)
            {
                originalopen = true;
            }
            else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
            {
                originalopen = false;
                edited       = false;
            }

            continue;
        }
        else if (originalopen)
        {
            continue;
        }

        switch (type)
        {
            case 0:
            {
                if (edited) name.addCond(nemesis::LinkedVar(line), modcode, nemesis::MOD_CODE, linenum);

                ++type;
                break;
            }
            case 1:
            {
                if (edited) unknown.addCond(nemesis::LinkedVar(line), modcode, nemesis::MOD_CODE, linenum);

                ++type;
                break;
            }
            case 2:
            {
                if (!isOnlyNumber(line)) ErrorMessage(5021, modcode, path, linenum);

                attackSetData.reserve(stoi(line));
            }
            case 3:
            {
                if (edited)
                {
                    if (asdCount < attackSetData.size())
                    {
                        attackSetData[asdCount].addCond(line, modcode, nemesis::MOD_CODE, linenum);
                    }
                    else
                    {
                        attackSetData.push_back(
                            nemesis::CondVar<AttackData>(line, modcode, nemesis::MOD_CODE));
                    }
                }

                ++asdCount;
            }
        }

        ++linenum;
    }
}

AnimSetPack::AnimSetPack(const string& _name,
                         const string& _unknown,
                         const vector<AttackData>& _attackSetData)
{
    name = _name;
    unknown = _unknown;
    attackSetData.reserve(_attackSetData.size());

    for (auto& each : _attackSetData)
    {
        attackSetData.push_back(each);
    }
}

void AnimSetPack::getlines(VecStr& storeline)
{
    getLinkedLines(name, storeline);
    getLinkedLines(unknown, storeline);
    storeline.push_back(to_string(attackSetData.size()));
    getLinkedLines(attackSetData, storeline);
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetPack>& linkedpack, VecStr& storeline)
{
    vector<pair<const string*, const nemesis::CondVar<AnimSetPack>*>> modcodelist;

    for (auto& cond : linkedpack.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(
                    make_pair<const string*, const nemesis::CondVar<AnimSetPack>*>(
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
        if (linkedpack.raw)
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
                linkedpack.raw->getlines(storeline);
                storeline.push_back("<!-- CLOSE -->");
            }
            else
            {
                linkedpack.raw->getlines(storeline);
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
    else if (linkedpack.raw)
    {
        linkedpack.raw->getlines(storeline);
    }
}
