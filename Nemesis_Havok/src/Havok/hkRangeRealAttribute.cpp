#include "Havok/hkRangeRealAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkRangeRealAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkRangeRealAttribute::Class; });

nemesis::hkRangeRealAttribute::hkRangeRealAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkRangeRealAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkRangeRealAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("absmin", m_absmin);   // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("absmax", m_absmax);   // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("softmin", m_softmin); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("softmax", m_softmax); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkRangeRealAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("absmin", m_absmin);   // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("absmax", m_absmax);   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("softmin", m_softmin); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("softmax", m_softmax); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}
