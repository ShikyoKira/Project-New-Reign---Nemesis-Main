#include "Havok/BSIsActiveModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSIsActiveModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSIsActiveModifier::Class; });

nemesis::BSIsActiveModifier::BSIsActiveModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSIsActiveModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSIsActiveModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);             // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("bIsActive0", m_bIsActive0);         // offset: 44/80 size: 1/1 align: 1/1
    serializer.WriteValue("bInvertActive0", m_bInvertActive0); // offset: 45/81 size: 1/1 align: 1/1
    serializer.WriteValue("bIsActive1", m_bIsActive1);         // offset: 46/82 size: 1/1 align: 1/1
    serializer.WriteValue("bInvertActive1", m_bInvertActive1); // offset: 47/83 size: 1/1 align: 1/1
    serializer.WriteValue("bIsActive2", m_bIsActive2);         // offset: 48/84 size: 1/1 align: 1/1
    serializer.WriteValue("bInvertActive2", m_bInvertActive2); // offset: 49/85 size: 1/1 align: 1/1
    serializer.WriteValue("bIsActive3", m_bIsActive3);         // offset: 50/86 size: 1/1 align: 1/1
    serializer.WriteValue("bInvertActive3", m_bInvertActive3); // offset: 51/87 size: 1/1 align: 1/1
    serializer.WriteValue("bIsActive4", m_bIsActive4);         // offset: 52/88 size: 1/1 align: 1/1
    serializer.WriteValue("bInvertActive4", m_bInvertActive4); // offset: 53/89 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 54/90 size: 2/6
    // class size: 56/96 align: 4/8
}

void nemesis::BSIsActiveModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);        // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("bIsActive0", m_bIsActive0);         // offset: 44/80 size: 1/1 align: 1/1
    deserializer.ReadValue("bInvertActive0", m_bInvertActive0); // offset: 45/81 size: 1/1 align: 1/1
    deserializer.ReadValue("bIsActive1", m_bIsActive1);         // offset: 46/82 size: 1/1 align: 1/1
    deserializer.ReadValue("bInvertActive1", m_bInvertActive1); // offset: 47/83 size: 1/1 align: 1/1
    deserializer.ReadValue("bIsActive2", m_bIsActive2);         // offset: 48/84 size: 1/1 align: 1/1
    deserializer.ReadValue("bInvertActive2", m_bInvertActive2); // offset: 49/85 size: 1/1 align: 1/1
    deserializer.ReadValue("bIsActive3", m_bIsActive3);         // offset: 50/86 size: 1/1 align: 1/1
    deserializer.ReadValue("bInvertActive3", m_bInvertActive3); // offset: 51/87 size: 1/1 align: 1/1
    deserializer.ReadValue("bIsActive4", m_bIsActive4);         // offset: 52/88 size: 1/1 align: 1/1
    deserializer.ReadValue("bInvertActive4", m_bInvertActive4); // offset: 53/89 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 54/90 size: 2/6
    // class size: 56/96 align: 4/8
}
