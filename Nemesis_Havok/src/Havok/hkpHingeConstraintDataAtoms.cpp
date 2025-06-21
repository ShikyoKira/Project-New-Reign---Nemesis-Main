#include "Havok/hkpHingeConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpHingeConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpHingeConstraintDataAtoms::Class; });

nemesis::hkpHingeConstraintDataAtoms::hkpHingeConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpHingeConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpHingeConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("setupStabilization",
                               m_setupStabilization); // offset: 144/144 size: 16/16 align: 4/4
    }

    serializer.WriteObject("2dAng", m_2dAng);           // offset: 160/160 size: 4/4 align: 2/2
    serializer.WriteObject("ballSocket", m_ballSocket); // offset: 164/164 size: 16/16 align: 4/4
    serializer.Pad(16);                                 // offset: 180/180 size: 12/12
    // class size: 192/192 align: 16/16
}

void nemesis::hkpHingeConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("setupStabilization",
                                m_setupStabilization); // offset: 144/144 size: 16/16 align: 4/4
    }

    deserializer.ReadObject("2dAng", m_2dAng);           // offset: 160/160 size: 4/4 align: 2/2
    deserializer.ReadObject("ballSocket", m_ballSocket); // offset: 164/164 size: 16/16 align: 4/4
    deserializer.Pad(16);                                // offset: 180/180 size: 12/12
    // class size: 192/192 align: 16/16
}
