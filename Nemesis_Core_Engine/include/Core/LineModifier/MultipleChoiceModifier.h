#pragma once

#include "Core/LineModifier/StandardLineModifier.h"

#include "Core/Statement/MultipleChoiceStatement.h"

namespace nemesis
{
    struct MultipleChoiceModifier : public nemesis::StandardLineModifier<nemesis::MultipleChoiceStatement>
    {
    public:
        MultipleChoiceModifier(size_t begin,
                               size_t end,
                               const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);
    };
}
