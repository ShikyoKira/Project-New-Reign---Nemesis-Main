#pragma once

#include <filesystem>

#include "utilities/types.h"

namespace nemesis
{
    struct Line;
    struct AnimationRequest;
    struct CompileState;
    struct TemplateClass;
    struct SemanticManager;

	struct Statement
    {
    protected:
        std::string Expression;
        size_t LineNum = 0;
        std::filesystem::path FilePath;
        VecStr Components;

        Statement() = default;
        Statement(const std::string& expression);
        Statement(const std::string& expression, size_t linenum, const std::filesystem::path filepath);
        Statement(const nemesis::Line& expression);

    public:
        const std::string& GetExpression() const noexcept;
        size_t GetLineNum() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;
        SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>
        GetTargetRequest(const nemesis::TemplateClass& template_class,
                         const nemesis::SemanticManager& manager) const;
        size_t GetTemplateNumber(const nemesis::TemplateClass& template_class) const;

        virtual std::string Serialize() const = 0;

        static VecStr SplitComponents(const std::string& value);
    };
}
