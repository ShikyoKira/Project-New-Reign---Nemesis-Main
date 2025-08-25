#include "Core/NLine.h"
#include "Core/CompileState.h"
#include "Core/ModLine.h"

#include "Utilities/Algorithm.h"
#include "Utilities/ConditionSyntax.h"

namespace ns = nemesis::syntax;

nemesis::NLine::NLine(const nemesis::NLine& nline)
{
    Value = std::make_unique<nemesis::Line>(*nline.Value);

    for (auto& each : nline.ModLines)
    {
        ModLines[each.first] = each.second->Clone();
    }

    for (auto& modifier_list : nline.Modifiers)
    {
        for (auto& each : modifier_list.second)
        {
            Modifiers[modifier_list.first].emplace_back(each);
        }
    }
}

nemesis::NLine::NLine(const std::string& expression,
                      size_t linenum,
                      const std::filesystem::path& filepath,
                      const nemesis::SemanticManager& manager)
{
    Value     = std::make_unique<nemesis::Line>(expression, linenum, filepath);
    Modifiers = LineModifierFactory::BuildModifiers(expression, linenum, filepath, manager);
}

nemesis::NLine::NLine(const nemesis::Line& line, const nemesis::SemanticManager& manager)
{
    Value = std::make_unique<nemesis::Line>(
        line, line.GetLineNumber(), line.GetFilePathPtr()->shared_from_this());
    Modifiers = LineModifierFactory::BuildModifiers(line, line.GetLineNumber(), line.GetFilePath(), manager);
}

void nemesis::NLine::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    try
    {
        if (!ModLines.empty())
        {
            auto mods = state.GetSelectedMods();

            for (auto& mod : mods)
            {
                auto modline = ModLines.find(mod);

                if (modline == ModLines.end()) continue;

                modline->second->CompileTo(lines, state);
                return;
            }
        }

        if (!Value || Value->find(ns::DeleteLine()) != NOT_FOUND) return;

        if (Modifiers.empty())
        {
            auto& line = lines.emplace_back(*Value);
            state.RaiseAddLineEvent(line, *this);
            return;
        }

        VecStr tokens;
        std::string merged;

        for (size_t i = 0; i < Value->length(); ++i)
        {
            tokens.emplace_back(1, (*Value)[i]);
        }

        for (auto& modifier_list : Modifiers)
        {
            for (auto& mod : modifier_list.second)
            {
                mod->Apply(tokens, state);
            }
        }

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            merged.append(tokens[i]);
        }

        if (merged.find(ns::DeleteLine()) != NOT_FOUND) return;

        auto& line_ref = lines.emplace_back(merged, Value->GetLineNumber(), Value->GetFilePath());
        state.RaiseAddLineEvent(line_ref, *this);
    }
    catch (const nemesis::Exception&)
    {
    }
    catch (const std::exception& ex)
    {
        throw nemesis::NObjectException(std::string(ex.what())
                                        + " (Line: " + std::to_string(Value->GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(Value->GetFilePath()) + ")");
    }
}

void nemesis::NLine::SerializeTo(DeqNstr& lines) const
{
    nemesis::Line value = Value ? *Value : nemesis::Line(ns::DeleteLine());

    if (ModLines.empty())
    {
        lines.emplace_back(value);
        return;
    }

    for (auto& modline : ModLines)
    {
        modline.second->SerializeTo(lines);
    }

    lines.emplace_back(value + ns::Spaces() + ns::LowerOriginal());
}

UPtr<nemesis::NObject> nemesis::NLine::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::NLine> nemesis::NLine::Clone() const
{
    return UPtr<nemesis::NLine>(new nemesis::NLine(*this));
}

void nemesis::NLine::MatchAndUpdate(const nemesis::NLine& nline)
{
    if (!nline.Value || *Value != *nline.Value)
    {
        throw nemesis::NObjectException(
            "Template update line to line to does not match (Line: "
            + (!nline.Value ? "???" : std::to_string(nline.Value->GetLineNumber()))
            + ", File: " + nemesis::to_utf8_string(nline.Value->GetFilePath()) + ")");
    }

    if (nline.ModLines.empty()) return;

    for (auto& mod_line : nline.ModLines)
    {
        ModLines[mod_line.first] = mod_line.second->Clone();
    }
}

void nemesis::NLine::MatchAndUpdate(const std::string& mod_code, const nemesis::NLine& nline)
{
    if (!nline.Value || *Value != *nline.Value)
    {
        throw nemesis::NObjectException(
            "Mod update line to line to does not match (Mod: " + mod_code
            + ", Line: " + (!nline.Value ? "???" : std::to_string(nline.Value->GetLineNumber()))
            + ", File: " + nemesis::to_utf8_string(nline.Value->GetFilePath()) + ")");
    }

    if (nline.ModLines.empty()) return;

    for (auto& mod_line : nline.ModLines)
    {
        ModLines[mod_line.first] = mod_line.second->Clone();
    }
}

void nemesis::NLine::AddModLine(const std::string& modcode,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                nemesis::SemanticManager& manager)
{
    ModLines.emplace(modcode, std::make_unique<nemesis::ModLine>(modcode, linenum, filepath, manager));
}

void nemesis::NLine::AddModLine(const std::string& modcode,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                nemesis::SemanticManager& manager,
                                const nemesis::Line& line)
{
    ModLines.emplace(modcode, std::make_unique<nemesis::ModLine>(modcode, linenum, filepath, manager, line));
}
