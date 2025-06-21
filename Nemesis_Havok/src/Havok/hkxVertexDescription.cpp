#include "Havok/hkxVertexDescription.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexDescription,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexDescription::Class; });

nemesis::hkxVertexDescription::hkxVertexDescription() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxVertexDescription::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexDescription::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        serializer.WriteValue("stride", m_stride);
        serializer.Pad(serializer.GetPointerSize());
    }

    serializer.WriteObject("decls", m_decls); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}

void nemesis::hkxVertexDescription::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        deserializer.ReadValue("stride", m_stride);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    deserializer.ReadObject("decls", m_decls); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}
