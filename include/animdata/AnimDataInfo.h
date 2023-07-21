#pragma once

#include "core/NLine.h"

namespace nemesis
{
	namespace animdata
    {
        struct AnimDataInfo : public nemesis::NObject
        {
        protected:
            nemesis::NLine Name;
            nemesis::NLine UniqueCode;
            nemesis::NLine PlaybackSpeed;
            nemesis::NLine CropStartAmountLocalTime;
            nemesis::NLine CropEndAmountLocalTime;

            Vec<UPtr<nemesis::NLine>> EventNames;

        public:
            AnimDataInfo() = default;

            void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
            void SerializeTo(DeqNstr& lines) const override;
        };
    }
}
