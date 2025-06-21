#include "Havok/hkMotionState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMotionState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMotionState::Class; });

nemesis::hkMotionState::hkMotionState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkMotionState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMotionState::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("transform", m_transform); // offset: 0/0 size: 64/64 align: 16/16
    serializer.WriteObject("sweptTransform",
                           m_sweptTransform);                  // offset: 64/64 size: 80/80 align: 16/16
    serializer.WriteValue("deltaAngle", m_deltaAngle);         // offset: 144/144 size: 16/16 align: 16/16
    serializer.WriteValue("objectRadius", m_objectRadius);     // offset: 160/160 size: 4/4 align: 4/4
    serializer.WriteValue("linearDamping", m_linearDamping);   // offset: 164/164 size: 2/2 align: 2/2
    serializer.WriteValue("angularDamping", m_angularDamping); // offset: 166/166 size: 2/2 align: 2/2

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("timeFactor", m_timeFactor); // offset: 168/168 size: 2/2 align: 2/2
    }

    serializer.WriteValue("maxLinearVelocity",
                          m_maxLinearVelocity); // offset: 170/170 size: 1/1 align: 1/1
    serializer.WriteValue("maxAngularVelocity",
                          m_maxAngularVelocity);                     // offset: 171/171 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationClass", m_deactivationClass); // offset: 172/172 size: 1/1 align: 1/1
    serializer.Pad(16);                                              // offset: 173/173 size: 3/3
    // class size: 176/176 align: 16/16
}

void nemesis::hkMotionState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("transform", m_transform); // offset: 0/0 size: 64/64 align: 16/16
    deserializer.ReadObject("sweptTransform",
                            m_sweptTransform);                  // offset: 64/64 size: 80/80 align: 16/16
    deserializer.ReadValue("deltaAngle", m_deltaAngle);         // offset: 144/144 size: 16/16 align: 16/16
    deserializer.ReadValue("objectRadius", m_objectRadius);     // offset: 160/160 size: 4/4 align: 4/4
    deserializer.ReadValue("linearDamping", m_linearDamping);   // offset: 164/164 size: 2/2 align: 2/2
    deserializer.ReadValue("angularDamping", m_angularDamping); // offset: 166/166 size: 2/2 align: 2/2

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("timeFactor", m_timeFactor); // offset: 168/168 size: 2/2 align: 2/2
    }

    deserializer.ReadValue("maxLinearVelocity",
                           m_maxLinearVelocity); // offset: 170/170 size: 1/1 align: 1/1
    deserializer.ReadValue("maxAngularVelocity",
                           m_maxAngularVelocity);                     // offset: 171/171 size: 1/1 align: 1/1
    deserializer.ReadValue("deactivationClass", m_deactivationClass); // offset: 172/172 size: 1/1 align: 1/1
    deserializer.Pad(16);                                             // offset: 173/173 size: 3/3
    // class size: 176/176 align: 16/16
}
