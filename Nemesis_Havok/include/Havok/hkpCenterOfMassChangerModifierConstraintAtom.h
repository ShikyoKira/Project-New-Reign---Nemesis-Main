#pragma once

#include "Havok/hkpModifierConstraintAtom.h"

namespace nemesis
{
    struct hkpCenterOfMassChangerModifierConstraintAtom : nemesis::hkpModifierConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x1d7dbdd2,
                                                "hkpCenterOfMassChangerModifierConstraintAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpCenterOfMassChangerModifierConstraintAtom);

        nemesis::hkVector4 m_displacementA;
        nemesis::hkVector4 m_displacementB;

    public:
        hkpCenterOfMassChangerModifierConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCenterOfMassChangerModifierConstraintAtom::Class;
}
