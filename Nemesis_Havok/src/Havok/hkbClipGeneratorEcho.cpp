#include "Havok/hkbClipGeneratorEcho.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbClipGeneratorEcho,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbClipGeneratorEcho::Class; });

nemesis::hkbClipGeneratorEcho::hkbClipGeneratorEcho() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbClipGeneratorEcho::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbClipGeneratorEcho::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("offsetLocalTime", m_offsetLocalTime); // offset: 0/0 size: 4/4 align: 16/16
    serializer.WriteValue("weight", m_weight);                   // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("dwdt", m_dwdt);                       // offset: 8/8 size: 4/4 align: 4/4
    serializer.Skip(4);                                          // offset: 12/12 size: 4/4
    // class size: 16/16 align: 16/16
}

void nemesis::hkbClipGeneratorEcho::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("offsetLocalTime", m_offsetLocalTime); // offset: 0/0 size: 4/4 align: 16/16
    deserializer.ReadValue("weight", m_weight);                   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("dwdt", m_dwdt);                       // offset: 8/8 size: 4/4 align: 4/4
    deserializer.Skip(4);                                         // offset: 12/12 size: 4/4
    // class size: 16/16 align: 16/16
}
