#include "utilities/lineprocess.h"

#include "core/preprocessline.h"

#include "generate/animation/singletemplate.h"

using namespace std;

void nemesis::LineProcess::BlockScopeValidation(const nemesis::Process& process) const
{
    size_t begin = process.GetBegin();
    size_t end   = process.GetEnd();

    for (auto& blockGroup : blockGroupBySize)
    {
        for (auto& block : blockGroup.second)
        { 
            if (!IsBlockOutOfScope(*block, begin, end)) continue;

            ErrorMessage(1193, block->GetFormat(), block->GetBehaviorFile(), plinkedline->GetLineNumber());
        }
    }
}

bool nemesis::LineProcess::IsEmpty()
{
    return blockGroupBySize.empty();
}

void nemesis::LineProcess::AddProcessLinePtr(nemesis::PreprocessLine* linkedline)
{
    plinkedline = linkedline;

    // instead of parsing from the inner most nested function to the outer most function
    // this is done by parsing the shortest function to the losngest function as...
    // outer most function = longest function
    // inner most function = shortest function
    for (auto& blockGroup : blockGroupBySize)
    {
        for (auto& process : blockGroup.second)
        {
            process->AddProcessLinePtr(linkedline);
        }
    }
}

void nemesis::LineProcess::AddChoice(size_t pointA, size_t pointB, UPtr<Condition>& conditionptr)
{
    multichoice.AddChoice(pointA, pointB, conditionptr);
}

void nemesis::LineProcess::AddProcess(UPtr<nemesis::Process>& process)
{
    BlockScopeValidation(*process);
    blockGroupBySize[process->GetSize()].emplace_back(move(process));
}

nemesis::Line nemesis::LineProcess::Compile(nemesis::ScopeInfo& scopeinfo) const
{
    VecStr blocks = plinkedline->GetByBlocks();

    for (auto& blockGroup : blockGroupBySize)
    {
        for (auto& process : blockGroup.second)
        {
            process->Compile(blocks, scopeinfo);
        }
    }

    nemesis::Line line(*plinkedline);
    line.clear();
    std::for_each(blocks.begin(), blocks.end(), [&](const std::string& block) { line.append(block); });
    return line;
}

const nemesis::MultiChoice& nemesis::LineProcess::GetMultiChoice() const noexcept
{
    return multichoice;
}

bool nemesis::LineProcess::IsBlockOutOfScope(const nemesis::Process& p, size_t begin, size_t end)
{
    if (end < begin) return false;

    if (p.GetBegin() < begin)
    {
        // p_begin < begin < p_end < end
        return begin < p.GetEnd() && p.GetEnd() < end;
    }

    // begin < p_begin < end < p_end
    return p.GetBegin() < end && end < p.GetEnd();
}
