#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbHandIkModifierHand : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x14dfe1dd,
                                                "hkbHandIkModifierHand",
                                                nullptr,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbHandIkModifierHand);

        nemesis::hkVector4 m_elbowAxisLS;
        nemesis::hkVector4 m_backHandNormalLS;
        nemesis::hkVector4 m_handOffsetLS;
        nemesis::hkQuaternion m_handOrienationOffsetLS;
        float m_maxElbowAngleDegrees;
        float m_minElbowAngleDegrees;
        short m_shoulderIndex;
        short m_shoulderSiblingIndex;
        short m_elbowIndex;
        short m_elbowSiblingIndex;
        short m_wristIndex;
        bool m_enforceEndPosition;
        bool m_enforceEndRotation;
        nemesis::hkStringPtr m_localFrameName;

    public:
        hkbHandIkModifierHand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandIkModifierHand::Class;
}
