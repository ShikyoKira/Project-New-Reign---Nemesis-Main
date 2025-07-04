#include "Core/LineModifier/RequestIndexModifier.h"

#include "Core/SemanticManager.h"

nemesis::RequestIndexModifier::RequestIndexModifier(size_t begin,
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
        statement = std::make_shared<nemesis::RequestIndexStatement>(expression, linenum, filepath, manager);
        manager.AddStatementToCache(expression, statement);
    }

    Statement = std::dynamic_pointer_cast<nemesis::RequestIndexStatement>(statement);
}

void nemesis::RequestIndexModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = Statement->GetValue(state);
}

const nemesis::RequestIndexStatement& nemesis::RequestIndexModifier::GetStatement() const noexcept
{
    return *Statement;
}
