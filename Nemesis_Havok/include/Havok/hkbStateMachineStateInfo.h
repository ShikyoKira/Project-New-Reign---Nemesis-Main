#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkArray.h"
#include "Havok/hkbStateListener.h"
#include "Havok/hkbStateMachineEventPropertyArray.h"
#include "Havok/hkbStateMachineTransitionInfoArray.h"
#include "Havok/hkClass.h"

namespace nemesis
{
    struct hkbStateMachineStateInfo : nemesis::hkbBindable
    {
        static constexpr nemesis::hkClass Class{0xed7f9d0,
                                                "hkbStateMachineStateInfo",
                                                &nemesis::hkbBindable::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineStateInfo);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbStateListener>> m_listeners;
        nemesis::hkRefPtr<nemesis::hkbStateMachineEventPropertyArray> m_enterNotifyEvents;
        nemesis::hkbEventProperty m_enterNotifyEvent;
        nemesis::hkRefPtr<nemesis::hkbStateMachineEventPropertyArray> m_exitNotifyEvents;
        nemesis::hkbEventProperty m_exitNotifyEvent;
        nemesis::hkRefPtr<nemesis::hkbStateMachineTransitionInfoArray> m_transitions;
        nemesis::hkRefPtr<nemesis::hkbGenerator> m_generator;
        nemesis::hkStringPtr m_name;
        int m_stateId;
        float m_probability;
        bool m_enable;

    public:
        hkbStateMachineStateInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineStateInfo::Class;
}
