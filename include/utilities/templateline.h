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

        mutable VecStr cacheblocks;

	public:
        TemplateLine(const nemesis::Line& line);

        void AddTemplate(WPtr<nemesis::Template> templateptr);
        void TryAddLineProcess(SPtr<nemesis::LineProcess> processptr);

        SPtr<nemesis::Template> GetTemplate() const noexcept;
        SPtr<nemesis::LineProcess> GetProcess() const noexcept;
        VecStr GetByBlocks() const noexcept;

        bool HasProcess() const noexcept override;
        nemesis::Line Process(nemesis::ScopeInfo& scopeinfo) const override;
	};
}
