#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpLinLimitConstraintAtom.h"
#include "Havok/hkpLinSoftConstraintAtom.h"
#include "Havok/hkpLinConstraintAtom.h"
#include "Havok/hkpSetLocalRotationsConstraintAtom.h"
#include "Havok/hkp2dAngConstraintAtom.h"

namespace nemesis
{
    struct hkpWheelConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x1188cbe1,
                                                "hkpWheelConstraintDataAtoms",
                                                nullptr,
                                                304,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpWheelConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_suspensionBase;
        nemesis::hkpLinLimitConstraintAtom m_lin0Limit;
        nemesis::hkpLinSoftConstraintAtom m_lin0Soft;
        nemesis::hkpLinConstraintAtom m_lin1;
        nemesis::hkpLinConstraintAtom m_lin2;
        nemesis::hkpSetLocalRotationsConstraintAtom m_steeringBase;
        nemesis::hkp2dAngConstraintAtom m_2dAng;

    public:
        hkpWheelConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWheelConstraintDataAtoms::Class;
}
