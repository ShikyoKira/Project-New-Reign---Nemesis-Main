#pragma once

#include "core/NLine.h"

#include "core/Statements/ModCodeStatement.h"

namespace nemesis
{
    struct ModLine : public nemesis::NLine
    {
    private:
        nemesis::ModCodeStatement Statement;

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
        std::string_view GetModCode() const;

        static bool MatchOpen(const std::string& line, std::string& condition);
        static bool MatchOriginal(const std::string& line);
        static bool MatchClose(const std::string& line);
    };
}
