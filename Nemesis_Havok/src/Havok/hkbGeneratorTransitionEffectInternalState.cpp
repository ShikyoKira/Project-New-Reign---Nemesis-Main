#include "Havok/hkbGeneratorTransitionEffectInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGeneratorTransitionEffectInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGeneratorTransitionEffectInternalState::Class; });

nemesis::hkbGeneratorTransitionEffectInternalState::hkbGeneratorTransitionEffectInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbGeneratorTransitionEffectInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGeneratorTransitionEffectInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);          // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("timeInTransition", m_timeInTransition); // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("duration", m_duration);                 // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("effectiveBlendInDuration",
                          m_effectiveBlendInDuration); // offset: 16/24 size: 4/4 align: 4/4
    serializer.WriteValue("effectiveBlendOutDuration",
                          m_effectiveBlendOutDuration);            // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteValue("toGeneratorState", m_toGeneratorState); // offset: 24/32 size: 1/1 align: 1/1
    serializer.WriteValue("echoTransitionGenerator",
                          m_echoTransitionGenerator); // offset: 25/33 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_1_0)
    {
        serializer.WriteValue("toGeneratorSelfTransitionMode", m_toGeneratorSelfTransitionMode);
    }
    else
    {
        serializer.WriteValue("echoToGenerator", m_echoToGenerator); // offset: 26/34 size: 1/1 align: 1/1
    }

    serializer.WriteValue("justActivated", m_justActivated);         // offset: 27/35 size: 1/1 align: 1/1
    serializer.WriteValue("updateActiveNodes", m_updateActiveNodes); // offset: 28/36 size: 1/1 align: 1/1
    serializer.WriteValue("stage", m_stage);                         // offset: 29/37 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                     // offset: 30/38 size: 2/2
    // class size: 32/40 align: 4/8
}

void nemesis::hkbGeneratorTransitionEffectInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);     // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("timeInTransition", m_timeInTransition); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("duration", m_duration);                 // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("effectiveBlendInDuration",
                           m_effectiveBlendInDuration); // offset: 16/24 size: 4/4 align: 4/4
    deserializer.ReadValue("effectiveBlendOutDuration",
                           m_effectiveBlendOutDuration);            // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadValue("toGeneratorState", m_toGeneratorState); // offset: 24/32 size: 1/1 align: 1/1
    deserializer.ReadValue("echoTransitionGenerator",
                           m_echoTransitionGenerator); // offset: 25/33 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_1_0)
    {
        deserializer.ReadValue("toGeneratorSelfTransitionMode", m_toGeneratorSelfTransitionMode);
    }
    else
    {
        deserializer.ReadValue("echoToGenerator", m_echoToGenerator); // offset: 26/34 size: 1/1 align: 1/1
    }

    deserializer.ReadValue("justActivated", m_justActivated);         // offset: 27/35 size: 1/1 align: 1/1
    deserializer.ReadValue("updateActiveNodes", m_updateActiveNodes); // offset: 28/36 size: 1/1 align: 1/1
    deserializer.ReadValue("stage", m_stage);                         // offset: 29/37 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 30/38 size: 2/2
    // class size: 32/40 align: 4/8
}
