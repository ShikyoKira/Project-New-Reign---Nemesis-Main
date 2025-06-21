#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpLimitedHingeConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpLimitedHingeConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x7c15bb6b,
                                                "hkpLimitedHingeConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                272,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpLimitedHingeConstraintData);

        nemesis::hkpLimitedHingeConstraintDataAtoms m_atoms; // align 16

    public:
        hkpLimitedHingeConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLimitedHingeConstraintData::Class;
}
