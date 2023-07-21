#include "core/LineModifier/PropertyIdModifier.h"

nemesis::PropertyIdModifier::PropertyIdModifier(size_t begin,
                                                size_t end,
                                                const std::string& expression,
                                                size_t linenum,
                                                const std::filesystem::path& filepath,
                                                const nemesis::SemanticManager& manager)
    : nemesis::BaseIdModifier<nemesis::PropertyIdStatement>(begin, end, expression, linenum, filepath, manager)
{
}
