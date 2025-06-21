#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/RotationDataStatement.h"

namespace nemesis
{
    struct SemanticManager;

    struct RotationDataModifier : public nemesis::StandardLineModifier<nemesis::RotationDataStatement>
    {
    public:
        RotationDataModifier(size_t begin,
                             size_t end,
                             const std::string& expression,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);
    };
}
