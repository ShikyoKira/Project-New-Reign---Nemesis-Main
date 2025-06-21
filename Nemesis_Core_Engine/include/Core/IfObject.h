#pragma once

#include "Core/NObject.h"

#include "Core/Statement/IfStatement.h"
#include "Core/Statement/ElseIfStatement.h"

namespace nemesis
{
    struct IfObject : nemesis::NObject
    {
    private:
        struct ElseIfObject
        {
            nemesis::ElseIfStatement Statement;
            UPtr<nemesis::NObject> Value;

        private:
            ElseIfObject(const nemesis::IfObject::ElseIfObject& elseif_obj);

        public:
            ElseIfObject(const std::string& expression,
                         size_t linenum,
                         const std::filesystem::path& filepath,
                         const nemesis::SemanticManager& manager,
                         UPtr<nemesis::NObject>&& value);

            UPtr<nemesis::IfObject::ElseIfObject> Clone() const;
        };

        nemesis::IfStatement Statement;
        UPtr<nemesis::NObject> Value;

        Vec<UPtr<nemesis::IfObject::ElseIfObject>> ElseIfCollection;
        UPtr<nemesis::NObject> ElseValue;

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
    };
}
