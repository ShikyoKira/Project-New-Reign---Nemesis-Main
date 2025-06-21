#include "Havok/hkbTransformVectorModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTransformVectorModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTransformVectorModifier::Class; });

nemesis::hkbTransformVectorModifier::hkbTransformVectorModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbTransformVectorModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTransformVectorModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("rotation", m_rotation);       // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("translation", m_translation); // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("vectorIn", m_vectorIn);       // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteValue("vectorOut", m_vectorOut);     // offset: 96/128 size: 16/16 align: 16/16
    serializer.WriteValue("rotateOnly", m_rotateOnly);   // offset: 112/144 size: 1/1 align: 1/1
    serializer.WriteValue("inverse", m_inverse);         // offset: 113/145 size: 1/1 align: 1/1
    serializer.WriteValue("computeOnActivate", m_computeOnActivate); // offset: 114/146 size: 1/1 align: 1/1
    serializer.WriteValue("computeOnModify", m_computeOnModify);     // offset: 115/147 size: 1/1 align: 1/1
    serializer.Skip(12);                                             // offset: 116/148 size: 12/12
    // class size: 128/160 align: 16/16
}

void nemesis::hkbTransformVectorModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("rotation", m_rotation);       // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("translation", m_translation); // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("vectorIn", m_vectorIn);       // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("vectorOut", m_vectorOut);     // offset: 96/128 size: 16/16 align: 16/16
    deserializer.ReadValue("rotateOnly", m_rotateOnly);   // offset: 112/144 size: 1/1 align: 1/1
    deserializer.ReadValue("inverse", m_inverse);         // offset: 113/145 size: 1/1 align: 1/1
    deserializer.ReadValue("computeOnActivate", m_computeOnActivate); // offset: 114/146 size: 1/1 align: 1/1
    deserializer.ReadValue("computeOnModify", m_computeOnModify);     // offset: 115/147 size: 1/1 align: 1/1
    deserializer.Skip(12);                                            // offset: 116/148 size: 12/12
    // class size: 128/160 align: 16/16
}
