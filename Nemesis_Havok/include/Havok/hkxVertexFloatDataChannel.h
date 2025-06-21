#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxVertexFloatDataChannel : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xbeeb397c,
                                                "hkxVertexFloatDataChannel",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexFloatDataChannel);

        using VertexFloatDimensions = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkArray<float> m_perVertexFloats;
        nemesis::hkEnum<VertexFloatDimensions, unsigned char> m_dimensions = {
            {"FLOAT", 0},
            {"DISTANCE", 1},
            {"ANGLE", 2},
        };

    public:
        hkxVertexFloatDataChannel() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexFloatDataChannel::Class;
}
