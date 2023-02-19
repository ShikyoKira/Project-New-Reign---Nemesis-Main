#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
	namespace animdata
    {
        struct AnimDataInfo
        {
            nemesis::LinkedPreprocessLine Name;
            nemesis::LinkedPreprocessLine UniqueCode;
            nemesis::LinkedPreprocessLine PlaybackSpeed;
            nemesis::LinkedPreprocessLine CropStartAmountLocalTime;
            nemesis::LinkedPreprocessLine CropEndAmountLocalTime;

            Vec<SPtr<nemesis::LinkedPreprocessLine>> EventNames;
        };
    }
}
