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
        throw std::runtime_error("Syntax Error: Crc32 only accepts 1 argument (Syntax: " + expression
                                 + ", Line: " + std::to_string(linenum)
                                 + ", File: " + nemesis::to_utf8_string(filepath) + ")");
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
