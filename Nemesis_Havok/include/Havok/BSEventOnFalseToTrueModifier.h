#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSEventOnFalseToTrueModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x81d0777a,
                                                "BSEventOnFalseToTrueModifier",
                                                &nemesis::hkbModifier::Class,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSEventOnFalseToTrueModifier);

        bool m_bEnableEvent1{};
        bool m_bVariableToTest1{};
        nemesis::hkbEventProperty m_EventToSend1;
        bool m_bEnableEvent2{};
        bool m_bVariableToTest2{};
        nemesis::hkbEventProperty m_EventToSend2;
        bool m_bEnableEvent3{};
        bool m_bVariableToTest3{};
        nemesis::hkbEventProperty m_EventToSend3;
        bool m_bSlot1ActivatedLastFrame{};
        bool m_bSlot2ActivatedLastFrame{};
        bool m_bSlot3ActivatedLastFrame{};

    public:
        BSEventOnFalseToTrueModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSEventOnFalseToTrueModifier::Class;
}
