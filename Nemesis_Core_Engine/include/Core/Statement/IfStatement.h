#pragma once

#include "Core/Statement/ConditionalStatement.h"

#include "Utilities/NonCopyable.h"

namespace nemesis
{
    struct CompileState;
    struct Template;

	struct IfStatement : public nemesis::ConditionalStatement, public nemesis::NonCopyableStruct
    {
    public:
        IfStatement(const std::string& expression,
                    size_t linenum,
                    const std::filesystem::path& filepath,
                    const nemesis::SemanticManager& manager);
        IfStatement(const nemesis::Line& line, const nemesis::SemanticManager& manager);
        IfStatement(const nemesis::IfStatement& statement);

        std::string Serialize() const override;

        bool IsTrue(nemesis::CompileState& state) const override;
    };
}
