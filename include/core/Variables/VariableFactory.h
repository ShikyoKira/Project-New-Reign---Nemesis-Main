#pragma once

#include <string>

#include "utilities/types.h"

namespace nemesis
{
    struct Variable;

	struct VariableFactory
    {
    private:

    public:
        VariableFactory(const std::string& expression);
        
        UPtr<Variable> Create();
    };
}
