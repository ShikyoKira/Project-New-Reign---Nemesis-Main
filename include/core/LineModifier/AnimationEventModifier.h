#pragma once

#include "core/SemanticManager.h"

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/AnimationEventStatement.h"

namespace nemesis
{
	struct AnimationEventModifier : public nemesis::LineModifier
    {
    private:
        nemesis::AnimationEventStatement Statement;

    public:
        AnimationEventModifier(size_t begin,
                               size_t end,
                               const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::AnimationEventStatement* GetStatement() const noexcept;
    };
}
