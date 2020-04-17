#include "update/animdata/animdataprojectcond.h"

using namespace std;

AnimDataProject_Condt::AnimDataProject_Condt(const VecStr& storeline)
{
    short type = 0;

    for (unsigned int i = 1; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        switch (type)
        {
            case 0:
                projectActive = line;
                ++type;
                break;
            case 1:
                // behavior txt count
                ++type;
                break;
            case 2:
                if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                behaviorlist.push_back(line);
                break;
            case 3:
                childActive = line;
                ++type;
                break;
            default:
                break;
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
                    projectActive           = line;
                    projectActive.linecount = linenum + i;
                    ++type;
                    break;
                case 1:
                    // behavior txt count
                    ++type;
                    break;
                case 2:
                    if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                    behaviorlist.push_back(line);
                    behaviorlist.back().linecount = linenum + i;
                    break;
                case 3:
                    childActive           = line;
                    childActive.linecount = linenum + i;
                    ++type;
                    break;
                default:
                    break;
            }
        }

        return;
    }
}

void AnimDataProject_Condt::modify(const ModCode& modcode, const VecStr& storeline)
{
    unsigned int bhvrCount = 0;
    short type             = 0;
    bool edited            = false;
    bool originalopen      = false;
    bool editopen          = false;

    for (unsigned int i = 1; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        if (line.find("<!--") != NOT_FOUND)
        {
            if (!edited && line.find("<!-- MOD_CODE", 0) != NOT_FOUND
                && line.find("OPEN -->", 0) != NOT_FOUND)
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
                if (edited) projectActive.addCond(nemesis::LinkedVar(line), modcode, nemesis::MOD_CODE, i);

                ++type;
                break;
            case 1:
                // behavior txt count
                ++type;
                break;
            case 2:
                if (i + 1 < storeline.size() && isOnlyNumber(storeline[i + 1])) ++type;

                if (edited)
                {
                    if (bhvrCount < behaviorlist.size())
                        behaviorlist[bhvrCount].addCond(line, modcode, nemesis::MOD_CODE);
                    else
                        behaviorlist.push_back(nemesis::CondVar<string>(line, modcode, nemesis::MOD_CODE));
                }

                ++bhvrCount;
                break;
            case 3:
                if (edited) childActive.addCond(line, modcode, nemesis::MOD_CODE);
                ++type;
                break;
            default:
                break;
        }
    }
}

AnimDataPack_Condt&
AnimDataProject_Condt::aadd(const Header& header, const ModCode& modcode, nemesis::CondType type)
{
    if (modcode != "original")
    {
        using DP = std::pair<Header, nemesis::LinkedVar<AnimDataPack_Condt>>;
        animdatalist.push_back(DataPackCondt<AnimDataPack_Condt>(nemesis::CondVar<DP>(DP(), modcode, type)));
        animdatalist.back().nestedcond.back().rawlist.back().raw->first = header;
        return *animdatalist.back().nestedcond.back().rawlist.back().raw->second.raw;
    }

    animdatalist.push_back(DataPackCondt<AnimDataPack_Condt>());
    animdatalist.back().raw->first = header;
    return *animdatalist.back().raw->second.raw;
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
        animdatalist.push_back(DataPackCondt<AnimDataPack_Condt>(nemesis::CondVar(DP(), modcode, type)));
        auto& refLinked     = animdatalist.back().nestedcond.back().rawlist.back();
        refLinked.linecount = linenum;
        refPair             = refLinked.raw;
    }
    else
    {
        animdatalist.push_back(DataPackCondt<AnimDataPack_Condt>());
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
    if (modcode != "original")
    {
        using DP = std::pair<Header, nemesis::LinkedVar<InfoDataPack_Condt>>;
        infodatalist.push_back(DataPackCondt<InfoDataPack_Condt>(nemesis::CondVar(DP(), modcode, type)));
        infodatalist.back().nestedcond.back().rawlist.back().raw->first = header;
        return *infodatalist.back().nestedcond.back().rawlist.back().raw->second.raw;
    }

    infodatalist.push_back(DataPackCondt<InfoDataPack_Condt>());
    infodatalist.back().raw->first = header;
    return *infodatalist.back().raw->second.raw;
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
        infodatalist.push_back(DataPackCondt<InfoDataPack_Condt>(nemesis::CondVar(DP(), modcode, type)));
        auto& refLinked     = infodatalist.back().nestedcond.back().rawlist.back();
        refLinked.linecount = linenum;
        refPair             = refLinked.raw;
    }
    else
    {
        infodatalist.push_back(DataPackCondt<InfoDataPack_Condt>());
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

        if (each.raw->first + "~" + *each.raw->second.raw->uniquecode.raw == header)
        {
            if (modcode == "original") return &*each.raw->second.raw;

            for (auto& eachcond : each.raw->second.nestedcond)
            {
                if (eachcond.conditions == modcode) return &*eachcond.rawlist.back().raw;
            }
        }
    }

    return nullptr;
}

InfoDataPack_Condt* AnimDataProject_Condt::ifind(const Header& header, const ModCode& modcode)
{
    for (auto& each : infodatalist)
    {
        if (!each.raw) continue;

        if (each.raw->first == header)
        {
            if (modcode == "original") return &*each.raw->second.raw;

            for (auto& eachcond : each.raw->second.nestedcond)
            {
                if (eachcond.conditions == modcode) return &*eachcond.rawlist.back().raw;
            }
        }
    }

    return nullptr;
}

void AnimDataProject_Condt::getlines(VecStr& storeline) 
{
    // project status
    shared_ptr<VecStr> output = getlinkedline(projectActive);

    // behavior count
    output->push_back(to_string(behaviorlist.size()));

    // behavior list
    for (auto& each : behaviorlist)
    {
        getlinkedline(each, *output);
    }

    // child status
    getlinkedline(childActive, *output);

    for (auto& each : animdatalist)
    {

    }
}
