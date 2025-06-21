#pragma once

#include "Havok/Base/hkpConvexShape.h"

namespace nemesis
{
    struct hkpBoxShape : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0x3444d2d5,
                                                "hkpBoxShape",
                                                &nemesis::hkpConvexShape::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBoxShape);

        nemesis::hkVector4 m_halfExtents;

    public:
        hkpBoxShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBoxShape::Class;
}
