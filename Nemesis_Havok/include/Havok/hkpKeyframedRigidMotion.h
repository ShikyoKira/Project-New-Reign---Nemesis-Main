#pragma once

#include "Havok/Base/hkpMotion.h"

namespace nemesis
{
    struct hkpKeyframedRigidMotion : nemesis::hkpMotion
    {
        static constexpr nemesis::hkClass Class{0xbafa2bb7,
                                                "hkpKeyframedRigidMotion",
                                                &nemesis::hkpMotion::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpKeyframedRigidMotion);

    public:
        hkpKeyframedRigidMotion() noexcept;
        hkpKeyframedRigidMotion(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpKeyframedRigidMotion::Class;
}
