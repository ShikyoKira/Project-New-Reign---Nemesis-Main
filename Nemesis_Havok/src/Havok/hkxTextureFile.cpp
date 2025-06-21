#include "Havok/hkxTextureFile.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxTextureFile,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxTextureFile::Class; });

nemesis::hkxTextureFile::hkxTextureFile() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxTextureFile::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxTextureFile::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("filename", m_filename);                 // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("name", m_name);                         // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("originalFilename", m_originalFilename); // offset: 16/32 size: 4/8 align: 4/8
    // class size: 20/40 align: 4/8
}

void nemesis::hkxTextureFile::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("filename", m_filename);                 // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("name", m_name);                         // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("originalFilename", m_originalFilename); // offset: 16/32 size: 4/8 align: 4/8
    // class size: 20/40 align: 4/8
}
