#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSEventEveryNEventsModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x6030970c,
                                                "BSEventEveryNEventsModifier",
                                                &nemesis::hkbModifier::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSEventEveryNEventsModifier);

        nemesis::hkbEventProperty m_eventToCheckFor;
        nemesis::hkbEventProperty m_eventToSend;
        char m_numberOfEventsBeforeSend;
        char m_minimumNumberOfEventsBeforeSend;
        bool m_randomizeNumberOfEvents;
        int m_numberOfEventsSeen;
        char m_calculatedNumberOfEventsBeforeSend;

    public:
        BSEventEveryNEventsModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSEventEveryNEventsModifier::Class;
}
