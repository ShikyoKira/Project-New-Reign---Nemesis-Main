#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbGeneratorSyncInfoSyncPoint : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb597cf92,
                                                "hkbGeneratorSyncInfoSyncPoint",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbGeneratorSyncInfoSyncPoint);

        int m_id;
        float m_time;

    public:
        hkbGeneratorSyncInfoSyncPoint() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGeneratorSyncInfoSyncPoint::Class;
}
