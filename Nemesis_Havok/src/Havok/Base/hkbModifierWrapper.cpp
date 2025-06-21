#include "Havok/Base/hkbModifierWrapper.h"

nemesis::hkbModifierWrapper::hkbModifierWrapper(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbModifier(cls)
{
}

void nemesis::hkbModifierWrapper::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);  // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("modifier", m_modifier); // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}

void nemesis::hkbModifierWrapper::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("modifier", m_modifier);     // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}
