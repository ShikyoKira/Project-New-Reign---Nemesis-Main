#include "ExAnimation/ExAnimationRequest.h"

nemesis::ExAnimationRequest::ExAnimationRequest(const nemesis::ExAnimationPack& pack,
                                                const std::filesystem::path& anim_path,
                                                const std::filesystem::path& ex_anim_path)
    : Pack(pack)
    , AnimPath(anim_path)
    , CanonAnimPath(std::filesystem::absolute(anim_path))
    , ExAnimPath(ex_anim_path)
    , CanonExAnimPath(std::filesystem::absolute(ex_anim_path))
{
}

const std::filesystem::path& nemesis::ExAnimationRequest::GetAnimPath() const
{
    return AnimPath;
}

const std::filesystem::path& nemesis::ExAnimationRequest::GetCanonAnimPath() const
{
    return CanonAnimPath;
}

const std::filesystem::path& nemesis::ExAnimationRequest::GetExAnimPath() const
{
    return ExAnimPath;
}

const std::filesystem::path& nemesis::ExAnimationRequest::GetCanonExAnimPath() const
{
    return CanonExAnimPath;
}

const nemesis::ExAnimationPack& nemesis::ExAnimationRequest::GetPack() const
{
    return Pack;
}
