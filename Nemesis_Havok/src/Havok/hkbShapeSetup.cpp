#include "Havok/hkbShapeSetup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbShapeSetup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbShapeSetup::Class; });

nemesis::hkbShapeSetup::hkbShapeSetup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbShapeSetup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbShapeSetup::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("capsuleHeight", m_capsuleHeight); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("capsuleRadius", m_capsuleRadius); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("fileName", m_fileName);           // offset: 8/8 size: 4/8 align: 4/8
    serializer.WriteValue("type", m_type);                   // offset: 12/16 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());             // offset: 13/17 size: 3/7
    // class size: 16/24 align: 4/8
}

void nemesis::hkbShapeSetup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("capsuleHeight", m_capsuleHeight); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("capsuleRadius", m_capsuleRadius); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("fileName", m_fileName);           // offset: 8/8 size: 4/8 align: 4/8
    deserializer.ReadValue("type", m_type);                   // offset: 12/16 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());          // offset: 13/17 size: 3/7
    // class size: 16/24 align: 4/8
}
