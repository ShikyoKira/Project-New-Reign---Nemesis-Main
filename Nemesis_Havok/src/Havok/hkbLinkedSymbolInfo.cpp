#include "Havok/hkbLinkedSymbolInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbLinkedSymbolInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbLinkedSymbolInfo::Class; });

nemesis::hkbLinkedSymbolInfo::hkbLinkedSymbolInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbLinkedSymbolInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbLinkedSymbolInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("eventNames", m_eventNames);       // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("variableNames", m_variableNames); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkbLinkedSymbolInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("eventNames", m_eventNames);        // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("variableNames", m_variableNames);  // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
