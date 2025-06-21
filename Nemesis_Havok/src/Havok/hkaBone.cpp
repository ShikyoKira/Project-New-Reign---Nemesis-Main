#include "Havok/hkaBone.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaBone,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaBone::Class; });

nemesis::hkaBone::hkaBone() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaBone::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaBone::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);                       // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("lockTranslation", m_lockTranslation); // offset: 4/8 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                 // offset: 5/9 size: 3/7
    // class size: 8/16 align: 4/8
}

void nemesis::hkaBone::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);                       // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("lockTranslation", m_lockTranslation); // offset: 4/8 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 5/9 size: 3/7
    // class size: 8/16 align: 4/8
}
