#pragma once

#include "Core/Statement/CompositeStatement.h"

namespace nemesis
{
	struct SubstringStatement : public nemesis::CompositeStatement
    {
    private:
        mutable std::move_only_function<std::string(nemesis::CompileState&)> GetValueFunction;

        void Parse3Components(const nemesis::SemanticManager& manager);
        void Parse4Components(const nemesis::SemanticManager& manager);

    public:
        SubstringStatement(const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath,
                           const nemesis::SemanticManager& manager);

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
