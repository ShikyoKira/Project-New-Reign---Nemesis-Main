#pragma once

#include "core/NObject.h"

#include "core/LineModifier/LineModifier.h"

namespace nemesis
{
    struct ModLine;

	struct NLine : public nemesis::NObject
    {
    protected:
        UPtr<nemesis::Line> Value;
        Map<std::string, UPtr<nemesis::ModLine>> ModLines;
        Map<size_t, Vec<SPtr<nemesis::LineModifier>>> Modifiers;

        NLine(const nemesis::NLine& nline);

    public:
        NLine() = default;
        NLine(const std::string& expression,
              size_t linenum,
              const std::filesystem::path& filepath,
              const nemesis::SemanticManager& manager);
        NLine(const nemesis::Line& line, const nemesis::SemanticManager& manager);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::NLine> Clone() const;

        void MatchAndUpdate(const nemesis::NLine& nline);

        virtual void AddModLine(const std::string& modcode,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                nemesis::SemanticManager& manager);
        virtual void AddModLine(const std::string& modcode,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                nemesis::SemanticManager& manager,
                                const nemesis::Line& line);
    };
}
