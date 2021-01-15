#include "utilities/lineprocess.h"

using namespace std;

void nemesis::LineProcess::BlockScopeValidation(const nemesis::Process& process) const
{
    uint begin = process.GetBegin();
    uint end   = process.GetEnd();

    for (auto& blockGroup : blockGroupBySize)
    {
        for (auto& block : blockGroup.second)
        {
            if (IsBlockOutOfScope(*block, begin, end))
            {
                ErrorMessage(1193,
                             block->GetFormat(),
                             block->GetBehaviorFile(),
                             pTemplateLine.lock()->GetLineNumber());
            }
        }
    }
}

bool nemesis::LineProcess::IsEmpty()
{
    return blockGroupBySize.empty();
}

void nemesis::LineProcess::AddTemplateLinePtr(std::shared_ptr<TemplateLine> pTemplateLine)
{
    this->pTemplateLine = pTemplateLine;

    for (auto& blockGroup : blockGroupBySize)
    {
        for (auto& process : blockGroup.second)
        {
            process->AddTemplateLinePtr(pTemplateLine);
        }
    }
}

void nemesis::LineProcess::AddProcess(unique_ptr<nemesis::Process>& process)
{
    BlockScopeValidation(*process);
    blockGroupBySize[process->GetSize()].push_back(move(process));
}

bool nemesis::LineProcess::IsBlockOutOfScope(const nemesis::Process& p, uint begin, uint end)
{
    if (end < begin) return false;

    if (p.GetBegin() < begin)
    {
        // p_front < front < p_back < back
        return begin < p.GetEnd() && p.GetEnd() < end;
    }

    // front < p_front < back < p_back
    return end < p.GetEnd();
}
