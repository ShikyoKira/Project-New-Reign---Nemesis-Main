#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpConvexVerticesConnectivity : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x63d38e9c,
                                                "hkpConvexVerticesConnectivity",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexVerticesConnectivity);

        nemesis::hkArray<unsigned short> m_vertexIndices;
        nemesis::hkArray<unsigned char> m_numVerticesPerFace;

    public:
        hkpConvexVerticesConnectivity() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexVerticesConnectivity::Class;
}
