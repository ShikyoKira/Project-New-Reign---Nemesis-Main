#include "Testers/ModifierTester.h"

#include "core/LineModifier/MultipleChoiceModifier.h"

#include "core/CompileState.h"
#include "core/AnimationRequest.h"

#include "core/Template/TemplateClass.h"

#include "utilities/stringextension.h"

void nemesis::ModifierTester::Run()
{
    VecStr tokens;
    std::string mc_line
        = "<hkname name=\"Time\" value=\"$@MultiChoice$\"/> <!-- "
          "o||fuo_1[][T][F][event]==fuo_1[F][T][F][time]||(!h&&(fuo_1[]==fuo_1[F]||fuo_1[B][h]))||ac||ac1 "
          "#NID~1 --> <!-- fuo_1[]==fuo_1[F]&&(AV||AVI) #NID~3 --> <!-- null -->";
    size_t pos = mc_line.find("$@MultiChoice$") + 1;

    for (size_t i = 0; i < mc_line.length(); ++i)
    {
        tokens.emplace_back(1, mc_line[i]);
    }

    nemesis::TemplateClass templt_class(
        "E:\\C++\\Project New Reign - Nemesis\\test environment\\behavior_templates\\fuo\\template_info.json");
    nemesis::TemplateObject templt(&templt_class);

    UPtr<nemesis::AnimationRequest> request = std::make_unique<nemesis::AnimationRequest>("fuo", 0, true);

    auto model  = templt_class.GetModel("T");
    auto option = model->TryCreateOption("TDodgeStop/1.05", 2, "file_list.txt");
    request->AddOption(std::move(option));
    
    auto model2  = templt_class.GetModel("h");
    auto option2 = model->TryCreateOption("h", 3, "file_list.txt");
    request->AddOption(std::move(option2));

    auto request_ptr = request.get();
    nemesis::AnimationRequestRepository repo;
    repo.AddRequest(std::move(request));
    nemesis::CompileState state(repo);
    
    state.SetBaseRequest(request_ptr);
    state.QueueCurrentRequest("fuo_1", request_ptr);

    nemesis::SemanticManager manager;
    manager.SetCurrentTemplate(&templt);

    //std::string line
    //    = "<!-- o||fuo_1[][T][F][event]==fuo_1[F][T][F][time]||(!h&&(fuo_1[]==fuo_1[F]||fuo_1[B][h]))||ac||ac1 "
    //      "#NID~1 --> <!-- fuo_1[]==fuo_1[F]&&(AV||AVI) #NID~3 --> <!-- null -->";

    //nemesis::MultipleChoiceModifier modifier(pos, pos + 3, line, 3, "C:\\mod.txt", manager);

    auto modifier_ptr = nemesis::LineModifierFactory::BuildModifier(pos, pos + 13, "@MultiChoice", mc_line, 3, "C:\\mod.txt", manager);
    nemesis::MultipleChoiceModifier& modifier = static_cast<nemesis::MultipleChoiceModifier&>(*modifier_ptr);

    std::string new_line = modifier.GetStatement()->Serialize();

    if (!StringEndWith(mc_line, new_line)) throw std::runtime_error("serializing or parsing mistake");

    modifier.Apply(tokens, state);

    std::string compiled;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        compiled.append(tokens[i]);
    }

    if (compiled == "<hkname name=\"Time\" value=\"#NID~1\"/> <!-- "
          "o||fuo_1[][T][F][event]==fuo_1[F][T][F][time]||(!h&&(fuo_1[]==fuo_1[F]||fuo_1[B][h]))||ac||ac1 "
           "#NID~1 --> <!-- fuo_1[]==fuo_1[F]&&(AV||AVI) #NID~3 --> <!-- null -->")
        return;

    //if (compiled == "<hkname name=\"Time\" value=\"null\"/>") return;

    throw std::runtime_error("compiling mistake");
}
