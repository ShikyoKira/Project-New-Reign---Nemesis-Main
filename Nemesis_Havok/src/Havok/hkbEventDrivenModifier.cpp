#include "Havok/hkbEventDrivenModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventDrivenModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventDrivenModifier::Class; });

nemesis::hkbEventDrivenModifier::hkbEventDrivenModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifierWrapper(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventDrivenModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventDrivenModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifierWrapper::SerializeTo(serializer);            // offset: 0/0 size: 48/88 align: 4/8
    serializer.WriteValue("activateEventId", m_activateEventId);     // offset: 48/88 size: 4/4 align: 4/4
    serializer.WriteValue("deactivateEventId", m_deactivateEventId); // offset: 52/92 size: 4/4 align: 4/4
    serializer.WriteValue("activeByDefault", m_activeByDefault);     // offset: 56/96 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("isActive", m_isActive);   // offset: 57/97 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                     // offset: 58/98 size: 2/6
    // class size: 60/104 align: 4/8
}

void nemesis::hkbEventDrivenModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifierWrapper::DeserializeFrom(deserializer);       // offset: 0/0 size: 48/88 align: 4/8
    deserializer.ReadValue("activateEventId", m_activateEventId);     // offset: 48/88 size: 4/4 align: 4/4
    deserializer.ReadValue("deactivateEventId", m_deactivateEventId); // offset: 52/92 size: 4/4 align: 4/4
    deserializer.ReadValue("activeByDefault", m_activeByDefault);     // offset: 56/96 size: 1/1 align: 1/1
    deserializer.ReadValue("isActive", m_isActive);                   // offset: 57/97 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 58/98 size: 2/6
    // class size: 60/104 align: 4/8
}
