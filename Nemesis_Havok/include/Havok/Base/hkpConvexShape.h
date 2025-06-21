#pragma once

#include "Havok/Base/hkpSphereRepShape.h"

namespace nemesis
{
    struct hkpConvexShape : nemesis::hkpSphereRepShape
    {
        static constexpr nemesis::hkClass Class{0xf8f74f85,
                                                "hkpConvexShape",
                                                &nemesis::hkpSphereRepShape::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        float m_radius;

    public:
        hkpConvexShape(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexShape::Class;
}
