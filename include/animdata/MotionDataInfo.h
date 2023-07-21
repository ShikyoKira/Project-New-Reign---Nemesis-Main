#pragma once

#include "core/NLine.h"

namespace nemesis
{
	namespace animdata
    {
        struct MotionDataInfo : public nemesis::NObject
        {
        protected:
            nemesis::NLine UniqueCode;
            nemesis::NLine Duration;

            Vec<UPtr<nemesis::NLine>> CoordinationData;
            Vec<UPtr<nemesis::NLine>> RotationData;

        public:
            MotionDataInfo() = default;

            void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
            void SerializeTo(DeqNstr& lines) const override;
        };
    }
}
