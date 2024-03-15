#include "core/Statements/CompositeStatement.h"

#include "core/LineModifier/LineModifier.h"

nemesis::CompositeStatement::DynamicComponent::DynamicComponent(const std::string& component,
                                                                size_t linenum,
                                                                const std::filesystem::path& filepath,
                                                                const nemesis::SemanticManager& manager)
    : RawValue(component)
{
    int layer = 0;
    Vec<Pair<size_t, size_t>> pos_list;

    for (size_t i = 0; i < component.length(); ++i)
    {
        const char& ch = component[i];
        Tokens.emplace_back(1, ch);

        switch (ch)
        {
            case '{':
            {
                if (layer++ != 0) break;

                pos_list.emplace_back(i + 1, 0);
                break;
            }
            case '}':
            {
                if (--layer < 0)
                {
                    throw std::runtime_error("Syntax error: Unopened '}' (Component: " + component
                                             + ", Line: " + std::to_string(linenum)
                                             + ", File: " + filepath.string() + ")");
                }

                if (layer > 0) break;

                pos_list.back() = std::make_pair(pos_list.back().first, i + 1);
                break;
            }
        }
    }

    if (pos_list.empty()) return;

    for (auto& pos_pair : pos_list)
    {
        std::string cur  = component.substr(pos_pair.first, pos_pair.second - pos_pair.first - 1);
        auto sh_modifier = nemesis::LineModifierFactory::BuildModifier(
            pos_pair.first, pos_pair.second, cur, component, linenum, filepath, manager);
        Modifiers.emplace_back(sh_modifier);
    }
}

std::string nemesis::CompositeStatement::DynamicComponent::GetValue(nemesis::CompileState& state) const
{
    if (Modifiers.empty()) return RawValue;

    VecStr tokens(Tokens);
    std::string merged;

    for (auto& modifier : Modifiers)
    {
        modifier->Apply(tokens, state);
    }

    for (auto& token : tokens)
    {
        merged.append(token);
    }

    return merged;
}

bool nemesis::CompositeStatement::IsComplexComponent(const std::string& component)
{
    size_t open = 0;
    size_t close = 0;

    for (const char& ch : component)
    {
        switch (ch)
        {
            case '{':
                ++open;
                break;
            case '}':
                ++close;
                break;
            default:
                break;
        }
    }

    return open > 0 && open == close;
}

nemesis::CompositeStatement::CompositeStatement(const std::string& expression,
                                                size_t linenum,
                                                const std::filesystem::path& filepath)
    : nemesis::Statement(expression, linenum, filepath)
{
}

std::string nemesis::CompositeStatement::Serialize() const
{
    return Expression;
}
