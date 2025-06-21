#include "Havok/BSDecomposeVectorModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSDecomposeVectorModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSDecomposeVectorModifier::Class; });

nemesis::BSDecomposeVectorModifier::BSDecomposeVectorModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSDecomposeVectorModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSDecomposeVectorModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("vector", m_vector); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("x", m_x);           // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("y", m_y);           // offset: 68/100 size: 4/4 align: 4/4
    serializer.WriteValue("z", m_z);           // offset: 72/104 size: 4/4 align: 4/4
    serializer.WriteValue("w", m_w);           // offset: 76/108 size: 4/4 align: 4/4
    // class size: 80/112 align: 16/16
}

void nemesis::BSDecomposeVectorModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("vector", m_vector); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("x", m_x);           // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("y", m_y);           // offset: 68/100 size: 4/4 align: 4/4
    deserializer.ReadValue("z", m_z);           // offset: 72/104 size: 4/4 align: 4/4
    deserializer.ReadValue("w", m_w);           // offset: 76/108 size: 4/4 align: 4/4
    // class size: 80/112 align: 16/16
}
