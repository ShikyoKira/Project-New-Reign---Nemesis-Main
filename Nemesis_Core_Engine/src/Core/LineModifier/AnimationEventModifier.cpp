#include "Core/LineModifier/AnimationEventModifier.h"

#include "Core/CompileState.h"

nemesis::AnimationEventModifier::AnimationEventModifier(size_t begin,
                                                        size_t end,
                                                        const std::string& expression,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::AnimationEventStatement>(
          begin, end, expression, linenum, filepath, manager)
{
}
