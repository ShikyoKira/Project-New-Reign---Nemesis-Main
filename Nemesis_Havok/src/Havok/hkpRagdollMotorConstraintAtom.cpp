#include "Havok/hkpRagdollMotorConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRagdollMotorConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRagdollMotorConstraintAtom::Class; });

nemesis::hkpRagdollMotorConstraintAtom::hkpRagdollMotorConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpRagdollMotorConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRagdollMotorConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);     // offset: 2/2 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.Skip(13);
    }
    else
    {
        serializer.Skip(1); // offset: 3/3 size: 1/1

        serializer.WriteValue("initializedOffset", m_initializedOffset); // offset: 4/4 size: 2/2 align: 2/2
        serializer.WriteValue("previousTargetAnglesOffset",
                              m_previousTargetAnglesOffset); // offset: 6/6 size: 2/2 align: 2/2
        serializer.Skip(8);                                  // offset: 8/8 size: 8/8
    }

    serializer.WriteValue("target_bRca", m_target_bRca); // offset: 16/16 size: 48/48 align: 16/16
    serializer.WriteObject("motors", m_motors);          // offset: 64/64 size: 12/24 align: 4/8
    serializer.Pad(16);                                  // offset: 76/88 size: 4/8
    // class size: 80/96 align: 16/16
}

void nemesis::hkpRagdollMotorConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);          // offset: 2/2 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.Skip(13);
    }
    else
    {
        deserializer.Skip(1); // offset: 3/3 size: 1/1

        deserializer.ReadValue("initializedOffset", m_initializedOffset); // offset: 4/4 size: 2/2 align: 2/2
        deserializer.ReadValue("previousTargetAnglesOffset",
                               m_previousTargetAnglesOffset); // offset: 6/6 size: 2/2 align: 2/2
        deserializer.Skip(8);                                 // offset: 8/8 size: 8/8
    }

    deserializer.ReadValue("target_bRca", m_target_bRca); // offset: 16/16 size: 48/48 align: 16/16
    deserializer.ReadObject("motors", m_motors);          // offset: 64/64 size: 12/24 align: 4/8
    deserializer.Pad(16);                                 // offset: 76/88 size: 4/8
    // class size: 80/96 align: 16/16
}
