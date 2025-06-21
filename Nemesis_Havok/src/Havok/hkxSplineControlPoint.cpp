#include "Havok/hkxSplineControlPoint.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSplineControlPoint,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSplineControlPoint::Class; });

nemesis::hkxSplineControlPoint::hkxSplineControlPoint() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxSplineControlPoint::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSplineControlPoint::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("position", m_position);     // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("tangentIn", m_tangentIn);   // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("tangentOut", m_tangentOut); // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("inType", m_inType);         // offset: 48/48 size: 1/1 align: 1/1
    serializer.WriteValue("outType", m_outType);       // offset: 49/49 size: 1/1 align: 1/1
    serializer.Skip(14);                               // offset: 50/50 size: 14/14
    // class size: 64/64 align: 16/16
}

void nemesis::hkxSplineControlPoint::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("position", m_position);     // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("tangentIn", m_tangentIn);   // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("tangentOut", m_tangentOut); // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("inType", m_inType);         // offset: 48/48 size: 1/1 align: 1/1
    deserializer.ReadValue("outType", m_outType);       // offset: 49/49 size: 1/1 align: 1/1
    deserializer.Skip(14);                              // offset: 50/50 size: 14/14
    // class size: 64/64 align: 16/16
}
