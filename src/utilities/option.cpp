#include "utilities/option.h"
#include "utilities/optionmodel.h"

nemesis::Option::Option(const std::string& query, const OptionModel& _model) noexcept
    : model(_model)
{
    this->query = query;
    name        = model.GetName();
}

std::string nemesis::Option::GetName() const noexcept
{
    return name;
}

const nemesis::OptionModel& nemesis::Option::GetModel() const noexcept
{
    return model;
}

std::string nemesis::Option::GetVariable(std::string key) const
{
    auto itr = variables.find(key);

    if (itr == variables.end()) return "";

    return itr->second;
}
