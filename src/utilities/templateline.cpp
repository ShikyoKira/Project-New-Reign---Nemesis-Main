#include "utilities/lineprocess.h"
#include "utilities/templateline.h"

void nemesis::TemplateLine::TryAddLineProcess(SPtr<nemesis::LineProcess> processptr)
{
    if (processptr->IsEmpty()) return;

    processptr->AddTemplateLinePtr(enable_shared_from_this::shared_from_this());
    this->processptr = processptr;
}

SPtr<nemesis::Template> nemesis::TemplateLine::GetTemplate()
{
    return templateptr.lock();
}

nemesis::TemplateLine::TemplateLine(const nemesis::Line& line)
{
    this->append(line);
    this->linenum = line.GetLineNumber();
}

void nemesis::TemplateLine::AddTemplate(WPtr<nemesis::Template> templateptr)
{
    this->templateptr = templateptr;
}

SPtr<nemesis::LineProcess> nemesis::TemplateLine::GetProcess() const
{
    return processptr;
}
