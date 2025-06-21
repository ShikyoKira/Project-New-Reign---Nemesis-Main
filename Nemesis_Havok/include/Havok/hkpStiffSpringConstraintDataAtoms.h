#pragma once

#include "Havok/hkpSetLocalTranslationsConstraintAtom.h"
#include "Havok/hkpSetupStabilizationAtom.h"
#include "Havok/hkpStiffSpringConstraintAtom.h"

namespace nemesis
{
    struct hkpStiffSpringConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x207eb376,
                                                "hkpStiffSpringConstraintDataAtoms",
                                                nullptr,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpStiffSpringConstraintDataAtoms);

        nemesis::hkpSetLocalTranslationsConstraintAtom m_pivots;
        nemesis::hkpSetupStabilizationAtom m_setupStabilization;
        nemesis::hkpStiffSpringConstraintAtom m_spring;

    public:
        hkpStiffSpringConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStiffSpringConstraintDataAtoms::Class;
}
