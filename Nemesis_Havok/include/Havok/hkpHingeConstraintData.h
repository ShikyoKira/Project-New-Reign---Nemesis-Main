#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpHingeConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpHingeConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x9590f046,
                                                "hkpHingeConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                224,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpHingeConstraintData);

        nemesis::hkpHingeConstraintDataAtoms m_atoms; // align 16

    public:
        hkpHingeConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpHingeConstraintData::Class;
}
