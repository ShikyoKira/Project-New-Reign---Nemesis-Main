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
    Vec<nemesis::LinkedVar<std::string>> behaviorlist;
    nemesis::LinkedVar<std::string> childActive;

    Vec<DataPackCondt<AnimDataPack_Condt>> animdatalist;
    Vec<DataPackCondt<InfoDataPack_Condt>> infodatalist;

    AnimDataProject_Condt() {}
    AnimDataProject_Condt(const VecNstr& storeline);

    void update(const ModCode& modcode, const VecNstr& storeline, size_t linenum);
    void modify(const ModCode& modcode, const VecNstr& storeline);

    AnimDataPack_Condt& aadd(const Header& header,
                             const ModCode& modcode = "original",
                             nemesis::CondType type = nemesis::CondType::NONE);
    AnimDataPack_Condt& aadd(const Header& header,
                             const ModCode& modcode,
                             const VecNstr& storeline,
                             size_t linenum,
                             nemesis::CondType type = nemesis::CondType::NONE);
    InfoDataPack_Condt& iadd(const Header& header,
                             const ModCode& modcode = "original",
                             nemesis::CondType type = nemesis::CondType::NONE);
    InfoDataPack_Condt& iadd(const Header& header,
                             const ModCode& modcode,
                             const VecNstr& storeline,
                             size_t linenum,
                             nemesis::CondType type = nemesis::CondType::NONE);

    nemesis::LinkedVar<AnimDataPack_Condt>* afindlist(const Header& header);
    nemesis::LinkedVar<InfoDataPack_Condt>* ifindlist(const Header& header);

    AnimDataPack_Condt* afind(const Header& header, const ModCode& modcode);
    InfoDataPack_Condt* ifind(const Header& header, const ModCode& modcode);

    bool
    ReadUniqueCodeFIle(const std::filesystem::path& uniquecode_path, const std::string& code, VecNstr& lines);
    bool AddPatch(const std::filesystem::path& uniquecode_path,
                  const std::string& code,
                  const std::string& projectname);
    bool AddExistingPatch(const std::filesystem::path& uniquecode_path,
                          const std::string& code,
                          const std::string& projectname);
    bool AddExistingAnimDataPatch(const std::filesystem::path& uniquecode_path,
                                  const std::string& code,
                                  const std::string& projectname);
    bool AddExistingInfoDataPatch(const std::filesystem::path& uniquecode_path,
                                  const std::string& code,
                                  const std::string& projectname);
    bool AddNewPatch(const std::filesystem::path& uniquecode_path,
                     const std::string& code,
                     const std::string& projectname);
    bool AddNewAnimDataPatch(const std::filesystem::path& uniquecode_path,
                             const std::string& code,
                             const std::string& projectname);
    bool AddNewInfoDataPatch(const std::filesystem::path& uniquecode_path,
                             const std::string& code,
                             const std::string& projectname);

    void getlines(VecStr& storeline);
};
