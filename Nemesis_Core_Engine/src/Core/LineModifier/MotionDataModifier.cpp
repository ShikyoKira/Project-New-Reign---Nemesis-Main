#include "Core/LineModifier/MotionDataModifier.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

nemesis::MotionDataModifier::MotionDataModifier(size_t begin,
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
        statement = std::make_shared<nemesis::MotionDataStatement>(expression, linenum, filepath, manager);
        manager.AddStatementToCache(expression, statement);
    }

    Statement = std::dynamic_pointer_cast<nemesis::MotionDataStatement>(statement);
}

void nemesis::MotionDataModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    auto* val_ptr = state.TryGetCachedStatementValue(Statement.get());

    if (val_ptr)
    {
        blocks[Begin] = *val_ptr;
        return;
    }

    state.CacheStatementValue(Statement.get(), blocks[Begin] = Statement->GetValue(state));
}

const nemesis::MotionDataStatement& nemesis::MotionDataModifier::GetStatement() const noexcept
{
    return *Statement;
}
