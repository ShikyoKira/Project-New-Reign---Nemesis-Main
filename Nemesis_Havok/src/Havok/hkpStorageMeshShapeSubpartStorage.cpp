#include "Havok/hkpStorageMeshShapeSubpartStorage.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStorageMeshShapeSubpartStorage,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStorageMeshShapeSubpartStorage::Class; });

nemesis::hkpStorageMeshShapeSubpartStorage::hkpStorageMeshShapeSubpartStorage() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpStorageMeshShapeSubpartStorage::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStorageMeshShapeSubpartStorage::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("vertices", m_vertices);                   // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("indices16", m_indices16);                 // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("indices32", m_indices32);                 // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("materialIndices", m_materialIndices);     // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteObject("materials", m_materials);                 // offset: 56/80 size: 12/16 align: 4/8
    serializer.WriteObject("materialIndices16", m_materialIndices16); // offset: 68/96 size: 12/16 align: 4/8
    // class size: 80/112 align: 4/8
}

void nemesis::hkpStorageMeshShapeSubpartStorage::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);        // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("vertices", m_vertices);                   // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("indices16", m_indices16);                 // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("indices32", m_indices32);                 // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("materialIndices", m_materialIndices);     // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadObject("materials", m_materials);                 // offset: 56/80 size: 12/16 align: 4/8
    deserializer.ReadObject("materialIndices16", m_materialIndices16); // offset: 68/96 size: 12/16 align: 4/8
    // class size: 80/112 align: 4/8
}
