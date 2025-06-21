#include "Havok/hkpDisplayBindingDataPhysicsSystem.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDisplayBindingDataPhysicsSystem,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDisplayBindingDataPhysicsSystem::Class; });

nemesis::hkpDisplayBindingDataPhysicsSystem::hkpDisplayBindingDataPhysicsSystem() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpDisplayBindingDataPhysicsSystem::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDisplayBindingDataPhysicsSystem::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("bindings", m_bindings);       // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("system", m_system);           // offset: 20/32 size: 4/8 align: 4/8
    // class size: 24/40 align: 4/8
}

void nemesis::hkpDisplayBindingDataPhysicsSystem::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("bindings", m_bindings);            // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("system", m_system);                // offset: 20/32 size: 4/8 align: 4/8
    // class size: 24/40 align: 4/8
}
