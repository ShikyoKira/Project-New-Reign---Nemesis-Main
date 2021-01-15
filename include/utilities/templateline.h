#pragma once

#include "utilities/line.h"
#include "utilities/types.h"

namespace nemesis
{
    struct Template;
    struct LineProcess;

	struct TemplateLine : public nemesis::Line, std::enable_shared_from_this<nemesis::TemplateLine>
	{
    private:
        SPtr<nemesis::LineProcess> processptr;
        WPtr<nemesis::Template> templateptr;

	public:
        TemplateLine(const nemesis::Line& line);

        void AddTemplate(WPtr<nemesis::Template> templateptr);
        void TryAddLineProcess(SPtr<nemesis::LineProcess> processptr);
        SPtr<nemesis::Template> GetTemplate();
        SPtr<nemesis::LineProcess> GetProcess() const;
	};
}
