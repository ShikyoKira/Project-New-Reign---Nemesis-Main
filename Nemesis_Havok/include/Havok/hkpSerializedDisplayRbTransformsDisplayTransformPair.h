#pragma once

#include "Havok/hkpRigidBody.h"

namespace nemesis
{
    struct hkpSerializedDisplayRbTransformsDisplayTransformPair : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x94ac5bec,
                                                "hkpSerializedDisplayRbTransformsDisplayTransformPair",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedDisplayRbTransformsDisplayTransformPair);

        nemesis::hkRefPtr<nemesis::hkpRigidBody> m_rb;
        nemesis::hkTransform m_localToDisplay;

    public:
        hkpSerializedDisplayRbTransformsDisplayTransformPair() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair::Class;
}
