#include "Havok/hkSweptTransform.h"

REGISTER_HAVOK_POINTER_SOURCE(hkSweptTransform,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkSweptTransform::Class; });

nemesis::hkSweptTransform::hkSweptTransform() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkSweptTransform::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkSweptTransform::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("centerOfMass0", m_centerOfMass0);         // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("centerOfMass1", m_centerOfMass1);         // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("rotation0", m_rotation0);                 // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("rotation1", m_rotation1);                 // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteValue("centerOfMassLocal", m_centerOfMassLocal); // offset: 64/64 size: 16/16 align: 16/16
    // class size: 80/80 align: 16/16
}

void nemesis::hkSweptTransform::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("centerOfMass0", m_centerOfMass0); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("centerOfMass1", m_centerOfMass1); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("rotation0", m_rotation0);         // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("rotation1", m_rotation1);         // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadValue("centerOfMassLocal",
                           m_centerOfMassLocal); // offset: 64/64 size: 16/16 align: 16/16
    // class size: 80/80 align: 16/16
}
