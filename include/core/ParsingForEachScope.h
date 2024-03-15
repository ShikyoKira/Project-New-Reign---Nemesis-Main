#pragma once

#include "core/SemanticManager.h"

#include "core/Statements/ForEachStatement.h"

namespace nemesis
{
	struct ParsingForEachScope
    {
    private:
        nemesis::SemanticManager* Manager;
        bool IsOption;

    public:
        ParsingForEachScope(const nemesis::ForEachStatement& statement, nemesis::SemanticManager& manager);
        ~ParsingForEachScope();
    };
}
