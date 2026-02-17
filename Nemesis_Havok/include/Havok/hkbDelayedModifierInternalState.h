#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbDelayedModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x85fb0b80,
                                                "hkbDelayedModifierInternalState",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbDelayedModifierInternalState);

        float m_secondsElapsed{};
        bool m_isActive{};

    public:
        hkbDelayedModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbDelayedModifierInternalState::Class;
}
