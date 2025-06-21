#include "Havok/hkPackedVector3.h"

REGISTER_HAVOK_POINTER_SOURCE(hkPackedVector3,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkPackedVector3::Class; });

nemesis::hkPackedVector3::hkPackedVector3() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkPackedVector3::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkPackedVector3::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("values", m_values); // offset: 0/0 size: 8/8 align: 2/2
    // class size: 8/8 align: 2/2
}

void nemesis::hkPackedVector3::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("values", m_values); // offset: 0/0 size: 8/8 align: 2/2
    // class size: 8/8 align: 2/2
}
