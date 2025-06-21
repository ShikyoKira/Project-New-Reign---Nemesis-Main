#include "Havok/BSRagdollContactListenerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSRagdollContactListenerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSRagdollContactListenerModifier::Class; });

nemesis::BSRagdollContactListenerModifier::BSRagdollContactListenerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSRagdollContactListenerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSRagdollContactListenerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                     // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteUSize(0);                                          // offset: 44/80 size: 4/8 align: 4/8
    serializer.WriteObject("contactEvent", m_contactEvent);            // offset: 48/88 size: 8/16 align: 4/8
    serializer.WriteObject("bones", m_bones);                          // offset: 56/104 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("throwEvent", m_throwEvent); // offset: 60/112 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                       // offset: 61/113 size: 3/7

    serializer.WriteSerializeIgnoredObject("ragdollRigidBodies",
                                           m_ragdollRigidBodies); // offset: 64/120 size: 12/16 align: 4/8
    // class size: 76/136 align: 4/8
}

void nemesis::BSRagdollContactListenerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);     // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadUSize();                                // offset: 44/80 size: 4/8 align: 4/8
    deserializer.ReadObject("contactEvent", m_contactEvent); // offset: 48/88 size: 8/16 align: 4/8
    deserializer.ReadObject("bones", m_bones);               // offset: 56/104 size: 4/8 align: 4/8
    deserializer.ReadValue("throwEvent", m_throwEvent);      // offset: 60/112 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());         // offset: 61/113 size: 3/7

    deserializer.ReadObject("ragdollRigidBodies",
                            m_ragdollRigidBodies); // offset: 64/120 size: 12/16 align: 4/8
    // class size: 76/136 align: 4/8
}
