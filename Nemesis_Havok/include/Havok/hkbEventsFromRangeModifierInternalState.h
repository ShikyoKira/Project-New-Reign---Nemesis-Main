#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbEventsFromRangeModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xcc47b48d,
                                                "hkbEventsFromRangeModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbEventsFromRangeModifierInternalState);

        nemesis::hkArray<bool> m_wasActiveInPreviousFrame;

    public:
        hkbEventsFromRangeModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventsFromRangeModifierInternalState::Class;
}
