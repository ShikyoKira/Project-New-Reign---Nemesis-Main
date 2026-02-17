#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbTimerModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x83ec2d42,
                                                "hkbTimerModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbTimerModifierInternalState);

        float m_secondsElapsed{};

    public:
        hkbTimerModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbTimerModifierInternalState::Class;
}
