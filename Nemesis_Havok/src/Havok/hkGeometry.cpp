#include "Havok/hkGeometry.h"

REGISTER_HAVOK_POINTER_SOURCE(hkGeometry,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkGeometry::Class; });

nemesis::hkGeometry::hkGeometry() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkGeometry::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkGeometry::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer);
    }

    if (cur_ver > HavokVersion::HK_660)
    {
        serializer.WriteObject("vertices", m_vertices);   // offset: 0/0 size: 12/16 align: 4/8
    }
    serializer.WriteObject("triangles", m_triangles); // offset: 12/16 size: 12/16 align: 4/8
    // class size: 24/32 align: 4/8
}

void nemesis::hkGeometry::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer);
    }

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.ReadObject("vertices", m_vertices);   // offset: 0/0 size: 12/16 align: 4/8
    }

    deserializer.ReadObject("triangles", m_triangles); // offset: 12/16 size: 12/16 align: 4/8
    // class size: 24/32 align: 4/8
}
