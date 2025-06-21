#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpRagdollLimitsDataAtoms.h"

namespace nemesis
{
    struct hkpRagdollLimitsData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0xcbdb44aa,
                                                "hkpRagdollLimitsData",
                                                &nemesis::hkpConstraintData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpRagdollLimitsData);

        nemesis::hkpRagdollLimitsDataAtoms m_atoms; // align 16

    public:
        hkpRagdollLimitsData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRagdollLimitsData::Class;
}
