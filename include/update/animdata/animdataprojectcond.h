#pragma once

#include "update/animdata/animdatapackcond.h"
#include "update/animdata/infodatapackcond.h"

struct AnimDataProject_Condt
{
    using Header  = std::string;
    using ModCode = std::string;

    template <typename Pack>
    using DataPackCondt = nemesis::LinkedVar<std::pair<Header, nemesis::LinkedVar<Pack>>>;

    nemesis::LinkedVar<std::string> projectActive;
    std::vector<nemesis::LinkedVar<std::string>> behaviorlist;
    nemesis::LinkedVar<std::string> childActive;

    std::vector<DataPackCondt<AnimDataPack_Condt>> animdatalist;
    std::vector<DataPackCondt<InfoDataPack_Condt>> infodatalist;

    AnimDataProject_Condt() {}
    AnimDataProject_Condt(const VecStr& storeline, size_t linenum = 1);

    void update(const ModCode& modcode, const VecStr& storeline, size_t linenum);
    void modify(const ModCode& modcode, const VecStr& storeline);

    AnimDataPack_Condt&
    aadd(const Header& header, const ModCode& modcode = "original", nemesis::CondType type = nemesis::NONE);
    AnimDataPack_Condt& aadd(const Header& header,
                             const ModCode& modcode,
                             const VecStr& storeline,
                             size_t linenum,
                             nemesis::CondType type = nemesis::NONE);
    InfoDataPack_Condt&
    iadd(const Header& header, const ModCode& modcode = "original", nemesis::CondType type = nemesis::NONE);
    InfoDataPack_Condt& iadd(const Header& header,
                             const ModCode& modcode,
                             const VecStr& storeline,
                             size_t linenum,
                             nemesis::CondType type = nemesis::NONE);

    nemesis::LinkedVar<AnimDataPack_Condt>* afindlist(const Header& header);
    nemesis::LinkedVar<InfoDataPack_Condt>* ifindlist(const Header& header);

    AnimDataPack_Condt* afind(const Header& header, const ModCode& modcode);
    InfoDataPack_Condt* ifind(const Header& header, const ModCode& modcode);

    void getlines(VecStr& storeline);
};
