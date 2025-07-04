#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/RequestIndexStatement.h"

namespace nemesis
{
	struct RequestIndexModifier : public nemesis::LineModifier
    {
    private:
        SPtr<nemesis::RequestIndexStatement> Statement;

    public:
        RequestIndexModifier(size_t begin,
                             size_t end,
                             const std::string& expression,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::RequestIndexStatement& GetStatement() const noexcept;
    };
}
