#pragma once

#include "Havok/hkbEventSequencedDataSequencedEvent.h"
#include "Havok/hkbSequencedData.h"

namespace nemesis
{
    struct hkbEventSequencedData : nemesis::hkbSequencedData
    {
        static constexpr nemesis::hkClass Class{0x76798eb8,
                                                "hkbEventSequencedData",
                                                &nemesis::hkbSequencedData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbEventSequencedData);

        nemesis::hkArray<nemesis::hkbEventSequencedDataSequencedEvent> m_events;

    public:
        hkbEventSequencedData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventSequencedData::Class;
}
