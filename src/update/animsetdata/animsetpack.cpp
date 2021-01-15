#include <functional>

#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"

#include "update/animsetdata/animsetpack.h"

using namespace std;
namespace ns = nemesis::syntax;

AnimSetPack::Parser::Parser(AnimSetPack& base, const VecStr& storeline)
    : host(base)
    , storeline(storeline)
{
}

void AnimSetPack::Parser::SetFormat(const ModCode& format)
{
    this->format = format;
}

void AnimSetPack::Parser::SetPath(const std::filesystem::path& path)
{
    this->path = path;
}

void AnimSetPack::Parser::AddAnimSetPack()
{
    type = 0;
    auto& attackSetData = host.attackSetData;

    for (auto& line : storeline)
    {
        switch (type)
        {
            case 0:
            {
                host.name = line;
                ++type;
                break;
            }
            case 1:
            {
                host.unknown = line;
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
                attackSetData.emplace_back(nemesis::LinkedVar<AttackData>(line));
            }
        }
    }
}

void AnimSetPack::Parser::ImportAnimSetPack()
{
    type     = 0;
    linenum  = 1;
    asdCount = 0;

    tobedeleted = nullptr;
    nemesis::ConditionScope cscope(format, path);
    auto& attackSetData = host.attackSetData;

    for (auto& line : storeline)
    {
        auto conditioninfo = cscope.TryGetConditionInfo(line);

        if (!conditioninfo)
        {
            if (cscope.Empty() || cscope.Back()->GetType() == nemesis::CondType::ORIGINAL) continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted();
                }

                //CloseEquip();
                tobedeleted = nullptr;
                break;
            }
        }

        /*
        if (line.find(ns::OpenComment()) != NOT_FOUND)
        {
            if (!edited && line.find(ns::ModCode(format)) != NOT_FOUND)
            {
                edited = true;
            }
            else if (line.find(ns::Original()) != NOT_FOUND)
            {
                originalopen = true;
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
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

        auto& AddLinkedCond = [&](nemesis::LinkedVar<string>& curline) {
            curline.addCond(line, format, nemesis::CondType::MOD_CODE, linenum);
        };

        switch (type)
        {
            case 0:
            {
                if (edited) AddLinkedCond(host.name);

                ++type;
                break;
            }
            case 1:
            {
                if (edited) AddLinkedCond(host.unknown);

                ++type;
                break;
            }
            case 2:
            {
                if (!isOnlyNumber(line)) ErrorMessage(5021, format, path, linenum);

                attackSetData.reserve(stoi(line));
            }
            case 3:
            {
                if (edited)
                {
                    if (asdCount < attackSetData.size())
                    {
                        AddLinkedCond(attackSetData[asdCount]);
                    }
                    else
                    {
                        attackSetData.emplace_back(
                            nemesis::CondVar<AttackData>(line, format, nemesis::CondType::MOD_CODE));
                    }
                }

                ++asdCount;
            }
        }
        */

        ++linenum;
    }
}

AnimSetPack::AnimSetPack(const VecStr& storeline)
{
    Parser parser(*this, storeline);

    parser.AddAnimSetPack();
}

AnimSetPack::AnimSetPack(const VecStr& storeline, const string& path, const ModCode& modcode)
{
    /*
    short type = 0;
    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;
    int linenum       = 1;
    uint asdCount     = 0;

    for (auto& line : storeline)
    {
        if (line.find(ns::OpenComment()) != NOT_FOUND)
        {
            if (!edited && line.find(ns::ModCode(modcode)) != NOT_FOUND)
            {
                edited = true;
            }
            else if (line.find(ns::Original()) != NOT_FOUND)
            {
                originalopen = true;
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
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

        auto& AddLinkedCond = [&](nemesis::LinkedVar<string>& curline)
        {
            curline.addCond(line, modcode, nemesis::CondType::MOD_CODE, linenum);
        };

        switch (type)
        {
            case 0:
            {
                if (edited) AddLinkedCond(name);

                ++type;
                break;
            }
            case 1:
            {
                if (edited) AddLinkedCond(unknown);

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
                        AddLinkedCond(attackSetData[asdCount]);
                    }
                    else
                    {
                        attackSetData.emplace_back(
                            nemesis::CondVar<AttackData>(line, modcode, nemesis::CondType::MOD_CODE));
                    }
                }

                ++asdCount;
            }
        }

        ++linenum;
    }*/
    Parser parser(*this, storeline);
    parser.SetFormat(modcode);
    parser.SetPath(path);

    parser.ImportAnimSetPack();
}

AnimSetPack::AnimSetPack(const nemesis::Line& _name,
                         const nemesis::Line& _unknown,
                         const Vec<AttackData>& _attackSetData)
{
    AddName(_name);
    AddUnknown(_unknown);
    attackSetData.reserve(_attackSetData.size());

    for (auto& each : _attackSetData)
    {
        attackSetData.emplace_back(each);
    }
}

bool AnimSetPack::AddName(const nemesis::Line& _name)
{
    if (!hasAlpha(_name)) return false;

    name = ToLinkedString(_name);
    return true;
}

bool AnimSetPack::AddUnknown(const nemesis::Line& _unknown)
{
    if (!isOnlyNumber(_unknown)) return false;

    unknown = ToLinkedString(_unknown);
    return true;
}

void AnimSetPack::AddAttackSetData(const nemesis::Line& _attackSetData)
{
    attackSetData.emplace_back(ToLinkedString(_attackSetData));
}

void AnimSetPack::getlines(VecStr& storeline)
{
    getLinkedLines(name, storeline);
    getLinkedLines(unknown, storeline);
    storeline.emplace_back(to_string(attackSetData.size()));
    getLinkedLines(attackSetData, storeline);
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetPack>& linkedpack, VecStr& storeline)
{
    vector<const nemesis::CondVar<AnimSetPack>*> tempstore;

    for (auto& cond : linkedpack.nestedcond)
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

                storeline.emplace_back(ns::Close());
                break;
            }
        }
    }

    if (!tempstore.empty())
    {
        if (linkedpack.raw)
        {
            for (auto& each : tempstore)
            {
                storeline.emplace_back(ns::ModCode(each->conditions));
                getLinkedLines(each->rawlist[0], storeline);
            }

            storeline.emplace_back(ns::Original());
            linkedpack.raw->getlines(storeline);
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
    else if (linkedpack.raw)
    {
        linkedpack.raw->getlines(storeline);
    }
}
