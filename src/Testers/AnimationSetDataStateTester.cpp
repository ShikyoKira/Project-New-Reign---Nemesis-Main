#include "Testers/AnimationSetDataStateTester.h"

#include "core/NLine.h"
#include "core/ModLine.h"
#include "core/CompileState.h"
#include "core/SemanticManager.h"
#include "core/AnimationRequest.h"

#include "core/Template/TemplateClass.h"

#include "animsetdata/AnimationSetDataState.h"

void nemesis::AnimationSetDataStateTester::Run()
{
    nemesis::TemplateClass templt_class("E:\\C++\\Project New Reign - Nemesis\\test "
                                        "environment\\behavior_templates\\fuo\\template_info.json");
    nemesis::TemplateObject templt(&templt_class);

    UPtr<nemesis::AnimationRequest> request = std::make_unique<nemesis::AnimationRequest>("ta", 0, true);

    auto model  = templt_class.GetModel("T");
    auto option = model->TryCreateOption("TDodgeStop/2512309553", 1, "file.txt");
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
    lines.emplace_back("V3", 1, filepath);
    lines.emplace_back("0", 2, filepath);
    lines.emplace_back("0", 3, filepath);
    lines.emplace_back("0", 4, filepath);

    lines.emplace_back("1", 5, filepath);
    lines.emplace_back("3064642194", 6, filepath);
    lines.emplace_back("1047251415", 7, filepath);
    lines.emplace_back("7891816", 8, filepath);

    VecNstr modified_lines;
    modified_lines.emplace_back("V3", 1, filepath);
    modified_lines.emplace_back("0", 2, filepath);
    modified_lines.emplace_back("<!-- IF ^!h^ -->", 3, filepath);
    modified_lines.emplace_back("LeftEquipment", 4, filepath);
    modified_lines.emplace_back("<!-- ENDIF -->", 5, filepath);
    modified_lines.emplace_back("0", 6, filepath);
    modified_lines.emplace_back("0", 7, filepath);

    modified_lines.emplace_back("1", 8, filepath);
    modified_lines.emplace_back("3064642194", 9, filepath);
    modified_lines.emplace_back("1047251415", 10, filepath);
    modified_lines.emplace_back("7891816", 11, filepath);
    modified_lines.emplace_back("<!-- FOREACH ^T^ -->", 12, filepath);
    modified_lines.emplace_back("3064642194", 13, filepath);
    modified_lines.emplace_back("$T[][time]$", 14, filepath);
    modified_lines.emplace_back("7891816", 15, filepath);
    modified_lines.emplace_back("<!-- CLOSE -->", 16, filepath);
    modified_lines.emplace_back("<!-- IF ^!o^ -->", 17, filepath);
    modified_lines.emplace_back("3064642194", 18, filepath);
    modified_lines.emplace_back("2158716333", 19, filepath);
    modified_lines.emplace_back("7891816", 20, filepath);
    modified_lines.emplace_back("<!-- ENDIF -->", 21, filepath);

    nemesis::LineStream stream(lines.begin(), lines.end());
    nemesis::LineStream m_stream(modified_lines.begin(), modified_lines.end());

    Vec<nemesis::AnimationSetDataState*> state_list;

    auto states = nemesis::AnimationSetDataState::ParseObjects(
        stream, manager, "ChickenProject.txt", {"FullCharacter.txt"});
    auto m_state = nemesis::AnimationSetDataState::Deserialize("FullCharacter.txt", m_stream, manager);

    auto* state_ptr = states.back().get();
    auto* m_state_ptr = m_state.get();

    state_ptr->MatchAndUpdate(*m_state_ptr);

    DeqNstr serialized = state_ptr->Serialize();
    DeqNstr compiled   = state_ptr->Compile(state);

    if (serialized.size() != modified_lines.size())
    {
        throw std::runtime_error("size not match");
    }

    for (size_t i = 0; i < serialized.size(); ++i)
    {
        if (serialized[i] == modified_lines[i]) continue;

        throw std::runtime_error("line not match");
    }
}
