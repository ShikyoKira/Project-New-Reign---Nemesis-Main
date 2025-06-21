#pragma once

#include "Havok/hkbNamedEventPayload.h"

namespace nemesis
{
    struct hkbNamedStringEventPayload : nemesis::hkbNamedEventPayload
    {
        static constexpr nemesis::hkClass Class{0x6caa9113,
                                                "hkbNamedStringEventPayload",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbNamedStringEventPayload);

        nemesis::hkStringPtr m_data;

    public:
        hkbNamedStringEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbNamedStringEventPayload::Class;
}
