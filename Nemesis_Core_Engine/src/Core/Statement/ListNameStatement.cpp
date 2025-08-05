#include "Core/Statement/ListNameStatement.h"

#include "Core/AnimationRequest.h"
#include "Core/CompileState.h"

nemesis::ListNameStatement::ListNameStatement(const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 1:
        {
            GetValueFunction
                = [this](nemesis::CompileState& state) { return GetBaseRequest(state)->GetListName(); };
            break;
        }
        case 3:
        {
            auto templt_class     = manager.GetCurrentTemplateClass();
            auto get_request_func = GetTargetRequest(*templt_class, manager);

            GetValueFunction = [get_request_func](nemesis::CompileState& state)
            {
                auto& request = (*get_request_func)(state);
                return request.GetListName();
            };
            break;
        }
        default:
            ThrowSyntaxError("Unsupported RequestId format");
    }
}

std::string nemesis::ListNameStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::ListNameStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}
