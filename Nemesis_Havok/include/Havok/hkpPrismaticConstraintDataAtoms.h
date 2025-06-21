#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpLinMotorConstraintAtom.h"
#include "Havok/hkpLinFrictionConstraintAtom.h"
#include "Havok/hkpAngConstraintAtom.h"
#include "Havok/hkpLinConstraintAtom.h"
#include "Havok/hkpLinLimitConstraintAtom.h"

namespace nemesis
{
    struct hkpPrismaticConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x7f516137,
                                                "hkpPrismaticConstraintDataAtoms",
                                                nullptr,
                                                208,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPrismaticConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpLinMotorConstraintAtom m_motor;
        nemesis::hkpLinFrictionConstraintAtom m_friction;
        nemesis::hkpAngConstraintAtom m_ang;
        nemesis::hkpLinConstraintAtom m_lin0;
        nemesis::hkpLinConstraintAtom m_lin1;
        nemesis::hkpLinLimitConstraintAtom m_linLimit;

    public:
        hkpPrismaticConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPrismaticConstraintDataAtoms::Class;
}
