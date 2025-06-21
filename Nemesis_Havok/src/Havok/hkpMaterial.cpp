#include "Havok/hkpMaterial.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMaterial,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMaterial::Class; });

nemesis::hkpMaterial::hkpMaterial() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpMaterial::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMaterial::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("responseType", m_responseType); // offset: 0/0 size: 1/1 align: 1/1
    serializer.Skip(1);                                    // offset: 1/1 size: 1/1

    serializer.WriteValue("rollingFrictionMultiplier",
                          m_rollingFrictionMultiplier);  // offset: 2/2 size: 2/2 align: 2/2
    serializer.WriteValue("friction", m_friction);       // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("restitution", m_restitution); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}

void nemesis::hkpMaterial::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("responseType", m_responseType); // offset: 0/0 size: 1/1 align: 1/1
    deserializer.Skip(1);                                   // offset: 1/1 size: 1/1

    deserializer.ReadValue("rollingFrictionMultiplier",
                           m_rollingFrictionMultiplier);  // offset: 2/2 size: 2/2 align: 2/2
    deserializer.ReadValue("friction", m_friction);       // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("restitution", m_restitution); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}
