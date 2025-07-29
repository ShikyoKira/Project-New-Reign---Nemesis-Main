#include "Core/LineModifier/ListNameModifier.h"

#include "Core/CompileState.h"

nemesis::ListNameModifier::ListNameModifier(size_t begin,
                                              size_t end,
                                              const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::ListNameStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
