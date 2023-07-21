#include "core/LineModifier/SubTemplateModifier.h"

#include "core/Statements/SubTemplateStatement.h"

#include "core/CompileState.h"

nemesis::SubTemplateModifier::SubTemplateModifier(size_t begin,
                                                  size_t end,
                                                  const std::string& expression,
                                                  size_t linenum,
                                                  const std::filesystem::path& filepath,
                                                  const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
    , Statement(expression, linenum, filepath, manager)
{
}

void nemesis::SubTemplateModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    auto components = Statement.GetComponents(state);

    if (!isOnlyNumber(components[1]))
    {
        state.AddSubTemplateRequest(components);
        blocks[Begin] = Statement.GetExpression();
        return;
    }

    blocks[Begin] = state.GetCurrentSubTemplateRequest()->GetArgument(std::stoul(components[1]));
}

const nemesis::SubTemplateStatement* nemesis::SubTemplateModifier::GetStatement() const noexcept
{
    return &Statement;
}
