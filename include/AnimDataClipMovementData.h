#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
    namespace animdata
    {
        struct AnimDataClipMovementData
        {
            int code;
            SPtr<nemesis::LinkedPreprocessLine> duration;
            Vec<SPtr<nemesis::LinkedPreprocessLine>> movementlist;
            Vec<SPtr<nemesis::LinkedPreprocessLine>> rotationlist;
        };
    }
}
