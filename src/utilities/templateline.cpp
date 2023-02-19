#include "utilities/lineprocess.h"
#include "utilities/templateline.h"

nemesis::TemplateLine::TemplateLine(const nemesis::Line& line)
{
    this->append(line);
    this->linenum = line.GetLineNumber();
}

void nemesis::TemplateLine::AddTemplate(WPtr<nemesis::Template> templateptr)
{
    this->templateptr = templateptr;
}

void nemesis::TemplateLine::TryAddLineProcess(SPtr<nemesis::LineProcess> processptr)
{
    if (processptr->IsEmpty()) return;

    //processptr->AddTemplateLinePtr(*this);
    this->processptr = processptr;
}

SPtr<nemesis::Template> nemesis::TemplateLine::GetTemplate() const noexcept
{
    return templateptr.lock();
}

SPtr<nemesis::LineProcess> nemesis::TemplateLine::GetProcess() const noexcept
{
    return processptr;
}

VecStr nemesis::TemplateLine::GetByBlocks() const noexcept
{
    if (!cacheblocks.empty()) return cacheblocks;

    if (empty()) return cacheblocks;

    for each (auto& ch in *this)
    {
        cacheblocks.emplace_back(std::string(1, ch));
    }

    return cacheblocks;
}

bool nemesis::TemplateLine::HasProcess() const noexcept
{
    return processptr != nullptr;
}

nemesis::Line nemesis::TemplateLine::Process(nemesis::ScopeInfo& scopeinfo) const
{
    if (!HasProcess()) return *this;

    return processptr->Compile(scopeinfo);
}
