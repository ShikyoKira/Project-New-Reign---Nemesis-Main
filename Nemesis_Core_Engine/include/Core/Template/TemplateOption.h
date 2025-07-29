#pragma once

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    struct TemplateOptionModel;

	struct TemplateOption
    {
    private:
        std::string Expression;
        std::string ExpressionName;
        const nemesis::TemplateOptionModel& Model;
        UMap<std::string, std::string> Variables;

    public:
        TemplateOption(const std::string& expression, const std::string& expression_name, const nemesis::TemplateOptionModel& model);

        const std::string& GetExpression() const;
        const std::string& GetName() const;
        const std::string& GetExpressionName() const;
        const VecStr& GetAliases() const;

        bool HasAlias(const std::string& alias) const;

        const std::string& GetVariableValue(const std::string& name) const;
        void AddVariable(const std::string& name, const std::string& value);
    };
}
