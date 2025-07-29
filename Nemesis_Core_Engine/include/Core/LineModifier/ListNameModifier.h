#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/ListNameStatement.h"

namespace nemesis
{
    struct ListNameModifier : public nemesis::StandardLineModifier<nemesis::ListNameStatement>
    {
    public:
        ListNameModifier(size_t begin,
                         size_t end,
                         const std::string& expression,
                         size_t linenum,
                         const std::filesystem::path& filepath,
                         const nemesis::SemanticManager& manager);
    };
}
