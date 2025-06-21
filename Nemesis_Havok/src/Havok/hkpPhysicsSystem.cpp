#include "Havok/hkpPhysicsSystem.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPhysicsSystem,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPhysicsSystem::Class; });

nemesis::hkpPhysicsSystem::hkpPhysicsSystem() noexcept
    : nemesis::hkpPhysicsSystem(Class)
{
}

nemesis::hkpPhysicsSystem::hkpPhysicsSystem(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkpPhysicsSystem::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPhysicsSystem::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("rigidBodies", m_rigidBodies); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("constraints", m_constraints); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("actions", m_actions);         // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("phantoms", m_phantoms);       // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 56/80 size: 4/8 align: 4/8
    serializer.WriteValue("userData", m_userData);        // offset: 60/88 size: 4/8 align: 4/8
    serializer.WriteValue("active", m_active);            // offset: 64/96 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());          // offset: 65/97 size: 3/7
    // class size: 68/104 align: 4/8
}

void nemesis::hkpPhysicsSystem::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("rigidBodies", m_rigidBodies);      // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("constraints", m_constraints);      // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("actions", m_actions);              // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("phantoms", m_phantoms);            // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 56/80 size: 4/8 align: 4/8
    deserializer.ReadValue("userData", m_userData);             // offset: 60/88 size: 4/8 align: 4/8
    deserializer.ReadValue("active", m_active);                 // offset: 64/96 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 65/97 size: 3/7
    // class size: 68/104 align: 4/8
}
