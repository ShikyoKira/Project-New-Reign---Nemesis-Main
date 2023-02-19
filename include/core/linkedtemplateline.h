#pragma once

#include "utilities/linkedvar.h"
#include "utilities/templateline.h"

namespace nemesis
{
    struct ScopeInfo;

    struct LinkedTemplateLine : public nemesis::LinkedVar<nemesis::TemplateLine>
    {
        using nemesis::LinkedVar<nemesis::TemplateLine>::LinkedVar;

        nemesis::TemplateLine GetTemplateLine();
        nemesis::TemplateLine CompileLine(nemesis::ScopeInfo& scopeinfo) const;
    };
}
