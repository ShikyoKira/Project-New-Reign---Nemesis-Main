#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpSetupStabilizationAtom.h"
#include "Havok/hkpAngMotorConstraintAtom.h"
#include "Havok/hkpAngFrictionConstraintAtom.h"
#include "Havok/hkpAngLimitConstraintAtom.h"
#include "Havok/hkp2dAngConstraintAtom.h"
#include "Havok/hkpBallSocketConstraintAtom.h"

namespace nemesis
{
    struct hkpLimitedHingeConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x54c7715b,
                                                "hkpLimitedHingeConstraintDataAtoms",
                                                nullptr,
                                                240,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpLimitedHingeConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpSetupStabilizationAtom m_setupStabilization;
        nemesis::hkpAngMotorConstraintAtom m_angMotor;
        nemesis::hkpAngFrictionConstraintAtom m_angFriction;
        nemesis::hkpAngLimitConstraintAtom m_angLimit;
        nemesis::hkp2dAngConstraintAtom m_2dAng;
        nemesis::hkpBallSocketConstraintAtom m_ballSocket;

    public:
        hkpLimitedHingeConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLimitedHingeConstraintDataAtoms::Class;
}
