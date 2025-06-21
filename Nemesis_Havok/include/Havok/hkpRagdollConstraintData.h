#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpRagdollConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpRagdollConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x8fb5dd29,
                                                "hkpRagdollConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                384,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRagdollConstraintData);

        nemesis::hkpRagdollConstraintDataAtoms m_atoms; // align 16

    public:
        hkpRagdollConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRagdollConstraintData::Class;
}
