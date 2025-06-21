#include "Havok/hkpLinearParametricCurve.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLinearParametricCurve,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLinearParametricCurve::Class; });

nemesis::hkpLinearParametricCurve::hkpLinearParametricCurve() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpParametricCurve(Class)
{
}

const nemesis::hkClass* nemesis::hkpLinearParametricCurve::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLinearParametricCurve::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpParametricCurve::SerializeTo(serializer);        // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("smoothingFactor", m_smoothingFactor); // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("closedLoop", m_closedLoop);           // offset: 12/20 size: 1/1 align: 1/1
    serializer.Pad(16);                                          // offset: 13/21 size: 3/11

    serializer.WriteValue("dirNotParallelToTangentAlongWholePath",
                          m_dirNotParallelToTangentAlongWholePath); // offset: 16/32 size: 16/16 align: 16/16
    serializer.WriteObject("points", m_points);                     // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("distance", m_distance);                 // offset: 44/64 size: 12/16 align: 4/8
    serializer.Pad(16);                                             // offset: 56/80 size: 8/0
    // class size: 64/80 align: 16/16
}

void nemesis::hkpLinearParametricCurve::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpParametricCurve::DeserializeFrom(deserializer);   // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("smoothingFactor", m_smoothingFactor); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("closedLoop", m_closedLoop);           // offset: 12/20 size: 1/1 align: 1/1
    deserializer.Pad(16);                                         // offset: 13/21 size: 3/11

    deserializer.ReadValue("dirNotParallelToTangentAlongWholePath",
                           m_dirNotParallelToTangentAlongWholePath); // offset: 16/32 size: 16/16 align: 16/16
    deserializer.ReadObject("points", m_points);                     // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("distance", m_distance);                 // offset: 44/64 size: 12/16 align: 4/8
    deserializer.Pad(16);                                            // offset: 56/80 size: 8/0
    // class size: 64/80 align: 16/16
}
