#pragma once

#include "Havok/hkpModifierConstraintAtom.h"

namespace nemesis
{
    struct hkpMassChangerModifierConstraintAtom : nemesis::hkpModifierConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xb6b28240,
                                                "hkpMassChangerModifierConstraintAtom",
                                                &nemesis::hkpModifierConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMassChangerModifierConstraintAtom);

        nemesis::hkVector4 m_factorA;
        nemesis::hkVector4 m_factorB;

    public:
        hkpMassChangerModifierConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMassChangerModifierConstraintAtom::Class;
}
