#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/Statement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

namespace nemesis
{
    template <typename StatementType>
    struct StandardLineModifier : public nemesis::LineModifier
    {
        static_assert(std::is_base_of_v<nemesis::Statement, StatementType>,
                      "StatementType must be derived from nemesis::Statement");

    protected:
        SPtr<StatementType> Statement;

    public:
        StandardLineModifier(size_t begin,
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
                statement = std::make_shared<StatementType>(expression, linenum, filepath, manager);
                manager.AddStatementToCache(expression, statement);
            }

            Statement = std::dynamic_pointer_cast<StatementType>(statement);
        }

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override
        {
            ClearCoveredBlocks(blocks);
            blocks[Begin] = Statement->GetValue(state);
        }

        const StatementType& GetStatement() const noexcept
        {
            return *Statement;
        }

        static_assert(std::is_base_of_v<nemesis::Statement, StatementType>,
                    "Non-Statement type is passed to StatementType");
    };
}