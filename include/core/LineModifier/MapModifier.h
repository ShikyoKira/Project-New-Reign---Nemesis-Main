#pragma once

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/MapStatement.h"

namespace nemesis
{
    struct SemanticManager;

	struct MapModifier : public nemesis::LineModifier
    {
    private:
        MapStatement Statement;

    public:
        MapModifier(size_t begin,
                    size_t end,
                    const std::string& expression,
                    size_t linenum,
                    const std::filesystem::path& filepath,
                    const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::MapStatement* GetStatement() const noexcept;
    };
}
