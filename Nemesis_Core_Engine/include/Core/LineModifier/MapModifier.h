#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/MapStatement.h"

namespace nemesis
{
    struct SemanticManager;

	struct MapModifier : public nemesis::StandardLineModifier<nemesis::MapStatement>
    {
    public:
        MapModifier(size_t begin,
                    size_t end,
                    const std::string& expression,
                    size_t linenum,
                    const std::filesystem::path& filepath,
                    const nemesis::SemanticManager& manager);
    };
}
