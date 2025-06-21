#include "Havok/hkpConstraintOwner.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintOwner,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintOwner::Class; });

nemesis::hkpConstraintOwner::hkpConstraintOwner() noexcept
    : nemesis::hkpConstraintOwner(Class)
{
}

nemesis::hkpConstraintOwner::hkpConstraintOwner(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkpConstraintOwner::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintOwner::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);       // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("constraintInfo", m_constraintInfo); // offset: 8/16 size: 16/16 align: 4/4
    // class size: 24/32 align: 4/8
}

void nemesis::hkpConstraintOwner::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);  // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("constraintInfo", m_constraintInfo); // offset: 8/16 size: 16/16 align: 4/4
    // class size: 24/32 align: 4/8
}
