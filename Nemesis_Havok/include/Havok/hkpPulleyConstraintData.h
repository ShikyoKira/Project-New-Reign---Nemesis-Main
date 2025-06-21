#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpPulleyConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpPulleyConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x972058ed,
                                                "hkpPulleyConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPulleyConstraintData);

        nemesis::hkpPulleyConstraintDataAtoms m_atoms; // align 16

    public:
        hkpPulleyConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPulleyConstraintData::Class;
}
