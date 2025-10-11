#include <regex>

#include "Core/LineModifier/AnimationBaseFilenameModifier.h"
#include "Core/LineModifier/AnimationEventModifier.h"
#include "Core/LineModifier/AnimationFilePathModifier.h"
#include "Core/LineModifier/AnimationFilenameModifier.h"
#include "Core/LineModifier/AnimationOrderModifier.h"
#include "Core/LineModifier/AttributeIdModifier.h"
#include "Core/LineModifier/CounterModifier.h"
#include "Core/LineModifier/Crc32Modifier.h"
#include "Core/LineModifier/CurrentCountModifier.h"
#include "Core/LineModifier/EventIdModifier.h"
#include "Core/LineModifier/LengthModifier.h"
#include "Core/LineModifier/LineModifierFactory.h"
#include "Core/LineModifier/ListNameModifier.h"
#include "Core/LineModifier/MapModifier.h"
#include "Core/LineModifier/MathModifier.h"
#include "Core/LineModifier/MaxModifier.h"
#include "Core/LineModifier/MinModifier.h"
#include "Core/LineModifier/MotionDataModifier.h"
#include "Core/LineModifier/MultipleChoiceModifier.h"
#include "Core/LineModifier/NumelementModifier.h"
#include "Core/LineModifier/OptionVariableModifier.h"
#include "Core/LineModifier/PropertyIdModifier.h"
#include "Core/LineModifier/RequestIdModifier.h"
#include "Core/LineModifier/RequestIndexModifier.h"
#include "Core/LineModifier/RotationDataModifier.h"
#include "Core/LineModifier/RunningNumberModifier.h"
#include "Core/LineModifier/StateIdModifier.h"
#include "Core/LineModifier/SubTemplateModifier.h"
#include "Core/LineModifier/SubstringModifier.h"
#include "Core/LineModifier/VariableIdModifier.h"

#include "Utilities/Algorithm.h"

nemesis::LineModifierFactory::ModifierBuilderCollection::ModifierBuilderCollection()
{
    FirstBuilders["@SubTemplate"]    = std::make_unique<ModifierBuilder<nemesis::SubTemplateModifier>>();
    FirstBuilders["@StateID"]        = std::make_unique<ModifierBuilder<nemesis::StateIdModifier>>();
    FirstBuilders["@EventID"]        = std::make_unique<ModifierBuilder<nemesis::EventIdModifier>>();
    FirstBuilders["@VariableID"]     = std::make_unique<ModifierBuilder<nemesis::VariableIdModifier>>();
    FirstBuilders["@AttributeID"]    = std::make_unique<ModifierBuilder<nemesis::AttributeIdModifier>>();
    FirstBuilders["@PropertyID"]     = std::make_unique<ModifierBuilder<nemesis::PropertyIdModifier>>();
    FirstBuilders["@Math"]           = std::make_unique<ModifierBuilder<nemesis::MathModifier>>();
    FirstBuilders["@CurrentCount"]   = std::make_unique<ModifierBuilder<nemesis::CurrentCountModifier>>();
    FirstBuilders["@AnimationOrder"] = std::make_unique<ModifierBuilder<nemesis::AnimationOrderModifier>>();
    FirstBuilders["@RunningNumber"]  = std::make_unique<ModifierBuilder<nemesis::RunningNumberModifier>>();
    FirstBuilders["@Crc32"]          = std::make_unique<ModifierBuilder<nemesis::Crc32Modifier>>();
    FirstBuilders["@Substring"]      = std::make_unique<ModifierBuilder<nemesis::SubstringModifier>>();
    FirstBuilders["@Length"]         = std::make_unique<ModifierBuilder<nemesis::LengthModifier>>();
    FirstBuilders["@Max"]            = std::make_unique<ModifierBuilder<nemesis::MaxModifier>>();
    FirstBuilders["@Min"]            = std::make_unique<ModifierBuilder<nemesis::MinModifier>>();

    LastBuilders["@ListName"]       = std::make_unique<ModifierBuilder<nemesis::ListNameModifier>>();
    LastBuilders["@ID"]             = std::make_unique<ModifierBuilder<nemesis::RequestIdModifier>>();
    LastBuilders["@Index"]          = std::make_unique<ModifierBuilder<nemesis::RequestIndexModifier>>();
    LastBuilders["@AnimationEvent"] = std::make_unique<ModifierBuilder<nemesis::AnimationEventModifier>>();
    LastBuilders["@AnimationFilePath"]
        = std::make_unique<ModifierBuilder<nemesis::AnimationFilePathModifier>>();
    LastBuilders["@AnimationFilename"]
        = std::make_unique<ModifierBuilder<nemesis::AnimationFilenameModifier>>();
    LastBuilders["@AnimationBaseFilename"]
        = std::make_unique<ModifierBuilder<nemesis::AnimationBaseFilenameModifier>>();
}

Map<size_t, Vec<SPtr<nemesis::LineModifier>>>
nemesis::LineModifierFactory::BuildModifiers(const std::string& line,
                                             size_t linenum,
                                             const std::filesystem::path& filepath,
                                             const nemesis::SemanticManager& manager)
{
    Map<size_t, Vec<SPtr<nemesis::LineModifier>>> modifiers;
    const nemesis::MultipleChoiceStatement* mc_statement = nullptr;

    for (size_t i = 0; i < line.length(); i++)
    {
        auto& ch = line[i];

        if (ch != '$') continue;

        size_t begin = i + 1;
        std::string component;
        char ch2;

        do
        {
            if (++i == line.length())
            {
                throw nemesis::ModifierException("Syntax Error: Unexpected '$' (Line: "
                                                 + std::to_string(linenum)
                                                 + ", File: " + nemesis::to_utf8_string(filepath) + ")");
            }

            ch2 = line[i];

            if (ch2 == '$') break;

            component.push_back(ch2);
        } while (true);

        size_t end = i + 1;

        if (mc_statement && mc_statement->IsPartOfChoiceValue(begin, end)) continue;

        auto modifier    = BuildModifier(begin, end, component, line, linenum, filepath, manager);
        auto mc_modifier = dynamic_cast<nemesis::MultipleChoiceModifier*>(modifier.get());

        if (mc_modifier)
        {
            if (mc_statement)
            {
                throw std::runtime_error("Syntax Error: only 1 MultiChoice per line (Line: "
                                         + std::to_string(linenum)
                                         + ", File: " + nemesis::to_utf8_string(filepath) + ")");
            }

            mc_statement = &mc_modifier->GetStatement();
        }

        modifiers[i - begin].emplace_back(modifier);
    }

    constexpr std::string_view hkxname_sv(R"(<hkparam name=")");
    size_t pos = line.find(hkxname_sv);

    if (pos == NOT_FOUND) return modifiers;

    pos += hkxname_sv.length();
    constexpr std::string_view numelement_sv(R"(numelements=")");
    size_t pos2 = line.find(numelement_sv, pos);

    if (pos2 == NOT_FOUND) return modifiers;

    pos2 += numelement_sv.length();
    size_t pos3 = line.find(R"(">)", pos2);

    if (pos3 == NOT_FOUND) return modifiers;

    std::string name = line.substr(pos, pos2 - pos - numelement_sv.length() - 2);
    pos3--;
    modifiers[pos3 - pos2].emplace_back(
        std::make_shared<nemesis::NumelementModifier>(pos2, pos3, name, linenum, filepath));
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
        std::smatch match;
        static std::regex start_counter_rgx(".*\\<\\!-- START_COUNTER_(.+?) --\\>.*");

        if (!std::regex_match(line, match, start_counter_rgx))
        {
            throw std::runtime_error(
                "Syntax Error: Element counter name not found (Expression: @Counter, Line: "
                + std::to_string(linenum) + ", File: " + nemesis::to_utf8_string(filepath) + ")");
        }

        return std::make_shared<nemesis::CounterModifier>(begin - 1, end - 1, match[1], linenum, filepath);
    }

    auto components = nemesis::Statement::SplitComponents(component, linenum, filepath);
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

    if (first == "@Map" || (components.size() > 2 && components[2] == "@Map"))
    {
        return std::make_shared<nemesis::MapModifier>(
            begin - 1, end - 1, component, linenum, filepath, manager);
    }

    if (first == "@MotionData" || (components.size() > 2 && components[2] == "@MotionData"))
    {
        return std::make_shared<nemesis::MotionDataModifier>(
            begin - 1, end - 1, component, linenum, filepath, manager);
    }

    if (first == "@RotationData" || (components.size() > 2 && components[2] == "@RotationData"))
    {
        return std::make_shared<nemesis::RotationDataModifier>(
            begin - 1, end - 1, component, linenum, filepath, manager);
    }

    return std::make_shared<nemesis::OptionVariableModifier>(
        begin - 1, end - 1, component, linenum, filepath, manager);
}
