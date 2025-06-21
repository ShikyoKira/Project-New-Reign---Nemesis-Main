#include "Havok/hkbSetWordVariableCommand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSetWordVariableCommand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSetWordVariableCommand::Class; });

nemesis::hkbSetWordVariableCommand::hkbSetWordVariableCommand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbSetWordVariableCommand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSetWordVariableCommand::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("quadValue", m_quadValue);     // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("characterId", m_characterId); // offset: 32/32 size: 8/8 align: 8/8
    serializer.WriteValue("variableId", m_variableId);   // offset: 40/40 size: 4/4 align: 4/4
    serializer.WriteObject("value", m_value);            // offset: 44/44 size: 4/4 align: 4/4
    serializer.WriteValue("type", m_type);               // offset: 48/48 size: 1/1 align: 1/1
    serializer.WriteValue("global", m_global);           // offset: 49/49 size: 1/1 align: 1/1
    serializer.Skip(14);                                 // offset: 50/50 size: 14/14
    // class size: 64/64 align: 16/16
}

void nemesis::hkbSetWordVariableCommand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("quadValue", m_quadValue);     // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("characterId", m_characterId); // offset: 32/32 size: 8/8 align: 8/8
    deserializer.ReadValue("variableId", m_variableId);   // offset: 40/40 size: 4/4 align: 4/4
    deserializer.ReadObject("value", m_value);            // offset: 44/44 size: 4/4 align: 4/4
    deserializer.ReadValue("type", m_type);               // offset: 48/48 size: 1/1 align: 1/1
    deserializer.ReadValue("global", m_global);           // offset: 49/49 size: 1/1 align: 1/1
    deserializer.Skip(14);                                // offset: 50/50 size: 14/14
    // class size: 64/64 align: 16/16
}
