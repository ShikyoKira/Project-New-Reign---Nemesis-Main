#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
	namespace animdata
    {
        struct AnimDataMovementData
        {
            std::string code;
            double length;
            Vec<nemesis::LinkedPreprocessLine> position_info;
            Vec<nemesis::LinkedPreprocessLine> direction_info;
        };
    }
}
