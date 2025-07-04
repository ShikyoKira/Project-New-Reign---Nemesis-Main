#include "Core/LineModifier/SubTemplateModifier.h"

#include "Core/Statement/SubTemplateStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

nemesis::SubTemplateModifier::SubTemplateModifier(size_t begin,
                                                  size_t end,
                                                  const std::string& expression,
                                                  size_t linenum,
                                                  const std::filesystem::path& filepath,
                                                  const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
{
    auto statement = manager.GetCachedStatement(expression);

    if (!statement)
    {
        statement = std::make_shared<nemesis::SubTemplateStatement>(expression, linenum, filepath, manager);
        manager.AddStatementToCache(expression, statement);
    }

    Statement = std::dynamic_pointer_cast<nemesis::SubTemplateStatement>(statement);
}

void nemesis::SubTemplateModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    auto components = Statement->GetComponents(state);

    if (!is_only_number(components[1]))
    {
        state.AddSubTemplateRequest(components);
        blocks[Begin] = components.front();
        return;
    }

    blocks[Begin] = state.GetCurrentSubTemplateRequest()->GetArgument(std::stoul(components[1]));
}

const nemesis::SubTemplateStatement& nemesis::SubTemplateModifier::GetStatement() const noexcept
{
    return *Statement;
}
