#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbRealEventPayload : nemesis::hkbEventPayload
    {
        static constexpr nemesis::hkClass Class{0x9416affd,
                                                "hkbRealEventPayload",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbRealEventPayload);

        float m_data{};

    public:
        hkbRealEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRealEventPayload::Class;
}
