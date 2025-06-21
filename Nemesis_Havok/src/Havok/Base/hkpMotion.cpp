#include "Havok/Base/hkpMotion.h"

#include "Havok/hkpMaxSizeMotion.h"

nemesis::hkpMotion::hkpMotion(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("type", m_type);                // offset: 8/16 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationIntegrateCounter",
                          m_deactivationIntegrateCounter); // offset: 9/17 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationNumInactiveFrames",
                          m_deactivationNumInactiveFrames); // offset: 10/18 size: 4/4 align: 2/2
    serializer.Pad(16);                                     // offset: 14/22 size: 2/10

    serializer.WriteObject("motionState", m_motionState); // offset: 16/32 size: 176/176 align: 16/16
    serializer.WriteValue("inertiaAndMassInv",
                          m_inertiaAndMassInv);                  // offset: 192/208 size: 16/16 align: 16/16
    serializer.WriteValue("linearVelocity", m_linearVelocity);   // offset: 208/224 size: 16/16 align: 16/16
    serializer.WriteValue("angularVelocity", m_angularVelocity); // offset: 224/240 size: 16/16 align: 16/16
    serializer.WriteValue("deactivationRefPosition",
                          m_deactivationRefPosition); // offset: 240/256 size: 32/32 align: 16/16
    serializer.WriteValue("deactivationRefOrientation",
                          m_deactivationRefOrientation);  // offset: 272/288 size: 8/8 align: 4/4
    serializer.WriteObject("savedMotion", m_savedMotion); // offset: 280/296 size: 4/8 align: 4/8
    serializer.WriteValue("savedQualityTypeIndex",
                          m_savedQualityTypeIndex);          // offset: 284/304 size: 2/2 align: 2/2
    serializer.WriteValue("gravityFactor", m_gravityFactor); // offset: 286/306 size: 2/2 align: 2/2
    serializer.Pad(16);                                      // offset: 288/308 size: 0/12
    // class size: 288/320 align: 16/16
}

void nemesis::hkpMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("type", m_type);                     // offset: 8/16 size: 1/1 align: 1/1
    deserializer.ReadValue("deactivationIntegrateCounter",
                           m_deactivationIntegrateCounter); // offset: 9/17 size: 1/1 align: 1/1
    deserializer.ReadValue("deactivationNumInactiveFrames",
                           m_deactivationNumInactiveFrames); // offset: 10/18 size: 4/4 align: 2/2
    deserializer.Pad(16);                                    // offset: 14/22 size: 2/10

    deserializer.ReadObject("motionState", m_motionState); // offset: 16/32 size: 176/176 align: 16/16
    deserializer.ReadValue("inertiaAndMassInv",
                           m_inertiaAndMassInv);                  // offset: 192/208 size: 16/16 align: 16/16
    deserializer.ReadValue("linearVelocity", m_linearVelocity);   // offset: 208/224 size: 16/16 align: 16/16
    deserializer.ReadValue("angularVelocity", m_angularVelocity); // offset: 224/240 size: 16/16 align: 16/16
    deserializer.ReadValue("deactivationRefPosition",
                           m_deactivationRefPosition); // offset: 240/256 size: 32/32 align: 16/16
    deserializer.ReadValue("deactivationRefOrientation",
                           m_deactivationRefOrientation);  // offset: 272/288 size: 8/8 align: 4/4
    deserializer.ReadObject("savedMotion", m_savedMotion); // offset: 280/296 size: 4/8 align: 4/8
    deserializer.ReadValue("savedQualityTypeIndex",
                           m_savedQualityTypeIndex);          // offset: 284/304 size: 2/2 align: 2/2
    deserializer.ReadValue("gravityFactor", m_gravityFactor); // offset: 286/306 size: 2/2 align: 2/2
    deserializer.Pad(16);                                     // offset: 288/308 size: 0/12
    // class size: 288/320 align: 16/16
}
