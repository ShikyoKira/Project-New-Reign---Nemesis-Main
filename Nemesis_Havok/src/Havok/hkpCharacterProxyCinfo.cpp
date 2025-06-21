#include "Havok/hkpCharacterProxyCinfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCharacterProxyCinfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCharacterProxyCinfo::Class; });

nemesis::hkpCharacterProxyCinfo::hkpCharacterProxyCinfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCharacterControllerCinfo(Class)
{
}

const nemesis::hkClass* nemesis::hkpCharacterProxyCinfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCharacterProxyCinfo::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        nemesis::hkpCharacterControllerCinfo::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
        serializer.Pad(16);                                            // offset: 8/16 size: 8/0
    }

    serializer.WriteValue("position", m_position);               // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("velocity", m_velocity);               // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("dynamicFriction", m_dynamicFriction); // offset: 48/48 size: 4/4 align: 4/4
    serializer.WriteValue("staticFriction", m_staticFriction);   // offset: 52/52 size: 4/4 align: 4/4
    serializer.WriteValue("keepContactTolerance",
                          m_keepContactTolerance); // offset: 56/56 size: 4/4 align: 4/4
    serializer.Skip(4);                            // offset: 60/60 size: 4/4

    serializer.WriteValue("up", m_up); // offset: 64/64 size: 16/16 align: 16/16
    serializer.WriteValue("extraUpStaticFriction",
                          m_extraUpStaticFriction); // offset: 80/80 size: 4/4 align: 4/4
    serializer.WriteValue("extraDownStaticFriction",
                          m_extraDownStaticFriction);       // offset: 84/84 size: 4/4 align: 4/4
    serializer.WriteObject("shapePhantom", m_shapePhantom); // offset: 88/88 size: 4/8 align: 4/8
    serializer.WriteValue("keepDistance", m_keepDistance);  // offset: 92/96 size: 4/4 align: 4/4
    serializer.WriteValue("contactAngleSensitivity",
                          m_contactAngleSensitivity);  // offset: 96/100 size: 4/4 align: 4/4
    serializer.WriteValue("userPlanes", m_userPlanes); // offset: 100/104 size: 4/4 align: 4/4
    serializer.WriteValue("maxCharacterSpeedForSolver",
                          m_maxCharacterSpeedForSolver);             // offset: 104/108 size: 4/4 align: 4/4
    serializer.WriteValue("characterStrength", m_characterStrength); // offset: 108/112 size: 4/4 align: 4/4
    serializer.WriteValue("characterMass", m_characterMass);         // offset: 112/116 size: 4/4 align: 4/4
    serializer.WriteValue("maxSlope", m_maxSlope);                   // offset: 116/120 size: 4/4 align: 4/4
    serializer.WriteValue("penetrationRecoverySpeed",
                          m_penetrationRecoverySpeed);               // offset: 120/124 size: 4/4 align: 4/4
    serializer.WriteValue("maxCastIterations", m_maxCastIterations); // offset: 124/128 size: 4/4 align: 4/4
    serializer.WriteValue("refreshManifoldInCheckSupport",
                          m_refreshManifoldInCheckSupport); // offset: 128/132 size: 1/1 align: 1/1
    serializer.Pad(16);                                     // offset: 129/133 size: 15/11
    // class size: 144/144 align: 16/16
}

void nemesis::hkpCharacterProxyCinfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        nemesis::hkpCharacterControllerCinfo::DeserializeFrom(
            deserializer);    // offset: 0/0 size: 8/16 align: 4/8
        deserializer.Pad(16); // offset: 8/16 size: 8/0
    }

    deserializer.ReadValue("position", m_position);               // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("velocity", m_velocity);               // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("dynamicFriction", m_dynamicFriction); // offset: 48/48 size: 4/4 align: 4/4
    deserializer.ReadValue("staticFriction", m_staticFriction);   // offset: 52/52 size: 4/4 align: 4/4
    deserializer.ReadValue("keepContactTolerance",
                           m_keepContactTolerance); // offset: 56/56 size: 4/4 align: 4/4
    deserializer.Skip(4);                           // offset: 60/60 size: 4/4

    deserializer.ReadValue("up", m_up); // offset: 64/64 size: 16/16 align: 16/16
    deserializer.ReadValue("extraUpStaticFriction",
                           m_extraUpStaticFriction); // offset: 80/80 size: 4/4 align: 4/4
    deserializer.ReadValue("extraDownStaticFriction",
                           m_extraDownStaticFriction);       // offset: 84/84 size: 4/4 align: 4/4
    deserializer.ReadObject("shapePhantom", m_shapePhantom); // offset: 88/88 size: 4/8 align: 4/8
    deserializer.ReadValue("keepDistance", m_keepDistance);  // offset: 92/96 size: 4/4 align: 4/4
    deserializer.ReadValue("contactAngleSensitivity",
                           m_contactAngleSensitivity);  // offset: 96/100 size: 4/4 align: 4/4
    deserializer.ReadValue("userPlanes", m_userPlanes); // offset: 100/104 size: 4/4 align: 4/4
    deserializer.ReadValue("maxCharacterSpeedForSolver",
                           m_maxCharacterSpeedForSolver);             // offset: 104/108 size: 4/4 align: 4/4
    deserializer.ReadValue("characterStrength", m_characterStrength); // offset: 108/112 size: 4/4 align: 4/4
    deserializer.ReadValue("characterMass", m_characterMass);         // offset: 112/116 size: 4/4 align: 4/4
    deserializer.ReadValue("maxSlope", m_maxSlope);                   // offset: 116/120 size: 4/4 align: 4/4
    deserializer.ReadValue("penetrationRecoverySpeed",
                           m_penetrationRecoverySpeed);               // offset: 120/124 size: 4/4 align: 4/4
    deserializer.ReadValue("maxCastIterations", m_maxCastIterations); // offset: 124/128 size: 4/4 align: 4/4
    deserializer.ReadValue("refreshManifoldInCheckSupport",
                           m_refreshManifoldInCheckSupport); // offset: 128/132 size: 1/1 align: 1/1
    deserializer.Pad(16);                                    // offset: 129/133 size: 15/11
    // class size: 144/144 align: 16/16
}
