#include "Testers/HkxBehaviorTester.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"

#include "core/Template/TemplateClass.h"

#include "hkx/hkxbehavior.h"

void nemesis::HkxBehaviorTester::Run()
{
    nemesis::TemplateClass templt_class(
        "E:\\C++\\Project New Reign - Nemesis\\test environment\\behavior_templates\\fuo\\template_info.json");
    UPtr<nemesis::AnimationRequest> request = std::make_unique<nemesis::AnimationRequest>("fuo", 0, true);

    auto model  = templt_class.GetModel("T");
    auto option = model->TryCreateOption("TDodgeStop/1.05", 1, "file.txt");
    request->AddOption(std::move(option));

    auto model2  = templt_class.GetModel("o");
    auto option2 = model2->TryCreateOption("o", 2, "file.txt");
    request->AddOption(std::move(option2));

    auto request_ptr = request.get();
    nemesis::AnimationRequestRepository repo;
    repo.AddRequest(std::move(request));
    nemesis::CompileState state(repo);

    state.SetBaseRequest(request_ptr);
    state.QueueCurrentRequest("fuo_1", request_ptr);

    auto behavior = nemesis::HkxBehavior::CreateFromFile("E:\\C++\\Project New Reign - Nemesis\\test environment\\data\\meshes\\actors\\character\\behaviors\\Nemesis_bashbehavior.xml");

    DeqNstr serialized = behavior->Serialize();
    DeqNstr compiled   = behavior->Compile(state);
}
