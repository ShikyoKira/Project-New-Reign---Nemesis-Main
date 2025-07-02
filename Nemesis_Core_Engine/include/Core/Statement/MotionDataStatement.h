#pragma once

#include "Core/Statement/CompositeStatement.h"

namespace nemesis
{
    struct MotionDataStatement : public nemesis::CompositeStatement
    {
    private:
        mutable std::move_only_function<std::string(nemesis::CompileState&)> GetValueFunction;

        bool TryParse2Components(const nemesis::SemanticManager& manager);
        bool TryParse4Components(const nemesis::SemanticManager& manager);
        bool TryParseExtraComponents(const nemesis::SemanticManager& manager);

    public:
        MotionDataStatement(const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager);

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
