#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/AnimationFilePathStatement.h"

namespace nemesis
{
    struct AnimationFilePathModifier
        : public nemesis::StandardLineModifier<nemesis::AnimationFilePathStatement>
    {
    public:
        AnimationFilePathModifier(size_t begin,
                                  size_t end,
                                  const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager);
    };
}
