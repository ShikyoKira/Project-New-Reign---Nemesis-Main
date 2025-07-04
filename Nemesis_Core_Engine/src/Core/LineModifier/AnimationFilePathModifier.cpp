#include "Core/LineModifier/AnimationFilePathModifier.h"

nemesis::AnimationFilePathModifier::AnimationFilePathModifier(size_t begin,
                                                              size_t end,
                                                              const std::string& expression,
                                                              size_t linenum,
                                                              const std::filesystem::path& filepath,
                                                              const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::AnimationFilePathStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
