
#include "core/linkedpreprocessline.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::LinkedPreprocessLine::LinkedPreprocessLine(const nemesis::Line& line) noexcept
{
    raw = std::make_shared<nemesis::PreprocessLine>(line);
}

nemesis::LinkedPreprocessLine::LinkedPreprocessLine(
    const nemesis::Linked<nemesis::PreprocessLine, LinkedPreprocessLine>& line) noexcept
    : nemesis::Linked<nemesis::PreprocessLine, LinkedPreprocessLine>(line)
{
}

void nemesis::LinkedPreprocessLine::TryAddLineProcess(SPtr<nemesis::LineProcess>& processptr)
{
    if (!raw) throw std::runtime_error("Failed to add process to line. Raw value cannot be found");

    raw->TryAddLineProcess(processptr);
}

void nemesis::LinkedPreprocessLine::AddBehavior(nemesis::HkxBehavior& behavior)
{
    if (!raw) throw std::runtime_error("Failed to add behavior. Raw value cannot be found");

    raw->AddBehavior(behavior);
}

void nemesis::LinkedPreprocessLine::AddTemplate(nemesis::Template& templateref)
{
    if (!raw) throw std::runtime_error("Failed to add behavior. Raw value cannot be found");

    raw->AddTemplate(templateref);
}

void nemesis::LinkedPreprocessLine::SetToStandAlone()
{
    for (size_t i = 0; i < conditions.size(); i++)
    {
        auto& cond = conditions[i];
        SetToStandAlone(*cond);
    }
}

void nemesis::LinkedPreprocessLine::SetToStandAlone(nemesis::LinkedPreprocessLine& linkedline)
{
    if (linkedline.HasCondition())
    {
        SetToStandAlone(linkedline.GetFrontCondition());
        return;
    }

    auto raw = linkedline.GetRawPtr();

    if (!raw) return;

    if (raw->IsStandAlone()) return;

    raw->SetToStandAlone();
}

void nemesis::LinkedPreprocessLine::SetToStandAlone(LinkedCondition& linkedcondition)
{
    auto& list = linkedcondition.GetDataList();

    if (list.empty()) return;

    SetToStandAlone(*list.front());
}

bool nemesis::LinkedPreprocessLine::HasProcess() const
{
    if (!raw) throw std::runtime_error("Failed to find process. Raw value cannot be found");

    return raw->HasProcess();
}

std::string nemesis::LinkedPreprocessLine::RawToString() const
{
    return raw->ToString();
}

VecNstr nemesis::LinkedPreprocessLine::GetProcessedLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines;
    auto prepro_lines = GetCompiledData(scopeinfo);
    std::transform(prepro_lines.begin(),
                   prepro_lines.end(),
                   std::back_inserter(lines),
                   [&scopeinfo](const nemesis::PreprocessLine* prepro_line)
                   { return prepro_line->Process(scopeinfo); });
    return lines;
}
