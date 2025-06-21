#include "Havok/hkpSpringAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSpringAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSpringAction::Class; });

nemesis::hkpSpringAction::hkpSpringAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBinaryAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpSpringAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSpringAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBinaryAction::SerializeTo(serializer);       // offset: 0/0 size: 32/64 align: 4/8
    serializer.WriteValue("lastForce", m_lastForce);         // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("positionAinA", m_positionAinA);   // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("positionBinB", m_positionBinB);   // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("restLength", m_restLength);       // offset: 80/112 size: 4/4 align: 4/4
    serializer.WriteValue("strength", m_strength);           // offset: 84/116 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);             // offset: 88/120 size: 4/4 align: 4/4
    serializer.WriteValue("onCompression", m_onCompression); // offset: 92/124 size: 1/1 align: 1/1
    serializer.WriteValue("onExtension", m_onExtension);     // offset: 93/125 size: 1/1 align: 1/1
    serializer.Skip(2);                                      // offset: 94/126 size: 2/2
    // class size: 96/128 align: 16/16
}

void nemesis::hkpSpringAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBinaryAction::DeserializeFrom(deserializer);  // offset: 0/0 size: 32/64 align: 4/8
    deserializer.ReadValue("lastForce", m_lastForce);         // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("positionAinA", m_positionAinA);   // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("positionBinB", m_positionBinB);   // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("restLength", m_restLength);       // offset: 80/112 size: 4/4 align: 4/4
    deserializer.ReadValue("strength", m_strength);           // offset: 84/116 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);             // offset: 88/120 size: 4/4 align: 4/4
    deserializer.ReadValue("onCompression", m_onCompression); // offset: 92/124 size: 1/1 align: 1/1
    deserializer.ReadValue("onExtension", m_onExtension);     // offset: 93/125 size: 1/1 align: 1/1
    deserializer.Skip(2);                                     // offset: 94/126 size: 2/2
    // class size: 96/128 align: 16/16
}
