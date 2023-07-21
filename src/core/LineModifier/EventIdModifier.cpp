#include "core/LineModifier/EventIdModifier.h"

nemesis::EventIdModifier::EventIdModifier(size_t begin,
                                          size_t end,
                                          const std::string& expression,
                                          size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::BaseIdModifier<nemesis::EventIdStatement>(begin, end, expression, linenum, filepath, manager)
{
}
