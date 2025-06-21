#include "Havok/hkbRaiseEventCommand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRaiseEventCommand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRaiseEventCommand::Class; });

nemesis::hkbRaiseEventCommand::hkbRaiseEventCommand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbRaiseEventCommand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRaiseEventCommand::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("global", m_global);            // offset: 16/24 size: 1/1 align: 1/1
    serializer.Skip(3);                                   // offset: 17/25 size: 3/3

    serializer.WriteValue("externalId", m_externalId); // offset: 20/28 size: 4/4 align: 4/4
    // class size: 24/32 align: 8/8
}

void nemesis::hkbRaiseEventCommand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("global", m_global);                 // offset: 16/24 size: 1/1 align: 1/1
    deserializer.Skip(3);                                       // offset: 17/25 size: 3/3

    deserializer.ReadValue("externalId", m_externalId); // offset: 20/28 size: 4/4 align: 4/4
    // class size: 24/32 align: 8/8
}
