#pragma once

#include "core/NObject.h"

namespace nemesis
{
	struct EmptyObject : public nemesis::NObject
    {
    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        static bool Match(const std::string& line) noexcept;
    };
}
