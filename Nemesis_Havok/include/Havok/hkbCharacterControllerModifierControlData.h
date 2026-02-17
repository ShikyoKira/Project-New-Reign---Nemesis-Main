#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbCharacterControllerControlData;

    struct hkbCharacterControllerModifierControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xc4e809af,
                                                "hkbCharacterControllerModifierControlData",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterControllerModifierControlData);

        float m_verticalGain{};
        float m_horizontalCatchUpGain{};
        float m_maxVerticalSeparation{};
        float m_maxHorizontalSeparation{};

    public:
        hkbCharacterControllerModifierControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        void CopyFrom(const nemesis::hkbCharacterControllerControlData& control_data);

        friend nemesis::hkbCharacterControllerControlData;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterControllerModifierControlData::Class;
}
