#include "core/Statements/AnimationEventStatement.h"

#include "utilities/templatecategory.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"
#include "core/AnimationRequest.h"

#include "core/Template/TemplateClass.h"

nemesis::AnimationEventStatement::AnimationEventStatement(const std::string& expression,
                                                          size_t linenum,
                                                          const std::filesystem::path& filepath,
                                                          const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 1:
        {
            GetValueFunction = [](nemesis::CompileState& state) { return state.GetBaseRequest()->GetAnimationEvent(); };
            break;
        }
        case 3:
        {
            auto templt_class     = manager.GetCurrentTemplateClass();
            auto get_request_func = GetTargetRequest(*templt_class, manager);

            GetValueFunction = [get_request_func](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return request->GetAnimationEvent();
            };
            break;
        }
        default:
            throw std::runtime_error("Syntax Error: Unsupported AnimationEvent format");
    }
}

std::string nemesis::AnimationEventStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::AnimationEventStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}
