#pragma once

#include "core/LineModifier/BaseIdModifier.h"

#include "core/Statements/AttributeIdStatement.h"

namespace nemesis
{
    struct SemanticManager;

    struct AttributeIdModifier : public nemesis::BaseIdModifier<nemesis::AttributeIdStatement>
    {
    public:
        AttributeIdModifier(size_t begin,
                            size_t end,
                            const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager);
    };
}
