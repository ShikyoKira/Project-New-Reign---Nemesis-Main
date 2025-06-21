#include "Havok/hkpNamedMeshMaterial.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpNamedMeshMaterial,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpNamedMeshMaterial::Class; });

nemesis::hkpNamedMeshMaterial::hkpNamedMeshMaterial() noexcept
    : nemesis::hkpMeshMaterial(Class)
{
}

const nemesis::hkClass* nemesis::hkpNamedMeshMaterial::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpNamedMeshMaterial::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMeshMaterial::SerializeTo(serializer); // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());       // offset: 4/4 size: 0/4

    serializer.WriteValue("name", m_name); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkpNamedMeshMaterial::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMeshMaterial::DeserializeFrom(deserializer); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());         // offset: 4/4 size: 0/4

    deserializer.ReadValue("name", m_name); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}
