#include "Testers/HkxNodeTester.h"

#include "core/NLine.h"
#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/CompileState.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"
#include "core/SemanticManager.h"
#include "core/AnimationRequest.h"

#include "core/Template/TemplateClass.h"

#include "hkx/HkxNode.h"

void nemesis::HkxNodeTester::Run()
{
    nemesis::TemplateClass templt_class(
        "E:\\C++\\Project New Reign - Nemesis\\test environment\\behavior_templates\\fuo\\template_info.json");
    nemesis::TemplateObject templt(&templt_class);

    UPtr<nemesis::AnimationRequest> request = std::make_unique<nemesis::AnimationRequest>("ta", 0, true);

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

    nemesis::SemanticManager manager;
    manager.SetCurrentTemplate(&templt);

    const std::filesystem::path filepath = "E:\\C++\\test.txt";

    VecNstr lines;
    lines.emplace_back("<!-- IF ^o^ -->", 1, filepath);
    lines.emplace_back("		<hkobject name=\"#NID~6\" class=\"hkbStateMachineEventPropertyArray\" "
                       "signature=\"0xb07b4388\">",
                       2,
                       filepath);
    lines.emplace_back("			<hkparam name=\"events\" numelements=\"$@ElementCount$\">			<!-- "
                       "START_COUNTER_D4 -->",
                       3,
                       filepath);
    lines.emplace_back("<!-- IF ^!h^ -->", 4, filepath);
    lines.emplace_back("				<hkobject>      <!-- ADD_COUNTER_D4 -->", 5, filepath);
    lines.emplace_back("					<hkparam name=\"id\">366</hkparam>", 6, filepath);
    lines.emplace_back("					<hkparam name=\"payload\">null</hkparam>", 7, filepath);
    lines.emplace_back("				</hkobject>", 8, filepath);
    lines.emplace_back("<!-- ENDIF -->", 9, filepath);
    lines.emplace_back("<!-- FOREACH ^T^ -->", 10, filepath);
    lines.emplace_back("				<hkobject>      <!-- ADD_COUNTER_D4 -->", 11, filepath);
    lines.emplace_back("					<hkparam name=\"id\">937</hkparam>", 12, filepath);
    lines.emplace_back("					<hkparam name=\"payload\">$T[][event]$</hkparam>", 13, filepath);
    lines.emplace_back("				</hkobject>", 14, filepath);
    lines.emplace_back("<!-- CLOSE -->", 15, filepath);

    lines.emplace_back(
        "			</hkparam>      <!-- END_COUNTER_D4 -->", 16, filepath);
    lines.emplace_back("		</hkobject>", 17, filepath);
    lines.emplace_back("", 18, filepath);
    lines.emplace_back("<!-- ENDIF -->", 19, filepath);
    //lines.emplace_back("		<hkobject name=\"#NID~2\" class=\"hkbStateMachineEventPropertyArray\" "
    //                   "signature=\"0xb07b4388\">",
    //                   20,
    //                   filepath);
    //lines.emplace_back("			<hkparam name=\"events\" numelements=\"0\"></hkparam>",
    //                   21, filepath);
    //lines.emplace_back("		</hkobject>", 22, filepath);

    nemesis::LineStream stream(lines.begin(), lines.end());
    auto hkx_node = nemesis::HkxNode::ParseHkxNode(stream, manager);

    DeqNstr serialized = hkx_node->Serialize();
    DeqNstr compiled   = hkx_node->Compile(state);

    if (serialized.size() != 19)
    {
        throw std::runtime_error("size not match");
    }

    for (size_t i = 0; i < serialized.size(); ++i)
    {
        if (serialized[i] == lines[i]) continue;

        throw std::runtime_error("line not match");
    }
}
