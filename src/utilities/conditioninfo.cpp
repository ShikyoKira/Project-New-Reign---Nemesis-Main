#include "utilities/condtype.h"
#include "utilities/conditioninfo.h"
#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::ConditionInfo::ConditionInfo(const nemesis::Line& line)
    : refline(&line)
{
    operator=(line);
}

bool nemesis::ConditionInfo::operator==(const std::string& condition) const noexcept
{
    return this->condition == condition;
}

bool nemesis::ConditionInfo::operator==(const ConditionInfo& condition) const noexcept
{
    return this->condition == condition.condition;
}

bool nemesis::ConditionInfo::operator==(const nemesis::CondType type) const noexcept
{
    return this->type == type;
}

bool nemesis::ConditionInfo::operator!=(const std::string& condition) const noexcept
{
    return this->condition != condition;
}

bool nemesis::ConditionInfo::operator!=(const ConditionInfo& condition) const noexcept
{
    return this->condition != condition.condition;
}

bool nemesis::ConditionInfo::operator!=(const nemesis::CondType type) const noexcept
{
    return this->type != type;
}

bool nemesis::ConditionInfo::IsSuccess() const noexcept
{
    return Success;
}

std::string& nemesis::ConditionInfo::operator=(const std::string& line)
{
    refline = nullptr;
    type = nemesis::ParseConditionType(line);

    switch (type)
    {
        case nemesis::CondType::IF:
        {
            condition = ns::IfCondition(line);
            break;
        }
        case nemesis::CondType::ELSEIF:
        {
            condition = ns::ElseIfCondition(line);
            break;
        }
        case nemesis::CondType::ELSE:
        case nemesis::CondType::ORIGINAL:
        {
            condition.clear();
            break;
        }
        case nemesis::CondType::FOREACH:
        {
            condition = ns::ForEachCondition(line);
            break;
        }
        case nemesis::CondType::MOD_CODE:
        {
            condition = ns::ModCodeCondition(line);
            break;
        }
        case nemesis::CondType::ASTERISK:
        {
            condition = ns::AsterCondition(line);

            if (refline)
            {
                contents.emplace_back(refline->substr(0, refline->rfind(ns::Spaces() + ns::Aster())));
                break;
            }

            contents.emplace_back(line.substr(0, line.rfind(ns::Spaces() + ns::Aster())));
            break;
        }
        case nemesis::CondType::LOWER_ORIGINAL:
        {
            condition.clear();
            size_t point = line.rfind(ns::Spaces() + ns::LowerOriginal());
            nemesis::Line nline(refline ? refline->substr(0, point) : line.substr(0, point));
            contents.emplace_back(nline);
        }
        default:
        {
            Success = false;
            condition.clear();
            return condition;
        }
    }

    return condition;
}

nemesis::Line& nemesis::ConditionInfo::operator=(const nemesis::Line& line)
{
    try
    {
        refline = &line;
        operator=(line.ToString());
        condition.SetLineNumber(line.GetLineNumber());
        return condition;
    }
    catch (const std::exception&)
    {
        ErrorMessage(1205, line.GetClassName(), line.GetFilePath(), line.GetLineNumber());
    }
}

void nemesis::ConditionInfo::AddContent(const nemesis::Line& line)
{
    contents.emplace_back(line);
}

void nemesis::ConditionInfo::Pop_Front()
{
    contents.pop_front();
}

void nemesis::ConditionInfo::Pop_Back()
{
    contents.pop_back();
}

void nemesis::ConditionInfo::SetCondition(const std::string& condition, nemesis::CondType type) 
{
    this->condition = condition;
    this->type      = type;
}

void nemesis::ConditionInfo::SetCondition(const nemesis::Line& condition, nemesis::CondType type)
{
    this->condition = condition;
    this->refline   = &condition;
    this->type      = type;
}

Deq<nemesis::Line>& nemesis::ConditionInfo::GetRefContents() noexcept
{
    return contents;
}

const Deq<nemesis::Line>& nemesis::ConditionInfo::GetContents() const noexcept
{
    return contents;
}

std::string nemesis::ConditionInfo::GetCondition() const noexcept
{
    return condition.ToString();
}

const nemesis::Line& nemesis::ConditionInfo::GetRefLine() const noexcept
{
    return *refline;
}

size_t nemesis::ConditionInfo::GetLineNumber() const noexcept
{
    return condition.GetLineNumber();
}

nemesis::CondType nemesis::ConditionInfo::GetType() const noexcept
{
    return type;
}
