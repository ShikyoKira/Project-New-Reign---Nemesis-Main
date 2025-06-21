#include "Havok/hkpRagdollConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRagdollConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRagdollConstraintDataAtoms::Class; });

nemesis::hkpRagdollConstraintDataAtoms::hkpRagdollConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpRagdollConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRagdollConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("setupStabilization",
                               m_setupStabilization); // offset: 144/144 size: 16/16 align: 4/4
    }

    serializer.WriteObject("ragdollMotors", m_ragdollMotors); // offset: 160/160 size: 80/96 align: 16/16
    serializer.WriteObject("angFriction", m_angFriction);     // offset: 240/256 size: 12/12 align: 4/4
    serializer.WriteObject("twistLimit", m_twistLimit);       // offset: 252/268 size: 20/20 align: 4/4
    serializer.WriteObject("coneLimit", m_coneLimit);         // offset: 272/288 size: 20/20 align: 4/4
    serializer.WriteObject("planesLimit", m_planesLimit);     // offset: 292/308 size: 20/20 align: 4/4
    serializer.WriteObject("ballSocket", m_ballSocket);       // offset: 312/328 size: 16/16 align: 4/4
    serializer.Skip(8);                                       // offset: 328/344 size: 8/8
    // class size: 336/352 align: 16/16
}

void nemesis::hkpRagdollConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("setupStabilization",
                                m_setupStabilization); // offset: 144/144 size: 16/16 align: 4/4
    }

    deserializer.ReadObject("ragdollMotors", m_ragdollMotors); // offset: 160/160 size: 80/96 align: 16/16
    deserializer.ReadObject("angFriction", m_angFriction);     // offset: 240/256 size: 12/12 align: 4/4
    deserializer.ReadObject("twistLimit", m_twistLimit);       // offset: 252/268 size: 20/20 align: 4/4
    deserializer.ReadObject("coneLimit", m_coneLimit);         // offset: 272/288 size: 20/20 align: 4/4
    deserializer.ReadObject("planesLimit", m_planesLimit);     // offset: 292/308 size: 20/20 align: 4/4
    deserializer.ReadObject("ballSocket", m_ballSocket);       // offset: 312/328 size: 16/16 align: 4/4
    deserializer.Skip(8);                                      // offset: 328/344 size: 8/8
    // class size: 336/352 align: 16/16
}
