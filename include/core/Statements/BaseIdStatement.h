#pragma once

#include <functional>

#include "core/Statements/CompositeStatement.h"

namespace nemesis
{
    struct LineModifier;
    struct SemanticManager;

    struct BaseIdStatement : public nemesis::CompositeStatement
    {
    protected:
        std::string GetComponentValue(nemesis::CompileState& state) const;

        virtual std::string GetTypeName() const;

    public:
        BaseIdStatement(const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        virtual std::string GetValue(nemesis::CompileState& state) const = 0;
    };
}
