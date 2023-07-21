#pragma once

#include "core/Variables/Variable.h"

#include "utilities/animquery.h"

namespace nemesis
{
	struct ForEachVariable : protected nemesis::Variable
    {
    protected:

    public:

        std::string_view GetExpression() const;
    };
}
