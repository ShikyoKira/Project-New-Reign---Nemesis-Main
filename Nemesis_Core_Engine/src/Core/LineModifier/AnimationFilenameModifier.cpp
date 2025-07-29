#include "Core/LineModifier/AnimationFilenameModifier.h"

nemesis::AnimationFilenameModifier::AnimationFilenameModifier(size_t begin,
                                                              size_t end,
                                                              const std::string& expression,
                                                              size_t linenum,
                                                              const std::filesystem::path& filepath,
                                                              const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::AnimationFilenameStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
