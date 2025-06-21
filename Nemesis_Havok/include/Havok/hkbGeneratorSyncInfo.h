#pragma once

#include "Havok/hkbGeneratorSyncInfoActiveInterval.h"
#include "Havok/hkbGeneratorSyncInfoSyncPoint.h"

namespace nemesis
{
    struct hkbGeneratorSyncInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa3c341f8,
                                                "hkbGeneratorSyncInfo",
                                                nullptr,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbGeneratorSyncInfo);

        std::array<nemesis::hkbGeneratorSyncInfoSyncPoint, 16> m_syncPoints;
        float m_baseFrequency;
        float m_duration;
        float m_localTime;
        float m_playbackSpeed;
        char m_numSyncPoints;
        bool m_isCyclic;
        bool m_isMirrored;
        bool m_isAdditive;
        nemesis::hkbGeneratorSyncInfoActiveInterval m_activeInterval;

    public:
        hkbGeneratorSyncInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGeneratorSyncInfo::Class;
}
