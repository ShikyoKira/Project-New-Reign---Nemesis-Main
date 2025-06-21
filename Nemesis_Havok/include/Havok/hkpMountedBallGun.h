#pragma once

#include "Havok/hkpBallGun.h"

namespace nemesis
{
    struct hkpMountedBallGun : nemesis::hkpBallGun
    {
        static constexpr nemesis::hkClass Class{0x6791ffce,
                                                "hkpMountedBallGun",
                                                &nemesis::hkpBallGun::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMountedBallGun);

        nemesis::hkVector4 m_position;

    public:
        hkpMountedBallGun() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMountedBallGun::Class;
}
