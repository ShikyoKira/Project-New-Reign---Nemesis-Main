#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpLinFrictionConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x3e94ef7c,
                                                "hkpLinFrictionConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpLinFrictionConstraintAtom);

        unsigned char m_isEnabled{};
        unsigned char m_frictionAxis{};
        float m_maxFrictionForce{};

    public:
        hkpLinFrictionConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLinFrictionConstraintAtom::Class;
}
