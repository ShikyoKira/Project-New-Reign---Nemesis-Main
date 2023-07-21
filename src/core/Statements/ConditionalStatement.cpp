#include <algorithm>

#include "core/Statements/ConditionalStatement.h"

#include "utilities/regex.h"
#include "utilities/stringextension.h"

#include "core/CompileState.h"
#include "core/AnimationRequest.h"

#include "core/Template/TemplateClass.h"

nemesis::ConditionalStatement::ConditionalCollection*
nemesis::ConditionalStatement::ConditionalNode::And(nemesis::ConditionalStatement::ConditionalNode* node)
{
    nemesis::ConditionalStatement::ConditionalCollection* col_node
        = new nemesis::ConditionalStatement::ConditionalCollection();

    try
    {
        return col_node->And(this)->And(node);
    }
    catch (const std::exception&)
    {
        if (col_node) delete col_node;

        throw;
    }
}

nemesis::ConditionalStatement::ConditionalCollection*
nemesis::ConditionalStatement::ConditionalNode::Or(nemesis::ConditionalStatement::ConditionalNode* node)
{
    nemesis::ConditionalStatement::ConditionalCollection* col_node
        = new nemesis::ConditionalStatement::ConditionalCollection();

    try
    {
        return col_node->And(this)->Or(node);
    }
    catch (const std::exception&)
    {
        if (col_node) delete col_node;

        throw;
    }
}

nemesis::ConditionalStatement::ConditionalString::ConditionalString(const std::string& expression,
                                                                    size_t linenum,
                                                                    const std::filesystem::path& filepath,
                                                                    const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (expression.front() == '"' && expression.back() == '"')
    {
        for (size_t i = 1; i < expression.length() - 1; i++)
        {
            switch (expression[i])
            {
                case '"':
                    throw std::runtime_error("Syntax error: near '\"' (Line: " + std::to_string(linenum)
                                             + ", FilePath: " + filepath.string() + ")");
            }
        }

        ConstantValue = expression.substr(1, expression.length() - 2);
        return;
    }

    DynamicComponents.emplace_back(expression, linenum, filepath, manager);
}

nemesis::ConditionalStatement::ConditionalStringComparer*
nemesis::ConditionalStatement::ConditionalString::EqualsTo(ConditionalString* term) noexcept
{
    return new ConditionalStringComparer(this, term, false);
}

nemesis::ConditionalStatement::ConditionalStringComparer*
nemesis::ConditionalStatement::ConditionalString::NotEqualsTo(ConditionalString* term) noexcept
{
    return new ConditionalStringComparer(this, term, true);
}

std::string
nemesis::ConditionalStatement::ConditionalString::GetValue(nemesis::CompileState& state) const
{
    return DynamicComponents.empty() ? ConstantValue : DynamicComponents.back().GetValue(state);
}

nemesis::ConditionalStatement::ConditionalBoolean::ConditionalBoolean(const std::string& expression,
                                                                      size_t linenum,
                                                                      const std::filesystem::path& filepath,
                                                                      const nemesis::SemanticManager& manager,
                                                                      bool negative)
    : nemesis::Statement(expression, linenum, filepath)
{
    auto template_class = manager.GetCurrentTemplateClass();
    Negative            = negative;

    switch (Components.size())
    {
        case 1:
        {
            if (!template_class->GetModel(Components.front()))
            {
                throw std::runtime_error("Syntax error: '" + Components.front() + "' is not a valid option");
            }

            IsTrueFunction = [this](nemesis::CompileState& state)
            {
                auto request = state.GetBaseRequest();
                return request->GetOption(Components.front()) != nullptr;
            };
            break;
        }
        case 2:
        {
            if (!template_class->GetModel(Components.front()))
            {
                throw std::runtime_error("Syntax error: '" + Components.front() + "' is not a valid option");
            }

            size_t index = std::stoul(Components.back());

            IsTrueFunction = [this, index](nemesis::CompileState& state)
            {
                auto request = state.GetBaseRequest();
                return index < request->GetOptions(Components.front()).size();
            };
            break;
        }
        case 3:
        {
            auto get_request_func   = GetTargetRequest(*template_class, manager);
            std::string option_name = Components[2];
            IsTrueFunction          = [get_request_func, option_name](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return request->GetOption(option_name) != nullptr;
            };
            break;
        }
        default:
            throw std::runtime_error("Syntax error");
    }
}

std::string nemesis::ConditionalStatement::ConditionalBoolean::Serialize() const
{
    return Expression;
}

bool nemesis::ConditionalStatement::ConditionalBoolean::IsTrue(nemesis::CompileState& state) const
{
    return IsTrueFunction(state) != Negative;
}

nemesis::ConditionalStatement::ConditionalAnimationRequest::ConditionalAnimationRequest(
    const std::string& expression,
    size_t linenum,
    const std::filesystem::path& filepath,
    const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    auto template_class = manager.GetCurrentTemplateClass();

    switch (Components.size())
    {
        case 2:
        {
            GetRequestFunction = GetTargetRequest(*template_class, manager);
            break;
        }
        default:
            throw std::runtime_error("Syntax error");
    }
}

std::string nemesis::ConditionalStatement::ConditionalAnimationRequest::Serialize() const
{
    return Expression;
}

nemesis::ConditionalStatement::ConditionalAnimationRequestComparer*
nemesis::ConditionalStatement::ConditionalAnimationRequest::EqualsTo(
    ConditionalAnimationRequest* request) noexcept
{
    return new ConditionalAnimationRequestComparer(this, request, false);
}

nemesis::ConditionalStatement::ConditionalAnimationRequestComparer*
nemesis::ConditionalStatement::ConditionalAnimationRequest::NotEqualsTo(
    ConditionalAnimationRequest* request) noexcept
{
    return new ConditionalAnimationRequestComparer(this, request, true);
}

const nemesis::AnimationRequest*
nemesis::ConditionalStatement::ConditionalAnimationRequest::GetRequest(nemesis::CompileState& state) const
{
    return (*GetRequestFunction)(state);
}

bool nemesis::ConditionalStatement::ConditionalAnimationRequest::IsAnimationRequest(
    const std::string& term, const nemesis::TemplateObject& template_object)
{
    auto components   = nemesis::Statement::SplitComponents(term);
    auto templt_class = template_object.GetTemplateClass();

    switch (components.size())
    {
        case 2:
        {
            auto& class_name = templt_class->GetName();
            auto& first      = components[0];

            if (class_name.length() + 2 > first.length()) return false;

            for (size_t i = 0; i < class_name.length(); ++i)
            {
                if (class_name[i] != first[i]) return false;
            }

            if (first[class_name.length()] != '_') return false;

            for (size_t i = class_name.length() + 1; i < first.length(); i++)
            {
                auto& ch = first[i];

                if (!isdigit(ch) || ch == '0') return false;
            }

            return true;
        }
        default:
            return false;
    }
}

nemesis::ConditionalStatement::ConditionalAnimationRequestComparer::ConditionalAnimationRequestComparer(
    ConditionalAnimationRequest* variable1, ConditionalAnimationRequest* variable2, bool negative) noexcept
    : Variable1(variable1)
    , Variable2(variable2)
{
    Negative = negative;
}

nemesis::ConditionalStatement::ConditionalAnimationRequestComparer::
    ~ConditionalAnimationRequestComparer() noexcept
{
    delete Variable1;
    delete Variable2;
}

bool nemesis::ConditionalStatement::ConditionalAnimationRequestComparer::IsTrue(nemesis::CompileState& state) const
{
    return (Variable1->GetRequest(state) == Variable2->GetRequest(state)) != Negative;
}

nemesis::ConditionalStatement::ConditionalStringComparer::ConditionalStringComparer(ConditionalString* first,
                                                                        ConditionalString* second,
                                                                        bool negative) noexcept
    : First(first)
    , Second(second)
{
    Negative = negative;
}

nemesis::ConditionalStatement::ConditionalStringComparer::~ConditionalStringComparer() noexcept
{
    delete First;
    delete Second;
}

bool nemesis::ConditionalStatement::ConditionalStringComparer::IsTrue(nemesis::CompileState& state) const
{
    return (First->GetValue(state) == Second->GetValue(state)) != Negative;
}

bool nemesis::ConditionalStatement::ConditionalCollection::AndIsTrue(nemesis::CompileState& state) const
{
    for (auto& condition : AndConditions)
    {
        if (!condition->IsTrue(state)) return false;
    }

    return true;
}

nemesis::ConditionalStatement::ConditionalCollection::~ConditionalCollection() noexcept
{
    for (auto& condition : AndConditions)
    {
        if (condition == nullptr) continue;

        delete condition;
    }

    for (auto& condition : OrConditions)
    {
        if (condition == nullptr) continue;

        delete condition;
    }
}

nemesis::ConditionalStatement::ConditionalCollection*
nemesis::ConditionalStatement::ConditionalCollection::And(
    nemesis::ConditionalStatement::ConditionalNode* node)
{
    AndConditions.emplace_back(node);
    return this;
}

nemesis::ConditionalStatement::ConditionalCollection*
nemesis::ConditionalStatement::ConditionalCollection::Or(nemesis::ConditionalStatement::ConditionalNode* node)
{
    OrConditions.emplace_back(node);
    return this;
}

bool nemesis::ConditionalStatement::ConditionalCollection::IsTrue(nemesis::CompileState& state) const
{
    if (AndIsTrue(state)) return true;

    for (auto& condition : OrConditions)
    {
        if (condition->IsTrue(state)) return true;
    }

    return false;
}

nemesis::ConditionalStatement::ConditionalParentheses::ConditionalParentheses(
    nemesis::ConditionalStatement::ConditionalNode* subnode, bool negative) noexcept
    : SubNode(subnode)
{
    Negative = negative;
}

nemesis::ConditionalStatement::ConditionalParentheses::~ConditionalParentheses() noexcept
{
    if (!SubNode) return;

    delete SubNode;
}

bool nemesis::ConditionalStatement::ConditionalParentheses::IsTrue(nemesis::CompileState& state) const
{
    return SubNode->IsTrue(state) != Negative;
}

nemesis::ConditionalStatement::ConditionalStatementParser::Token::Token(TokenType type,
                                                                        const std::string& value) noexcept
    : Type(type)
    , Value(value)
{
}

nemesis::ConditionalStatement::ConditionalStatementParser::TokenParser::TokenParser()
    : TokenRegex("\\(|\\)|\\=\\=|\\!\\=|&&|\\|\\|")
    , TokenMap({
          {"(", TokenType::OPRN},
          {")", TokenType::CPRN},
          {"==", TokenType::EQL},
          {"!=", TokenType::NEQL},
          {"&&", TokenType::AND},
          {"||", TokenType::OR},
      })
{
}

const nemesis::ConditionalStatement::ConditionalStatementParser::Token&
nemesis::ConditionalStatement::ConditionalStatementParser::Advance() const noexcept
{
    return Tokens[TokenIndex++];
}

const nemesis::ConditionalStatement::ConditionalStatementParser::Token&
nemesis::ConditionalStatement::ConditionalStatementParser::Previous() const noexcept
{
    return Tokens[TokenIndex - 1];
}

bool nemesis::ConditionalStatement::ConditionalStatementParser::Match(
    nemesis::ConditionalStatement::ConditionalStatementParser::TokenType type) const noexcept
{
    return TokenIndex < Tokens.size() && Tokens[TokenIndex].Type == type;
}

const nemesis::ConditionalStatement::ConditionalStatementParser::Token&
nemesis::ConditionalStatement::ConditionalStatementParser::Consume(TokenType type) const
{
    if (!Match(type)) throw std::runtime_error("Syntax error");

    return Advance();
}

bool nemesis::ConditionalStatement::ConditionalStatementParser::IsEnd() const noexcept
{
    return TokenIndex == Tokens.size();
}

nemesis::ConditionalStatement::ConditionalNode*
nemesis::ConditionalStatement::ConditionalStatementParser::ParseExpression() const
{
    nemesis::ConditionalStatement::ConditionalNode* node  = nullptr;
    nemesis::ConditionalStatement::ConditionalNode* node2 = nullptr;

    try
    {
        node = ParseTerm();

        if (!Match(TokenType::OR)) return node;

        while (Match(TokenType::OR))
        {
            Consume(TokenType::OR);
            node2 = ParseTerm();
            node  = node->Or(node2);
        }

        return node;
    }
    catch (const std::exception&)
    {
        if (node) delete node;
        if (node2) delete node2;

        throw;
    }
}

nemesis::ConditionalStatement::ConditionalNode*
nemesis::ConditionalStatement::ConditionalStatementParser::ParseTerm() const
{
    nemesis::ConditionalStatement::ConditionalNode* node  = nullptr;
    nemesis::ConditionalStatement::ConditionalNode* node2 = nullptr;

    try
    {
        node = ParseFactor();

        while (Match(TokenType::AND))
        {
            Consume(TokenType::AND);
            node2 = ParseFactor();
            node  = node->And(node2);
        }

        return node;
    }
    catch (const std::exception&)
    {
        if (node) delete node;
        if (node2) delete node2;

        throw;
    }
}

nemesis::ConditionalStatement::ConditionalNode*
nemesis::ConditionalStatement::ConditionalStatementParser::ParseFactor() const
{
    bool negative = false;

    nemesis::ConditionalStatement::ConditionalNode* node1 = nullptr;
    nemesis::ConditionalStatement::ConditionalNode* node2 = nullptr;

    nemesis::ConditionalStatement::ConditionalString* str  = nullptr;
    nemesis::ConditionalStatement::ConditionalString* str2 = nullptr;

    nemesis::ConditionalStatement::ConditionalAnimationRequest* req  = nullptr;
    nemesis::ConditionalStatement::ConditionalAnimationRequest* req2 = nullptr;

    try
    {
        if (Match(TokenType::NOT))
        {
            Consume(TokenType::NOT);
            negative = true;
        }

        if (Match(TokenType::OPRN))
        {
            Consume(TokenType::OPRN);
            node1 = ParseExpression();
            node2 = new nemesis::ConditionalStatement::ConditionalParentheses(node1, negative);
            Consume(TokenType::CPRN);
            return node2;
        }

        if (Match(TokenType::SYM))
        {
            auto token = Advance();

            if (Match(TokenType::EQL))
            {
                if (negative)
                {
                    throw std::runtime_error("Syntax error: Near \"" + token.Value + "\" (Line: " + std::to_string(LineNum)
                                             + ", File: " + FilePathPtr->string() + ")");
                }

                Consume(TokenType::EQL);

                if (!nemesis::ConditionalStatement::ConditionalAnimationRequest::IsAnimationRequest(
                        token.Value, *SemanticManager.GetCurrentTemplate()))
                {
                    str = new nemesis::ConditionalStatement::ConditionalString(
                        token.Value, LineNum, *FilePathPtr, SemanticManager);
                    auto token2 = Consume(TokenType::SYM);
                    str2        = new nemesis::ConditionalStatement::ConditionalString(
                        token2.Value, LineNum, *FilePathPtr, SemanticManager);
                    return str->EqualsTo(str2);
                }

                req = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                    token.Value, LineNum, *FilePathPtr, SemanticManager);
                auto token2 = Consume(TokenType::SYM);
                req2        = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                    token2.Value, LineNum, *FilePathPtr, SemanticManager);
                return req->EqualsTo(req2);
            }
            else if (Match(TokenType::NEQL))
            {
                if (negative)
                {
                    throw std::runtime_error("Syntax error: Near \"" + token.Value
                                             + "\" (Line: " + std::to_string(LineNum)
                                             + ", File: " + FilePathPtr->string() + ")");
                }

                Consume(TokenType::NEQL);

                if (!nemesis::ConditionalStatement::ConditionalAnimationRequest::IsAnimationRequest(
                        token.Value, *SemanticManager.GetCurrentTemplate()))
                {
                    str = new nemesis::ConditionalStatement::ConditionalString(
                        token.Value, LineNum, *FilePathPtr, SemanticManager);
                    auto token2 = Consume(TokenType::SYM);
                    str2        = new nemesis::ConditionalStatement::ConditionalString(
                        token2.Value, LineNum, *FilePathPtr, SemanticManager);
                    return str->NotEqualsTo(str2);
                }

                req = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                    token.Value, LineNum, *FilePathPtr, SemanticManager);
                auto token2 = Consume(TokenType::SYM);
                req2        = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                    token2.Value, LineNum, *FilePathPtr, SemanticManager);
                return req->NotEqualsTo(req2);
            }

            return new nemesis::ConditionalStatement::ConditionalBoolean(
                token.Value, LineNum, *FilePathPtr, SemanticManager, negative);
        }

        auto token = Advance();
        throw std::runtime_error("Syntax error: Near \"" + token.Value + "\" (Line: "
                                 + std::to_string(LineNum) + ", File: " + FilePathPtr->string() + ")");
    }
    catch (const std::exception&)
    {
        if (node1) delete node1;
        if (node2) delete node2;
        if (str) delete str;
        if (str2) delete str2;
        if (req) delete req;
        if (req2) delete req2;

        throw;
    }
}

nemesis::ConditionalStatement::ConditionalStatementParser::ConditionalStatementParser(
    const nemesis::SemanticManager& manager) noexcept
    : SemanticManager(manager)
{
}

void nemesis::ConditionalStatement::ConditionalStatementParser::SetExpression(
    const std::string& expression)
{
    const static TokenParser Parser;
    bool non_symbol = true;
    Tokens.clear();

    for (std::sregex_token_iterator it(expression.begin(), expression.end(), Parser.TokenRegex, {-1, 0});
         it != Parser.End;
         it++)
    {
        std::string str = StringTrim(*it);
        non_symbol      = !non_symbol;

        if (str.empty()) continue;

        if (non_symbol)
        {
            Tokens.emplace_back(Parser.TokenMap.at(str), str);
            continue;
        }

        if (str.front() == '!')
        {
            Tokens.emplace_back(TokenType::NOT, "!");

            if (str.length() == 1) continue;

            str = str.substr(1);
        }

        Tokens.emplace_back(TokenType::SYM, str);
    }
}

void nemesis::ConditionalStatement::ConditionalStatementParser::SetLineNumber(size_t linenum) noexcept
{
    LineNum = linenum;
}

void nemesis::ConditionalStatement::ConditionalStatementParser::SetFilePath(
    const std::filesystem::path& filepath) noexcept
{
    FilePathPtr = &filepath;
}

nemesis::ConditionalStatement::ConditionalNode*
nemesis::ConditionalStatement::ConditionalStatementParser::MakeCondition() const
{
    TokenIndex = 0;

    if (Tokens.empty()) return nullptr;

    auto node = ParseExpression();

    if (IsEnd()) return node;

    auto token = Advance();
    throw std::runtime_error("syntax error. Near " + token.Value);
}

nemesis::ConditionalStatement::ConditionalStatement(const std::string& expression,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
{
    Expression = expression;
    LineNum    = linenum;
    FilePath   = filepath;

    ConditionalStatementParser parser(manager);
    parser.SetExpression(Expression);
    parser.SetLineNumber(LineNum);
    parser.SetFilePath(FilePath);
    CondNode = parser.MakeCondition();
}

nemesis::ConditionalStatement::ConditionalStatement(const nemesis::Line& line,
                                                    const nemesis::SemanticManager& manager)
{
    Expression = line;
    LineNum    = line.GetLineNumber();
    FilePath   = line.GetFilePath();

    ConditionalStatementParser parser(manager);
    parser.SetExpression(Expression);
    parser.SetLineNumber(LineNum);
    parser.SetFilePath(FilePath);
    CondNode = parser.MakeCondition();
}

nemesis::ConditionalStatement::~ConditionalStatement() noexcept
{
    if (!CondNode) return;

    delete CondNode;
}
