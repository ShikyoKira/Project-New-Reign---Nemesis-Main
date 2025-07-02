#include "Core/LineModifier/MaxModifier.h"

nemesis::MaxModifier::MaxModifier(size_t begin,
                                  size_t end,
                                  const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::MaxStatement>(begin, end, expression, linenum, filepath, manager)
{
}
