#pragma once

#include "Core/Statement/CompositeStatement.h"

namespace nemesis
{
	struct LengthStatement : public nemesis::CompositeStatement
    {
    public:
        LengthStatement(const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
