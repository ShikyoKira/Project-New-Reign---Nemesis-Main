#include "Core/LineModifier/RequestIdModifier.h"

#include "Core/CompileState.h"

nemesis::RequestIdModifier::RequestIdModifier(size_t begin,
                                              size_t end,
                                              const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::RequestIdStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
