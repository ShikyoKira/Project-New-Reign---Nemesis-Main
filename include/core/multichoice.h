#pragma once

#include "utilities/types.h"

#include "core/condition.h"

namespace nemesis
{
    struct ScopeInfo;

    struct MultiChoice
    {
        struct Choice
        {
            size_t pointA;
            size_t pointB;

        private:
            UPtr<Condition> condition;

        public:
            Choice(size_t pointA, size_t pointB, UPtr<Condition>& condition);

            bool IsTrue(nemesis::ScopeInfo& scopeinfo) const;
        };

    private:
        Vec<Choice> choices;

    public:
        MultiChoice() = default;

        void AddChoice(size_t pointA, size_t pointB, UPtr<Condition>& condition);
        const Vec<Choice>& GetChoiceList() const noexcept;
    };
}
