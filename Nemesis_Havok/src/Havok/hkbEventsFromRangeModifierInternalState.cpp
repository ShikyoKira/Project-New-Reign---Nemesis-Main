#include "Havok/hkbEventsFromRangeModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventsFromRangeModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventsFromRangeModifierInternalState::Class; });

nemesis::hkbEventsFromRangeModifierInternalState::hkbEventsFromRangeModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventsFromRangeModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventsFromRangeModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("wasActiveInPreviousFrame",
                           m_wasActiveInPreviousFrame); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbEventsFromRangeModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("wasActiveInPreviousFrame",
                            m_wasActiveInPreviousFrame); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
