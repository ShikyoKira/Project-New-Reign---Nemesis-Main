#pragma once

#include "core/NObject.h"

namespace nemesis
{
	struct EmptyObject : public nemesis::NObject
    {
    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::EmptyObject> Clone() const;

        static bool Match(const std::string& line) noexcept;
    };
}
