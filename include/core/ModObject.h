#pragma once

#include "core/NObject.h"

#include "core/Statements/ModCodeStatement.h"

namespace nemesis
{
    struct ModObject : public nemesis::NObject
    {
    private:
        nemesis::ModCodeStatement Statement;
        UPtr<nemesis::NObject> Value;

    public:
        ModObject(const std::string& modcode,
                  size_t linenum,
                  const std::filesystem::path& filepath,
                  nemesis::SemanticManager& manager,
                  UPtr<nemesis::NObject>&& value);

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        bool IsSelected(nemesis::CompileState& state) const;
        std::string_view GetModCode() const noexcept;
    };
}
