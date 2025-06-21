#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbNamedEventPayload : nemesis::hkbEventPayload
    {
        static constexpr nemesis::hkClass Class{0x65bdd3a0,
                                                "hkbNamedEventPayload",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbNamedEventPayload);

        nemesis::hkStringPtr m_name;

    public:
        hkbNamedEventPayload() noexcept;
        hkbNamedEventPayload(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbNamedEventPayload::Class;
}
