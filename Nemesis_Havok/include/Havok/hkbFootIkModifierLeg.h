#pragma once

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbFootIkModifierLeg : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9f3e3a04,
                                                "hkbFootIkModifierLeg",
                                                nullptr,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkModifierLeg);

        nemesis::hkQsTransform m_originalAnkleTransformMS;
        nemesis::hkQuaternion m_prevAnkleRotLS;
        nemesis::hkVector4 m_kneeAxisLS;
        nemesis::hkVector4 m_footEndLS;
        nemesis::hkbEventProperty m_ungroundedEvent;
        float m_footPlantedAnkleHeightMS;
        float m_footRaisedAnkleHeightMS;
        float m_maxAnkleHeightMS;
        float m_minAnkleHeightMS;
        float m_maxKneeAngleDegrees;
        float m_minKneeAngleDegrees;
        float m_verticalError;
        float m_maxAnkleAngleDegrees;
        short m_hipIndex;
        short m_kneeIndex;
        short m_ankleIndex;
        bool m_hitSomething;
        bool m_isPlantedMS;
        bool m_isOriginalAnkleTransformMSSet;

    public:
        hkbFootIkModifierLeg() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkModifierLeg::Class;
}
