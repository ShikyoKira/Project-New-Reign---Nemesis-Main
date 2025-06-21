#pragma once

#include "Core/Statement/CompositeStatement.h"

namespace nemesis
{
    struct AnimationOrderStatement : public nemesis::CompositeStatement
    {
    private:
        std::function<std::string(nemesis::CompileState&)> GetValueFunction;

    public:
        AnimationOrderStatement(const std::string& expression,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                const nemesis::SemanticManager& manager);

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
