#include <regex>
#include <sstream>

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
        throw nemesis::StatementException("Syntax Error: empty expression detected (Line: "
                                          + std::to_string(linenum)
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
        throw nemesis::StatementException(
            "Syntax Error: empty expression detected (Line: " + std::to_string(expression.GetLineNumber())
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
    const std::function<bool(nemesis::CompileState&, const nemesis::AnimationRequest&)>& callback)
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
            [this, num, templt_name, templt_code, callback](nemesis::CompileState& state)
            {
                try
                {
                    // Vacuously false: Default to false
                    if (num == 1)
                    {
                        for (auto& request : state.GetRequests(templt_name))
                        {
                            if (!callback(state, *request)) continue;

                            return true;
                        }

                        return false;
                    }

                    auto* request = state.GetBaseRequest();
                    const nemesis::AnimationRequest* parent;

                    if (request->GetLevel() + 1 == num)
                    {
                        parent = request;
                    }
                    else
                    {
                        parent = &state.GetCurrentRequest(templt_code);
                    }

                    for (auto& request : parent->GetRequests())
                    {
                        if (!callback(state, *request)) continue;

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
            [this, num, templt_name, templt_code, callback](nemesis::CompileState& state)
            {
                try
                {
                    // Vacuously true: Default to true
                    if (num == 1)
                    {
                        auto& collection = state.GetRequests(templt_name);

                        for (auto& request : collection)
                        {
                            if (callback(state, *request)) continue;

                            return false;
                        }

                        return true;
                    }

                    auto* request = state.GetBaseRequest();
                    const nemesis::AnimationRequest* parent;

                    if (request->GetLevel() + 1 == num)
                    {
                        parent = request;
                    }
                    else
                    {
                        parent = &state.GetCurrentRequest(templt_code);
                    }

                    auto req_list = parent->GetRequests();

                    for (auto& request : req_list)
                    {
                        if (callback(state, *request)) continue;

                        return false;
                    }

                    return true;
                }
                catch (const std::exception& ex)
                {
                    ThrowSyntaxError(ex.what());
                }
            });
    }

    auto get_request = GetTargetRequest(templt_class, manager);
    return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
        [get_request, callback](nemesis::CompileState& state)
        { return callback(state, (*get_request)(state)); });
}

SPtr<nemesis::Statement::RequestEvaluator>
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
    SPtr<nemesis::Statement::RequestEvaluator> rst;

    if (index_str == "")
    {
        rst = std::make_shared<nemesis::Statement::RequestEvaluator>(
            [&templt_code](nemesis::CompileState& state) -> const nemesis::AnimationRequest&
            { return state.GetCurrentRequest(templt_code); });
    }
    else if (is_only_number(index_str))
    {
        size_t index = std::stoul(index_str);
        rst          = std::make_shared<nemesis::Statement::RequestEvaluator>(
            [this, &templt_code, index](nemesis::CompileState& state) -> const nemesis::AnimationRequest&
            {
                auto& request = state.GetCurrentRequest(templt_code);
                auto parents  = request.GetParents();

                if (!parents.empty())
                {
                    auto list = parents.back()->GetRequests();

                    if (index < list.size()) return *list[index];

                    ThrowInvalidError("Index is larger than list");
                }

                auto& collection = state.GetRequests(request.GetTemplateName());

                if (index < collection.size()) return *collection[index];

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
                rst = std::make_shared<nemesis::Statement::RequestEvaluator>(
                    [&templt_code](nemesis::CompileState& state) -> const nemesis::AnimationRequest&
                    { return *state.GetFirstRequest(templt_code); });
                break;
            }
            case 'L':
            {
                rst = std::make_shared<nemesis::Statement::RequestEvaluator>(
                    [&templt_code](nemesis::CompileState& state) -> const nemesis::AnimationRequest&
                    { return *state.GetLastRequest(templt_code); });
                break;
            }
            case 'B':
            {
                if (!manager.HasRequestInQueue(templt_code))
                {
                    ThrowInaccessibleError("Unable to get target request from queue");
                }

                return std::make_shared<nemesis::Statement::RequestEvaluator>(
                    [&templt_code](nemesis::CompileState& state) -> const nemesis::AnimationRequest&
                    { return *state.GetBackRequest(templt_code); });
            }
            case 'N':
            {
                if (!manager.HasRequestInQueue(templt_code))
                {
                    ThrowInaccessibleError("Unable to get target request from queue");
                }

                return std::make_shared<nemesis::Statement::RequestEvaluator>(
                    [&templt_code](nemesis::CompileState& state) -> const nemesis::AnimationRequest&
                    { return *state.GetNextRequest(templt_code); });
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

SPtr<nemesis::Statement::AggregatePropertyAccessor>
nemesis::Statement::GetTargetAggregatePropertyAccessor(const nemesis::TemplateClass& templt_class,
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
    SPtr<std::function<std::string(
        std::function<std::string(const nemesis::AnimationRequest&, nemesis::CompileState&)>,
        nemesis::CompileState&)>>
        rst;

    if (index_str == "")
    {
        rst = std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
            [&templt_code](nemesis::Statement::PropertyAccessor get_value,
                           nemesis::CompileState& state) -> std::string
            { return get_value(state.GetCurrentRequest(templt_code), state); });
    }
    else if (is_only_number(index_str))
    {
        size_t index = std::stoul(index_str);
        rst          = std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
            [this, &templt_code, index](nemesis::Statement::PropertyAccessor get_value,
                                        nemesis::CompileState& state) -> std::string
            {
                auto& request = state.GetCurrentRequest(templt_code);
                auto parents  = request.GetParents();

                if (!parents.empty())
                {
                    auto list = parents.back()->GetRequests();

                    if (index < list.size()) return get_value(*list[index], state);

                    ThrowInvalidError("Index is larger than list");
                }

                auto& collection = state.GetRequests(request.GetTemplateName());

                if (index < collection.size()) return get_value(*collection[index], state);

                ThrowInvalidError("Index is larger than list");
            });
    }
    else if (index_str == "ALL")
    {
        rst = std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
            [&templt_code](nemesis::Statement::PropertyAccessor get_value,
                                 nemesis::CompileState& state) -> std::string
            {
                auto& request = state.GetCurrentRequest(templt_code);
                auto parents  = request.GetParents();
                std::ostringstream oss;
                size_t i = 0;

                if (!parents.empty())
                {
                    auto list = parents.back()->GetRequests();

                    for (; i < list.size(); ++i)
                    {
                        std::string str = get_value(*list[i], state);

                        if (str.empty()) continue;

                        oss << str;
                        break;
                    }

                    for (++i; i < list.size(); ++i)
                    {
                        std::string str = get_value(*list[i], state);

                        if (str.empty()) continue;

                        oss << ' ' << str;
                    }

                    return oss.str();
                }

                auto& collection = state.GetRequests(request.GetTemplateName());

                for (; i < collection.size(); ++i)
                {
                    std::string str = get_value(*collection[i], state);

                    if (str.empty()) continue;

                    oss << str;
                    break;
                }

                for (++i; i < collection.size(); ++i)
                {
                    std::string str = get_value(*collection[i], state);

                    if (str.empty()) continue;

                    oss << ' ' << str;
                }

                return oss.str();
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
                rst = std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
                    [&templt_code](nemesis::Statement::PropertyAccessor get_value,
                                   nemesis::CompileState& state) -> std::string
                    { return get_value(*state.GetFirstRequest(templt_code), state); });
                break;
            }
            case 'L':
            {
                rst = std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
                    [&templt_code](nemesis::Statement::PropertyAccessor get_value,
                                   nemesis::CompileState& state) -> std::string
                    { return get_value(*state.GetLastRequest(templt_code), state); });
                break;
            }
            case 'B':
            {
                if (!manager.HasRequestInQueue(templt_code))
                {
                    ThrowInaccessibleError("Unable to get target request from queue");
                }

                return std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
                    [&templt_code](nemesis::Statement::PropertyAccessor get_value,
                                   nemesis::CompileState& state) -> std::string
                    { return get_value(*state.GetBackRequest(templt_code), state); });
            }
            case 'N':
            {
                if (!manager.HasRequestInQueue(templt_code))
                {
                    ThrowInaccessibleError("Unable to get target request from queue");
                }

                return std::make_shared<nemesis::Statement::AggregatePropertyAccessor>(
                    [&templt_code](nemesis::Statement::PropertyAccessor get_value,
                                   nemesis::CompileState& state) -> std::string
                    { return get_value(*state.GetNextRequest(templt_code), state); });
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

void nemesis::Statement::ThrowSyntaxError(const std::string& msg,
                                          const std::string& expression,
                                          size_t line_num,
                                          const std::filesystem::path& filepath)
{
    throw nemesis::StatementException("Syntax Error: " + msg + " (Expression: " + expression
                                      + ", Line: " + std::to_string(line_num)
                                      + ", File: " + nemesis::to_utf8_string(filepath) + ")");
}

void nemesis::Statement::ThrowSyntaxError(const std::string& msg) const
{
    ThrowSyntaxError(msg, Expression, LineNum, FilePath);
}

void nemesis::Statement::ThrowInvalidError(const std::string& msg) const
{
    throw nemesis::StatementException("Invalid Value: " + msg + " (Expression: " + Expression
                                      + ", Line: " + std::to_string(LineNum)
                                      + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

void nemesis::Statement::ThrowInaccessibleError(const std::string& msg) const
{
    throw nemesis::StatementException("Value Inaccessible: " + msg + " (Expression: " + Expression
                                      + ", Line: " + std::to_string(LineNum)
                                      + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

void nemesis::Statement::ThrowTemplateUnsupported(const std::string& msg,
                                                  const std::string& templt_name) const
{
    throw nemesis::StatementException("Invalid Template: " + msg + " (Template: " + templt_name
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
            if (!in_bracket) ThrowSyntaxError("Unexpected ']'", value, linenum, filepath);

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
            ThrowSyntaxError("Unexpected character '" + std::string(1, c) + "'", value, linenum, filepath);
        }
    }

    if (first)
    {
        components.emplace_back(std::move(cur_com));
    }

    if (in_bracket) ThrowSyntaxError("Unclosed '['", value, linenum, filepath);

    if (bracket_count > 0) ThrowSyntaxError("Unmatched '['", value, linenum, filepath);

    return components;
}
