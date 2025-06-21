#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/MotionDataStatement.h"

namespace nemesis
{
    struct SemanticManager;

	struct MotionDataModifier : public nemesis::LineModifier
    {
    private:
        nemesis::MotionDataStatement Statement;

    public:
        MotionDataModifier(size_t begin,
                           size_t end,
                           const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath,
                           const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::MotionDataStatement& GetStatement() const noexcept;
    };
}
