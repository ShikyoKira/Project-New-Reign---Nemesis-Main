#include <functional>

#include "Global.h"

#include "utilities/conditionsyntax.h"

#include "update/animdata/animdataprojectcond.h"


using namespace std;
namespace ns = nemesis::syntax;

using LinkedAnimPair = pair<string, nemesis::LinkedVar<AnimDataPack_Condt>>;
using LinkedInfoPair = pair<string, nemesis::LinkedVar<InfoDataPack_Condt>>;

void getanimdatapack(const nemesis::LinkedVar<LinkedAnimPair>& animdatapack, VecStr& storeline,
                     string& curmodcode);
void getinfodatapack(const nemesis::LinkedVar<LinkedInfoPair>& infodatapack, VecStr& storeline,
                     string& curmodcode);

AnimDataProject_Condt::AnimDataProject_Condt(const VecStr& storeline, size_t linenum)
{
    short type = 0;

    for (unsigned int i = 1; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        switch (type)
        {
            case 0:
            {
                projectActive           = line;
                projectActive.linecount = linenum + i;
                ++type;
                break;
            }
            case 1:
            {
                // behavior txt count
                if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                ++type;
                break;
            }
            case 2:
            {
                if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                behaviorlist.emplace_back(line);
                behaviorlist.back().linecount = linenum + i;
                break;
            }
            case 3:
            {
                childActive = line;
                childActive.linecount = linenum + i;
                ++type;
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void AnimDataProject_Condt::update(const ModCode& modcode, const VecStr& storeline, size_t linenum)
{
    short type = 0;

    if (modcode == "original")
    {
        for (unsigned int i = 1; i < storeline.size(); ++i)
        {
            const string& line = storeline[i];

            switch (type)
            {
                case 0:
                {
                    projectActive           = line;
                    projectActive.linecount = linenum + i;
                    ++type;
                    break;
                }
                case 1:
                {
                    // behavior txt count
                    if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                    ++type;
                    break;
                }
                case 2:
                {
                    if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                    behaviorlist.emplace_back(line);
                    behaviorlist.back().linecount = linenum + i;
                    break;
                }
                case 3:
                {
                    childActive           = line;
                    childActive.linecount = linenum + i;
                    ++type;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        return;
    }
}

void AnimDataProject_Condt::modify(const ModCode& modcode, const VecStr& storeline)
{
    uint bhvrCount    = 0;
    short type        = 0;
    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;

    for (uint i = 1; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

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

        switch (type)
        {
            case 0:
            {
                if (edited) projectActive.addCond(nemesis::LinkedVar(line), modcode, nemesis::CondType::MOD_CODE);

                ++type;
                break;
            }
            case 1:
            {
                // behavior txt count
                ++type;
                break;
            }
            case 2:
            {
                if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                if (edited)
                {
                    if (bhvrCount < behaviorlist.size())
                    {
                        behaviorlist[bhvrCount].addCond(line, modcode, nemesis::CondType::MOD_CODE);
                    }
                    else
                    {
                        behaviorlist.emplace_back(nemesis::CondVar<string>(line, modcode, nemesis::CondType::MOD_CODE));
                    }
                }

                ++bhvrCount;
                break;
            }
            case 3:
            {
                if (edited) childActive.addCond(line, modcode, nemesis::CondType::MOD_CODE, i + 1);

                ++type;
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

AnimDataPack_Condt&
AnimDataProject_Condt::aadd(const Header& header, const ModCode& modcode, nemesis::CondType type)
{
    using DP = std::pair<Header, nemesis::LinkedVar<AnimDataPack_Condt>>;

    if (modcode == "original")
    {
        animdatalist.emplace_back(DataPackCondt<AnimDataPack_Condt>());
        animdatalist.back().raw        = make_shared<DP>();
        animdatalist.back().raw->first = header;
        return *animdatalist.back().raw->second.raw;
    }

    animdatalist.emplace_back(DataPackCondt<AnimDataPack_Condt>(nemesis::CondVar<DP>(DP(), modcode, type)));
    animdatalist.back().nestedcond.back().rawlist.back().raw->first = header;
    return *animdatalist.back().nestedcond.back().rawlist.back().raw->second.raw;
}

AnimDataPack_Condt& AnimDataProject_Condt::aadd(const Header& header,
                                                const ModCode& modcode,
                                                const VecStr& storeline,
                                                size_t linenum,
                                                nemesis::CondType type)
{
    using DP = std::pair<Header, nemesis::LinkedVar<AnimDataPack_Condt>>;
    shared_ptr<DP> refPair;

    if (modcode != "original")
    {
        animdatalist.emplace_back(DataPackCondt<AnimDataPack_Condt>(nemesis::CondVar(DP(), modcode, type)));
        auto& refLinked     = animdatalist.back().nestedcond.back().rawlist.back();
        refLinked.linecount = linenum;
        refPair             = refLinked.raw;
    }
    else
    {
        animdatalist.emplace_back(DataPackCondt<AnimDataPack_Condt>());
        auto& refLinked     = animdatalist.back();
        refLinked.linecount = linenum;
        refLinked.raw       = make_shared<DP>(make_pair(Header(), nemesis::LinkedVar<AnimDataPack_Condt>()));
        refPair             = refLinked.raw;
    }

    refPair->first            = header;
    refPair->second.raw       = make_shared<AnimDataPack_Condt>(storeline, linenum);
    refPair->second.linecount = linenum;
    return *refPair->second.raw;
}

InfoDataPack_Condt&
AnimDataProject_Condt::iadd(const Header& header, const ModCode& modcode, nemesis::CondType type)
{
    using DP = std::pair<Header, nemesis::LinkedVar<InfoDataPack_Condt>>;

    if (modcode == "original")
    {
        infodatalist.emplace_back(DataPackCondt<InfoDataPack_Condt>());
        infodatalist.back().raw        = make_shared<DP>();
        infodatalist.back().raw->first = header;
        return *infodatalist.back().raw->second.raw;
    }

    infodatalist.emplace_back(DataPackCondt<InfoDataPack_Condt>(nemesis::CondVar(DP(), modcode, type)));
    infodatalist.back().nestedcond.back().rawlist.back().raw->first = header;
    return *infodatalist.back().nestedcond.back().rawlist.back().raw->second.raw;
}

InfoDataPack_Condt& AnimDataProject_Condt::iadd(const Header& header,
                                                const ModCode& modcode,
                                                const VecStr& storeline,
                                                size_t linenum,
                                                nemesis::CondType type)
{
    using DP = std::pair<Header, nemesis::LinkedVar<InfoDataPack_Condt>>;
    shared_ptr<DP> refPair;

    if (modcode != "original")
    {
        infodatalist.emplace_back(DataPackCondt<InfoDataPack_Condt>(nemesis::CondVar(DP(), modcode, type)));
        auto& refLinked     = infodatalist.back().nestedcond.back().rawlist.back();
        refLinked.linecount = linenum;
        refPair             = refLinked.raw;
    }
    else
    {
        infodatalist.emplace_back(DataPackCondt<InfoDataPack_Condt>());
        auto& refLinked     = infodatalist.back();
        refLinked.linecount = linenum;
        refLinked.raw       = make_shared<DP>(make_pair(Header(), nemesis::LinkedVar<InfoDataPack_Condt>()));
        refPair             = refLinked.raw;
    }

    refPair->first            = header;
    refPair->second.raw       = make_shared<InfoDataPack_Condt>(storeline, linenum);
    refPair->second.linecount = linenum;
    return *refPair->second.raw;
}

nemesis::LinkedVar<AnimDataPack_Condt>* AnimDataProject_Condt::afindlist(const Header& header)
{
    for (auto& each : animdatalist)
    {
        if (!each.raw) continue;

        if (each.raw->first == header) return &each.raw->second;
    }

    return nullptr;
}

nemesis::LinkedVar<InfoDataPack_Condt>* AnimDataProject_Condt::ifindlist(const Header& header)
{
    for (auto& each : infodatalist)
    {
        if (!each.raw) continue;

        if (each.raw->first == header) return &each.raw->second;
    }

    return nullptr;
}

AnimDataPack_Condt* AnimDataProject_Condt::afind(const Header& header, const ModCode& modcode)
{
    for (auto& each : animdatalist)
    {
        if (!each.raw) continue;

        if (each.raw->first + "~" + *each.raw->second.raw->uniquecode.raw != header) continue;

        if (modcode == "original") return &*each.raw->second.raw;

        for (auto& eachcond : each.raw->second.nestedcond)
        {
            if (eachcond.conditions == modcode) return &*eachcond.rawlist.back().raw;
        }
    }

    return nullptr;
}

InfoDataPack_Condt* AnimDataProject_Condt::ifind(const Header& header, const ModCode& modcode)
{
    for (auto& each : infodatalist)
    {
        if (!each.raw) continue;

        if (each.raw->first != header) continue;

        if (modcode == "original") return &*each.raw->second.raw;

        for (auto& eachcond : each.raw->second.nestedcond)
        {
            if (eachcond.conditions == modcode) return &*eachcond.rawlist.back().raw;
        }
    }

    return nullptr;
}

void AnimDataProject_Condt::getlines(VecStr& storeline)
{
    // project status
    shared_ptr<VecStr> output = getLinkedLines(projectActive);

    // behavior count
    output->emplace_back(to_string(behaviorlist.size()));

    // behavior list
    for (auto& each : behaviorlist)
    {
        getLinkedLines(each, *output);
    }

    // child status
    getLinkedLines(childActive, *output);

    string mod;

    // anim data
    for (auto& each : animdatalist)
    {
        getanimdatapack(each, *output, mod);
    }

    if (mod != "") output->emplace_back(ns::Close());

    size_t pos = output->size();

    if (animdatalist.size() > 0)
    {
        output->emplace_back("");
        mod.clear();

        // info data
        for (auto& each : infodatalist)
        {
            getinfodatapack(each, *output, mod);
        }

        if (mod != "") output->emplace_back(ns::Close());

        output->at(pos) = to_string(output->size() - 1 - pos);
    }

    storeline.emplace_back(to_string(pos));
    storeline.insert(storeline.end(), output->begin(), output->end());
}

void getanimdatapack(const nemesis::LinkedVar<LinkedAnimPair>& animdatapack,
                     VecStr& storeline,
                     string& curmodcode)
{
    vector<pair<const string*, const nemesis::CondVar<LinkedAnimPair>*>> modcodelist;

    for (auto& cond : animdatapack.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.emplace_back(make_pair<const string*, const nemesis::CondVar<LinkedAnimPair>*>(&cond.conditions, &cond));
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                if (curmodcode != "")
                {
                    storeline.emplace_back(ns::Close());
                    curmodcode.clear();
                }

                storeline.emplace_back(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getanimdatapack(each, storeline, curmodcode);
                }

                storeline.emplace_back(ns::Close());
                break;
            }
        }
    }

    if (!modcodelist.empty())
    {
        if (animdatapack.raw)
        {
            if (curmodcode != "")
            {
                storeline.emplace_back(ns::Close());
                curmodcode.clear();
            }

            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.emplace_back(make_pair(*modcode.first, VecStr()));
                getanimdatapack(modcode.second->rawlist[0], list.back().second, curmodcode);

                for (auto& each : list.back().second)
                {
                    each.append(ns::Spaces() + ns::Aster(*modcode.first));
                }
            }

            size_t max = 0;
            list.emplace_back(make_pair<string, VecStr>("original", VecStr()));
            getLinkedLines(animdatapack.raw->second, list.back().second);

            for (auto& line : list.back().second)
            {
                line.append(ns::Spaces() + ns::LowerOriginal());
            }

            for (auto& each : list)
            {
                max = max > each.second.size() ? max : each.second.size();
            }

            for (size_t i = 0; i < max; ++i)
            {
                for (auto& each : list)
                {
                    if (i >= each.second.size())
                    {
                        string constr = each.first == "original" ? ns::LowerOriginal()
                                                                 : ns::Aster(each.first);
                        storeline.emplace_back(ns::DeleteLine() + ns::Spaces() + constr);
                    }
                    else
                    {
                        storeline.emplace_back(each.second[i]);
                    }
                }
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                if (curmodcode != *modcode.first)
                {
                    if (curmodcode != "") storeline.emplace_back(ns::Close());

                    storeline.emplace_back(ns::ModCode(*modcode.first));
                    curmodcode = *modcode.first;
                }

                for (auto& each : modcode.second->rawlist)
                {
                    getanimdatapack(each, storeline, curmodcode);
                }
            }
        }
    }
    else if (animdatapack.raw)
    {
        getLinkedLines(animdatapack.raw->second, storeline);
    }
    else
    {
        storeline.emplace_back(ns::DeleteLine());
    }
}

void getinfodatapack(const nemesis::LinkedVar<LinkedInfoPair>& infodatapack,
                     VecStr& storeline,
                     string& curmodcode)
{
    vector<pair<const string*, const nemesis::CondVar<LinkedInfoPair>*>> modcodelist;

    for (auto& cond : infodatapack.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.emplace_back(
                    make_pair<const string*, const nemesis::CondVar<LinkedInfoPair>*>(&cond.conditions, &cond));
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                if (curmodcode != "")
                {
                    storeline.emplace_back(ns::Close());
                    curmodcode.clear();
                }

                storeline.emplace_back(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getinfodatapack(each, storeline, curmodcode);
                }

                storeline.emplace_back(ns::Close());
                break;
            }
        }
    }

    if (!modcodelist.empty())
    {
        if (infodatapack.raw)
        {
            if (curmodcode != "")
            {
                storeline.emplace_back(ns::Close());
                curmodcode.clear();
            }

            vector<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.emplace_back(make_pair(*modcode.first, VecStr()));
                getinfodatapack(modcode.second->rawlist[0], list.back().second, curmodcode);

                for (auto& each : list.back().second)
                {
                    each.append(ns::Spaces() + ns::Aster(*modcode.first));
                }
            }

            size_t max = 0;
            list.emplace_back(make_pair<string, VecStr>("original", VecStr()));
            getLinkedLines(infodatapack.raw->second, list.back().second);

            for (auto& line : list.back().second)
            {
                line.append(ns::Spaces() + ns::LowerOriginal());
            }

            for (auto& each : list)
            {
                max = max > each.second.size() ? max : each.second.size();
            }

            for (size_t i = 0; i < max; ++i)
            {
                for (auto& each : list)
                {
                    if (i >= each.second.size())
                    {
                        string constr = each.first == "original" ? ns::LowerOriginal()
                                                                 : ns::Aster(each.first);
                        storeline.emplace_back(ns::DeleteLine() + ns::Spaces() + constr);
                    }
                    else
                    {
                        storeline.emplace_back(each.second[i]);
                    }
                }
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                if (curmodcode != *modcode.first)
                {
                    if (curmodcode != "") storeline.emplace_back(ns::Close());

                    storeline.emplace_back(ns::ModCode(*modcode.first));
                    curmodcode = *modcode.first;
                }

                for (auto& each : modcode.second->rawlist)
                {
                    getinfodatapack(each, storeline, curmodcode);
                }
            }
        }
    }
    else if (infodatapack.raw)
    {
        getLinkedLines(infodatapack.raw->second, storeline);
    }
    else
    {
        storeline.emplace_back(ns::DeleteLine());
    }
}
