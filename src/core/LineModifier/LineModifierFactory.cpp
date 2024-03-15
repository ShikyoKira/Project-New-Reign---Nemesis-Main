#include "core/LineModifier/MapModifier.h"
#include "core/LineModifier/MathModifier.h"
#include "core/LineModifier/StateIdModifier.h"
#include "core/LineModifier/EventIdModifier.h"
#include "core/LineModifier/CounterModifier.h"
#include "core/LineModifier/RequestIdModifier.h"
#include "core/LineModifier/VariableIdModifier.h"
#include "core/LineModifier/PropertyIdModifier.h"
#include "core/LineModifier/NumelementModifier.h"
#include "core/LineModifier/AttributeIdModifier.h"
#include "core/LineModifier/SubTemplateModifier.h"
#include "core/LineModifier/LineModifierFactory.h"
#include "core/LineModifier/RequestIndexModifier.h"
#include "core/LineModifier/CurrentCountModifier.h"
#include "core/LineModifier/AnimationEventModifier.h"
#include "core/LineModifier/MultipleChoiceModifier.h"
#include "core/LineModifier/OptionVariableModifier.h"
#include "core/LineModifier/AnimationFilePathModifier.h"

#include "utilities/regex.h"

nemesis::LineModifierFactory::ModifierBuilderCollection::ModifierBuilderCollection()
{
    FirstBuilders["@SubTemplate"]  = std::make_shared<ModifierBuilder<nemesis::SubTemplateModifier>>();
    FirstBuilders["@StateID"]      = std::make_shared<ModifierBuilder<nemesis::StateIdModifier>>();
    FirstBuilders["@EventID"]      = std::make_shared<ModifierBuilder<nemesis::EventIdModifier>>();
    FirstBuilders["@VariableID"]   = std::make_shared<ModifierBuilder<nemesis::VariableIdModifier>>();
    FirstBuilders["@AttributeID"]  = std::make_shared<ModifierBuilder<nemesis::AttributeIdModifier>>();
    FirstBuilders["@PropertyID"]   = std::make_shared<ModifierBuilder<nemesis::PropertyIdModifier>>();
    FirstBuilders["@Math"]         = std::make_shared<ModifierBuilder<nemesis::MathModifier>>();
    FirstBuilders["@CurrentCount"] = std::make_shared<ModifierBuilder<nemesis::CurrentCountModifier>>();

    LastBuilders["@ID"]             = std::make_shared<ModifierBuilder<nemesis::RequestIdModifier>>();
    LastBuilders["@Index"]          = std::make_shared<ModifierBuilder<nemesis::RequestIndexModifier>>();
    LastBuilders["@AnimationEvent"] = std::make_shared<ModifierBuilder<nemesis::AnimationEventModifier>>();
    LastBuilders["@AnimationFilePath"]
        = std::make_shared<ModifierBuilder<nemesis::AnimationFilePathModifier>>();
}

Map<size_t, Vec<SPtr<nemesis::LineModifier>>>
nemesis::LineModifierFactory::BuildModifiers(const std::string& line,
                                             size_t linenum,
                                             const std::filesystem::path& filepath,
                                             const nemesis::SemanticManager& manager)
{
    Map<size_t, Vec<SPtr<nemesis::LineModifier>>> modifiers;

    if (line.find("$") != NOT_FOUND)
    {
        std::stringstream ss(line);
        std::string component;
        size_t begin                                         = 0;
        const nemesis::MultipleChoiceStatement* mc_statement = nullptr;

        while (std::getline(ss, component, '$') && !ss.eof())
        {
            begin = ss.tellg();

            if (!std::getline(ss, component, '$')) throw std::runtime_error("Syntax error: Unexpected '$'");

            size_t end = ss.tellg();

            if (mc_statement && mc_statement->IsPartOfChoiceValue(begin, end)) continue;

            auto modifier    = BuildModifier(begin, end, component, line, linenum, filepath, manager);
            auto mc_modifier = dynamic_cast<nemesis::MultipleChoiceModifier*>(modifier.get());

            if (mc_modifier)
            {
                if (mc_statement)
                {
                    throw std::runtime_error("Syntax error: only 1 MultiChoice per line");
                }

                mc_statement = mc_modifier->GetStatement();
            }

            modifiers[end - begin].emplace_back(modifier);
        }
    }

    constexpr std::string_view hkxname_sv(R"(<hkparam name=")");
    size_t pos = line.find(hkxname_sv);

    if (pos == NOT_FOUND) return modifiers;

    constexpr std::string_view numelement_sv(R"(numelements=")");
    pos = line.find(numelement_sv, pos + hkxname_sv.length());

    if (pos == NOT_FOUND) return modifiers;

    pos += numelement_sv.length();
    size_t pos2 = line.find(R"(">)", pos);

    if (pos2 == NOT_FOUND) return modifiers;

    pos2--;
    modifiers[pos2 - pos].emplace_back(std::make_shared<nemesis::NumelementModifier>(pos, pos2, linenum, filepath));
    return modifiers;
}

SPtr<nemesis::LineModifier>
nemesis::LineModifierFactory::BuildModifier(size_t begin,
                                            size_t end,
                                            const std::string& component,
                                            const std::string& line,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            const nemesis::SemanticManager& manager)
{
    if (component == "@MultiChoice")
    {
        return std::make_shared<nemesis::MultipleChoiceModifier>(
            begin - 1, end - 1, line, linenum, filepath, manager);
    }
    else if (component == "@Counter")
    {
        nemesis::smatch match;

        if (!nemesis::regex_match(line, match, ".*\\<\\!-- START_COUNTER_(.+?) --\\>.*"))
        {
            throw std::runtime_error("Syntax Error: Element counter name not found");
        }

        return std::make_shared<nemesis::CounterModifier>(
            begin - 1, end - 1, match[1], linenum, filepath);
    }

    auto components = nemesis::Statement::SplitComponents(component);
    auto& first     = components.front();
    auto& last      = components.back();

    static ModifierBuilderCollection builders;

    auto itr = builders.FirstBuilders.find(first);

    if (itr != builders.FirstBuilders.end())
    {
        return itr->second->Build(begin - 1, end - 1, component, linenum, filepath, manager);
    }

    itr = builders.LastBuilders.find(last);

    if (itr != builders.LastBuilders.end())
    {
        return itr->second->Build(begin - 1, end - 1, component, linenum, filepath, manager);
    }

    if (components.front() == "@Map" || (components.size() > 3 && components[2] == "@Map"))
    {
        return std::make_shared<nemesis::MapModifier>(
            begin - 1, end - 1, component, linenum, filepath, manager);
    }

    return std::make_shared<nemesis::OptionVariableModifier>(
        begin - 1, end - 1, component, linenum, filepath, manager);
}
