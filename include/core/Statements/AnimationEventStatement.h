#pragma once

#include "core/Statements/Statement.h"

namespace nemesis
{
	struct AnimationEventStatement : public nemesis::Statement
    {
    private:
        std::function<std::string(nemesis::CompileState&)> GetValueFunction;

    public:
        AnimationEventStatement(const std::string& expression,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
