#pragma once

#include <deque>

#include "utilities/line.h"

#include "update/animsetdata/animsetdatapackcond.h"
#include "update/animsetdata/animsetdatacrc32packcond.h"

struct AnimSetData
{
    using DataStr       = std::string;
    using ModCode       = std::string;

    struct WeapType
    {
        nemesis::LinkedVar<std::string> name;
        nemesis::LinkedVar<std::string> from;
        nemesis::LinkedVar<std::string> to;

        void getlines(VecStr& storeline);
    };

    nemesis::LinkedVar<std::string> name;
    std::vector<nemesis::LinkedVar<DataStr>> equiplist;
    std::vector<nemesis::LinkedVar<WeapType>> typelist;
    std::vector<nemesis::LinkedVar<AnimSetPack>> animlist;
    std::vector<nemesis::LinkedVar<AnimSetCRC32Pack>> crc32list;

    AnimSetData(std::string _name, size_t num);

    void importData(std::filesystem::path path, size_t& num, const VecNstr& storeline);

    void importData(std::filesystem::path path,
                    size_t& num,
                    const VecNstr& storeline,
                    const ModCode& format);

    void importDataTemplate(std::filesystem::path path,
                            size_t& num,
                            const VecNstr& storeline,
                            const ModCode& format);

    void getlines(VecStr& storeline);
    void getname(VecStr& storeline);
    std::string getfirstname();
    std::string getfirstname(nemesis::LinkedVar<std::string> _name);

private:
    std::shared_ptr<DataStr> addEquip(const DataStr& equip, size_t num);
    std::shared_ptr<DataStr>
    addEquip(const DataStr& equip, size_t num, const nemesis::CondDetails& condition);
    std::shared_ptr<WeapType> addType(const WeapType& type, size_t num);
    std::shared_ptr<WeapType>
    addType(const WeapType& type, size_t num, const nemesis::CondDetails& condition);
    std::shared_ptr<AnimSetPack> addAnimPack(const AnimSetPack& animpack, size_t num);
    std::shared_ptr<AnimSetPack>
    addAnimPack(const AnimSetPack& animpack, size_t num, const nemesis::CondDetails& condition);
    std::shared_ptr<AnimSetCRC32Pack> addCrc32Pack(const AnimSetCRC32Pack& crc32pack, size_t num);
    std::shared_ptr<AnimSetCRC32Pack> addCrc32Pack(const AnimSetCRC32Pack& crc32pack,
                                                   size_t num, const nemesis::CondDetails& condition);

    template <typename BaseData>
    std::shared_ptr<BaseData>
    addData(std::vector<nemesis::LinkedVar<BaseData>>& target, const BaseData& type, size_t num)
    {
        target.push_back(nemesis::LinkedVar<BaseData>(type, num));
        return target.back().raw;
    }

    template <typename BaseData>
    std::shared_ptr<BaseData> addData(std::vector<nemesis::LinkedVar<BaseData>>& target,
                                      const BaseData& type,
                                      size_t num,
                                      const nemesis::CondDetails& condition)
    {
        target.push_back(nemesis::CondVar(nemesis::LinkedVar<BaseData>()));
        target.back().linecount          = 0;
        auto& curcond                    = target.back().backCond();
        curcond.conditions               = condition.condition;
        curcond.conditionType            = condition.type;
        curcond.linenum                  = num;
        curcond.rawlist.back().linecount = num;
        curcond.rawlist.back().raw       = std::make_shared<BaseData>(type);
        return curcond.rawlist.back().raw;
    }

    void importEquip(std::filesystem::path path,
                     size_t& num,
                     const VecNstr& storeline,
                     nemesis::CondCheckFunc condfunc,
                     const std::string& format);

    void importType(std::filesystem::path path,
                    size_t& num,
                    const VecNstr& storeline,
                    nemesis::CondCheckFunc condfunc,
                    const std::string& format);

    void importAttackSetData(std::filesystem::path path,
                             size_t& num,
                             const VecNstr& storeline,
                             nemesis::CondCheckFunc condfunc,
                             const std::string& format);

    void importCrc32Pack(std::filesystem::path path,
                         size_t& num,
                         const VecNstr& storeline,
                         nemesis::CondCheckFunc condfunc,
                         const std::string& format);

    uint getEndAttackSetData(std::filesystem::path path,
                             const VecNstr& storeline,
                             nemesis::CondCheckFunc condfunc,
                             const std::string& format);

    std::vector<uint> AnimSetData::getAttackSetDataSections(uint size,
                                                            int num,
                                                            std::filesystem::path path,
                                                            const VecNstr& storeline,
                                                            nemesis::CondCheckFunc condfunc,
                                                            const std::string& condition);

    void exportEquip(VecStr& storeline);
    void exportType(VecStr& storeline);
    void exportAnimPack(VecStr& storeline);
    void exportCrc32Pack(VecStr& storeline);
};

void getLinkedLines(const nemesis::LinkedVar<AnimSetData>& linkeddata, VecStr& storeline, VecStr& namelist);
