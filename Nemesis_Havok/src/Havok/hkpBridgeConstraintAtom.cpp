#include "Havok/hkpBridgeConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBridgeConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBridgeConstraintAtom::Class; });

nemesis::hkpBridgeConstraintAtom::hkpBridgeConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpBridgeConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBridgeConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());         // offset: 2/2 size: 2/6

    serializer.WriteSerializeIgnoredObject("buildJacobianFunc",
                                           m_buildJacobianFunc); // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteObject("constraintData", m_constraintData);  // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("padding", m_padding); // offset: 12/24 size: 4/4 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                 // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

void nemesis::hkpBridgeConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());           // offset: 2/2 size: 2/6

    deserializer.ReadObject("buildJacobianFunc", m_buildJacobianFunc); // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadObject("constraintData", m_constraintData);       // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("padding", m_padding);                      // offset: 12/24 size: 4/4 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                   // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}
