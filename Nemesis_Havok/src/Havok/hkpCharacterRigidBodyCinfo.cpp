#include "Havok/hkpCharacterRigidBodyCinfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCharacterRigidBodyCinfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCharacterRigidBodyCinfo::Class; });

nemesis::hkpCharacterRigidBodyCinfo::hkpCharacterRigidBodyCinfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCharacterControllerCinfo(Class)
{
}

const nemesis::hkClass* nemesis::hkpCharacterRigidBodyCinfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCharacterRigidBodyCinfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCharacterControllerCinfo::SerializeTo(serializer);       // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                         // offset: 12/20 size: 0/4

    serializer.WriteObject("shape", m_shape);                        // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("position", m_position);                   // offset: 16/32 size: 16/16 align: 16/16
    serializer.WriteValue("rotation", m_rotation);                   // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("mass", m_mass);                           // offset: 48/64 size: 4/4 align: 4/4
    serializer.WriteValue("friction", m_friction);                   // offset: 52/68 size: 4/4 align: 4/4
    serializer.WriteValue("maxLinearVelocity", m_maxLinearVelocity); // offset: 56/72 size: 4/4 align: 4/4
    serializer.WriteValue("allowedPenetrationDepth",
                          m_allowedPenetrationDepth); // offset: 60/76 size: 4/4 align: 4/4
    serializer.WriteValue("up", m_up);                // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteValue("maxSlope", m_maxSlope);    // offset: 80/96 size: 4/4 align: 4/4
    serializer.WriteValue("maxForce", m_maxForce);    // offset: 84/100 size: 4/4 align: 4/4
    serializer.WriteValue("unweldingHeightOffsetFactor",
                          m_unweldingHeightOffsetFactor); // offset: 88/104 size: 4/4 align: 4/4
    serializer.WriteValue("maxSpeedForSimplexSolver",
                          m_maxSpeedForSimplexSolver);           // offset: 92/108 size: 4/4 align: 4/4
    serializer.WriteValue("supportDistance", m_supportDistance); // offset: 96/112 size: 4/4 align: 4/4
    serializer.WriteValue("hardSupportDistance",
                          m_hardSupportDistance);  // offset: 100/116 size: 4/4 align: 4/4
    serializer.WriteValue("vdbColor", m_vdbColor); // offset: 104/120 size: 4/4 align: 4/4
    serializer.Skip(4);                            // offset: 108/124 size: 4/4
    // class size: 112/128 align: 16/16
}

void nemesis::hkpCharacterRigidBodyCinfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCharacterControllerCinfo::DeserializeFrom(deserializer);  // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                      // offset: 12/20 size: 0/4

    deserializer.ReadObject("shape", m_shape);      // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("position", m_position); // offset: 16/32 size: 16/16 align: 16/16
    deserializer.ReadValue("rotation", m_rotation); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("mass", m_mass);         // offset: 48/64 size: 4/4 align: 4/4
    deserializer.ReadValue("friction", m_friction); // offset: 52/68 size: 4/4 align: 4/4
    deserializer.ReadValue("maxLinearVelocity", m_maxLinearVelocity); // offset: 56/72 size: 4/4 align: 4/4
    deserializer.ReadValue("allowedPenetrationDepth",
                           m_allowedPenetrationDepth); // offset: 60/76 size: 4/4 align: 4/4
    deserializer.ReadValue("up", m_up);                // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadValue("maxSlope", m_maxSlope);    // offset: 80/96 size: 4/4 align: 4/4
    deserializer.ReadValue("maxForce", m_maxForce);    // offset: 84/100 size: 4/4 align: 4/4
    deserializer.ReadValue("unweldingHeightOffsetFactor",
                           m_unweldingHeightOffsetFactor); // offset: 88/104 size: 4/4 align: 4/4
    deserializer.ReadValue("maxSpeedForSimplexSolver",
                           m_maxSpeedForSimplexSolver);           // offset: 92/108 size: 4/4 align: 4/4
    deserializer.ReadValue("supportDistance", m_supportDistance); // offset: 96/112 size: 4/4 align: 4/4
    deserializer.ReadValue("hardSupportDistance",
                           m_hardSupportDistance);  // offset: 100/116 size: 4/4 align: 4/4
    deserializer.ReadValue("vdbColor", m_vdbColor); // offset: 104/120 size: 4/4 align: 4/4
    deserializer.Skip(4);                           // offset: 108/124 size: 4/4
    // class size: 112/128 align: 16/16
}
