#include "Havok/hkpSetLocalRotationsConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSetLocalRotationsConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSetLocalRotationsConstraintAtom::Class; });

nemesis::hkpSetLocalRotationsConstraintAtom::hkpSetLocalRotationsConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpSetLocalRotationsConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSetLocalRotationsConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(14);                                 // offset: 2/2 size: 14/14

    serializer.WriteValue("rotationA", m_rotationA); // offset: 16/16 size: 48/48 align: 16/16
    serializer.WriteValue("rotationB", m_rotationB); // offset: 64/64 size: 48/48 align: 16/16
    // class size: 112/112 align: 16/16
}

void nemesis::hkpSetLocalRotationsConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(14);                                     // offset: 2/2 size: 14/14

    deserializer.ReadValue("rotationA", m_rotationA); // offset: 16/16 size: 48/48 align: 16/16
    deserializer.ReadValue("rotationB", m_rotationB); // offset: 64/64 size: 48/48 align: 16/16
    // class size: 112/112 align: 16/16
}
