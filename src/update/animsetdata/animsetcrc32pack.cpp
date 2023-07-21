#include "utilities/conditionsyntax.h"

#include "update/animsetdata/animsetcrc32pack.h"

using namespace std;
namespace ns = nemesis::syntax;

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack::crc32>& linkedcrc32, VecStr& storeline);

bool AnimSetCRC32Pack::Validation(const nemesis::Line& line)
{
    return (line.front() == '$' && line.back() == '$') || isOnlyNumber(line);
}

void AnimSetCRC32Pack::getlines(VecStr& storeline)
{
    getLinkedLines(filepath, storeline);
    getLinkedLines(filename, storeline);
    getLinkedLines(extension, storeline);
}

bool AnimSetCRC32Pack::AddFilePath(const nemesis::Line& _filepath)
{
    filepath = ToLinkedString(_filepath);
    return Validation(_filepath);
}

bool AnimSetCRC32Pack::AddFileName(const nemesis::Line& _filename)
{
    filename = ToLinkedString(_filename);
    return Validation(_filename);
}

bool AnimSetCRC32Pack::AddExtension(const nemesis::Line& _extension)
{
    extension = ToLinkedString(_extension);
    return true;
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack>& linkedcrc32, VecStr& storeline)
{
    vector<const nemesis::CondVar<AnimSetCRC32Pack>*> tempstore;

    for (auto& cond : linkedcrc32.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                tempstore.emplace_back(&cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                storeline.emplace_back(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline);
                }

                storeline.emplace_back(nemesis::syntax::Close());
                break;
            }
        }
    }

    if (!tempstore.empty())
    {
        if (linkedcrc32.raw)
        {
            for (auto& each : tempstore)
            {
                storeline.emplace_back(ns::ModCode(each->conditions));
                getLinkedLines(each->rawlist[0], storeline);
            }

            storeline.emplace_back(ns::ModOriginal());
            linkedcrc32.raw->getlines(storeline);
            storeline.emplace_back(ns::Close());
            return;
        }

        for (auto& each : tempstore)
        {
            storeline.emplace_back(ns::ModCode(each->conditions));
            getLinkedLines(each->rawlist[0], storeline);
            storeline.emplace_back(ns::Close());
        }
    }
    else if (linkedcrc32.raw)
    {
        linkedcrc32.raw->getlines(storeline);
    }
}
