#include "Testers/NObjectTester.h"

#include "core/NLine.h"
#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/CompileState.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"
#include "core/SemanticManager.h"
#include "core/AnimationRequest.h"

#include "core/Template/TemplateClass.h"

void nemesis::NObjectTester::Run()
{
    nemesis::TemplateClass templt_class(
        "E:\\C++\\Project New Reign - Nemesis\\test environment\\behavior_templates\\fuo\\template_info.json");
    nemesis::TemplateObject templt(&templt_class);

    UPtr<nemesis::AnimationRequest> request = std::make_unique<nemesis::AnimationRequest>("ta", 0);

    auto model  = templt_class.GetModel("T");
    auto option = model->TryCreateOption("TDodgeStop/1.05", 1, "file.txt");
    request->AddOption(std::move(option));

    auto model2  = templt_class.GetModel("o");
    auto option2  = model2->TryCreateOption("o", 2, "file.txt");
    request->AddOption(std::move(option2));

    auto request_ptr = request.get();
    nemesis::AnimationRequestRepository repo;
    repo.AddRequest(std::move(request));
    nemesis::CompileState state(repo);

    state.SetBaseRequest(request_ptr);
    state.QueueCurrentRequest("fuo_1", request_ptr);

    nemesis::SemanticManager manager;
    manager.SetCurrentTemplate(&templt);

    const std::filesystem::path filepath = "E:\\C++\\test.txt";

    nemesis::CollectionObject list;
    list.AddObject(std::make_unique<nemesis::NLine>("_start_", 0, filepath, manager));

    auto nested_list     = std::make_unique<nemesis::CollectionObject>();
    auto nested_list_ptr = nested_list.get();
    auto if_obj = std::make_unique<nemesis::IfObject>("o", 1, filepath, manager, std::move(nested_list));

    nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "		<hkobject name=\"#NID~6\" class=\"hkbStateMachineEventPropertyArray\" "
        "signature=\"0xb07b4388\">",
        2,
        filepath,
        manager));
    nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "			<hkparam name=\"events\" numelements=\"$@ElementCount$\">			<!-- "
        "START_COUNTER_D4 -->",
        3,
        filepath,
        manager));

    auto nested_nested_list     = std::make_unique<nemesis::CollectionObject>();
    auto nested_nested_list_ptr = nested_nested_list.get();
    UPtr<nemesis::IfObject> nested_if_obj
        = std::make_unique<nemesis::IfObject>("!h", 4, filepath, manager, std::move(nested_nested_list));
    nested_nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "				<hkobject>      <!-- ADD_COUNTER_D4 -->", 5, filepath, manager));
    nested_nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "					<hkparam name=\"id\">366</hkparam>", 6, filepath, manager));
    nested_nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "					<hkparam name=\"payload\">null</hkparam>", 7, filepath, manager));
    nested_nested_list_ptr->AddObject(
        std::make_unique<nemesis::NLine>("				</hkobject>", 8, filepath, manager));

    auto fe_list      = std::make_unique<CollectionObject>();
    auto* fe_list_ptr = fe_list.get();
    UPtr<nemesis::ForEachObject> foreach_obj
        = std::make_unique<nemesis::ForEachObject>("T", 9, filepath, manager, std::move(fe_list));
    auto statement = foreach_obj->GetStatement();
    std::string option_name;

    if (statement.TryGetOptionName(option_name))
    {
        manager.TryAddOptionToQueue(option_name, statement.GetExpression());
    }
    else
    {
        manager.TryAddRequestToQueue(statement.GetExpression());
    }

    fe_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "				<hkobject>      <!-- ADD_COUNTER_D4 -->", 10, filepath, manager));
    fe_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "					<hkparam name=\"id\">937</hkparam>", 11, filepath, manager));
    fe_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "					<hkparam name=\"payload\">$T[][event]$</hkparam>", 12, filepath, manager));
    fe_list_ptr->AddObject(
        std::make_unique<nemesis::NLine>("				</hkobject>", 13, filepath, manager));

    if (statement.TryGetOptionName(option_name))
    {
        manager.TryRemoveLastOption();
    }
    else
    {
        manager.TryRemoveLastRequest();
    }

    nested_list_ptr->AddObject(std::move(nested_if_obj));
    nested_list_ptr->AddObject(std::move(foreach_obj));
    nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>(
        "			</hkparam>      <!-- END_COUNTER_D4 -->", 14, filepath, manager));
    nested_list_ptr->AddObject(std::make_unique<nemesis::NLine>("		</hkobject>", 15, filepath, manager));

    list.AddObject(std::move(if_obj));
    list.AddObject(std::make_unique<nemesis::NLine>("_end_", 16, filepath, manager));

    DeqNstr compiled   = list.Compile(state);
    DeqNstr serialized = list.Serialize();
}
