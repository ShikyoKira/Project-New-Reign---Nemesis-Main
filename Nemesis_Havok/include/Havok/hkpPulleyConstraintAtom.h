#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpPulleyConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x94a08848,
                                                "hkpPulleyConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPulleyConstraintAtom);

        nemesis::hkVector4 m_fixedPivotAinWorld;
        nemesis::hkVector4 m_fixedPivotBinWorld;
        float m_ropeLength{};
        float m_leverageOnBodyB{};

    public:
        hkpPulleyConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPulleyConstraintAtom::Class;
}
