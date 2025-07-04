#include "Core/LineModifier/MultipleChoiceModifier.h"

nemesis::MultipleChoiceModifier::MultipleChoiceModifier(size_t begin,
                                                        size_t end,
                                                        const std::string& expression,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::MultipleChoiceStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
