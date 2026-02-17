#pragma once

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbEventRangeData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x6cb92c76,
                                                "hkbEventRangeData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbEventRangeData);

        using EventRangeMode = nemesis::hkInternalClassEnumItem<char>;

        float m_upperBound{};
        nemesis::hkbEventProperty m_event;
        nemesis::hkEnum<EventRangeMode, char> m_eventMode = {
            {"EVENT_MODE_SEND_ON_ENTER_RANGE", 0},
            {"EVENT_MODE_SEND_WHEN_IN_RANGE", 1},
        };

    public:
        hkbEventRangeData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventRangeData::Class;
}
