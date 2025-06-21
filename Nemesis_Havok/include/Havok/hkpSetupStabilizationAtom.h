#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpSetupStabilizationAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xf05d137e,
                                                "hkpSetupStabilizationAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSetupStabilizationAtom);

        bool m_enabled;
        float m_maxLinImpulse;
        float m_maxAngImpulse;
        float m_maxAngle;
        std::array<unsigned char, 8> m_padding;

    public:
        hkpSetupStabilizationAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSetupStabilizationAtom::Class;
}
