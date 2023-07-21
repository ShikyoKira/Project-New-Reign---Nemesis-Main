#pragma once

#include "core/SemanticManager.h"

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/RequestIdStatement.h"

namespace nemesis
{
	struct RequestIdModifier : public nemesis::LineModifier
    {
    private:
        RequestIdStatement Statement;

    public:
        RequestIdModifier(size_t begin,
                          size_t end,
                          const std::string& expression,
                          size_t linenum,
                          const std::filesystem::path& filepath,
                          const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::RequestIdStatement* GetStatement() const noexcept;
    };
}
