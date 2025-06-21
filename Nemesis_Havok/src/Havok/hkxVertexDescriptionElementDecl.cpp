#include "Havok/hkxVertexDescriptionElementDecl.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexDescriptionElementDecl,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexDescriptionElementDecl::Class; });

nemesis::hkxVertexDescriptionElementDecl::hkxVertexDescriptionElementDecl() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxVertexDescriptionElementDecl::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexDescriptionElementDecl::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("byteOffset", m_byteOffset); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("type", m_type);             // offset: 4/4 size: 2/2 align: 2/2
    serializer.WriteValue("usage", m_usage);           // offset: 6/6 size: 2/2 align: 2/2

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        serializer.WriteValue("byteStride", m_byteStride); // offset: 8/8 size: 4/4 align: 4/4
    }

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("numElements", m_numElements); // offset: 12/12 size: 1/1 align: 1/1
        serializer.Skip(3);                                  // offset: 13/13 size: 3/3
    }

    if (cur_ver > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteValue("channelID", m_channelID);
    }
    else if (cur_ver > HavokVersion::HK_2012_1_0)
    {
        serializer.WriteValue("hint", m_hint);
        serializer.Skip(2);
    }

    // class size: 16/16 align: 4/4
}

void nemesis::hkxVertexDescriptionElementDecl::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("byteOffset", m_byteOffset); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("type", m_type);             // offset: 4/4 size: 2/2 align: 2/2
    deserializer.ReadValue("usage", m_usage);           // offset: 6/6 size: 2/2 align: 2/2

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.ReadValue("byteStride", m_byteStride); // offset: 8/8 size: 4/4 align: 4/4
    }

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("numElements", m_numElements); // offset: 12/12 size: 1/1 align: 1/1
        deserializer.Skip(3);                                 // offset: 13/13 size: 3/3
    }

    if (cur_ver > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadValue("channelID", m_channelID);
    }
    else if (cur_ver > HavokVersion::HK_2012_1_0)
    {
        deserializer.ReadValue("hint", m_hint);
        deserializer.Skip(2);
    }

    // class size: 16/16 align: 4/4
}
