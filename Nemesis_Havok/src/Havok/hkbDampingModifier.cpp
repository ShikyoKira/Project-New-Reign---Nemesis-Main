#include "Havok/hkbDampingModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDampingModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDampingModifier::Class; });

nemesis::hkbDampingModifier::hkbDampingModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbDampingModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDampingModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                       // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("kP", m_kP);                                   // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteValue("kI", m_kI);                                   // offset: 48/84 size: 4/4 align: 4/4
    serializer.WriteValue("kD", m_kD);                                   // offset: 52/88 size: 4/4 align: 4/4
    serializer.WriteValue("enableScalarDamping", m_enableScalarDamping); // offset: 56/92 size: 1/1 align: 1/1
    serializer.WriteValue("enableVectorDamping", m_enableVectorDamping); // offset: 57/93 size: 1/1 align: 1/1
    serializer.Skip(2);                                                  // offset: 58/94 size: 2/2

    serializer.WriteValue("rawValue", m_rawValue);       // offset: 60/96 size: 4/4 align: 4/4
    serializer.WriteValue("dampedValue", m_dampedValue); // offset: 64/100 size: 4/4 align: 4/4
    serializer.Pad(16);                                  // offset: 68/104 size: 12/8

    serializer.WriteValue("rawVector", m_rawVector);               // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteValue("dampedVector", m_dampedVector);         // offset: 96/128 size: 16/16 align: 16/16
    serializer.WriteValue("vecErrorSum", m_vecErrorSum);           // offset: 112/144 size: 16/16 align: 16/16
    serializer.WriteValue("vecPreviousError", m_vecPreviousError); // offset: 128/160 size: 16/16 align: 16/16
    serializer.WriteValue("errorSum", m_errorSum);                 // offset: 144/176 size: 4/4 align: 4/4
    serializer.WriteValue("previousError", m_previousError);       // offset: 148/180 size: 4/4 align: 4/4
    serializer.Skip(8);                                            // offset: 152/184 size: 8/8
    // class size: 160/192 align: 16/16
}

void nemesis::hkbDampingModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("kP", m_kP);                  // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("kI", m_kI);                  // offset: 48/84 size: 4/4 align: 4/4
    deserializer.ReadValue("kD", m_kD);                  // offset: 52/88 size: 4/4 align: 4/4
    deserializer.ReadValue("enableScalarDamping",
                           m_enableScalarDamping); // offset: 56/92 size: 1/1 align: 1/1
    deserializer.ReadValue("enableVectorDamping",
                           m_enableVectorDamping); // offset: 57/93 size: 1/1 align: 1/1
    deserializer.Skip(2);                          // offset: 58/94 size: 2/2

    deserializer.ReadValue("rawValue", m_rawValue);       // offset: 60/96 size: 4/4 align: 4/4
    deserializer.ReadValue("dampedValue", m_dampedValue); // offset: 64/100 size: 4/4 align: 4/4
    deserializer.Pad(16);                                 // offset: 68/104 size: 12/8

    deserializer.ReadValue("rawVector", m_rawVector);       // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("dampedVector", m_dampedVector); // offset: 96/128 size: 16/16 align: 16/16
    deserializer.ReadValue("vecErrorSum", m_vecErrorSum);   // offset: 112/144 size: 16/16 align: 16/16
    deserializer.ReadValue("vecPreviousError",
                           m_vecPreviousError);               // offset: 128/160 size: 16/16 align: 16/16
    deserializer.ReadValue("errorSum", m_errorSum);           // offset: 144/176 size: 4/4 align: 4/4
    deserializer.ReadValue("previousError", m_previousError); // offset: 148/180 size: 4/4 align: 4/4
    deserializer.Skip(8);                                     // offset: 152/184 size: 8/8
    // class size: 160/192 align: 16/16
}
