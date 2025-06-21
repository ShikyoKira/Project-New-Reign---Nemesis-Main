#include "Havok/Base/hkpBinaryAction.h"

nemesis::hkpBinaryAction::hkpBinaryAction(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpAction(cls)
{
}

void nemesis::hkpBinaryAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpAction::SerializeTo(serializer);  // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteObject("entityA", m_entityA); // offset: 24/48 size: 4/8 align: 4/8
    serializer.WriteObject("entityB", m_entityB); // offset: 28/56 size: 4/8 align: 4/8
    // class size: 32/64 align: 4/8
}

void nemesis::hkpBinaryAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpAction::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadObject("entityA", m_entityA);     // offset: 24/48 size: 4/8 align: 4/8
    deserializer.ReadObject("entityB", m_entityB);     // offset: 28/56 size: 4/8 align: 4/8
    // class size: 32/64 align: 4/8
}
