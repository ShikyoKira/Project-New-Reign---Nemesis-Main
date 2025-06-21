#pragma once

#include "Havok/hkpAgent1nSector.h"

namespace nemesis
{
    struct hkpSerializedSubTrack1nInfo;

    struct hkpSerializedTrack1nInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf12d48d9,
                                                "hkpSerializedTrack1nInfo",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedTrack1nInfo);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpAgent1nSector>> m_sectors;
        nemesis::hkArray<nemesis::hkpSerializedSubTrack1nInfo*> m_subTracks;

    public:
        hkpSerializedTrack1nInfo() noexcept;
        hkpSerializedTrack1nInfo(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedTrack1nInfo::Class;
}
