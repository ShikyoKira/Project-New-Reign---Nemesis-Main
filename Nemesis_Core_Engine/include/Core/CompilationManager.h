#pragma once

#include <mutex>

#include "Core/Template.h"
#include "Core/CompileState.h"

#include "ExAnimation/ExAnimationRepository.h"

#include "AlternateAnimation/AlterAnimRepository.h"

namespace nemesis
{
	struct CompilationManager
    {
    private:
        const nemesis::AlterAnimRepository& AlterAnimationRepository;
        const nemesis::AnimationRequestRepository& AnimationRepository;
        const nemesis::TemplateRepository& TemplateRepository;
        const nemesis::ExAnimationRepository& ExAnimRepository;

        UMap<std::filesystem::path, nemesis::CompileState> StateList;
        VecStr SelectedMods;
        
        Map<std::string, std::string> CheckSumMap;
        std::mutex CheckSumMutex;

        std::mutex CreateMutex;

    public:
        CompilationManager(const VecStr& selected_mods,
                           const nemesis::AlterAnimRepository& alter_anim_repo,
                           const nemesis::AnimationRequestRepository& anim_repo,
                           const nemesis::TemplateRepository& templt_repo,
                           const nemesis::ExAnimationRepository& ex_anim_repo);

        bool IsModSelected(const std::string& modcode) const;
        bool IsModSelected(const std::string_view& modcode) const;
        const VecStr& GetSelectedMods() const noexcept;

        nemesis::CompileState& CreateCompileState(const std::filesystem::path& filepath);
        nemesis::CompileState* GetCompileState(const std::filesystem::path& filepath);

        const nemesis::AlterAnimRepository& GetAlterAnimRepository() const noexcept;
        const nemesis::AnimationRequestRepository& GetAnimationRepository() const noexcept;
        const nemesis::TemplateRepository& GetTemplateRepository() const noexcept;
        const nemesis::ExAnimationRepository& GetExAnimationRepository() const noexcept;

        void AddCheckSum(const std::filesystem::path& target_path, const std::string& checksum);
        void ClearCheckSum() noexcept;
        size_t GetFullCheckSum() const noexcept;
    };
}
