#include "Testers/AnimationDataSingleFileTester.h"

#include "core/AnimationRequest.h"
#include "core/CompileState.h"
#include "core/ModLine.h"
#include "core/NLine.h"
#include "core/SemanticManager.h"

#include "core/Template/TemplateClass.h"

#include "animdata/AnimationDataSingleFile.h"

void nemesis::AnimationDataSingleFileTester::Run()
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
    state.SelectMod("zcbe");

    nemesis::SemanticManager manager;
    manager.SetCurrentTemplate(&templt);

    const std::filesystem::path filepath
        = "E:\\C++\\Project New Reign - Nemesis\\test "
          "environment\\data\\meshes\\nemesis_animationdatasinglefile.txt";

    auto singlefile = nemesis::AnimationDataSingleFile::ParseFromFile(filepath);

    auto project_ptr   = singlefile->GetProject("DefaultFemale.txt", 1);
    auto clip_data_ptr = project_ptr->GetClipData("DodgeBack", "643");

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
          "environment\\behavior_templates\\s\\meshes\\animationdatasinglefile\\DefaultFemale.txt~1\\DodgeBack~643.txt";
    auto m_clipdata = nemesis::AnimationDataClipData::DeserializeFromFile(m_filepath, manager);
    clip_data_ptr->MatchAndUpdate(*m_clipdata);

    DeqNstr serialized    = clip_data_ptr->Serialize();
    DeqNstr compiled      = clip_data_ptr->Compile(state);
    DeqNstr compiled_file = singlefile->Compile(state);

    VecNstr modified_lines;
    GetFileLines(m_filepath, modified_lines);

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
