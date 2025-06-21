#include "Havok/hkpLimitedHingeConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLimitedHingeConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLimitedHingeConstraintDataAtoms::Class; });

nemesis::hkpLimitedHingeConstraintDataAtoms::hkpLimitedHingeConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpLimitedHingeConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLimitedHingeConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("setupStabilization",
                               m_setupStabilization); // offset: 144/144 size: 16/16 align: 4/4
    }

    serializer.WriteObject("angMotor", m_angMotor);       // offset: 160/160 size: 20/24 align: 4/8
    serializer.WriteObject("angFriction", m_angFriction); // offset: 180/184 size: 12/12 align: 4/4
    serializer.WriteObject("angLimit", m_angLimit);       // offset: 192/196 size: 16/16 align: 4/4
    serializer.WriteObject("2dAng", m_2dAng);             // offset: 208/212 size: 4/4 align: 2/2
    serializer.WriteObject("ballSocket", m_ballSocket);   // offset: 212/216 size: 16/16 align: 4/4
    serializer.Pad(16);                                   // offset: 228/232 size: 12/8
    // class size: 240/240 align: 16/16
}

void nemesis::hkpLimitedHingeConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("setupStabilization",
                                m_setupStabilization); // offset: 144/144 size: 16/16 align: 4/4
    }

    deserializer.ReadObject("angMotor", m_angMotor);       // offset: 160/160 size: 20/24 align: 4/8
    deserializer.ReadObject("angFriction", m_angFriction); // offset: 180/184 size: 12/12 align: 4/4
    deserializer.ReadObject("angLimit", m_angLimit);       // offset: 192/196 size: 16/16 align: 4/4
    deserializer.ReadObject("2dAng", m_2dAng);             // offset: 208/212 size: 4/4 align: 2/2
    deserializer.ReadObject("ballSocket", m_ballSocket);   // offset: 212/216 size: 16/16 align: 4/4
    deserializer.Pad(16);                                  // offset: 228/232 size: 12/8
    // class size: 240/240 align: 16/16
}
