#include "Havok/hkpMeshShapeSubpart.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMeshShapeSubpart,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMeshShapeSubpart::Class; });

nemesis::hkpMeshShapeSubpart::hkpMeshShapeSubpart() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpMeshShapeSubpart::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMeshShapeSubpart::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("vertexBase", m_vertexBase); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("vertexStriding", m_vertexStriding);          // offset: 4/8 size: 4/4 align: 4/4
    serializer.WriteValue("numVertices", m_numVertices);                // offset: 8/12 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredObject("indexBase", m_indexBase);   // offset: 12/16 size: 4/8 align: 4/8
    serializer.WriteValue("stridingType", m_stridingType);              // offset: 16/24 size: 1/1 align: 1/1
    serializer.WriteValue("materialIndexStridingType",
                          m_materialIndexStridingType); // offset: 17/25 size: 1/1 align: 1/1
    serializer.Skip(2);                                 // offset: 18/26 size: 2/2

    serializer.WriteValue("indexStriding", m_indexStriding); // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteValue("flipAlternateTriangles",
                          m_flipAlternateTriangles);       // offset: 24/32 size: 4/4 align: 4/4
    serializer.WriteValue("numTriangles", m_numTriangles); // offset: 28/36 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredObject("materialIndexBase",
                                           m_materialIndexBase); // offset: 32/40 size: 4/8 align: 4/8
    serializer.WriteValue("materialIndexStriding",
                          m_materialIndexStriding); // offset: 36/48 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());    // offset: 40/52 size: 0/4

    serializer.WriteSerializeIgnoredObject("materialBase",
                                           m_materialBase);        // offset: 40/56 size: 4/8 align: 4/8
    serializer.WriteValue("materialStriding", m_materialStriding); // offset: 44/64 size: 4/4 align: 4/4
    serializer.WriteValue("numMaterials", m_numMaterials);         // offset: 48/68 size: 4/4 align: 4/4
    serializer.WriteValue("triangleOffset", m_triangleOffset);     // offset: 52/72 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 56/76 size: 0/4
    // class size: 56/80 align: 4/8
}

void nemesis::hkpMeshShapeSubpart::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("vertexBase", m_vertexBase);        // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("vertexStriding", m_vertexStriding); // offset: 4/8 size: 4/4 align: 4/4
    deserializer.ReadValue("numVertices", m_numVertices);       // offset: 8/12 size: 4/4 align: 4/4
    deserializer.ReadObject("indexBase", m_indexBase);          // offset: 12/16 size: 4/8 align: 4/8
    deserializer.ReadValue("stridingType", m_stridingType);     // offset: 16/24 size: 1/1 align: 1/1
    deserializer.ReadValue("materialIndexStridingType",
                           m_materialIndexStridingType); // offset: 17/25 size: 1/1 align: 1/1
    deserializer.Skip(2);                                // offset: 18/26 size: 2/2

    deserializer.ReadValue("indexStriding", m_indexStriding); // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadValue("flipAlternateTriangles",
                           m_flipAlternateTriangles);                  // offset: 24/32 size: 4/4 align: 4/4
    deserializer.ReadValue("numTriangles", m_numTriangles);            // offset: 28/36 size: 4/4 align: 4/4
    deserializer.ReadObject("materialIndexBase", m_materialIndexBase); // offset: 32/40 size: 4/8 align: 4/8
    deserializer.ReadValue("materialIndexStriding",
                           m_materialIndexStriding); // offset: 36/48 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 40/52 size: 0/4

    deserializer.ReadObject("materialBase", m_materialBase);        // offset: 40/56 size: 4/8 align: 4/8
    deserializer.ReadValue("materialStriding", m_materialStriding); // offset: 44/64 size: 4/4 align: 4/4
    deserializer.ReadValue("numMaterials", m_numMaterials);         // offset: 48/68 size: 4/4 align: 4/4
    deserializer.ReadValue("triangleOffset", m_triangleOffset);     // offset: 52/72 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 56/76 size: 0/4
    // class size: 56/80 align: 4/8
}
