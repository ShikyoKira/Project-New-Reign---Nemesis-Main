#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxVertexVectorDataChannel : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x2ea63179,
                                                "hkxVertexVectorDataChannel",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexVectorDataChannel);

        mutable nemesis::hkArray<nemesis::hkVector4> m_perVertexVectors_0;
        mutable nemesis::hkArray<float> m_perVertexVectors_1;

    public:
        hkxVertexVectorDataChannel() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexVectorDataChannel::Class;
}
