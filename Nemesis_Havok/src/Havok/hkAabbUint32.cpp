#include "Havok/hkAabbUint32.h"

REGISTER_HAVOK_POINTER_SOURCE(hkAabbUint32,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkAabbUint32::Class; });

nemesis::hkAabbUint32::hkAabbUint32() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkAabbUint32::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkAabbUint32::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("min", m_min);                       // offset: 0/0 size: 12/12 align: 16/16
    serializer.WriteValue("expansionMin", m_expansionMin);     // offset: 12/12 size: 3/3 align: 1/1
    serializer.WriteValue("expansionShift", m_expansionShift); // offset: 15/15 size: 1/1 align: 1/1
    serializer.WriteValue("max", m_max);                       // offset: 16/16 size: 12/12 align: 4/4
    serializer.WriteValue("expansionMax", m_expansionMax);     // offset: 28/28 size: 3/3 align: 1/1
    serializer.WriteValue("shapeKeyByte", m_shapeKeyByte);     // offset: 31/31 size: 1/1 align: 1/1
    // class size: 32/32 align: 16/16
}

void nemesis::hkAabbUint32::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("min", m_min);                       // offset: 0/0 size: 12/12 align: 16/16
    deserializer.ReadValue("expansionMin", m_expansionMin);     // offset: 12/12 size: 3/3 align: 1/1
    deserializer.ReadValue("expansionShift", m_expansionShift); // offset: 15/15 size: 1/1 align: 1/1
    deserializer.ReadValue("max", m_max);                       // offset: 16/16 size: 12/12 align: 4/4
    deserializer.ReadValue("expansionMax", m_expansionMax);     // offset: 28/28 size: 3/3 align: 1/1
    deserializer.ReadValue("shapeKeyByte", m_shapeKeyByte);     // offset: 31/31 size: 1/1 align: 1/1
    // class size: 32/32 align: 16/16
}
