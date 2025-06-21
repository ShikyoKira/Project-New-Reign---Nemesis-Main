#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxAnimatedVector : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x34b1a197,
                                                "hkxAnimatedVector",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxAnimatedVector);

        using Hint = nemesis::hkInternalClassEnumItem<unsigned char>;

        mutable nemesis::hkArray<nemesis::hkVector4> m_vectors_0;
        mutable nemesis::hkArray<float> m_vectors_1;
        nemesis::hkEnum<Hint, unsigned char> m_hint = {
            {"HINT_NONE", 0},
            {"HINT_IGNORE", 1},
            {"HINT_TRANSFORM", 2},
            {"HINT_SCALE", 4},
            {"HINT_TRANSFORM_AND_SCALE", 6},
            {"HINT_FLIP", 8},
        };

    public:
        hkxAnimatedVector() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxAnimatedVector::Class;
}
