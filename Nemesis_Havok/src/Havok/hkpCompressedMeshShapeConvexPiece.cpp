#include "Havok/hkpCompressedMeshShapeConvexPiece.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCompressedMeshShapeConvexPiece,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCompressedMeshShapeConvexPiece::Class; });

nemesis::hkpCompressedMeshShapeConvexPiece::hkpCompressedMeshShapeConvexPiece() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpCompressedMeshShapeConvexPiece::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCompressedMeshShapeConvexPiece::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("offset", m_offset);      // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteObject("vertices", m_vertices); // offset: 16/16 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        serializer.WriteObject("faceVertices", m_faceVertices); // offset: 28/32 size: 12/16 align: 4/8
        serializer.WriteObject("faceOffsets", m_faceOffsets);   // offset: 40/48 size: 12/16 align: 4/8
    }

    serializer.WriteValue("reference", m_reference);           // offset: 52/64 size: 2/2 align: 2/2
    serializer.WriteValue("transformIndex", m_transformIndex); // offset: 54/66 size: 2/2 align: 2/2
    serializer.Pad(16);                                        // offset: 56/68 size: 8/12
    // class size: 64/80 align: 16/16
}

void nemesis::hkpCompressedMeshShapeConvexPiece::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("offset", m_offset);      // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadObject("vertices", m_vertices); // offset: 16/16 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("faceVertices", m_faceVertices); // offset: 28/32 size: 12/16 align: 4/8
        deserializer.ReadObject("faceOffsets", m_faceOffsets);   // offset: 40/48 size: 12/16 align: 4/8
    }

    deserializer.ReadValue("reference", m_reference);           // offset: 52/64 size: 2/2 align: 2/2
    deserializer.ReadValue("transformIndex", m_transformIndex); // offset: 54/66 size: 2/2 align: 2/2
    deserializer.Pad(16);                                       // offset: 56/68 size: 8/12
    // class size: 64/80 align: 16/16
}
