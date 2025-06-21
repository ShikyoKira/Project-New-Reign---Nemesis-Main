#pragma once

#include "Havok/hkpSetLocalRotationsConstraintAtom.h"
#include "Havok/hkpAngLimitConstraintAtom.h"
#include "Havok/hkp2dAngConstraintAtom.h"

namespace nemesis
{
    struct hkpHingeLimitsDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x555876ff,
                                                "hkpHingeLimitsDataAtoms",
                                                nullptr,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpHingeLimitsDataAtoms);

        nemesis::hkpSetLocalRotationsConstraintAtom m_rotations;
        nemesis::hkpAngLimitConstraintAtom m_angLimit;
        nemesis::hkp2dAngConstraintAtom m_2dAng;

    public:
        hkpHingeLimitsDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpHingeLimitsDataAtoms::Class;
}
