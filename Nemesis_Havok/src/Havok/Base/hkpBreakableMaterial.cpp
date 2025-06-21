#include "Havok/Base/hkpBreakableMaterial.h"

nemesis::hkpBreakableMaterial::hkpBreakableMaterial(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpBreakableMaterial::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);         // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("strength", m_strength);                // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("typeAndFlags", m_typeAndFlags);        // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteObject("extractedMotion", m_extractedMotion); // offset: 16/24 size: 4/8 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkpBreakableMaterial::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);    // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("strength", m_strength);                // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("typeAndFlags", m_typeAndFlags);        // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadObject("extractedMotion", m_extractedMotion); // offset: 16/24 size: 4/8 align: 4/8
    // class size: 20/32 align: 4/8
}
