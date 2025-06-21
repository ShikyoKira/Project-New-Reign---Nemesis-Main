#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/OptionVariableStatement.h"

namespace nemesis
{
    struct OptionVariableModifier : public nemesis::StandardLineModifier<nemesis::OptionVariableStatement>
    {
    public:
        OptionVariableModifier(size_t begin,
                               size_t end,
                               const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);
    };
}
