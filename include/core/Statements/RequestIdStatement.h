#pragma once

#include "core/SemanticManager.h"

#include "core/Statements/Statement.h"

namespace nemesis
{
	struct RequestIdStatement : public nemesis::Statement
    {
    private:
        std::function<std::string(nemesis::CompileState&)> GetValueFunction;

    public:
        RequestIdStatement(const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath,
                           const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
