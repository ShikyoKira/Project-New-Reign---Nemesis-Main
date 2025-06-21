#pragma once

#include "Havok/hkbNamedEventPayload.h"

namespace nemesis
{
    struct hkbNamedRealEventPayload : nemesis::hkbNamedEventPayload
    {
        static constexpr nemesis::hkClass Class{0x9c99fd70,
                                                "hkbNamedRealEventPayload",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbNamedRealEventPayload);

        float m_data;

    public:
        hkbNamedRealEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbNamedRealEventPayload::Class;
}
