#pragma once

#include "Core/NObject.h"

namespace nemesis
{
    struct GoToObject : nemesis::NObject
    {
    private:
        size_t ToLineNum;

        GoToObject(const nemesis::GoToObject& goto_obj) noexcept;

    public:
        GoToObject(const nemesis::Line& line, nemesis::SemanticManager& manager);

        GoToObject(const std::string& expression,
                   size_t linenum,
                   const std::filesystem::path& filepath,
                   const nemesis::SemanticManager& manager);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::GoToObject> Clone() const;

        size_t GoToLineNum() const noexcept;
    };
}
