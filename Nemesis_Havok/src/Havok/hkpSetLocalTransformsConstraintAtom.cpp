#include "Havok/hkpSetLocalTransformsConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSetLocalTransformsConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSetLocalTransformsConstraintAtom::Class; });

nemesis::hkpSetLocalTransformsConstraintAtom::hkpSetLocalTransformsConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpSetLocalTransformsConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSetLocalTransformsConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(14);                                 // offset: 2/2 size: 14/14

    serializer.WriteValue("transformA", m_transformA); // offset: 16/16 size: 64/64 align: 16/16
    serializer.WriteValue("transformB", m_transformB); // offset: 80/80 size: 64/64 align: 16/16
    // class size: 144/144 align: 16/16
}

void nemesis::hkpSetLocalTransformsConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(14);                                     // offset: 2/2 size: 14/14

    deserializer.ReadValue("transformA", m_transformA); // offset: 16/16 size: 64/64 align: 16/16
    deserializer.ReadValue("transformB", m_transformB); // offset: 80/80 size: 64/64 align: 16/16
    // class size: 144/144 align: 16/16
}
