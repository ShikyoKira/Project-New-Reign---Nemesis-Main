#include "Core/LineModifier/AnimationOrderModifier.h"

nemesis::AnimationOrderModifier::AnimationOrderModifier(size_t begin,
                                                        size_t end,
                                                        const std::string& expression,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::AnimationOrderStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
