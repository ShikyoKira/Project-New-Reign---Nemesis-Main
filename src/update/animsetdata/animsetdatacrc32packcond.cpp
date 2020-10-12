#include "animsetdatacrc32packcond.h"

#include "utilities/conditionsyntax.h"

using namespace std;

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack::crc32>& linkedcrc32, VecStr& storeline);

bool AnimSetCRC32Pack::validation(const nemesis::Line& line)
{
    return (line.front() == '$' && line.back() == '$') || isOnlyNumber(line);
}

void AnimSetCRC32Pack::getlines(VecStr& storeline)
{
    getLinkedLines(filepath, storeline);
    getLinkedLines(filename, storeline);
    getLinkedLines(extension, storeline);
}

bool AnimSetCRC32Pack::addFilePath(const nemesis::Line& _filepath)
{
    filepath = _filepath.ToLinkedVarString();
    return validation(_filepath);
}

bool AnimSetCRC32Pack::addFileName(const nemesis::Line& _filename)
{
    filename = _filename.ToLinkedVarString();
    return validation(_filename);
}

bool AnimSetCRC32Pack::addExtension(const nemesis::Line& _extension)
{
    extension = _extension.ToLinkedVarString();
    return true;
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

                storeline.push_back(nemesis::syntax::Close());
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
