#include "Havok/hkbTestStateChooser.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTestStateChooser,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTestStateChooser::Class; });

nemesis::hkbTestStateChooser::hkbTestStateChooser() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbStateChooser(Class)
{
}

const nemesis::hkClass* nemesis::hkbTestStateChooser::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTestStateChooser::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbStateChooser::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() >= HavokVersion::HK_2013_2_0) return;

    serializer.WriteValue("int", m_int);               // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("real", m_real);             // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("string", m_string);         // offset: 16/24 size: 4/8 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbTestStateChooser::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbStateChooser::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() >= HavokVersion::HK_2013_2_0) return;

    deserializer.ReadValue("int", m_int);                    // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("real", m_real);                  // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("string", m_string);              // offset: 16/24 size: 4/8 align: 4/8
    // class size: 20/32 align: 4/8
}
