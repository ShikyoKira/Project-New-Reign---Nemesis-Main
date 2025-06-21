#pragma once

#include "Havok/hkpSerializedTrack1nInfo.h"

namespace nemesis
{
    struct hkpSerializedSubTrack1nInfo : nemesis::hkpSerializedTrack1nInfo
    {
        static constexpr nemesis::hkClass Class{0x10155a,
                                                "hkpSerializedSubTrack1nInfo",
                                                &nemesis::hkpSerializedTrack1nInfo::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedSubTrack1nInfo);

        int m_sectorIndex;
        int m_offsetInSector;

    public:
        hkpSerializedSubTrack1nInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedSubTrack1nInfo::Class;
}
