#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpAngLimitConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x9be0d9d,
                                                "hkpAngLimitConstraintAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpAngLimitConstraintAtom);

        unsigned char m_isEnabled;
        unsigned char m_limitAxis;
        float m_minAngle;
        float m_maxAngle;
        float m_angularLimitsTauFactor;

    public:
        hkpAngLimitConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAngLimitConstraintAtom::Class;
}
