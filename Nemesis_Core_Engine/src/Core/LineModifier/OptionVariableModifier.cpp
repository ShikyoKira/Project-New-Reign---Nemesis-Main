#include "Core/LineModifier/OptionVariableModifier.h"

nemesis::OptionVariableModifier::OptionVariableModifier(size_t begin,
                                                        size_t end,
                                                        const std::string& expression,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::OptionVariableStatement>(
        begin, end, expression, linenum, filepath, manager)
{
}
