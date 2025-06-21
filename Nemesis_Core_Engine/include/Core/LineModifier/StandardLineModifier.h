#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/Statement.h"

#include "Core/CompileState.h"

namespace nemesis
{
    template <typename StatementType>
    struct StandardLineModifier : public nemesis::LineModifier
    {
    protected:
        StatementType Statement;

    public:
        StandardLineModifier(size_t begin,
                             size_t end,
                             const std::string& expression,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager)
            : nemesis::LineModifier(begin, end)
            , Statement(expression, linenum, filepath, manager)
        {
        }

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override
        {
            ClearCoveredBlocks(blocks);
            blocks[Begin] = Statement.GetValue(state);
        }

        const StatementType& GetStatement() const noexcept
        {
            return Statement;
        }

        static_assert(std::is_base_of_v<nemesis::Statement, StatementType>,
                    "Non-Statement type is passed to StatementType");
    };
}