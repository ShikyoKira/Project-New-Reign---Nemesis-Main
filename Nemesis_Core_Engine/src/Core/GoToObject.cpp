#include <stdexcept>

#include "Core/GoToObject.h"

#include "Utilities/ConditionSyntax.h"
#include "Utilities/Algorithm.h"

namespace ns = nemesis::syntax;

nemesis::GoToObject::GoToObject(const nemesis::GoToObject& goto_obj) noexcept
{
    ToLineNum = goto_obj.ToLineNum;
}

nemesis::GoToObject::GoToObject(const nemesis::Line& line, nemesis::SemanticManager& manager)
{
    if (is_only_number(line))
    {
        ToLineNum = std::stoul(line);
        return;
    }

    throw nemesis::NObjectException(
        "Syntax Error: GOTO value must be a numeric value equal or larger than 0 (Expression: " + line.ToString()
        + ", Line: " + std::to_string(line.GetLineNumber())
        + ", File: " + nemesis::to_utf8_string(line.GetFilePath()) + ")");

}

nemesis::GoToObject::GoToObject(const std::string& expression,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                const nemesis::SemanticManager& manager)
{
    if (is_only_number(expression))
    {
        ToLineNum = std::stoul(expression);
        return;
    }

    throw nemesis::NObjectException(
        "Syntax Error: GOTO value must be a numeric value equal or larger than 0 (Expression: " + expression
        + ", Line: " + std::to_string(linenum) + ", File: " + nemesis::to_utf8_string(filepath) + ")");
}

void nemesis::GoToObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
}

void nemesis::GoToObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(ns::GoTo(std::to_string(ToLineNum)));
}

UPtr<nemesis::NObject> nemesis::GoToObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::GoToObject> nemesis::GoToObject::Clone() const
{
    return UPtr<nemesis::GoToObject>(new nemesis::GoToObject(*this));
}

size_t nemesis::GoToObject::GoToLineNum() const noexcept
{
    return ToLineNum;
}
