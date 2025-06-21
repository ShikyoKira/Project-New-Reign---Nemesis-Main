#pragma once

#include "Havok/hkpMeshShape.h"

namespace nemesis
{
    struct hkpFastMeshShape : nemesis::hkpMeshShape
    {
        static constexpr nemesis::hkClass Class{0x3d3da311,
                                                "hkpFastMeshShape",
                                                &nemesis::hkpMeshShape::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpFastMeshShape);

    public:
        hkpFastMeshShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpFastMeshShape::Class;
}
