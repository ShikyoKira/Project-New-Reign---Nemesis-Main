#include "utilities/conditionsyntax.h"

#include "update/animdata/linkedanimdata.h"

namespace ns = nemesis::syntax;

void (nemesis::LinkedAnimData::*nemesis::LinkedAnimData::get_rawdata)(VecNstr&) const = nullptr;

void nemesis::LinkedAnimData::_GetRawHeaderData(VecNstr& rawdata) const
{
    Vec<LinkedCondition*> modlist;
    auto this_func = &nemesis::LinkedAnimData::_GetRawHeaderData;

    for (auto& condition : conditions)
    {
        switch (condition->GetType())
        {
            case nemesis::CondType::MOD_CODE:
            {
                modlist.emplace_back(condition.get());
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                rawdata.emplace_back(nemesis::syntax::ForEach(condition->GetExpression()));
                condition->GetRawDataList(rawdata, this_func);
                rawdata.emplace_back(nemesis::syntax::Close());
                break;
            }
        }
    }

    if (modlist.empty())
    {
        if (!raw) return;

        raw->base.GetRawData(rawdata);
        return;
    }

    if (!raw)
    {
        for (auto& cond : modlist)
        {
            rawdata.emplace_back(nemesis::syntax::ModCode(cond->GetExpression()));
            cond->GetRawDataList(rawdata, this_func);
            rawdata.emplace_back(nemesis::syntax::Close());
        }

        return;
    }

    Vec<VecNstr> mod_rawdata_list;
    mod_rawdata_list.reserve(modlist.size());

    for (auto& cond : modlist)
    {
        mod_rawdata_list.emplace_back();
        auto& mod_rawdata = mod_rawdata_list.back();
        cond->GetRawDataList(mod_rawdata, this_func);
        nemesis::Line mline = mod_rawdata.empty() ? nemesis::syntax::DeleteLine() : mod_rawdata.front();
        mline += nemesis::syntax::Spaces() + nemesis::syntax::Aster(cond->GetExpression());
        rawdata.emplace_back(mline);
    }

    VecNstr mlines;
    raw->base.GetRawData(mlines);
    std::for_each(mlines.begin(), mlines.end(), [&](nemesis::Line& line) {
        line.append(nemesis::syntax::Spaces() + nemesis::syntax::LowerOriginal());
    });
    rawdata.insert(rawdata.end(), mlines.begin(), mlines.end());

    for (size_t i = 0; i < mod_rawdata_list.size(); ++i)
    {
        auto& mod_rawdata = mod_rawdata_list[i];

        if (mod_rawdata.size() <= 1) continue;

        rawdata.emplace_back(nemesis::syntax::ModCode(modlist[i]->GetExpression()));

        for (size_t k = 1; k < mod_rawdata.size(); ++k)
        {
            auto& line = mod_rawdata[k];
            rawdata.emplace_back(line);
        }

        rawdata.emplace_back(nemesis::syntax::Close());
    }
}

void nemesis::LinkedAnimData::_GetRawBaseData(VecNstr& rawdata) const
{
    Vec<LinkedCondition*> modlist;
    auto this_func = &nemesis::LinkedAnimData::_GetRawBaseData;

    for (auto& condition : conditions)
    {
        switch (condition->GetType())
        {
            case nemesis::CondType::MOD_CODE:
            {
                modlist.emplace_back(condition.get());
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                rawdata.emplace_back(nemesis::syntax::ForEach(condition->GetExpression()));
                condition->GetRawDataList(rawdata, this_func);
                rawdata.emplace_back(nemesis::syntax::Close());
                break;
            }
        }
    }

    if (modlist.empty())
    {
        if (!raw) return;

        raw->base.GetRawData(rawdata);
        return;
    }

    if (!raw)
    {
        for (auto& cond : modlist)
        {
            rawdata.emplace_back(nemesis::syntax::ModCode(cond->GetExpression()));
            cond->GetRawDataList(rawdata, this_func);
            rawdata.emplace_back(nemesis::syntax::Close());
        }

        return;
    }

    Vec<VecNstr> mod_rawdata_list;
    mod_rawdata_list.reserve(modlist.size());

    for (auto& cond : modlist)
    {
        mod_rawdata_list.emplace_back();
        auto& mod_rawdata = mod_rawdata_list.back();
        cond->GetRawDataList(mod_rawdata, this_func);
        nemesis::Line mline = mod_rawdata.empty() ? nemesis::syntax::DeleteLine() : mod_rawdata.front();
        mline += nemesis::syntax::Spaces() + nemesis::syntax::Aster(cond->GetExpression());
        rawdata.emplace_back(mline);
    }

    VecNstr mlines;
    raw->base.GetRawData(mlines);
    std::for_each(mlines.begin(), mlines.end(), [&](nemesis::Line& line) {
        line.append(nemesis::syntax::Spaces() + nemesis::syntax::LowerOriginal());
    });
    rawdata.insert(rawdata.end(), mlines.begin(), mlines.end());

    for (size_t i = 0; i < mod_rawdata_list.size(); ++i)
    {
        auto& mod_rawdata = mod_rawdata_list[i];

        if (mod_rawdata.size() <= 1) continue;

        rawdata.emplace_back(nemesis::syntax::ModCode(modlist[i]->GetExpression()));
        rawdata.insert(rawdata.end(), mod_rawdata.begin() + 1, mod_rawdata.end());
        rawdata.emplace_back(nemesis::syntax::Close());
    }
}

void nemesis::LinkedAnimData::GetRawData(VecNstr& rawdata) const
{
    (this->*get_rawdata)(rawdata);
}

void nemesis::LinkedAnimData::GetRawHeaderData(VecNstr& rawdata) const
{
    get_rawdata = &nemesis::LinkedAnimData::_GetRawHeaderData;
    _GetRawHeaderData(rawdata);
}

void nemesis::LinkedAnimData::GetRawBaseData(VecNstr& rawdata) const
{
    get_rawdata = &nemesis::LinkedAnimData::_GetRawBaseData;
    _GetRawBaseData(rawdata);
}
