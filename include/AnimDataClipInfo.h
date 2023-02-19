#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
	namespace animdata
    {
        struct AnimDataClipInfo
        {
            SPtr<nemesis::LinkedPreprocessLine> name;
            int code;
            SPtr<nemesis::LinkedPreprocessLine> playbackspeed;
            SPtr<nemesis::LinkedPreprocessLine> cropstart;
            SPtr<nemesis::LinkedPreprocessLine> cropend;
            Vec<SPtr<nemesis::LinkedPreprocessLine>> events;
        };
    }
}
