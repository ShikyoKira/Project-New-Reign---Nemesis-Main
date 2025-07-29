#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/AnimationFilenameStatement.h"

namespace nemesis
{
    struct AnimationFilenameModifier
        : public nemesis::StandardLineModifier<nemesis::AnimationFilenameStatement>
    {
    public:
        AnimationFilenameModifier(size_t begin,
                                  size_t end,
                                  const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager);
    };
}
