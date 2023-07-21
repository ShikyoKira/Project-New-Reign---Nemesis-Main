#pragma once

#include "core/LineModifier/LineModifier.h"

namespace nemesis
{
	struct NumelementModifier : public nemesis::LineModifier
    {
    private:
        size_t LineNum;
        std::filesystem::path FilePath;

    public:
        NumelementModifier(size_t begin, size_t end, size_t linenum, const std::filesystem::path& filepath);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;
    };
}
