#include "Core/Statement/AnimationBaseFilenameStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

nemesis::AnimationBaseFilenameStatement::AnimationBaseFilenameStatement(
    const std::string& expression,
    size_t linenum,
    const std::filesystem::path& filepath,
    const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 1:
        {
            GetValueFunction = [this](nemesis::CompileState& state)
            { return nemesis::to_utf8_string(GetBaseRequest(state)->GetAnimationFilePath().stem()); };
            break;
        }
        case 3:
        {
            auto templt_class     = manager.GetCurrentTemplateClass();
            auto get_request_func = GetTargetRequest(*templt_class, manager);

            GetValueFunction = [get_request_func](nemesis::CompileState& state)
            {
                auto& request = (*get_request_func)(state);
                return nemesis::to_utf8_string(request.GetAnimationFilePath().stem());
            };
            break;
        }
        default:
            ThrowSyntaxError("Unsupported AnimationFilePath format");
    }
}

std::string nemesis::AnimationBaseFilenameStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::AnimationBaseFilenameStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}
