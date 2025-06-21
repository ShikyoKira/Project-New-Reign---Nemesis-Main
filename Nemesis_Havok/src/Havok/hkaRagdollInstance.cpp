#include "Havok/hkaRagdollInstance.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaRagdollInstance,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaRagdollInstance::Class; });

nemesis::hkaRagdollInstance::hkaRagdollInstance() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaRagdollInstance::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaRagdollInstance::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("rigidBodies", m_rigidBodies); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("constraints", m_constraints); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("boneToRigidBodyMap",
                           m_boneToRigidBodyMap);   // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("skeleton", m_skeleton); // offset: 44/64 size: 4/8 align: 4/8
    // class size: 48/72 align: 4/8
}

void nemesis::hkaRagdollInstance::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("rigidBodies", m_rigidBodies);      // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("constraints", m_constraints);      // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("boneToRigidBodyMap",
                            m_boneToRigidBodyMap);   // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("skeleton", m_skeleton); // offset: 44/64 size: 4/8 align: 4/8
    // class size: 48/72 align: 4/8
}
