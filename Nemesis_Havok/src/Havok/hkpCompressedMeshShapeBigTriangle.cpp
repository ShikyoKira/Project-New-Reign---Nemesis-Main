#include "Havok/hkpCompressedMeshShapeBigTriangle.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCompressedMeshShapeBigTriangle,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCompressedMeshShapeBigTriangle::Class; });

nemesis::hkpCompressedMeshShapeBigTriangle::hkpCompressedMeshShapeBigTriangle() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpCompressedMeshShapeBigTriangle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCompressedMeshShapeBigTriangle::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("a", m_a); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("b", m_b); // offset: 2/2 size: 2/2 align: 2/2
    serializer.WriteValue("c", m_c); // offset: 4/4 size: 2/2 align: 2/2
    serializer.Skip(2);              // offset: 6/6 size: 2/2

    serializer.WriteValue("material", m_material);       // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("weldingInfo", m_weldingInfo); // offset: 12/12 size: 2/2 align: 2/2

    if (serializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        serializer.WriteValue("transformIndex", m_transformIndex); // offset: 14/14 size: 2/2 align: 2/2
    }
    else
    {
        serializer.Skip(2);
    }

    // class size: 16/16 align: 4/4
}

void nemesis::hkpCompressedMeshShapeBigTriangle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("a", m_a); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("b", m_b); // offset: 2/2 size: 2/2 align: 2/2
    deserializer.ReadValue("c", m_c); // offset: 4/4 size: 2/2 align: 2/2
    deserializer.Skip(2);             // offset: 6/6 size: 2/2

    deserializer.ReadValue("material", m_material);       // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("weldingInfo", m_weldingInfo); // offset: 12/12 size: 2/2 align: 2/2

    if (deserializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        deserializer.ReadValue("transformIndex", m_transformIndex); // offset: 14/14 size: 2/2 align: 2/2
    }
    else
    {
        deserializer.Skip(2);
    }

    // class size: 16/16 align: 4/4
}
