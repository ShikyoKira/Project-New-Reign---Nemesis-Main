#include "Core/LineModifier/MapModifier.h"

nemesis::MapModifier::MapModifier(size_t begin,
                                  size_t end,
                                  const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::MapStatement>(
        begin, end, expression, linenum, filepath, manager)
{
}
