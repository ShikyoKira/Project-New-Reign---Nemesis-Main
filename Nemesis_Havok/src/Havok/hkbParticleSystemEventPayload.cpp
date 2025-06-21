#include "Havok/hkbParticleSystemEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbParticleSystemEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbParticleSystemEventPayload::Class; });

nemesis::hkbParticleSystemEventPayload::hkbParticleSystemEventPayload() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbParticleSystemEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbParticleSystemEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventPayload::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("type", m_type);             // offset: 8/16 size: 1/1 align: 1/1
    serializer.Skip(1);                                // offset: 9/17 size: 1/1

    serializer.WriteValue("emitBoneIndex", m_emitBoneIndex); // offset: 10/18 size: 2/2 align: 2/2
    serializer.Pad(16);                                      // offset: 12/20 size: 4/12

    serializer.WriteValue("offset", m_offset);             // offset: 16/32 size: 16/16 align: 16/16
    serializer.WriteValue("direction", m_direction);       // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("numParticles", m_numParticles); // offset: 48/64 size: 4/4 align: 4/4
    serializer.WriteValue("speed", m_speed);               // offset: 52/68 size: 4/4 align: 4/4
    serializer.Skip(8);                                    // offset: 56/72 size: 8/8
    // class size: 64/80 align: 16/16
}

void nemesis::hkbParticleSystemEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("type", m_type);                  // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Skip(1);                                    // offset: 9/17 size: 1/1

    deserializer.ReadValue("emitBoneIndex", m_emitBoneIndex); // offset: 10/18 size: 2/2 align: 2/2
    deserializer.Pad(16);                                     // offset: 12/20 size: 4/12

    deserializer.ReadValue("offset", m_offset);             // offset: 16/32 size: 16/16 align: 16/16
    deserializer.ReadValue("direction", m_direction);       // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("numParticles", m_numParticles); // offset: 48/64 size: 4/4 align: 4/4
    deserializer.ReadValue("speed", m_speed);               // offset: 52/68 size: 4/4 align: 4/4
    deserializer.Skip(8);                                   // offset: 56/72 size: 8/8
    // class size: 64/80 align: 16/16
}
