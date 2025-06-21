#include "Havok/hkbProxyModifierProxyInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbProxyModifierProxyInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbProxyModifierProxyInfo::Class; });

nemesis::hkbProxyModifierProxyInfo::hkbProxyModifierProxyInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbProxyModifierProxyInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbProxyModifierProxyInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("dynamicFriction", m_dynamicFriction);           // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("staticFriction", m_staticFriction);             // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("keepContactTolerance", m_keepContactTolerance); // offset: 8/8 size: 4/4 align: 4/4
    serializer.Skip(4);                                                    // offset: 12/12 size: 4/4

    serializer.WriteValue("up", m_up);                     // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("keepDistance", m_keepDistance); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("contactAngleSensitivity",
                          m_contactAngleSensitivity);  // offset: 36/36 size: 4/4 align: 4/4
    serializer.WriteValue("userPlanes", m_userPlanes); // offset: 40/40 size: 4/4 align: 4/4
    serializer.WriteValue("maxCharacterSpeedForSolver",
                          m_maxCharacterSpeedForSolver);             // offset: 44/44 size: 4/4 align: 4/4
    serializer.WriteValue("characterStrength", m_characterStrength); // offset: 48/48 size: 4/4 align: 4/4
    serializer.WriteValue("characterMass", m_characterMass);         // offset: 52/52 size: 4/4 align: 4/4
    serializer.WriteValue("maxSlope", m_maxSlope);                   // offset: 56/56 size: 4/4 align: 4/4
    serializer.WriteValue("penetrationRecoverySpeed",
                          m_penetrationRecoverySpeed);               // offset: 60/60 size: 4/4 align: 4/4
    serializer.WriteValue("maxCastIterations", m_maxCastIterations); // offset: 64/64 size: 4/4 align: 4/4
    serializer.WriteValue("refreshManifoldInCheckSupport",
                          m_refreshManifoldInCheckSupport); // offset: 68/68 size: 1/1 align: 1/1
    serializer.Skip(11);                                    // offset: 69/69 size: 11/11
    // class size: 80/80 align: 16/16
}

void nemesis::hkbProxyModifierProxyInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("dynamicFriction", m_dynamicFriction); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("staticFriction", m_staticFriction);   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("keepContactTolerance",
                           m_keepContactTolerance); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.Skip(4);                           // offset: 12/12 size: 4/4

    deserializer.ReadValue("up", m_up);                     // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("keepDistance", m_keepDistance); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("contactAngleSensitivity",
                           m_contactAngleSensitivity);  // offset: 36/36 size: 4/4 align: 4/4
    deserializer.ReadValue("userPlanes", m_userPlanes); // offset: 40/40 size: 4/4 align: 4/4
    deserializer.ReadValue("maxCharacterSpeedForSolver",
                           m_maxCharacterSpeedForSolver);             // offset: 44/44 size: 4/4 align: 4/4
    deserializer.ReadValue("characterStrength", m_characterStrength); // offset: 48/48 size: 4/4 align: 4/4
    deserializer.ReadValue("characterMass", m_characterMass);         // offset: 52/52 size: 4/4 align: 4/4
    deserializer.ReadValue("maxSlope", m_maxSlope);                   // offset: 56/56 size: 4/4 align: 4/4
    deserializer.ReadValue("penetrationRecoverySpeed",
                           m_penetrationRecoverySpeed);               // offset: 60/60 size: 4/4 align: 4/4
    deserializer.ReadValue("maxCastIterations", m_maxCastIterations); // offset: 64/64 size: 4/4 align: 4/4
    deserializer.ReadValue("refreshManifoldInCheckSupport",
                           m_refreshManifoldInCheckSupport); // offset: 68/68 size: 1/1 align: 1/1
    deserializer.Skip(11);                                   // offset: 69/69 size: 11/11
    // class size: 80/80 align: 16/16
}
