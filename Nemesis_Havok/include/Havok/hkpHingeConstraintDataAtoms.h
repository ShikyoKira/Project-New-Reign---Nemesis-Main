#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpSetupStabilizationAtom.h"
#include "Havok/hkp2dAngConstraintAtom.h"
#include "Havok/hkpBallSocketConstraintAtom.h"

namespace nemesis
{
    struct hkpHingeConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x6958371c,
                                                "hkpHingeConstraintDataAtoms",
                                                nullptr,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpHingeConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpSetupStabilizationAtom m_setupStabilization;
        nemesis::hkp2dAngConstraintAtom m_2dAng;
        nemesis::hkpBallSocketConstraintAtom m_ballSocket;

    public:
        hkpHingeConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpHingeConstraintDataAtoms::Class;
}
