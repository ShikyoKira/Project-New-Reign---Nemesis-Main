#pragma once

#include "Havok/hkbNodeInternalStateInfo.h"
#include "Havok/hkbStateMachineTransitionInfo.h"
#include "Havok/hkbStateMachineTransitionInfoReference.h"

namespace nemesis
{
    struct hkbStateMachineActiveTransitionInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xbb90d54f,
                                                "hkbStateMachineActiveTransitionInfo",
                                                nullptr,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineActiveTransitionInfo);

        nemesis::hkRefPtr<nemesis::hkbStateMachineTransitionInfo> m_transitionInfo;
        nemesis::hkRefPtr<nemesis::hkbTransitionEffect> m_transitionEffect;
        nemesis::hkRefPtr<nemesis::hkbNodeInternalStateInfo> m_transitionEffectInternalStateInfo;
        nemesis::hkbStateMachineTransitionInfoReference m_transitionInfoReference;
        nemesis::hkbStateMachineTransitionInfoReference m_transitionInfoReferenceForTE;
        int m_fromStateId{};
        int m_toStateId{};
        bool m_isReturnToPreviousState{};

    public:
        hkbStateMachineActiveTransitionInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineActiveTransitionInfo::Class;
}
