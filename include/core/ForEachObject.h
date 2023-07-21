#pragma once

#include "utilities/regex.h"

#include "core/NObject.h"

#include "core/Statements/ForEachStatement.h"

namespace nemesis
{
    struct ForEachObject : public nemesis::NObject
    {
    private:
        nemesis::ForEachStatement Statement;
        UPtr<nemesis::NObject> Value;

    public:
        ForEachObject(const std::string& expression,
                      size_t linenum,
                      const std::filesystem::path& filepath,
                      nemesis::SemanticManager& manager,
                      UPtr<nemesis::NObject>&& value);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        const nemesis::ForEachStatement& GetStatement() const noexcept;

        static bool MatchForEach(const std::string& line, std::string& condition);
        static bool MatchClose(const std::string& line) noexcept;
    };
}
