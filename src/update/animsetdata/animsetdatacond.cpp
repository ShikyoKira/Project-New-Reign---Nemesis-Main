#include "utilities/alphanum.hpp"

#include "update/animsetdata/animsetdatacond.h"

using namespace std;

void getLinkedLines(const nemesis::LinkedVar<AnimSetData::WeapType>& linkedtype, VecStr& storeline);

void AnimSetData::WeapType::getlines(VecStr& storeline)
{
    getLinkedLines(name, storeline);
    getLinkedLines(from, storeline);
    getLinkedLines(to, storeline);
}

AnimSetData::AnimSetData(string _name, size_t num)
{
    name = _name;
    name.linecount = num;
}

void AnimSetData::importData(filesystem::path path,
                             size_t& num,
                             const VecNstr& storeline)
{
    // Adding equip
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (isOnlyNumber(storeline[i]))
        {
            num = i + 1;
            break;
        }

        addEquip(storeline[i], i + 1);
    }

    // Adding type
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (isOnlyNumber(storeline[i]))
        {
            num = i + 1;
            break;
        }

        AnimSetData::WeapType wt;
        wt.name           = storeline[i++];
        wt.name.linecount = i;
        wt.from           = storeline[i++];
        wt.from.linecount = i;
        wt.to             = storeline[i];
        wt.to.linecount   = i + 1;
        addType(wt, i + 1);
    }

    // Adding attack set data
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (isOnlyNumber(storeline[i]))
        {
            num = i + 1;
            break;
        }

        int curline  = i + 1;
        auto name    = make_pair(storeline[i++], i);
        auto unknown = make_pair(storeline[i++], i);
        size_t size  = stoi(storeline[i]);
        VecStr attackSetData;

        for (size_t k = i + 1; k < size + i + 1; ++k)
        {
            attackSetData.push_back(storeline[k]);
        }

        AnimSetPack pack(name.first, unknown.first, attackSetData);
        pack.name.linecount    = name.second;
        pack.unknown.linecount = unknown.second;
        addAnimPack(pack, curline);
        i += size;
    }

    // Adding CRC32 file path
    for (size_t i = num; i < storeline.size(); ++i)
    {
        if (i + 1 >= storeline.size() || storeline[i] == "V3" || !isOnlyNumber(storeline[i + 1]))
        {
            num = i;
            break;
        }

        AnimSetCRC32Pack pack;
        auto& filepathref = storeline[i++];
        int curline       = i;

        if (!isOnlyNumber(filepathref)) ErrorMessage(5020, path, i);

        pack.filepath           = filepathref;
        pack.filepath.linecount = i;
        auto& filenameref       = storeline[i++];

        if (!isOnlyNumber(filenameref)) ErrorMessage(5020, path, i);

        pack.filename           = filenameref;
        pack.filename.linecount = i;
        auto& extref            = storeline[i];

        if (!isOnlyNumber(extref)) ErrorMessage(5020, path, i + 1);

        pack.extension           = extref;
        pack.extension.linecount = i;
        addCrc32Pack(pack, curline);
    }
}

void AnimSetData::importData(filesystem::path path,
                             size_t& num,
                             const VecNstr& storeline,
                             const ModCode& format)
{
    importEquip(path, num, storeline, nemesis::CondDetails::modCheck, format);
    importType(path, num, storeline, nemesis::CondDetails::modCheck, format);
    importAttackSetData(path, num, storeline, nemesis::CondDetails::modCheck, format);
    importCrc32Pack(path, num, storeline, nemesis::CondDetails::modCheck, format);
}

void AnimSetData::importDataTemplate(filesystem::path path,
                                     size_t& num,
                                     const VecNstr& storeline,
                                     const ModCode& format)
{
    importEquip(path, num, storeline, nemesis::CondDetails::templateCheck, format);
    importType(path, num, storeline, nemesis::CondDetails::templateCheck, format);
    importAttackSetData(path, num, storeline, nemesis::CondDetails::templateCheck, format);
    importCrc32Pack(path, num, storeline, nemesis::CondDetails::templateCheck, format);
}

shared_ptr<string> AnimSetData::addEquip(const DataStr& equip, size_t num)
{
    return addData(equiplist, equip, num);
}

shared_ptr<string> AnimSetData::addEquip(const DataStr& equip, size_t num, const nemesis::CondDetails& condition)
{
    return addData(equiplist, equip, num, condition);
}

shared_ptr<AnimSetData::WeapType> AnimSetData::addType(const WeapType& type, size_t num)
{
    return addData(typelist, type, num);
}

shared_ptr<AnimSetData::WeapType>
AnimSetData::addType(const WeapType& type, size_t num, const nemesis::CondDetails& condition)
{
    return addData(typelist, type, num, condition);
}

std::shared_ptr<AnimSetPack> AnimSetData::addAnimPack(const AnimSetPack& animpack, size_t num)
{
    return addData(animlist, animpack, num);
}

shared_ptr<AnimSetPack>
AnimSetData::addAnimPack(const AnimSetPack& animpack, size_t num, const nemesis::CondDetails& condition)
{
    return addData(animlist, animpack, num, condition);
}

std::shared_ptr<AnimSetCRC32Pack> AnimSetData::addCrc32Pack(const AnimSetCRC32Pack& crc32pack, size_t num)
{
    return addData(crc32list, crc32pack, num);
}

shared_ptr<AnimSetCRC32Pack> AnimSetData::addCrc32Pack(const AnimSetCRC32Pack& crc32pack,
                                                       size_t num,
                                                       const nemesis::CondDetails& condition)
{
    return addData(crc32list, crc32pack, num, condition);
}

void AnimSetData::getlines(VecStr& storeline)
{
    storeline.push_back("V3");
    exportEquip(storeline);
    exportType(storeline);
    exportAnimPack(storeline);
    exportCrc32Pack(storeline);
}

void AnimSetData::getname(VecStr& storeline)
{
    getLinkedLines(name, storeline);
}

string AnimSetData::getfirstname()
{
    if (name.raw) return *name.raw;

    for (auto& cond : name.nestedcond)
    {
        for (auto& raw : cond.rawlist)
        {
            return getfirstname(raw);
        }
    }

    return "";
}

string AnimSetData::getfirstname(nemesis::LinkedVar<string> _name)
{
    if (_name.raw) return *_name.raw;

    for (auto& cond : _name.nestedcond)
    {
        for (auto& raw : cond.rawlist)
        {
            return getfirstname(raw);
        }
    }

    return "";
}

void AnimSetData::importEquip(filesystem::path path,
                              size_t& num,
                              const VecNstr& storeline,
                              nemesis::CondCheckFunc condfunc,
                              const string& format)
{
    bool edited       = false;
    bool originalopen = false;
    uint index        = 0;
    vector<nemesis::CondDetails> condtype;
    deque<nemesis::Line>* edits = nullptr;

    // Adding equip
    for (size_t i = num; i < storeline.size(); ++i)
    {
        const auto& line = storeline[i];

        if (isOnlyNumber(line))
        {
            num = i + 1;
            break;
        }

        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.size() == 0 && line.find("<!--") == NOT_FOUND)
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
                addEquip(edit, edit.linenum, condtype.back());
            }

            originalopen = false;
            condtype.pop_back();
            edits = condtype.size() > 0 ? &condtype.back().contents : nullptr;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            equiplist[index++].addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                       condtype.back(),
                                       vline.linenum);
        }
        else if (edits)
        {
            edits->push_back(line);
        }
    }

    if (condtype.size() > 0) ErrorMessage(5024, format, path, condtype.back().contents.front().linenum - 1);
}

void AnimSetData::importType(filesystem::path path,
                             size_t& num,
                             const VecNstr& storeline,
                             nemesis::CondCheckFunc condfunc,
                             const string& format)
{
    short type        = 0;
    bool edited       = false;
    bool originalopen = false;
    uint index        = 0;
    vector<nemesis::CondDetails> condtype;
    deque<nemesis::Line>* edits = nullptr;

    // Adding type
    for (size_t i = num; i < storeline.size(); ++i)
    {
        const auto& line = storeline[i];

        if (i + 1 == storeline.size())
        {
            num = i + 1;
            break;
        }

        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.size() == 0 && line.find("<!--") == NOT_FOUND)
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
            if (type != 0 && edits->size() > 0) ErrorMessage(5021, format, path, line.linenum);

            for (uint k = 0; k < edits->size(); ++k)
            {
                WeapType wt;
                auto& edit = (*edits)[k++];

                if (!hasAlpha(edit)) ErrorMessage(5021, format, path, edit.linenum);

                wt.name           = edit;
                wt.name.linecount = edit.linenum;
                edit              = (*edits)[k++];

                if (!isOnlyNumber(edit)) ErrorMessage(5021, format, path, edit.linenum);

                wt.from           = edit;
                wt.from.linecount = edit.linenum;
                edit              = (*edits)[k++];

                if (!isOnlyNumber(edit)) ErrorMessage(5021, format, path, edit.linenum);

                wt.to           = edit;
                wt.to.linecount = edit.linenum;
                addType(wt, wt.name.linecount, condtype.back());
            }

            edited       = false;
            originalopen = false;
            condtype.pop_back();
            edits = condtype.size() > 0 ? &condtype.back().contents : nullptr;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);

            switch (type)
            {
                case 0:
                {
                    typelist[index].raw->name.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                      condtype.back(),
                                                      vline.linenum);
                    ++type;
                    break;
                }
                case 1:
                {
                    typelist[index].raw->from.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                      condtype.back(),
                                                      vline.linenum);
                    ++type;
                    break;
                }
                case 2:
                {
                    typelist[index].raw->to.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                    condtype.back(),
                                                    vline.linenum);
                    type = 0;
                    ++index;
                    break;
                }
            }
        }
        else if (edits)
        {
            edits->push_back(line);
        }
    }

    if (type != 0) ErrorMessage(5024, format, path, num);

    if (condtype.size() > 0) ErrorMessage(5024, format, path, condtype.back().contents.front().linenum - 1);
}

void AnimSetData::importAttackSetData(filesystem::path path,
                                      size_t& num,
                                      const VecNstr& storeline,
                                      nemesis::CondCheckFunc condfunc,
                                      const string& format)
{
    short type        = 0;
    bool edited       = false;
    bool originalopen = false;
    uint index          = 0;
    uint iindex         = 0;
    uint size           = getEndAttackSetData(path, storeline, condfunc, format);
    vector<uint> points = getAttackSetDataSections(size, num, path, storeline, condfunc, format);
    vector<nemesis::CondDetails> condtype;
    deque<nemesis::Line>* edits = nullptr;

    for (size_t i = num; i < size; ++i)
    {
        const auto& line = storeline[i];

        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.size() == 0 && line.find("<!--") == NOT_FOUND)
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
                    if (points.back() != line.linenum)
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
                    ++iindex;

                    if (points.back() != line.linenum) break;

                    points.pop_back();
                    type   = 0;
                    iindex = 0;
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
            uint k = 0;
            VecNstr temp;
            temp.reserve(edits->size());

            for (auto& each : *edits)
            {
                temp.push_back(each);
            }

            vector<uint> npoints = getAttackSetDataSections(edits->size(), 0, path, temp, condfunc, format);

            if (type == 3)
            {
                auto& curAtkSD = animlist[index].raw->attackSetData;

                while (0 != edits->size())
                {
                    auto& edit = edits->front();

                    curAtkSD.push_back(nemesis::CondVar(static_cast<string>(edit), condtype.back()));
                    curAtkSD.back().linecount = edit.linenum;
                    curAtkSD.back().backCond().linenum = edit.linenum;
                    curAtkSD.back().backCond().rawlist.back().linecount = edit.linenum;

                    if (npoints.back() == edit.linenum)
                    {
                        npoints.back();
                        edits->pop_front();
                        type = 0;
                        break;
                    }

                    edits->pop_front();
                }
            }
            else if (type != 0 && edits->size() > 0)
            {
                ErrorMessage(5021, format, path, line.linenum);
            }

            while (0 != edits->size())
            {
                AnimSetPack ap;
                auto& edit = edits->front();

                if (!hasAlpha(edit) || 1 == edits->size())
                {
                    ErrorMessage(5021, format, path, edit);
                }

                ap.name           = edit;
                ap.name.linecount = edit.linenum;
                edits->pop_front();
                edit = edits->front();

                if (!isOnlyNumber(edit) || 1 == edits->size())
                {
                    ErrorMessage(5021, format, path, edit);
                }

                ap.unknown           = edit;
                ap.unknown.linecount = edit.linenum;
                edits->pop_front();
                edit = edits->front();

                if (!isOnlyNumber(edit)) ErrorMessage(5021, format, path, edit.linenum);

                if (npoints.back() != edit.linenum)
                {
                    do
                    {
                        edits->pop_front();
                        edit = edits->front();
                        ap.attackSetData.push_back(edit);
                        ap.attackSetData.back().linecount = edit.linenum;
                    } while (npoints.back() != edits->front().linenum);
                }

                edits->pop_front();
                npoints.pop_back();
                addAnimPack(ap, ap.name.linecount, condtype.back());
            }

            condtype.pop_back();
            edits        = condtype.size() > 0 ? &condtype.back().contents : nullptr;
            edited       = false;
            originalopen = false;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);

            switch (type)
            {
                case 0:
                {
                    animlist[index].raw->name.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                      condtype.back(),
                                                      vline.linenum);
                    ++type;
                    break;
                }
                case 1:
                {
                    animlist[index].raw->unknown.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                         condtype.back(),
                                                         vline.linenum);
                    ++type;
                    break;
                }
                case 2:
                {
                    if (points.back() != line.linenum)
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
                    animlist[index].raw->attackSetData[iindex].addCond(
                        nemesis::LinkedVar(static_cast<string>(vline)), condtype.back(), vline.linenum);
                    ++iindex;

                    if (points.back() != line.linenum) break;

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
            edits->push_back(line);
        }
    }

    num = size + 1;

    if (type != 0) ErrorMessage(5024, format, path, num);

    if (condtype.size() > 0) ErrorMessage(5024, format, path, edits->front().linenum);
}

void AnimSetData::importCrc32Pack(filesystem::path path,
                                  size_t& num,
                                  const VecNstr& storeline,
                                  nemesis::CondCheckFunc condfunc,
                                  const string& format)
{
    short type          = 0;
    bool originalopen   = false;
    uint index          = 0;
    uint iindex         = 0;
    vector<nemesis::CondDetails> condtype;
    deque<nemesis::Line>* edits = nullptr;

    // Adding CRC32 file path
    for (size_t i = num; i < storeline.size(); ++i)
    {
        const auto& line = storeline[i];

        if (condfunc(path, format, line, condtype))
        {
            edits = &condtype.back().contents;
            continue;
        }

        if (condtype.size() == 0 && line.find("<!--") == NOT_FOUND)
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
            if (type != 0 && edits->size() > 0) ErrorMessage(5021, format, path, edits->back().linenum);

            AnimSetCRC32Pack pack;

            for (auto& edit : *edits)
            {
                if ((edit.front() != '$' || edit.back() != '$') && !isOnlyNumber(edit))
                {
                    ErrorMessage(5021, format, path, edit.linenum);
                }

                switch (type)
                {
                    case 0:
                    {
                        pack                    = AnimSetCRC32Pack();
                        pack.filepath           = edit;
                        pack.filepath.linecount = edit.linenum;
                        ++type;
                        break;
                    }
                    case 1:
                    {
                        pack.filename           = edit;
                        pack.filename.linecount = edit.linenum;
                        ++type;
                        break;
                    }
                    case 2:
                    {
                        pack.extension           = edit;
                        pack.extension.linecount = edit.linenum;
                        addCrc32Pack(pack, pack.filepath.linecount, condtype.back());
                        type = 0;
                        break;
                    }
                }
            }

            if (type != 0) ErrorMessage(5021, format, path, i);

            originalopen = false;
            condtype.pop_back();
            edits = condtype.size() > 0 ? &condtype.back().contents : nullptr;
        }
        else if (originalopen)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);

            if (!isOnlyNumber(vline)) ErrorMessage(5021, format, path, vline.linenum);

            switch (type)
            {
                case 0:
                {
                    crc32list[index].raw->filepath.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                           condtype.back(),
                                                           vline.linenum);
                    ++type;
                    break;
                }
                case 1:
                {
                    crc32list[index].raw->filename.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                           condtype.back(),
                                                           vline.linenum);
                    ++type;
                    break;
                }
                case 2:
                {
                    crc32list[index].raw->extension.addCond(nemesis::LinkedVar(static_cast<string>(vline)),
                                                            condtype.back(),
                                                            vline.linenum);
                    ++index;
                    type = 0;
                    break;
                }
            }
        }
        else if (edits)
        {
            edits->push_back(line);
        }
    }

    if (type != 0) ErrorMessage(5024, format, path, num);

    if (condtype.size() > 0) ErrorMessage(5024, format, path, edits->front().linenum);
}

uint AnimSetData::getEndAttackSetData(filesystem::path path,
                                      const VecNstr& storeline,
                                      nemesis::CondCheckFunc condfunc,
                                      const string& format)
{
    uint sp = storeline.size() - 1;

    while (sp > 0 && storeline[sp].length() == 0)
    {
        --sp;
    }

    short type = 2;
    vector<nemesis::CondDetails> condtype;
    deque<nemesis::Line>* records;
    auto& curcondfunc = condfunc == nemesis::CondDetails::modCheck ? nemesis::CondDetails::modCheckRev
                                                                   : nemesis::CondDetails::templateCheckRev;

    for (int i = sp; i >= 0; --i)
    {
        const auto& line = storeline[i];

        if (curcondfunc(path, format, line, condtype))
        {
            records = condtype.size() > 0 ? &condtype.back().contents : nullptr;
            continue;
        }
        else if (nemesis::CondDetails::originalScopeRev(path, format, line, condtype))
        {
            if (condtype.size() == 1)
            {
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
                            if (eachline != "7891816") ErrorMessage(5025, format, path, eachline.linenum);

                            --type;
                            break;
                        }
                    }
                }
            }

            continue;
        }
        else if (nemesis::CondDetails::closeScopeRev(line, condtype))
        {
            records = &condtype.back().contents;
            continue;
        }
        else if (line.find("<!--") != NOT_FOUND)
        {
            continue;
        }
        else if (condtype.size() == 1)
        {
            records->push_front(line);
            continue;
        }

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

    ErrorMessage(5016, format, path);
    return -1;
}

vector<uint> AnimSetData::getAttackSetDataSections(uint size,
                                                   int num,
                                                   filesystem::path path,
                                                   const VecNstr& storeline,
                                                   nemesis::CondCheckFunc condfunc,
                                                   const string& format)
{
    short type  = 0;
    bool edited = false;
    vector<uint> points;
    vector<nemesis::CondDetails> condtype;
    deque<nemesis::Line>* records;
    auto& curcondfunc = condfunc == nemesis::CondDetails::modCheck ? nemesis::CondDetails::modCheckRev
                                                                   : nemesis::CondDetails::templateCheckRev;

    for (int i = size - 1; i >= num; --i)
    {
        const auto& line = storeline[i];

        if (curcondfunc(path, format, line, condtype))
        {
            records = condtype.size() > 0 ? &condtype.back().contents : nullptr;
            continue;
        }
        else if (nemesis::CondDetails::originalScopeRev(path, format, line, condtype))
        {
            if (condtype.size() == 1)
            {
                for (auto& eachline : *records)
                {
                    switch (type)
                    {
                        case 0:
                        {
                            points.push_back(eachline.linenum);
                            ++type;
                        }
                        case 1:
                        {
                            if (isOnlyNumber(eachline))
                            {
                                ++type;
                                break;
                            }

                            if (!hasAlpha(eachline)) ErrorMessage(5021, format, path, eachline.linenum);

                            break;
                        }
                        case 2:
                        {
                            if (!isOnlyNumber(eachline)) ErrorMessage(5021, format, path, eachline.linenum);

                            ++type;
                            break;
                        }
                        case 3:
                        {
                            if (!hasAlpha(eachline)) ErrorMessage(5021, format, path, eachline.linenum);

                            type = 0;
                            break;
                        }
                    }
                }
            }

            continue;
        }
        else if (nemesis::CondDetails::closeScopeRev(line, condtype))
        {
            records = &condtype.back().contents;
            continue;
        }
        else if (line.find("<!--") != NOT_FOUND)
        {
            continue;
        }
        else if (condtype.size() == 1)
        {
            records->push_front(line);
            continue;
        }

        switch (type)
        {
            case 0:
            {
                points.push_back(line.linenum);
                ++type;
            }
            case 1:
            {
                if (isOnlyNumber(line))
                {
                    ++type;
                    break;
                }

                if (!hasAlpha(line)) ErrorMessage(5021, format, path, line.linenum);

                break;
            }
            case 2:
            {
                if (!isOnlyNumber(line)) ErrorMessage(5021, format, path, line.linenum);

                ++type;
                break;
            }
            case 3:
            {
                if (!hasAlpha(line)) ErrorMessage(5021, format, path, line.linenum);

                type = 0;
                break;
            }
        }
    }

    return points;
}

void AnimSetData::exportEquip(VecStr& storeline)
{
    storeline.push_back(to_string(equiplist.size()));
    getLinkedLines(equiplist, storeline);
}

void AnimSetData::exportType(VecStr& storeline)
{
    storeline.push_back(to_string(typelist.size()));
    string last;

    for (auto& type : typelist)
    {
        VecStr temp;
        getLinkedLines(type, temp);
        combineLines(last, temp, storeline);
    }
}

void AnimSetData::exportAnimPack(VecStr& storeline)
{
    storeline.push_back(to_string(animlist.size()));
    string last;

    for (auto& anim : animlist)
    {
        VecStr temp;
        getLinkedLines(anim, temp);
        combineLines(last, temp, storeline);
    }
}

void AnimSetData::exportCrc32Pack(VecStr& storeline)
{
    storeline.push_back(to_string(crc32list.size()));
    string last;

    for (auto& crc32 : crc32list)
    {
        VecStr temp;
        getLinkedLines(crc32, temp);
        combineLines(last, temp, storeline);
    }
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetData::WeapType>& linkedtype, VecStr& storeline)
{
    vector<pair<const string*, const nemesis::CondVar<AnimSetData::WeapType>*>> modcodelist;

    for (auto& cond : linkedtype.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(
                    make_pair<const string*, const nemesis::CondVar<AnimSetData::WeapType>*>(&cond.conditions,
                                                                                             &cond));
                break;
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline);
                }

                storeline.push_back("<!-- CLOSE -->");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        if (linkedtype.raw)
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
                linkedtype.raw->getlines(storeline);
                storeline.push_back("<!-- CLOSE -->");
            }
            else
            {
                linkedtype.raw->getlines(storeline);
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
    else if (linkedtype.raw)
    {
        linkedtype.raw->getlines(storeline);
    }
}

void getLinkedLines(const nemesis::LinkedVar<AnimSetData>& linkeddata, VecStr& storeline, VecStr& namelines)
{
    vector<pair<const string*, const nemesis::CondVar<AnimSetData>*>> modcodelist;

    for (auto& cond : linkeddata.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(
                    make_pair<const string*, const nemesis::CondVar<AnimSetData>*>(&cond.conditions, &cond));
                break;
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");
                namelines.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline, namelines);
                }

                storeline.push_back("<!-- CLOSE -->");
                namelines.push_back("<!-- CLOSE -->");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        if (linkeddata.raw)
        {
            vector<pair<string, VecStr>> list;
            vector<pair<string, VecStr>> namelist;

            for (auto& modcode : modcodelist)
            {
                list.push_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                namelist.push_back(pair<string, VecStr>());
                namelist.back().first = *modcode.first;
                getLinkedLines(modcode.second->rawlist[0], list.back().second, namelist.back().second);
            }

            for (auto& each : list)
            {
                storeline.push_back("<!-- NEW *" + each.first + "* -->");
                storeline.insert(storeline.end(), each.second.begin(), each.second.end());
            }

            for (auto& each : namelist)
            {
                namelines.push_back("<!-- NEW *" + each.first + "* -->");
                namelines.insert(namelines.end(), each.second.begin(), each.second.end());
            }

            if (list.size() > 0)
            {
                storeline.push_back("<!-- ORIGINAL -->");
                linkeddata.raw->getlines(storeline);
                storeline.push_back("<!-- CLOSE -->");

                namelines.push_back("<!-- ORIGINAL -->");
                linkeddata.raw->getname(namelines);
                namelines.push_back("<!-- CLOSE -->");
            }
            else
            {
                linkeddata.raw->getlines(storeline);
                linkeddata.raw->getname(namelines);
            }
        }
        else
        {
            map<string, vector<const nemesis::CondVar<AnimSetData>*>, alphanum_less> mappedLink;

            for (auto& modcode : modcodelist)
            {
                mappedLink[*modcode.first].push_back(modcode.second);
            }

            for (auto& asdtxtlist : mappedLink)
            {
                storeline.push_back("<!-- NEW *" + asdtxtlist.first + "* -->");
                namelines.push_back("<!-- NEW *" + asdtxtlist.first + "* -->");

                for (auto& asd : asdtxtlist.second)
                {
                    getLinkedLines(asd->rawlist[0], storeline, namelines);
                }

                storeline.push_back("<!-- CLOSE -->");
                namelines.push_back("<!-- CLOSE -->");
            }
        }
    }
    else if (linkeddata.raw)
    {
        linkeddata.raw->getlines(storeline);
        linkeddata.raw->getname(namelines);
    }
}
