#pragma once

#include "core/NObject.h"

namespace nemesis
{
    struct SemanticManager;

	struct BreakObject : public nemesis::NObject
    {
    public:
        struct BreakException : public std::exception
        {
        private:
            std::string Expression;

        public:
            BreakException(const std::string& expression, size_t linenum, const std::filesystem::path& filepath);

            const std::string& GetExpression() const noexcept;
        };

    private:
        std::string Expression;
        size_t LineNum;
        std::filesystem::path FilePath;

        BreakObject() = default;

    public:
        BreakObject(const nemesis::Line& line,
                    nemesis::SemanticManager& manager);
        BreakObject(const std::string& expression,
                    size_t linenum,
                    const std::filesystem::path& filepath,
                    nemesis::SemanticManager& manager);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::BreakObject> Clone() const;
    };
}
