#pragma once

#include "Havok/hkClass.h"

#include "Havok/Types/hkQuaternion.h"
#include "Havok/Types/hkVector4.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbFootIkDriverInfoLeg : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x224b18d1,
                                                "hkbFootIkDriverInfoLeg",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkDriverInfoLeg);

        nemesis::hkQuaternion m_prevAnkleRotLS;
        nemesis::hkVector4 m_kneeAxisLS;
        nemesis::hkVector4 m_footEndLS;
        float m_footPlantedAnkleHeightMS{};
        float m_footRaisedAnkleHeightMS{};
        float m_maxAnkleHeightMS{};
        float m_minAnkleHeightMS{};
        float m_maxKneeAngleDegrees{};
        float m_minKneeAngleDegrees{};
        float m_maxAnkleAngleDegrees{};
        short m_hipIndex{};
        short m_hipSiblingIndex{};
        short m_kneeIndex{};
        short m_kneeSiblingIndex{};
        short m_ankleIndex{};

    public:
        hkbFootIkDriverInfoLeg() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkDriverInfoLeg::Class;
}
