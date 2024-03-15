#pragma once

#include "core/NLine.h"

#include "core/Statements/ModCodeStatement.h"

namespace nemesis
{
    struct ModLine : public nemesis::NLine
    {
    private:
        nemesis::ModCodeStatement Statement;

        ModLine(const nemesis::ModLine& modline);

    public:
        ModLine(const std::string& modcode,
                size_t linenum,
                const std::filesystem::path& filepath,
                nemesis::SemanticManager& manager);
        ModLine(const std::string& modcode,
                size_t linenum,
                const std::filesystem::path& filepath,
                nemesis::SemanticManager& manager,
                const nemesis::Line& line);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::ModLine> Clone() const;

        const nemesis::ModCodeStatement& GetStatement() const noexcept;

        void AddModLine(const std::string& modcode,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        nemesis::SemanticManager& manager) override;
        void AddModLine(const std::string& modcode,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        nemesis::SemanticManager& manager,
                        const nemesis::Line& line) override;

        bool IsSelected(nemesis::CompileState& state) const;
        const std::string& GetModCode() const;

        static bool MatchOpen(const std::string& line, std::string& condition);
        static bool MatchOriginal(const std::string& line);
        static bool MatchClose(const std::string& line);
    };
}
