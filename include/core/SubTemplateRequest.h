#pragma once

#include "utilities/types.h"

namespace nemesis
{
	struct SubTemplateRequest
    {
    private:
        std::string SubTemplateName;
        VecStr Arguments;

    public:
        SubTemplateRequest(const VecStr& arguments) noexcept;

        const std::string& GetSubTemplateName() const noexcept;
        const std::string& GetArgument(size_t index) const noexcept;
        const VecStr& GetArguments() const noexcept;
    };
}
