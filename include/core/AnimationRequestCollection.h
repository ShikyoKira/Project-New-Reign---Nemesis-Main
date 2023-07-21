#pragma once

#include "utilities/types.h"

#include "core/AnimationRequest.h"

namespace nemesis
{
    struct AnimationRequestCollection : public Vec<UPtr<nemesis::AnimationRequest>>
    {
    };
}
