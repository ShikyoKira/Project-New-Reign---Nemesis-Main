#pragma once

#include "core/LineModifier/LineModifier.h"

#include "utilities/regex.h"

namespace nemesis
{
	struct CounterModifier : public nemesis::LineModifier
    {
    private:
        std::string CounterName, AddCounterSyntax, EndCounterSyntax;

        nemesis::regex StartCounterRegex, AddCounterRegex, EndCounterRegex;

        size_t LineNum;
        std::filesystem::path FilePath;

        static std::string GenerateGuid();

    public:
        CounterModifier(size_t begin,
                        size_t end,
                        const std::string& name,
                        size_t linenum,
                        const std::filesystem::path& filepath) noexcept;

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;
    };
}
