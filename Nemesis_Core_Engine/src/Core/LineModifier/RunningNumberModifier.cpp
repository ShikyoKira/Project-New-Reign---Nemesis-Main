#include "Core/LineModifier/RunningNumberModifier.h"

nemesis::RunningNumberModifier::RunningNumberModifier(size_t begin,
                                                      size_t end,
                                                      const std::string& expression,
                                                      size_t linenum,
                                                      const std::filesystem::path& filepath,
                                                      const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::RunningNumberStatement>(
        begin, end, expression, linenum, filepath, manager)
{
}
