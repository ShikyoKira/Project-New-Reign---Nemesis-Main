#pragma once

#include "Havok/hkpSetLocalRotationsConstraintAtom.h"
#include "Havok/hkpAngConstraintAtom.h"

namespace nemesis
{
    struct hkpRotationalConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa0c64586,
                                                "hkpRotationalConstraintDataAtoms",
                                                nullptr,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRotationalConstraintDataAtoms);

        nemesis::hkpSetLocalRotationsConstraintAtom m_rotations;
        nemesis::hkpAngConstraintAtom m_ang;

    public:
        hkpRotationalConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRotationalConstraintDataAtoms::Class;
}
