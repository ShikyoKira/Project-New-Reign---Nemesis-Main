#include "Havok/hkbStateMachineDelayedTransitionInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineDelayedTransitionInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineDelayedTransitionInfo::Class; });

nemesis::hkbStateMachineDelayedTransitionInfo::hkbStateMachineDelayedTransitionInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineDelayedTransitionInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineDelayedTransitionInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("delayedTransition", m_delayedTransition); // offset: 0/0 size: 16/16 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("timeDelayed", m_timeDelayed); // offset: 16/16 size: 4/4 align: 4/4
    }

    serializer.WriteValue("isDelayedTransitionReturnToPreviousState",
                          m_isDelayedTransitionReturnToPreviousState); // offset: 20/20 size: 1/1 align: 1/1
    serializer.WriteValue("wasInAbutRangeLastFrame",
                          m_wasInAbutRangeLastFrame); // offset: 21/21 size: 1/1 align: 1/1
    serializer.Pad(4);                                // offset: 22/22 size: 2/2
    // class size: 24/24 align: 4/4
}

void nemesis::hkbStateMachineDelayedTransitionInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("delayedTransition", m_delayedTransition); // offset: 0/0 size: 16/16 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("timeDelayed", m_timeDelayed); // offset: 16/16 size: 4/4 align: 4/4
    }

    deserializer.ReadValue("isDelayedTransitionReturnToPreviousState",
                           m_isDelayedTransitionReturnToPreviousState); // offset: 20/20 size: 1/1 align: 1/1
    deserializer.ReadValue("wasInAbutRangeLastFrame",
                           m_wasInAbutRangeLastFrame); // offset: 21/21 size: 1/1 align: 1/1
    deserializer.Pad(4);                               // offset: 22/22 size: 2/2
    // class size: 24/24 align: 4/4
}
