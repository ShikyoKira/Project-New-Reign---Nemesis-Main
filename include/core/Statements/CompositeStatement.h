#pragma once

#include "core/Statements/Statement.h"

namespace nemesis
{
    struct LineModifier;

	struct CompositeStatement : public nemesis::Statement
    {
    protected:
        struct DynamicComponent
        {
        private:
            std::string RawValue;
            VecStr Tokens;
            Vec<SPtr<nemesis::LineModifier>> Modifiers;

        public:
            DynamicComponent(const std::string& component,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);

            std::string GetValue(nemesis::CompileState& state) const;
        };

        Vec<DynamicComponent> DynamicComponents;

        static bool IsComplexComponent(const std::string& component);

    public:
        CompositeStatement() = default;
        CompositeStatement(const std::string& expression,
                           size_t linenum,
                           const std::filesystem::path& filepath);

        std::string Serialize() const override;

        virtual std::string GetValue(nemesis::CompileState& state) const = 0;
    };
}
