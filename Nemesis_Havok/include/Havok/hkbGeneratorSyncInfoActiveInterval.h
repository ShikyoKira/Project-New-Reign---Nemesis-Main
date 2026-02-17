#pragma once

#include "Havok/hkbGeneratorSyncInfoSyncPoint.h"

namespace nemesis
{
    struct hkbGeneratorSyncInfoActiveInterval : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkbGeneratorSyncInfoActiveInterval",
                                                nullptr,
                                                20,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbGeneratorSyncInfoActiveInterval);

        std::array<nemesis::hkbGeneratorSyncInfoSyncPoint, 2> m_syncPoints;
        float m_fraction{};

    public:
        hkbGeneratorSyncInfoActiveInterval() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGeneratorSyncInfoActiveInterval::Class;
}
