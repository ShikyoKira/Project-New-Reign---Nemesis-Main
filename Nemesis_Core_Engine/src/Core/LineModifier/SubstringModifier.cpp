#include "Core/LineModifier/SubstringModifier.h"

nemesis::SubstringModifier::SubstringModifier(size_t begin,
                                              size_t end,
                                              const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::SubstringStatement>(
        begin, end, expression, linenum, filepath, manager)
{
}
