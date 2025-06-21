#include "Core/Statement/AnimationFilePathStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

nemesis::AnimationFilePathStatement::AnimationFilePathStatement(const std::string& expression,
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
            { return GetBaseRequest(state)->GetAnimationFilePath().string(); };
            break;
        }
        case 3:
        {
            auto templt_class     = manager.GetCurrentTemplateClass();
            auto get_request_func = GetTargetRequest(*templt_class, manager);

            GetValueFunction = [get_request_func](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return request->GetAnimationFilePath().string();
            };
            break;
        }
        default:
            throw std::runtime_error("Syntax Error: Unsupported AnimationFilePath format (Line: "
                                     + std::to_string(linenum) + ", File: " + filepath.string() + ")");
    }
}

std::string nemesis::AnimationFilePathStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::AnimationFilePathStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}
