#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/MathStatement.h"

namespace nemesis
{
    struct MathModifier : public nemesis::StandardLineModifier<nemesis::MathStatement>
    {
    public:
        MathModifier(size_t begin,
                     size_t end,
                     const std::string& expression,
                     size_t linenum,
                     const std::filesystem::path& filepath,
                     const nemesis::SemanticManager& manager);

    };
}
