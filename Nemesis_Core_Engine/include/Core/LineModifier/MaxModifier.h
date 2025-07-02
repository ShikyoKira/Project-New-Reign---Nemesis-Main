#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/MaxStatement.h"

namespace nemesis
{
    struct MaxModifier : public nemesis::StandardLineModifier<nemesis::MaxStatement>
    {
    public:
        MaxModifier(size_t begin,
                     size_t end,
                     const std::string& expression,
                     size_t linenum,
                     const std::filesystem::path& filepath,
                     const nemesis::SemanticManager& manager);
    };
}
