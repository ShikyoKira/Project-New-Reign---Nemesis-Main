#include "core/condition.h"

#include "utilities/template.h"

UMap<std::string, nemesis::Condition::Lexer::Type> nemesis::Condition::Lexer::lexingmap = 
{
    { "(", nemesis::Condition::Lexer::Type::OPRN },
    { ")", nemesis::Condition::Lexer::Type::CPRN },
    { "==", nemesis::Condition::Lexer::Type::EQL },
    { "!=", nemesis::Condition::Lexer::Type::NEQL },
    { "&", nemesis::Condition::Lexer::Type::AND },
    { "|", nemesis::Condition::Lexer::Type::OR },
};

const std::regex nemesis::Condition::Lexer::lexingrgx("\\(|\\)|\\=\\=|\\!\\=|&|\\|");
const std::sregex_token_iterator nemesis::Condition::Lexer::end;

nemesis::Condition::Lexer::Lexer(const std::string expression)
{
    bool non_symbol = true;

    for (std::sregex_token_iterator it(expression.begin(), expression.end(), lexingrgx, {-1, 0}); it != end;
         it++)
    {
        std::string str = *it;
        non_symbol      = !non_symbol;

        if (str.empty()) continue;

        if (non_symbol)
        {
            tokens.emplace_back(std::make_pair(lexingmap[str], str));
            continue;
        }

        if (str.front() == '!')
        {
            tokens.emplace_back(std::make_pair(Type::NOT, "!"));

            if (str.length() == 1) continue;

            str = str.substr(1);
        }

        tokens.emplace_back(std::make_pair(Type::SYM, str));
    }
}

nemesis::Condition::Parser::Parser(const Lexer& lexer, Condition& condition)
{
    try
    {
        size_t i = 0;

        if (condition.expression == "(md|!s[][md])&!AV&!AVI")
        {
            i = 0;
        }

        Compare(lexer.tokens, i, condition);
        FollowingExpression(lexer.tokens, i, condition);
    }
    catch (ErrorType ertype)
    {
        errortype = ertype;
    }
}

nemesis::Condition::Parser::Parser(const Lexer::Iter& tokens, Condition& condition)
{
    try
    {
        size_t i = 0;
        Compare(tokens, i, condition);
        FollowingExpression(tokens, i, condition);
    }
    catch (ErrorType ertype)
    {
        errortype = ertype;
    }
}

void nemesis::Condition::Parser::CopyTo(Condition& condition)
{
    if (errortype == ErrorType::NONE)
    {
        condition.negative = negative;
        condition.varA     = std::move(varA);
        condition.varB     = std::move(varB);

        for (auto& each : and_conditions)
        {
            condition.and_conditions.emplace_back(std::move(each));
        }

        for (auto& each : or_conditions)
        {
            condition.or_conditions.emplace_back(std::move(each));
        }

        return;
    }

    switch (errortype)
    {
        case ErrorType::UNCLOSED_BRACKET:
            ErrorMessage(static_cast<int>(errortype),
                         condition.fileptr->GetFileClassName(),
                         condition.fileptr->GetFilePath(),
                         condition.refline.GetLineNumber());
    }
}

void nemesis::Condition::Parser::GetNestedTokens(const Lexer::Iter& tokens, size_t& offset)
{
    for (size_t& i = offset, counter = 1; i < tokens.size(); i++)
    {
        switch (tokens[i].first)
        {
            case Lexer::Type::OPRN:
            {
                ++counter;
                break;
            }
            case Lexer::Type::CPRN:
            {
                --counter;

                if (counter != 0) break;

                ++i;
                return;
            }
        }

        nestedtokens.emplace_back(tokens[i]);
    }

    throw ErrorType::UNCLOSED_BRACKET;
}

void nemesis::Condition::Parser::Compare(const Lexer::Iter& tokens, size_t& offset, Condition& condition)
{
    int expect = Lexer::ALL;

    for (size_t& i = offset; i < tokens.size(); i++)
    {
        auto& token = tokens[i];

        if ((expect & token.first) != token.first) throw ErrorType::SYNTAX_ERROR;

        switch (token.first)
        {
            case Lexer::Type::OPRN:
            {
                OpenParenthesis(tokens, i);
                return;
            }
            case Lexer::Type::EQL:
            {
                negative = false;
                expect   = Lexer::Type::SYM;
                break;
            }
            case Lexer::Type::NEQL:
            {
                negative = true;
                expect   = Lexer::Type::SYM;
                break;
            }
            case Lexer::Type::SYM:
            {
                auto linenum = condition.refline.GetLineNumber();
                auto templt  = condition.fileptr;

                if (varA)
                {
                    varB = std::make_unique<nemesis::AnimVarPtr>(token.second, linenum, *templt);
                    ++i;
                    return;
                }

                // having problem with parsing AVI0
                varA   = std::make_unique<nemesis::AnimVarPtr>(token.second, linenum, *templt);
                expect = expect == Lexer::Type::NOT ?
                         Lexer::Type::AND | Lexer::Type::OR :
                         Lexer::Type::EQL | Lexer::Type::NEQL | Lexer::Type::AND | Lexer::Type::OR;
                break;
            }
            case Lexer::Type::NOT:
            {
                negative = true;
                expect   = Lexer::Type::SYM;
                break;
            }
            case Lexer::Type::AND:
            case Lexer::Type::OR:
            {
                if (varA) return;
            }
            default:
            {
                throw ErrorType::SYNTAX_ERROR;
            }
        }
    }
}

void nemesis::Condition::Parser::FollowingExpression(const Lexer::Iter& tokens,
                                                     size_t& offset,
                                                     Condition& condition)
{
    for (size_t& i = offset; i < tokens.size(); i++)
    {
        auto& token = tokens[i];

        switch (token.first)
        {
            case Lexer::Type::AND:
            {
                And(tokens, i, condition);
                break;
            }
            case Lexer::Type::OR:
            {
                Or(tokens, i, condition);
                break;
            }
            default:
            {
                throw ErrorType::SYNTAX_ERROR;
            }
        }
    }

    if (nestedtokens.empty()) return;

    condition_ptr->emplace_back(std::make_unique<Condition>(nestedtokens, condition));
}

void nemesis::Condition::Parser::OpenParenthesis(const Lexer::Iter& tokens, size_t& offset)
{
    if (nestedtokens.empty())
    {
        GetNestedTokens(tokens, ++offset);
        return;
    }

    switch (nestedtokens.back().first)
    {
        case Lexer::Type::AND:
        case Lexer::Type::OR:
        {
            GetNestedTokens(tokens, offset);
            nestedtokens.emplace_back(tokens[offset]);
            break;
        }
        default:
        {
            throw ErrorType::SYNTAX_ERROR;
        }
    }
}

void nemesis::Condition::Parser::And(const Lexer::Iter& tokens, size_t& offset, Condition& condition)
{
    for (size_t& i = offset; i < tokens.size(); i++)
    {
        auto& token = tokens[i];

        switch (token.first)
        {
            case Lexer::Type::OR:
            {
                --i;
                return;
            }
            case Lexer::Type::AND:
            {
                And(condition);
                break;
            }
            case Lexer::Type::OPRN:
            {
                OpenParenthesis(tokens, i);
                break;
            }
            default:
            {
                nestedtokens.emplace_back(token);
                break;
            }
        }
    }
}

void nemesis::Condition::Parser::And(Condition& condition)
{
    if (condition_ptr)
    {
        if (nestedtokens.empty()) throw ErrorType::SYNTAX_ERROR;

        condition_ptr->emplace_back(std::make_unique<Condition>(nestedtokens, condition));
    }
    else if (!nestedtokens.empty())
    {
        and_conditions.emplace_back(std::make_unique<Condition>(nestedtokens, condition));
    }

    nestedtokens.clear();
    condition_ptr = &and_conditions;
}

void nemesis::Condition::Parser::Or(const Lexer::Iter& tokens, size_t& offset, Condition& condition)
{
    for (size_t& i = offset; i < tokens.size(); i++)
    {
        auto& token = tokens[i];

        switch (token.first)
        {
            case Lexer::Type::OR:
            {
                Or(condition);
                break;
            }
            case Lexer::Type::OPRN:
            {
                OpenParenthesis(tokens, i);
                break;
            }
            default:
            {
                nestedtokens.emplace_back(token);
                break;
            }
        }
    }
}

void nemesis::Condition::Parser::Or(Condition& condition)
{
    if (condition_ptr)
    {
        if (nestedtokens.empty()) throw ErrorType::SYNTAX_ERROR;

        condition_ptr->emplace_back(std::make_unique<Condition>(nestedtokens, condition));
    }
    else if (!nestedtokens.empty())
    {
        or_conditions.emplace_back(std::make_unique<Condition>(nestedtokens, condition));
    }

    nestedtokens.clear();
    condition_ptr = &or_conditions;
}

nemesis::Condition::Condition(const std::string& expression,
                              const nemesis::Line& line,
                              const nemesis::File& file)
    : fileptr(&file)
    , refline(line)
    , expression(expression)
{
    Lexer lexer(expression);
    Parser parser(lexer, *this);
    parser.CopyTo(*this);
}

nemesis::Condition::Condition(const Lexer::Iter& tokens, Condition& condition)
    : fileptr(condition.fileptr)
    , refline(condition.refline)
    , expression(condition.expression)
{
    Parser parser(tokens, condition);
    parser.CopyTo(*this);
}

const nemesis::AnimVarPtr& nemesis::Condition::GetVariableA() const
{
    return *varA;
}

const nemesis::AnimVarPtr& nemesis::Condition::GetVariableB() const
{
    return *varB;
}

std::string nemesis::Condition::GetExpression() const
{
    return expression;
}

size_t nemesis::Condition::GetLineNum() const
{
    return refline.GetLineNumber();
}

const nemesis::File& nemesis::Condition::GetFile() const
{
    return *fileptr;
}

bool nemesis::Condition::IsTrue(nemesis::ScopeInfo& scopeinfo) const
{
    if (expression.empty()) return true;

    if (IsCurrentTrue(scopeinfo))
    {
        if (IsAndTrue(scopeinfo)) return true;
    }

    return IsOrTrue(scopeinfo);
}

bool nemesis::Condition::IsCurrentTrue(nemesis::ScopeInfo& scopeinfo) const
{
    if (!varB) return (varA == nullptr) != negative || varA->IsExist(scopeinfo) != negative;

    return (varA->GetResult(scopeinfo) == varB->GetResult(scopeinfo)) != negative;
}

bool nemesis::Condition::IsAndTrue(nemesis::ScopeInfo& scopeinfo) const
{
    for (auto& condition : and_conditions)
    {
        if (!condition->IsTrue(scopeinfo)) return false;
    }

    return true;
}

bool nemesis::Condition::IsOrTrue(nemesis::ScopeInfo& scopeinfo) const
{
    for (auto& condition : or_conditions)
    {
        if (condition->IsTrue(scopeinfo)) return true;
    }

    return false;
}

void nemesis::Condition::TrimCondition(std::string& condition)
{
    while (!condition.empty() && condition.back() == ' ')
    {
        condition.pop_back();
    }

    while (!condition.empty() && condition.front() == ' ')
    {
        condition.erase(0);
    }
}
