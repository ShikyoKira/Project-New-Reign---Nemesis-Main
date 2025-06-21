#include "Havok/hkMeshBoneIndexMapping.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMeshBoneIndexMapping,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMeshBoneIndexMapping::Class; });

nemesis::hkMeshBoneIndexMapping::hkMeshBoneIndexMapping() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkMeshBoneIndexMapping::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMeshBoneIndexMapping::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("mapping", m_mapping); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}

void nemesis::hkMeshBoneIndexMapping::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("mapping", m_mapping); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}
