#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxAnimatedQuaternion : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xb4f01baa,
                                                "hkxAnimatedQuaternion",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxAnimatedQuaternion);

        mutable nemesis::hkArray<nemesis::hkQuaternion> m_quaternions_0;
        mutable nemesis::hkArray<float> m_quaternions_1;

        static void Combine(nemesis::hkArray<float>& quaternions, const nemesis::hkQuaternion& qrtn);
        static size_t
        Combine(nemesis::hkQuaternion& qrtn, const nemesis::hkArray<float>& quaternions, size_t index);

    public:
        hkxAnimatedQuaternion() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxAnimatedQuaternion::Class;
}
