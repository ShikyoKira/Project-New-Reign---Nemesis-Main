#include "Core/LineModifier/MinModifier.h"

nemesis::MinModifier::MinModifier(size_t begin,
                                  size_t end,
                                  const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::MinStatement>(begin, end, expression, linenum, filepath, manager)
{
}
