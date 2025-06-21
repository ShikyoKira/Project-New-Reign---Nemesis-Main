#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpStiffSpringConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpStiffSpringConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0xb98f66f4,
                                                "hkpStiffSpringConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpStiffSpringConstraintData);

        nemesis::hkpStiffSpringConstraintDataAtoms m_atoms; // align 16

    public:
        hkpStiffSpringConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStiffSpringConstraintData::Class;
}
