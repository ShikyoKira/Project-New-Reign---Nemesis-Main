#pragma once

#include "core/Statements/BaseIdStatement.h"

namespace nemesis
{
    struct EventIdStatement : public nemesis::BaseIdStatement
    {
    protected:
        std::string GetTypeName() const override;

    public:
        EventIdStatement(const std::string& expression,
                         size_t linenum,
                         const std::filesystem::path& filepath,
                         const nemesis::SemanticManager& manager);

        std::string GetValue(nemesis::CompileState& state) const override;
    };
}
