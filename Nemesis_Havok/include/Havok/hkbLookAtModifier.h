#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbLookAtModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x3d28e066,
                                                "hkbLookAtModifier",
                                                &nemesis::hkbModifier::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbLookAtModifier);

        nemesis::hkVector4 m_targetWS;
        nemesis::hkVector4 m_headForwardLS;
        nemesis::hkVector4 m_neckForwardLS;
        nemesis::hkVector4 m_neckRightLS;
        nemesis::hkVector4 m_eyePositionHS;
        float m_newTargetGain;
        float m_onGain;
        float m_offGain;
        float m_limitAngleDegrees;
        float m_limitAngleLeft;
        float m_limitAngleRight;
        float m_limitAngleUp;
        float m_limitAngleDown;
        short m_headIndex;
        short m_neckIndex;
        bool m_isOn;
        bool m_individualLimitsOn;
        bool m_isTargetInsideLimitCone;
        nemesis::hkVector4 m_lookAtLastTargetWS;
        float m_lookAtWeight;

    public:
        hkbLookAtModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbLookAtModifier::Class;
}
