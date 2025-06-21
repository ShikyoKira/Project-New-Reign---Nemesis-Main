#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/SubstringStatement.h"

namespace nemesis
{
    struct SubstringModifier : public nemesis::StandardLineModifier<nemesis::SubstringStatement>
    {
    public:
        SubstringModifier(size_t begin,
                          size_t end,
                          const std::string& expression,
                          size_t linenum,
                          const std::filesystem::path& filepath,
                          const nemesis::SemanticManager& manager);
    };
}
