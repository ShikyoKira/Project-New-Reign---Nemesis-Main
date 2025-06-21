#pragma once

#include "Havok/Base/hkpHeightFieldShape.h"

namespace nemesis
{
    struct hkpPlaneShape : nemesis::hkpHeightFieldShape
    {
        static constexpr nemesis::hkClass Class{0xc36bbd30,
                                                "hkpPlaneShape",
                                                &nemesis::hkpHeightFieldShape::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPlaneShape);

        nemesis::hkVector4 m_plane;
        nemesis::hkVector4 m_aabbCenter;
        nemesis::hkVector4 m_aabbHalfExtents;

    public:
        hkpPlaneShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPlaneShape::Class;
}
