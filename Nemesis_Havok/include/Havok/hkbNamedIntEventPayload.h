#pragma once

#include "Havok/hkbNamedEventPayload.h"

namespace nemesis
{
    struct hkbNamedIntEventPayload : nemesis::hkbNamedEventPayload
    {
        static constexpr nemesis::hkClass Class{0x3c99bda4,
                                                "hkbNamedIntEventPayload",
                                                &nemesis::hkbNamedEventPayload::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbNamedIntEventPayload);

        int m_data{};

    public:
        hkbNamedIntEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbNamedIntEventPayload::Class;
}
