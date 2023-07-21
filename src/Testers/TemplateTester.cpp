#include "Testers/TemplateTester.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"

#include "core/Template/TemplateClass.h"

void nemesis::TemplateTester::Run()
{
    nemesis::TemplateClass templt_class(
        "E:\\C++\\Project New Reign - Nemesis\\test environment\\behavior_templates\\fuo\\template_info.json");
    auto templt = nemesis::TemplateHkx::CreateFromFile(
        &templt_class,
        "E:\\C++\\Project New Reign - Nemesis\\test "
        "environment\\behavior_templates\\fuo\\meshes\\actors\\character\\behaviors\\mt_behavior\\fuo_1.xml");

    UPtr<nemesis::AnimationRequest> request = std::make_unique<nemesis::AnimationRequest>("fuo", 0);
    request->SetAnimationEvent("ExampleAnimation");

    auto model  = templt_class.GetModel("T");
    auto option = model->TryCreateOption("TDodgeStop/1.05", 1, "file.txt");
    request->AddOption(std::move(option));

    auto model2  = templt_class.GetModel("o");
    auto option2 = model2->TryCreateOption("o", 2, "file.txt");
    request->AddOption(std::move(option2));
    
    //auto model3  = templt_class.GetModel("ac");
    //auto option3 = model3->TryCreateOption("ac", 3, "file.txt");
    //request->AddOption(std::move(option3));

    auto option3 = model->TryCreateOption("TDodgeStart/1.3", 4, "file.txt");
    request->AddOption(std::move(option3));

    auto option4 = model->TryCreateOption("TAttackQuickStart/2.1", 5, "file.txt");
    request->AddOption(std::move(option4));

    {
        UPtr<nemesis::AnimationRequest> irequest = std::make_unique<nemesis::AnimationRequest>("fuo", 1);
        irequest->SetAnimationEvent("NewAnimation");

        auto model  = templt_class.GetModel("T");
        auto option = model->TryCreateOption("TRollStop/3.05", 1, "file.txt");
        irequest->AddOption(std::move(option));

        auto model2  = templt_class.GetModel("h");
        auto option2 = model2->TryCreateOption("h", 2, "file.txt");
        irequest->AddOption(std::move(option2));

        auto model3  = templt_class.GetModel("o");
        auto option3 = model3->TryCreateOption("o", 3, "file.txt");
        irequest->AddOption(std::move(option3));

        auto option5 = model->TryCreateOption("TRollStart/4.3", 4, "file.txt");
        irequest->AddOption(std::move(option5));

        auto option4 = model->TryCreateOption("TBlockStart/1.1", 5, "file.txt");
        irequest->AddOption(std::move(option4));

        request->AddRequest(std::move(irequest));
    }

    auto request_ptr = request.get();
    nemesis::AnimationRequestRepository repo;
    repo.AddRequest(std::move(request));
    nemesis::CompileState state(repo);

    state.AddEventName("ExampleAnimation");
    state.AddEventName("DodgeStop");
    state.AddEventName("DodgeStart");
    state.AddEventName("AttackQuickStart");
    state.AddEventName("NewAnimation");
    state.AddEventName("RollStop");
    state.AddEventName("RollStart");
    state.AddEventName("BlockStart");
    state.SetBaseRequest(request_ptr);

    DeqNstr serialized = templt->Serialize();
    DeqNstr compiled   = templt->Compile(state);
}
