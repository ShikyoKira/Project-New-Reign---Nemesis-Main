#include "core/Statements/RequestIndexStatement.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"

nemesis::RequestIndexStatement::RequestIndexStatement(const std::string& expression,
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
                = [](nemesis::CompileState& state) { return std::to_string(state.GetBaseRequest()->GetIndex()); };
            break;
        }
        case 3:
        {
            auto templt_class     = manager.GetCurrentTemplateClass();
            auto get_request_func = GetTargetRequest(*templt_class, manager);

            GetValueFunction = [get_request_func](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return std::to_string(request->GetIndex());
            };
            break;
        }
        default:
            throw std::runtime_error("Syntax Error: Unsupported AnimationEvent format");
    }
}

std::string nemesis::RequestIndexStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::RequestIndexStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}
