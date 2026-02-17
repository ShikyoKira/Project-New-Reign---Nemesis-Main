#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventRangeDataArray.h"

namespace nemesis
{
    struct hkbEventsFromRangeModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xbc561b6e,
                                                "hkbEventsFromRangeModifier",
                                                &nemesis::hkbModifier::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEventsFromRangeModifier);

        float m_inputValue{};
        float m_lowerBound{};
        nemesis::hkRefPtr<nemesis::hkbEventRangeDataArray> m_eventRanges;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_wasActiveInPreviousFrame;

    public:
        hkbEventsFromRangeModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventsFromRangeModifier::Class;
}
