#pragma once

#include <mutex>
#include <shared_mutex>
#include <future>

#include "ExAnimation/ExAnimationRequest.h"

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
	struct ExAnimationPack
    {
    private:
        Vec<UPtr<nemesis::ExAnimationRequest>> RequestList;
#if _WIN32
        UMap<std::wstring, const nemesis::ExAnimationRequest*> RequestPathMap;
#else
        UMap<std::string, const nemesis::ExAnimationRequest*> RequestPathMap;
#endif
        unsigned short Order;
        std::string Name;

        mutable UMap<std::filesystem::path, const nemesis::ExAnimationRequest*> ExAnimMap;
        mutable std::shared_mutex ExAnimMapMutex;

        mutable UMap<std::filesystem::path, std::shared_future<const nemesis::ExAnimationRequest*>> OngoingGetter;
        mutable std::shared_mutex OngoingGetterMutex;

    public:
        ExAnimationPack(const std::filesystem::path& pack_dir,
                        const std::filesystem::path& aim_dir,
                        const std::filesystem::path& ex_anim_dir);

        unsigned short GetOrder() const noexcept;
        const std::string& GetName() const noexcept;
        std::string GetVariableName() const noexcept;
        const Vec<UPtr<nemesis::ExAnimationRequest>>& GetRequestList() const noexcept;
        const nemesis::ExAnimationRequest* GetExAnimRequest(const std::filesystem::path& canon_anim_path) const;
    };
}
