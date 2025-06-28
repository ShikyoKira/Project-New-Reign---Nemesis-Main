#include "Core/Statement/Crc32Statement.h"

#include "Utilities/Crc32.h"

#include "Utilities/Algorithm.h"

nemesis::Crc32Statement::Crc32Statement(const std::string& expression,
                                        size_t linenum,
                                        const std::filesystem::path& filepath,
                                        const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() != 2)
    {
        ThrowSyntaxError("Crc32 only accepts 1 argument");
    }

    auto& key = Components.back();

    if (!IsComplexComponent(key)) return;

    DynamicComponents.emplace_back(key, linenum, filepath, manager);
}

std::string nemesis::Crc32Statement::GetValue(nemesis::CompileState& state) const
{
    static nemesis::CRC32 crc32;

    if (DynamicComponents.empty()) return std::to_string(crc32.FullCRC(Components.back()));

    return std::to_string(crc32.FullCRC(DynamicComponents.back().GetValue(state)));
}
