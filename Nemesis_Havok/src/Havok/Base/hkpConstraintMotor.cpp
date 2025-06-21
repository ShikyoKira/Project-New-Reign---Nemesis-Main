#include "Havok/Base/hkpConstraintMotor.h"

nemesis::hkpConstraintMotor::hkpConstraintMotor(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpConstraintMotor::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("type", m_type);                // offset: 8/16 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());          // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}

void nemesis::hkpConstraintMotor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("type", m_type);                     // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}
