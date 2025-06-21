#include "Havok/hkpMultiRayShapeRay.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMultiRayShapeRay,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMultiRayShapeRay::Class; });

nemesis::hkpMultiRayShapeRay::hkpMultiRayShapeRay() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpMultiRayShapeRay::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMultiRayShapeRay::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("start", m_start); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("end", m_end);     // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

void nemesis::hkpMultiRayShapeRay::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("start", m_start); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("end", m_end);     // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}
