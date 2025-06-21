#include "Havok/Base/hkpUnaryAction.h"

nemesis::hkpUnaryAction::hkpUnaryAction(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpAction(cls)
{
}

void nemesis::hkpUnaryAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpAction::SerializeTo(serializer); // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteObject("entity", m_entity);  // offset: 24/48 size: 4/8 align: 4/8
    // class size: 28/56 align: 4/8
}

void nemesis::hkpUnaryAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpAction::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadObject("entity", m_entity);       // offset: 24/48 size: 4/8 align: 4/8
    // class size: 28/56 align: 4/8
}
