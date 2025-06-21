#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/RunningNumberStatement.h"

namespace nemesis
{
    struct RunningNumberModifier : public nemesis::StandardLineModifier<nemesis::RunningNumberStatement>
    {
    public:
        RunningNumberModifier(size_t begin,
                              size_t end,
                              const std::string& expression,
                              size_t linenum,
                              const std::filesystem::path& filepath,
                              const nemesis::SemanticManager& manager);
    };
}
