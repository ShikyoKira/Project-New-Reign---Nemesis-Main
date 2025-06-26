#include <algorithm>
#include <regex>

#include "Core/Statement/ConditionalStatement.h"

#include "Utilities/Algorithm.h"
#include "Utilities/StringExtension.h"

#include "Core/AnimationRequest.h"
#include "Core/CompileState.h"

#include "Core/Template.h"

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
        for (size_t i = 1; i < expression.length() - 1; ++i)
        {
            switch (expression[i])
            {
                case '"':
                    throw std::runtime_error("Syntax Error: near '\"' (Line: " + std::to_string(linenum)
                                             + ", File: " + nemesis::to_utf8_string(filepath) + ")");
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

std::string nemesis::ConditionalStatement::ConditionalString::GetValue(nemesis::CompileState& state) const
{
    return DynamicComponents.empty() ? ConstantValue : DynamicComponents.back().GetValue(state);
}

void nemesis::ConditionalStatement::ConditionalBoolean::Parse1Component(
    const nemesis::TemplateClass* templt_class, const nemesis::SemanticManager& manager)
{
    const std::string& name = Components.front();

    if (IsComplexComponent(name))
    {
        const auto& dynamic_name = DynamicComponents.emplace_back(name, LineNum, FilePath, manager);
        IsTrueFunction           = [this, templt_class, &dynamic_name](nemesis::CompileState& state)
        {
            const std::string name = dynamic_name.GetValue(state);

            if (name == "@MotionData")
            {
                auto request = GetBaseRequest(state);
                return !request->GetMotionDataList().empty();
            }

            if (name == "@RotationData")
            {
                auto request = GetBaseRequest(state);
                return !request->GetRotationDataList().empty();
            }

            if (!templt_class->GetModel(name))
            {
                throw std::runtime_error("Syntax Error: '" + name
                                         + "' is not a valid option (Line: " + std::to_string(LineNum)
                                         + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
            }

            auto request = GetBaseRequest(state);
            return request->GetOption(name) != nullptr;
        };
        return;
    }

    if (name == "@MotionData")
    {
        IsTrueFunction = [this](nemesis::CompileState& state)
        {
            auto request = GetBaseRequest(state);
            return !request->GetMotionDataList().empty();
        };
        return;
    }

    if (name == "@RotationData")
    {
        IsTrueFunction = [this](nemesis::CompileState& state)
        {
            auto request = GetBaseRequest(state);
            return !request->GetRotationDataList().empty();
        };
        return;
    }

    if (!templt_class->GetModel(name))
    {
        throw std::runtime_error("Syntax Error: '" + name
                                 + "' is not a valid option (Line: " + std::to_string(LineNum)
                                 + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
    }

    IsTrueFunction = [this, &name](nemesis::CompileState& state)
    {
        auto request = GetBaseRequest(state);
        return request->GetOption(name) != nullptr;
    };
}

void nemesis::ConditionalStatement::ConditionalBoolean::Parse2Components(
    const nemesis::TemplateClass* templt_class, const nemesis::SemanticManager& manager)
{
    const std::string& name = Components.front();

    if (IsComplexComponent(name))
    {
        const auto& dynamic_name = DynamicComponents.emplace_back(name, LineNum, FilePath, manager);
        IsTrueFunction           = [this, templt_class, &dynamic_name](nemesis::CompileState& state)
        {
            const std::string name = dynamic_name.GetValue(state);

            if (name == "@Map")
            {
                const std::string& key = Components.back();
                auto request           = GetBaseRequest(state);
                return !request->GetMapValueList(key).empty();
            }

            if (!templt_class->GetModel(name))
            {
                throw std::runtime_error("Syntax Error: '" + name
                                         + "' is not a valid option (Line: " + std::to_string(LineNum)
                                         + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
            }

            size_t index = std::stoul(Components.back());
            auto request = GetBaseRequest(state);
            return index < request->GetOptions(name).size();
        };
        return;
    }

    if (name == "@Map")
    {
        const std::string& key = Components.back();
        IsTrueFunction         = [this, &key](nemesis::CompileState& state)
        {
            auto request = GetBaseRequest(state);
            return !request->GetMapValueList(key).empty();
        };
        return;
    }

    if (!templt_class->GetModel(name))
    {
        throw std::runtime_error("Syntax Error: '" + name
                                 + "' is not a valid option (Line: " + std::to_string(LineNum)
                                 + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
    }

    size_t index = std::stoul(Components.back());

    IsTrueFunction = [this, &name, index](nemesis::CompileState& state)
    {
        auto request = GetBaseRequest(state);
        return index < request->GetOptions(name).size();
    };
}

void nemesis::ConditionalStatement::ConditionalBoolean::Parse3Components(
    const nemesis::TemplateClass* templt_class, const nemesis::SemanticManager& manager)
{
    const std::string& name = Components.back();
    SPtr<std::function<bool(nemesis::CompileState&)>> callback_requests;
    std::function<bool(nemesis::CompileState&, const nemesis::AnimationRequest*)> callback;

    if (IsComplexComponent(name))
    {
        const auto& dynamic_name = DynamicComponents.emplace_back(name, LineNum, FilePath, manager);
        callback                 = [this, templt_class, dynamic_name](nemesis::CompileState& state,
                                                      const nemesis::AnimationRequest* request)
        {
            const std::string name = dynamic_name.GetValue(state);

            if (name == "@MotionData") return !request->GetMotionDataList().empty();

            if (name == "@RotationData") return !request->GetRotationDataList().empty();

            if (templt_class->GetModel(name)) return request->GetOption(name) != nullptr;

            throw std::runtime_error("Syntax Error: \"" + name
                                     + "\" is not a valid option (Line: " + std::to_string(LineNum)
                                     + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
        };
    }
    else if (name == "@MotionData")
    {
        callback = [](nemesis::CompileState& state, const nemesis::AnimationRequest* request)
        { return !request->GetMotionDataList().empty(); };
    }
    else if (name == "@RotationData")
    {
        callback = [](nemesis::CompileState& state, const nemesis::AnimationRequest* request)
        { return !request->GetRotationDataList().empty(); };
    }
    else if (templt_class->GetModel(name))
    {
        callback = [&name](nemesis::CompileState& state, const nemesis::AnimationRequest* request)
        { return request->GetOption(name) != nullptr; };
    }
    else
    {
        throw std::runtime_error("Syntax Error: \"" + name
                                 + "\" is not a valid option (Line: " + std::to_string(LineNum)
                                 + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
    }

    callback_requests = CallbackTargetRequests(*templt_class, manager, callback);
    IsTrueFunction
        = [callback_requests](nemesis::CompileState& state) { return (*callback_requests)(state); };
}

void nemesis::ConditionalStatement::ConditionalBoolean::Parse4Components(
    const nemesis::TemplateClass* templt_class, const nemesis::SemanticManager& manager)
{
    auto get_request_func   = GetTargetRequest(*templt_class, manager);
    const std::string& name = Components[2];
    const std::string& key  = Components.back();
    SPtr<std::function<std::string(nemesis::CompileState&)>> get_key;

    if (IsComplexComponent(key))
    {
        const auto& dynamic_key = DynamicComponents.emplace_back(key, LineNum, FilePath, manager);
        get_key                 = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_key](nemesis::CompileState& state) { return dynamic_key.GetValue(state); });
    }
    else
    {
        get_key = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&key](nemesis::CompileState& state) { return key; });
    }

    if (IsComplexComponent(name))
    {
        const auto& dynamic_name = DynamicComponents.emplace_back(name, LineNum, FilePath, manager);
        IsTrueFunction           = [get_request_func, &dynamic_name, &get_key](nemesis::CompileState& state)
        {
            const std::string name = dynamic_name.GetValue(state);
            const std::string& key = (*get_key)(state);

            if (name == "@Map")
            {
                auto request = (*get_request_func)(state);
                return !request->GetMapValueList(key).empty();
            }

            size_t index = std::stoul(key);
            auto request = (*get_request_func)(state);
            return index < request->GetOptions(name).size();
        };
        return;
    }

    if (name == "@Map")
    {
        IsTrueFunction = [get_request_func, get_key](nemesis::CompileState& state)
        {
            auto request = (*get_request_func)(state);
            return !request->GetMapValueList((*get_key)(state)).empty();
        };
        return;
    }

    if (!templt_class->GetModel(name))
    {
        throw std::runtime_error("Syntax Error: '" + name
                                 + "' is not a valid option (Line: " + std::to_string(LineNum)
                                 + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
    }

    IsTrueFunction = [get_request_func, &name, get_key](nemesis::CompileState& state)
    {
        size_t index = std::stoul((*get_key)(state));
        auto request = (*get_request_func)(state);
        return index < request->GetOptions(name).size();
    };
}

nemesis::ConditionalStatement::ConditionalBoolean::ConditionalBoolean(const std::string& expression,
                                                                      size_t linenum,
                                                                      const std::filesystem::path& filepath,
                                                                      const nemesis::SemanticManager& manager,
                                                                      bool negative)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    auto templt_class = manager.GetCurrentTemplateClass();
    Negative          = negative;

    switch (Components.size())
    {
        case 1:
        {
            Parse1Component(templt_class, manager);
            break;
        }
        case 2:
        {
            Parse2Components(templt_class, manager);
            break;
        }
        case 3:
        {
            Parse3Components(templt_class, manager);
            break;
        }
        case 4:
        {
            Parse4Components(templt_class, manager);
            break;
        }
        default:
            throw std::runtime_error("Syntax Error: Invalid condition statement (Line: "
                                     + std::to_string(linenum)
                                     + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    }
}

std::string nemesis::ConditionalStatement::ConditionalBoolean::Serialize() const
{
    return Expression;
}

std::string nemesis::ConditionalStatement::ConditionalBoolean::GetExpression() const
{
    return (Negative ? "!" : "") + Expression;
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
    auto templt_class = manager.GetCurrentTemplateClass();

    if (Components.size() != 2)
    {
        throw std::runtime_error(
            "Syntax Error: ConditionalAnimationRequest only accepts 1 argument (Expression: " + expression
            + ", Line: " + std::to_string(linenum) + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    GetRequestFunction = *GetTargetRequest(*templt_class, manager);
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
    return GetRequestFunction(state);
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

            for (size_t i = class_name.length() + 1; i < first.length(); ++i)
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

nemesis::ConditionalStatement::ConditionalOption::ConditionalOption(const std::string& expression,
                                                                    size_t linenum,
                                                                    const std::filesystem::path& filepath,
                                                                    const nemesis::SemanticManager& manager)
    : nemesis::OptionStatement(expression, linenum, filepath, manager)
{
}

nemesis::ConditionalStatement::ConditionOptionComparer*
nemesis::ConditionalStatement::ConditionalOption::EqualsTo(ConditionalOption* option) noexcept
{
    return new ConditionOptionComparer(this, option, false);
}

nemesis::ConditionalStatement::ConditionOptionComparer*
nemesis::ConditionalStatement::ConditionalOption::NotEqualsTo(ConditionalOption* option) noexcept
{
    return new ConditionOptionComparer(this, option, true);
}

bool nemesis::ConditionalStatement::ConditionalOption::IsOption(
    const std::string& expression, const nemesis::TemplateObject& template_object)
{
    auto components   = nemesis::Statement::SplitComponents(expression);
    auto templt_class = template_object.GetTemplateClass();

    switch (components.size())
    {
        case 1:
        case 2:
        {
            const std::string& name = components.front();
            auto model              = templt_class->GetModel(name);
            return model;
        }
        case 3:
        case 4:
        {
            if (!std::regex_match(expression,
                                  std::regex("^" + templt_class->GetName()
                                             + "_([1-9]+)\\[.*?\\](?:\\[.+?\\]|)(?:\\[.+?\\]|)?$")))
            {
                return false;
            }

            const std::string& name = components[2];
            auto model              = templt_class->GetModel(name);
            return model;
        }
        default:
            return false;
    }
}

nemesis::ConditionalStatement::ConditionOptionComparer::ConditionOptionComparer(ConditionalOption* variable1,
                                                                                ConditionalOption* variable2,
                                                                                bool negative) noexcept
    : Variable1(variable1)
    , Variable2(variable2)
{
    Negative = negative;
}

nemesis::ConditionalStatement::ConditionOptionComparer::~ConditionOptionComparer() noexcept
{
    delete Variable1;
    delete Variable2;
}

std::string nemesis::ConditionalStatement::ConditionOptionComparer::GetExpression() const
{
    if (Expression.empty())
    {
        Expression = Variable1->GetExpression() + (Negative ? " != " : " == ") + Variable2->GetExpression();
    }

    return Expression;
}

bool nemesis::ConditionalStatement::ConditionOptionComparer::IsTrue(nemesis::CompileState& state) const
{
    return (Variable1->GetOption(state) == Variable2->GetOption(state)) != Negative;
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

std::string nemesis::ConditionalStatement::ConditionalAnimationRequestComparer::GetExpression() const
{
    if (Expression.empty())
    {
        Expression = Variable1->GetExpression() + (Negative ? " != " : " == ") + Variable2->GetExpression();
    }

    return Expression;
}

bool nemesis::ConditionalStatement::ConditionalAnimationRequestComparer::IsTrue(
    nemesis::CompileState& state) const
{
    auto expr    = GetExpression();
    auto rst_ptr = state.TryGetCacheConditionResult(expr);

    if (rst_ptr) return *rst_ptr;

    bool rst = (Variable1->GetRequest(state) == Variable2->GetRequest(state)) != Negative;

    state.CacheConditionResult(expr, rst);
    return rst;
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

std::string nemesis::ConditionalStatement::ConditionalStringComparer::GetExpression() const
{
    if (Expression.empty())
    {
        Expression = First->GetExpression() + (Negative ? " != " : " == ") + First->GetExpression();
    }

    return Expression;
}

bool nemesis::ConditionalStatement::ConditionalStringComparer::IsTrue(nemesis::CompileState& state) const
{
    auto expr    = GetExpression();
    auto rst_ptr = state.TryGetCacheConditionResult(expr);

    if (rst_ptr) return *rst_ptr;

    bool rst = (First->GetValue(state) == Second->GetValue(state)) != Negative;
    state.CacheConditionResult(expr, rst);
    return rst;
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
        if (!condition) continue;

        delete condition;
    }

    for (auto& condition : OrConditions)
    {
        if (!condition) continue;

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

std::string nemesis::ConditionalStatement::ConditionalCollection::GetExpression() const
{
    if (Expression.empty())
    {
        if (!AndConditions.empty())
        {
            Expression.append(AndConditions.front()->GetExpression());

            for (size_t i = 1; i < AndConditions.size(); ++i)
            {
                Expression.append(" && ");
                Expression.append(AndConditions[i]->GetExpression());
            }
        }

        if (!OrConditions.empty())
        {
            Expression.append(OrConditions.front()->GetExpression());

            for (size_t i = 1; i < OrConditions.size(); ++i)
            {
                Expression.append(" || ");
                Expression.append(OrConditions[i]->GetExpression());
            }
        }
    }

    return Expression;
}

bool nemesis::ConditionalStatement::ConditionalCollection::IsTrue(nemesis::CompileState& state) const
{
    auto expr    = GetExpression();
    auto rst_ptr = state.TryGetCacheConditionResult(expr);

    if (rst_ptr) return *rst_ptr;

    if (AndIsTrue(state))
    {
        state.CacheConditionResult(expr, true);
        return true;
    }

    for (auto& condition : OrConditions)
    {
        if (condition->IsTrue(state))
        {
            state.CacheConditionResult(expr, true);
            return true;
        }
    }

    state.CacheConditionResult(expr, false);
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

std::string nemesis::ConditionalStatement::ConditionalParentheses::GetExpression() const
{
    if (Expression.empty())
    {
        Expression = (Negative ? "!(" : "(") + SubNode->GetExpression() + ")";
    }

    return Expression;
}

bool nemesis::ConditionalStatement::ConditionalParentheses::IsTrue(nemesis::CompileState& state) const
{
    auto expr    = GetExpression();
    auto rst_ptr = state.TryGetCacheConditionResult(expr);

    if (rst_ptr) return *rst_ptr;

    bool rst = SubNode->IsTrue(state) != Negative;
    state.CacheConditionResult(expr, rst);
    return rst;
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
    if (!Match(type)) throw std::runtime_error("Syntax Error");

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

    nemesis::ConditionalStatement::ConditionalOption* opt  = nullptr;
    nemesis::ConditionalStatement::ConditionalOption* opt2 = nullptr;

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
                    throw std::runtime_error("Syntax Error: Near \"" + token.Value
                                             + "\" (Line: " + std::to_string(LineNum)
                                             + ", File: " + FilePathPtr->string() + ")");
                }

                Consume(TokenType::EQL);
                auto& templt_obj = *SemanticManager.GetCurrentTemplate();

                if (nemesis::ConditionalStatement::ConditionalAnimationRequest::IsAnimationRequest(
                        token.Value, templt_obj))
                {
                    req = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                        token.Value, LineNum, *FilePathPtr, SemanticManager);
                    auto token2 = Consume(TokenType::SYM);
                    req2        = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                        token2.Value, LineNum, *FilePathPtr, SemanticManager);
                    return req->EqualsTo(req2);
                }
                else if (nemesis::ConditionalStatement::ConditionalOption::IsOption(token.Value, templt_obj))
                {
                    opt = new nemesis::ConditionalStatement::ConditionalOption(
                        token.Value, LineNum, *FilePathPtr, SemanticManager);
                    auto token2 = Consume(TokenType::SYM);
                    opt2        = new nemesis::ConditionalStatement::ConditionalOption(
                        token2.Value, LineNum, *FilePathPtr, SemanticManager);
                    return opt->EqualsTo(opt2);
                }

                str = new nemesis::ConditionalStatement::ConditionalString(
                    token.Value, LineNum, *FilePathPtr, SemanticManager);
                auto token2 = Consume(TokenType::SYM);
                str2        = new nemesis::ConditionalStatement::ConditionalString(
                    token2.Value, LineNum, *FilePathPtr, SemanticManager);
                return str->EqualsTo(str2);
            }
            else if (Match(TokenType::NEQL))
            {
                if (negative)
                {
                    throw std::runtime_error("Syntax Error: Near \"" + token.Value
                                             + "\" (Line: " + std::to_string(LineNum)
                                             + ", File: " + FilePathPtr->string() + ")");
                }

                Consume(TokenType::NEQL);
                auto& templt_obj = *SemanticManager.GetCurrentTemplate();

                if (nemesis::ConditionalStatement::ConditionalAnimationRequest::IsAnimationRequest(
                        token.Value, *SemanticManager.GetCurrentTemplate()))
                {
                    req = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                        token.Value, LineNum, *FilePathPtr, SemanticManager);
                    auto token2 = Consume(TokenType::SYM);
                    req2        = new nemesis::ConditionalStatement::ConditionalAnimationRequest(
                        token2.Value, LineNum, *FilePathPtr, SemanticManager);
                    return req->NotEqualsTo(req2);
                }
                else if (nemesis::ConditionalStatement::ConditionalOption::IsOption(token.Value, templt_obj))
                {
                    opt = new nemesis::ConditionalStatement::ConditionalOption(
                        token.Value, LineNum, *FilePathPtr, SemanticManager);
                    auto token2 = Consume(TokenType::SYM);
                    opt2        = new nemesis::ConditionalStatement::ConditionalOption(
                        token2.Value, LineNum, *FilePathPtr, SemanticManager);
                    return opt->NotEqualsTo(opt2);
                }

                str = new nemesis::ConditionalStatement::ConditionalString(
                    token.Value, LineNum, *FilePathPtr, SemanticManager);
                auto token2 = Consume(TokenType::SYM);
                str2        = new nemesis::ConditionalStatement::ConditionalString(
                    token2.Value, LineNum, *FilePathPtr, SemanticManager);
                return str->NotEqualsTo(str2);
            }

            return new nemesis::ConditionalStatement::ConditionalBoolean(
                token.Value, LineNum, *FilePathPtr, SemanticManager, negative);
        }

        auto token = Advance();
        throw std::runtime_error("Syntax Error: Near \"" + token.Value + "\" (Line: "
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
        if (opt) delete opt;
        if (opt2) delete opt2;

        throw;
    }
}

nemesis::ConditionalStatement::ConditionalStatementParser::ConditionalStatementParser(
    const nemesis::SemanticManager& manager) noexcept
    : SemanticManager(manager)
{
}

void nemesis::ConditionalStatement::ConditionalStatementParser::SetExpression(const std::string& expression)
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

SPtr<nemesis::ConditionalStatement::ConditionalNode>
nemesis::ConditionalStatement::ConditionalStatementParser::MakeCondition() const
{
    TokenIndex = 0;

    if (Tokens.empty()) return nullptr;

    auto node = ParseExpression();

    if (IsEnd()) return SPtr<nemesis::ConditionalStatement::ConditionalNode>(node);

    auto token = Advance();
    throw std::runtime_error("syntax error. Near " + token.Value);
}

nemesis::ConditionalStatement::ConditionalStatement(const std::string& expression,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath, true)
{
    ConditionalStatementParser parser(manager);
    parser.SetExpression(Expression);
    parser.SetLineNumber(LineNum);
    parser.SetFilePath(FilePath);
    CondNode = parser.MakeCondition();
    Components.emplace_back(CondNode->GetExpression());
}

nemesis::ConditionalStatement::ConditionalStatement(const nemesis::Line& line,
                                                    const nemesis::SemanticManager& manager)
    : nemesis::Statement(line)
{
    ConditionalStatementParser parser(manager);
    parser.SetExpression(Expression);
    parser.SetLineNumber(LineNum);
    parser.SetFilePath(FilePath);
    CondNode = parser.MakeCondition();
    Components.emplace_back(CondNode->GetExpression());
}

nemesis::ConditionalStatement::ConditionalStatement(const nemesis::ConditionalStatement& statement)
    : nemesis::Statement(statement)
{
    CondNode = statement.CondNode;
}
