#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/Crc32Statement.h"

namespace nemesis
{
    struct Crc32Modifier : public nemesis::StandardLineModifier<nemesis::Crc32Statement>
    {
    public:
        Crc32Modifier(size_t begin,
                      size_t end,
                      const std::string& expression,
                      size_t linenum,
                      const std::filesystem::path& filepath,
                      const nemesis::SemanticManager& manager);
    };
}
