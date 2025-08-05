#pragma once

#include <filesystem>
#include <functional>

#include "Nemesis_Core_Engine/include/Utilities/Line.h"
#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    struct CompileState;
    struct TemplateClass;
    struct SemanticManager;
    struct AnimationRequest;

	struct Statement
    {
    protected:
        std::string Expression;
        size_t LineNum = 0;
        std::filesystem::path FilePath;
        VecStr Components;

        Statement(const std::string& expression, size_t linenum, const std::filesystem::path filepath, bool no_component = false);
        Statement(const nemesis::Line& expression, bool no_component = false);
        Statement(const nemesis::Statement& statement);

        virtual SPtr<std::function<bool(nemesis::CompileState&)>> CallbackTargetRequests(
            const nemesis::TemplateClass& templt_class,
            const nemesis::SemanticManager& manager,
            const std::function<bool(nemesis::CompileState&, const nemesis::AnimationRequest&)>& callback);

        virtual SPtr<std::function<const nemesis::AnimationRequest&(nemesis::CompileState&)>>
        GetTargetRequest(const nemesis::TemplateClass& templt_class, const nemesis::SemanticManager& manager);

        const nemesis::AnimationRequest* GetBaseRequest(nemesis::CompileState& state) const;

        [[noreturn]] static void ThrowSyntaxError(const std::string& msg, const std::string& expression, size_t line_num, const std::filesystem::path& filepath);
        [[noreturn]] void ThrowSyntaxError(const std::string& msg) const;
        [[noreturn]] void ThrowInvalidError(const std::string& msg) const;
        [[noreturn]] void ThrowInaccessibleError(const std::string& msg) const;
        [[noreturn]] void ThrowTemplateUnsupported(const std::string& msg, const std::string& templt_name) const;
    public:
        virtual ~Statement() noexcept {}

        const std::string& GetExpression() const noexcept;
        size_t GetLineNum() const noexcept;
        const std::filesystem::path& GetFilePath() const noexcept;
        size_t GetTemplateNumber(const nemesis::TemplateClass& templt_class) const;

        virtual std::string Serialize() const = 0;

        VecStr SplitComponents(const std::string& value);
        static VecStr
        SplitComponents(const std::string& value, size_t linenum, const std::filesystem::path& filepath);
    };
}
