#include "hkx/HkxNodeObject.h"
#include "hkx/HkxBehaviorFile.h"

#include "utilities/linkedvar.h"
#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::HkxNodeObject::Exporter::Exporter(const HkxNodeObject& host, VecNstr& lines)
    : hostref(host)
    , lines(lines)
{
}

void nemesis::HkxNodeObject::Exporter::Compile(VecNstr& storeline)
{
    for (auto& curstack : hostref.lines)
    {
        bool uniqueskip = false;
        std::shared_ptr<nemesis::Line> line;
        VecStr lineblocks;

        if (curstack.nestedcond.empty())
        {
            line = TryProcessLine(curstack);
        }
        else
        {
            uniqueskip = true;
        }

        if (!uniqueskip)
        {
            storeline.emplace_back(*line);
        }

        if (error) throw nemesis::exception();
    }
}

SPtr<nemesis::Line> nemesis::HkxNodeObject::Exporter::TryProcessLine(const nemesis::LinkedVar<nemesis::Line>& line)
{
    if (!line.hasProcess) return line.raw;

    // continue here
    //process.blocksCompile(line.lineblocks, );

    return std::make_shared<nemesis::Line>();
}

nemesis::HkxNodeObject::HkxNodeObject(const nemesis::Line& _id, const nemesis::HkxBehaviorFile& _parent)
    : id(_id)
    , parent(_parent)
{
}

void nemesis::HkxNodeObject::InitializeStream()
{
    stream.emplace_back(&lines);
}

void nemesis::HkxNodeObject::AddLine(const nemesis::Line& line, const SPtr<nemesis::ConditionInfo>& conditioninfo)
{
    if (line == ns::DeleteLine()) return;

    if (!conditioninfo)
    {
        stream.back()->emplace_back(line);
        return;
    }

    auto type = conditioninfo->GetType();

    switch (type)
    {
        case nemesis::CondType::NONE:
        {
            stream.back()->emplace_back(line);
            break;
        }
        case nemesis::CondType::IF:
        case nemesis::CondType::FOREACH:
        {
            auto cond = nemesis::CondVar<nemesis::Line>(conditioninfo->GetCondition(), type);
            stream.back()->emplace_back(nemesis::LinkedVar<nemesis::Line>(cond, line.GetLineNumber()));
            stream.emplace_back(&stream.back()->back().backCond().rawlist);
            break;
        }
        case nemesis::CondType::ELSEIF:
        {
            auto cond    = nemesis::CondVar<nemesis::Line>(conditioninfo->GetCondition(), type);
            stream.back() = &stream.back()->back().addCond(cond).rawlist;
            break;
        }
        case nemesis::CondType::ELSE:
        {
            auto cond    = nemesis::CondVar<nemesis::Line>(type);
            stream.back() = &stream.back()->back().addCond(cond).rawlist;
            break;
        }
        case nemesis::CondType::ENDIF:
        case nemesis::CondType::CLOSE:
        {
            stream.pop_back();
            break;
        }
    }
}

void nemesis::HkxNodeObject::Compile(VecNstr& storeline) const
{
    for (auto& line : lines)
    {
        if (line.HasCondition())
        {
            for (auto& condition : line.nestedcond)
            {
            }
        }
    }
}

void nemesis::HkxNodeObject::getlines(VecNstr& storeline) const
{
    for (auto& line : lines)
    {
        //getLinkedLines(line, storeline);
    }
}

void getLinkedLines(const nemesis::LinkedVar<nemesis::HkxNodeObject>& linkednode, VecNstr& storeline)
{
    Vec<const nemesis::CondVar<nemesis::HkxNodeObject>*> tempstore;

    //for (auto& cond : linkednode.nestedcond)
    //{
    //    switch (cond.conditionType)
    //    {
    //        case nemesis::CondType::MOD_CODE:
    //        {
    //            tempstore.emplace_back(&cond);
    //            break;
    //        }
    //        case nemesis::CondType::FOREACH:
    //        {
    //            storeline.emplace_back(ns::ForEach(cond.conditions));

    //            for (auto& each : cond.rawlist)
    //            {
    //                getLinkedLines(each, storeline);
    //            }

    //            storeline.emplace_back(nemesis::syntax::Close());
    //            break;
    //        }
    //    }
    //}

    //if (!tempstore.empty())
    //{
    //    if (linkednode.raw)
    //    {
    //        for (auto& each : tempstore)
    //        {
    //            storeline.emplace_back(ns::ModCode(each->conditions));
    //            getLinkedLines(each->rawlist[0], storeline);
    //        }

    //        storeline.emplace_back(ns::ModOriginal());
    //        linkednode.raw->getlines(storeline);
    //        storeline.emplace_back(ns::Close());
    //        return;
    //    }

    //    for (auto& each : tempstore)
    //    {
    //        storeline.emplace_back(ns::ModCode(each->conditions));
    //        getLinkedLines(each->rawlist[0], storeline);
    //        storeline.emplace_back(ns::Close());
    //    }
    //}
    //else if (linkednode.raw)
    //{
    //    linkednode.raw->getlines(storeline);
    //}
}

//std::string_view nemesis::HkxNodeObject::GetIDView(std::string_view line)
//{
//    std::string_view openSyntax = "<hkobject name=\"";
//    line.remove_prefix(line.find(openSyntax) + openSyntax.length());
//    line.remove_suffix(line.length() - line.find("\""));
//    return line;
//}
//
//std::string nemesis::HkxNodeObject::GetID(const std::string& line)
//{
//    return std::string(GetIDView(line));
//}