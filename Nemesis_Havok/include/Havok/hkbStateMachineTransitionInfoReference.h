#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbStateMachineTransitionInfoReference : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9810c2d0,
                                                "hkbStateMachineTransitionInfoReference",
                                                nullptr,
                                                6,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineTransitionInfoReference);

        short m_fromStateIndex{};
        short m_transitionIndex{};
        short m_stateMachineId{};

    public:
        hkbStateMachineTransitionInfoReference() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineTransitionInfoReference::Class;
}
