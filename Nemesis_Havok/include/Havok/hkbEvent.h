#pragma once

#include "Havok/Base/hkbEventBase.h"

namespace nemesis
{
    struct hkbEvent : nemesis::hkbEventBase
    {
        static constexpr nemesis::hkClass Class{0x3e0fd810,
                                                "hkbEvent",
                                                &nemesis::hkbEventBase::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEvent);

        nemesis::hkRefPtr<nemesis::HavokObject> m_sender;

    public:
        hkbEvent() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEvent::Class;
}
