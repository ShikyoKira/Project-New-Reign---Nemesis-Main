#include "Havok/hkxTextureInplace.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxTextureInplace,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxTextureInplace::Class; });

nemesis::hkxTextureInplace::hkxTextureInplace() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxTextureInplace::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxTextureInplace::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("fileType", m_fileType, true); // offset: 8/16 size: 4/4 align: 1/1
    serializer.Pad(serializer.GetPointerSize());         // offset: 12/20 size: 0/4

    serializer.WriteObject("data", m_data);                        // offset: 12/24 size: 12/16 align: 4/8
    serializer.WriteValue("name", m_name);                         // offset: 24/40 size: 4/8 align: 4/8
    serializer.WriteValue("originalFilename", m_originalFilename); // offset: 28/48 size: 4/8 align: 4/8
    // class size: 32/56 align: 4/8
}

void nemesis::hkxTextureInplace::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("fileType", m_fileType, true); // offset: 8/16 size: 4/4 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 12/20 size: 0/4

    deserializer.ReadObject("data", m_data);                        // offset: 12/24 size: 12/16 align: 4/8
    deserializer.ReadValue("name", m_name);                         // offset: 24/40 size: 4/8 align: 4/8
    deserializer.ReadValue("originalFilename", m_originalFilename); // offset: 28/48 size: 4/8 align: 4/8
    // class size: 32/56 align: 4/8
}
