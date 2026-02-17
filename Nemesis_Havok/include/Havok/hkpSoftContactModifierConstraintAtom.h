#pragma once

#include "Havok/hkpModifierConstraintAtom.h"

namespace nemesis
{
    struct hkpSoftContactModifierConstraintAtom : nemesis::hkpModifierConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xecb34e27,
                                                "hkpSoftContactModifierConstraintAtom",
                                                &nemesis::hkpModifierConstraintAtom::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSoftContactModifierConstraintAtom);

        float m_tau{};
        float m_maxAcceleration{};

    public:
        hkpSoftContactModifierConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSoftContactModifierConstraintAtom::Class;
}
