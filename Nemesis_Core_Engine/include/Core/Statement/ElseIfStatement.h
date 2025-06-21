#pragma once

#include "Core/Statement/ConditionalStatement.h"

#include "Utilities/NonCopyable.h"

namespace nemesis
{
    struct ElseIfStatement : public nemesis::ConditionalStatement, public nemesis::NonCopyableStruct
    {
    public:
        ElseIfStatement(const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);
        ElseIfStatement(const nemesis::Line& line, const nemesis::SemanticManager& manager);
        ElseIfStatement(const nemesis::ElseIfStatement& statement);

        std::string Serialize() const override;

        bool IsTrue(nemesis::CompileState& state) const override;
    };
}
