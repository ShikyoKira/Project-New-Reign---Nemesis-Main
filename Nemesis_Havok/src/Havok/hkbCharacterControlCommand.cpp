#include "Havok/hkbCharacterControlCommand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterControlCommand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterControlCommand::Class; });

nemesis::hkbCharacterControlCommand::hkbCharacterControlCommand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterControlCommand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterControlCommand::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("command", m_command);          // offset: 16/24 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.Skip(3);                          // offset: 17/25 size: 3/3
        serializer.WriteValue("padding", m_padding); // offset: 20/28 size: 4/4 align: 4/4
    }
    else
    {
        serializer.Skip(7);
    }

    // class size: 24/32 align: 8/8
}

void nemesis::hkbCharacterControlCommand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("command", m_command);               // offset: 16/24 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.Skip(3);                         // offset: 17/25 size: 3/3
        deserializer.ReadValue("padding", m_padding); // offset: 20/28 size: 4/4 align: 4/4
    }
    else
    {
        deserializer.Skip(7);
    }

    // class size: 24/32 align: 8/8
}
