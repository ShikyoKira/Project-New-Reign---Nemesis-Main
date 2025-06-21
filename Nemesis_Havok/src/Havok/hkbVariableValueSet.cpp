#include "Havok/hkbVariableValueSet.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbVariableValueSet,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbVariableValueSet::Class; });

nemesis::hkbVariableValueSet::hkbVariableValueSet() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbVariableValueSet::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbVariableValueSet::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);               // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("wordVariableValues", m_wordVariableValues); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("quadVariableValues",
                           m_quadVariableValues); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("variantVariableValues",
                           m_variantVariableValues); // offset: 32/48 size: 12/16 align: 4/8
    // class size: 44/64 align: 4/8
}

void nemesis::hkbVariableValueSet::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("wordVariableValues",
                            m_wordVariableValues); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("quadVariableValues",
                            m_quadVariableValues); // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("variantVariableValues",
                            m_variantVariableValues); // offset: 32/48 size: 12/16 align: 4/8
    // class size: 44/64 align: 4/8
}
