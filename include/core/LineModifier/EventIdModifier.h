#pragma once

#include "core/LineModifier/BaseIdModifier.h"

#include "core/Statements/EventIdStatement.h"

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
