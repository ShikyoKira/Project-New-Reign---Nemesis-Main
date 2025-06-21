#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbCharacterControllerModifierControlData;

    struct hkbCharacterControllerControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x5b6c03d9,
                                                "hkbCharacterControllerControlData",
                                                nullptr,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterControllerControlData);

        nemesis::hkVector4 m_desiredVelocity;
        float m_verticalGain;
        float m_horizontalCatchUpGain;
        float m_maxVerticalSeparation;
        float m_maxHorizontalSeparation;

    public:
        hkbCharacterControllerControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        void CopyFrom(const nemesis::hkbCharacterControllerModifierControlData& control_data);

        friend nemesis::hkbCharacterControllerModifierControlData;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterControllerControlData::Class;
}
