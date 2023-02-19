#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
	namespace animdata
    {
        struct MotionDataInfo
        {
            nemesis::LinkedPreprocessLine UniqueCode;
            nemesis::LinkedPreprocessLine Duration;

            Vec<SPtr<nemesis::LinkedPreprocessLine>> CoordinationData;
            Vec<SPtr<nemesis::LinkedPreprocessLine>> RotationData;
        };
    }
}
