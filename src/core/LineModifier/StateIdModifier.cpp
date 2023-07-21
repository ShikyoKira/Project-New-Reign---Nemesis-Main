#include "core/LineModifier/StateIdModifier.h"

nemesis::StateIdModifier::StateIdModifier(size_t begin,
                                      size_t end,
                                      const std::string& expression,
                                      size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::BaseIdModifier<nemesis::StateIdStatement>(begin, end, expression, linenum, filepath, manager)
{
}
