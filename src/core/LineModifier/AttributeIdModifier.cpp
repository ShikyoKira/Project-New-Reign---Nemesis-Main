#include "core/LineModifier/AttributeIdModifier.h"

nemesis::AttributeIdModifier::AttributeIdModifier(size_t begin,
                                                  size_t end,
                                                  const std::string& expression,
                                                  size_t linenum,
                                                  const std::filesystem::path& filepath,
                                                  const nemesis::SemanticManager& manager)
    : nemesis::BaseIdModifier<nemesis::AttributeIdStatement>(begin, end, expression, linenum, filepath, manager)
{
}
