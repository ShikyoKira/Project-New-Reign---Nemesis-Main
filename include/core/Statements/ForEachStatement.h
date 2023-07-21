#pragma once

#include <functional>

#include "core/Statements/Statement.h"

#include "core/AnimationRequest.h"

namespace nemesis
{
    struct CompileState;
    struct SemanticManager;

	struct ForEachStatement : public nemesis::Statement
    {
    private:
        std::string* OptionName = nullptr;
        int LoopCycle = 0;
        std::function<void(nemesis::CompileState&, std::function<void()>)> ForEachFunction;

        void ParseComponents(nemesis::SemanticManager& manager);

    public:
        ForEachStatement(const std::string& expression,
                         size_t linenum,
                         const std::filesystem::path& filepath,
                         nemesis::SemanticManager& manager);
        ForEachStatement(const nemesis::Line& line, nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        void ForEach(nemesis::CompileState& state, std::function<void()> action) const;
        bool TryGetOptionName(std::string& option_name) const noexcept;
    };
}
