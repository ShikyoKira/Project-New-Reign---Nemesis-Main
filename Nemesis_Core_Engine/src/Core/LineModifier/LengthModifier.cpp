#include "Core/LineModifier/LengthModifier.h"

nemesis::LengthModifier::LengthModifier(size_t begin,
                                        size_t end,
                                        const std::string& expression,
                                        size_t linenum,
                                        const std::filesystem::path& filepath,
                                        const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::LengthStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
