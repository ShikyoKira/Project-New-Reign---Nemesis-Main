#pragma once

#include "core/Statements/IfStatement.h"

namespace nemesis
{
    struct ElseIfStatement : public nemesis::IfStatement
    {
    public:
        ElseIfStatement(const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);
        ElseIfStatement(const nemesis::ElseIfStatement& statement);

        std::string Serialize() const override;
    };
}
