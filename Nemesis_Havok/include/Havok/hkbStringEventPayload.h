#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbStringEventPayload : nemesis::hkbEventPayload
    {
        static constexpr nemesis::hkClass Class{0xed04256a,
                                                "hkbStringEventPayload",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbStringEventPayload);

        nemesis::hkStringPtr m_data;

    public:
        hkbStringEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStringEventPayload::Class;
}
