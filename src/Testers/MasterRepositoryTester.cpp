#include "Testers/MasterRepositoryTester.h"

#include "hkx/hkxnode.h"

#include "core/CompileState.h"
#include "core/MasterRepository.h"

#include "utilities/threadpool.h"
#include "utilities/stringextension.h"

namespace sf = std::filesystem;

extern sf::path CurrentExeDirectory;

void nemesis::MasterRepositoryTester::Run()
{
    nemesis::MasterRepository repository;
    
    {
        nemesis::ThreadPool tp;

        for (const auto& entry : sf::recursive_directory_iterator(CurrentExeDirectory / "data\\meshes"))
        {
            if (!entry.is_regular_file()) continue;

            const sf::path& path = entry.path();
            std::string ext      = path.extension().string();
            std::string filename = path.stem().string();

#if _DEBUG
            if (!nemesis::iequals(filename, "nemesis_mt_behavior")) continue;

            if (path.string().find("_1stperson") != NOT_FOUND) continue;
#endif

            if (ext == ".xml" && StringStartWith_NC(filename, "nemesis_"))
            {
                auto behavior_uptr = nemesis::HkxBehavior::CreateFromFile(path, tp);
                repository.AddBehavior(std::move(behavior_uptr));
            }
        }

        tp.join_all();
    }

    {
        nemesis::ThreadPool tp;

        for (const auto& entry : sf::directory_iterator(CurrentExeDirectory / L"behavior_templates"))
        {
            if (entry.is_directory())
            {
                std::filesystem::path template_info_file(entry.path() / L"template_info.json");

                if (!std::filesystem::exists(template_info_file)) continue;

                auto template_class = std::make_unique<nemesis::TemplateClass>(template_info_file);
                auto class_ptr      = template_class.get();
                repository.AddTemplateClass(std::move(template_class));

                for (const auto& inner_entry : std::filesystem::recursive_directory_iterator(entry.path()))
                {
                    if (!inner_entry.is_regular_file()) continue;

                    auto& path = inner_entry.path();

                    if (!nemesis::iequals(path.extension().wstring(), L".xml")) continue;

                    sf::path parent_path(nemesis::to_lower_copy(path.parent_path()));
#if _DEBUG
                    if (parent_path.stem().wstring() != L"mt_behavior") continue;
#endif

                    sf::path behavior_path(
                        parent_path.wstring().substr(parent_path.wstring().find(L"meshes\\")));
                    auto behavior_ptr = repository.GetBehaviorByPath(behavior_path);

                    if (!behavior_ptr)
                    {
                        continue;
                        throw std::runtime_error("Behavior referenced by template cannot be found");
                    }

                    if (path.filename().wstring().front() != '#')
                    {
                        auto templt_obj = nemesis::TemplateHkx::CreateFromFile(class_ptr, path, tp);
                        class_ptr->AddTemplate(templt_obj);
                        behavior_ptr->AddTemplate(templt_obj);
                        continue;
                    }

                    behavior_ptr->UpdateNodeFromFile(path, class_ptr);
                }

                continue;
            }

            if (entry.path().extension() != ".txt") continue;

            auto list = repository.GetBehaviorList();

            for (auto& each : list)
            {
                auto sub_template = nemesis::SubTemplateObject::CreateFromFile(entry.path());
                each->AddSubTemplateNode(std::move(sub_template));
            }
        }

        for (const auto& entry : sf::directory_iterator(CurrentExeDirectory / L"mods"))
        {
            if (entry.is_regular_file())
            {

            }
        }

        tp.join_all();
    }

    auto* templt_class = repository.GetTemplateClassByName("fuo");

    auto request
        = templt_class->CreateRequest("fuo -TDodgeStop/1.05,o,ac,TDodgeStart/1.3,TAttackQuickStart/2.1 ExampleAnimation  "
                                      "idlestop.hkx AnimobjectSweetRoll/1",
                                      0,
                                      0,
                                      "E:\\C++\\Project New Reign - Nemesis\\test "
                                      "environment\\data\\meshes\\actors\\character\\animations\\animated"
                                      "eatingredux\\fnis_animatedeatingredux_list.txt");
    auto request_ptr = request.get();
    auto& repo       = repository.GetAnimationRequestRepository();
    repo.AddRequest(std::move(request));

    {
        auto request
            = templt_class->CreateRequest("fuo -TRollStop/3.05,h,o,TRollStart/4.3,TBlockStart/1.1 FZSweetRoll1  ..\\animobjecteatingidlebase.hkx AnimobjectSweetRoll/1",
                                          1,
                                          1,
                                          "E:\\C++\\Project New Reign - Nemesis\\test "
                                          "environment\\data\\meshes\\actors\\character\\animations\\animated"
                                          "eatingredux\\fnis_animatedeatingredux_list.txt");
        request_ptr->AddRequest(std::move(request));
    }

    {
        nemesis::ThreadPool tp;

        repository.CompileAllBehaviors(tp);

        tp.join_all();
    }
}
