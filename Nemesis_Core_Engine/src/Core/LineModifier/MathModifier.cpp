#include "Core/LineModifier/MathModifier.h"

nemesis::MathModifier::MathModifier(size_t begin,
                                    size_t end,
                                    const std::string& expression,
                                    size_t linenum,
                                    const std::filesystem::path& filepath,
                                    const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::MathStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
