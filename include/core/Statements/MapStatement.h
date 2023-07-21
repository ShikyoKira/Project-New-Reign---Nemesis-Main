#pragma once

#include <functional>

#include "core/Statements/Statement.h"

namespace nemesis
{
    struct CompileState;
    struct SemanticManager;

	struct MapStatement : public nemesis::Statement
    {
    private:
        std::function<std::string(nemesis::CompileState&)> GetValueFunction;

        bool SetupGetValueFunction(
            SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>> get_request,
            const std::string& key,
            const std::string& index_str,
            const nemesis::SemanticManager& manager);

    public:
        MapStatement(const std::string& expression,
                     size_t linenum,
                     const std::filesystem::path& filepath,
                     const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(nemesis::CompileState& state) const;
    };
}
