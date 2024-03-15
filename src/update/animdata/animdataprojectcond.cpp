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

AnimDataProject_Condt::AnimDataProject_Condt(const VecNstr& storeline)
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
                projectActive           = line.ToString();
                projectActive.linecount = line.GetLineNumber();
                ++type;
                break;
            }
            case 1:
            {
                // behavior txt count
                if (linecounter < storeline.size() && isOnlyNumber(storeline[linecounter])) ++type;

                ++type;
                break;
            }
            case 2:
            {
                if (linecounter < storeline.size() && isOnlyNumber(storeline[linecounter])) ++type;

                behaviorlist.emplace_back(line.ToString());
                behaviorlist.back().linecount = line.GetLineNumber();
                break;
            }
            case 3:
            {
                childActive = line.ToString();
                childActive.linecount = line.GetLineNumber();
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

void AnimDataProject_Condt::update(const ModCode& modcode, const VecNstr& storeline, size_t linenum)
{
    if (modcode != "original") return;

    short type = 0;

    for (size_t i = 1; i < storeline.size(); ++i)
    {
        auto& line = storeline.at(i);

        switch (type)
        {
            case 0:
            {
                projectActive           = line.ToString();
                projectActive.linecount = line.GetLineNumber();
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

                behaviorlist.emplace_back(line.ToString());
                behaviorlist.back().linecount = line.GetLineNumber();
                break;
            }
            case 3:
            {
                childActive           = line.ToString();
                childActive.linecount = line.GetLineNumber();
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

void AnimDataProject_Condt::modify(const ModCode& modcode, const VecNstr& storeline)
{
    size_t bhvrCount    = 0;
    short type        = 0;
    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;

    for (size_t i = 1; i < storeline.size(); ++i)
    {
        auto& line = storeline[i];

        if (line.find(ns::OpenComment()) != NOT_FOUND)
        {
            if (!edited && line.find(ns::ModCode(modcode)) != NOT_FOUND)
            {
                edited = true;
            }
            else if (line.find(ns::ModOriginal()) != NOT_FOUND)
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
                if (edited)
                {
                    projectActive.addCond(nemesis::LinkedVar(line.ToString()),
                                          modcode,
                                          nemesis::CondType::MOD_CODE,
                                          line.GetLineNumber());
                }

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
                        behaviorlist[bhvrCount].addCond(line.ToString(), modcode, nemesis::CondType::MOD_CODE);
                    }
                    else
                    {
                        behaviorlist.emplace_back(
                            nemesis::CondVar<string>(line, modcode, nemesis::CondType::MOD_CODE));
                    }
                }

                ++bhvrCount;
                break;
            }
            case 3:
            {
                if (edited)
                {
                    childActive.addCond(
                        line.ToString(), modcode, nemesis::CondType::MOD_CODE, line.GetLineNumber());
                }

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
    using DP = Pair<Header, nemesis::LinkedVar<AnimDataPack_Condt>>;

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
                                                const VecNstr& storeline,
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
    refPair->second.raw       = make_shared<AnimDataPack_Condt>(storeline);
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
                                                const VecNstr& storeline,
                                                size_t linenum,
                                                nemesis::CondType type)
{
    using DP = Pair<Header, nemesis::LinkedVar<InfoDataPack_Condt>>;
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
    refPair->second.raw       = make_shared<InfoDataPack_Condt>(storeline);
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

bool AnimDataProject_Condt::ReadUniqueCodeFIle(const std::filesystem::path& uniquecode_path,
                                               const std::string& code,
                                               VecNstr& lines)
{
    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;

    return GetFileLines(uniquecode_path, lines, [&](std::string& line) {
        if (line.find("<!--") != NOT_FOUND)
        {
            if (!edited && line.find(ns::ModCode(code)) != NOT_FOUND)
            {
                edited = true;
            }
            else if (line.find(ns::ModOriginal()) != NOT_FOUND)
            {
                originalopen = true;
            }
            else if (line.find(ns::Close()) != NOT_FOUND)
            {
                originalopen = false;
                edited       = false;
            }

            return false;
        }

        return !originalopen;
    });
}

bool AnimDataProject_Condt::AddPatch(const std::filesystem::path& uniquecode_path,
                                     const std::string& code,
                                     const std::string& projectname)
{
    if (uniquecode_path.stem().string().find(code + "$") == NOT_FOUND)
    {
        return AddExistingPatch(uniquecode_path, code, projectname);
    }

    return AddNewPatch(uniquecode_path, code, projectname);
}

bool AnimDataProject_Condt::AddExistingPatch(const std::filesystem::path& uniquecode_path,
                                             const std::string& code,
                                             const std::string& projectname)
{
    if (!isOnlyNumber(uniquecode_path.stem().string()))
    {
        return AddExistingAnimDataPatch(uniquecode_path, code, projectname);
    }

    return AddExistingInfoDataPatch(uniquecode_path, code, projectname);
}

bool AnimDataProject_Condt::AddExistingAnimDataPatch(const std::filesystem::path& uniquecode_path,
                                                     const std::string& code,
                                                     const std::string& projectname)
{
    VecNstr lines;

    if (!ReadUniqueCodeFIle(uniquecode_path, code, lines)) return false;

    std::string uniquecode = uniquecode_path.stem().string();

    if (!hasAlpha(lines[0])) ErrorMessage(3006, projectname, uniquecode);

    nemesis::smatch smatch;
    std::string filename = uniquecode_path.stem().string();

    if (!nemesis::regex_search(filename, smatch, nemesis::regex("^([^~]+)~([0-9]+)$")))
    {
        ErrorMessage(2004, uniquecode);
    }

    auto animdata = smatch.str(1);
    auto* curptr  = afindlist(animdata);

    if (!curptr) ErrorMessage(2027, animdata, uniquecode);

    auto* infoptr = ifindlist(smatch.str(2));

    if (!infoptr) ErrorMessage(2004, uniquecode);  

    curptr->addCond(nemesis::LinkedVar(AnimDataPack_Condt(lines)), code, nemesis::CondType::MOD_CODE);
    return true;
}

bool AnimDataProject_Condt::AddExistingInfoDataPatch(const std::filesystem::path& uniquecode_path,
                                                     const std::string& code,
                                                     const std::string& projectname)
{
    VecNstr lines;

    if (!ReadUniqueCodeFIle(uniquecode_path, code, lines)) return false;

    std::string filename = uniquecode_path.stem().string();

    // info data confirmation
    if (!isOnlyNumber(lines[0].ToString())) ErrorMessage(3006, projectname, filename);

    auto* curptr = ifindlist(filename);

    if (!curptr) ErrorMessage(2004, uniquecode_path);

    curptr->addCond(nemesis::LinkedVar(InfoDataPack_Condt(lines)), code, nemesis::CondType::MOD_CODE);
    return true;
}

bool AnimDataProject_Condt::AddNewPatch(const std::filesystem::path& uniquecode_path,
                                        const std::string& code,
                                        const std::string& projectname)
{
    if (!isOnlyNumber(uniquecode_path.stem().string()))
    {
        return AddNewAnimDataPatch(uniquecode_path, code, projectname);
    }

    return AddNewInfoDataPatch(uniquecode_path, code, projectname);
}

bool AnimDataProject_Condt::AddNewAnimDataPatch(const std::filesystem::path& uniquecode_path,
                                        const std::string& code,
                                        const std::string& projectname)
{
    VecNstr lines;

    if (!ReadUniqueCodeFIle(uniquecode_path, code, lines)) return false;

    std::string filename = uniquecode_path.stem().string();

    // anim data
    if (!hasAlpha(lines[0])) ErrorMessage(3006, projectname, filename);

    nemesis::LinkedVar<AnimDataPack_Condt>* animDataPtr = afindlist(filename);

    // new anim data
    if (!animDataPtr)
    {
        auto pair     = make_pair(std::string(lines[0]), nemesis::LinkedVar(AnimDataPack_Condt(lines)));
        auto condpair = nemesis::CondVar(pair, code, nemesis::CondType::MOD_CODE);
        animdatalist.push_back(condpair);
        return true;
    }

    auto shptr = nemesis::CondVar(AnimDataPack_Condt(lines));
    //(*animDataPtr).addCond(make_shared<AnimDataPack_Condt>(lines), modcode, nemesis::CondType::MOD_CODE); does the same thing as line below
    (*animDataPtr).nestedcond.push_back(shptr);
    return true;
}

bool AnimDataProject_Condt::AddNewInfoDataPatch(const std::filesystem::path& uniquecode_path,
                                                const std::string& code,
                                                const std::string& projectname)
{
    std::string filename = uniquecode_path.stem().string();

    if (!nemesis::regex_search(filename, nemesis::regex(code + "[$]([0-9]+)")))
    {
        ErrorMessage(2004, uniquecode_path);
    }

    VecNstr lines;
    
    if (!ReadUniqueCodeFIle(uniquecode_path, code, lines)) return false;

    nemesis::LinkedVar<InfoDataPack_Condt>* infoDataPtr = ifindlist(filename);

    // new info data
    if (!infoDataPtr)
    {
        auto pair     = make_pair(std::string(lines[0]), nemesis::LinkedVar(InfoDataPack_Condt(lines)));
        auto condpair = nemesis::CondVar(pair, code, nemesis::CondType::MOD_CODE);
        infodatalist.push_back(condpair);
        return true;
    }

    auto shptr = nemesis::CondVar(InfoDataPack_Condt(lines), code, nemesis::CondType::MOD_CODE);
    //(*infoDataPtr).addCond(make_shared<InfoDataPack_Condt>(storeline), modcode, nemesis::CondType::MOD_CODE); does the same thing as line below
    (*infoDataPtr).nestedcond.push_back(shptr);
    return true;
}

void AnimDataProject_Condt::getlines(VecStr& storeline)
{
    // project status
    SPtr<VecStr> output = getLinkedLines(projectActive);

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

    if (!animdatalist.empty())
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
