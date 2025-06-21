#include "Havok/hkpConvexVerticesShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexVerticesShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexVerticesShape::Class; });

nemesis::hkpConvexVerticesShape::hkpConvexVerticesShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpConvexVerticesShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexVerticesShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer); // offset: 0/0 size: 20/40 align: 4/8
    serializer.Pad(16);                               // offset: 20/40 size: 12/8

    serializer.WriteValue("aabbHalfExtents", m_aabbHalfExtents);  // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("aabbCenter", m_aabbCenter);            // offset: 48/64 size: 16/16 align: 16/16

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        serializer.WriteObject("rotatedVertices", m_rotatedVertices_2);
    }
    else if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteObject("rotatedVertices", m_rotatedVertices_1);
    }
    else
    {
        serializer.WriteObject("rotatedVertices",
                               m_rotatedVertices_0); // offset: 64/80 size: 12/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_660 && cur_ver <= HavokVersion::HK_710)
    {
        serializer.WriteSerializeIgnoredObject("convexPiece", m_convexPiece);
    }

    serializer.WriteValue("numVertices", m_numVertices);          // offset: 76/96 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                  // offset: 80/100 size: 0/4

    serializer.WriteSerializeIgnoredObject("externalObject",
                                           m_externalObject); // offset: 80/104 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("getFaceNormals",
                                           m_getFaceNormals);   // offset: 84/112 size: 4/8 align: 4/8
    serializer.WriteObject("planeEquations", m_planeEquations); // offset: 88/120 size: 12/16 align: 4/8
    serializer.WriteObject("connectivity", m_connectivity);     // offset: 100/136 size: 4/8 align: 4/8
    serializer.Pad(16);                                         // offset: 104/144 size: 8/0
    // class size: 112/144 align: 16/16
}

void nemesis::hkpConvexVerticesShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.Pad(16);                                   // offset: 20/40 size: 12/8

    deserializer.ReadValue("aabbHalfExtents", m_aabbHalfExtents);  // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("aabbCenter", m_aabbCenter);            // offset: 48/64 size: 16/16 align: 16/16

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        deserializer.ReadObject("rotatedVertices", m_rotatedVertices_2);
    }
    else if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("rotatedVertices", m_rotatedVertices_1);
    }
    else
    {
        deserializer.ReadObject("rotatedVertices",
                                m_rotatedVertices_0); // offset: 64/80 size: 12/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_660 && cur_ver <= HavokVersion::HK_710)
    {
        deserializer.ReadObject("convexPiece", m_convexPiece);
    }

    deserializer.ReadValue("numVertices", m_numVertices);          // offset: 76/96 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());               // offset: 80/100 size: 0/4

    deserializer.ReadObject("externalObject", m_externalObject); // offset: 80/104 size: 4/8 align: 4/8
    deserializer.ReadObject("getFaceNormals", m_getFaceNormals); // offset: 84/112 size: 4/8 align: 4/8
    deserializer.ReadObject("planeEquations", m_planeEquations); // offset: 88/120 size: 12/16 align: 4/8
    deserializer.ReadObject("connectivity", m_connectivity);     // offset: 100/136 size: 4/8 align: 4/8
    deserializer.Pad(16);                                        // offset: 104/144 size: 8/0
    // class size: 112/144 align: 16/16
}
