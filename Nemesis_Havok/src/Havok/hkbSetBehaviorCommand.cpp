#include "Havok/hkbSetBehaviorCommand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSetBehaviorCommand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSetBehaviorCommand::Class; });

nemesis::hkbSetBehaviorCommand::hkbSetBehaviorCommand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbSetBehaviorCommand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSetBehaviorCommand::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);      // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteObject("behavior", m_behavior);           // offset: 16/24 size: 4/8 align: 4/8
    serializer.WriteObject("rootGenerator", m_rootGenerator); // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteObject("referencedBehaviors",
                           m_referencedBehaviors);               // offset: 24/40 size: 12/16 align: 4/8
    serializer.WriteValue("startStateIndex", m_startStateIndex); // offset: 36/56 size: 4/4 align: 4/4
    serializer.WriteValue("randomizeSimulation", m_randomizeSimulation); // offset: 40/60 size: 1/1 align: 1/1
    serializer.Skip(3);                                                  // offset: 41/61 size: 3/3

    serializer.WriteValue("padding", m_padding); // offset: 44/64 size: 4/4 align: 4/4
    serializer.Pad(8);                           // offset: 48/68 size: 0/4
    // class size: 48/72 align: 8/8
}

void nemesis::hkbSetBehaviorCommand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadObject("behavior", m_behavior);            // offset: 16/24 size: 4/8 align: 4/8
    deserializer.ReadObject("rootGenerator", m_rootGenerator);  // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadObject("referencedBehaviors",
                            m_referencedBehaviors);               // offset: 24/40 size: 12/16 align: 4/8
    deserializer.ReadValue("startStateIndex", m_startStateIndex); // offset: 36/56 size: 4/4 align: 4/4
    deserializer.ReadValue("randomizeSimulation",
                           m_randomizeSimulation); // offset: 40/60 size: 1/1 align: 1/1
    deserializer.Skip(3);                          // offset: 41/61 size: 3/3

    deserializer.ReadValue("padding", m_padding); // offset: 44/64 size: 4/4 align: 4/4
    deserializer.Pad(8);                          // offset: 48/68 size: 0/4
    // class size: 48/72 align: 8/8
}
