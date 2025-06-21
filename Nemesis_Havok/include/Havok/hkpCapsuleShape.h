#pragma once

#include "Havok/Base/hkpConvexShape.h"

namespace nemesis
{
    struct hkpCapsuleShape : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0xdd0b1fd3,
                                                "hkpCapsuleShape",
                                                &nemesis::hkpConvexShape::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCapsuleShape);

        nemesis::hkVector4 m_vertexA;
        nemesis::hkVector4 m_vertexB;

    public:
        hkpCapsuleShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCapsuleShape::Class;
}
