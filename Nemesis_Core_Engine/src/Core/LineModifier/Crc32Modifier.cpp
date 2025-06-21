#include "Core/LineModifier/Crc32Modifier.h"

nemesis::Crc32Modifier::Crc32Modifier(size_t begin,
                                      size_t end,
                                      const std::string& expression,
                                      size_t linenum,
                                      const std::filesystem::path& filepath,
                                      const nemesis::SemanticManager& manager)
    : nemesis::StandardLineModifier<nemesis::Crc32Statement>(begin, end, expression, linenum, filepath, manager)
{
}
