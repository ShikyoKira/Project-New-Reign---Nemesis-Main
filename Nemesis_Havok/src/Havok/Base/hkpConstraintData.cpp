#include "Havok/Base/hkpConstraintData.h"

nemesis::hkpConstraintData::hkpConstraintData(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("userData", m_userData);        // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkpConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("userData", m_userData);             // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
