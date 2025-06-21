#pragma once

#include "Core/LineModifier/BaseIdModifier.h"

#include "Core/Statement/EventIdStatement.h"

namespace nemesis
{
    struct SemanticManager;

    struct EventIdModifier : public nemesis::BaseIdModifier<nemesis::EventIdStatement>
    {
    public:
        EventIdModifier(size_t begin,
                        size_t end,
                        const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);
    };
}
