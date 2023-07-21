#include <functional>

#include "utilities/alphanum.hpp"
#include "utilities/linkedparser.h"
#include "utilities/conditionsyntax.h"

#include "update/animsetdata/animsetdata.h"

using namespace std;

namespace ns = nemesis::syntax;

void getLinkedLines(const nemesis::LinkedVar<AnimSetData::WeapType>& linkedtype, VecStr& storeline);

void AnimSetData::WeapType::Getlines(VecStr& storeline)
{
    getLinkedLines(name, storeline);
    getLinkedLines(from, storeline);
    getLinkedLines(to, storeline);
}

bool AnimSetData::WeapType::AddName(const nemesis::Line& _name)
{
    name = ToLinkedString(_name);
    return hasAlpha(_name);
}

bool AnimSetData::WeapType::AddFrom(const nemesis::Line& _from)
{
    from = ToLinkedString(_from);
    return isOnlyNumber(_from);
}

bool AnimSetData::WeapType::AddTo(const nemesis::Line& _to)
{
    to = ToLinkedString(_to);
    return isOnlyNumber(_to);
}

AnimSetData::AnimSetData(string _name, size_t num)
{
    name = _name;
    name.linecount = num;
}

void AnimSetData::ImportData(filesystem::path path,
                             size_t& num,
                             const VecNstr& storeline)
{
    Parser parser(*this, storeline);
    parser.SetPath(path);
    parser.SetStartIndex(num);

    parser.AddEquipList();
    parser.AddTypeList();
    parser.AddAttackSetDataList();
    parser.AddCrc32PackList();
    num = parser.GetNum();
}

void AnimSetData::ImportData(filesystem::path path,
                             size_t& num,
                             const VecNstr& storeline,
                             const ModCode& format)
{
    Parser parser(*this, storeline);
    parser.SetPath(path);
    parser.SetStartIndex(num);
    parser.SetCondCheckFunc(nemesis::CondDetails::modCheck);
    parser.SetFormat(format);

    parser.ImportEquip();
    parser.ImportType();
    parser.ImportAttackSetData();
    parser.ImportCrc32Pack();
    num = parser.GetNum();
}

void AnimSetData::ImportDataTemplate(filesystem::path path,
                                     size_t& num,
                                     const VecNstr& storeline,
                                     const ModCode& format)
{
    Parser parser(*this, storeline);
    parser.SetPath(path);
    parser.SetStartIndex(num);
    parser.SetCondCheckFunc(nemesis::CondDetails::templateCheck);
    parser.SetFormat(format);

    parser.ImportEquip();
    parser.ImportType();
    parser.ImportAttackSetData();
    parser.ImportCrc32Pack();
    num = parser.GetNum();
}

SPtr<string>& AnimSetData::AddEquip(const DataStr& equip, size_t num)
{
    return AddData(equiplist, equip, num);
}

SPtr<string>&
AnimSetData::AddEquip(const DataStr& equip, size_t num, const nemesis::CondDetails& condition)
{
    return AddData(equiplist, equip, num, condition);
}

SPtr<string>&
AnimSetData::AddEquip(const DataStr& equip, size_t num, const nemesis::ConditionInfo& conditioninfo)
{
    return AddData(equiplist, equip, num, conditioninfo);
}

SPtr<AnimSetData::WeapType>& AnimSetData::AddType(const WeapType& type, size_t num)
{
    return AddData(typelist, type, num);
}

SPtr<AnimSetData::WeapType>&
AnimSetData::AddType(const WeapType& type, size_t num, const nemesis::CondDetails& condition)
{
    return AddData(typelist, type, num, condition);
}

SPtr<AnimSetData::WeapType>&
AnimSetData::AddType(const WeapType& type, size_t num, const nemesis::ConditionInfo& conditioninfo)
{
    return AddData(typelist, type, num, conditioninfo);
}

SPtr<AnimSetPack>& AnimSetData::AddAnimPack(const AnimSetPack& animpack, size_t num)
{
    return AddData(animlist, animpack, num);
}

SPtr<AnimSetPack>&
AnimSetData::AddAnimPack(const AnimSetPack& animpack, size_t num, const nemesis::CondDetails& condition)
{
    return AddData(animlist, animpack, num, condition);
}

SPtr<AnimSetPack>&
AnimSetData::AddAnimPack(const AnimSetPack& animpack, size_t num, const nemesis::ConditionInfo& conditioninfo)
{
    return AddData(animlist, animpack, num, conditioninfo);
}

SPtr<AnimSetCRC32Pack>& AnimSetData::AddCrc32Pack(const AnimSetCRC32Pack& crc32pack, size_t num)
{
    return AddData(crc32list, crc32pack, num);
}

SPtr<AnimSetCRC32Pack>& AnimSetData::AddCrc32Pack(const AnimSetCRC32Pack& crc32pack,
                                                       size_t num,
                                                       const nemesis::CondDetails& condition)
{
    return AddData(crc32list, crc32pack, num, condition);
}

SPtr<AnimSetCRC32Pack>& AnimSetData::AddCrc32Pack(const AnimSetCRC32Pack& crc32pack,
                                                 size_t num,
                                                 const nemesis::ConditionInfo& conditioninfo)
{
    return AddData(crc32list, crc32pack, num, conditioninfo);
}

template <typename BaseData>
SPtr<BaseData>&
AnimSetData::AddData(Vec<nemesis::LinkedVar<BaseData>>& tarGet, const BaseData& type, size_t num)
{
    tarGet.push_back(nemesis::LinkedVar<BaseData>(type, num));
    return tarGet.back().raw;
}

template <typename BaseData>
SPtr<BaseData>& AnimSetData::AddData(Vec<nemesis::LinkedVar<BaseData>>& tarGet,
                                     const BaseData& data,
                                     size_t num,
                                     const nemesis::CondDetails& condition)
{
    tarGet.push_back(nemesis::CondVar(nemesis::LinkedVar<BaseData>()));
    tarGet.back().linecount          = 0;
    auto& curcond                    = tarGet.back().backCond();
    curcond.conditions               = condition.condition;
    curcond.conditionType            = condition.type;
    curcond.linenum                  = num;
    curcond.rawlist.back().linecount = num;
    curcond.rawlist.back().raw       = std::make_shared<BaseData>(data);
    return curcond.rawlist.back().raw;
}

template <typename BaseData>
SPtr<BaseData>& AnimSetData::AddData(Vec<nemesis::LinkedVar<BaseData>>& tarGet,
                                     const BaseData& data,
                                     size_t num,
                                     const nemesis::ConditionInfo& conditioninfo)
{
    tarGet.push_back(nemesis::CondVar(nemesis::LinkedVar<BaseData>()));
    tarGet.back().linecount          = 0;
    auto& curcond                    = tarGet.back().backCond();
    curcond.conditions               = conditioninfo.GetCondition();
    curcond.conditionType            = conditioninfo.GetType();
    curcond.linenum                  = num;
    curcond.rawlist.back().linecount = num;
    curcond.rawlist.back().raw       = std::make_shared<BaseData>(data);
    return curcond.rawlist.back().raw;
}

template <typename BaseData>
SPtr<BaseData>& AnimSetData::AddData(Vec<nemesis::LinkedVar<BaseData>>& tarGet,
                                     const Vec<nemesis::LinkedVar<BaseData>>& datalist,
                                     size_t num,
                                     const nemesis::ConditionInfo& conditioninfo)
{
    tarGet.push_back(nemesis::CondVar(nemesis::LinkedVar<BaseData>()));
    tarGet.back().linecount = 0;
    auto& curcond           = tarGet.back().backCond();
    curcond.conditions      = conditioninfo.GetCondition();
    curcond.conditionType   = conditioninfo.GetType();
    curcond.linenum         = num;
    curcond.rawlist         = datalist;
    return curcond.rawlist.back().raw;
}

void AnimSetData::Getlines(VecStr& storeline) const
{
    storeline.emplace_back("V3");
    ExportEquip(storeline);
    ExportType(storeline);
    ExportAnimPack(storeline);
    ExportCrc32Pack(storeline);
}

void AnimSetData::Getname(VecStr& storeline)
{
    getLinkedLines(name, storeline);
}

string AnimSetData::Getfirstname()
{
    if (name.raw) return *name.raw;

    for (auto& cond : name.nestedcond)
    {
        for (auto& raw : cond.rawlist)
        {
            return Getfirstname(raw);
        }
    }

    return "";
}

string AnimSetData::Getfirstname(nemesis::LinkedVar<string> _name)
{
    if (_name.raw) return *_name.raw;

    for (auto& cond : _name.nestedcond)
    {
        for (auto& raw : cond.rawlist)
        {
            return Getfirstname(raw);
        }
    }

    return "";
}

void AnimSetData::ExportEquip(VecStr& storeline) const
{
    storeline.emplace_back(to_string(equiplist.size()));
    getLinkedLines(equiplist, storeline);
}

void AnimSetData::ExportType(VecStr& storeline) const
{
    storeline.emplace_back(to_string(typelist.size()));
    string last;

    for (auto& type : typelist)
    {
        VecStr temp;
        getLinkedLines(type, temp);
        combineLines(last, temp, storeline);
    }
}

void AnimSetData::ExportAnimPack(VecStr& storeline) const
{
    storeline.emplace_back(to_string(animlist.size()));
    string last;

    for (auto& anim : animlist)
    {
        VecStr temp;
        getLinkedLines(anim, temp);
        combineLines(last, temp, storeline);
    }
}

void AnimSetData::ExportCrc32Pack(VecStr& storeline) const
{
    storeline.emplace_back(to_string(crc32list.size()));
    string last;

    for (auto& crc32 : crc32list)
    {
        VecStr temp;
        getLinkedLines(crc32, temp);
        combineLines(last, temp, storeline);
    }
}

AnimSetData::Parser::Parser(AnimSetData& base, const VecNstr& _storeline)
    : host(base)
    , storeline(_storeline)
{
}

void AnimSetData::Parser::SetFormat(const string& _format) noexcept
{
    format = _format;
}

void AnimSetData::Parser::SetPath(const filesystem::path& _path) noexcept
{
    path = _path;
}

void AnimSetData::Parser::SetStartIndex(const size_t& startIndex) noexcept
{
    num = startIndex;
}

void AnimSetData::Parser::SetCondCheckFunc(const nemesis::CondCheckFunc& _condfunc) noexcept
{
    condfunc = _condfunc;
}

void AnimSetData::Parser::AddEquipList()
{
    for (size_t i = num; i < storeline.size(); ++i)
    {
        auto& line = storeline[i];

        if (isOnlyNumber(line))
        {
            num = i + 1;
            return;
        }

        host.AddEquip(line, line.GetLineNumber());
    }
}

void AnimSetData::Parser::AddTypeList()
{
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (isOnlyNumber(storeline[i]))
        {
            num = i + 1;
            return;
        }

        AnimSetData::WeapType wt;
        wt.AddName(storeline[i++]);
        wt.AddFrom(storeline[i++]);
        wt.AddTo(storeline[i]);
        host.AddType(wt, storeline[i].GetLineNumber());
    }
}

void AnimSetData::Parser::AddAttackSetDataList()
{
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (isOnlyNumber(storeline[i]))
        {
            num = i + 1;
            return;
        }

        AnimSetPack pack;
        auto& name = storeline[i++];
        auto& unknown = storeline[i++];
        auto size = stoi(storeline[i]);

        if (!pack.AddName(name)) ErrorMessage(5020, path, name.GetLineNumber());

        if (!pack.AddUnknown(unknown)) ErrorMessage(5020, path, unknown.GetLineNumber());

        size_t length = stoi(storeline[i]);

        for (size_t k = i + 1; k < length + i + 1; ++k)
        {
            pack.AddAttackSetData(storeline[k]);
        }

        host.AddAnimPack(pack, pack.name.linecount);
        i += length;
    }
}

void AnimSetData::Parser::AddCrc32PackList()
{
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (i + 1 >= storeline.size() || storeline[i] == "V3" || !isOnlyNumber(storeline[i + 1]))
        {
            num = i;
            return;
        }

        AnimSetCRC32Pack pack;
        auto& filepathref = storeline[i++];
        auto& filenameref = storeline[i++];
        auto& extref  = storeline[i];

        if (!pack.AddFilePath(filepathref)) ErrorMessage(5020, path, filepathref.GetLineNumber());

        if (!pack.AddFileName(filenameref)) ErrorMessage(5020, path, filenameref.GetLineNumber());

        if (!pack.AddExtension(extref)) ErrorMessage(5020, path, extref.GetLineNumber());

        host.AddCrc32Pack(pack, filepathref.GetLineNumber());
    }
}

void AnimSetData::Parser::ImportEquip()
{
    //bool edited       = false;
    //bool originalopen = false;
    //auto& equiplist   = host.equiplist;

    index        = 0;
    tobedeleted = nullptr;

    //deque<nemesis::Line>* edits = nullptr;
    EquipConditionScope cscope(format, path);
    auto& stream = equipstream;
    auto parser  = nemesis::LinkedParser<DataStr>::CreateParser(host.equiplist, format, path);

    parser.SetPointingListFunc([&parser]() {
        ++parser.index;
    });
    parser.SetPointingOriginalFunc([&parser]() {
        auto& predelete = parser.tobedeleted;
        auto& linkedvar = (*parser.stream.front())[parser.index++];
        auto vline      = nemesis::CondDetails::getOriginalLine(&predelete->GetRefContents());
        auto linkedline = nemesis::ToLinkedString(vline);
        linkedvar.addCond(linkedline, predelete->GetType(), vline.GetLineNumber());
    });
    parser.SetClosingListFunc([&parser]() {
        auto& predelete = parser.tobedeleted;

        switch (predelete->GetType())
        {
            case nemesis::CondType::MOD_CODE:
            case nemesis::CondType::ORIGINAL:
            {
                auto& contents = predelete->GetContents();

                if (contents.empty()) return;

                auto cond      = nemesis::CondVar<DataStr>(*predelete);
                auto linkedvar = nemesis::LinkedVar<DataStr>(cond, predelete->GetLineNumber());
                parser.stream.front()->emplace_back(linkedvar);
                auto& modstream = parser.stream.front()->back().backCond().rawlist;

                for (auto& edit : contents)
                {
                    AnimSetData::AddData<std::string>(modstream, edit, edit.GetLineNumber(), *predelete);
                }

                break;
            }
        }
    });

    // Adding equip
    for (size_t i = num; i < storeline.size(); ++i)
    {
        const auto& line = storeline[i];

        if (isOnlyNumber(line))
        {
            num = i + 1;
            break;
        }

        parser.ParseLine(line);

        if (error) throw nemesis::exception();

        continue;

        /*
        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.empty() && line.find(ns::OpenComment()) == NOT_FOUND)
        {
            ++index;
            continue;
        }

        if (nemesis::CondDetails::originalScope(path, format, line, condtype))
        {
            originalopen = true;
        }
        else if (nemesis::CondDetails::closeScope(path, format, line, condtype))
        {
            for (auto& edit : *edits)
            {
                host.AddEquip(edit, edit.GetLineNumber(), condtype.back());
            }

            originalopen = false;
            condtype.pop_back();
            edits = !condtype.empty() ? &condtype.back().contents : nullptr;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            equiplist[index++].addCond(
                nemesis::LinkedVar(vline.ToString()), condtype.back(), vline.GetLineNumber());
        }
        else if (edits)
        {
            edits->emplace_back(line);
        }
        */

        SPtr<nemesis::ConditionInfo> conditioninfo;

        try
        {
            conditioninfo = cscope.TryGetConditionInfo(line);
        }
        catch (const std::exception&)
        {
            ErrorMessage(1205, format, path, line.GetLineNumber());
        }

        if (!conditioninfo)
        {
            if (cscope.Empty())
            {
                PointingEquip();
            }
            else if (cscope.Back().GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriEquip();
            }
            else
            {
                stream.back()->emplace_back(line.ToString());
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::IF:
            case nemesis::CondType::FOREACH:
            {
                auto cond = nemesis::CondVar<DataStr>(*conditioninfo);
                Vec<nemesis::LinkedVar<DataStr>>* listptr;

                if (stream.empty())
                {
                    auto& currentequip = host.equiplist[index];
                    auto& condequip    = currentequip.addCond(cond);
                    listptr            = &condequip.rawlist;
                }
                else
                {
                    stream.back()->emplace_back(nemesis::LinkedVar<DataStr>(cond, line.GetLineNumber()));
                    listptr = &stream.back()->back().backCond().rawlist;
                }

                stream.emplace_back(listptr);
                break;
            }
            case nemesis::CondType::ELSEIF:
            {
                auto cond    = nemesis::CondVar<DataStr>(*conditioninfo);
                stream.back() = &stream.back()->back().addCond(cond).rawlist;
                break;
            }
            case nemesis::CondType::ELSE:
            {
                auto cond    = nemesis::CondVar<DataStr>(conditioninfo->GetType());
                stream.back() = &stream.back()->back().addCond(cond).rawlist;
                break;
            }
            case nemesis::CondType::ENDIF:
            {
                stream.pop_back();
                break;
            }
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                }

                CloseEquip();
                tobedeleted = nullptr;
                break;
            }
        }
    }

    //if (!condtype.empty())
    //{
    //    ErrorMessage(5024, format, path, condtype.back().contents.front().GetLineNumber() - 1);
    //}
}

void AnimSetData::Parser::ImportType()
{
    bool edited       = false;
    bool originalopen = false;
    auto& typelist    = host.typelist;

    type  = 0;
    index = 0;
    tobedeleted = nullptr;

    //deque<nemesis::Line>* edits = nullptr;
    nemesis::ConditionScope cscope(format, path);

    // Adding type
    for (size_t i = num; i < storeline.size(); ++i)
    {
        const auto& line = storeline[i];

        if (i + 1 == storeline.size())
        {
            num = i + 1;
            break;
        }

        auto conditioninfo = cscope.TryGetConditionInfo(line);

        if (!conditioninfo)
        {
            if (cscope.Empty())
            {
                if (!PointingType(line, i)) return;
            }
            else if (cscope.Back().GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriType();
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                }

                CloseType(line);
                tobedeleted = nullptr;
                break;
            }
        }

        /*
        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.empty() && line.find(ns::OpenComment()) == NOT_FOUND)
        {
            switch (type)
            {
                case 0:
                {
                    if (type == 0 && isOnlyNumber(line))
                    {
                        num = i + 1;
                        return;
                    }
                }
                case 1:
                {
                    ++type;
                    break;
                }
                case 2:
                {
                    type = 0;
                    ++index;
                    break;
                }
            }

            continue;
        }

        if (nemesis::CondDetails::originalScope(path, format, line, condtype))
        {
            originalopen = true;
        }
        else if (nemesis::CondDetails::closeScope(path, format, line, condtype))
        {
            if (type != 0 && !edits->empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

            if (!ImportNewWeapType(edits)) ErrorMessage(5021, format, path, edits->front().GetLineNumber());

            edited       = false;
            originalopen = false;
            condtype.pop_back();
            edits = !condtype.empty() ? &condtype.back().contents : nullptr;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            auto& AddLinkedCond = [&](nemesis::LinkedVar<string>& curline) {
                curline.addCond(nemesis::LinkedVar(vline.ToString()), condtype.back(), vline.GetLineNumber());
            };

            switch (type)
            {
                case 0:
                {
                    AddLinkedCond(typelist[index].raw->name);
                    ++type;
                    break;
                }
                case 1:
                {
                    AddLinkedCond(typelist[index].raw->from);
                    ++type;
                    break;
                }
                case 2:
                {
                    AddLinkedCond(typelist[index].raw->to);
                    type = 0;
                    ++index;
                    break;
                }
            }
        }
        else if (edits)
        {
            edits->emplace_back(line);
        }
        */
    }

    if (type != 0) ErrorMessage(5024, format, path, num);

    //if (!condtype.empty())
    //{
    //    ErrorMessage(5024, format, path, condtype.back().contents.front().GetLineNumber() - 1);
    //}
}

void AnimSetData::Parser::ImportAttackSetData()
{
    bool edited       = false;
    bool originalopen = false;
    auto& animlist    = host.animlist;

    type   = 0;
    index  = 0;
    iindex = 0;
    size   = GetEndAttackSetData();
    points = GetAttackSetDataSections(num, size);
    tobedeleted = nullptr;

    deque<nemesis::Line>* edits = nullptr;
    nemesis::ConditionScope cscope(format, path);

    for (size_t i = num; i < size; ++i)
    {
        const auto& line   = storeline[i];
        auto conditioninfo = cscope.TryGetConditionInfo(line);

        if (!conditioninfo)
        {
            if (cscope.Empty())
            {
                PointingAttackSetData(line);
            }
            else if (cscope.Back().GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriAttackSetData(line);
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                }

                CloseAttackSetData(line);
                tobedeleted = nullptr;
                break;
            }
        }

        /*
        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.empty() && line.find(ns::OpenComment()) == NOT_FOUND)
        {
            PointingAttackSetData(line);
            continue;
        }

        if (nemesis::CondDetails::originalScope(path, format, line, condtype))
        {
            originalopen = true;
        }
        else if (nemesis::CondDetails::closeScope(path, format, line, condtype))
        {
            size_t k = 0;
            Vec<size_t> npoints = GetAttackSetDataSections(0, edits->size(), *edits);

            if (type == 3)
            {
                auto& curAtkSD = animlist[index].raw->attackSetData;

                while (!edits->empty())
                {
                    auto& edit = edits->front();

                    curAtkSD.emplace_back(nemesis::CondVar(edit.ToString(), condtype.back()));
                    curAtkSD.back().linecount                           = edit.GetLineNumber();
                    curAtkSD.back().backCond().linenum                  = edit.GetLineNumber();
                    curAtkSD.back().backCond().rawlist.back().linecount = edit.GetLineNumber();

                    if (npoints.empty()) ErrorMessage(5021, format, path, edit.GetLineNumber());

                    if (npoints.back() == edit.GetLineNumber())
                    {
                        npoints.back();
                        edits->pop_front();
                        type = 0;
                        break;
                    }

                    edits->pop_front();
                }
            }
            else if (type != 0 && !edits->empty())
            {
                ErrorMessage(5021, format, path, line.GetLineNumber());
            }

            if (!ImportNewAnimPack(edits, npoints))
            {
                ErrorMessage(5021, format, path, edits->front().GetLineNumber());
            }

            condtype.pop_back();
            edits        = !condtype.empty() ? &condtype.back().contents : nullptr;
            edited       = false;
            originalopen = false;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            auto& AddLinkedCond = [&](nemesis::LinkedVar<string>& curline)
            {
                curline.addCond(nemesis::LinkedVar(vline.ToString()), condtype.back(), vline.GetLineNumber());
            };

            switch (type)
            {
                case 0:
                {
                    AddLinkedCond(animlist[index].raw->name);
                    ++type;
                    break;
                }
                case 1:
                {
                    AddLinkedCond(animlist[index].raw->unknown);
                    ++type;
                    break;
                }
                case 2:
                {
                    if (points.empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

                    if (points.back() != line.GetLineNumber())
                    {
                        ++type;
                        break;
                    }

                    points.pop_back();
                    type   = 0;
                    iindex = 0;
                    ++index;
                    break;
                }
                case 3:
                {
                    if (points.empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

                    AddLinkedCond(animlist[index].raw->attackSetData[iindex]);
                    ++iindex;

                    if (points.back() != line.GetLineNumber()) break;

                    points.pop_back();
                    type   = 0;
                    iindex = 0;
                    ++index;
                    break;
                }
            }
        }
        else if (edits)
        {
            edits->emplace_back(line);
        }
        */

    }

    num = size + 1;

    if (type != 0) ErrorMessage(5021, format, path, num);

    if (!condtype.empty()) ErrorMessage(5024, format, path, edits->front().GetLineNumber());
}

void AnimSetData::Parser::ImportCrc32Pack()
{
    bool originalopen = false;
    auto& crc32list   = host.crc32list;

    type   = 0;
    index  = 0;
    iindex = 0;
    tobedeleted = nullptr;

    deque<nemesis::Line>* edits = nullptr;
    nemesis::ConditionScope cscope(format, path);

    // Adding CRC32 file path
    for (size_t i = num; i < storeline.size(); ++i)
    {
        const auto& line   = storeline[i];
        auto conditioninfo = cscope.TryGetConditionInfo(line);

        if (!conditioninfo)
        {
            if (cscope.Empty())
            {
                PointingCrc32Pack();
            }
            else if (cscope.Back().GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriCrc32Pack();
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                }

                CloseCrc32Pack();
                tobedeleted = nullptr;
                break;
            }
        }

        /*
        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.empty() && line.find(ns::OpenComment()) == NOT_FOUND)
        {
            switch (type)
            {
                case 0:
                case 1:
                {
                    ++type;
                    break;
                }
                case 2:
                {
                    type = 0;
                    break;
                }
            }

            continue;
        }

        if (nemesis::CondDetails::originalScope(path, format, line, condtype))
        {
            originalopen = true;
        }
        else if (nemesis::CondDetails::closeScope(path, format, line, condtype))
        {
            if (type != 0 && !edits->empty()) ErrorMessage(5021, format, path, edits->back().GetLineNumber());

            if (!ImportNewCRC32Pack(edits)) ErrorMessage(5021, format, path, edits->front().GetLineNumber());

            originalopen = false;
            condtype.pop_back();
            edits = !condtype.empty() ? &condtype.back().contents : nullptr;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);

            if (!isOnlyNumber(vline)) ErrorMessage(5021, format, path, vline.GetLineNumber());

            auto& addCondFunc = [&](nemesis::LinkedVar<string>& data) {
                data.addCond(nemesis::LinkedVar(vline.ToString()), condtype.back(), vline.GetLineNumber());
            };

            switch (type)
            {
                case 0:
                {
                    addCondFunc(crc32list[index].raw->filepath);
                    ++type;
                    break;
                }
                case 1:
                {
                    addCondFunc(crc32list[index].raw->filename);
                    ++type;
                    break;
                }
                case 2:
                {
                    addCondFunc(crc32list[index].raw->extension);
                    ++index;
                    type = 0;
                    break;
                }
            }
        }
        else if (edits)
        {
            edits->emplace_back(line);
        }
        */
    }

    if (type != 0) ErrorMessage(5024, format, path, num);

    //if (!condtype.empty()) ErrorMessage(5024, format, path, edits->front().GetLineNumber());
}

size_t AnimSetData::Parser::GetNum() const noexcept
{
    return num;
}

size_t AnimSetData::Parser::GetEndAttackSetData()
{
    size_t sp = storeline.size() - 1;

    while (sp > 0 && storeline[sp].empty())
    {
        --sp;
    }

    short type = 2;
    Vec<nemesis::CondDetails> tempcondtype;
    deque<nemesis::Line>* records = nullptr;
    auto& curcondfunc = condfunc == nemesis::CondDetails::modCheck ? nemesis::CondDetails::modCheckRev
                                                                   : nemesis::CondDetails::templateCheckRev;

    for (size_t i = sp; i >= 0; --i)
    {
        const auto& line = storeline[i];

        if (curcondfunc(path, format, line, tempcondtype))
        {
            records = !tempcondtype.empty() ? &tempcondtype.back().contents : nullptr;
        }
        else if (nemesis::CondDetails::originalScopeRev(path, format, line, tempcondtype))
        {
            if (tempcondtype.size() != 1) continue;

            for (auto& eachline : *records)
            {
                switch (type)
                {
                    case 0:
                    {
                        type = 2;
                        break;
                    }
                    case 1:
                    {
                        --type;
                        break;
                    }
                    case 2:
                    {
                        if (eachline != "7891816") ErrorMessage(5025, format, path, eachline.GetLineNumber());

                        --type;
                        break;
                    }
                }
            }
        }
        else if (nemesis::CondDetails::closeScopeRev(line, tempcondtype))
        {
            records = &tempcondtype.back().contents;
        }
        else if (line.find("<!--") != NOT_FOUND)
        {
        }
        else if (tempcondtype.size() == 1)
        {
            records->push_front(line);
        }
        else
        {
            switch (type)
            {
                case 0:
                {
                    type = 2;
                    break;
                }
                case 1:
                {
                    --type;
                    break;
                }
                case 2:
                {
                    if (line != "7891816") return i;

                    --type;
                    break;
                }
            }
        }
    }

    ErrorMessage(5016, format, path);
    return -1;
}

bool AnimSetData::Parser::ImportNewWeapType(Deq<nemesis::Line>* edits)
{
    while (!edits->empty())
    {
        if (3 > edits->size()) ErrorMessage(5021, format, path, edits->back().GetLineNumber());

        WeapType wt;

        if (!wt.AddName(edits->front())) return false;

        edits->pop_front();

        if (!wt.AddFrom(edits->front())) return false;

        edits->pop_front();

        if (!wt.AddTo(edits->front())) return false;

        edits->pop_front();
        host.AddType(wt, wt.name.linecount, *tobedeleted);
    }

    return true;
}

bool AnimSetData::Parser::ImportNewAnimPack(Deq<nemesis::Line>* edits, Vec<size_t>& npoints)
{
    while (!edits->empty())
    {
        auto* edit = &edits->front();

        if (3 > edits->size()) ErrorMessage(5021, format, path, edits->back().GetLineNumber());

        AnimSetPack ap;

        if (!ap.AddName(*edit)) return false;

        edits->pop_front();
        edit = &edits->front();

        if (!ap.AddUnknown(*edit)) return false;

        edits->pop_front();
        edit = &edits->front();

        if (!isOnlyNumber(*edit)) return false;

        while (npoints.back() != edit->GetLineNumber())
        {
            edits->pop_front();
            edit = &edits->front();
            ap.AddAttackSetData(*edit);
        }

        edits->pop_front();
        npoints.pop_back();
        host.AddAnimPack(ap, ap.name.linecount, *tobedeleted);
    }

    return true;
}

bool AnimSetData::Parser::ImportNewCRC32Pack(Deq<nemesis::Line>* edits)
{
    while (!edits->empty())
    {
        if (3 > edits->size()) ErrorMessage(5021, format, path, edits->back().GetLineNumber());

        AnimSetCRC32Pack pack;

        if (!pack.AddFilePath(edits->front())) return false;

        edits->pop_front();

        if (!pack.AddFileName(edits->front())) return false;

        edits->pop_front();

        if (!pack.AddExtension(edits->front())) return false;

        edits->pop_front();
        host.AddCrc32Pack(pack, pack.filepath.linecount, *tobedeleted);
    }

    return true;
}

void AnimSetData::Parser::PointingEquip() noexcept
{
    ++index;
}

void AnimSetData::Parser::PointingOriEquip()
{
    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(&tobedeleted->GetRefContents());
    nemesis::LinkedVar linkedline = nemesis::ToLinkedString(vline);
    host.equiplist[index++].addCond(linkedline, tobedeleted->GetType(), vline.GetLineNumber());
}

bool AnimSetData::Parser::PointingType(const nemesis::Line& line, size_t i)
{
    switch (type)
    {
        case 0:
        {
            if (type == 0 && isOnlyNumber(line))
            {
                num = i + 1;
                return false;
            }
        }
        case 1:
        {
            ++type;
            break;
        }
        case 2:
        {
            type = 0;
            ++index;
            break;
        }
    }

    return true;
}

void AnimSetData::Parser::PointingOriType()
{
    auto& typelist      = host.typelist;
    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(&tobedeleted->GetRefContents());
    nemesis::LinkedVar linkedline(vline.ToString());
    auto AddLinkedCond = [&](nemesis::LinkedVar<string>& curline) {
        curline.addCond(linkedline, tobedeleted->GetType(), vline.GetLineNumber());
    };

    switch (type)
    {
        case 0:
        {
            AddLinkedCond(typelist[index].raw->name);
            ++type;
            break;
        }
        case 1:
        {
            AddLinkedCond(typelist[index].raw->from);
            ++type;
            break;
        }
        case 2:
        {
            AddLinkedCond(typelist[index].raw->to);
            type = 0;
            ++index;
            break;
        }
    }
}

void AnimSetData::Parser::PointingAttackSetData(const nemesis::Line& line)
{
    switch (type)
    {
        case 0:
        case 1:
        {
            ++type;
            break;
        }
        case 2:
        {
            if (points.empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

            if (points.back() != line.GetLineNumber())
            {
                ++type;
                break;
            }

            points.pop_back();
            type   = 0;
            iindex = 0;
            ++index;
            break;
        }
        case 3:
        {
            if (points.empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

            ++iindex;

            if (points.back() != line.GetLineNumber()) break;

            points.pop_back();
            type   = 0;
            iindex = 0;
            ++index;
            break;
        }
    }
}

void AnimSetData::Parser::PointingCrc32Pack() noexcept
{
    switch (type)
    {
        case 0:
        case 1:
        {
            ++type;
            break;
        }
        case 2:
        {
            type = 0;
            break;
        }
    }
}

void AnimSetData::Parser::PointingOriAttackSetData(const nemesis::Line& line)
{
    auto& animlist      = host.animlist;
    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(&tobedeleted->GetRefContents());
    nemesis::LinkedVar linkedline(vline.ToString());
    auto AddLinkedCond = [&](nemesis::LinkedVar<string>& curline) {
        curline.addCond(linkedline, tobedeleted->GetType(), vline.GetLineNumber());
    };

    switch (type)
    {
        case 0:
        {
            AddLinkedCond(animlist[index].raw->name);
            ++type;
            break;
        }
        case 1:
        {
            AddLinkedCond(animlist[index].raw->unknown);
            ++type;
            break;
        }
        case 2:
        {
            if (points.empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

            if (points.back() != line.GetLineNumber())
            {
                ++type;
                break;
            }

            points.pop_back();
            type   = 0;
            iindex = 0;
            ++index;
            break;
        }
        case 3:
        {
            if (points.empty()) ErrorMessage(5021, format, path, line.GetLineNumber());

            AddLinkedCond(animlist[index].raw->attackSetData[iindex]);
            ++iindex;

            if (points.back() != line.GetLineNumber()) break;

            points.pop_back();
            type   = 0;
            iindex = 0;
            ++index;
            break;
        }
    }
}

void AnimSetData::Parser::PointingOriCrc32Pack()
{
    auto& crc32list     = host.crc32list;
    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(&tobedeleted->GetRefContents());

    if (!isOnlyNumber(vline)) ErrorMessage(5021, format, path, vline.GetLineNumber());

    nemesis::LinkedVar linkedline(vline.ToString());
    auto AddCondFunc = [&](nemesis::LinkedVar<string>& data) {
        data.addCond(linkedline, tobedeleted->GetType(), vline.GetLineNumber());
    };

    switch (type)
    {
        case 0:
        {
            AddCondFunc(crc32list[index].raw->filepath);
            ++type;
            break;
        }
        case 1:
        {
            AddCondFunc(crc32list[index].raw->filename);
            ++type;
            break;
        }
        case 2:
        {
            AddCondFunc(crc32list[index].raw->extension);
            ++index;
            type = 0;
            break;
        }
    }
}

void AnimSetData::Parser::CloseEquip()
{
    switch (tobedeleted->GetType())
    {
        case nemesis::CondType::MOD_CODE:
        case nemesis::CondType::ORIGINAL:
        {
            auto& contents = tobedeleted->GetContents();
            auto cond      = nemesis::CondVar<DataStr>(*tobedeleted);
            host.equiplist.emplace_back(nemesis::LinkedVar<DataStr>(cond, tobedeleted->GetLineNumber()));

            for (auto& edit : contents)
            {
                host.AddData<std::string>(host.equiplist, edit, edit.GetLineNumber(), *tobedeleted);
            }
            break;
        }
        default:
        {
            equipstream.pop_back();
            break;
        }
    }
}

void AnimSetData::Parser::CloseType(const nemesis::Line& line)
{
    if (type != 0 && !tobedeleted->GetContents().empty())
    {
        ErrorMessage(5021, format, path, line.GetLineNumber());
    }

    auto edits = &tobedeleted->GetRefContents();

    if (!ImportNewWeapType(edits))
    {
        ErrorMessage(5021, format, path, edits->front().GetLineNumber());
    }
}

void AnimSetData::Parser::CloseAttackSetData(const nemesis::Line& line)
{
    auto& edits  = tobedeleted->GetRefContents();
    auto npoints = GetAttackSetDataSections(0, edits.size(), edits);

    if (type == 3)
    {
        auto& curAtkSD = host.animlist[index].raw->attackSetData;

        while (!edits.empty())
        {
            auto& edit = edits.front();

            curAtkSD.emplace_back(nemesis::CondVar(edit.ToString(), tobedeleted->GetType()));
            curAtkSD.back().linecount                           = edit.GetLineNumber();
            curAtkSD.back().backCond().linenum                  = edit.GetLineNumber();
            curAtkSD.back().backCond().rawlist.back().linecount = edit.GetLineNumber();

            if (npoints.empty()) ErrorMessage(5021, format, path, edit.GetLineNumber());

            if (npoints.back() == edit.GetLineNumber())
            {
                edits.pop_front();
                type = 0;
                break;
            }

            edits.pop_front();
        }
    }
    else if (type != 0 && !edits.empty())
    {
        ErrorMessage(5021, format, path, line.GetLineNumber());
    }

    if (!ImportNewAnimPack(&edits, npoints))
    {
        ErrorMessage(5021, format, path, edits.front().GetLineNumber());
    }
}

void AnimSetData::Parser::CloseCrc32Pack()
{
    auto edits = &tobedeleted->GetRefContents();

    if (type != 0 && !edits->empty()) ErrorMessage(5021, format, path, edits->back().GetLineNumber());

    if (!ImportNewCRC32Pack(edits)) ErrorMessage(5021, format, path, edits->front().GetLineNumber());
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetData::WeapType>& linkedtype, VecStr& storeline)
{
    Vec<pair<const string*, const nemesis::CondVar<AnimSetData::WeapType>*>> modcodelist;

    for (auto& cond : linkedtype.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelist.emplace_back(
                    make_pair<const string*, const nemesis::CondVar<AnimSetData::WeapType>*>(&cond.conditions,
                                                                                             &cond));
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

    if (!modcodelist.empty())
    {
        if (linkedtype.raw)
        {
            Vec<pair<string, VecStr>> list;

            for (auto& modcode : modcodelist)
            {
                list.emplace_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                getLinkedLines(modcode.second->rawlist[0], list.back().second);
            }

            for (auto& each : list)
            {
                storeline.emplace_back(ns::ModCode(each.first));
                storeline.insert(storeline.end(), each.second.begin(), each.second.end());
            }

            if (!list.empty())
            {
                storeline.emplace_back(ns::ModOriginal());
                linkedtype.raw->Getlines(storeline);
                storeline.emplace_back(ns::Close());
            }
            else
            {
                linkedtype.raw->Getlines(storeline);
            }
        }
        else
        {
            for (auto& modcode : modcodelist)
            {
                storeline.emplace_back(ns::ModCode(*modcode.first));
                getLinkedLines(modcode.second->rawlist[0], storeline);
                storeline.emplace_back(ns::Close());
            }
        }
    }
    else if (linkedtype.raw)
    {
        linkedtype.raw->Getlines(storeline);
    }
}

Vec<size_t> AnimSetData::Parser::GetAttackSetDataSections(size_t start, size_t end) const
{
    return GetAttackSetDataSections(start, end, storeline);
}

Vec<size_t>
AnimSetData::Parser::GetAttackSetDataSections(size_t start, size_t end, const VecNstr& _storeline) const
{
    short type  = 0;
    bool edited = false;
    Vec<size_t> points;
    Vec<nemesis::CondDetails> tempcondtype;
    deque<nemesis::Line>* records = nullptr;
    auto& curcondfunc = condfunc == nemesis::CondDetails::modCheck ? nemesis::CondDetails::modCheckRev
                                                                   : nemesis::CondDetails::templateCheckRev;

    auto cacheAndValidate = [&](const nemesis::Line& curline) {
        switch (type)
        {
            case 0:
            {
                points.emplace_back(curline.GetLineNumber());
                ++type;
            }
            case 1:
            {
                if (isOnlyNumber(curline))
                {
                    ++type;
                    break;
                }

                if (!hasAlpha(curline)) ErrorMessage(5021, format, path, curline.GetLineNumber());

                break;
            }
            case 2:
            {
                if (!isOnlyNumber(curline)) ErrorMessage(5021, format, path, curline.GetLineNumber());

                ++type;
                break;
            }
            case 3:
            {
                if (!hasAlpha(curline)) ErrorMessage(5021, format, path, curline.GetLineNumber());

                type = 0;
                break;
            }
        }
    };

    // required to ensure the for loop to not break
    int iStart = start;

    for (int i = end - 1; i >= iStart; i--)
    {
        const auto& line = _storeline[i];

        if (curcondfunc(path, format, line, tempcondtype))
        {
            records = !tempcondtype.empty() ? &tempcondtype.back().contents : nullptr;
        }
        else if (nemesis::CondDetails::originalScopeRev(path, format, line, tempcondtype))
        {
            if (tempcondtype.size() != 1) continue;

            for (auto& eachline : *records)
            {
                cacheAndValidate(eachline);
            }
        }
        else if (nemesis::CondDetails::closeScopeRev(line, tempcondtype))
        {
            records = &tempcondtype.back().contents;
        }
        else if (line.find(ns::OpenComment()) == NOT_FOUND)
        {
            if (tempcondtype.size() == 1)
            {
                records->push_front(line);
            }
            else
            {
                cacheAndValidate(line);
            }
        }
    }

    return points;
}

Vec<size_t> AnimSetData::Parser::GetAttackSetDataSections(size_t start,
                                                          size_t end,
                                                          const Deq<nemesis::Line>& _storeline) const
{
    VecNstr temp(_storeline.begin(), _storeline.end());
    return GetAttackSetDataSections(start, end, temp);
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetData>& linkeddata, VecStr& storeline, VecStr& namelines)
{
    Vec<const nemesis::CondVar<AnimSetData>*> tempstore;
    map<string, Vec<const nemesis::CondVar<AnimSetData>*>, alphanum_less> mappedLink;
    function<void(const nemesis::CondVar<AnimSetData>&)> modcodelambda;

    if (linkeddata.raw)
    {
        modcodelambda = [&](const nemesis::CondVar<AnimSetData>& project)
        {
            tempstore.emplace_back(&project);
        };
    }
    else
    {
        modcodelambda = [&](const nemesis::CondVar<AnimSetData>& project)
        {
            mappedLink[project.conditions].emplace_back(&project);
        };
    }

    auto AddToBoth = [&](string line)
    {
        storeline.emplace_back(line);
        namelines.emplace_back(line);
    };

    for (auto& cond : linkeddata.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelambda(cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                AddToBoth(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline, namelines);
                }

                AddToBoth(ns::Close());
                break;
            }
        }
    }

    if (!tempstore.empty())
    {
        for (auto& each : tempstore)
        {
            AddToBoth(ns::ModCode(each->conditions));
            getLinkedLines(each->rawlist[0], storeline, namelines);
        }

        AddToBoth(ns::ModOriginal());
        linkeddata.raw->Getlines(storeline);
        linkeddata.raw->Getname(namelines);
        AddToBoth(ns::Close());
    }
    else if (!mappedLink.empty())
    {
        for (auto& each : mappedLink)
        {
            AddToBoth(ns::ModCode(each.first));

            for (auto& asd : each.second)
            {
                getLinkedLines(asd->rawlist[0], storeline, namelines);
            }

            AddToBoth(ns::Close());
        }
    }
    else if (linkeddata.raw)
    {
        linkeddata.raw->Getlines(storeline);
        linkeddata.raw->Getname(namelines);
    }
}
