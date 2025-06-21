#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxAnimatedMatrix : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x5838e337,
                                                "hkxAnimatedMatrix",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxAnimatedMatrix);

        using Hint = nemesis::hkInternalClassEnumItem<unsigned char>;

        mutable nemesis::hkArray<nemesis::hkMatrix4> m_matrices_0;
        mutable nemesis::hkArray<float> m_matrices_1;
        nemesis::hkEnum<Hint, unsigned char> m_hint = {
            {"HINT_NONE", 0},
            {"HINT_IGNORE", 1},
            {"HINT_TRANSFORM", 2},
            {"HINT_SCALE", 4},
            {"HINT_TRANSFORM_AND_SCALE", 6},
            {"HINT_FLIP", 8},
        };

        static void Combine(nemesis::hkArray<float>& matrices, const nemesis::hkVector4& vec4);
        static size_t
        Combine(nemesis::hkVector4& vec4, const nemesis::hkArray<float>& matrices, size_t index);

    public:
        hkxAnimatedMatrix() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxAnimatedMatrix::Class;
}
