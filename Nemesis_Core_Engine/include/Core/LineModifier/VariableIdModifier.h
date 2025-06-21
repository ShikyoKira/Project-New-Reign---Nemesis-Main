#pragma once

#include "Core/LineModifier/BaseIdModifier.h"

#include "Core/Statement/VariableIdStatement.h"

namespace nemesis
{
    struct VariableIdModifier : public nemesis::BaseIdModifier<nemesis::VariableIdStatement>
    {
    public:
        VariableIdModifier(size_t begin,
                           size_t end,
                           const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath,
                           const nemesis::SemanticManager& manager);
    };
}
