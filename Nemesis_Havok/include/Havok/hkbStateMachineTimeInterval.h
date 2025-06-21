#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbStateMachineTimeInterval : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x60a881e5,
                                                "hkbStateMachineTimeInterval",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineTimeInterval);

        nemesis::hkEventId m_enterEventId;
        nemesis::hkEventId m_exitEventId;
        float m_enterTime;
        float m_exitTime;

    public:
        hkbStateMachineTimeInterval() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineTimeInterval::Class;
}
