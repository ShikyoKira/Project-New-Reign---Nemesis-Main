#include "Havok/hkpBreakableShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBreakableShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBreakableShape::Class; });

nemesis::hkpBreakableShape::hkpBreakableShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpBreakableShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBreakableShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);   // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("physicsShape", m_physicsShape); // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("material", m_material);         // offset: 12/24 size: 12/16 align: 4/8
    // class size: 24/40 align: 4/8
}

void nemesis::hkpBreakableShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("physicsShape", m_physicsShape);    // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("material", m_material);            // offset: 12/24 size: 12/16 align: 4/8
    // class size: 24/40 align: 4/8
}
