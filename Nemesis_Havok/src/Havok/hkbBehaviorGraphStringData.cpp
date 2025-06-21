#include "Havok/hkbBehaviorGraphStringData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorGraphStringData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorGraphStringData::Class; });

nemesis::hkbBehaviorGraphStringData::hkbBehaviorGraphStringData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkArray<nemesis::hkCString>& nemesis::hkbBehaviorGraphStringData::GetEventNames() const
{
    return m_eventNames;
}

const nemesis::hkArray<nemesis::hkCString>& nemesis::hkbBehaviorGraphStringData::GetAttributeNames() const
{
    return m_attributeNames;
}

const nemesis::hkArray<nemesis::hkCString>& nemesis::hkbBehaviorGraphStringData::GetVariableNames() const
{
    return m_variableNames;
}

const nemesis::hkArray<nemesis::hkCString>&
nemesis::hkbBehaviorGraphStringData::GetCharacterPropertyNames() const
{
    return m_characterPropertyNames;
}

const nemesis::hkClass* nemesis::hkbBehaviorGraphStringData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorGraphStringData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);       // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("eventNames", m_eventNames);         // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("attributeNames", m_attributeNames); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("variableNames", m_variableNames);   // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("characterPropertyNames",
                           m_characterPropertyNames); // offset: 44/64 size: 12/16 align: 4/8
    // class size: 56/80 align: 4/8
}

void nemesis::hkbBehaviorGraphStringData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);  // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("eventNames", m_eventNames);         // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("attributeNames", m_attributeNames); // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("variableNames", m_variableNames);   // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("characterPropertyNames",
                            m_characterPropertyNames); // offset: 44/64 size: 12/16 align: 4/8
    // class size: 56/80 align: 4/8
}
