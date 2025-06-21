#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbEventBase : virtual nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x76bddb31,
                                                "hkbEventBase",
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

    protected:
        nemesis::hkEventId m_id;
        nemesis::hkRefPtr<nemesis::hkbEventPayload> m_payload;

    public:
        hkbEventBase(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventBase::Class;
}
