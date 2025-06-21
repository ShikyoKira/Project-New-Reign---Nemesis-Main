#include "Havok/hkpConvexPieceStreamData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexPieceStreamData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexPieceStreamData::Class; });

nemesis::hkpConvexPieceStreamData::hkpConvexPieceStreamData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpConvexPieceStreamData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexPieceStreamData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("convexPieceStream", m_convexPieceStream); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("convexPieceOffsets",
                           m_convexPieceOffsets); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("convexPieceSingleTriangles",
                           m_convexPieceSingleTriangles); // offset: 32/48 size: 12/16 align: 4/8
    // class size: 44/64 align: 4/8
}

void nemesis::hkpConvexPieceStreamData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);        // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("convexPieceStream", m_convexPieceStream); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("convexPieceOffsets",
                            m_convexPieceOffsets); // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("convexPieceSingleTriangles",
                            m_convexPieceSingleTriangles); // offset: 32/48 size: 12/16 align: 4/8
    // class size: 44/64 align: 4/8
}
