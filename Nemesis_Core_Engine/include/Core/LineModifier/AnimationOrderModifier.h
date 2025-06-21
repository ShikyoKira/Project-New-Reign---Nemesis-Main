#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/AnimationOrderStatement.h"

namespace nemesis
{
    struct AnimationOrderModifier : public nemesis::StandardLineModifier<nemesis::AnimationOrderStatement>
    {
    public:
        AnimationOrderModifier(size_t begin,
                               size_t end,
                               const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);
    };
}
