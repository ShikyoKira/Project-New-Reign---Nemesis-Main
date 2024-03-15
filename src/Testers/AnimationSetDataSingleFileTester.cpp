#include "Testers/AnimationSetDataSingleFileTester.h"

#include "core/AnimationRequest.h"
#include "core/CompileState.h"
#include "core/ModLine.h"
#include "core/NLine.h"
#include "core/SemanticManager.h"

#include "core/Template/TemplateClass.h"

#include "animsetdata/AnimationSetDataSingleFile.h"

void nemesis::AnimationSetDataSingleFileTester::Run()
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
    state.SelectMod("tudm");

    nemesis::SemanticManager manager;
    manager.SetCurrentTemplate(&templt);

    const std::filesystem::path filepath = "E:\\C++\\Project New Reign - Nemesis\\test environment\\data\\meshes\\nemesis_animationsetdatasinglefile.txt";
    auto singlefile = nemesis::AnimationSetDataSingleFile::ParseFromFile(filepath);

    auto project_ptr = singlefile->GetProject("DefaultFemaleData\\DefaultFemale.txt");
    auto state_ptr   = project_ptr->GetState("_MTSolo.txt");

    VecNstr original_lines;
    GetFileLines(filepath, original_lines, false);
    DeqNstr original_compiled_file = singlefile->Compile(state);

    if (original_lines.size() != original_compiled_file.size())
    {
        throw std::runtime_error("size not match");
    }

    for (size_t i = 0; i < original_lines.size(); ++i)
    {
        if (original_lines[i] == original_compiled_file[i]) continue;

        throw std::runtime_error("line not match");
    }

    const std::filesystem::path m_filepath
        = "E:\\C++\\Project New Reign - Nemesis\\test "
          "environment\\mod\\tudm\\meshes\\animationsetdatasinglefile\\DefaultFemaleData~DefaultFemale\\_"
          "MTSolo.txt";
    auto m_state = nemesis::AnimationSetDataState::DeserializeFromFile(m_filepath);
    state_ptr->MatchAndUpdate(*m_state);

    DeqNstr serialized = state_ptr->Serialize();
    DeqNstr compiled   = state_ptr->Compile(state);
    DeqNstr compiled_file = singlefile->Compile(state);

    VecNstr modified_lines;
    GetFileLines(m_filepath, modified_lines, false);

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
