#pragma once

#include "Havok/Base/hkpSphereRepShape.h"

namespace nemesis
{
    struct hkpMultiSphereShape : nemesis::hkpSphereRepShape
    {
        static constexpr nemesis::hkClass Class{0x61a590fc,
                                                "hkpMultiSphereShape",
                                                &nemesis::hkpSphereRepShape::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMultiSphereShape);

        int m_numSpheres;
        std::array<nemesis::hkVector4, 8> m_spheres;

    public:
        hkpMultiSphereShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMultiSphereShape::Class;
}
