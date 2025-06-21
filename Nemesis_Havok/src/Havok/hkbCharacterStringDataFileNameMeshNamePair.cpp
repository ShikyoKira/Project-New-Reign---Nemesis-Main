#include "Havok/hkbCharacterStringDataFileNameMeshNamePair.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterStringDataFileNameMeshNamePair,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterStringDataFileNameMeshNamePair::Class; });

nemesis::hkbCharacterStringDataFileNameMeshNamePair::hkbCharacterStringDataFileNameMeshNamePair() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbCharacterStringDataFileNameMeshNamePair::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterStringDataFileNameMeshNamePair::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("fileName", m_fileName); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("meshName", m_meshName); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbCharacterStringDataFileNameMeshNamePair::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("fileName", m_fileName); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("meshName", m_meshName); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}
