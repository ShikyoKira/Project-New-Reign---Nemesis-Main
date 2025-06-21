#pragma once

#include <filesystem>

namespace nemesis
{
    struct ExAnimationPack;

	struct ExAnimationRequest
    {
    private:
        const nemesis::ExAnimationPack& Pack;

        std::filesystem::path AnimPath;
        std::filesystem::path CanonAnimPath;

        std::filesystem::path ExAnimPath;
        std::filesystem::path CanonExAnimPath;

    public:
        ExAnimationRequest(const nemesis::ExAnimationPack& pack,
                           const std::filesystem::path& anim_path,
                           const std::filesystem::path& ex_anim_path);

        const std::filesystem::path& GetAnimPath() const;
        const std::filesystem::path& GetCanonAnimPath() const;

        const std::filesystem::path& GetExAnimPath() const;
        const std::filesystem::path& GetCanonExAnimPath() const;

        const nemesis::ExAnimationPack& GetPack() const;


    };
}
