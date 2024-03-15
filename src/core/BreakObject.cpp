#include "core/BreakObject.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"

nemesis::BreakObject::BreakException::BreakException(const std::string& expression,
                                                     size_t linenum,
                                                     const std::filesystem::path& filepath)
    : std::exception(std::string("Uncaptured ForEach Break (Expression: " + expression + ", Line: "
                                 + std::to_string(linenum) + ", File: " + filepath.string() + ")")
                         .c_str())
    , Expression(expression)
{
}

const std::string& nemesis::BreakObject::BreakException::GetExpression() const noexcept
{
    return Expression;
}

nemesis::BreakObject::BreakObject(const nemesis::Line& line, nemesis::SemanticManager& manager)
    : nemesis::BreakObject::BreakObject(line, line.GetLineNumber(), line.GetFilePath(), manager)
{
}

nemesis::BreakObject::BreakObject(const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  nemesis::SemanticManager& manager)
    : Expression(expression)
    , LineNum(linenum)
    , FilePath(filepath)
{
    if (manager.HasForEachInQueue(expression)) return;

    throw std::runtime_error(
        "Syntax error: Break must be within stated scope of FOREACH and CLOSE (Expression: " + expression
        + ", Line: " + std::to_string(linenum) + ", File: " + filepath.string() + ")");
}

void nemesis::BreakObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const 
{
    throw BreakException(Expression, LineNum, FilePath);
}

void nemesis::BreakObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back("", lines.size() + 1, FilePath);
}

UPtr<nemesis::NObject> nemesis::BreakObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::BreakObject> nemesis::BreakObject::Clone() const
{
    auto bo_ptr        = UPtr<nemesis::BreakObject>(new nemesis::BreakObject());
    bo_ptr->Expression = Expression;
    bo_ptr->LineNum    = LineNum;
    bo_ptr->FilePath   = FilePath;
    return bo_ptr;
}
