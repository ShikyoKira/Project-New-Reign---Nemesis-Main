#pragma once

#include "core/Variables/VariableFactory.h"

namespace nemesis
{
	struct Variable
    {
    protected:
        std::string expression;

    public:
        VecStr GetValues() const;

        std::string_view GetExpression() const;

        friend VariableFactory;
    };
}
