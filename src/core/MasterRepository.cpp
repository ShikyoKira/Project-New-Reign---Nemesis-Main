#include "core/CompileState.h"
#include "core/MasterRepository.h"

#include "utilities/writetextfile.h"

#include "generate/hkxcompiler.h"

namespace sf = std::filesystem;

void nemesis::MasterRepository::AddBehavior(UPtr<nemesis::HkxBehavior>&& behavior)
{
    auto* ptr              = BehaviorList.emplace_back(std::move(behavior)).get();
    auto path              = ptr->GetFilePath();
    std::wstring path_wstr = path.wstring();

    path = path_wstr.substr(path_wstr.find(L"meshes\\"));
    path = path.parent_path() / path.stem().wstring().substr(8);

    BehaviorMap[nemesis::to_lower_copy(path)] = ptr;
}

nemesis::HkxBehavior* nemesis::MasterRepository::GetBehaviorByPath(const std::filesystem::path& filepath)
{
    auto path = nemesis::to_lower_copy(filepath);
    auto itr  = BehaviorMap.find(path);

    if (itr == BehaviorMap.end()) return nullptr;

    return itr->second;
}

const nemesis::HkxBehavior*
nemesis::MasterRepository::GetBehaviorByPath(const std::filesystem::path& filepath) const
{
    auto path = nemesis::to_lower_copy(filepath);
    auto itr  = BehaviorMap.find(path);

    if (itr == BehaviorMap.end()) return nullptr;

    return itr->second;
}

Vec<nemesis::HkxBehavior*> nemesis::MasterRepository::GetBehaviorList()
{
    Vec<nemesis::HkxBehavior*> behavior_list;

    for (auto& behavior : BehaviorList)
    {
        behavior_list.emplace_back(behavior.get());
    }

    return behavior_list;
}

Vec<const nemesis::HkxBehavior*> nemesis::MasterRepository::GetBehaviorList() const
{
    Vec<const nemesis::HkxBehavior*> behavior_list;

    for (auto& behavior : BehaviorList)
    {
        behavior_list.emplace_back(behavior.get());
    }

    return behavior_list;
}

void nemesis::MasterRepository::CompileAllBehaviors() const
{
    for (auto& behavior : BehaviorMap)
    {
        nemesis::CompileState state(RequestRepository);
        sf::path behavior_path(behavior.second->GetFilePath());
        sf::path hkx_path(behavior_path.parent_path()
                          / (sf::path(behavior.first).stem().wstring() + L".hkx"));

        if (behavior.second->IsSameAsCached(state))
        {
            sf::copy_file(
                behavior.second->GetCachedFilePath(), hkx_path, sf::copy_options::overwrite_existing);
            return;
        }

        DeqNstr lines = behavior.second->Compile(state);

        sf::path xml_path(behavior_path.parent_path() / (nemesis::generate_guid_w() + L".xml"));
        std::ofstream output_file(xml_path);

        for (auto& line : lines)
        {
            output_file << line.ToString() << "\n";
        }

        output_file.close();

        HkxCompiler::ConvertToHkx(xml_path, hkx_path);

        sf::remove(xml_path);
    }
}

void nemesis::MasterRepository::CompileAllBehaviors(nemesis::ThreadPool& tp) const
{
    for (auto& behavior : BehaviorMap)
    {
        tp.enqueue(
            [this, &behavior]
            {
                nemesis::CompileState state(RequestRepository);
                sf::path behavior_path(behavior.second->GetFilePath());
                sf::path hkx_path(behavior_path.parent_path()
                                  / (sf::path(behavior.first).stem().wstring() + L".hkx"));

                if (behavior.second->IsSameAsCached(state))
                {
                    sf::copy_file(
                        behavior.second->GetCachedFilePath(), hkx_path, sf::copy_options::overwrite_existing);
                    return;
                }

                DeqNstr lines = behavior.second->Compile(state);

                sf::path xml_path(behavior_path.parent_path()
                                  / (nemesis::generate_guid_w() + L".xml"));
                std::ofstream output_file(xml_path);

                for (auto& line : lines)
                {
                    output_file << line.ToString() << "\n";
                }

                output_file.close();

                HkxCompiler::ConvertToHkx(xml_path, hkx_path);

                sf::remove(xml_path);
            });
    }
}

void nemesis::MasterRepository::AddTemplateClass(UPtr<nemesis::TemplateClass>&& templt_class)
{
    auto* templt_class_ptr = TemplateClasses.emplace_back(std::move(templt_class)).get();

    for (size_t i = 0; i < templt_class_ptr->GetSize(); ++i)
    {
        auto templt    = templt_class_ptr->GetTemplate(i);
        auto* behavior = GetBehaviorByPath(templt->GetTargetPath());
        
        if (!behavior) throw std::runtime_error("Missing based master objpect");

        behavior->AddTemplate(templt);
    }
}

nemesis::TemplateClass* nemesis::MasterRepository::GetTemplateClassByName(const std::string& name)
{
    for (auto& templt_class : TemplateClasses)
    {
        if (templt_class->GetName() == name) return templt_class.get();
    }

    return nullptr;
}

const nemesis::TemplateClass* nemesis::MasterRepository::GetTemplateClassByName(const std::string& name) const
{
    for (auto& templt_class : TemplateClasses)
    {
        if (templt_class->GetName() == name) return templt_class.get();
    }

    return nullptr;
}

Vec<nemesis::TemplateClass*> nemesis::MasterRepository::GetTemplateClassList()
{
    Vec<nemesis::TemplateClass*> list;

    for (auto& templt_class : TemplateClasses)
    {
        list.emplace_back(templt_class.get());
    }

    return list;
}

Vec<const nemesis::TemplateClass*> nemesis::MasterRepository::GetTemplateClassList() const
{
    Vec<const nemesis::TemplateClass*> list;

    for (auto& templt_class : TemplateClasses)
    {
        list.emplace_back(templt_class.get());
    }

    return list;
}

nemesis::AnimationRequestRepository& nemesis::MasterRepository::GetAnimationRequestRepository()
{
    return RequestRepository;
}

const nemesis::AnimationRequestRepository& nemesis::MasterRepository::GetAnimationRequestRepository() const
{
    return RequestRepository;
}
