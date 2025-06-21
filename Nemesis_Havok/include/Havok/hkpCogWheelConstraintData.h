#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpCogWheelConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpCogWheelConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x7f0e53fc,
                                                "hkpCogWheelConstraintData",
                                                &nemesis::hkpConstraintData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpCogWheelConstraintData);

        nemesis::hkpCogWheelConstraintDataAtoms m_atoms; // align 16

    public:
        hkpCogWheelConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCogWheelConstraintData::Class;
}
