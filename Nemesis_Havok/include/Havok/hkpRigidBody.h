#pragma once

#include "Havok/hkpEntity.h"

namespace nemesis
{
    struct hkpRigidBody : nemesis::hkpEntity
    {
        static constexpr nemesis::hkClass Class{0x75f8d805,
                                                "hkpRigidBody",
                                                &nemesis::hkpEntity::Class,
                                                720,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRigidBody);

    public:
        hkpRigidBody() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRigidBody::Class;
}
