#pragma once

#include "Havok/hkpSetLocalRotationsConstraintAtom.h"
#include "Havok/hkpTwistLimitConstraintAtom.h"
#include "Havok/hkpConeLimitConstraintAtom.h"

namespace nemesis
{
    struct hkpRagdollLimitsDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x82b894c3,
                                                "hkpRagdollLimitsDataAtoms",
                                                nullptr,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRagdollLimitsDataAtoms);

        nemesis::hkpSetLocalRotationsConstraintAtom m_rotations;
        nemesis::hkpTwistLimitConstraintAtom m_twistLimit;
        nemesis::hkpConeLimitConstraintAtom m_coneLimit;
        nemesis::hkpConeLimitConstraintAtom m_planesLimit;

    public:
        hkpRagdollLimitsDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRagdollLimitsDataAtoms::Class;
}
