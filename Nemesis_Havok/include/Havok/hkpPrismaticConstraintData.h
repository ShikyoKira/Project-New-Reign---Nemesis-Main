#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpPrismaticConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpPrismaticConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x3996c387,
                                                "hkpPrismaticConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                240,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPrismaticConstraintData);

        nemesis::hkpPrismaticConstraintDataAtoms m_atoms; // align 16

    public:
        hkpPrismaticConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPrismaticConstraintData::Class;
}
