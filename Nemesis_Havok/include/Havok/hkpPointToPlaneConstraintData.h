#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpPointToPlaneConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpPointToPlaneConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x65c56e17,
                                                "hkpPointToPlaneConstraintData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPointToPlaneConstraintData);

        nemesis::hkpPointToPlaneConstraintDataAtoms m_atoms; // align 16

    public:
        hkpPointToPlaneConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPointToPlaneConstraintData::Class;
}
