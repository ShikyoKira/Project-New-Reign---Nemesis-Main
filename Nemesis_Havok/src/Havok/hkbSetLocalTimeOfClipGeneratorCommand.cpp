#include "Havok/hkbSetLocalTimeOfClipGeneratorCommand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSetLocalTimeOfClipGeneratorCommand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSetLocalTimeOfClipGeneratorCommand::Class; });

nemesis::hkbSetLocalTimeOfClipGeneratorCommand::hkbSetLocalTimeOfClipGeneratorCommand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbSetLocalTimeOfClipGeneratorCommand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSetLocalTimeOfClipGeneratorCommand::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("localTime", m_localTime);      // offset: 16/24 size: 4/4 align: 4/4
    serializer.WriteValue("nodeId", m_nodeId);            // offset: 20/28 size: 2/2 align: 2/2
    serializer.Skip(2);                                   // offset: 22/30 size: 2/2
    // class size: 24/32 align: 8/8
}

void nemesis::hkbSetLocalTimeOfClipGeneratorCommand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("localTime", m_localTime);           // offset: 16/24 size: 4/4 align: 4/4
    deserializer.ReadValue("nodeId", m_nodeId);                 // offset: 20/28 size: 2/2 align: 2/2
    deserializer.Skip(2);                                       // offset: 22/30 size: 2/2
    // class size: 24/32 align: 8/8
}
