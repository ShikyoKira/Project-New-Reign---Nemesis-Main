#include "Havok/hkbGeneratorTransitionEffect.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGeneratorTransitionEffect,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGeneratorTransitionEffect::Class; });

nemesis::hkbGeneratorTransitionEffect::hkbGeneratorTransitionEffect() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbTransitionEffect(Class)
{
}

const nemesis::hkClass* nemesis::hkbGeneratorTransitionEffect::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGeneratorTransitionEffect::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbTransitionEffect::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("transitionGenerator",
                           m_transitionGenerator);                 // offset: 44/80 size: 4/8 align: 4/8
    serializer.WriteValue("blendInDuration", m_blendInDuration);   // offset: 48/88 size: 4/4 align: 4/4
    serializer.WriteValue("blendOutDuration", m_blendOutDuration); // offset: 52/92 size: 4/4 align: 4/4
    serializer.WriteValue("syncToGeneratorStartTime",
                          m_syncToGeneratorStartTime); // offset: 56/96 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());       // offset: 57/97 size: 3/7

    serializer.WriteSerializeIgnoredObject("fromGenerator",
                                           m_fromGenerator); // offset: 60/104 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("toGenerator",
                                           m_toGenerator); // offset: 64/112 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("timeInTransition",
                                          m_timeInTransition);     // offset: 68/120 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("duration", m_duration); // offset: 72/124 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("effectiveBlendInDuration",
                                          m_effectiveBlendInDuration); // offset: 76/128 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("effectiveBlendOutDuration",
                                          m_effectiveBlendOutDuration); // offset: 80/132 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("toGeneratorState",
                                          m_toGeneratorState); // offset: 84/136 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("echoTransitionGenerator",
                                          m_echoTransitionGenerator); // offset: 85/137 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("echoToGenerator",
                                          m_echoToGenerator); // offset: 86/138 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("justActivated",
                                          m_justActivated); // offset: 87/139 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("updateActiveNodes",
                                          m_updateActiveNodes); // offset: 88/140 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("stage", m_stage);    // offset: 89/141 size: 1/1 align: 1/1
    serializer.Skip(2);                                         // offset: 90/142 size: 2/2
    // class size: 92/144 align: 4/8
}

void nemesis::hkbGeneratorTransitionEffect::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbTransitionEffect::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("transitionGenerator",
                            m_transitionGenerator);                 // offset: 44/80 size: 4/8 align: 4/8
    deserializer.ReadValue("blendInDuration", m_blendInDuration);   // offset: 48/88 size: 4/4 align: 4/4
    deserializer.ReadValue("blendOutDuration", m_blendOutDuration); // offset: 52/92 size: 4/4 align: 4/4
    deserializer.ReadValue("syncToGeneratorStartTime",
                           m_syncToGeneratorStartTime); // offset: 56/96 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());    // offset: 57/97 size: 3/7

    deserializer.ReadObject("fromGenerator", m_fromGenerator);      // offset: 60/104 size: 4/8 align: 4/8
    deserializer.ReadObject("toGenerator", m_toGenerator);          // offset: 64/112 size: 4/8 align: 4/8
    deserializer.ReadValue("timeInTransition", m_timeInTransition); // offset: 68/120 size: 4/4 align: 4/4
    deserializer.ReadValue("duration", m_duration);                 // offset: 72/124 size: 4/4 align: 4/4
    deserializer.ReadValue("effectiveBlendInDuration",
                           m_effectiveBlendInDuration); // offset: 76/128 size: 4/4 align: 4/4
    deserializer.ReadValue("effectiveBlendOutDuration",
                           m_effectiveBlendOutDuration);            // offset: 80/132 size: 4/4 align: 4/4
    deserializer.ReadValue("toGeneratorState", m_toGeneratorState); // offset: 84/136 size: 1/1 align: 1/1
    deserializer.ReadValue("echoTransitionGenerator",
                           m_echoTransitionGenerator);                // offset: 85/137 size: 1/1 align: 1/1
    deserializer.ReadValue("echoToGenerator", m_echoToGenerator);     // offset: 86/138 size: 1/1 align: 1/1
    deserializer.ReadValue("justActivated", m_justActivated);         // offset: 87/139 size: 1/1 align: 1/1
    deserializer.ReadValue("updateActiveNodes", m_updateActiveNodes); // offset: 88/140 size: 1/1 align: 1/1
    deserializer.ReadValue("stage", m_stage);                         // offset: 89/141 size: 1/1 align: 1/1
    deserializer.Skip(2);                                             // offset: 90/142 size: 2/2
    // class size: 92/144 align: 4/8
}
