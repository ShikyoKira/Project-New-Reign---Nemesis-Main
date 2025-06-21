#include "Core/CompilationManager.h"

#include "Utilities/Crc32.h"
#include "Utilities/Algorithm.h"


nemesis::CompilationManager::CompilationManager(const VecStr& selected_mods,
                                                const nemesis::AlterAnimRepository& alter_anim_repo,
                                                const nemesis::AnimationRequestRepository& anim_repo,
                                                const nemesis::TemplateRepository& templt_repo,
                                                const nemesis::ExAnimationRepository& ex_anim_repo)
    : AlterAnimationRepository(alter_anim_repo)
    , AnimationRepository(anim_repo)
    , TemplateRepository(templt_repo)
    , ExAnimRepository(ex_anim_repo)
    , SelectedMods(selected_mods)
{
}

bool nemesis::CompilationManager::IsModSelected(const std::string& modcode) const
{
    for (size_t i = 0; i < SelectedMods.size(); ++i)
    {
        if (modcode == SelectedMods[i]) return true;
    }

    return false;
}

bool nemesis::CompilationManager::IsModSelected(const std::string_view& modcode) const
{
    for (size_t i = 0; i < SelectedMods.size(); ++i)
    {
        if (modcode == SelectedMods[i]) return true;
    }

    return false;
}

const VecStr& nemesis::CompilationManager::GetSelectedMods() const noexcept
{
    return SelectedMods;
}

nemesis::CompileState& nemesis::CompilationManager::CreateCompileState(const std::filesystem::path& filepath)
{
    std::scoped_lock<std::mutex> lock(CreateMutex);

    auto itr = StateList.find(filepath.string());

    if (itr == StateList.end()) return StateList.try_emplace(filepath.string(), *this).first->second;

    throw std::runtime_error("CompileState for file already exist (" + filepath.string() + ")");
}

nemesis::CompileState* nemesis::CompilationManager::GetCompileState(const std::filesystem::path& filepath)
{
    auto itr = StateList.find(filepath.string());

    if (itr != StateList.end()) return &itr->second;

    return nullptr;
}

const nemesis::AlterAnimRepository& nemesis::CompilationManager::GetAlterAnimRepository() const noexcept
{
    return AlterAnimationRepository;
}

const nemesis::AnimationRequestRepository& nemesis::CompilationManager::GetAnimationRepository() const noexcept
{
    return AnimationRepository;
}

const nemesis::TemplateRepository& nemesis::CompilationManager::GetTemplateRepository() const noexcept
{
    return TemplateRepository;
}

const nemesis::ExAnimationRepository& nemesis::CompilationManager::GetExAnimationRepository() const noexcept
{
    return ExAnimRepository;
}

void nemesis::CompilationManager::AddCheckSum(const std::filesystem::path& target_path,
                                              const std::string& checksum)
{
    std::scoped_lock<std::mutex> lock(CheckSumMutex);
    CheckSumMap[nemesis::to_lower_copy(target_path.string())] = checksum;
}

void nemesis::CompilationManager::ClearCheckSum() noexcept
{
    CheckSumMap.clear();
}

size_t nemesis::CompilationManager::GetFullCheckSum() const noexcept
{
    static nemesis::CRC32 crc32;
    std::string checksum;

    for (auto& each : CheckSumMap)
    {
        checksum.append(each.first + "_" + each.second + "\n");
    }

    return crc32.FullCRC(checksum) % 973328672;
}
