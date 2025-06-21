#pragma once

#include "Core/SemanticManager.h"

#include "Core/Statement/ForEachStatement.h"

namespace nemesis
{
	struct ParsingForEachScope
    {
    private:
        nemesis::SemanticManager* Manager;
        nemesis::ForEachStatement::ForEachType Type;
        const std::string* Expression_Ptr;

    public:
        ParsingForEachScope(const nemesis::ForEachStatement& statement, nemesis::SemanticManager& manager);
        ~ParsingForEachScope() noexcept;
    };
}
