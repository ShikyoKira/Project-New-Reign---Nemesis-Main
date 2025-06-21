#include "Havok/hkbStateMachineActiveTransitionInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineActiveTransitionInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineActiveTransitionInfo::Class; });

nemesis::hkbStateMachineActiveTransitionInfo::hkbStateMachineActiveTransitionInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineActiveTransitionInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineActiveTransitionInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        serializer.WriteSerializeIgnoredObject("transitionEffect",
                                               m_transitionEffect); // offset: 0/0 size: 4/8 align: 4/8
        serializer.WriteObject("transitionEffectInternalStateInfo",
                               m_transitionEffectInternalStateInfo); // offset: 4/8 size: 4/8 align: 4/8
        serializer.WriteObject("transitionInfoReference",
                               m_transitionInfoReference); // offset: 8/16 size: 6/6 align: 2/2
        serializer.WriteObject("transitionInfoReferenceForTE",
                               m_transitionInfoReferenceForTE); // offset: 14/22 size: 6/6 align: 2/2
    }
    else
    {
        serializer.WriteObject("transitionInfo", m_transitionInfo);
        serializer.WriteObject("transitionEffect", m_transitionEffect);
    }

    serializer.WriteValue("fromStateId", m_fromStateId); // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteValue("toStateId", m_toStateId);     // offset: 24/32 size: 4/4 align: 4/4
    serializer.WriteValue("isReturnToPreviousState",
                          m_isReturnToPreviousState); // offset: 28/36 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());      // offset: 29/37 size: 3/3
    // class size: 32/40 align: 4/8
}

void nemesis::hkbStateMachineActiveTransitionInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        deserializer.ReadObject("transitionEffect", m_transitionEffect); // offset: 0/0 size: 4/8 align: 4/8
        deserializer.ReadObject("transitionEffectInternalStateInfo",
                                m_transitionEffectInternalStateInfo); // offset: 4/8 size: 4/8 align: 4/8
        deserializer.ReadObject("transitionInfoReference",
                                m_transitionInfoReference); // offset: 8/16 size: 6/6 align: 2/2
        deserializer.ReadObject("transitionInfoReferenceForTE",
                                m_transitionInfoReferenceForTE); // offset: 14/22 size: 6/6 align: 2/2
    }
    else
    {
        deserializer.ReadObject("transitionInfo", m_transitionInfo);
        deserializer.ReadObject("transitionEffect", m_transitionEffect);
    }

    deserializer.ReadValue("fromStateId", m_fromStateId); // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadValue("toStateId", m_toStateId);     // offset: 24/32 size: 4/4 align: 4/4
    deserializer.ReadValue("isReturnToPreviousState",
                           m_isReturnToPreviousState); // offset: 28/36 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());   // offset: 29/37 size: 3/3
    // class size: 32/40 align: 4/8
}
