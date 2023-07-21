#include "utilities/option.h"
#include "utilities/optionmodel.h"

nemesis::Option::Option(const std::string& query,
                        const nemesis::OptionModel& model,
                        const nemesis::AnimationRequest& request) noexcept
    : model(model)
    , Request(&request)
    , query(query)
    , name(model.GetName())
    , variables(model.ParseVariables(query))
{
    success = model.HasVariable() != variables.empty();
}

nemesis::Option::Option(const std::string& query,
                        const nemesis::OptionModel& model,
                        const nemesis::AnimQuery& animquery) noexcept
    : model(model)
    , animquery(&animquery)
    , query(query)
    , name(model.GetName())
    , variables(model.ParseVariables(query))
{
    success     = model.HasVariable() != variables.empty();
}

const nemesis::AnimationRequest& nemesis::Option::GetAnimationRequest() const
{
    return *Request;
}

const nemesis::AnimQuery& nemesis::Option::GetAnimQuery() const
{
    return *animquery;
}

const nemesis::OptionModel& nemesis::Option::GetModel() const noexcept
{
    return model;
}

std::string nemesis::Option::GetName() const noexcept
{
    return name;
}

std::string nemesis::Option::GetVariable(const std::string& key) const
{
    for (auto& var : variables)
    {
        if (nemesis::iequals(var.first, key)) return var.second;
    }

    return "";
}

bool nemesis::Option::IsSuccess() const noexcept
{
    return success;
}
