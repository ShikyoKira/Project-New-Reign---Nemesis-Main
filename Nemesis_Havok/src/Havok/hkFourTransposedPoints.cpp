#include "Havok/hkFourTransposedPoints.h"

REGISTER_HAVOK_POINTER_SOURCE(hkFourTransposedPoints,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkFourTransposedPoints::Class; });

nemesis::hkFourTransposedPoints::hkFourTransposedPoints() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkFourTransposedPoints::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkFourTransposedPoints::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("vertices", m_vertices); // offset: 0/0 size: 48/48 align: 16/16
    // class size: 48/48 align: 16/16
}

void nemesis::hkFourTransposedPoints::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("vertices", m_vertices); // offset: 0/0 size: 48/48 align: 16/16
    // class size: 48/48 align: 16/16
}
