#include "Havok/hkpMeshMaterial.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMeshMaterial,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMeshMaterial::Class; });

nemesis::hkpMeshMaterial::hkpMeshMaterial() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

nemesis::hkpMeshMaterial::hkpMeshMaterial(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpMeshMaterial::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMeshMaterial::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("filterInfo", m_filterInfo); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}

void nemesis::hkpMeshMaterial::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("filterInfo", m_filterInfo); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}
