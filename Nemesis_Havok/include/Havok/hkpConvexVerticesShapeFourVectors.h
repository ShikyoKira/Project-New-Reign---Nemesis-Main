#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpConvexVerticesShapeFourVectors : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x3d80c5bf,
                                                "hkpConvexVerticesShapeFourVectors",
                                                nullptr,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexVerticesShapeFourVectors);

        nemesis::hkVector4 m_x;
        nemesis::hkVector4 m_y;
        nemesis::hkVector4 m_z;

    public:
        hkpConvexVerticesShapeFourVectors() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexVerticesShapeFourVectors::Class;
}
