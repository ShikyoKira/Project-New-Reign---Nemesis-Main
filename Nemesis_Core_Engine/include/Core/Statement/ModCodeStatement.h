#pragma once

#include "Core/Statement/Statement.h"

namespace nemesis
{
    struct CompileState;

	struct ModCodeStatement : public nemesis::Statement
    {
    public:
        ModCodeStatement(const std::string& modcode,
                         size_t linenum,
                         const std::filesystem::path& filepath) noexcept;
        ModCodeStatement(const std::string& modcode,
                         size_t linenum,
                         const std::filesystem::path& filepath,
                         nemesis::SemanticManager& manager) noexcept;
        ModCodeStatement(const nemesis::Line& mod_code, nemesis::SemanticManager& manager) noexcept;
        ModCodeStatement(const nemesis::ModCodeStatement& statement);

        std::string Serialize() const override;

        bool IsTrue(nemesis::CompileState& state) const;
    };
}
