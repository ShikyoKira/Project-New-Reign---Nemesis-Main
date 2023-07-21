#pragma once

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/AnimationFilePathStatement.h"

namespace nemesis
{
	struct AnimationFilePathModifier : public nemesis::LineModifier
    {
    private:
        nemesis::AnimationFilePathStatement Statement;

    public:
        AnimationFilePathModifier(size_t begin,
                                  size_t end,
                                  const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::AnimationFilePathStatement* GetStatement() const noexcept;
    };
}
