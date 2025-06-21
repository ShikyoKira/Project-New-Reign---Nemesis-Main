#include "Havok/hkbCharacterControllerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterControllerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterControllerModifier::Class; });

nemesis::hkbCharacterControllerModifier::hkbCharacterControllerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterControllerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterControllerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.Pad(16);                                     // offset: 44/80 size: 4/0
        serializer.WriteObject("controlData", m_controlData_0); // offset: 48/80 size: 32/32 align: 16/16
    }
    else
    {
        serializer.WriteObject("controlData", m_controlData_1);
        serializer.Pad(16);
    }

    serializer.WriteValue("initialVelocity", m_initialVelocity); // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteValue("initialVelocityCoordinates",
                          m_initialVelocityCoordinates); // offset: 96/128 size: 1/1 align: 1/1
    serializer.WriteValue("motionMode", m_motionMode);   // offset: 97/129 size: 1/1 align: 1/1
    serializer.WriteValue("forceDownwardMomentum",
                          m_forceDownwardMomentum);                    // offset: 98/130 size: 1/1 align: 1/1
    serializer.WriteValue("applyGravity", m_applyGravity);             // offset: 99/131 size: 1/1 align: 1/1
    serializer.WriteValue("setInitialVelocity", m_setInitialVelocity); // offset: 100/132 size: 1/1 align: 1/1
    serializer.WriteValue("isTouchingGround", m_isTouchingGround);     // offset: 101/133 size: 1/1 align: 1/1
    serializer.Skip(10);                                               // offset: 102/134 size: 10/10

    serializer.WriteSerializeIgnoredValue("gravity", m_gravity);   // offset: 112/144 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("timestep", m_timestep); // offset: 128/160 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("isInitialVelocityAdded",
                                          m_isInitialVelocityAdded); // offset: 132/164 size: 1/1 align: 1/1
    serializer.Skip(11);                                             // offset: 133/165 size: 11/11
    // class size: 144/176 align: 16/16
}

void nemesis::hkbCharacterControllerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.Pad(16);                                    // offset: 44/80 size: 4/0
        deserializer.ReadObject("controlData", m_controlData_0); // offset: 48/80 size: 32/32 align: 16/16
        m_controlData_1.CopyFrom(m_controlData_0);
    }
    else
    {
        deserializer.ReadObject("controlData", m_controlData_1);
        m_controlData_0.CopyFrom(m_controlData_1);
        deserializer.Pad(16);
    }

    deserializer.ReadValue("initialVelocity", m_initialVelocity); // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("initialVelocityCoordinates",
                           m_initialVelocityCoordinates); // offset: 96/128 size: 1/1 align: 1/1
    deserializer.ReadValue("motionMode", m_motionMode);   // offset: 97/129 size: 1/1 align: 1/1
    deserializer.ReadValue("forceDownwardMomentum",
                           m_forceDownwardMomentum);        // offset: 98/130 size: 1/1 align: 1/1
    deserializer.ReadValue("applyGravity", m_applyGravity); // offset: 99/131 size: 1/1 align: 1/1
    deserializer.ReadValue("setInitialVelocity",
                           m_setInitialVelocity);                   // offset: 100/132 size: 1/1 align: 1/1
    deserializer.ReadValue("isTouchingGround", m_isTouchingGround); // offset: 101/133 size: 1/1 align: 1/1
    deserializer.Skip(10);                                          // offset: 102/134 size: 10/10

    deserializer.ReadValue("gravity", m_gravity);   // offset: 112/144 size: 16/16 align: 16/16
    deserializer.ReadValue("timestep", m_timestep); // offset: 128/160 size: 4/4 align: 4/4
    deserializer.ReadValue("isInitialVelocityAdded",
                           m_isInitialVelocityAdded); // offset: 132/164 size: 1/1 align: 1/1
    deserializer.Skip(11);                            // offset: 133/165 size: 11/11
    // class size: 144/176 align: 16/16
}
