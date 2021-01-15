#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace nemesis
{
    struct OptionModel;

    struct Option
    {
    private:
        std::string name;
        std::string query;
        std::unordered_map<std::string, std::string> variables;

        const nemesis::OptionModel& model;

    public:
        Option(const std::string& query, const OptionModel& _model) noexcept;

        std::string GetName() const noexcept;
        const nemesis::OptionModel& GetModel() const noexcept;
        std::string GetVariable(std::string key) const;
    };
}
