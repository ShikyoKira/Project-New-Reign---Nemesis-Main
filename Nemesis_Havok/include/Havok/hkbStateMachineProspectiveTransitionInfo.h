#pragma once

#include "Havok/hkbStateMachineTransitionInfoReference.h"

namespace nemesis
{
    struct hkbStateMachineProspectiveTransitionInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x3ab09a2e,
                                                "hkbStateMachineProspectiveTransitionInfo",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineProspectiveTransitionInfo);

        nemesis::hkbStateMachineTransitionInfoReference m_transitionInfoReference;
        nemesis::hkbStateMachineTransitionInfoReference m_transitionInfoReferenceForTE;
        int m_toStateId;

    public:
        hkbStateMachineProspectiveTransitionInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineProspectiveTransitionInfo::Class;
}
