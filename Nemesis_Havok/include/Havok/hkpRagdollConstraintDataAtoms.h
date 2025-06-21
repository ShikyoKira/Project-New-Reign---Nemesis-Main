#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpSetupStabilizationAtom.h"
#include "Havok/hkpRagdollMotorConstraintAtom.h"
#include "Havok/hkpAngFrictionConstraintAtom.h"
#include "Havok/hkpTwistLimitConstraintAtom.h"
#include "Havok/hkpConeLimitConstraintAtom.h"
#include "Havok/hkpBallSocketConstraintAtom.h"

namespace nemesis
{
    struct hkpRagdollConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xeed76b00,
                                                "hkpRagdollConstraintDataAtoms",
                                                nullptr,
                                                352,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRagdollConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpSetupStabilizationAtom m_setupStabilization;
        nemesis::hkpRagdollMotorConstraintAtom m_ragdollMotors;
        nemesis::hkpAngFrictionConstraintAtom m_angFriction;
        nemesis::hkpTwistLimitConstraintAtom m_twistLimit;
        nemesis::hkpConeLimitConstraintAtom m_coneLimit;
        nemesis::hkpConeLimitConstraintAtom m_planesLimit;
        nemesis::hkpBallSocketConstraintAtom m_ballSocket;

    public:
        hkpRagdollConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRagdollConstraintDataAtoms::Class;
}
