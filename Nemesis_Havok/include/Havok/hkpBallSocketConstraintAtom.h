#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpBallSocketConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xe70e4dfa,
                                                "hkpBallSocketConstraintAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpBallSocketConstraintAtom);

        using SolvingMethod = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<SolvingMethod, unsigned char> m_solvingMethod = {
            {"METHOD_STABILIZED", 0},
            {"METHOD_OLD", 1},
        };
        unsigned char m_bodiesToNotify{};
        nemesis::hkUFloat8 m_velocityStabilizationFactor;
        bool m_enableLinearImpulseLimit{};
        float m_maxImpulse{};
        float m_inertiaStabilizationFactor{};

    public:
        hkpBallSocketConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBallSocketConstraintAtom::Class;
}
