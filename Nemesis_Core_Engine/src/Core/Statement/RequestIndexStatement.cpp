#include "Core/Statement/RequestIndexStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

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
            GetValueFunction = [this](nemesis::CompileState& state)
            { return std::to_string(GetBaseRequest(state)->GetIndex()); };
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
            throw std::runtime_error("Syntax Error: Unsupported request index format (Line: "
                                     + std::to_string(linenum)
                                     + ", File: " + nemesis::to_utf8_string(filepath) + ")");
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
