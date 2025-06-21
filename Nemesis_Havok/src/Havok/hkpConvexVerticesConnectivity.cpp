#include "Havok/hkpConvexVerticesConnectivity.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexVerticesConnectivity,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexVerticesConnectivity::Class; });

nemesis::hkpConvexVerticesConnectivity::hkpConvexVerticesConnectivity() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpConvexVerticesConnectivity::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexVerticesConnectivity::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("vertexIndices", m_vertexIndices); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("numVerticesPerFace",
                           m_numVerticesPerFace); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkpConvexVerticesConnectivity::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("vertexIndices", m_vertexIndices);  // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("numVerticesPerFace",
                            m_numVerticesPerFace); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
