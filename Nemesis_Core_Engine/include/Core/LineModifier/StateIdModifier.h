#pragma once

#include "Core/LineModifier/BaseIdModifier.h"

#include "Core/Statement/StateIdStatement.h"

namespace nemesis
{
    struct StateIdModifier : public nemesis::BaseIdModifier<nemesis::StateIdStatement>
    {
    public:
        StateIdModifier(size_t begin,
                        size_t end,
                        const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);
    };
}
