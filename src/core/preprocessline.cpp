#include "core/preprocessline.h"

#include "hkx/HkxBehaviorFile.h"

#include "utilities/lineprocess.h"
#include "utilities/templateline.h"

#include "scope/scopeinfo.h"

nemesis::PreprocessLine::PreprocessLine(const nemesis::Line& line) noexcept
    : nemesis::Line(line)
{
}

nemesis::PreprocessLine::PreprocessLine(const nemesis::PreprocessLine& linkedline) noexcept
    : nemesis::Line(static_cast<const nemesis::Line&>(linkedline))
{
    cacheblocks = linkedline.cacheblocks;

    if (linkedline.processptr)
    {
        processptr = linkedline.processptr;
    }
    
    if (linkedline.templateptr)
    {
        templateptr = linkedline.templateptr;
    }

    if (linkedline.pbehavior)
    {
        pbehavior = linkedline.pbehavior;
    }
}

void nemesis::PreprocessLine::AddBehavior(nemesis::HkxBehaviorFile& behavior)
{
    file      = &behavior;
    pbehavior = &behavior;
}

void nemesis::PreprocessLine::AddTemplate(nemesis::Template& templateref)
{
    file        = &templateref;
    templateptr = &templateref;
}

void nemesis::PreprocessLine::TryAddLineProcess(SPtr<nemesis::LineProcess>& processptr)
{
    if (processptr == nullptr) return;

    if (processptr->IsEmpty()) return;

    for (auto& ch : base)
    {
        cacheblocks.emplace_back(std::string(1, ch));
    }

    this->processptr = processptr;
    this->processptr->AddProcessLinePtr(this);
}

bool nemesis::PreprocessLine::IsStandAlone()
{
    return s_path != nullptr;
}

void nemesis::PreprocessLine::SetToStandAlone()
{
    s_path = r_path->shared_from_this();
}

const nemesis::File* nemesis::PreprocessLine::GetFile() const noexcept
{
    return file;
}

const nemesis::Template* nemesis::PreprocessLine::GetTemplate() const noexcept
{
    return templateptr;
}

const nemesis::LineProcess* nemesis::PreprocessLine::GetProcess() const noexcept
{
    return processptr.get();
}

VecStr nemesis::PreprocessLine::GetByBlocks() const noexcept
{
    return cacheblocks;
}

bool nemesis::PreprocessLine::HasProcess() const noexcept
{
    return processptr != nullptr;
}

nemesis::Line nemesis::PreprocessLine::Process(nemesis::ScopeInfo& scopeinfo) const
{
    if (!HasProcess()) return *this;
    
    return processptr->Compile(scopeinfo);
}
