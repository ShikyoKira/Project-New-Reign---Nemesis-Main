#include <regex>

#include "Core/Statement/Statement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

#include "Core/Template/TemplateClass.h"

nemesis::Statement::Statement(const std::string& expression,
                              size_t linenum,
                              const std::filesystem::path filepath,
                              bool no_component)
{
    if (expression.empty())
    {
        throw std::runtime_error("Syntax Error: empty expression detected (Line: " + std::to_string(linenum)
                                 + ", File: " + filepath.string() + ")");
    }

    Expression = expression;
    LineNum    = linenum;
    FilePath   = filepath;

    if (no_component) return;

    Components = SplitComponents(expression);
}

nemesis::Statement::Statement(const nemesis::Line& expression, bool no_component)
{
    if (expression.empty())
    {
        throw std::runtime_error("Syntax Error: empty expression detected (Line: "
                                 + std::to_string(expression.GetLineNumber())
                                 + ", File: " + expression.GetFilePath().string() + ")");
    }

    Expression = expression;
    LineNum    = expression.GetLineNumber();
    FilePath   = expression.GetFilePath();

    if (no_component) return;

    Components = SplitComponents(expression);
}

nemesis::Statement::Statement(const nemesis::Statement& statement)
{
    Expression = statement.Expression;
    LineNum    = statement.LineNum;
    FilePath   = statement.FilePath;
    Components = statement.Components;
}

SPtr<std::function<bool(nemesis::CompileState&)>> nemesis::Statement::CallbackTargetRequests(
    const nemesis::TemplateClass& templt_class,
    const nemesis::SemanticManager& manager,
    const std::function<bool(nemesis::CompileState&, const nemesis::AnimationRequest*)>& callback)
{
    auto& templt_name = templt_class.GetName();
    auto& templt_code = Components.front();

    if (!std::regex_match(templt_code, std::regex("^" + templt_name + "_[0-9]+$")))
    {
        throw std::runtime_error("Syntax Error: Unable to access (" + templt_code
                                 + ") template (Template: " + templt_name + ", File: " + FilePath.string()
                                 + ",  Line: " + std::to_string(LineNum) + ")");
    }

    const std::string& index_str = Components[1];

    if (!nemesis::iequals(index_str, "ANY") && !nemesis::iequals(index_str, "ALL"))
    {
        auto get_request = GetTargetRequest(templt_class, manager);
        return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
            [get_request, callback](nemesis::CompileState& state)
            { return callback(state, (*get_request)(state)); });
    }

    if (nemesis::iequals(index_str, "ANY"))
    {
        return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
            [&templt_code, callback](nemesis::CompileState& state)
            {
                auto request = state.GetCurrentRequest(templt_code);
                auto parents = request->GetParents();

                // Vacuous false: Default to false
                if (!parents.empty())
                {
                    for (auto& request : parents.back()->GetRequests())
                    {
                        if (!callback(state, request)) continue;

                        return true;
                    }

                    return false;
                }

                for (auto& request : state.GetRequests(request->GetTemplateName()))
                {
                    if (!callback(state, request)) continue;

                    return true;
                }

                return false;
            });
    }

    return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
        [&templt_code, callback](nemesis::CompileState& state)
        {
            auto request = state.GetCurrentRequest(templt_code);
            auto parents = request->GetParents();

            // Vacuous false: Default to false
            if (!parents.empty())
            {
                auto req_list = parents.back()->GetRequests();

                if (req_list.empty()) return false;

                for (auto& request : req_list)
                {
                    if (callback(state, request)) continue;

                    return false;
                }

                return true;
            }

            auto& collection = state.GetRequests(request->GetTemplateName());

            if (collection.empty()) return false;

            for (auto& request : collection)
            {
                if (callback(state, request)) continue;

                return false;
            }

            return true;
        });
}

SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>
nemesis::Statement::GetTargetRequest(const nemesis::TemplateClass& templt_class,
                                     const nemesis::SemanticManager& manager)
{
    size_t num                     = GetTemplateNumber(templt_class);
    auto& templt_name              = templt_class.GetName();
    const std::string& templt_code = Components.front();

    if (!std::regex_match(templt_code, std::regex("^" + templt_name + "_[0-9]+$")))
    {
        throw std::runtime_error("Syntax Error: Unable to access (" + templt_code
                                 + ") template (Template: " + templt_name + ", File: " + FilePath.string()
                                 + ",  Line: " + std::to_string(LineNum) + ")");
    }

    const std::string& index_str = Components[1];
    SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>> rst;

    if (index_str == "")
    {
        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [&templt_code](nemesis::CompileState& state)
            { return state.GetCurrentRequest(templt_code); });
    }
    else if (is_only_number(index_str))
    {
        size_t index = std::stoul(index_str);
        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this, &templt_code, index](nemesis::CompileState& state)
            {
                auto request = state.GetCurrentRequest(templt_code);
                auto parents = request->GetParents();

                if (!parents.empty())
                {
                    auto list = parents.back()->GetRequests();

                    if (index < list.size()) return list[index];

                    goto Unaccessible;
                }

                {
                    auto& collection = state.GetRequests(request->GetTemplateName());

                    if (index < collection.size()) return collection[index];
                }

            Unaccessible:
                throw std::runtime_error("Value Unaccessible: Index is larger than list (Syntax: "
                                         + Expression + ", Line: " + std::to_string(LineNum)
                                         + ", File: " + FilePath.string() + ")");
            });
    }
    else if (index_str.size() != 1)
    {
    Invalid:
        throw std::runtime_error("Syntax Error: Invalid request target (Expression: " + Expression
                                 + ", Line: " + std::to_string(LineNum) + ", File: " + FilePath.string()
                                 + ")");
    }
    else
    {
        switch (index_str.front())
        {
            case 'F':
            {
                rst = std::make_shared<
                    std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [&templt_code](nemesis::CompileState& state)
                    { return state.GetFirstRequest(templt_code); });
                break;
            }
            case 'L':
            {
                rst = std::make_shared<
                    std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [&templt_code](nemesis::CompileState& state)
                    { return state.GetLastRequest(templt_code); });
                break;
            }
            case 'B':
            {
                if (!manager.HasRequestInQueue(templt_code)) goto Unaccessible;

                return std::make_shared<
                    std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [&templt_code](nemesis::CompileState& state)
                    { return state.GetBackRequest(templt_code); });
            }
            case 'N':
            {
                if (!manager.HasRequestInQueue(templt_code)) goto Unaccessible;

                return std::make_shared<
                    std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [&templt_code](nemesis::CompileState& state)
                    { return state.GetNextRequest(templt_code); });
            }
            default:
                goto Invalid;
        }
    }

    if (manager.HasRequestInQueue(templt_code)
        || manager.HasRequestInQueue(templt_name + "_" + std::to_string(num - 1)))
    {
        return rst;
    }

Unaccessible:
    throw std::runtime_error("Syntax Error: Unable to get target request from queue (Expression: "
                             + Expression + ", Line: " + std::to_string(LineNum)
                             + ", File: " + FilePath.string() + ")");
}

const nemesis::AnimationRequest* nemesis::Statement::GetBaseRequest(nemesis::CompileState& state) const
{
    auto request = state.GetBaseRequest();

    if (!request)
    {
        throw std::runtime_error("Invalid Access: Base request cannot be found. Use specific "
                                 "request reference instead (<template_code>[]) (Expression: "
                                 + Expression + ", Line: " + std::to_string(LineNum)
                                 + ", File: " + FilePath.string() + ")");
    }

    return request;
}

const std::string& nemesis::Statement::GetExpression() const noexcept
{
    return Expression;
}

size_t nemesis::Statement::GetLineNum() const noexcept
{
    return LineNum;
}

const std::filesystem::path& nemesis::Statement::GetFilePath() const noexcept
{
    return FilePath;
}

size_t nemesis::Statement::GetTemplateNumber(const nemesis::TemplateClass& templt_class) const
{
    std::smatch match;
    auto& template_name = templt_class.GetName();

    if (!std::regex_match(
            Expression,
            match,
            std::regex("^" + template_name + "_([1-9]+)\\[.*?\\](?:\\[.+?\\]|)(?:\\[.+?\\]|)?$")))
    {
        throw std::runtime_error("Syntax Error: Invalid request target (Expression: " + Expression
                                 + ", Line: " + std::to_string(LineNum) + ", File: " + FilePath.string()
                                 + ")");
    }

    size_t num = stoi(match.str(1));
    std::smatch fmatch;
    std::string filename = FilePath.stem().string();

    if (std::regex_match(filename, fmatch, std::regex("^" + template_name + "_([1-9]+)$")))
    {
        size_t fnum = std::stoi(fmatch.str(1));

        if (num - 1 > fnum)
        {
            throw std::runtime_error("Template can only access to current request, parent "
                                     "requests and immediate child request. It "
                                     "cannot access to anything beyond the child requests");
        }
    }

    return num;
}

VecStr nemesis::Statement::SplitComponents(const std::string& value)
{
    VecStr components;
    std::string cur_com;

    bool first        = true;
    bool in_bracket   = false;
    int bracket_count = 0;

    for (const char& c : value)
    {
        if (c == '[')
        {
            if (first)
            {
                components.emplace_back(std::move(cur_com));
                first = false;
            }

            if (in_bracket)
            {
                cur_com.push_back(c);
            }

            in_bracket = true;
            bracket_count++;
        }
        else if (c == ']')
        {
            if (!in_bracket) throw std::runtime_error("Syntax Error: Unexpected ']'");

            if (--bracket_count > 0)
            {
                cur_com.push_back(c);
                continue;
            }

            components.emplace_back(std::move(cur_com));
            in_bracket = false;
        }
        else if (in_bracket || first)
        {
            cur_com.push_back(c);
        }
        else
        {
            throw std::runtime_error("Syntax Error: Unexpected character '" + std::string(1, c) + "'");
        }
    }

    if (first)
    {
        components.emplace_back(std::move(cur_com));
    }

    if (in_bracket) throw std::runtime_error("Syntax Error: Unclosed '['");

    if (bracket_count > 0) throw std::runtime_error("Syntax Error: Unmatched '['");

    return components;
}
