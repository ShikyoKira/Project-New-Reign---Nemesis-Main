#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/AnimationBaseFilenameStatement.h"

namespace nemesis
{
    struct AnimationBaseFilenameModifier
        : public nemesis::StandardLineModifier<nemesis::AnimationBaseFilenameStatement>
    {
    public:
        AnimationBaseFilenameModifier(size_t begin,
                                      size_t end,
                                      const std::string& expression,
                                      size_t linenum,
                                      const std::filesystem::path& filepath,
                                      const nemesis::SemanticManager& manager);
    };
}
