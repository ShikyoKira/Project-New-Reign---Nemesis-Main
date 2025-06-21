#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpWheelConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpWheelConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0xb4c46671,
                                                "hkpWheelConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                368,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpWheelConstraintData);

        nemesis::hkpWheelConstraintDataAtoms m_atoms; // align 16
        nemesis::hkVector4 m_initialAxleInB;
        nemesis::hkVector4 m_initialSteeringAxisInB;

    public:
        hkpWheelConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWheelConstraintData::Class;
}
