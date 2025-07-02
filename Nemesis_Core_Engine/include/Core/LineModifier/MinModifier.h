#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/MinStatement.h"

namespace nemesis
{
    struct MinModifier : public nemesis::StandardLineModifier<nemesis::MinStatement>
    {
    public:
        MinModifier(size_t begin,
                    size_t end,
                    const std::string& expression,
                    size_t linenum,
                    const std::filesystem::path& filepath,
                    const nemesis::SemanticManager& manager);
    };
}
