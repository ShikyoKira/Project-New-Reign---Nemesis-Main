#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpCogWheelConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xf2b1f399,
                                                "hkpCogWheelConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCogWheelConstraintAtom);

        float m_cogWheelRadiusA;
        float m_cogWheelRadiusB;
        bool m_isScrew;
        char m_memOffsetToInitialAngleOffset;
        char m_memOffsetToPrevAngle;
        char m_memOffsetToRevolutionCounter;

    public:
        hkpCogWheelConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCogWheelConstraintAtom::Class;
}
