#include "core/SubTemplateRequest.h"

nemesis::SubTemplateRequest::SubTemplateRequest(const VecStr& arguments) noexcept

    : SubTemplateName(arguments[1])
    , Arguments(arguments)
{
}

const std::string& nemesis::SubTemplateRequest::GetSubTemplateName() const noexcept
{
    return SubTemplateName;
}

const std::string& nemesis::SubTemplateRequest::GetArgument(size_t index) const noexcept
{
    return Arguments[index];
}

const VecStr& nemesis::SubTemplateRequest::GetArguments() const noexcept
{
    return Arguments;
}
