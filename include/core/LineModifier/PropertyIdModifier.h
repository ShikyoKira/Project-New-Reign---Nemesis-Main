#pragma once

#include "core/LineModifier/BaseIdModifier.h"

#include "core/Statements/PropertyIdStatement.h"

namespace nemesis
{
    struct SemanticManager;

    struct PropertyIdModifier : public nemesis::BaseIdModifier<nemesis::PropertyIdStatement>
    {
    public:
        PropertyIdModifier(size_t begin,
                           size_t end,
                           const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath,
                           const nemesis::SemanticManager& manager);
    };
}
