#pragma once

#include "core/Statements/BaseIdStatement.h"

namespace nemesis
{
    struct PropertyIdStatement : public nemesis::BaseIdStatement
    {
    protected:
        std::string GetTypeName() const override;

    public:
        PropertyIdStatement(const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager);

        std::string GetValue(nemesis::CompileState& state) const override;
    };
}
