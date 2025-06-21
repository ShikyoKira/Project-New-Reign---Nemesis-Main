#pragma once

#include "Havok/Base/hkpPhantom.h"

#include "Havok/hkMotionState.h"

namespace nemesis
{
    struct hkpShapePhantom : nemesis::hkpPhantom
    {
        static constexpr nemesis::hkClass Class{0xcb22fbcd,
                                                "hkpShapePhantom",
                                                &nemesis::hkpPhantom::Class,
                                                416,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkMotionState m_motionState;

    public:
        hkpShapePhantom(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpShapePhantom::Class;
}
