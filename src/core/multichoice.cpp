#include "core/multichoice.h"

nemesis::MultiChoice::Choice::Choice(size_t pointA, size_t pointB, UPtr<Condition>& condition)
    : pointA(pointA)
    , pointB(pointB)
    , condition(std::move(condition))
{
}

bool nemesis::MultiChoice::Choice::IsTrue(nemesis::ScopeInfo& scopeinfo) const
{
    return condition->IsTrue(scopeinfo);
}

void nemesis::MultiChoice::AddChoice(size_t pointA, size_t pointB, UPtr<Condition>& condition)
{
    choices.emplace_back(std::move(Choice(pointA, pointB, condition)));
}

const Vec<nemesis::MultiChoice::Choice>& nemesis::MultiChoice::GetChoiceList() const noexcept
{
    return choices;
}
