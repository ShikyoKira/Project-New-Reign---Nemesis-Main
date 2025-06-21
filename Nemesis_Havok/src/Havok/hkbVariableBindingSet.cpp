#include "Havok/hkbVariableBindingSet.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbVariableBindingSet,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbVariableBindingSet::Class; });

nemesis::hkbVariableBindingSet::hkbVariableBindingSet() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbVariableBindingSet::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbVariableBindingSet::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("bindings", m_bindings);       // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("indexOfBindingToEnable",
                          m_indexOfBindingToEnable); // offset: 20/32 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("hasOutputBinding",
                                          m_hasOutputBinding); // offset: 24/36 size: 1/1 align: 1/1
    serializer.Skip(3);                                        // offset: 25/37 size: 3/3
    // class size: 28/40 align: 4/8
}

void nemesis::hkbVariableBindingSet::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("bindings", m_bindings);            // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("indexOfBindingToEnable",
                           m_indexOfBindingToEnable);               // offset: 20/32 size: 4/4 align: 4/4
    deserializer.ReadValue("hasOutputBinding", m_hasOutputBinding); // offset: 24/36 size: 1/1 align: 1/1
    deserializer.Skip(3);                                           // offset: 25/37 size: 3/3
    // class size: 28/40 align: 4/8
}
