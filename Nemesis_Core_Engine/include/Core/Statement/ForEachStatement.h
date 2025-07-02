#pragma once

#include <functional>

#include "Core/Statement/CompositeStatement.h"

#include "Core/AnimationRequest.h"

#include "Utilities/NonCopyable.h"

namespace nemesis
{
    struct CompileState;
    struct SemanticManager;

	struct ForEachStatement : public nemesis::CompositeStatement, public nemesis::NonCopyableStruct
    {
        enum ForEachType
        {
            NONE,
            REQUEST,
            OPTION,
            MAP,
            MOTION_DATA,
            ROTATION_DATA
        };

    private:
        ForEachType Type        = nemesis::ForEachStatement::ForEachType::NONE;
        std::string* Key        = nullptr;
        int LoopCycle           = 0;
        mutable std::move_only_function<void(nemesis::CompileState&, std::function<void()>)> ForEachFunction;

        void Parse1Component(nemesis::SemanticManager& manager);
        void Parse2Components(nemesis::SemanticManager& manager);
        void Parse3Components(nemesis::SemanticManager& manager);
        bool Parse4Components(nemesis::SemanticManager& manager);
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
        bool TryGetMapKey(std::string& map_key) const noexcept;
        nemesis::ForEachStatement::ForEachType GetType() const noexcept;
    };
}
