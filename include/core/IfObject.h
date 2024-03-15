#pragma once

#include "utilities/regex.h"

#include "core/NObject.h"

#include "core/Statements/ElseIfStatement.h"

namespace nemesis
{
    struct IfObject : nemesis::NObject
    {
    private:
        struct ElseIfObject
        {
            nemesis::ElseIfStatement Statement;
            UPtr<nemesis::NObject> Value;

            ElseIfObject(const std::string& expression,
                         size_t linenum,
                         const std::filesystem::path& filepath,
                         const nemesis::SemanticManager& manager,
                         UPtr<nemesis::NObject>&& value);
            ElseIfObject(const nemesis::IfObject::ElseIfObject& elif_obj);
        };

        nemesis::IfStatement Statement;
        UPtr<nemesis::NObject> Value;

        Vec<ElseIfObject> ElseIfCollection;
        UPtr<nemesis::NObject> ElseValue;

        static nemesis::regex if_rgx;
        static nemesis::regex elseif_rgx;

        IfObject(const nemesis::IfObject& if_obj);

    public:
        IfObject(const std::string& expression,
                 size_t linenum,
                 const std::filesystem::path& filepath,
                 const nemesis::SemanticManager& manager,
                 UPtr<nemesis::NObject>&& value);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::IfObject> Clone() const;

        const nemesis::IfStatement& GetStatement() const noexcept;

        void ElseIf(const std::string& expression,
                    size_t linenum,
                    const std::filesystem::path& filepath,
                    const nemesis::SemanticManager& manager,
                    UPtr<nemesis::NObject>&& value);
        void Else(UPtr<nemesis::NObject>&& value) noexcept;

        static bool MatchIf(const std::string& line, std::string& condition);
        static bool MatchElseIf(const std::string& line, std::string& condition);
        static bool MatchEndIf(const std::string& line) noexcept;
    };
}
