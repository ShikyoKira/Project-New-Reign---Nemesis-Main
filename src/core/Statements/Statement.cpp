#include "core/Statements/Statement.h"

#include "core/CompileState.h"

#include "utilities/regex.h"
#include "utilities/templatecategory.h"

#include "core/Template/TemplateClass.h"

nemesis::Statement::Statement(const std::string& expression)
{
    if (expression.empty()) throw std::runtime_error("Syntax Error: empty expression detected");

    Expression = expression;
    Components = SplitComponents(expression);
}

nemesis::Statement::Statement(const std::string& expression,
                              size_t linenum,
                              const std::filesystem::path filepath)
{
    if (expression.empty()) throw std::runtime_error("Syntax Error: empty expression detected");

    Expression = expression;
    LineNum    = linenum;
    FilePath   = filepath;
    Components = SplitComponents(expression);
}

nemesis::Statement::Statement(const nemesis::Line& expression)
{
    if (expression.empty()) throw std::runtime_error("Syntax Error: empty expression detected");

    Expression = expression;
    LineNum    = expression.GetLineNumber();
    FilePath   = expression.GetFilePath();
    Components = SplitComponents(expression);
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

SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>
nemesis::Statement::GetTargetRequest(const nemesis::TemplateClass& template_class,
                                     const nemesis::SemanticManager& manager) const
{
    size_t num          = GetTemplateNumber(template_class);
    auto& template_name = template_class.GetName();

    const std::string& index_str = Components[1];
    SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>> rst;

    if (index_str == "")
    {
        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this](nemesis::CompileState& state) { return state.GetCurrentRequest(Components.front()); });
    }
    else if (index_str == "F")
    {
        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this](nemesis::CompileState& state) { return state.GetFirstRequest(Components.front()); });
    }
    else if (index_str == "L")
    {
        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this](nemesis::CompileState& state) { return state.GetLastRequest(Components.front()); });
    }
    else if (isOnlyNumber(index_str))
    {
        size_t index = std::stoul(index_str);

        rst = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this, index](nemesis::CompileState& state)
            {
                auto request = state.GetCurrentRequest(Components.front());
                auto parents = request->GetParents();

                if (!parents.empty()) return parents.back()->GetRequests()[index];

                const nemesis::AnimationRequest* ptr
                    = state.GetRequests(request->GetTemplateName())[index].get(); 
                return ptr;
            });
    }

    if (rst)
    {
        if (!manager.HasRequestInQueue(Components.front())
            && !manager.HasRequestInQueue(template_name + "_" + std::to_string(num - 1)))
        {
            throw std::runtime_error("Syntax error: Unable to get target request from queue (Expression: "
                                     + Expression + ", Line: " + std::to_string(LineNum)
                                     + ", FilePath: " + FilePath.string() + ")");
        }

        return rst;
    }

    if (!manager.HasRequestInQueue(Components.front()))
    {
        throw std::runtime_error("Syntax error: Unable to get target request from queue (Expression: "
                                 + Expression + ", Line: " + std::to_string(LineNum)
                                 + ", FilePath: " + FilePath.string() + ")");
    }

    if (index_str == "B")
    {
        return std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this](nemesis::CompileState& state) { return state.GetBackRequest(Components.front()); });
    }
    else if (index_str == "N")
    {
        return std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
            [this](nemesis::CompileState& state) { return state.GetNextRequest(Components.front()); });
    }

    throw std::runtime_error("Syntax error: Invalid request target (Expression: " + Expression + ", Line: "
                             + std::to_string(LineNum) + ", FilePath: " + FilePath.string() + ")");
}

size_t nemesis::Statement::GetTemplateNumber(const nemesis::TemplateClass& template_class) const
{
    nemesis::smatch match;
    auto& template_name = template_class.GetName();

    if (!nemesis::regex_match(
            Expression, match, "^" + template_name + "_([1-9]+)\\[.*?\\](?:\\[.+?\\]|)(?:\\[.+?\\]|)?$"))
    {
        throw std::runtime_error("Syntax error: Invalid request target (Expression: " + Expression
                                 + ", Line: " + std::to_string(LineNum) + ", FilePath: " + FilePath.string()
                                 + ")");
    }

    size_t num = stoi(match.str(1));
    nemesis::smatch fmatch;

    if (nemesis::regex_match(FilePath.stem().string(), fmatch, "^" + template_name + "_([1-9]+)$"))
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
