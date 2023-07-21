#include "core/LineModifier/VariableIdModifier.h"

nemesis::VariableIdModifier::VariableIdModifier(size_t begin,
                                                size_t end,
                                                const std::string& expression,
                                                size_t linenum,
                                                const std::filesystem::path& filepath,
                                                const nemesis::SemanticManager& manager)
    : nemesis::BaseIdModifier<nemesis::VariableIdStatement>(begin, end, expression, linenum, filepath, manager)
{
}
