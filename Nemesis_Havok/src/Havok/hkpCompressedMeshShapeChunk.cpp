#include "Havok/hkpCompressedMeshShapeChunk.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCompressedMeshShapeChunk,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCompressedMeshShapeChunk::Class; });

nemesis::hkpCompressedMeshShapeChunk::hkpCompressedMeshShapeChunk() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpCompressedMeshShapeChunk::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCompressedMeshShapeChunk::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("offset", m_offset);                 // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteObject("vertices", m_vertices);            // offset: 16/16 size: 12/16 align: 4/8
    serializer.WriteObject("indices", m_indices);              // offset: 28/32 size: 12/16 align: 4/8
    serializer.WriteObject("stripLengths", m_stripLengths);    // offset: 40/48 size: 12/16 align: 4/8
    serializer.WriteObject("weldingInfo", m_weldingInfo);      // offset: 52/64 size: 12/16 align: 4/8
    serializer.WriteValue("materialInfo", m_materialInfo);     // offset: 64/80 size: 4/4 align: 4/4
    serializer.WriteValue("reference", m_reference);           // offset: 68/84 size: 2/2 align: 2/2
    serializer.WriteValue("transformIndex", m_transformIndex); // offset: 70/86 size: 2/2 align: 2/2
    serializer.Skip(8);                                        // offset: 72/88 size: 8/8
    // class size: 80/96 align: 16/16
}

void nemesis::hkpCompressedMeshShapeChunk::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("offset", m_offset);                 // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadObject("vertices", m_vertices);            // offset: 16/16 size: 12/16 align: 4/8
    deserializer.ReadObject("indices", m_indices);              // offset: 28/32 size: 12/16 align: 4/8
    deserializer.ReadObject("stripLengths", m_stripLengths);    // offset: 40/48 size: 12/16 align: 4/8
    deserializer.ReadObject("weldingInfo", m_weldingInfo);      // offset: 52/64 size: 12/16 align: 4/8
    deserializer.ReadValue("materialInfo", m_materialInfo);     // offset: 64/80 size: 4/4 align: 4/4
    deserializer.ReadValue("reference", m_reference);           // offset: 68/84 size: 2/2 align: 2/2
    deserializer.ReadValue("transformIndex", m_transformIndex); // offset: 70/86 size: 2/2 align: 2/2
    deserializer.Skip(8);                                       // offset: 72/88 size: 8/8
    // class size: 80/96 align: 16/16
}
