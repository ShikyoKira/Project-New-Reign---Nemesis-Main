#include "Havok/hkContactPointMaterial.h"

REGISTER_HAVOK_POINTER_SOURCE(hkContactPointMaterial,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkContactPointMaterial::Class; });

nemesis::hkContactPointMaterial::hkContactPointMaterial() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkContactPointMaterial::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkContactPointMaterial::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("userData", m_userData);       // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("friction", m_friction);       // offset: 4/8 size: 1/1 align: 1/1
    serializer.WriteValue("restitution", m_restitution); // offset: 5/9 size: 1/1 align: 1/1
    serializer.WriteValue("maxImpulse", m_maxImpulse);   // offset: 6/10 size: 1/1 align: 1/1
    serializer.WriteValue("flags", m_flags);             // offset: 7/11 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());         // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkContactPointMaterial::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("userData", m_userData);       // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("friction", m_friction);       // offset: 4/8 size: 1/1 align: 1/1
    deserializer.ReadValue("restitution", m_restitution); // offset: 5/9 size: 1/1 align: 1/1
    deserializer.ReadValue("maxImpulse", m_maxImpulse);   // offset: 6/10 size: 1/1 align: 1/1
    deserializer.ReadValue("flags", m_flags);             // offset: 7/11 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}
