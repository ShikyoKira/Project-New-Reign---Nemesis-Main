#pragma once

#include "core/LineModifier/LineModifier.h"

namespace nemesis
{
	struct CurrentCountModifier : public nemesis::LineModifier
    {
    private:
        std::string Name;

    public:
        CurrentCountModifier(size_t begin,
                             size_t end,
                             const std::string& expression,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const std::string& GetName() const;
    };
}
