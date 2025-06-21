#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpRackAndPinionConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpRackAndPinionConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0xd180ebe0,
                                                "hkpRackAndPinionConstraintData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpRackAndPinionConstraintData);

        nemesis::hkpRackAndPinionConstraintDataAtoms m_atoms; // align 16

    public:
        hkpRackAndPinionConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRackAndPinionConstraintData::Class;
}
