#include "core/LineModifier/CurrentCountModifier.h"

#include "core/CompileState.h"

#include "core/Statements/Statement.h"

nemesis::CurrentCountModifier::CurrentCountModifier(size_t begin,
                                                    size_t end,
                                                    const std::string& expression,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
{
    auto components = nemesis::Statement::SplitComponents(expression);

    if (components.size() != 2)
    {
        throw std::runtime_error(
            "Syntax error: CurrentCount only accepts 1 argument (Expression: " + expression + ", Line: " + std::to_string(linenum) + ", File: " + filepath.string() + ")");
    }

     Name = components.back();
}

void nemesis::CurrentCountModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const 
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = std::to_string(*state.GetCounter(Name));
}

const std::string& nemesis::CurrentCountModifier::GetName() const
{
    return Name;
}
