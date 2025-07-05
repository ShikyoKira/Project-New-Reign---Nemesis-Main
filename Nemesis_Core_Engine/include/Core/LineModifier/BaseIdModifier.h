#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/BaseIdStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

namespace nemesis
{
    template<typename BaseIdStatementType>
	struct BaseIdModifier : public nemesis::LineModifier
    {
    private:
        SPtr<BaseIdStatementType> Statement;

    public:
        BaseIdModifier(size_t begin,
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
                statement = std::make_shared<BaseIdStatementType>(expression, linenum, filepath, manager);
                manager.AddStatementToCache(expression, statement);
            }

            Statement = std::dynamic_pointer_cast<BaseIdStatementType>(statement);
        }

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override
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

        const BaseIdStatementType& GetStatement() const noexcept
        {
            return *Statement;
        }

        static_assert(std::is_base_of_v<BaseIdStatement, BaseIdStatementType>,
                      "Non-BaseIdStatement is passed to BaseIdStatementType");
    };

}
