#pragma once

#include "Havok/hkbEvent.h"

namespace nemesis
{
    struct hkbEventSequencedDataSequencedEvent : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9139b821,
                                                "hkbEventSequencedDataSequencedEvent",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbEventSequencedDataSequencedEvent);

        nemesis::hkbEvent m_event;
        float m_time;

    public:
        hkbEventSequencedDataSequencedEvent() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventSequencedDataSequencedEvent::Class;
}
