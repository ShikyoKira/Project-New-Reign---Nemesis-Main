#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/BaseIdStatement.h"

namespace nemesis
{
    template<typename BaseIdStatementType>
	struct BaseIdModifier : public nemesis::LineModifier
    {
    private:
        BaseIdStatementType Statement;

    public:
        BaseIdModifier(size_t begin,
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

        const BaseIdStatementType& GetStatement() const noexcept
        {
            return &Statement;
        }

        static_assert(std::is_base_of_v<BaseIdStatement, BaseIdStatementType>,
                      "Non-BaseIdStatement is passed to BaseIdStatementType");
    };

}
