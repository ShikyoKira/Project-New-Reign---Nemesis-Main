#pragma once

#include "Havok/hkpKeyframedRigidMotion.h"

namespace nemesis
{
    struct hkpFixedRigidMotion : nemesis::hkpKeyframedRigidMotion
    {
        static constexpr nemesis::hkClass Class{0x64abf85c,
                                                "hkpFixedRigidMotion",
                                                &nemesis::hkpKeyframedRigidMotion::Class,
                                                320,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpFixedRigidMotion);

    public:
        hkpFixedRigidMotion() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpFixedRigidMotion::Class;
}
