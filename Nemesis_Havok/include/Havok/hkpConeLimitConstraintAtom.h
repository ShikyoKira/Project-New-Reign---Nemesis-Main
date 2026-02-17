#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpConeLimitConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xf19443c8,
                                                "hkpConeLimitConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                20,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpConeLimitConstraintAtom);

        using MeasurementMode = nemesis::hkInternalClassEnumItem<unsigned char>;

        unsigned char m_isEnabled{};
        unsigned char m_twistAxisInA{};
        unsigned char m_refAxisInB{};
        nemesis::hkEnum<MeasurementMode, unsigned char> m_angleMeasurementMode = {
            {"ZERO_WHEN_VECTORS_ALIGNED", 0},
            {"ZERO_WHEN_VECTORS_PERPENDICULAR", 1},
        };
        unsigned char m_memOffsetToAngleOffset{};
        float m_minAngle{};
        float m_maxAngle{};
        float m_angularLimitsTauFactor{};

    public:
        hkpConeLimitConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConeLimitConstraintAtom::Class;
}
