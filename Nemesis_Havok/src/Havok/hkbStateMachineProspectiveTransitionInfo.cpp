#include "Havok/hkbStateMachineProspectiveTransitionInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineProspectiveTransitionInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineProspectiveTransitionInfo::Class; });

nemesis::hkbStateMachineProspectiveTransitionInfo::hkbStateMachineProspectiveTransitionInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineProspectiveTransitionInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineProspectiveTransitionInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() <= HavokVersion::HK_710) return;

    serializer.WriteObject("transitionInfoReference",
                           m_transitionInfoReference); // offset: 0/0 size: 6/6 align: 2/2
    serializer.WriteObject("transitionInfoReferenceForTE",
                           m_transitionInfoReferenceForTE); // offset: 6/6 size: 6/6 align: 2/2
    serializer.WriteValue("toStateId", m_toStateId);        // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkbStateMachineProspectiveTransitionInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() <= HavokVersion::HK_710) return;

    deserializer.ReadObject("transitionInfoReference",
                            m_transitionInfoReference); // offset: 0/0 size: 6/6 align: 2/2
    deserializer.ReadObject("transitionInfoReferenceForTE",
                            m_transitionInfoReferenceForTE); // offset: 6/6 size: 6/6 align: 2/2
    deserializer.ReadValue("toStateId", m_toStateId);        // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}
