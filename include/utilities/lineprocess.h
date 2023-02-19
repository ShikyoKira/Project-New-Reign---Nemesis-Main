#pragma once

#include "core/multichoice.h"

#include "utilities/process.h"

struct AnimThreadInfo;

namespace nemesis
{
    struct TemplateLine;
    struct ScopeInfo;

    struct LineProcess
    {
    private:
        using BlockGroup = Vec<UPtr<nemesis::Process>>;

        MultiChoice multichoice;
        Map<size_t, BlockGroup> blockGroupBySize;
        nemesis::PreprocessLine* plinkedline;

        void BlockScopeValidation(const nemesis::Process& process) const;

    public:
        LineProcess() = default;

        bool IsEmpty();
        void AddProcessLinePtr(nemesis::PreprocessLine* linkedline);
        void AddChoice(size_t pointA, size_t pointB, UPtr<Condition>& conditionptr);
        void AddProcess(UPtr<nemesis::Process>& process);
        nemesis::Line Compile(nemesis::ScopeInfo& scopeinfo) const;

        const MultiChoice& GetMultiChoice() const noexcept;
    private:
        static bool IsBlockOutOfScope(const nemesis::Process& process, size_t begin, size_t end);
    };
}
