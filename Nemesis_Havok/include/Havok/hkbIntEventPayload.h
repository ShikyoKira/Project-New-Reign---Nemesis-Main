#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbIntEventPayload : nemesis::hkbEventPayload
    {
        static constexpr nemesis::hkClass Class{0xebbc1bd3,
                                                "hkbIntEventPayload",
                                                &nemesis::hkbEventPayload::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbIntEventPayload);

        int m_data;

    public:
        hkbIntEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbIntEventPayload::Class;
}
