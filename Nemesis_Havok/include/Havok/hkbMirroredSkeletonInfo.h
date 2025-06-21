#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbMirroredSkeletonInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc6c2da4f,
                                                "hkbMirroredSkeletonInfo",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbMirroredSkeletonInfo);

        nemesis::hkVector4 m_mirrorAxis;
        nemesis::hkArray<short> m_bonePairMap;
        nemesis::hkArray<short> m_partitionPairMap;

    public:
        hkbMirroredSkeletonInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbMirroredSkeletonInfo::Class;
}
