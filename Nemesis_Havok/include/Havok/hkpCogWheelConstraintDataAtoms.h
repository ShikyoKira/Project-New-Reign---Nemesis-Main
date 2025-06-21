#pragma once

#include "Havok/hkpCogWheelConstraintAtom.h"
#include "Havok/hkpSetLocalTransformsConstraintAtom.h"

namespace nemesis
{
    struct hkpCogWheelConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf855ba44,
                                                "hkpCogWheelConstraintDataAtoms",
                                                nullptr,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCogWheelConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpCogWheelConstraintAtom m_cogWheels;

    public:
        hkpCogWheelConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCogWheelConstraintDataAtoms::Class;
}
