#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpRotationalConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpRotationalConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x74867d9e,
                                                "hkpRotationalConstraintData",
                                                &nemesis::hkpConstraintData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpRotationalConstraintData);

        nemesis::hkpRotationalConstraintDataAtoms m_atoms; // align 16

    public:
        hkpRotationalConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRotationalConstraintData::Class;
}
