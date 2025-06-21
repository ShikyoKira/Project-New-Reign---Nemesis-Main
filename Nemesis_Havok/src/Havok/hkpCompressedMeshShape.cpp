#include "Havok/hkpCompressedMeshShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCompressedMeshShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCompressedMeshShape::Class; });

nemesis::hkpCompressedMeshShape::hkpCompressedMeshShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShapeCollection(Class)
{
}

const nemesis::hkClass* nemesis::hkpCompressedMeshShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCompressedMeshShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShapeCollection::SerializeTo(serializer);    // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteValue("bitsPerIndex", m_bitsPerIndex);   // offset: 24/48 size: 4/4 align: 4/4
    serializer.WriteValue("bitsPerWIndex", m_bitsPerWIndex); // offset: 28/52 size: 4/4 align: 4/4
    serializer.WriteValue("wIndexMask", m_wIndexMask);       // offset: 32/56 size: 4/4 align: 4/4
    serializer.WriteValue("indexMask", m_indexMask);         // offset: 36/60 size: 4/4 align: 4/4
    serializer.WriteValue("radius", m_radius);               // offset: 40/64 size: 4/4 align: 4/4
    serializer.WriteValue("weldingType", m_weldingType);     // offset: 44/68 size: 1/1 align: 1/1
    serializer.WriteValue("materialType", m_materialType);   // offset: 45/69 size: 1/1 align: 1/1
    serializer.Skip(2);                                      // offset: 46/70 size: 2/2

    serializer.WriteObject("materials", m_materials);     // offset: 48/72 size: 12/16 align: 4/8
    serializer.WriteObject("materials16", m_materials16); // offset: 60/88 size: 12/16 align: 4/8
    serializer.WriteObject("materials8", m_materials8);   // offset: 72/104 size: 12/16 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteObject("transforms", m_transforms_1); // offset: 84/120 size: 12/16 align: 4/8
    }
    else
    {
        serializer.WriteObject("transforms", m_transforms_0);
    }

    serializer.WriteObject("bigVertices", m_bigVertices);   // offset: 96/136 size: 12/16 align: 4/8
    serializer.WriteObject("bigTriangles", m_bigTriangles); // offset: 108/152 size: 12/16 align: 4/8
    serializer.WriteObject("chunks", m_chunks);             // offset: 120/168 size: 12/16 align: 4/8
    serializer.WriteObject("convexPieces", m_convexPieces); // offset: 132/184 size: 12/16 align: 4/8
    serializer.WriteValue("error", m_error);                // offset: 144/200 size: 4/4 align: 4/4
    serializer.Pad(16);                                     // offset: 148/204 size: 12/4

    serializer.WriteObject("bounds", m_bounds); // offset: 160/208 size: 32/32 align: 16/16

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteValue("defaultCollisionFilterInfo",
                              m_defaultCollisionFilterInfo); // offset: 192/240 size: 4/4 align: 4/4
        serializer.Pad(serializer.GetPointerSize());         // offset: 196/244 size: 0/4
    }

    serializer.WriteSerializeIgnoredObject("meshMaterials",
                                           m_meshMaterials);       // offset: 196/248 size: 4/8 align: 4/8
    serializer.WriteValue("materialStriding", m_materialStriding); // offset: 200/256 size: 2/2 align: 2/2
    serializer.WriteValue("numMaterials", m_numMaterials);         // offset: 202/258 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                   // offset: 204/260 size: 0/4

    serializer.WriteObject("namedMaterials", m_namedMaterials); // offset: 204/264 size: 12/16 align: 4/8
    serializer.Skip(8);                                         // offset: 216/280 size: 8/8

    serializer.WriteValue("scaling", m_scaling); // offset: 224/288 size: 16/16 align: 16/16
    // class size: 240/304 align: 16/16
}

void nemesis::hkpCompressedMeshShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShapeCollection::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadValue("bitsPerIndex", m_bitsPerIndex);     // offset: 24/48 size: 4/4 align: 4/4
    deserializer.ReadValue("bitsPerWIndex", m_bitsPerWIndex);   // offset: 28/52 size: 4/4 align: 4/4
    deserializer.ReadValue("wIndexMask", m_wIndexMask);         // offset: 32/56 size: 4/4 align: 4/4
    deserializer.ReadValue("indexMask", m_indexMask);           // offset: 36/60 size: 4/4 align: 4/4
    deserializer.ReadValue("radius", m_radius);                 // offset: 40/64 size: 4/4 align: 4/4
    deserializer.ReadValue("weldingType", m_weldingType);       // offset: 44/68 size: 1/1 align: 1/1
    deserializer.ReadValue("materialType", m_materialType);     // offset: 45/69 size: 1/1 align: 1/1
    deserializer.Skip(2);                                       // offset: 46/70 size: 2/2

    deserializer.ReadObject("materials", m_materials);     // offset: 48/72 size: 12/16 align: 4/8
    deserializer.ReadObject("materials16", m_materials16); // offset: 60/88 size: 12/16 align: 4/8
    deserializer.ReadObject("materials8", m_materials8);   // offset: 72/104 size: 12/16 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadObject("transforms", m_transforms_1); // offset: 84/120 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.ReadObject("transforms", m_transforms_0);
    }

    deserializer.ReadObject("bigVertices", m_bigVertices);   // offset: 96/136 size: 12/16 align: 4/8
    deserializer.ReadObject("bigTriangles", m_bigTriangles); // offset: 108/152 size: 12/16 align: 4/8
    deserializer.ReadObject("chunks", m_chunks);             // offset: 120/168 size: 12/16 align: 4/8
    deserializer.ReadObject("convexPieces", m_convexPieces); // offset: 132/184 size: 12/16 align: 4/8
    deserializer.ReadValue("error", m_error);                // offset: 144/200 size: 4/4 align: 4/4
    deserializer.Pad(16);                                    // offset: 148/204 size: 12/4

    deserializer.ReadObject("bounds", m_bounds); // offset: 160/208 size: 32/32 align: 16/16

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadValue("defaultCollisionFilterInfo",
                               m_defaultCollisionFilterInfo); // offset: 192/240 size: 4/4 align: 4/4
        deserializer.Pad(deserializer.GetPointerSize());      // offset: 196/244 size: 0/4
    }

    deserializer.ReadObject("meshMaterials", m_meshMaterials);      // offset: 196/248 size: 4/8 align: 4/8
    deserializer.ReadValue("materialStriding", m_materialStriding); // offset: 200/256 size: 2/2 align: 2/2
    deserializer.ReadValue("numMaterials", m_numMaterials);         // offset: 202/258 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 204/260 size: 0/4

    deserializer.ReadObject("namedMaterials", m_namedMaterials); // offset: 204/264 size: 12/16 align: 4/8
    deserializer.Skip(8);                                        // offset: 216/280 size: 8/8

    deserializer.ReadValue("scaling", m_scaling); // offset: 224/288 size: 16/16 align: 16/16
    // class size: 240/304 align: 16/16
}
