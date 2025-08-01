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
                                 + ", File: " + nemesis::to_utf8_string(filepath) + ")");
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
                                 + ", File: " + nemesis::to_utf8_string(expression.GetFilePath()) + ")");
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
    size_t num        = GetTemplateNumber(templt_class);
    auto& templt_name = templt_class.GetName();
    auto templt_code  = templt_name + "_" + std::to_string(num - 1);

    if (!manager.HasRequestInQueue(Components.front()) && !manager.HasRequestInQueue(templt_code))
    {
        ThrowInaccessibleError("Unable to get target request from queue");
    }

    const std::string& index_str = Components[1];

    if (nemesis::iequals(index_str, "ANY"))
    {
        return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
            [this, templt_code, callback](nemesis::CompileState& state)
            {
                try
                {
                    const nemesis::AnimationRequest* request;

                    if (Components.front() == nemesis::to_utf8_string(FilePath.stem()))
                    {
                        request = state.GetBaseRequest();
                    }
                    else
                    {
                        request = state.GetCurrentRequest(templt_code);
                    }

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
                }
                catch (const std::exception& ex)
                {
                    ThrowSyntaxError(ex.what());
                }
            });
    }
    else if (nemesis::iequals(index_str, "ALL"))
    {
        return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
            [this, templt_code, callback](nemesis::CompileState& state)
            {
                const nemesis::AnimationRequest* request;

                if (Components.front() == nemesis::to_utf8_string(FilePath.stem()))
                {
                    request = state.GetBaseRequest();
                }
                else
                {
                    request = state.GetCurrentRequest(templt_code);
                }

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

    auto get_request = GetTargetRequest(templt_class, manager);
    return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
        [get_request, callback](nemesis::CompileState& state)
        { return callback(state, (*get_request)(state)); });
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
        ThrowTemplateUnsupported("Template unsupported '" + templt_code + "'", templt_name);
    }

    const std::string& index_str = Components[1];
    SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>> rst;

    if (index_str == "")
    {
        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [&templt_code](nemesis::CompileState& state) { return state.GetCurrentRequest(templt_code); });
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

                    ThrowInvalidError("Index is larger than list");
                }

                auto& collection = state.GetRequests(request->GetTemplateName());

                if (index < collection.size()) return collection[index];

                ThrowInvalidError("Index is larger than list");
            });
    }
    else if (index_str.size() != 1)
    {
        ThrowInvalidError("Invalid index value (" + index_str + ")");
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
                if (!manager.HasRequestInQueue(templt_code))
                {
                    ThrowInaccessibleError("Unable to get target request from queue");
                }

                return std::make_shared<
                    std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [&templt_code](nemesis::CompileState& state)
                    { return state.GetBackRequest(templt_code); });
            }
            case 'N':
            {
                if (!manager.HasRequestInQueue(templt_code))
                {
                    ThrowInaccessibleError("Unable to get target request from queue");
                }

                return std::make_shared<
                    std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [&templt_code](nemesis::CompileState& state)
                    { return state.GetNextRequest(templt_code); });
            }
            default:
                ThrowSyntaxError("Invalid value (" + index_str + ")");
        }
    }

    if (manager.HasRequestInQueue(templt_code)
        || manager.HasRequestInQueue(templt_name + "_" + std::to_string(num - 1)))
    {
        return rst;
    }

    ThrowInaccessibleError("Unable to get target request from queue");
}

void nemesis::Statement::ThrowSyntaxError(const std::string& msg) const
{
    throw std::runtime_error("Syntax Error: " + msg + " (Expression: "
                             + Expression + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

void nemesis::Statement::ThrowInvalidError(const std::string& msg) const
{
    throw std::runtime_error("Invalid Value: " + msg + " (Expression: " + Expression
                             + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

void nemesis::Statement::ThrowInaccessibleError(const std::string& msg) const
{
    throw std::runtime_error("Value Inaccessible: " + msg + " (Expression: " + Expression
                             + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

void nemesis::Statement::ThrowTemplateUnsupported(const std::string& msg,
                                                  const std::string& templt_name) const
{
    throw std::runtime_error("Invalid Template: " + msg + " (Template: " + templt_name
                             + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

const nemesis::AnimationRequest* nemesis::Statement::GetBaseRequest(nemesis::CompileState& state) const
{
    auto request = state.GetBaseRequest();

    if (!request)
    {
        ThrowInaccessibleError("Base request cannot be found. Use specific "
                               "request reference instead (<template_code>[]) (Expression: "
                               + Expression + ", Line: " + std::to_string(LineNum)
                               + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
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
        ThrowSyntaxError("Invalid request target");
    }

    size_t num = stoi(match.str(1));
    std::smatch fmatch;
    std::string filename = nemesis::to_utf8_string(FilePath.stem());

    if (std::regex_match(filename, fmatch, std::regex("^" + template_name + "_([1-9]+)$")))
    {
        size_t fnum = std::stoi(fmatch.str(1));

        if (num - 1 > fnum)
        {
            ThrowInaccessibleError("Template can only access to current request, parent "
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
            if (!in_bracket) ThrowSyntaxError("Unexpected ']'");

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
            ThrowSyntaxError("Unexpected character '" + std::string(1, c) + "'");
        }
    }

    if (first)
    {
        components.emplace_back(std::move(cur_com));
    }

    if (in_bracket) ThrowSyntaxError("Unclosed '['");

    if (bracket_count > 0) ThrowSyntaxError("Unmatched '['");

    return components;
}

VecStr nemesis::Statement::SplitComponents(const std::string& value,
                                           size_t linenum,
                                           const std::filesystem::path& filepath)
{
    VecStr components;
    std::string cur_com;

    bool first        = true;
    bool in_bracket   = false;
    int bracket_count = 0;

    static std::function<void(const std::string&, const std::string&, size_t, const std::filesystem::path&)>
        syntax_error = [](const std::string& msg,
                          const std::string& val,
                          size_t linenum,
                          const std::filesystem::path& filepath)
    {
        throw std::runtime_error("Syntax Error: " + msg + " (Expression: " + val
                                 + ", Line: " + std::to_string(linenum)
                                 + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    };

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
            if (!in_bracket) syntax_error("Unexpected ']'", value, linenum, filepath);

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
            syntax_error("Unexpected character '" + std::string(1, c) + "'", value, linenum, filepath);
        }
    }

    if (first)
    {
        components.emplace_back(std::move(cur_com));
    }

    if (in_bracket) syntax_error("Unclosed '['", value, linenum, filepath);

    if (bracket_count > 0) syntax_error("Unmatched '['", value, linenum, filepath);

    return components;
}
