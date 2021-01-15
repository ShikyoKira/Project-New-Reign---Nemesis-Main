#include "utilities/condtype.h"
#include "utilities/conditioninfo.h"
#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::ConditionInfo::ConditionInfo(const std::string& line)
{
    operator=(line);
}

nemesis::ConditionInfo::ConditionInfo(const nemesis::Line& line)
{
    operator=(line.ToString());
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

std::string& nemesis::ConditionInfo::operator=(const std::string& line)
{
    type = nemesis::ParseConditionType(line);

    switch (type)
    {
        case nemesis::CondType::IF:
            condition = ns::IfCondition(line);
            break;
        case nemesis::CondType::ELSEIF:
            condition = ns::ElseIfCondition(line);
            break;
        case nemesis::CondType::FOREACH:
            condition = ns::ForEachCondition(line);
            break;
        case nemesis::CondType::MOD_CODE:
            condition = ns::ModCodeCondition(line);
            break;
        case nemesis::CondType::ASTERISK:
            condition = ns::AsterCondition(line);
            break;
        default:
            condition.clear();
            return condition;
    }

    return condition;
}

nemesis::Line& nemesis::ConditionInfo::operator=(const nemesis::Line& line)
{
    operator=(line.ToString());
    condition.SetLineNumber(line.GetLineNumber());
    return condition;
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

Deq<nemesis::Line>& nemesis::ConditionInfo::GetRefContents() noexcept
{
    return contents;
}

const Deq<nemesis::Line>& nemesis::ConditionInfo::GetContents() const noexcept
{
    return contents;
}

const std::string& nemesis::ConditionInfo::GetCondition() const noexcept
{
    return condition.ToString();
}

uint nemesis::ConditionInfo::GetLineNumber() const noexcept
{
    return condition.GetLineNumber();
}

const nemesis::CondType& nemesis::ConditionInfo::GetType() const noexcept
{
    return type;
}
