#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpRackAndPinionConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x30cae006,
                                                "hkpRackAndPinionConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                12,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRackAndPinionConstraintAtom);

        float m_pinionRadiusOrScrewPitch{};
        bool m_isScrew{};
        char m_memOffsetToInitialAngleOffset{};
        char m_memOffsetToPrevAngle{};
        char m_memOffsetToRevolutionCounter{};

    public:
        hkpRackAndPinionConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRackAndPinionConstraintAtom::Class;
}
