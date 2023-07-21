#pragma once

namespace nemesis
{
    struct TemplateOptionModel;

	struct TemplateOption
    {
    private:
        std::string Expression;
        std::string Name;
        VecStr Children;
        UMap<std::string, std::string> Variables;

    public:
        TemplateOption(const std::string& expression, const std::string& name, const VecStr& children);

        const std::string& GetExpression() const;
        const std::string& GetName() const;
        const VecStr& GetChildren() const;

        bool HasChildren(const std::string& child_name) const;

        const std::string& GetVariableValue(const std::string& name) const;
        void AddVariable(const std::string& name, const std::string& value);
    };
}
